#include "plugin_netlist_simulator/netlist_simulator.h"

#include "core/log.h"
#include "netlist/gate_library/gate_type/gate_type_sequential.h"

#include <algorithm>

template<typename T>
static T toggle(T v)
{
    if (v == 0 || v == 1)
    {
        return static_cast<T>(1 - v);
    }
    return v;
}

namespace hal
{
#define seconds_since(X) ((double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - (X)).count() / 1000)

#define measure_block_time(X) measure_block_time_t _UNIQUE_NAME_(X);

    class measure_block_time_t
    {
    public:
        measure_block_time_t(const std::string& section_name)
        {
            m_name       = section_name;
            m_begin_time = std::chrono::high_resolution_clock::now();
        }

        ~measure_block_time_t()
        {
            log_info("netlist simulator", "{} took {:3.2f}s", m_name, seconds_since(m_begin_time));
        }

    private:
        std::string m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_time;
    };

    // #define measure_block_time(X)

    NetlistSimulator::NetlistSimulator()
    {
        reset();
    }

    void NetlistSimulator::add_gates(const std::vector<Gate*>& gates)
    {
        m_simulation_set.insert(gates.begin(), gates.end());

        compute_input_nets();
        compute_output_nets();

        m_needs_initialization = true;
    }

    void NetlistSimulator::add_clock_hertz(Net* clock_net, u64 hertz, bool start_at_zero)
    {
        u64 period = 1'000'000'000'000ul / hertz;
        add_clock_period(clock_net, period, start_at_zero);
    }

    void NetlistSimulator::add_clock_period(Net* clock_net, u64 period, bool start_at_zero)
    {
        Clock c;
        c.clock_net     = clock_net;
        c.switch_time   = period / 2;
        c.start_at_zero = start_at_zero;
        m_clocks.push_back(c);
    }

    std::unordered_set<Gate*> NetlistSimulator::get_gates() const
    {
        return m_simulation_set;
    }

    std::vector<Net*> NetlistSimulator::get_input_nets() const
    {
        return m_input_nets;
    }

    std::vector<Net*> NetlistSimulator::get_output_nets() const
    {
        return m_output_nets;
    }

    void NetlistSimulator::set_input(Net* net, SignalValue value)
    {
        if (auto it = m_simulation.m_events.find(net); it != m_simulation.m_events.end())
        {
            if (value == it->second.back().new_value)
            {
                return;
            }
        }

        Event e;
        e.affected_net = net;
        e.time         = m_current_time;
        e.new_value    = value;
        m_event_queue.push_back(e);
    }

    void NetlistSimulator::load_initial_values()
    {
        // has to work even if the simulation was not started, i.e., initialize was not called yet
        // so we cannot use the SimulationGateFF type

        for (auto gate : m_simulation_set)
        {
            if (gate->get_type()->get_base_type() == GateType::BaseType::ff)
            {
                // extract init string
                auto gate_type = dynamic_cast<const GateTypeSequential*>(gate->get_type());
                auto init_str  = std::get<1>(gate->get_data_by_key(gate_type->get_init_data_category(), gate_type->get_init_data_identifier()));

                if (!init_str.empty())
                {
                    // parse init string

                    SignalValue value = SignalValue::X;
                    if (init_str == "1")
                    {
                        value = SignalValue::ONE;
                    }
                    else if (init_str == "0")
                    {
                        value = SignalValue::ZERO;
                    }
                    else
                    {
                        log_error("netlist simulator", "init value of sequential gate '{}' (type '{}') is neither '1' or '0'", gate->get_name(), gate_type->get_name());
                    }

                    SignalValue inv_value = toggle(value);

                    // generate events
                    for (const auto& pin : gate_type->get_state_output_pins())
                    {
                        Event e;
                        e.affected_net = gate->get_fan_out_net(pin);
                        e.new_value    = value;
                        e.time         = m_current_time;
                        m_event_queue.push_back(e);
                    }
                    for (const auto& pin : gate_type->get_inverted_state_output_pins())
                    {
                        Event e;
                        e.affected_net = gate->get_fan_out_net(pin);
                        e.new_value    = inv_value;
                        e.time         = m_current_time;
                        m_event_queue.push_back(e);
                    }
                }
            }
        }
    }

    void NetlistSimulator::simulate(u64 picoseconds)
    {
        if (m_needs_initialization)
        {
            initialize();
        }

        prepare_clock_events(picoseconds);

        process_events(m_current_time + picoseconds);
    }

