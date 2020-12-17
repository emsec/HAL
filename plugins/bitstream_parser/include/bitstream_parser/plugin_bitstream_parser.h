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

        /**
         * Return command line interface options.
         *
         * @returns The program options.
         */
        ProgramOptions get_cli_options() const override;

        /**
         * Pass command line arguments for the initialization of the plugin.
         *
         * @param[in] args - Program arguments.
         * @returns True on success, false otherwise.
         */
        bool handle_cli_init(ProgramArguments& args) override;

        /**
         * Entry point to handle a command line interface call.
         *
         * @param[in] netlist - The netlist.
         * @param[in] args - Program arguments.
         * @returns True on success, false otherwise.
         */
        bool handle_cli_call(Netlist* nl, ProgramArguments& args) override;
    };
}    // namespace hal
