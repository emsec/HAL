#include "bitstream_parser/hdl_parser_bitstream.h"

#include "hal_core/netlist/gate_library/gate_library.h"
#include "hal_core/netlist/gate_library/gate_library_manager.h"
#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"
#include "hal_core/utilities/log.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

namespace hal
{
    HDLParserBitstream::HDLParserBitstream(std::filesystem::path output_path, std::filesystem::path database_path, std::filesystem::path bitread_path, const std::string& part)
        : m_output_path(output_path), m_database_path(database_path), m_bitread_path(bitread_path), m_part(part)
    {
    }

    bool HDLParserBitstream::parse(const std::filesystem::path& file_path)
    {
        std::string file_name = file_path.stem();

        std::filesystem::path connection_path = m_output_path / (file_name + ".db");
        std::filesystem::path fasm_path       = m_output_path / (file_name + ".fasm");
        std::filesystem::path xdc_path        = m_output_path / (file_name + ".xdc");
        m_verilog_path                        = m_output_path / (file_name + ".v");

        std::map<std::string, std::string> part_to_tool_input = {{"7a35tcpg236", "xc7a35tcpg236-1"},
                                                                 {"7a35tcsg324", "xc7a35tcsg324-1"},
                                                                 {"7a35tftg256", "xc7a35tftg256-1"},
                                                                 {"7a50tfgg484", "xc7a50tfgg484-1"},
                                                                 {"7a100tcsg324", "xc7a100tcsg324-1"},
                                                                 {"7a100tfgg676", "xc7a100tfgg676-1"},
                                                                 {"7a200tffg1156", "xc7a200tffg1156-1"},
                                                                 {"7a200tsbg484", "xc7a200tsbg484-1"},
                                                                 {"7k70tfbg676", "xc7k70tfbg676-2"},
                                                                 {"7z010clg400", "xc7z010clg400-1"},
                                                                 {"7z020clg484", "xc7z020clg484-1"}};

        std::string device = "";
        if (part_to_tool_input.find(m_part) != part_to_tool_input.end())
        {
            switch (m_part.at(1))
            {
                case 'a':
                    device = "artix7";
                    break;
                case 'k':
                    device = "kintex7";
                    break;
                case 'z':
                    device = "zynq7";
                    break;
                default:
                    assert(false);
            }
        }
        else
        {
            log_error("bitstream_parser", "Unsupported part '{}'.", m_part);
            return false;
        }

        std::string cmd = "python3 -mfasm2bels";
        cmd += " --connection_database " + connection_path.string();
        cmd += " --db_root " + (m_database_path / device).string();
        cmd += " --part " + m_part;
        cmd += " --fasm_file " + fasm_path.string();
        cmd += " --bit_file " + file_path.string();
        cmd += " --bitread " + m_bitread_path.string();
        cmd += " --verilog_file " + m_verilog_path.string();
        cmd += " --xdc_file " + xdc_path.string();
        cmd += " --iostandard " + m_io_standard;
        cmd += " --drive 16 " + m_io_drive;

        log_info("bitstream_parser", "Executing command: '{}'", cmd);

        // TODO call this shit

        return true;
    }

    std::unique_ptr<Netlist> HDLParserBitstream::instantiate(const GateLibrary* gl)
    {
        UNUSED(gl);

        GateLibrary* gate_library = gate_library_manager::get_gate_library_by_name("XILINX_UNISIM.lib");
        if (gate_library == nullptr)
        {
            log_error("bitstream", "gate library not found");
            return nullptr;
        }

        std::unique_ptr<Netlist> netlist = hdl_parser_manager::parse(m_verilog_path, gate_library);
        if (netlist == nullptr)
        {
            log_error("bitstream", "netlist could not be parsed");
            return nullptr;
        }

        return netlist;
    }

}    // namespace hal