    void NetlistSimulator::reset()
    {
        m_current_time = 0;
        m_id_counter   = 0;
        m_simulation   = Simulation();
        m_event_queue.clear();
        m_needs_initialization = true;
    }

    void NetlistSimulator::set_state(const Simulation& state)
    {
        m_simulation = state;
    }

    Simulation NetlistSimulator::get_current_state() const
    {
        return m_simulation;
    }

    void NetlistSimulator::compute_input_nets()
    {
        m_input_nets.clear();
        for (auto gate : m_simulation_set)
        {
            for (auto net : gate->get_fan_in_nets())
            {
                // "input net" is either a global input...
                if (net->is_global_input_net())
                {
                    m_input_nets.push_back(net);
                }
                else    // ... or has a source outside of the simulation set
                {
                    for (auto src : net->get_sources())
                    {
                        if (m_simulation_set.find(src.get_gate()) == m_simulation_set.end())
                        {
                            m_input_nets.push_back(net);
                            break;
                        }
                    }
                }
            }
        }
    }

    void NetlistSimulator::compute_output_nets()
    {
        m_output_nets.clear();
        for (auto gate : m_simulation_set)
        {
            for (auto net : gate->get_fan_out_nets())
            {
                // "output net" is either a global output...
                if (net->is_global_output_net())
                {
                    m_output_nets.push_back(net);
                }
                else    // ... or has a destination outside of the simulation set
                {
                    for (auto dst : net->get_destinations())
                    {
                        if (m_simulation_set.find(dst.get_gate()) == m_simulation_set.end())
                        {
                            m_output_nets.push_back(net);
                            break;
                        }
                    }
                }
            }
        }
    }

    void NetlistSimulator::set_iteration_timeout(u64 iterations)
    {
        m_timeout_iterations = iterations;
    }

    u64 NetlistSimulator::get_simulation_timeout() const
    {
        return m_timeout_iterations;
    }

