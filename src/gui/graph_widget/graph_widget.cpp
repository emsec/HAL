#include "gui/graph_widget/graph_widget.h"

#include "netlist/gate.h"
#include "netlist/module.h"
#include "netlist/net.h"

#include "gui/graph_tab_widget/graph_tab_widget.h"
#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/graph_widget/graph_context_manager.h"
#include "gui/graph_widget/graph_graphics_view.h"
#include "gui/graph_widget/graph_layout_progress_widget.h"
#include "gui/graph_widget/graph_layout_spinner_widget.h"
#include "gui/graph_widget/graph_navigation_widget.h"
#include "gui/graph_widget/graphics_scene.h"
#include "gui/graph_widget/items/graphics_gate.h"
#include "gui/gui_globals.h"
#include "gui/hal_content_manager/hal_content_manager.h"
#include "gui/overlay/dialog_overlay.h"
#include "gui/toolbar/toolbar.h"

#include <QDebug>
#include <QInputDialog>
#include <QKeyEvent>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVariantAnimation>

graph_widget::graph_widget(graph_context* context, QWidget* parent)
    : content_widget("Graph", parent), m_view(new graph_graphics_view(this)), m_context(context), m_overlay(new dialog_overlay(this)), m_navigation_widget(new graph_navigation_widget(nullptr)),
      m_progress_widget(new graph_layout_progress_widget(this)), m_spinner_widget(new graph_layout_spinner_widget(this)), m_current_expansion(0)
{
    connect(m_navigation_widget, &graph_navigation_widget::navigation_requested, this, &graph_widget::handle_navigation_jump_requested);
    connect(m_navigation_widget, &graph_navigation_widget::close_requested, m_overlay, &dialog_overlay::hide);
    connect(m_navigation_widget, &graph_navigation_widget::close_requested, this, &graph_widget::reset_focus);

    connect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    connect(m_view, &graph_graphics_view::module_double_clicked, this, &graph_widget::handle_module_double_clicked);

    m_overlay->hide();
    m_overlay->set_widget(m_navigation_widget);
    m_spinner_widget->hide();
    m_content_layout->addWidget(m_view);

    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setRenderHint(QPainter::Antialiasing, false);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);

    m_context->subscribe(this);

    if (!m_context->scene_update_in_progress())
    {
        m_view->setScene(m_context->scene());
        m_view->centerOn(0, 0);
    }
}

graph_context* graph_widget::get_context() const
{
    return m_context;
}

void graph_widget::handle_scene_available()
{
    m_view->setScene(m_context->scene());

    connect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    m_overlay->hide();
    m_spinner_widget->hide();
    m_overlay->set_widget(m_navigation_widget);

    if (hasFocus())
        m_view->setFocus();
}

void graph_widget::handle_scene_unavailable()
{
    m_view->setScene(nullptr);

    disconnect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    m_overlay->set_widget(m_spinner_widget);

    if (m_overlay->isHidden())
        m_overlay->show();
}

void graph_widget::handle_context_about_to_be_deleted()
{
    m_view->setScene(nullptr);
    m_context = nullptr;
}

void graph_widget::handle_status_update(const int percent)
{
    Q_UNUSED(percent)
}

void graph_widget::handle_status_update(const QString& message)
{
    Q_UNUSED(message)
}

void graph_widget::keyPressEvent(QKeyEvent* event)
{
    if (!m_context)
        return;

    if (m_context->scene_update_in_progress())
        return;

    switch (event->key())
    {
        case Qt::Key_Left:
        {
            handle_navigation_left_request();
            break;
        }
        case Qt::Key_Right:
        {
            handle_navigation_right_request();
            break;
        }
        case Qt::Key_Up:
        {
            handle_navigation_up_request();
            break;
        }
        case Qt::Key_Down:
        {
            handle_navigation_down_request();
            break;
        }
        case Qt::Key_Z:
        {
            if (event->modifiers() & Qt::ControlModifier)    // modifiers are set as bitmasks
            {
                handle_history_step_back_request();
            }
            break;
        }
        default:
            break;
    }
}

