//  MIT License
//
//  Copyright (c) 2019 Ruhr-University Bochum, Germany, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "hal_core/defines.h"
#include "hal_core/netlist/hdl_parser/hdl_parser.h"
#include "hal_core/netlist/netlist.h"

#include <filesystem>

namespace hal
{
    /**
     * @ingroup hdl_parsers
     */
    class HDL_PARSER_API HDLParserBitstream : public HDLParser
    {
    public:
        HDLParserBitstream(std::filesystem::path output_path, std::filesystem::path database_path, std::filesystem::path bitread_path, const std::string& part);
        ~HDLParserBitstream() = default;

        /**
         * Parses a Xilinx 7-series bitstream file into a netlist and then into HAL.
         *
         * @param[in] file_name - The file to parse.
         * @returns True on success, false otherwise.
         */
        bool parse(const std::filesystem::path& file_name) override;

        std::unique_ptr<Netlist> instantiate(const GateLibrary* gl) override;

    private:
        std::filesystem::path m_output_path;
        std::filesystem::path m_database_path;
        std::filesystem::path m_bitread_path;
        std::filesystem::path m_verilog_path;
        std::string m_part;
        std::string m_io_standard = "LVCMOS33";
        std::string m_io_drive    = "16";
    };
}    // namespace hal