    /*
     * This function precomputes all the stuff that shall be cached for simulation.
     */
    void NetlistSimulator::initialize()
    {
        measure_block_time("NetlistSimulator::initialize()");
        m_successors.clear();
        m_sim_gates.clear();

        std::unordered_map<Gate*, SimulationGate*> sim_gates_map;
        std::unordered_set<Net*> all_nets;

        // precompute everything that is gate-related
        for (auto gate : m_simulation_set)
        {
            std::vector<std::string> input_pins;
            std::vector<Net*> input_nets;

            // gather input pins + respective nets at same positions of vectors for fast iteration
            for (auto pin : gate->get_input_pins())
            {
                input_pins.push_back(pin);
                input_nets.push_back(gate->get_fan_in_net(pin));
            }

            SimulationGate* sim_gate_base = nullptr;

            if (gate->get_type()->get_base_type() == GateType::BaseType::ff)
            {
                auto sim_gate_owner = std::make_unique<SimulationGateFF>();
                auto sim_gate       = sim_gate_owner.get();
                sim_gate_base       = sim_gate;
                m_sim_gates.push_back(std::move(sim_gate_owner));
                sim_gate->is_flip_flop = true;
                sim_gate->input_pins   = input_pins;
                sim_gate->input_nets   = input_nets;

                auto gate_type            = static_cast<const GateTypeSequential*>(gate->get_type());
                sim_gate->clock_func      = gate->get_boolean_function("clock");
                sim_gate->preset_func     = gate->get_boolean_function("preset");
                sim_gate->clear_func      = gate->get_boolean_function("clear");
                sim_gate->next_state_func = gate->get_boolean_function("next_state");
                for (auto pin : gate_type->get_state_output_pins())
                {
                    sim_gate->state_output_nets.push_back(gate->get_fan_out_net(pin));
                }
                for (auto pin : gate_type->get_inverted_state_output_pins())
                {
                    sim_gate->state_inverted_output_nets.push_back(gate->get_fan_out_net(pin));
                }
                for (auto pin : gate_type->get_clock_pins())
                {
                    sim_gate->clock_nets.push_back(gate->get_fan_in_net(pin));
                }
                auto behavior                      = gate_type->get_set_reset_behavior();
                sim_gate->sr_behavior_out          = behavior.first;
                sim_gate->sr_behavior_out_inverted = behavior.second;
            }
            else if (gate->get_type()->get_base_type() == GateType::BaseType::combinatorial || gate->get_type()->get_base_type() == GateType::BaseType::lut)
            {
                auto sim_gate_owner = std::make_unique<SimulationGateCombinational>();
                auto sim_gate       = sim_gate_owner.get();
                sim_gate_base       = sim_gate;
                m_sim_gates.push_back(std::move(sim_gate_owner));
                sim_gate->is_flip_flop = false;
                sim_gate->input_pins   = input_pins;
                sim_gate->input_nets   = input_nets;

                auto all_functions = gate->get_boolean_functions();

                sim_gate->output_pins = gate->get_output_pins();

                for (auto pin : sim_gate->output_pins)
                {
                    auto out_net = gate->get_fan_out_net(pin);
                    sim_gate->output_nets.push_back(out_net);

                    auto func = all_functions.at(pin);

                    while (true)
                    {
                        auto vars = func.get_variables();
                        bool exit = true;
                        for (auto other_pin : sim_gate->output_pins)
                        {
                            if (vars.find(other_pin) != vars.end())
                            {
                                func = func.substitute(other_pin, all_functions.at(other_pin));
                                exit = false;
                            }
                        }
                        if (exit)
                        {
                            break;
                        }
                    }

                    sim_gate->functions.emplace(out_net, func);
                }
            }
            else
            {
                log_error("netlist simulator", "no support for gate type {} of gate {}", gate->get_type()->get_name(), gate->get_name());
                m_successors.clear();
                m_sim_gates.clear();
                return;
            }

            sim_gate_base->gate = gate;

            sim_gates_map.emplace(gate, sim_gate_base);

            auto out_nets = gate->get_fan_out_nets();
            all_nets.insert(out_nets.begin(), out_nets.end());
        }

        all_nets.insert(m_input_nets.begin(), m_input_nets.end());

        // find all successors of nets and transform them to their respective simulation gate instance
        for (auto net : all_nets)
        {
            if (auto it = m_successors.find(net); it != m_successors.end())
            {
                continue;
            }
            auto endpoints  = net->get_destinations();
            auto& dst_gates = m_successors[net];
            for (auto ep : endpoints)
            {
                auto gate = ep.get_gate();
                if (m_simulation_set.find(gate) == m_simulation_set.end())
                {
                    continue;
                }
                dst_gates.push_back(sim_gates_map.at(gate));
            }
        }

        // create one-time events for global gnd and vcc gates
        for (auto g : m_simulation_set)
        {
            if (g->is_gnd_gate())
            {
                for (auto n : g->get_fan_out_nets())
                {
                    Event e;
                    e.affected_net = n;
                    e.new_value    = SignalValue::ZERO;
                    e.time         = m_current_time;
                    m_event_queue.push_back(e);
                }
            }
            else if (g->is_vcc_gate())
            {
                for (auto n : g->get_fan_out_nets())
                {
                    Event e;
                    e.affected_net = n;
                    e.new_value    = SignalValue::ONE;
                    e.time         = m_current_time;
                    m_event_queue.push_back(e);
                }
            }
        }

        // set initialization flag only if this point is reached
        m_needs_initialization = false;
    }

    void NetlistSimulator::prepare_clock_events(u64 picoseconds)
    {
        for (auto& c : m_clocks)
        {
            u64 base_time = m_current_time - (m_current_time % c.switch_time);
            u64 time      = 0;

            // determine next signal state
            // works also if we are in the middle of a cycle
            SignalValue v = static_cast<SignalValue>(base_time & 1);
            if (!c.start_at_zero)
            {
                v = toggle(v);
            }

            // insert the required amount of clock signal switch events
            while (time < picoseconds)
            {
                Event e;
                e.affected_net = c.clock_net;
                e.new_value    = v;
                e.time         = base_time + time;
                m_event_queue.push_back(e);

                v = toggle(v);
                time += c.switch_time;
            }
        }
    }

