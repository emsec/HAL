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

#include "core/program_options.h"
#include "def.h"
#include "netlist/gate_library/gate_library.h"

namespace hal
{
    /* forward declaration */
    class Netlist;
    class GateLibrary;

    /**
     * @file
     */

    /**
     * \namespace netlist_factory
     * @ingroup netlist
     */
    namespace netlist_factory
    {
        /**
         * Creates a new netlist for a specific gate library.
         *
         * @param[in] gate_library - The underlying gate library.
         * @returns The new netlist.
         */
        NETLIST_API std::unique_ptr<Netlist> create_netlist(const GateLibrary* gate_library);

        /**
         * Creates a new netlist for a specific file.
         *
         * @param[in] hdl_file - Path to the HDL file.
         * @param[in] gate_library_file - Path to the gate library file.
         * @returns The new netlist.
         */
        NETLIST_API std::unique_ptr<Netlist> load_netlist(const std::filesystem::path& hdl_file, const std::filesystem::path& gate_library_file);

        /**
         * Creates a new netlist for a specific '.hal' file.
         *
         * @param[in] hal_file - Name of the '.hal' file.
         * file.
         * @returns The new netlist.
         */
        NETLIST_API std::unique_ptr<Netlist> load_netlist(const std::filesystem::path& hal_file);

        /**
         * Creates a new netlist entirely from program options.
         * Invokes parsers or serializers as needed.<br>
         *
         * @param[in] args - Command line options.
         * @returns The new netlist.
         */
        NETLIST_API std::unique_ptr<Netlist> load_netlist(const ProgramArguments& args);

        /**
          * Creates a new netlist from an HDL file for each matching pre-loaded gate library.
          *
          * @param[in] hdl_file - Path to the HDL file.
          * @returns A vector of netlists.
          */
        NETLIST_API std::vector<std::unique_ptr<Netlist>> load_netlists(const std::filesystem::path& hdl_file);
    }    // namespace netlist_factory
}    // namespace hal
