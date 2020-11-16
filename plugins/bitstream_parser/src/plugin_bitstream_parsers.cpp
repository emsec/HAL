#include "bitstream_parser/plugin_bitstream_parser.h"

#include "bitstream_parser/hdl_parser_bitstream.h"

#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"

namespace hal
{
    //TODO add struct for data

    extern std::unique_ptr<BasePluginInterface> create_plugin_instance()
    {
        return std::make_unique<BitstreamParserPlugin>();
    }

    std::string BitstreamParserPlugin::get_name() const
    {
        return std::string("plugin_bitstream_parser");
    }

    std::string BitstreamParserPlugin::get_version() const
    {
        return std::string("0.1");
    }

    void BitstreamParserPlugin::on_load()
    {
        // TODO add data struct to HDLParserBitstream constructor
        hdl_parser_manager::register_parser("Default Bitstream Parser", [](){return std::make_unique<HDLParserBitstream>();}, {".bit"});    
    }

    void BitstreamParserPlugin::on_unload()
    {
        hdl_parser_manager::unregister_parser("Default Bitstream Parser");
    }

    ProgramOptions BitstreamParserPlugin::get_cli_options() const
    {
        ProgramOptions description;
        description.add("--out_path", "provide path to output directory", {""});
        description.add("--db_root_path", "provide path to prjxray database for part", {""});
        description.add("--part", "FPGA part", {""});
        return description;
    }

    bool BitstreamParserPlugin::handle_cli_call(Netlist* nl, ProgramArguments& args)
    {
        UNUSED(nl);
        UNUSED(args);
        return true;
    }

    bool BitstreamParserPlugin::handle_pre_netlist_cli_call(ProgramArguments& args){
        UNUSED(args);
        // TODO handle additional cli arguments and write to struct
        return true;
    }
}    // namespace hal