void graph_widget::handle_navigation_jump_requested(const u32 via_net, const u32 to_gate)
{
    setFocus();
    // ASSERT INPUTS ARE VALID ?
    auto n = g_netlist->get_net_by_id(via_net);
    auto g = g_netlist->get_gate_by_id(to_gate);

    if (!g || !n)
        return;

    if (!m_context->gates().contains(to_gate))
    {
        for (const auto& m : g_netlist->get_modules())
        {
            if (m->get_name() == m_context->name().toStdString())
            {
                u32 cnt = 0;
                while (true)
                {
                    ++cnt;
                    QString new_name = m_context->name() + " modified";
                    if (cnt > 1)
                    {
                        new_name += " (" + QString::number(cnt) + ")";
                    }
                    bool found = false;
                    for (const auto& ctx : g_graph_context_manager.get_contexts())
                    {
                        if (ctx->name() == new_name)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        g_graph_context_manager.rename_graph_context(m_context, new_name);
                        break;
                    }
                }
                break;
            }
        }

        m_context->add({}, {to_gate});
    }
    else
    {
        m_overlay->hide();
        //if (hasFocus())
        m_view->setFocus();
    }

    // SELECT IN RELAY
    g_selection_relay.clear();
    g_selection_relay.m_selected_gates.insert(to_gate);
    g_selection_relay.m_focus_type = selection_relay::item_type::gate;
    g_selection_relay.m_focus_id   = to_gate;
    g_selection_relay.m_subfocus   = selection_relay::subfocus::none;

    u32 cnt = 0;
    for (const auto& pin : g->get_input_pin_types())
    {
        if (g->get_fan_in_net(pin) == n)    // input net
        {
            g_selection_relay.m_subfocus       = selection_relay::subfocus::left;
            g_selection_relay.m_subfocus_index = cnt;
            break;
        }
        cnt++;
    }
    if (g_selection_relay.m_subfocus == selection_relay::subfocus::none)
    {
        cnt = 0;
        for (const auto& pin : g->get_output_pin_types())
        {
            if (g->get_fan_out_net(pin) == n)    // input net
            {
                g_selection_relay.m_subfocus       = selection_relay::subfocus::right;
                g_selection_relay.m_subfocus_index = cnt;
                break;
            }
            cnt++;
        }
    }

    g_selection_relay.relay_selection_changed(nullptr);

    // JUMP TO THE GATE
    ensure_gate_visible(to_gate);
}

void graph_widget::handle_module_double_clicked(const u32 id)
{
    // CONNECT DIRECTLY TO HANDLE ???
    // MAYBE ADDITIONAL CODE NECESSARY HERE...
    handle_enter_module_requested(id);
}

// ADD SOUND OR ERROR MESSAGE TO FAILED NAVIGATION ATTEMPTS
void graph_widget::handle_navigation_left_request()
{
    switch (g_selection_relay.m_focus_type)
    {
        case selection_relay::item_type::none:
        {
            return;
        }
        case selection_relay::item_type::gate:
        {
            std::shared_ptr<gate> g = g_netlist->get_gate_by_id(g_selection_relay.m_focus_id);

            if (!g)
                return;

            if (g_selection_relay.m_subfocus == selection_relay::subfocus::left)
            {
                std::string pin_type   = g->get_input_pin_types()[g_selection_relay.m_subfocus_index];
                std::shared_ptr<net> n = g->get_fan_in_net(pin_type);

                if (!n)
                    return;

                if (n->get_src().gate == nullptr)
                {
                    g_selection_relay.clear();
                    g_selection_relay.m_selected_nets.insert(n->get_id());
                    g_selection_relay.m_focus_type = selection_relay::item_type::net;
                    g_selection_relay.m_focus_id   = n->get_id();
                    g_selection_relay.relay_selection_changed(nullptr);
                }
                else
                {
                    handle_navigation_jump_requested(n->get_id(), n->get_src().get_gate()->get_id());
                }
            }
            else if (g->get_input_pin_types().size())
            {
                g_selection_relay.m_subfocus       = selection_relay::subfocus::left;
                g_selection_relay.m_subfocus_index = 0;

                g_selection_relay.relay_subfocus_changed(nullptr);
            }

            return;
        }
        case selection_relay::item_type::net:
        {
            std::shared_ptr<net> n = g_netlist->get_net_by_id(g_selection_relay.m_focus_id);

            if (!n)
                return;

            if (n->get_src().gate != nullptr)
            {
                handle_navigation_jump_requested(n->get_id(), n->get_src().get_gate()->get_id());
            }

            return;
        }
        case selection_relay::item_type::module:
        {
            return;
        }
    }
}

void graph_widget::handle_navigation_right_request()
{
    switch (g_selection_relay.m_focus_type)
    {
        case selection_relay::item_type::none:
        {
            return;
        }
        case selection_relay::item_type::gate:
        {
            std::shared_ptr<gate> g = g_netlist->get_gate_by_id(g_selection_relay.m_focus_id);

            if (!g)
                return;

            if (g_selection_relay.m_subfocus == selection_relay::subfocus::right)
            {
                auto n = g->get_fan_out_net(g->get_output_pin_types()[g_selection_relay.m_subfocus_index]);
                if (n->get_num_of_dsts() == 0)
                {
                    g_selection_relay.clear();
                    g_selection_relay.m_selected_nets.insert(n->get_id());
                    g_selection_relay.m_focus_type = selection_relay::item_type::net;
                    g_selection_relay.m_focus_id   = n->get_id();
                    g_selection_relay.relay_selection_changed(nullptr);
                }
                else if (n->get_num_of_dsts() == 1)
                {
                    handle_navigation_jump_requested(n->get_id(), n->get_dsts()[0].get_gate()->get_id());
                }
                else
                {
                    m_navigation_widget->setup();
                    m_navigation_widget->setFocus();
                    m_overlay->show();
                }
            }
            else if (g->get_output_pin_types().size())
            {
                g_selection_relay.m_subfocus       = selection_relay::subfocus::right;
                g_selection_relay.m_subfocus_index = 0;

                g_selection_relay.relay_subfocus_changed(nullptr);
            }

            return;
        }
        case selection_relay::item_type::net:
        {
            std::shared_ptr<net> n = g_netlist->get_net_by_id(g_selection_relay.m_focus_id);

            if (!n)
                return;

            if (n->get_num_of_dsts() == 0)
                return;

            if (n->get_num_of_dsts() == 1)
            {
                handle_navigation_jump_requested(n->get_id(), n->get_dsts()[0].get_gate()->get_id());
            }
            else
            {
                m_navigation_widget->setup();
                m_navigation_widget->setFocus();
                m_overlay->show();
            }

            return;
        }
        case selection_relay::item_type::module:
        {
            return;
        }
    }
}