    void NetlistSimulator::process_events(u64 timeout)
    {
        measure_block_time("NetlistSimulator::process_events(" + std::to_string(timeout) + ")");

        // iteration counter to catch infinite loops
        u64 total_iterations_for_one_timeslot = 0;

        // strategy: propagate all signals at the current time
        // THEN evaluate all FFs that were clocked by these signals
        // hence we need to remember FFs that were clocked
        std::vector<SimulationGateFF*> ffs;
        bool ffs_processed = false;

        while (!m_event_queue.empty() || !ffs.empty())
        {
            std::map<std::pair<Net*, u64>, SignalValue> new_events;

            // sort events by time
            std::sort(m_event_queue.begin(), m_event_queue.end());

            // queue empty or all events of the current point in time processed?
            if (m_event_queue.empty() || m_current_time != m_event_queue[0].time)
            {
                // are there FFs that were clocked? process them now!
                if (!ffs.empty() && !ffs_processed)
                {
                    for (auto ff : ffs)
                    {
                        simulate_ff(ff, new_events);
                    }
                    ffs.clear();
                    ffs_processed = true;
                }
                else if (m_event_queue.empty())
                {
                    // no FFs and queue empty -> simulation is done
                    break;
                }
                else    // no FFs but queue is not empty -> advance point in time
                {
                    m_current_time                    = m_event_queue[0].time;
                    total_iterations_for_one_timeslot = 0;
                    ffs_processed                     = false;
                }
            }

            // note: at this point not all events are processed necessarily!
            // but they are processed when simulation is resumed, so no worries
            if (m_current_time > timeout)
            {
                break;
            }

            // process all events of the current point in time
            u32 processed = 0;
            for (; processed < m_event_queue.size() && m_event_queue[processed].time <= m_current_time; ++processed)
            {
                auto& event = m_event_queue[processed];

                // is there already a value recorded for the net?
                if (auto it = m_simulation.m_events.find(event.affected_net); it != m_simulation.m_events.end())
                {
                    // if the event does not change anything, skip it
                    if (it->second.back().new_value == event.new_value)
                    {
                        continue;
                    }
                    // if the event does change something, but there was already an event for this point in time, we simply update the value
                    else if (it->second.back().time == event.time)
                    {
                        it->second.back().new_value = event.new_value;
                        if (it->second.size() > 1 && it->second[it->second.size() - 2].new_value == event.new_value)
                        {
                            it->second.pop_back();
                        }
                    }
                    else    // new event
                    {
                        m_simulation.m_events[event.affected_net].push_back(event);
                    }
                }
                else    // no value recorded -> new event
                {
                    m_simulation.m_events[event.affected_net].push_back(event);
                }

                // simulate affected gates
                // record all FFs that have to be clocked
                for (auto gate : m_successors.at(event.affected_net))
                {
                    if (!simulate_gate(gate, event, new_events))
                    {
                        ffs.push_back(static_cast<SimulationGateFF*>(gate));
                    }
                }
            }

            // check for iteration limit
            total_iterations_for_one_timeslot += processed;
            if (m_timeout_iterations > 0 && total_iterations_for_one_timeslot > m_timeout_iterations)
            {
                log_error("netlist simulator", "reached iteration timeout of {} without advancing in time, aborting simulation. Is there a combinational loop?", m_timeout_iterations);
                return;
            }

            // remove processed events
            m_event_queue.erase(m_event_queue.begin(), m_event_queue.begin() + processed);

            // add new events
            m_event_queue.reserve(m_event_queue.size() + new_events.size());
            for (const auto& event_it : new_events)
            {
                Event e;
                e.affected_net = event_it.first.first;
                e.time         = event_it.first.second;
                e.new_value    = event_it.second;
                e.id           = m_id_counter++;
                m_event_queue.push_back(e);
            }
        }

        // adjust point in time
        m_current_time = timeout;
    }

