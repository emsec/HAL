
#include "netlist_test_utils.h"

#include <core/utils.h>
#include <math.h>

//using namespace hal;
//namespace test_utils = hal;
namespace hal
{
    std::unique_ptr<Netlist> test_utils::create_empty_netlist(const int id)
    {
        NO_COUT_BLOCK;
        // GateLibrary* gl = gate_library_manager::get_gate_library(g_lib_name);
        auto nl = std::make_unique<Netlist>(get_testing_gate_library());

        if (id >= 0)
        {
            nl->set_id(id);
        }
        nl->set_device_name("device_name");
        nl->set_design_name("design_name");

        return nl;
    }

    void test_utils::init_log_channels()
    {
        // All channels that are registered:
        static bool already_init = false;
        if (!already_init)
        {
            std::vector<std::string> channel_ids = {
                "core", "gate_library_manager", "liberty_parser", "netlist", "module", "netlist.internal", "netlist.persistent", "hdl_parser", "hdl_writer", "python_context"};

            LogManager& lm = LogManager::get_instance();
            for (std::string ch_id : channel_ids)
            {
                lm.add_channel(ch_id, {LogManager::create_stdout_sink()}, "info");
            }
            already_init = true;
        }
    }

    Endpoint test_utils::get_endpoint(Netlist* nl, const int gate_id, const std::string& pin_type, bool is_destination)
    {
        Gate* g = nl->get_gate_by_id(gate_id);
        if (g != nullptr)
        {
            Net* net = is_destination ? g->get_fan_in_net(pin_type) : g->get_fan_out_net(pin_type);
            return Endpoint(g, pin_type, net, is_destination);
        }
        else
            return Endpoint();
    }

    Endpoint test_utils::get_endpoint(Gate* g, const std::string& pin_type)
    {
        if (g == nullptr || pin_type == "")
        {
            return Endpoint();
        }
        auto nl             = g->get_netlist();
        int gate_id         = g->get_id();
        auto in_pins        = g->get_type()->get_input_pins();
        bool is_destination = (std::find(in_pins.begin(), in_pins.end(), pin_type) != in_pins.end());
        return get_endpoint(nl, gate_id, pin_type, is_destination);
    }

    bool test_utils::is_empty(const Endpoint& ep)
    {
        return ((ep.get_gate() == nullptr) && (ep.get_pin() == ""));
    }

    std::vector<BooleanFunction::Value> test_utils::minimize_truth_table(const std::vector<BooleanFunction::Value> tt)
    {
        int var_amt = round(log2(tt.size()));
        if ((1 << var_amt) != tt.size())
        {
            std::cerr << "[Test] minimize_truth_table: Tablesize must be a power of two!" << std::endl;
            return std::vector<BooleanFunction::Value>();
        }
        for (int v = 0; v < var_amt; v++)
        {
            int interval = 2 << v;
            std::vector<BooleanFunction::Value> v_eq_0;
            std::vector<BooleanFunction::Value> v_eq_1;
            for (int i = 0; i < tt.size(); i++)
            {
                if (i % interval < (interval >> 1))
                {
                    v_eq_0.push_back(tt[i]);
                }
                else
                {
                    v_eq_1.push_back(tt[i]);
                }
            }
            if (v_eq_0 == v_eq_1)
            {
                return minimize_truth_table(v_eq_0);
            }
        }
        return tt;
    }

    const GateType* test_utils::get_gate_type_by_name(std::string name, GateLibrary* gate_lib)
    {
        GateLibrary* gl;
        if (gate_lib == nullptr)
        {
            gl = get_testing_gate_library();
        }
        else
        {
            gl = gate_lib;
        }
        auto names_to_type = gl->get_gate_types();
        // If the Gate type isn't found in the Gate library
        if (names_to_type.find(name) == names_to_type.end())
        {
            std::cerr << "Gate type: \'" << name << "\' can't be found in the Gate library \'" << gl->get_name() << "\'" << std::endl;
            return nullptr;
        }
        else
        {
            return names_to_type.at(name);
        }
    }

    bool test_utils::string_contains_substring(const std::string str, const std::string sub_str)
    {
        return (str.find(sub_str) != std::string::npos);
    }

