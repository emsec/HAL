#include "bitstream_parser/plugin_bitstream_parser.h"

#include "bitstream_parser/hdl_parser_bitstream.h"
#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"
#include "hal_core/utilities/log.h"

namespace hal
{
    extern std::unique_ptr<BasePluginInterface> create_plugin_instance()
    {
        return std::make_unique<BitstreamParserPlugin>();
    }

    std::string BitstreamParserPlugin::get_name() const
    {
        return std::string("bitstream_parser");
    }

    std::string BitstreamParserPlugin::get_version() const
    {
        return std::string("0.1");
    }

    void BitstreamParserPlugin::on_load()
    {
    }

    void BitstreamParserPlugin::on_unload()
    {
        hdl_parser_manager::unregister_parser("Default Bitstream Parser");
    }

    ProgramOptions BitstreamParserPlugin::get_cli_options() const
    {
        ProgramOptions description;
        description.add("--db_root_path", "provide path to prjxray database for part", {""});
        description.add("--part", "FPGA part", {""});
        return description;
    }

    bool BitstreamParserPlugin::handle_cli_init(ProgramArguments& args)
    {
        // TODO read from command line
        // TODO major reqrite of plugin system: load should not be executed before this part has been
        std::filesystem::path output_path   = "";
        std::filesystem::path database_path = "";
        std::filesystem::path bitread_path  = "";

        HDLParserBitstream parser(output_path, database_path, bitread_path, "");

        hdl_parser_manager::register_parser("Default Bitstream Parser", [parser]() { return std::make_unique<HDLParserBitstream>(parser); }, {".bit"});

        return true;
    }

    bool BitstreamParserPlugin::handle_cli_call(Netlist* nl, ProgramArguments& args)
    {
        UNUSED(nl);
        UNUSED(args);
        return true;
    }
}    // namespace hal
