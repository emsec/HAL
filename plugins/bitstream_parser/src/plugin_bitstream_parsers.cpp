#include "bitstream_parser/empty_hdl_parser.h"
#include "bitstream_parser/hdl_parser_bitstream.h"
#include "bitstream_parser/plugin_bitstream_parser.h"
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

    bool BitstreamParserPlugin::handle_pre_netlist_cli_call(ProgramArguments& args)
    {
        // TODO handle additional cli arguments and write to struct

        m_configuration.out_path     = args.get_parameter("--out_path");
        m_configuration.db_root_path = args.get_parameter("--db_root_path");
        m_configuration.part         = args.get_parameter("--part");

        m_configuration_complete = args.is_option_set("--out_path") && args.is_option_set("--db_root_path") && args.is_option_set("--part");

        if (!m_configuration_complete)
        {
            log_error(get_name(), "configuration incomplete! you need to supply values for all options of this plugin!");
        }
        else
        {
            log_info(get_name(), "configuration complete");
        }

        return m_configuration_complete;
    }

    std::unique_ptr<HDLParser> BitstreamParserPlugin::create_parser()
    {
        if (!m_configuration_complete)
        {
            log_error(get_name(), "cannot create parser from incomplete configuration!");
            return std::make_unique<EmptyHDLParser>();
        }
        return std::make_unique<HDLParserBitstream>(m_configuration);
    }

    void BitstreamParserPlugin::on_load()
    {
        // TODO add data struct to HDLParserBitstream constructor
        hdl_parser_manager::register_parser("Default Bitstream Parser", std::bind(&BitstreamParserPlugin::create_parser, this), {".bit"});
    }

    void BitstreamParserPlugin::on_unload()
    {
        hdl_parser_manager::unregister_parser("Default Bitstream Parser");
    }
}    // namespace hal