    Net* test_utils::get_net_by_subname(Netlist* nl, const std::string subname)
    {
        if (nl == nullptr)
            return nullptr;
        std::unordered_set<Net*> nets = nl->get_nets();
        Net* res                      = nullptr;
        for (auto n : nets)
        {
            std::string n_name = n->get_name();
            if (n_name.find(subname) != n_name.npos)
            {
                if (res != nullptr)
                {
                    std::cerr << "Multiple gates contains the subtring '" << subname << "'! This should not happen..." << std::endl;
                    return nullptr;
                }
                res = n;
            }
        }
        return res;
    }

    Gate* test_utils::get_gate_by_subname(Netlist* nl, const std::string subname)
    {
        if (nl == nullptr)
            return nullptr;
        auto gates = nl->get_gates();
        Gate* res  = nullptr;
        for (auto g : gates)
        {
            std::string g_name = g->get_name();
            if (g_name.find(subname) != g_name.npos)
            {
                if (res != nullptr)
                {
                    std::cerr << "Multiple gates contains the subtring '" << subname << "'! This should not happen..." << std::endl;
                    return nullptr;
                }
                res = g;
            }
        }
        return res;
    }

    std::filesystem::path test_utils::create_sandbox_directory()
    {
        std::filesystem::path sb_path = core_utils::get_base_directory() / sandbox_directory_path;
        std::filesystem::create_directory(sb_path);
        return sb_path;
    }

    void test_utils::remove_sandbox_directory()
    {
        std::filesystem::remove_all((core_utils::get_base_directory() / sandbox_directory_path));
    }

    std::filesystem::path test_utils::create_sandbox_path(const std::string file_name)
    {
        std::filesystem::path sb_path = (core_utils::get_base_directory() / sandbox_directory_path);
        if (!std::filesystem::exists(sb_path))
        {
            std::cerr << "[netlist_test_utils] create_sandbox_path: sandbox is not created yet. "
                      << "Please use \'create_sandbox_directory()\' to create it beforehand.";
            return std::filesystem::path();
        }
        return sb_path / file_name;
    }

    std::filesystem::path test_utils::create_sandbox_file(std::string file_name, std::string content)
    {
        std::filesystem::path sb_path = (core_utils::get_base_directory() / sandbox_directory_path);
        if (!std::filesystem::exists(sb_path))
        {
            std::cerr << "[netlist_test_utils] create_sandbox_file: sandbox is not created yet. "
                      << "Please use \'create_sandbox_directory()\' to create it beforehand.";
            return std::filesystem::path();
        }
        std::filesystem::path f_path = sb_path / file_name;
        std::ofstream sb_file(f_path.string());
        sb_file << content;
        sb_file.close();
        return f_path;
    }

