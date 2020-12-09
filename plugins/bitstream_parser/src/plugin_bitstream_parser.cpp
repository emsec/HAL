#include "bitstream_parser/plugin_bitstream_parser.h"

#include "bitstream_parser/hdl_parser_bitstream.h"
#include "hal_core/netlist/hdl_parser/hdl_parser_manager.h"

namespace hal
{
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
        if (!m_configuration_complete)
        {
            m_configuration = {"", "", "", ""};
        }
        hdl_parser_manager::register_parser("Default Bitstream Parser", [this]() { return std::make_unique<HDLParserBitstream>(m_configuration); }, {".bit"});
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

    bool BitstreamParserPlugin::handle_cli_call(Netlist* nl, ProgramArguments& args)
    {
        UNUSED(nl);
        UNUSED(args);
        return true;
    }

    bool BitstreamParserPlugin::handle_pre_netlist_cli_call(ProgramArguments& args)
    {
        UNUSED(args);
        m_configuration.db_root_path = "/home/sinanboecker/Dokumente/Workspace/symbiflow-xc-fasm2bels/third_party/prjxray/database";
        m_configuration.bitread_path = "/home/sinanboecker/Dokumente/Workspace/symbiflow-xc-fasm2bels/third_party/prjxray/build/tools/bitread";

        m_configuration_complete = true;
        log_info(get_name(), "Pre netlist generation CLI handling.");

        return true;
    }
}    // namespace hal
