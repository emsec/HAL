#include "gui/graph_widget/items/nodes/modules/graphics_module.h"

#include "netlist/module.h"
#include "netlist/net.h"

namespace hal
{
    GraphicsModule::GraphicsModule(Module* m) : GraphicsNode(hal::item_type::module, m->get_id(), QString::fromStdString(m->get_name()))
    {
        for (Net* n : m->get_input_nets())
            m_input_pins.append(module_pin{QString::fromStdString(m->get_input_port_name(n)), n->get_id()});

        for (Net* n : m->get_output_nets())
            m_output_pins.append(module_pin{QString::fromStdString(m->get_output_port_name(n)), n->get_id()});
    }
}