void graph_widget::handle_navigation_up_request()
{
    if (g_selection_relay.m_focus_type == selection_relay::item_type::gate)
        if (m_context->gates().contains(g_selection_relay.m_focus_id))
            g_selection_relay.navigate_up();
}

void graph_widget::handle_navigation_down_request()
{
    if (g_selection_relay.m_focus_type == selection_relay::item_type::gate)
        if (m_context->gates().contains(g_selection_relay.m_focus_id))
            g_selection_relay.navigate_down();
}

void graph_widget::handle_history_step_back_request()
{
    /*
    if (m_context_history.empty())
        return;

    auto entry = m_context_history.back();
    m_context_history.pop_back();

    m_context->begin_change();
    m_context->clear();
    m_context->add(entry.m_modules, entry.m_gates);
    m_context->end_change();

    qDebug() << "REMOVED context to history, full history:";
    int i = 0;

    for (const context_history_entry& e : m_context_history)
    {
        qDebug() << "Entry " + QString::number(i);

        QString modules = "Modules: " + QString::number(e.m_modules.size());
        //        QString modules = "Modules: ";
        //        for (u32 id : e.m_modules)
        //            modules.append(QString::number(id) + ", ");

        qDebug() << modules;

        QString gates = "Gates: " + QString::number(e.m_gates.size());
        //        QString gates = "Gates: ";
        //        for (u32 id : e.m_gates)
        //            gates.append(QString::number(id) + ", ");

        qDebug() << gates;

        ++i;
    }

    qDebug() << "-------------------------------------";
    */
}

void graph_widget::handle_enter_module_requested(const u32 id)
{
    if (m_context->gates().isEmpty() && m_context->modules() == QSet<u32>({id}))
    {
        m_context->unfold_module(id);
        return;
    }

    auto m = g_netlist->get_module_by_id(id);
    QSet<u32> gate_ids;
    QSet<u32> module_ids;
    for (const auto& g : m->get_gates())
    {
        gate_ids.insert(g->get_id());
    }
    for (const auto& sm : m->get_submodules())
    {
        module_ids.insert(sm->get_id());
    }

    for (const auto& ctx : g_graph_context_manager.get_contexts())
    {
        if ((ctx->gates().isEmpty() && ctx->modules() == QSet<u32>({id})) || (ctx->modules() == module_ids && ctx->gates() == gate_ids))
        {
            g_content_manager->get_graph_tab_widget()->show_context(ctx);
            return;
        }
    }

    auto ctx = g_graph_context_manager.create_new_context(QString::fromStdString(m->get_name()));
    ctx->add(module_ids, gate_ids);
}

void graph_widget::ensure_gate_visible(const u32 gate)
{
    if (m_context->scene_update_in_progress())
        return;

    const graphics_gate* itm = m_context->scene()->get_gate_item(gate);

    auto anim = new QVariantAnimation();
    anim->setDuration(1000);

    QPointF center = m_view->mapToScene(QRect(0, 0, m_view->viewport()->width(), m_view->viewport()->height())).boundingRect().center();
    anim->setStartValue(center);
    anim->setEndValue(itm->pos());

    connect(anim, &QVariantAnimation::valueChanged, [=](const QVariant& value) { m_view->centerOn(value.toPoint()); });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void graph_widget::add_context_to_history()
{
    /*context_history_entry entry;

    entry.m_modules = m_context->modules();
    entry.m_gates   = m_context->gates();

    m_context_history.push_back(entry);

    while (m_context_history.size() > 10)
    {
        m_context_history.pop_front();
    }

    qDebug() << "ADDED context to history, full history:";
    int i = 0;

    for (const context_history_entry& e : m_context_history)
    {
        qDebug() << "Entry " + QString::number(i);

        QString modules = "Modules: " + QString::number(e.m_modules.size());
        //        QString modules = "Modules: ";
        //        for (u32 id : e.m_modules)
        //            modules.append(QString::number(id) + ", ");

        qDebug() << modules;

        QString gates = "Gates: " + QString::number(e.m_gates.size());
        //        QString gates = "Gates: ";
        //        for (u32 id : e.m_gates)
        //            gates.append(QString::number(id) + ", ");

        qDebug() << gates;

        ++i;
    }

    qDebug() << "-------------------------------------";*/
}

void graph_widget::reset_focus()
{
    m_view->setFocus();
}

graph_graphics_view* graph_widget::view()
{
    return m_view;
}
