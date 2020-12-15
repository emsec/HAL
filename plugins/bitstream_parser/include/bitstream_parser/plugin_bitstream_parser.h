#pragma once

#include "bitstream_parser/hdl_parser_bitstream.h"
#include "hal_core/plugin_system/plugin_interface_cli.h"

namespace hal
{
    class PLUGIN_API BitstreamParserPlugin : public CLIPluginInterface
    {
    public:
        std::string get_name() const override;
        std::string get_version() const override;

        void on_load() override;
        void on_unload() override;

        /** interface implementation: i_cli */
        ProgramOptions get_cli_options() const override;

        /** interface implementation: i_cli */
        bool handle_pre_netlist_cli_call(ProgramArguments& args) override;

        /** interface implementation: i_cli */
        bool handle_cli_call(Netlist* nl, ProgramArguments& args) override;

    private:
        HDLParserBitstream::Configuration m_configuration;
        bool m_configuration_complete = false;
        std::unique_ptr<HDLParser> create_parser();
    };
}    // namespace hal
