#include "gui/gui_api/gui_api.h"

#include "gui/gui_globals.h"

#include <algorithm>

#include <QSet>

namespace hal
{
    GuiApi::GuiApi()
    {
        g_selection_relay.register_sender(this, "GUI API");
    }

    std::vector<u32> GuiApi::get_selected_gate_ids()
    {
        return std::vector<u32>(g_selection_relay.m_selected_gates.begin(), g_selection_relay.m_selected_gates.end());
    }

    std::vector<u32> GuiApi::get_selected_net_ids()
    {
        return std::vector<u32>(g_selection_relay.m_selected_nets.begin(), g_selection_relay.m_selected_nets.end());
    }

    std::vector<u32> GuiApi::get_selected_module_ids()
    {
        return std::vector<u32>(g_selection_relay.m_selected_modules.begin(), g_selection_relay.m_selected_modules.end());
    }

    std::tuple<std::vector<u32>, std::vector<u32>, std::vector<u32>> GuiApi::get_selected_item_ids()
    {
        return std::make_tuple(get_selected_gate_ids(), get_selected_net_ids(), get_selected_module_ids());
    }

    std::vector<Gate*> GuiApi::get_selected_gates()
    {
        std::vector<Gate*> gates(g_selection_relay.m_selected_gates.size());
        std::transform(g_selection_relay.m_selected_gates.begin(), g_selection_relay.m_selected_gates.end(), gates.begin(), [](u32 id){return g_netlist->get_gate_by_id(id);});

        return gates;
    }

    std::vector<Net*> GuiApi::get_selected_nets()
    {
        std::vector<Net*> nets(g_selection_relay.m_selected_nets.size());
        std::transform(g_selection_relay.m_selected_nets.begin(), g_selection_relay.m_selected_nets.end(), nets.begin(), [](u32 id){return g_netlist->get_net_by_id(id);});

        return nets;
    }

    std::vector<Module*> GuiApi::get_selected_modules()
    {
        std::vector<Module*> modules(g_selection_relay.m_selected_modules.size());
        std::transform(g_selection_relay.m_selected_modules.begin(), g_selection_relay.m_selected_modules.end(), modules.begin(), [](u32 id){return g_netlist->get_module_by_id(id);});

        return modules;
    }

    std::tuple<std::vector<Gate*>, std::vector<Net*>, std::vector<Module*>> GuiApi::get_selected_items()
    {
        return std::make_tuple(get_selected_gates(), get_selected_nets(), get_selected_modules());
    }

