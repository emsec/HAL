#include "netlist/hdl_writer/hdl_writer.h"

hdl_writer::hdl_writer(std::stringstream& stream) : m_stream(stream)
{
    m_netlist = nullptr;
}