    bool NetlistSimulator::simulate_gate(SimulationGate* gate, Event& event, std::map<std::pair<Net*, u64>, SignalValue>& new_events)
    {
        // compute delay, currently just a placeholder
        u64 delay = 0;

        // gather inputs
        auto input_values = gather_input_values(gate);

        // compute output for flip flop
        if (gate->is_flip_flop)
        {
            auto ff = static_cast<SimulationGateFF*>(gate);

            // is the event triggering a clock pin?
            // if so remember to process the flipflop later!
            if (std::find(ff->clock_nets.begin(), ff->clock_nets.end(), event.affected_net) != ff->clock_nets.end())
            {
                // return true if the event was completely handled
                // -> true if the gate is NOT clocked at this point
                return (ff->clock_func.evaluate(input_values) != BooleanFunction::ONE);
            }
            else    // not a clock pin -> only check for asynchronous signals
            {
                auto async_set   = ff->preset_func.evaluate(input_values);
                auto async_reset = ff->clear_func.evaluate(input_values);

                // check whether an asynchronous set or reset ist triggered
                if (async_set == BooleanFunction::ONE || async_reset == BooleanFunction::ONE)
                {
                    SignalValue result     = SignalValue::X;
                    SignalValue inv_result = SignalValue::X;

                    if (async_set == BooleanFunction::ONE && async_reset == BooleanFunction::ONE)
                    {
                        // both signals set? -> evaluate special behavior
                        SignalValue old_output = SignalValue::X;
                        if (!ff->state_output_nets.empty())
                        {
                            auto out_net = ff->state_output_nets[0];
                            if (auto it = m_simulation.m_events.find(out_net); it != m_simulation.m_events.end())
                            {
                                old_output = it->second.back().new_value;
                            }
                        }
                        SignalValue old_output_inv = SignalValue::X;
                        if (!ff->state_inverted_output_nets.empty())
                        {
                            auto out_net = ff->state_inverted_output_nets[0];
                            if (auto it = m_simulation.m_events.find(out_net); it != m_simulation.m_events.end())
                            {
                                old_output_inv = it->second.back().new_value;
                            }
                        }
                        result     = process_set_reset_behavior(ff->sr_behavior_out, old_output);
                        inv_result = process_set_reset_behavior(ff->sr_behavior_out_inverted, old_output_inv);
                    }
                    else if (async_set == BooleanFunction::ONE)
                    {
                        // only asynch set is 1
                        result     = SignalValue::ONE;
                        inv_result = toggle(result);
                    }
                    else if (async_reset == BooleanFunction::ONE)
                    {
                        // only asynch reset is 1
                        result     = SignalValue::ZERO;
                        inv_result = toggle(result);
                    }

                    // generate events
                    for (auto out_net : ff->state_output_nets)
                    {
                        new_events[std::make_pair(out_net, m_current_time + delay)] = result;
                    }
                    for (auto out_net : ff->state_inverted_output_nets)
                    {
                        new_events[std::make_pair(out_net, m_current_time + delay)] = inv_result;
                    }
                }
            }
        }
        else    // compute output for combinational gate
        {
            auto comb = static_cast<SimulationGateCombinational*>(gate);
            for (auto out_net : comb->output_nets)
            {
                auto result = comb->functions[out_net](input_values);

                new_events[std::make_pair(out_net, m_current_time + delay)] = static_cast<SignalValue>(result);
            }
        }
        return true;
    }

    void NetlistSimulator::simulate_ff(SimulationGateFF* gate, std::map<std::pair<Net*, u64>, SignalValue>& new_events)
    {
        // compute delay, currently just a placeholder
        u64 delay = 0;

        // gather inputs
        auto input_values = gather_input_values(gate);

        // compute output
        auto result     = static_cast<SignalValue>(gate->next_state_func.evaluate(input_values));
        auto inv_result = toggle(result);

        // generate events
        for (auto out_net : gate->state_output_nets)
        {
            new_events[std::make_pair(out_net, m_current_time + delay)] = result;
        }
        for (auto out_net : gate->state_inverted_output_nets)
        {
            new_events[std::make_pair(out_net, m_current_time + delay)] = inv_result;
        }
    }

    std::map<std::string, BooleanFunction::Value> NetlistSimulator::gather_input_values(SimulationGate* gate)
    {
        std::map<std::string, BooleanFunction::Value> input_values;
        for (u32 i = 0; i < gate->input_pins.size(); ++i)
        {
            auto pin          = gate->input_pins[i];
            auto net          = gate->input_nets[i];
            SignalValue value = SignalValue::X;
            if (auto it = m_simulation.m_events.find(net); it != m_simulation.m_events.end())
            {
                value = it->second.back().new_value;
            }
            input_values.emplace(pin, static_cast<BooleanFunction::Value>(value));
        }
        return input_values;
    }

    SignalValue NetlistSimulator::process_set_reset_behavior(GateTypeSequential::SetResetBehavior behavior, SignalValue previous_output)
    {
        if (behavior == GateTypeSequential::SetResetBehavior::U)
        {
            log_warning("netlist simulator", "undefined simultaneous set/reset behavior encountered");
            return SignalValue::X;
        }
        else if (behavior == GateTypeSequential::SetResetBehavior::N)
        {
            return previous_output;
        }
        else if (behavior == GateTypeSequential::SetResetBehavior::X)
        {
            return SignalValue::X;
        }
        else if (behavior == GateTypeSequential::SetResetBehavior::L)
        {
            return SignalValue::ZERO;
        }
        else if (behavior == GateTypeSequential::SetResetBehavior::H)
        {
            return SignalValue::ONE;
        }
        else if (behavior == GateTypeSequential::SetResetBehavior::T)
        {
            return toggle(previous_output);
        }
        log_error("netlist simulator", "unsupported set/reset behavior {}", behavior);
        return SignalValue::X;
    }

}    // namespace hal