    GateLibrary* test_utils::get_testing_gate_library()
    {
        //GateLibrary* gl = std::make_unique<GateLibrary>("Testing Library");
        static std::unique_ptr<GateLibrary> gl = nullptr;
        if (gl != nullptr)
        {
            return gl.get();
        }
        gl = std::make_unique<GateLibrary>("imaginary_path", "Testing Library");

        std::unique_ptr<GateType> gt;

        gt = std::make_unique<GateType>("gate_1_to_1");
        gt->add_input_pins({"I"});
        gt->add_output_pins({"O"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_2_to_2");
        gt->add_input_pins({"I0", "I1"});
        gt->add_output_pins({"O0", "O1"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_2_to_1");
        gt->add_input_pins({"I0", "I1"});
        gt->add_output_pins({"O"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_1_to_2");
        gt->add_input_pins({"I"});
        gt->add_output_pins({"O0", "O1"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_3_to_3");
        gt->add_input_pins({"I0", "I1", "I2"});
        gt->add_output_pins({"O0", "O1", "O2"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_3_to_1");
        gt->add_input_pins({"I0", "I1", "I2"});
        gt->add_output_pins({"O"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_1_to_3");
        gt->add_input_pins({"I"});
        gt->add_output_pins({"O0", "O1", "O2"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_4_to_4");
        gt->add_input_pins({"I0", "I1", "I2", "I3"});
        gt->add_output_pins({"O0", "O1", "O2", "O3"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_4_to_1");
        gt->add_input_pins({"I0", "I1", "I2", "I3"});
        gt->add_output_pins({"O"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_1_to_4");
        gt->add_input_pins({"I"});
        gt->add_output_pins({"O0", "O1", "O2", "O3"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_8_to_8");
        gt->add_input_pins({"I0", "I1", "I2", "I3", "I4", "I5", "I6", "I7"});
        gt->add_output_pins({"O0", "O1", "O2", "O3", "O4", "O5", "O6", "O7"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_8_to_1");
        gt->add_input_pins({"I0", "I1", "I2", "I3", "I4", "I5", "I6", "I7"});
        gt->add_output_pins({"O"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_1_to_8");
        gt->add_input_pins({"I"});
        gt->add_output_pins({"O0", "O1", "O2", "O3", "O4", "O5", "O6", "O7"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gate_2_to_0");
        gt->add_input_pins({"I0", "I1"});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("pin_group_gate_4_to_4");
        gt->add_input_pins({"I(0)", "I(1)", "I(2)", "I(3)"});
        gt->add_output_pins({"O(0)", "O(1)", "O(2)", "O(3)"});
        gt->assign_input_pin_group("I", {{0, "I(0)"}, {1, "I(1)"}, {2, "I(2)"}, {3, "I(3)"}});
        gt->assign_output_pin_group("O", {{0, "O(0)"}, {1, "O(1)"}, {2, "O(2)"}, {3, "O(3)"}});
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("gnd");
        gt->add_output_pins({"O"});
        gt->add_boolean_function("O", BooleanFunction::from_string("0"));
        gl->add_gate_type(std::move(gt));

        gt = std::make_unique<GateType>("vcc");
        gt->add_output_pins({"O"});
        gt->add_boolean_function("O", BooleanFunction::from_string("1"));
        gl->add_gate_type(std::move(gt));

        return gl.get();
    }

    std::unique_ptr<Netlist> test_utils::create_example_netlist(const int id)
    {
        NO_COUT_BLOCK;
        GateLibrary* gl = get_testing_gate_library();
        auto nl         = create_empty_netlist(id);
        if (id >= 0)
        {
            nl->set_id(id);
        }

        // Create the gates
        Gate* gate_0 = nl->create_gate(MIN_GATE_ID + 0, gl->get_gate_types().at("gate_2_to_1"), "gate_0");
        Gate* gate_1 = nl->create_gate(MIN_GATE_ID + 1, gl->get_gate_types().at("gnd"), "gate_1");
        Gate* gate_2 = nl->create_gate(MIN_GATE_ID + 2, gl->get_gate_types().at("vcc"), "gate_2");
        Gate* gate_3 = nl->create_gate(MIN_GATE_ID + 3, gl->get_gate_types().at("gate_1_to_1"), "gate_3");
        Gate* gate_4 = nl->create_gate(MIN_GATE_ID + 4, gl->get_gate_types().at("gate_1_to_1"), "gate_4");
        Gate* gate_5 = nl->create_gate(MIN_GATE_ID + 5, gl->get_gate_types().at("gate_2_to_1"), "gate_5");
        Gate* gate_6 = nl->create_gate(MIN_GATE_ID + 6, gl->get_gate_types().at("gate_2_to_0"), "gate_6");
        Gate* gate_7 = nl->create_gate(MIN_GATE_ID + 7, gl->get_gate_types().at("gate_2_to_1"), "gate_7");
        Gate* gate_8 = nl->create_gate(MIN_GATE_ID + 8, gl->get_gate_types().at("gate_2_to_1"), "gate_8");

        // Add the nets (net_x_y1_y2... := Net between the Gate with id x and the gates y1,y2,...)
        Net* net_1_3 = nl->create_net(MIN_NET_ID + 13, "net_1_3");
        net_1_3->add_source(gate_1, "O");
        net_1_3->add_destination(gate_3, "I");

        Net* net_3_0 = nl->create_net(MIN_NET_ID + 30, "net_3_0");
        net_3_0->add_source(gate_3, "O");
        net_3_0->add_destination(gate_0, "I0");

        Net* net_2_0 = nl->create_net(MIN_NET_ID + 20, "net_2_0");
        net_2_0->add_source(gate_2, "O");
        net_2_0->add_destination(gate_0, "I1");

        Net* net_0_4_5 = nl->create_net(MIN_NET_ID + 045, "net_0_4_5");
        net_0_4_5->add_source(gate_0, "O");
        net_0_4_5->add_destination(gate_4, "I");
        net_0_4_5->add_destination(gate_5, "I0");

        Net* net_7_8 = nl->create_net(MIN_NET_ID + 78, "net_7_8");
        net_7_8->add_source(gate_7, "O");
        net_7_8->add_destination(gate_8, "I0");

        return nl;
    }

    std::unique_ptr<Netlist> test_utils::create_example_netlist_2(const int id)
    {
        NO_COUT_BLOCK;
        GateLibrary* gl = get_testing_gate_library();
        auto nl         = create_empty_netlist(id);

        // Create the gates
        Gate* gate_0 = nl->create_gate(MIN_GATE_ID + 0, gl->get_gate_types().at("gate_4_to_1"), "gate_0");
        Gate* gate_1 = nl->create_gate(MIN_GATE_ID + 1, gl->get_gate_types().at("gate_4_to_1"), "gate_1");
        Gate* gate_2 = nl->create_gate(MIN_GATE_ID + 2, gl->get_gate_types().at("gate_4_to_1"), "gate_2");
        Gate* gate_3 = nl->create_gate(MIN_GATE_ID + 3, gl->get_gate_types().at("gate_4_to_1"), "gate_3");

        // Add the nets (net_x_y1_y2... := Net between the Gate with id x and the gates y1,y2,...)

        Net* net_0_1_3 = nl->create_net(MIN_NET_ID + 013, "net_0_1_3");
        net_0_1_3->add_source(gate_0, "O");
        net_0_1_3->add_destination(gate_1, "I0");
        net_0_1_3->add_destination(gate_1, "I1");
        net_0_1_3->add_destination(gate_1, "I2");
        net_0_1_3->add_destination(gate_3, "I0");

        Net* net_2_1 = nl->create_net(MIN_NET_ID + 21, "net_2_1");
        net_2_1->add_source(gate_2, "O");
        net_2_1->add_destination(gate_1, "I3");

        return nl;
    }

    std::unique_ptr<Netlist> test_utils::create_example_netlist_negative(const int id)
    {
        NO_COUT_BLOCK;
        GateLibrary* gl = get_testing_gate_library();
        auto nl         = create_empty_netlist(id);

        // Create the Gate
        Gate* gate_0 = nl->create_gate(MIN_GATE_ID + 0, gl->get_gate_types().at("gate_1_to_1"), "gate_0");

        // Net connected to the input pin
        Net* net_X_1 = nl->create_net(MIN_GATE_ID + 0, "net_X_1");
        net_X_1->add_destination(gate_0, "I");

        // Net connected to the output pin
        Net* net_1_X = nl->create_net(MIN_GATE_ID + 1, "net_1_X");
        net_1_X->add_source(gate_0, "O");

        return nl;
    }

    std::unique_ptr<Netlist> test_utils::create_example_parse_netlist(int id)
    {
        NO_COUT_BLOCK;
        GateLibrary* gl = get_testing_gate_library();
        auto nl         = create_empty_netlist(id);

        // Create the gates
        Gate* gate_0 = nl->create_gate(MIN_GATE_ID + 0, gl->get_gate_types().at("gate_2_to_1"), "gate_0");
        Gate* gate_1 = nl->create_gate(MIN_GATE_ID + 1, gl->get_gate_types().at("gnd"), "gate_1");
        Gate* gate_2 = nl->create_gate(MIN_GATE_ID + 2, gl->get_gate_types().at("vcc"), "gate_2");
        Gate* gate_3 = nl->create_gate(MIN_GATE_ID + 3, gl->get_gate_types().at("gate_1_to_1"), "gate_3");
        Gate* gate_4 = nl->create_gate(MIN_GATE_ID + 4, gl->get_gate_types().at("gate_1_to_1"), "gate_4");
        Gate* gate_5 = nl->create_gate(MIN_GATE_ID + 5, gl->get_gate_types().at("gate_2_to_1"), "gate_5");
        Gate* gate_6 = nl->create_gate(MIN_GATE_ID + 6, gl->get_gate_types().at("gate_2_to_1"), "gate_6");
        Gate* gate_7 = nl->create_gate(MIN_GATE_ID + 7, gl->get_gate_types().at("gate_2_to_1"), "gate_7");

        // Add the nets (net_x_y1_y2... := Net between the Gate with id x and the gates y1,y2,...)
        Net* net_1_3 = nl->create_net(MIN_NET_ID + 13, "gnd_net");
        net_1_3->add_source(gate_1, "O");
        net_1_3->add_destination(gate_3, "I");

        Net* net_3_0 = nl->create_net(MIN_NET_ID + 30, "net_3_0");
        net_3_0->add_source(gate_3, "O");
        net_3_0->add_destination(gate_0, "I0");

        Net* net_2_0 = nl->create_net(MIN_NET_ID + 20, "vcc_net");
        net_2_0->add_source(gate_2, "O");
        net_2_0->add_destination(gate_0, "I1");

        Net* net_0_4_5 = nl->create_net(MIN_NET_ID + 045, "net_0_4_5");
        net_0_4_5->add_source(gate_0, "O");
        net_0_4_5->add_destination(gate_4, "I");
        net_0_4_5->add_destination(gate_5, "I0");

        Net* net_6_7 = nl->create_net(MIN_NET_ID + 67, "net_6_7");
        net_6_7->add_source(gate_6, "O");
        net_6_7->add_destination(gate_7, "I0");

        Net* net_4_out = nl->create_net(MIN_NET_ID + 400, "net_4_out");
        net_4_out->add_source(gate_4, "O");

        Net* net_5_out = nl->create_net(MIN_NET_ID + 500, "net_5_out");
        net_5_out->add_source(gate_5, "O");

        Net* net_7_out = nl->create_net(MIN_NET_ID + 700, "net_7_out");
        net_7_out->add_source(gate_7, "O");

        return nl;
    }

    Gate* test_utils::create_test_gate(Netlist* nl, const u32 id)
    {
        GateLibrary* gl = get_testing_gate_library();
        Gate* res_gate  = nl->create_gate(id, gl->get_gate_types().at("gate_3_to_1"), "gate_" + std::to_string(id));

        return res_gate;
    }

    Endpoint test_utils::get_destination_by_pin_type(const std::vector<Endpoint> dsts, const std::string pin_type)
    {
        for (auto dst : dsts)
        {
            if (dst.get_pin() == pin_type)
            {
                return dst;
            }
        }
        return Endpoint();
    }

    bool test_utils::nets_are_equal(Net* n0, Net* n1, const bool ignore_id, const bool ignore_name)
    {
        if (n0 == nullptr || n1 == nullptr)
        {
            if (n0 == n1)
                return true;
            else
                return false;
        }
        if (!ignore_id && n0->get_id() != n1->get_id())
            return false;
        if (!ignore_name && n0->get_name() != n1->get_name())
            return false;
        if (n0->get_source().get_pin() != n1->get_source().get_pin())
            return false;
        if (!gates_are_equal(n0->get_source().get_gate(), n1->get_source().get_gate(), ignore_id, ignore_name))
            return false;
        for (auto n0_destination : n0->get_destinations())
        {
            if (!gates_are_equal(n0_destination.get_gate(), get_destination_by_pin_type(n1->get_destinations(), n0_destination.get_pin()).get_gate(), ignore_id, ignore_name))
            {
                return false;
            }
        }
        if (n0->get_data() != n1->get_data())
            return false;
        if (n0->is_global_input_net() != n1->is_global_input_net())
            return false;
        if (n0->is_global_output_net() != n1->is_global_output_net())
            return false;

        return true;
    }

    bool test_utils::gates_are_equal(Gate* g0, Gate* g1, const bool ignore_id, const bool ignore_name)
    {
        if (g0 == nullptr || g1 == nullptr)
        {
            if (g0 == g1)
                return true;
            else
                return false;
        }
        if (!ignore_id && g0->get_id() != g1->get_id())
            return false;
        if (!ignore_name && g0->get_name() != g1->get_name())
            return false;
        if (g0->get_type() != g1->get_type())
            return false;
        if (g0->get_data() != g1->get_data())
            return false;
        if (g0->is_gnd_gate() != g1->is_gnd_gate())
            return false;
        if (g0->is_vcc_gate() != g1->is_vcc_gate())
            return false;
        return true;
    }

    bool test_utils::modules_are_equal(Module* m_0, Module* m_1, const bool ignore_id, const bool ignore_name)
    {
        // Not only one of them may be a nullptr
        if (m_0 == nullptr || m_1 == nullptr)
        {
            if (m_0 == m_1)
            {
                return true;
            }
            else
            {
                std::cout << "one module is null while the other isnt" << std::endl;
                return false;
            }
        }
        // The ids should be equal
        if (!ignore_id && m_0->get_id() != m_1->get_id())
        {
            std::cout << "ids unequal" << std::endl;
            return false;
        }
        // The names should be equal
        if (!ignore_name && m_0->get_name() != m_1->get_name())
        {
            std::cout << "names unequal" << std::endl;
            return false;
        }
        // The types should be the same
        if (m_0->get_type() != m_1->get_type())
        {
            std::cout << "type unequal" << std::endl;
            return false;
        }
        // The stored data should be equal
        if (m_0->get_data() != m_1->get_data())
        {
            std::cout << "data unequal" << std::endl;
            return false;
        }

        // Check if gates and nets are the same
        if (m_0->get_gates().size() != m_1->get_gates().size())
        {
            std::cout << "number of gates unequal" << std::endl;
            return false;
        }
        for (auto g_0 : m_0->get_gates())
        {
            Gate* g_1 = m_1->get_netlist()->get_gate_by_id(g_0->get_id());
            if (!gates_are_equal(g_0, g_1, ignore_id, ignore_name))
                return false;
            if (!m_1->contains_gate(g_1))
                return false;
        }

        // Check that the port names are the same
        // -- input ports
        if (m_0->get_input_port_names().size() != m_0->get_input_port_names().size())
        {
            std::cout << "number of input port names unequal" << std::endl;
            return false;
        }
        auto m_1_input_port_names = m_1->get_input_port_names();
        for (auto const& [n_0, p_name_0] : m_0->get_input_port_names())
        {
            auto n_1_list = m_1->get_netlist()->get_nets(net_name_filter(n_0->get_name()));
            if (n_1_list.size() != 1)
                return false;
            Net* n_1 = *n_1_list.begin();
            if (m_1_input_port_names.find(n_1) == m_1_input_port_names.end())
                return false;
            if (m_1_input_port_names[n_1] != p_name_0)
                return false;
        }
        // -- output ports
        if (m_0->get_output_port_names().size() != m_0->get_output_port_names().size())
        {
            std::cout << "number of output port names unequal" << std::endl;
            return false;
        }
        auto m_1_output_port_names = m_1->get_output_port_names();
        for (auto const& [n_0, p_name_0] : m_0->get_output_port_names())
        {
            auto n_1_list = m_1->get_netlist()->get_nets(net_name_filter(n_0->get_name()));
            if (n_1_list.size() != 1)
                return false;
            Net* n_1 = *n_1_list.begin();
            if (m_1_output_port_names.find(n_1) == m_1_output_port_names.end())
                return false;
            if (m_1_output_port_names[n_1] != p_name_0)
                return false;
        }

        // The parents and submodules should be equal as well (to test this we only check their id, since
        // their equality will be tested as well)
        if (m_0->get_parent_module() == nullptr || m_1->get_parent_module() == nullptr)
        {
            if (m_0->get_parent_module() != m_1->get_parent_module())
            {
                std::cout << "parent unequal" << std::endl;
                return false;
            }
        }
        if (m_0->get_submodules(nullptr, true).size() != m_1->get_submodules(nullptr, true).size())
        {
            std::cout << "number of submodules unequal" << std::endl;
            return false;
        }
        for (auto sm_0 : m_0->get_submodules(nullptr, true))
        {
            if (sm_0 == nullptr)
                continue;
            if (m_1->get_netlist()->get_module_by_id(sm_0->get_id()) == nullptr)
            {
                return false;
            }
        }

        return true;
    }

    bool test_utils::netlists_are_equal(Netlist* nl_0, Netlist* nl_1, const bool ignore_id)
    {
        if (nl_0 == nullptr || nl_1 == nullptr)
        {
            if (nl_0 == nl_1)
                return true;
            else
                return false;
        }
        // Check if the ids are the same
        if (!ignore_id && nl_0->get_id() != nl_1->get_id())
            return false;
        // Check that the Gate libraries are the same
        auto t_1 = nl_0->get_gate_library()->get_name();
        auto t_2 = nl_1->get_gate_library()->get_name();
        if (nl_0->get_gate_library()->get_name() != nl_1->get_gate_library()->get_name())
            return false;
        // Check that the design/device names are the same
        if (nl_0->get_design_name() != nl_1->get_design_name())
            return false;
        if (nl_0->get_device_name() != nl_1->get_device_name())
            return false;

        // Check if gates and nets are the same
        if (nl_0->get_gates().size() != nl_1->get_gates().size())
            return false;
        for (auto g_0 : nl_0->get_gates())
        {
            if (ignore_id)
            {
                auto g_1_list = nl_1->get_gates(gate_name_filter(g_0->get_name()));
                if (g_1_list.size() != 1)
                    return false;
                if (!gates_are_equal(g_0, *g_1_list.begin(), ignore_id))
                    return false;
            }
            else
            {
                if (!gates_are_equal(g_0, nl_1->get_gate_by_id(g_0->get_id()), ignore_id))
                    return false;
            }
        }

        if (nl_0->get_nets().size() != nl_1->get_nets().size())
            return false;
        for (auto n_0 : nl_0->get_nets())
        {
            if (ignore_id)
            {
                auto n_1_list = nl_1->get_nets(net_name_filter(n_0->get_name()));
                if (n_1_list.size() != 1)
                    return false;
                if (!nets_are_equal(n_0, *n_1_list.begin(), ignore_id))
                    return false;
            }
            else
            {
                if (!nets_are_equal(n_0, nl_1->get_net_by_id(n_0->get_id()), ignore_id))
                    return false;
            }
        }

        // -- Check if the modules are the same
        if (nl_0->get_modules().size() != nl_1->get_modules().size())
            return false;
        std::set<Module*> mods_1 = nl_1->get_modules();
        for (auto m_0 : nl_0->get_modules())
        {
            if (ignore_id)
            {
                auto m_1_all = nl_1->get_modules();
                auto m_1     = std::find_if(m_1_all.begin(), m_1_all.end(), [m_0](Module* m) { return m->get_name() == m_0->get_name(); });
                if (m_1 == m_1_all.end())
                    return false;
                if (!modules_are_equal(m_0, *m_1, ignore_id))
                    return false;
            }
            else
            {
                if (!modules_are_equal(m_0, nl_1->get_module_by_id(m_0->get_id()), ignore_id))
                    return false;
            }
        }

        return true;
    }

    // Filter Functions

    std::function<bool(Module*)> test_utils::module_name_filter(const std::string& name)
    {
        return [name](auto m) { return m->get_name() == name; };
    }

    std::function<bool(Gate*)> test_utils::gate_filter(const std::string& type, const std::string& name)
    {
        return [name, type](auto g) { return g->get_name() == name && g->get_type()->get_name() == type; };
    }

    std::function<bool(Gate*)> test_utils::gate_name_filter(const std::string& name)
    {
        return [name](auto g) { return g->get_name() == name; };
    }

    std::function<bool(Gate*)> test_utils::gate_type_filter(const std::string& type)
    {
        return [type](auto g) { return g->get_type()->get_name() == type; };
    }

    std::function<bool(Net*)> test_utils::net_name_filter(const std::string& name)
    {
        return [name](auto n) { return n->get_name() == name; };
    }

    std::function<bool(const Endpoint&)> test_utils::endpoint_gate_type_filter(const std::string& gate_type)
    {
        return [gate_type](auto& ep) { return ep.get_gate()->get_type()->get_name() == gate_type; };
    }

    std::function<bool(const Endpoint&)> test_utils::endpoint_gate_name_filter(const std::string& name)
    {
        return [name](auto& ep) { return ep.get_gate()->get_name() == name; };
    }

    std::function<bool(const Endpoint&)> test_utils::endpoint_pin_type_filter(const std::string& pin_type)
    {
        return [pin_type](auto& ep) { return ep.get_pin() == pin_type; };
    }

    std::function<bool(const std::string&, const Endpoint&)> test_utils::adjacent_pin_filter(const std::string& pin)
    {
        return [pin](auto&, auto& ep) { return ep.get_pin() == pin; };
    }
    std::function<bool(const std::string&, const Endpoint&)> test_utils::starting_pin_filter(const std::string& pin)
    {
        return [pin](auto& starting_pin, auto&) { return starting_pin == pin; };
    }

    std::function<bool(const std::string&, const Endpoint&)> test_utils::adjacent_gate_type_filter(const std::string& type)
    {
        return [type](auto&, auto& ep) { return ep.get_gate()->get_type()->get_name() == type; };
    }
}    // namespace hal
