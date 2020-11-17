#pragma once

#include "hal_core/netlist/hdl_parser/hdl_parser.h"

namespace hal
{
    class HDL_PARSER_API EmptyHDLParser : public HDLParser
    {
    public:
        EmptyHDLParser()  = default;
        ~EmptyHDLParser() = default;

        bool parse(const std::filesystem::path&) override
        {
            return false;
        }

        std::unique_ptr<Netlist> instantiate(const GateLibrary*) override
        {
            return nullptr;
        }
    };
}    // namespace hal
