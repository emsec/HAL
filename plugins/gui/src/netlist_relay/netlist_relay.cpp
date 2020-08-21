#include "gui/netlist_relay/netlist_relay.h"

#include "core/log.h"

#include "netlist/gate.h"
#include "netlist/module.h"
#include "netlist/net.h"

#include "gui/module_model/module_item.h"
#include "gui/module_model/module_model.h"

#include "gui/file_manager/file_manager.h"    // DEBUG LINE
#include "gui/gui_globals.h"                  // DEBUG LINE
#include "gui/gui_utils/graphics.h"

#include <functional>

#include <QColorDialog>    // DEBUG LINE
#include <QDebug>
#include <QInputDialog>    // DEBUG LINE

namespace hal
{
    NetlistRelay::NetlistRelay(QObject* parent) : QObject(parent), m_ModuleModel(new ModuleModel(this))
    {
        connect(FileManager::get_instance(), &FileManager::file_opened, this, &NetlistRelay::debug_handle_file_opened);    // DEBUG LINE
        register_callbacks();
        log_info("test", "register callbacks");
    }

    NetlistRelay::~NetlistRelay()
    {
        log_info("test", "unregister callbacks");
        netlist_event_handler::unregister_callback("relay");
        net_event_handler::unregister_callback("relay");
        gate_event_handler::unregister_callback("relay");
        module_event_handler::unregister_callback("relay");
    }

