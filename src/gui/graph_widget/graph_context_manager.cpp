#include "gui/graph_widget/graph_context_manager.h"

#include "netlist/gate.h"
#include "netlist/module.h"
#include "netlist/netlist.h"

#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/graph_widget/layouters/physical_graph_layouter.h"
#include "gui/graph_widget/layouters/standard_graph_layouter.h"
#include "gui/graph_widget/shaders/module_shader.h"
#include "gui/gui_globals.h"

graph_context_manager::graph_context_manager()
{
}

graph_context* graph_context_manager::create_new_context(const QString& name)
{
    graph_context* context = new graph_context(name);
    context->set_layouter(get_default_layouter(context));
    context->set_shader(get_default_shader(context));
    m_graph_contexts.append(context);
    Q_EMIT context_created(context);
    return context;
}

void graph_context_manager::rename_graph_context(graph_context* ctx, const QString& new_name)
{
    ctx->m_name = new_name;

    Q_EMIT context_renamed(ctx);
}

void graph_context_manager::delete_graph_context(graph_context* ctx)
{
    Q_EMIT deleting_context(ctx);
    m_graph_contexts.remove(m_graph_contexts.indexOf(ctx));
    delete ctx;
}

QVector<graph_context*> graph_context_manager::get_contexts() const
{
    return m_graph_contexts;
}

bool graph_context_manager::context_with_name_exists(const QString& name) const
{
    for (const auto& ctx : m_graph_contexts)
    {
        if (ctx->name() == name)
        {
            return true;
        }
    }
    return false;
}

void graph_context_manager::handle_module_removed(const std::shared_ptr<module> m)
{
    for (graph_context* context : m_graph_contexts)
        if (context->modules().contains(m->get_id()))
            context->remove({m->get_id()}, {});
}

void graph_context_manager::handle_module_name_changed(const std::shared_ptr<module> m) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->modules().contains(m->get_id()))
            context->schedule_scene_update();
}

void graph_context_manager::handle_module_color_changed(const std::shared_ptr<module> m) const
{
    auto gates = m->get_gates();
    QSet<u32> gateIDs;
    for (auto g : gates)
        gateIDs.insert(g->get_id());
    for (graph_context* context : m_graph_contexts)
        if (context->modules().contains(m->get_id()) // contains module
            || context->gates().intersects(gateIDs)) // contains gate from module
            context->schedule_scene_update();
    // a context can contain a gate from a module if it is showing the module
    // or if it's showing a parent and the module is unfolded
}

void graph_context_manager::handle_module_submodule_added(const std::shared_ptr<module> m, const u32 added_module) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->is_showing_module(m->get_id(), {added_module}, {}, {}, {}))
            context->add({added_module}, {});
}

void graph_context_manager::handle_module_submodule_removed(const std::shared_ptr<module> m, const u32 removed_module)
{
    // FIXME this also triggers on module deletion (not only moving)
    // and collides with handle_module_removed
    for (graph_context* context : m_graph_contexts)
        if (context->is_showing_module(m->get_id(), {}, {}, {removed_module}, {}))
        {
            context->remove({removed_module}, {});
            if (context->empty())
            {
                delete_graph_context(context);
            }
        }
}

void graph_context_manager::handle_module_gate_assigned(const std::shared_ptr<module> m, const u32 inserted_gate) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->is_showing_module(m->get_id(), {}, {inserted_gate}, {}, {}))
            context->add({}, {inserted_gate});
}

void graph_context_manager::handle_module_gate_removed(const std::shared_ptr<module> m, const u32 removed_gate)
{
    for (graph_context* context : m_graph_contexts)
        if (context->is_showing_module(m->get_id(), {}, {}, {}, {removed_gate}))
        {
            context->remove({}, {removed_gate});
            if (context->empty())
            {
                delete_graph_context(context);
            }
        }
}

void graph_context_manager::handle_gate_name_changed(const std::shared_ptr<gate> g) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->gates().contains(g->get_id()))
            context->schedule_scene_update();
}

void graph_context_manager::handle_net_created(const std::shared_ptr<net> n) const
{
    Q_UNUSED(n)

    // CAN NETS BE CREATED WITH SRC AND DST INFORMATION ?
    // IF NOT THIS EVENT DOESNT NEED TO BE HANDLED
}

void graph_context_manager::handle_net_removed(const std::shared_ptr<net> n) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->nets().contains(n->get_id()))
            context->schedule_scene_update();
}

void graph_context_manager::handle_net_name_changed(const std::shared_ptr<net> n) const
{
    Q_UNUSED(n)

    // TRIGGER RESHADE FOR ALL CONTEXTS THAT RECURSIVELY CONTAIN THE NET
}

void graph_context_manager::handle_net_src_changed(const std::shared_ptr<net> n) const
{
    for (graph_context* context : m_graph_contexts)
        if (context->nets().contains(n->get_id()))
            context->schedule_scene_update();
}

void graph_context_manager::handle_net_dst_added(const std::shared_ptr<net> n, const u32 dst_gate_id) const
{
    Q_UNUSED(dst_gate_id)

    for (graph_context* context : m_graph_contexts)
        if (context->nets().contains(n->get_id()) || context->gates().contains(dst_gate_id))
            context->schedule_scene_update();
}

void graph_context_manager::handle_net_dst_removed(const std::shared_ptr<net> n, const u32 dst_gate_id) const
{
    Q_UNUSED(dst_gate_id)

    for (graph_context* context : m_graph_contexts)
        if (context->nets().contains(n->get_id()))
            context->schedule_scene_update();
}

graph_layouter* graph_context_manager::get_default_layouter(graph_context* const context) const
{
    // USE SETTINGS + FACTORY
    return new standard_graph_layouter(context);
}

graph_shader* graph_context_manager::get_default_shader(graph_context* const context) const
{
    // USE SETTINGS + FACTORY
    return new module_shader(context);
}