    void GuiApi::select_gate(Gate* gate, bool clear_current_selection, bool navigate_to_selection)
    {
        if(!g_netlist->is_gate_in_netlist(gate))
            return;

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_gates.insert(gate->get_id());
        g_selection_relay.selection_changed(this);
        g_selection_relay.m_focus_type = SelectionRelay::item_type::gate;
        g_selection_relay.m_focus_id = gate->get_id();

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_gate(u32 gate_id, bool clear_current_selection, bool navigate_to_selection)
    {
        select_gate(g_netlist->get_gate_by_id(gate_id), clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select_gate(const std::vector<Gate*>& gates, bool clear_current_selection, bool navigate_to_selection)
    {
        QSet<u32> gate_ids;

        for(auto gate : gates)
        {
            if(!g_netlist->is_gate_in_netlist(gate))
                return;

            gate_ids.insert(gate->get_id());
        }

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_gates.unite(gate_ids);
        g_selection_relay.selection_changed(this);

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_gate(const std::vector<u32>& gate_ids, bool clear_current_selection, bool navigate_to_selection)
    {
        std::vector<Gate*> gates(gate_ids.size());
        std::transform(gate_ids.begin(), gate_ids.end(), gates.begin(), [](u32 gate_id){return g_netlist->get_gate_by_id(gate_id);});
        select_gate(gates, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select_net(Net* net, bool clear_current_selection, bool navigate_to_selection)
    {
        if(!g_netlist->is_net_in_netlist(net))
            return;

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_nets.insert(net->get_id());
        g_selection_relay.selection_changed(this);
        g_selection_relay.m_focus_type = SelectionRelay::item_type::net;
        g_selection_relay.m_focus_id = net->get_id();

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_net(u32 net_id, bool clear_current_selection, bool navigate_to_selection)
    {
        select_net(g_netlist->get_net_by_id(net_id), clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select_net(const std::vector<Net*>& nets, bool clear_current_selection, bool navigate_to_selection)
    {
        QSet<u32> net_ids;

        for(auto net : nets)
        {
            if(!g_netlist->is_net_in_netlist(net))
                return;

            net_ids.insert(net->get_id());
        }

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_nets.unite(net_ids);
        g_selection_relay.selection_changed(this);

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_net(const std::vector<u32>& net_ids, bool clear_current_selection, bool navigate_to_selection)
    {
        std::vector<Net*> nets(net_ids.size());
        std::transform(net_ids.begin(), net_ids.end(), nets.begin(), [](u32 net_id){return g_netlist->get_net_by_id(net_id);});
        select_net(nets, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select_module(Module* module, bool clear_current_selection, bool navigate_to_selection)
    {
        if(!g_netlist->is_module_in_netlist(module))
            return;

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_modules.insert(module->get_id());
        g_selection_relay.selection_changed(this);
        g_selection_relay.m_focus_type = SelectionRelay::item_type::module;
        g_selection_relay.m_focus_id = module->get_id();

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_module(u32 module_id, bool clear_current_selection, bool navigate_to_selection)
    {
        select_module(g_netlist->get_module_by_id(module_id), clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select_module(const std::vector<Module*>& modules, bool clear_current_selection, bool navigate_to_selection)
    {
        QSet<u32> module_ids;

        for(auto module : modules)
        {
            if(!g_netlist->is_module_in_netlist(module))
                return;

            module_ids.insert(module->get_id());
        }

        if(clear_current_selection)
            g_selection_relay.clear();

        g_selection_relay.m_selected_modules.unite(module_ids);
        g_selection_relay.selection_changed(this);

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select_module(const std::vector<u32>& module_ids, bool clear_current_selection, bool navigate_to_selection)
    {
        std::vector<Module*> modules(module_ids.size());
        std::transform(module_ids.begin(), module_ids.end(), modules.begin(), [](u32 g_id){return g_netlist->get_module_by_id(g_id);});
        select_module(modules, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select(Gate* gate, bool clear_current_selection, bool navigate_to_selection)
    {
        select_gate(gate, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select(Net* net, bool clear_current_selection, bool navigate_to_selection)
    {
        select_net(net, clear_current_selection, navigate_to_selection);
    }
    void GuiApi::select(Module* module, bool clear_current_selection, bool navigate_to_selection)
    {
        select_module(module, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select(const std::vector<Gate*>& gates, bool clear_current_selection, bool navigate_to_selection)
    {
        select_gate(gates, clear_current_selection, navigate_to_selection);
    }
    void GuiApi::select(const std::vector<Net*>& nets, bool clear_current_selection, bool navigate_to_selection)
    {
        select_net(nets, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select(const std::vector<Module*>& modules, bool clear_current_selection, bool navigate_to_selection)
    {
        select_module(modules, clear_current_selection, navigate_to_selection);
    }

    void GuiApi::select(const std::vector<u32>& gate_ids, const std::vector<u32>& net_ids, const std::vector<u32>& module_ids, bool clear_current_selection, bool navigate_to_selection)
    {
        if(clear_current_selection)
            g_selection_relay.clear();

        select_gate(gate_ids, false, false);
        select_net(net_ids, false, false);
        select_module(module_ids, false, false);

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::select(const std::vector<Gate*>& gates, const std::vector<Net*>& nets, const std::vector<Module*>& modules, bool clear_current_selection, bool navigate_to_selection)
    {
        if(clear_current_selection)
            g_selection_relay.clear();

        select_gate(gates, false, false);
        select_net(nets, false, false);
        select_module(modules, false, false);

        if(navigate_to_selection)
            Q_EMIT navigation_requested();
    }

    void GuiApi::deselect_gate(Gate* gate)
    {
        if(!g_netlist->is_gate_in_netlist(gate))
            return;

        g_selection_relay.m_selected_gates.remove(gate->get_id());
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_gate(u32 gate_id)
    {
        deselect_gate(g_netlist->get_gate_by_id(gate_id));
    }

    void GuiApi::deselect_gate(const std::vector<Gate*>& gates)
    {
        QSet<u32> gate_ids;

        for(Gate* gate : gates)
        {
            if(!g_netlist->is_gate_in_netlist(gate))
                return;

            gate_ids.insert(gate->get_id());
        }

        g_selection_relay.m_selected_gates.subtract(gate_ids);
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_gate(const std::vector<u32>& gate_ids)
    {
        std::vector<Gate*> gates(gate_ids.size());
        std::transform(gate_ids.begin(), gate_ids.end(), gates.begin(), [](u32 gate_id){return g_netlist->get_gate_by_id(gate_id);});
        deselect_gate(gates);
    }

    void GuiApi::deselect_net(Net* net)
    {
        if(!g_netlist->is_net_in_netlist(net))
            return;

        g_selection_relay.m_selected_nets.remove(net->get_id());
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_net(u32 net_id)
    {
        deselect_net(g_netlist->get_net_by_id(net_id));
    }

    void GuiApi::deselect_net(const std::vector<Net*>& nets)
    {
        QSet<u32> net_ids;

        for(Net* net : nets)
        {
            if(!g_netlist->is_net_in_netlist(net))
                return;

            net_ids.insert(net->get_id());
        }

        g_selection_relay.m_selected_nets.subtract(net_ids);
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_net(const std::vector<u32>& net_ids)
    {
        std::vector<Net*> nets(net_ids.size());
        std::transform(net_ids.begin(), net_ids.end(), nets.begin(), [](u32 net_id){return g_netlist->get_net_by_id(net_id);});
        deselect_net(nets);
    }

    void GuiApi::deselect_module(Module* module)
    {
        if(!g_netlist->is_module_in_netlist(module))
            return;

        g_selection_relay.m_selected_modules.remove(module->get_id());
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_module(u32 module_id)
    {
        deselect_module(g_netlist->get_module_by_id(module_id));
    }

    void GuiApi::deselect_module(const std::vector<Module*>& modules)
    {
        QSet<u32> module_ids;

        for(Module* module : modules)
        {
            if(!g_netlist->is_module_in_netlist(module))
                return;

            module_ids.insert(module->get_id());
        }

        g_selection_relay.m_selected_modules.subtract(module_ids);
        g_selection_relay.selection_changed(this);
    }

    void GuiApi::deselect_module(const std::vector<u32>& module_ids)
    {
        std::vector<Module*> modules(module_ids.size());
        std::transform(module_ids.begin(), module_ids.end(), modules.begin(), [](u32 module_id){return g_netlist->get_module_by_id(module_id);});
        deselect_module(modules);
    }

    void GuiApi::deselect(Gate* gate)
    {
        deselect_gate(gate);
    }

    void GuiApi::deselect(Net* net)
    {
        deselect_net(net);
    }
    void GuiApi::deselect(Module* module)
    {
        deselect_module(module);
    }

    void GuiApi::deselect(const std::vector<Gate*>& gates)
    {
        deselect_gate(gates);
    }
    void GuiApi::deselect(const std::vector<Net*>& nets)
    {
        deselect_net(nets);
    }

    void GuiApi::deselect(const std::vector<Module*>& modules)
    {
        deselect_module(modules);
    }

    void GuiApi::deselect(const std::vector<u32>& gate_ids, const std::vector<u32>& net_ids, const std::vector<u32>& module_ids)
    {
        deselect_gate(gate_ids);
        deselect_net(net_ids);
        deselect_module(module_ids);
    }

    void GuiApi::deselect(const std::vector<Gate*>& gates, const std::vector<Net*>& nets, const std::vector<Module*>& modules)
    {
        deselect_gate(gates);
        deselect_net(nets);
        deselect_module(modules);
    }

    void GuiApi::deselect_all_items()
    {
        g_selection_relay.clear_and_update();
    }
}