    void NetlistRelay::register_callbacks()
    {
        netlist_event_handler::register_callback("relay",
                                                 std::function<void(netlist_event_handler::event, Netlist*, u32)>(
                                                     std::bind(&NetlistRelay::relay_netlist_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

        net_event_handler::register_callback("relay",
                                             std::function<void(net_event_handler::event, Net*, u32)>(
                                                 std::bind(&NetlistRelay::relay_net_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

        gate_event_handler::register_callback("relay",
                                              std::function<void(gate_event_handler::event, Gate*, u32)>(
                                                  std::bind(&NetlistRelay::relay_gate_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

        module_event_handler::register_callback("relay",
                                                std::function<void(module_event_handler::event, Module*, u32)>(
                                                    std::bind(&NetlistRelay::relay_module_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    }

    QColor NetlistRelay::get_module_color(const u32 id)
    {
        return m_module_colors.value(id);
    }

    ModuleModel* NetlistRelay::get_ModuleModel()
    {
        return m_ModuleModel;
    }

    void NetlistRelay::debug_change_module_name(const u32 id)
    {
        // NOT THREADSAFE

        Module* m = g_netlist->get_module_by_id(id);
        assert(m);

        bool ok;
        QString text = QInputDialog::getText(nullptr, "Rename Module", "New Name", QLineEdit::Normal, QString::fromStdString(m->get_name()), &ok);

        if (ok && !text.isEmpty())
            m->set_name(text.toStdString());
    }

    void NetlistRelay::debug_change_module_type(const u32 id)
    {
        // NOT THREADSAFE

        Module* m = g_netlist->get_module_by_id(id);
        assert(m);

        bool ok;
        QString text = QInputDialog::getText(nullptr, "Change Module Type", "New Type", QLineEdit::Normal, QString::fromStdString(m->get_type()), &ok);

        if (ok && !text.isEmpty())
            m->set_type(text.toStdString());
    }

    void NetlistRelay::debug_change_module_color(const u32 id)
    {
        // NOT THREADSAFE

        Module* m = g_netlist->get_module_by_id(id);
        assert(m);

        QColor color = QColorDialog::getColor();

        if (!color.isValid())
            return;

        m_module_colors.insert(id, color);
        m_ModuleModel->update_module(id);

        // Since color is our Overlay over the netlist data, no event is
        // automatically fired. We need to take care of that ourselves here.
        g_graph_context_manager->handle_module_color_changed(m);

        Q_EMIT module_color_changed(m);
    }

    void NetlistRelay::debug_add_selection_to_module(const u32 id)
    {
        // NOT THREADSAFE
        // DECIDE HOW TO HANDLE MODULES

        Module* m = g_netlist->get_module_by_id(id);

        assert(m);

        for (auto sel_id : g_selection_relay->m_selected_gates)
        {
            Gate* g = g_netlist->get_gate_by_id(sel_id);

            if (g)
                m->assign_gate(g);
        }
    }

    void NetlistRelay::debug_add_child_module(const u32 id)
    {
        // NOT THREADSAFE

        bool ok;
        QString name = QInputDialog::getText(nullptr, "", "Module Name:", QLineEdit::Normal, "", &ok);

        if (!ok || name.isEmpty())
            return;

        Module* m = g_netlist->get_module_by_id(id);

        if (!m)
            return;

        Module* s = g_netlist->create_module(g_netlist->get_unique_module_id(), name.toStdString(), m);
    }

    void NetlistRelay::debug_delete_module(const u32 id)
    {
        Module* m = g_netlist->get_module_by_id(id);
        assert(m);

        g_netlist->delete_module(m);
    }

    void NetlistRelay::relay_netlist_event(netlist_event_handler::event ev, Netlist* object, u32 associated_data)
    {
        if (!object)
            return;    // SHOULD NEVER BE REACHED

        //qDebug() << "relay_netlist_event called: event ID =" << ev << "for object at" << object.get();
        //Q_EMIT netlist_event(ev, object, associated_data);

        switch (ev)
        {
            case netlist_event_handler::event::id_changed:
            {
                ///< associated_data = old id

                Q_EMIT netlist_id_changed(object, associated_data);
                break;
            }
            case netlist_event_handler::event::input_filename_changed:
            {
                ///< no associated_data

                Q_EMIT netlist_input_filename_changed(object);
                break;
            }
            case netlist_event_handler::event::design_name_changed:
            {
                ///< no associated_data

                Q_EMIT netlist_design_name_changed(object);
                break;
            }
            case netlist_event_handler::event::device_name_changed:
            {
                ///< no associated_data

                Q_EMIT netlist_device_name_changed(object);
                break;
            }
            case netlist_event_handler::event::marked_global_vcc:
            {
                ///< associated_data = id of gate

                Q_EMIT netlist_marked_global_vcc(object, associated_data);
                break;
            }
            case netlist_event_handler::event::marked_global_gnd:
            {
                ///< associated_data = id of gate

                Q_EMIT netlist_marked_global_gnd(object, associated_data);
                break;
            }
            case netlist_event_handler::event::unmarked_global_vcc:
            {
                ///< associated_data = id of gate

                Q_EMIT netlist_unmarked_global_vcc(object, associated_data);
                break;
            }
            case netlist_event_handler::event::unmarked_global_gnd:
            {
                ///< associated_data = id of gate

                Q_EMIT netlist_unmarked_global_gnd(object, associated_data);
                break;
            }
            case netlist_event_handler::event::marked_global_input:
            {
                ///< associated_data = id of net
                g_graph_context_manager->handle_marked_global_input(associated_data);

                Q_EMIT netlist_marked_global_input(object, associated_data);
                break;
            }
            case netlist_event_handler::event::marked_global_output:
            {
                ///< associated_data = id of net
                g_graph_context_manager->handle_marked_global_output(associated_data);

                Q_EMIT netlist_marked_global_output(object, associated_data);
                break;
            }
            case netlist_event_handler::event::marked_global_inout:
            {
                ///< associated_data = id of net

                Q_EMIT netlist_marked_global_inout(object, associated_data);
                break;
            }
            case netlist_event_handler::event::unmarked_global_input:
            {
                ///< associated_data = id of net
                g_graph_context_manager->handle_unmarked_global_input(associated_data);

                Q_EMIT netlist_unmarked_global_input(object, associated_data);
                break;
            }
            case netlist_event_handler::event::unmarked_global_output:
            {
                ///< associated_data = id of net
                g_graph_context_manager->handle_unmarked_global_output(associated_data);

                Q_EMIT netlist_unmarked_global_output(object, associated_data);
                break;
            }
            case netlist_event_handler::event::unmarked_global_inout:
            {
                ///< associated_data = id of net

                Q_EMIT netlist_unmarked_global_inout(object, associated_data);
                break;
            }
        }
    }

    void NetlistRelay::relay_module_event(module_event_handler::event ev, Module* object, u32 associated_data)
    {
        if (!object)
            return;    // SHOULD NEVER BE REACHED

        //qDebug() << "relay_module_event called: event ID =" << ev << "for object at" << object.get();
        //Q_EMIT module_event(ev, object, associated_data);

        switch (ev)
        {
            case module_event_handler::event::created:
            {
                //< no associated_data

                // suppress actions if we receive this for the top module
                if (object->get_parent_module() != nullptr)
                {
                    m_module_colors.insert(object->get_id(), gui_utility::get_random_color());
                }

                Q_EMIT module_created(object);
                break;
            }
            case module_event_handler::event::removed:
            {
                //< no associated_data

                m_module_colors.remove(object->get_id());

                g_graph_context_manager->handle_module_removed(object);
                g_selection_relay->handle_module_removed(object->get_id());

                Q_EMIT module_removed(object);
                break;
            }
            case module_event_handler::event::name_changed:
            {
                //< no associated_data

                m_ModuleModel->update_module(object->get_id());

                g_graph_context_manager->handle_module_name_changed(object);

                Q_EMIT module_name_changed(object);
                break;
            }
            case module_event_handler::event::parent_changed:
            {
                //< no associated_data

                Q_EMIT module_parent_changed(object);
                break;
            }
            case module_event_handler::event::submodule_added:
            {
                //< associated_data = id of added module

                m_ModuleModel->add_module(associated_data, object->get_id());

                g_graph_context_manager->handle_module_submodule_added(object, associated_data);

                Q_EMIT module_submodule_added(object, associated_data);
                break;
            }
            case module_event_handler::event::submodule_removed:
            {
                //< associated_data = id of removed module

                m_ModuleModel->remove_module(associated_data);

                g_graph_context_manager->handle_module_submodule_removed(object, associated_data);

                Q_EMIT module_submodule_removed(object, associated_data);
                break;
            }
            case module_event_handler::event::gate_assigned:
            {
                //< associated_data = id of inserted gate

                g_graph_context_manager->handle_module_gate_assigned(object, associated_data);

                Q_EMIT module_gate_assigned(object, associated_data);
                break;
            }
            case module_event_handler::event::gate_removed:
            {
                //< associated_data = id of removed gate

                g_graph_context_manager->handle_module_gate_removed(object, associated_data);

                Q_EMIT module_gate_removed(object, associated_data);
                break;
            }
            case module_event_handler::event::input_port_name_changed:
            {
                //< associated data = respective net

                Q_EMIT module_input_port_name_changed(object, associated_data);
                break;
            }
            case module_event_handler::event::output_port_name_changed:
            {
                //< associated data = respective net

                Q_EMIT module_output_port_name_changed(object, associated_data);
                break;
            }
            case module_event_handler::event::type_changed:
            {
                //< no associated_data

                Q_EMIT module_type_changed(object);
                break;
            }
        }
    }

    void NetlistRelay::relay_gate_event(gate_event_handler::event ev, Gate* object, u32 associated_data)
    {
        UNUSED(associated_data);
        if (!object)
            return;    // SHOULD NEVER BE REACHED

        //qDebug() << "relay_gate_event called: event ID =" << ev << "for object at" << object.get();
        //Q_EMIT gate_event(ev, object, associated_data);

        switch (ev)
        {
            case gate_event_handler::event::created:
            {
                //< no associated_data

                Q_EMIT gate_created(object);
                break;
            }
            case gate_event_handler::removed:
            {
                //< no associated_data

                g_selection_relay->handle_gate_removed(object->get_id());

                Q_EMIT gate_removed(object);
                break;
            }
            case gate_event_handler::name_changed:
            {
                //< no associated_data

                g_graph_context_manager->handle_gate_name_changed(object);

                Q_EMIT gate_name_changed(object);
                break;
            }
            default:
                break;
        }
    }

    void NetlistRelay::relay_net_event(net_event_handler::event ev, Net* object, u32 associated_data)
    {
        if (!object)
            return;    // SHOULD NEVER BE REACHED

        //qDebug() << "relay_net_event called: event ID =" << ev << "for object at" << object.get();
        //Q_EMIT net_event(ev, object, associated_data);

        switch (ev)
        {
            case net_event_handler::event::created:
            {
                //< no associated_data

                g_graph_context_manager->handle_net_created(object);

                Q_EMIT net_created(object);
                break;
            }
            case net_event_handler::event::removed:
            {
                //< no associated_data

                g_graph_context_manager->handle_net_removed(object);
                g_selection_relay->handle_net_removed(object->get_id());

                Q_EMIT net_removed(object);
                break;
            }
            case net_event_handler::event::name_changed:
            {
                //< no associated_data

                g_graph_context_manager->handle_net_name_changed(object);

                Q_EMIT net_name_changed(object);
                break;
            }
            // FIXME add src_added, src_removed
            // case net_event_handler::event::src_changed:
            // {
            //     //< no associated_data

            //     g_graph_context_manager->handle_net_source_changed(object);

            //     Q_EMIT net_source_changed(object);
            //     break;
            // }
            case net_event_handler::event::src_added:
            {
                //< associated_data = id of src gate

                g_graph_context_manager->handle_net_source_added(object, associated_data);

                Q_EMIT net_source_added(object, associated_data);
                break;
            }
            case net_event_handler::event::src_removed:
            {
                //< associated_data = id of src gate

                g_graph_context_manager->handle_net_source_removed(object, associated_data);

                Q_EMIT net_source_removed(object, associated_data);
                break;
            }
            case net_event_handler::event::dst_added:
            {
                //< associated_data = id of dst gate

                g_graph_context_manager->handle_net_destination_added(object, associated_data);

                Q_EMIT net_destination_added(object, associated_data);
                break;
            }
            case net_event_handler::event::dst_removed:
            {
                //< associated_data = id of dst gate

                g_graph_context_manager->handle_net_destination_removed(object, associated_data);

                Q_EMIT net_destination_removed(object, associated_data);
                break;
            }
        }
    }

    void NetlistRelay::debug_handle_file_opened()
    {
        for (Module* m : g_netlist->get_modules())
            m_module_colors.insert(m->get_id(), gui_utility::get_random_color());

        m_module_colors.insert(1, QColor(96, 110, 112));

        m_ModuleModel->init();
    }

    void NetlistRelay::debug_handle_file_closed()
    {
        m_ModuleModel->clear();
        m_module_colors.clear();
    }
}
