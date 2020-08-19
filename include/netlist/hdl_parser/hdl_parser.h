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

#include "def.h"
#include <sstream>

namespace hal
{
    /* forward declaration*/
    class Netlist;
    class GateLibrary;

    /**
     * @ingroup hdl_parsers
     */
    class HDL_PARSER_API HDLParser
    {
    public:
        HDLParser() = default;
        virtual ~HDLParser() = default;

        /**
         * Parses a netlist into an internal intermediate format.
         *
         * @param[in] stream - The string stream filled with the hdl code.
         * @returns True on success, false otherwise.
         */
        virtual bool parse(std::stringstream& stream) = 0;

        /**
         * Instantiates the parsed netlist using a specific gate library.
         *
         * @param[in] gl - The gate library.
         * @returns A pointer to the resulting netlist.
         */
        virtual std::unique_ptr<Netlist> instantiate(const GateLibrary* gl) = 0;


        /**
         * Parses and instantiates a netlist using the specified gate library.
         *
         * @param[in] stream - The string stream filled with the hdl code.
         * @param[in] gl - The gate library.
         * @returns A pointer to the resulting netlist.
         */
        std::unique_ptr<Netlist> parse_and_instantiate(std::stringstream& stream, const GateLibrary* gl)
        {
            if (parse(stream))
            {
                return instantiate(gl);
            }
            return nullptr;
        }

    };
}    // namespace hal
