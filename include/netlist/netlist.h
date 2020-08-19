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
#include "netlist/gate_library/gate_library.h"

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace hal
{
    /** forward declaration */
    class NetlistInternalManager;
    class Net;
    class Gate;
    class Module;
    class Endpoint;

    /**
     * Netlist class containing information about the netlist including its gates, modules, and nets, as well as the underlying gate library.
     *
     * @ingroup netlist
     */
    class NETLIST_API Netlist
    {
        friend class NetlistInternalManager;

    public:
        /**
         * Constructs a new netlist for a specific gate library.<br>
         * Use the netlist_factory to create instances!
         *
         * @param[in] library - Shared pointer to gate library.
         */
        explicit Netlist(const GateLibrary* library);

        ~Netlist();

        /**
         * Get the netlist id.<br>
         * If not changed via set_id(), the id is zero.
         *
         * @returns The netlist's id.
         */
        u32 get_id() const;

        /**
         * Sets the netlist id to a new value.
         *
         * @param[in] id - The new netlist id.
         */
        void set_id(const u32 id);

        /**
         * Get the file name of the input design.
         *
         * @returns The input file's name.
         */
        std::filesystem::path get_input_filename() const;

        /**
         * Set the file name of the input design.
         *
         * @param[in] input_filename - File name of the design.
         */
        void set_input_filename(const std::filesystem::path& input_filename);

        /**
         * Get the design name,
         *
         * @returns The design name.
         */
        std::string get_design_name() const;

        /**
         * Set the design name
         *
         * @param[in] design_name - New design name.
         */
        void set_design_name(const std::string& design_name);

        /**
         * Get the name of the hardware device if specified.
         *
         * @return The target device name.
         */
        std::string get_device_name() const;

        /**
         * Set the name of the target hardware device.
         *
         * @param[in] device_name - Name of hardware device
         */
        void set_device_name(const std::string& device_name);

        /**
         * Get the gate library associated with the netlist.
         *
         * @returns A pointer to the gate library.
         */
        const GateLibrary* get_gate_library() const;

        /**
         * Clears internal caches of the netlist.
         * In a typical application calling this function is not required.
         */
        void clear_caches();

        /*
         * ################################################################
         *      module functions
         * ################################################################
         */

        /**
         * Gets an unoccupied module id.<br>
         * The value 0 is reserved and represents an invalid id.
         *
         * @returns An unoccupied unique id.
         */
        u32 get_unique_module_id();

        /**
         * Creates and adds a new module to the netlist.<br>
         * It is identifiable via its unique ID.
         *
         * @param[in] id - The unique ID != 0 for the new module.
         * @param[in] name - A name for the module.
         * @param[in] parent - The parent module.
         * @param[in] gates - Gates to assign to the new gate.
         * @returns The new module on success, nullptr on error.
         */
        Module* create_module(const u32 id, const std::string& name, Module* parent, const std::vector<Gate*>& gates = {});

        /**
         * Creates and adds a new module to the netlist.<br>
         * It is identifiable via its unique ID which is automatically set to the next free ID.
         *
         * @param[in] name - A name for the module.
         * @param[in] parent - The parent module.
         * @param[in] gates - Gates to assign to the new gate.
         * @returns The new module on success, nullptr on error.
         */
        Module* create_module(const std::string& name, Module* parent, const std::vector<Gate*>& gates = {});

        /**
         * Removes and a module from the netlist.
         *
         * @param[in] module - module to remove.
         * @returns True on success.
         */
        bool delete_module(Module* module);

        /**
         * Get the top module of the netlist.
         *
         * @returns The top module.
         */
        Module* get_top_module() const;

        /**
         * Get a module by its ID. <br>
         * It does not matter at which depth the module is located.
         *
         * @param[in] id - the id of the desired module
         * @returns The desired module.
         */
        Module* get_module_by_id(u32 id) const;

        /**
         * Get a set of all modules of the netlist including the top module.
         *
         * @returns The modules of the netlist.
         */
        std::set<Module*> get_modules() const;

        /**
         * Checks whether a module is registered in the netlist.
         *
         * @param[in] module - The module to check.
         * @returns True if the module is in netlist
         */
        bool is_module_in_netlist(Module* module) const;

        /*
         * ################################################################
         *      gate functions
         * ################################################################
         */

        /**
         * Gets an unoccupied gate id. <br>
         * The value 0 is reserved and represents an invalid id.
         *
         * @returns An unoccupied unique id.
         */
        u32 get_unique_gate_id();

        /**
         * Creates and adds a new gate to the netlist.<br>
         * It is identifiable via its unique ID.
         *
         * @param[in] id - The unique ID != 0 for the new gate.
         * @param[in] gt - The gate type.
         * @param[in] name - A name for the gate.
         * @param[in] x - The x-coordinate of the gate.
         * @param[in] y - The y-coordinate of the gate.
         * @returns The new gate on success, nullptr on error.
         */
        Gate* create_gate(const u32 id, const GateType* gt, const std::string& name = "", float x = -1, float y = -1);

        /**
         * Creates and adds a new gate to the netlist.<br>
         * It is identifiable via its unique ID which is automatically set to the next free ID.
         *
         * @param[in] gt - The gate type.
         * @param[in] name - A name for the gate.
         * @param[in] x - The x-coordinate of the gate.
         * @param[in] y - The y-coordinate of the gate.
         * @returns The new gate on success, nullptr on error.
         */
        Gate* create_gate(const GateType* gt, const std::string& name = "", float x = -1, float y = -1);

        /**
         * Removes a gate from the netlist.
         *
         * @param[in] gate - Pointer to the gate pointer.
         * @returns True on success.
         */
        bool delete_gate(Gate* gate);

        /**
         * Checks whether a gate is registered in the netlist.
         *
         * @param[in] gate - The gate to check.
         * @returns True if the gate is in netlist
         */
        bool is_gate_in_netlist(Gate* gate) const;

        /**
         * Get a gate specified by id.
         *
         * @param[in] gate_id - The gate's id.
         * @returns The gate or a nullptr.
         */
        Gate* get_gate_by_id(const u32 gate_id) const;

        /**
         * Get all gates of the netlist regardless of the module they are in. <br>
         * You can filter the set before output with the optional parameters.
         *
         * @param[in] filter - Filter for the gates
         * @returns A set of gates.
         */
        std::set<Gate*> get_gates(const std::function<bool(Gate*)>& filter = nullptr) const;

        /**
         * Mark a gate as a global vcc gate.
         *
         * @param[in] gate - The gate.
         * @returns True on success.
         */
        bool mark_vcc_gate(Gate* gate);

        /**
         * Mark a gate as a global gnd gate.
         *
         * @param[in] gate - The negate.
         * @returns True on success.
         */
        bool mark_gnd_gate(Gate* gate);

        /**
         * Unmark a global vcc gate.
         *
         * @param[in] gate - The gate.
         * @returns True on success.
         */
        bool unmark_vcc_gate(Gate* gate);

        /**
         * Unmark a global gate.
         *
         * @param[in] gate - The new gate.
         * @returns True on success.
         */
        bool unmark_gnd_gate(Gate* gate);

        /**
         * Checks whether a gate is a global vcc gate.
         *
         * @param[in] gate - The gate to check.
         * @returns True if the gate is a global vcc gate.
         */
        bool is_vcc_gate(Gate* gate) const;

        /**
         * Checks whether a gate is a global gnd gate.
         *
         * @param[in] gate - The gate to check.
         * @returns True if the gate is a global gnd gate.
         */
        bool is_gnd_gate(Gate* gate) const;

        /**
         * Get all global vcc gates.
         *
         * @returns A set of gates.
         */
        std::set<Gate*> get_vcc_gates() const;

        /**
         * Get all global gnd gates.
         *
         * @returns A set of gates.
         */
        std::set<Gate*> get_gnd_gates() const;

        /*
         * ################################################################
         *      net functions
         * ################################################################
         */

        /**
         * Gets an unoccupied net id. <br>
         * The value 0 is reserved and represents an invalid id.
         *
         * @returns An unoccupied unique id.
         */
        u32 get_unique_net_id();

        /**
         * Creates and adds a new net to the netlist.<br>
         * It is identifiable via its unique ID.
         *
         * @param[in] id - The unique ID != 0 for the new net.
         * @param[in] name - A name for the net.
         * @returns The new net on success, nullptr on error.
         */
        Net* create_net(const u32 id, const std::string& name = "");

        /**
         * Creates and adds a new net to the netlist.<br>
         * It is identifiable via its unique ID which is automatically set to the next free ID.
         *
         * @param[in] name - A name for the net.
         * @returns The new net on success, nullptr on error.
         */
        Net* create_net(const std::string& name = "");

        /**
         * Removes a net from the netlist.
         *
         * @param[in] n - Pointer to the net pointer.
         * @returns True on success.
         */
        bool delete_net(Net* n);

        /**
         * Checks whether a net is registered in the netlist.
         *
         * @param[in] n - The net to check.
         * @returns True if the net is in netlist
         */
        bool is_net_in_netlist(Net* n) const;

        /**
         * Get a net specified by id.
         *
         * @param[in] id - The net's id.
         * @returns The net or a nullptr.
         */
        Net* get_net_by_id(u32 id) const;

        /**
         * Get all nets of the netlist regardless of the module they are in. <br>
         * You can filter the set before output with the optional parameter.
         *
         * @param[in] filter - Filter for the nets
         * @return A set of nets.
         */
        std::unordered_set<Net*> get_nets(const std::function<bool(Net*)>& filter = nullptr) const;

        /**
         * Mark a net as a global input net.
         *
         * @param[in] net - The net.
         * @returns True on success.
         */
        bool mark_global_input_net(Net* net);

        /**
         * Mark a net as a global output net.
         *
         * @param[in] net - The net.
         * @returns True on success.
         */
        bool mark_global_output_net(Net* net);

        /**
         * Unmark a global input net.
         *
         * @param[in] net - The net.
         * @returns True on success.
         */
        bool unmark_global_input_net(Net* net);

        /**
         * Unmark a global output net.
         *
         * @param[in] net - The net.
         * @returns True on success.
         */
        bool unmark_global_output_net(Net* net);

        /**
         * Checks whether a net is a global input net.
         *
         * @param[in] net - The net to check.
         * @returns True if the net is a global input net.
         */
        bool is_global_input_net(Net* net) const;

        /**
         * Checks whether a net is a global output net.
         *
         * @param[in] net - The net to check.
         * @returns True if the net is a global output net.
         */
        bool is_global_output_net(Net* net) const;

        /**
         * Get all global input nets.
         *
         * @returns A set of nets.
         */
        std::set<Net*> get_global_input_nets() const;

        /**
         * Get all global output nets.
         *
         * @returns A set of nets.
         */
        std::set<Net*> get_global_output_nets() const;

    private:
        /* stores the pointer to the netlist internal manager */
        NetlistInternalManager* m_manager;

        /* stores the gate library */
        const GateLibrary* m_gate_library;

        /* stores the netlist id */
        u32 m_netlist_id;

        /* stores the name of the input file */
        std::filesystem::path m_file_name;

        /* stores the name of the design */
        std::string m_design_name;

        /* stores the name of the device */
        std::string m_device_name;

        /* stores the auto generated ids for fast next id */
        u32 m_next_gate_id;
        std::set<u32> m_used_gate_ids;
        std::set<u32> m_free_gate_ids;
        u32 m_next_net_id;
        std::set<u32> m_used_net_ids;
        std::set<u32> m_free_net_ids;
        u32 m_next_module_id;
        std::set<u32> m_used_module_ids;
        std::set<u32> m_free_module_ids;

        /* stores the modules */
        Module* m_top_module;
        std::unordered_map<u32, std::unique_ptr<Module>> m_modules;

        /* stores the nets */
        std::unordered_map<u32, std::unique_ptr<Net>> m_nets_map;
        std::unordered_set<Net*> m_nets_set;

        /* stores the gates */
        std::unordered_map<u32, std::unique_ptr<Gate>> m_gates_map;
        std::unordered_set<Gate*> m_gates_set;

        /* stores the set of global gates and nets */
        std::set<Net*> m_global_input_nets;
        std::set<Net*> m_global_output_nets;
        std::set<Gate*> m_gnd_gates;
        std::set<Gate*> m_vcc_gates;
    };
}    // namespace hal
