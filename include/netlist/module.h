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
#include "netlist/data_container.h"
#include "netlist/gate_library/gate_library.h"

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>

namespace hal
{
    /** forward declaration */
    class Netlist;
    class NetlistInternalManager;
    class Net;
    class Gate;

    /**
     * Module class containing information about a module including its gates, submodules, and parent module.
     *
     * @ingroup module
     */
    class NETLIST_API Module : public DataContainer
    {
        friend class NetlistInternalManager;
        friend class Netlist;

    public:
        /**
         * Get the module's id.
         *
         * @returns The module's id.
         */
        u32 get_id() const;

        /**
         * Get the module's name.
         *
         * @returns The module's name.
         */
        std::string get_name() const;

        /**
         * Set the module's name.
         *
         * @params[in] name - The new name.
         */
        void set_name(const std::string& name);

        /**
         * Get the module's type.
         *
         * @returns The module's type.
         */
        std::string get_type() const;

        /**
         * Set the module's type.
         *
         * @params[in] type - The new type.
         */
        void set_type(const std::string& type);

        /**
         * Get the parent of this module.<br>
         * This returns nullptr for the top module.
         *
         * @returns The parent module.
         */
        Module* get_parent_module() const;

        /**
         * Sets a new parent for this module.<br>
         * If the new parent is a submodule of this module, the new parent is added as a direct submodule to the old parent first.
         *
         * @param[in] new_parent - the new parent module
         * @returns True if the parent was changed
         */
        bool set_parent_module(Module* new_parent);

        /**
         * Get all direct submodules of this module.<br>
         * If \p recursive is true, all indirect submodules are also included.
         *
         * @param[in] filter - Filter for the modules
         * @param[in] recursive - Look into submodules as well
         * @returns The set of submodules
         */
        std::set<Module*> get_submodules(const std::function<bool(Module*)>& filter = nullptr, bool recursive = false) const;

        /**
         * Checks whether another module is a submodule of this module.<br>
         * If \p recursive is true, all indirect submodules are also included.
         *
         * @param[in] other - Other module to check
         * @param[in] recursive - Look into submodules as well
         * @returns True if the other module is a submodule
         */
        bool contains_module(Module* other, bool recursive = false) const;

        /**
         * Get the netlist this module is associated with.
         *
         * @returns The netlist.
         */
        Netlist* get_netlist() const;

        /**
         * Get the input nets to this module.<br>
         * A module input net is either a global input to the netlist or has a source outside of the module.
         *
         * @returns The set of module input nets.
         */
        std::vector<Net*> get_input_nets() const;

        /**
         * Get the output nets of this module.<br>
         * A module output net is either a global output of the netlist or has a destination outside of the module.
         *
         * @returns The set of module output nets.
         */
        std::vector<Net*> get_output_nets() const;

        /**
         * Get the internal nets of this module.<br>
         * A net is internal if its source and at least one output are inside the module.<br>
         * Therefore it may contain some nets that are also regarded as output nets.
         *
         * @returns The set of module input nets.
         */
        std::vector<Net*> get_internal_nets() const;

        /**
         * Set the name of the port corresponding to the specified input net to the given string.
         *
         * @param[in] input_net - The input net.
         * @param[in] port_name - The port name.
         */
        void set_input_port_name(Net* input_net, const std::string& port_name);

        /**
         * Set the name of the port corresponding to the specified output net to the given string.
         *
         * @param[in] output_net - The output net.
         * @param[in] port_name - The port name.
         */
        void set_output_port_name(Net* output_net, const std::string& port_name);

        /**
         * Get the name of the port corresponding to the specified input net.
         *
         * @param[in] net - The input net.
         * @returns The port name.
         */
        std::string get_input_port_name(Net* net);

        /**
         * Get the name of the port corresponding to the specified output net.
         *
         * @param[in] net - The output net.
         * @returns The port name.
         */
        std::string get_output_port_name(Net* net);

        /**
         * Get the input net of the port corresponding to the specified port name.
         *
         * @param[in] port_name - The input port name.
         * @returns The input net.
         */
        Net* get_input_port_net(const std::string& port_name);

        /**
         * Get the output net of the port corresponding to the specified port name.
         *
         * @param[in] port_name - The output port name.
         * @returns The output net.
         */
        Net* get_output_port_net(const std::string& port_name);

        /**
         * Get the mapping of all input nets to their corresponding port names.
         *
         * @returns The map from input net to port name.
         */
        const std::map<Net*, std::string>& get_input_port_names() const;

        /**
         * Get the mapping of all output nets to their corresponding port names.
         *
         * @returns The map from output net to port name.
         */
        const std::map<Net*, std::string>& get_output_port_names() const;

        /*
         * ################################################################
         *      gate functions
         * ################################################################
         */

        /**
         * Moves a gate into this module.<br>
         * The gate is removed from its previous module in the process.
         *
         * @param[in] gate - The gate to move.
         * @returns True on success.
         */
        bool assign_gate(Gate* gate);

        /**
         * Removes a gate from the module.<br>
         * It is automatically moved to the netlist's top module.
         *
         * @param[in] gate - Pointer to the gate pointer.
         * @returns True on success.
         */
        bool remove_gate(Gate* gate);

        /**
         * Checks whether a gate is in the module.<br>
         * If \p recursive is true, all submodules are searched as well.
         *
         * @param[in] gate - The gate to check.
         * @param[in] recursive - Look into submodules too
         * @returns True if the gate is in module
         */
        bool contains_gate(Gate* gate, bool recursive = false) const;

        /**
         * Get a gate specified by id.<br>
         * If \p recursive is true, all submodules are searched as well.
         *
         * @param[in] id - The gate's id.
         * @param[in] recursive - Look into submodules too
         * @returns The gate or a nullptr.
         */
        Gate* get_gate_by_id(const u32 id, bool recursive = false) const;

        /**
         * Get all gates of the module. <br>
         * You can filter the set before output with the optional parameters.<br>
         * If \p recursive is true, all submodules are searched as well.
         *
         * @param[in] filter - Filter for the returned gates
         * @param[in] recursive - Look into submodules too
         * @return A set of gates.
         */
        std::set<Gate*> get_gates(const std::function<bool(Gate*)>& filter = nullptr, bool recursive = false) const;

    private:
        Module(u32 id, Module* parent, const std::string& name, NetlistInternalManager* internal_manager);

        Module(const Module&) = delete;               //disable copy-constructor
        Module& operator=(const Module&) = delete;    //disable copy-assignment

        std::string m_name;
        std::string m_type;

        NetlistInternalManager* m_internal_manager;
        u32 m_id;

        Module* m_parent;
        std::map<u32, Module*> m_submodules_map;
        std::set<Module*> m_submodules_set;

        /* port names */
        mutable u32 m_next_input_port_id  = 0;
        mutable u32 m_next_output_port_id = 0;
        mutable std::set<Net*> m_named_input_nets;
        mutable std::set<Net*> m_named_output_nets;
        mutable std::map<Net*, std::string> m_input_net_to_port_name;
        mutable std::map<Net*, std::string> m_output_net_to_port_name;

        /* stores gates sorted by id */
        std::map<u32, Gate*> m_gates_map;
        std::set<Gate*> m_gates_set;

        mutable bool m_input_nets_dirty;
        mutable std::vector<Net*> m_input_nets;
        mutable bool m_output_nets_dirty;
        mutable std::vector<Net*> m_output_nets;
        mutable bool m_internal_nets_dirty;
        mutable std::vector<Net*> m_internal_nets;
    };
}    // namespace hal
