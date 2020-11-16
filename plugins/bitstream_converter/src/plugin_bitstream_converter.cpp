#include "bitstream_converter/plugin_bitstream_converter.h"

#include <fstream>
#include <vector>

#include "hal_core/netlist/gate_library/gate_library.h"
#include "hal_core/netlist/gate_library/gate_library_manager.h"
#include "hal_core/netlist/hdl_parser/hdl_parser.h"
#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"
#include "hal_core/netlist/netlist.h"

namespace hal
{

    extern std::unique_ptr<BasePluginInterface> create_plugin_instance()
    {
        return std::make_unique<BitstreamConverterPlugin>();
    }

    std::string BitstreamConverterPlugin::get_name() const
    {
        return std::string("bitstream_converter");
    }

    std::string BitstreamConverterPlugin::get_version() const
    {
        return std::string("0.1");
    }

    ProgramOptions BitstreamConverterPlugin::get_cli_options() const
    {
        ProgramOptions description;

        description.add("--bitstream", "executes the bitstream conversion plugin");

        description.add("--bit_path", "provide path to input bitstream file", {""});

        description.add("--out_path", "provide path to output directory", {""});

        description.add("--db_root_path", "provide path to prjxray database for part", {""});

        description.add("--part", "FPGA part", {""});

        

        return description;
    }

    bool BitstreamConverterPlugin::handle_pre_netlist_cli_call(ProgramArguments& args)
    {
        UNUSED(args);
        return true;
    }

    bool BitstreamConverterPlugin::handle_cli_call(Netlist* nl, ProgramArguments& args)
    {
        //UNUSED(args);
        UNUSED(nl);

        std::string bit_path;
        std::string out_path;

        if (args.is_option_set("--input-file")){
            log_info("bitstream", "Input file: {}", args.get_parameter("--bit_path"));
        }

        if (args.is_option_set("--bit_path"))
        {
            bit_path = args.get_parameter("--bit_path");
        }
        else
        {
            log_error("bitstream", "bitstream path parameter not set");
        }
        if (args.is_option_set("--out_path"))
        {
            if (args.get_parameter("--out_path").back() == '/')
                out_path = args.get_parameter("--out_path");
            else
                out_path = args.get_parameter("--out_path") + "/";
        }
        else
        {
            log_error("bitstream", "output path parameter not set");
        }

        std::string db_root = "/home/sinanboecker/Dokumente/Workspace/symbiflow-xc-fasm2bels/third_party/prjxray/database";
        std::string bitread = "/home/sinanboecker/Dokumente/Workspace/symbiflow-xc-fasm2bels/third_party/prjxray/build/tools/bitread";
        std::string db_path = out_path + "connection.db";
        std::string fasm_path = out_path + "out.fasm";
        std::string v_path = out_path + "out.v";
        std::string xdc_path = out_path + "out.xdc";
        std::string device = "";


        // get part from bitstream
        // open the file:
        std::ifstream file(bit_path, std::ios::binary);

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
        std::string cmd = "python3 -mfasm2bels --connection_database " + db_path + " --db_root " + db_root + "/" + device + " --part " + bin_part + " --fasm_file " + fasm_path + " --bit_file " + bit_path + " --bitread " + bitread + " --verilog_file " + v_path + " --xdc_file " + xdc_path + " --iostandard LVCMOS33 --drive 16";
        log_info("bitstream", "Command: {}", cmd);


        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        log_info("bitstream", "{}", result);

        auto gate_library = gate_library_manager::get_gate_library("XILINX_UNISIM.lib"); // insert correct gate lib here
        if (gate_library == nullptr)
        {
            log_error("bitstream", "gate library not found");
            return false;
        }
        auto netlist = hdl_parser_manager::parse(v_path, gate_library);
        if (netlist == nullptr)
        {
            log_error("bitstream", "netlist could not be parsed");
            return false;
        }

        return true;
    }

    void BitstreamConverterPlugin::initialize()
    {

    }
}
