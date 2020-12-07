#include "bitstream_parser/hdl_parser_bitstream.h"

#include "hal_core/utilities/log.h"
#include "hal_core/utilities/utils.h"
#include "hal_core/netlist/gate_library/gate_library.h"
#include "hal_core/netlist/gate_library/gate_library_manager.h"
#include "hal_core/netlist/hdl_parser/hdl_parser.h"
#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"

#include <algorithm>
#include <iomanip>

namespace hal
{
    std::string dirnameOf(const std::string& fname)
    {
     size_t pos = fname.find_last_of("\\/");
     return (std::string::npos == pos)
         ? ""
         : fname.substr(0, pos);
    }   
    
    HDLParserBitstream::HDLParserBitstream(const Configuration& config)
    {
        m_config = config;
    }

    bool HDLParserBitstream::parse(const std::filesystem::path& file_name)
    {
        m_config.out_path = dirnameOf(file_name);

        // TODO delete existing files

        // set necessary infos
        std::string db_path = m_config.out_path + "connection.db";
        std::string fasm_path = m_config.out_path + "out.fasm";
        m_v_path = m_config.out_path + "out.v";
        std::string xdc_path = m_config.out_path + "out.xdc";
        std::string device = "";


        // get part from bitstream
        // open the file:
        std::ifstream file(file_name, std::ios::binary);
        // Stop eating new lines in binary mode!!!
        file.unsetf(std::ios::skipws);
        // get its size:
        std::streampos fileSize;
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        // reserve capacity
        std::vector<char> vec;
        vec.reserve(fileSize);
        // read the data:
        vec.insert(vec.begin(),
                std::istream_iterator<char>(file),
                std::istream_iterator<char>());
        std::string bin_part = "";

        for (int i = 0x51; i<=0x5B; i++){
            bin_part = bin_part + vec[i];
        }

        log_info("bitstream", "Part from bin: {}", bin_part);

        // Artix7 
        if (bin_part.rfind("7a", 0) == 0) {
            device = "artix7";
            if (bin_part == "7a35tcpg236") {
                bin_part = "xc7a35tcpg236-1";
            }
            else if (bin_part == "7a35tcsg324") {
                bin_part = "xc7a35tcsg324-1";
            }
            else if (bin_part == "7a35tftg256") {
                bin_part = "xc7a35tftg256-1";
            }
            else if (bin_part == "7a50tfgg484") {
                bin_part = "xc7a50tfgg484-1";
            }
            else if (bin_part == "7a100tcsg324") {
                bin_part = "xc7a100tcsg324-1";
            }
            else if (bin_part == "7a100tfgg676") {
                bin_part = "xc7a100tfgg676-1";
            }
            else if (bin_part == "7a200tffg1156") {
                bin_part = "xc7a200tffg1156-1";
            }
            else if (bin_part == "7a200tsbg484") {
                bin_part = "xc7a200tsbg484-1";
            }
        }

        // Kintex7
        else if (bin_part.rfind("7k", 0) == 0) {
            device = "kintex7"; 
            if (bin_part == "7k70tfbg676") {
                bin_part = "xc7k70tfbg676-2";
            }
        }

        // Zynq7 
        else if (bin_part.rfind("7z", 0) == 0) {
            device = "zynq7"; 
            if (bin_part == "7z010clg400") {
                bin_part = "xc7z010clg400-1";
            }
            else if (bin_part == "7z020clg484") {
                bin_part = "xc7z020clg484-1";
            }
        } else {
            log_error("bitstream", "Unknown device {}", bin_part);
            return false;
        }

        // std::string cmd = "python3 -mfasm2bels --connection_database data/connection.db --db_root third_party/prjxray-db/artix7/ --part xc7a35tcpg236-1 --fasm_file data/out.fasm --bit_file data/EL1BEG2.bit --bitread third_party/prjxray/build/tools/bitread --verilog_file data/out.v --xdc_file data/out.xdc --iostandard LVCMOS33 --drive 16";
        std::string cmd = "python3 -mfasm2bels --connection_database " + db_path + " --db_root " + m_config.db_root_path + "/" + device + " --part " + bin_part + " --fasm_file " + fasm_path + " --bit_file " + file_name.string() + " --bitread " + m_config.bitread_path + " --verilog_file " + m_v_path + " --xdc_file " + xdc_path + " --iostandard LVCMOS33 --drive 16";
        log_info("bitstream", "Command: {}", cmd);

        if (std::filesystem::exists(m_v_path)) {
            return true;   
        } else {
            return false;
        }
    }

    std::unique_ptr<Netlist> HDLParserBitstream::instantiate(const GateLibrary* gl){
        UNUSED(gl);
        auto gate_library = gate_library_manager::get_gate_library("XILINX_UNISIM.lib"); // insert correct gate lib here
        if (gate_library == nullptr)
        {
            log_error("bitstream", "gate library not found");
            return nullptr;        
        }
        auto netlist = hdl_parser_manager::parse(m_v_path, gate_library);
        if (netlist == nullptr)
        {
            log_error("bitstream", "netlist could not be parsed");
            return nullptr;  
        }

        return netlist;
    }

}    // namespace hal
