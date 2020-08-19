#pragma once

#include "core/log.h"
#include "netlist/boolean_function.h"
#include "netlist/endpoint.h"
#include "netlist/gate.h"
#include "netlist/gate_library/gate_library.h"
#include "netlist/gate_library/gate_library_manager.h"
#include "netlist/module.h"
#include "netlist/net.h"
#include "netlist/netlist.h"
#include "test_def.h"

#include <core/utils.h>
#include <fstream>
#include <math.h>

/*
namespace hal
{


}    // namespace test_utils
*/
namespace hal
{
    namespace test_utils
    {
        /*********************************************************
         *                      Constants                        *
         *********************************************************/

        // Minimum and invali ids for netlists, gates, nets and modules
        const u32 INVALID_GATE_ID   = 0;
        const u32 INVALID_NET_ID    = 0;
        const u32 INVALID_MODULE_ID = 0;

        const u32 MIN_MODULE_ID  = 2;
        const u32 MIN_GATE_ID    = 1;
        const u32 MIN_NET_ID     = 1;
        const u32 MIN_NETLIST_ID = 1;
        const u32 TOP_MODULE_ID  = 1;

        // Name for accessing our example Gate library
        static const std::string g_lib_name = "EXAMPLE_GATE_LIBRARY.lib";
        //static const std::string g_lib_name = "example_library";
        // Name for accessing the custom Gate library after the call of 'create_temp_gate_lib()'
        const std::string temp_lib_name = "TEMP_GATE_LIBRARY.lib";

        const std::filesystem::path sandbox_directory_path = std::filesystem::path("tests/sandbox_directory");

        /*********************************************************
         *                      Functions                        *
         *********************************************************/
        /**
         * Creates an empty netlist.
         *
         * @param[in] id - Id of the netlist
         * @returns An empty netlist
         */
        std::unique_ptr<hal::Netlist> create_empty_netlist(const int id = -1);
        /**
         * Initializes all log channels used by hal.
         */
        void init_log_channels();

        /**
         * Creating an Endpoint object by passing the netlist, the id of the Gate and the pin type.
         * if there is no Gate with the passed id, it returns (nullptr, "")
         *
         * @param[in] nl - netlist
         * @param[in] gate_id - id of the Gate
         * @param[in] pin_type - pin type
         * @param[in] is_destination - direction of Endpoint
         * @returns the Endpoint object
         */
        Endpoint get_endpoint(Netlist* nl, const int gate_id, const std::string& pin_type, bool is_destination);

        /**
         * Creating an Endpoint object by passing the Gate and the pin_type. The is_destination flag is taken from the
         * Gate library of the Gate and the netlist by the Gate.
         *
         * @param[in] gate_id - id of the Gate
         * @param[in] pin_type - pin type
         * @returns the Endpoint object
         */
        Endpoint get_endpoint(Gate* g, const std::string& pin_type);

        /**
         * Checks if an Endpoint is empty (i.e. (nullptr, ""))
         *
         * @param[in] ep - Endpoint
         * @return true, if the Endpoint is the empty Endpoint
         */
        bool is_empty(const Endpoint& ep);

        /**
         * Minimizes a truth table of a boolean function such that variables that do not matter are eliminated.
         * E.g: {0,0,1,1,1,1,0,0} becomes {0,1,1,0} (the first variable is eliminated)
         *
         * This function is used to compare optimized functions with non-optimized ones
         *
         * @param tt - the truth table to minimize
         * @returns the minimized truth table
         */
        std::vector<BooleanFunction::Value> minimize_truth_table(const std::vector<BooleanFunction::Value> tt);

        /**
         * Get a Gate type by its name
         *
         * @param name - the name of the GateType
         * @param gate_library_name - the name of the Gate library, the GateType can be found in. If empty, the example Gate library (g_lib_name) is taken.
         * @return the GateType pointer if found. If no Gate type matches, return nullptr
         */
        const GateType* get_gate_type_by_name(std::string name, GateLibrary* GateLibrary = nullptr);

        /**
         * Given a vector of endpoints. Returns the first Endpoint that has a certain pin type
         *
         * @param[in] dsts - vector of endpoints
         * @param[in] pin_type - pin type
         * @returns the first Endpoint of a certain pin type. (nullptr, "") if no Endpoint matches.
         */
        Endpoint get_destination_by_pin_type(const std::vector<Endpoint> dsts, const std::string pin_type);

        // NOTE: Using create_test_gate is messy. It should not exist. Will be removed someday...
        /**
         * Create a test Gate with 3 input pins
         *
         * @param nl - the netlist, the test Gate is created in
         * @param[in] id - id of the Gate
         * @returns an already created AND3 Gate
         */
        Gate* create_test_gate(Netlist* nl, const u32 id);
        /**
         * Checks if two vectors have the same content regardless of their order. Shouldn't be used for
         * large vectors, since it isn't really efficient.
         *
         * @param[in] vec_1 - first vector
         * @param[in] vec_2 - second vector
         * @returns TRUE if both vectors have the same content. FALSE otherwise
         */
        template<typename T>
        bool vectors_have_same_content(const std::vector<T> vector_1, const std::vector<T> vector_2)
        {
            std::vector<T> vec_1(vector_1);
            std::vector<T> vec_2(vector_2);

            if (vec_1.size() != vec_2.size())
            {
                return false;
            }

            // Each element of vec_1 must be found in vec_2
            while (vec_1.size() > 0)
            {
                auto it_1       = vec_1.begin();
                bool found_elem = false;
                for (auto it_2 = vec_2.begin(); it_2 != vec_2.end(); it_2++)
                {
                    if (*it_1 == *it_2)
                    {
                        found_elem = true;
                        vec_2.erase(it_2);
                        break;
                    }
                }
                if (!found_elem)
                {
                    return false;
                }
                vec_1.erase(it_1);
            }

            return true;
        }

        /**
         * Checks if a string contains a substring
         *
         * @param[in] str - string to search in
         * @param[in] sub_str - the substring searched for
         * @returns TRUE if str contains sub_str. FALSE otherwise.
         */
        bool string_contains_substring(const std::string str, const std::string sub_str);

        /**
         * Get the pointer of a Net, which name contains a certain substring. There must be only one name with this subname.
         *
         * @param nl - netlist
         * @param subname - substring of the Net name
         * @returns the Net pointer if there is exactly one Net with the subname. Returns nullptr otherwise.
         */
        Net* get_net_by_subname(Netlist* nl, const std::string subname);

        /**
         * Get the pointer of a Gate, which name contains a certain substring. There must be only one name with this subname.
         *
         * @param nl - netlist
         * @param subname - substring of the Net name
         * @returns the Gate pointer if there is exactly one Gate with the subname. Returns nullptr otherwise.
         */
        Gate* get_gate_by_subname(Netlist* nl, const std::string subname);

        // ===== File Management =====
        /**
         * Create a sandbox directory within the build folder, where temporary files can be stored. Please use the function
         * remove_sandbox_directory() at the end of the test in order to remove the directory.
         *
         * @returns the absolute path of the sandbox directory
         */
        std::filesystem::path create_sandbox_directory();

        /**
         * Remove the sandbox directory if it was created before
         */
        void remove_sandbox_directory();

        /**
         * Creates a file path to a file in the sandbox directory. Note that this function can only called, if the sandbox
         * directory exists. (Use create_sandbox_directory() to create it)
         *
         * @param file_name - The name of the file (including its extension)
         * @returns the absolute path of the file.
         */
        std::filesystem::path create_sandbox_path(const std::string file_name);

        /**
         * Creates a file with a given content within the sandbox directory. Note that this function can only called
         * if the sandbox directory exists. (Use create_sandbox_directory() to create it)
         *
         * @param file_name - The name of the file (including its extension)
         * @param content - The content of the file
         * @returns the absolute path of the file
         */
        std::filesystem::path create_sandbox_file(std::string file_name, std::string content);

        /**
         * Creates a Gate library dedicated solely to testing. Construction of that Gate library is independent of the Gate library parser.
         */
        GateLibrary* get_testing_gate_library();

        // ===== Example Netlists =====

        /*
        *      example_netlist
        *
        *
        *      gnd (1) =--= gate_1_to_1 (3) =--=                  .------= gate_1_to_1 (4) =
        *                                        gate_2_to_1 (0) =+
        *      vcc (2) =-----------------------=                  '------=
        *                                                                  gate_2_to_1 (5) =
        *                                                                =
        *
        *     =                       =                 =----------=
        *       gate_2_to_0 (6)         gate_2_to_1 (7)            ...  gate_2_to_1 (8) =
        *     =                       =                 =          =
        */
        /**
         * Creates the netlist shown in the diagram above. Sets a concrete id if passed.
         *
         * @param[in] id - id of the netlist
         * @returns the created netlist object
         */
        std::unique_ptr<Netlist> create_example_netlist(const int id = -1);

        /*
         *      example netlist II
         *
         *    =                    .-------=                 =
         *    =                    +-------=                 =
         *    = gate_4_to_1 (0) =--+-------= gate_4_to_1 (1) =
         *    =                    |    .--=                 =
         *                         |    |
         *    =                    |    |
         *    =                    |    |
         *    = gate_4_to_1 (2) =--~----'
         *    =                    '-------=
         *                                 =
         *                                 =  gate_4_to_1 (3) =
         *                                 =
         */
        /**
         * Creates the netlist shown in the diagram above. Sets a concrete id if passed.
         *
         * @param[in] id - id of the netlist
         * @returns the created netlist object
         */
        std::unique_ptr<Netlist> create_example_netlist_2(const int id = -1);

        /*
         *      example netlist negative
         *
         *        -----= INV (1) =-----
         *
         */
        /**
         * Creates the netlist shown in the diagram above. Sets a concrete id if passed.
         *
         * @param[in] id - id of the netlist
         * @returns the created netlist object
         */
        std::unique_ptr<Netlist> create_example_netlist_negative(const int id = -1);

        /*
          *      Example netlist circuit diagram (Id in brackets). Used for get fan in and
          *      out nets.
          *
          *
          *      GND (1) =-= INV (3) =--=             .------=  INV (4)  =---
          *                                 AND2 (0) =-
          *      VCC (2) =--------------=             '------=
          *                                                     AND2 (5) =---
          *                                                  =
          *
          *                           =                .-----=
          *                              OR2 (6)  =----'        OR2 (7)  =---
          *                           =                      =
          */
        // Creates a simple netlist shown in the diagram above. The nets that have a GND/VCC Gate as a source are named '0'/'1'
        /**
         * Creates the netlist shown in the diagram above.
         * The nets which are connected to a GND/VCC Gate are named '0'/'1' (necessary for some parser/writer tests).
         * Sets a concrete id if passed.
         *
         * @param[in] id - id of the netlist
         * @returns the created netlist object
         */
        std::unique_ptr<Netlist> create_example_parse_netlist(int id = -1);

        // ===== Netlist Comparison Functions (mainly used to test parser and writer) =====

        /*
         * The following functions are mainly used to compare netlists that are written and parsed. Therefore
         * the pointers of the netlists objects can't be compared but their content (name, id, etc.) and
         * their connection to other objects.
         */
        /**
         * Checks if two nets are equal regardless if they are in the same netlist (they doesn't share a pointer).
         * Two nets are considered equal iff:
         * id is equal AND name is equal AND the stored data is equal AND the connected pin type is equal AND
         * the connected gates are equal according the function 'gates_are_equal'  AND booth or neither are global inputs/outputs
         *
         * @param n0[in] - Net
         * @param n1[in] - other Net
         * @param ignore_id - if the ids should be ignored in comparison
         * @param ignore_name - if the names should be ignored in comparison
         * @returns TRUE if n0 and n1 are equal under the considered conditions. FALSE otherwise.
         */
        bool nets_are_equal(Net* n0, Net* n1, const bool ignore_id = false, const bool ignore_name = false);

        /**
         * Checks if two gates are equal regardless if they are in the same netlist (they doesn't share a pointer).
         * Two gates are considered equal iff:
         * id is equal AND name is equal AND the Gate type is equal AND the stored data is equal AND both or neither are VCC/GND gates
         *
         * @param g0[in] - Gate
         * @param g1[in] - other Gate
         * @param ignore_id - if the ids should be ignored in comparison
         * @param ignore_name - if the names should be ignored in comparison
         * @return
         */
        bool gates_are_equal(Gate* g0, Gate* g1, const bool ignore_id = false, const bool ignore_name = false);

        /**
         * Checks if two modules are equal regardless if they are in the same netlist (they doesn't share a pointer).
         * Two modules are considered equal iff:
         * id is equal AND name is equal AND the stored data is equal AND
         * their gates are equal according the function 'gates_are_equal' AND its submodules are equal (only considering the ids)
         *
         * @param m_0[in] - module
         * @param m_1[in] - other module
         * @param ignore_id - if the ids should be ignored in comparison
         * @param ignore_name - if the names should be ignored in comparison
         * @returns TRUE if m_0 and m_1 are equal under the considered conditions. FALSE otherwise.
         */
        bool modules_are_equal(Module* m_0, Module* m_1, const bool ignore_id = false, const bool ignore_name = false);

        /**
         * Checks if two netlist are equal regardless if they are the same object.
         * Two netlists are considered equal iff:
         * id is equal AND Gate library is equal AND their gates are equal according the function 'gates_are_equal' AND
         * their nets are equal according the function 'nets_are_equal' AND
         * their modules are equal according the function 'modules_are_equal' AND
         * they have the same global GND/VCC gates AND they have the same input/output/inout nets
         *
         *
         * @param nl_0[in] - nl
         * @param nl_1[in] - other nl
         * @param ignore_id - if the ids should be ignored in comparison (in this case the module-,Gate-,Net names must be unique)
         * @returns TRUE if nl_0 and nl_1 are equal under the considered conditions. FALSE otherwise.
         */
        bool netlists_are_equal(Netlist* nl_0, Netlist* nl_1, const bool ignore_id = false);

        // ===== Filter Factory Functions (used in module::get_gates, netlist::get_nets, moduleget_submodules, Gate::get_sucessors, Gate::get_predecessors) =====

        // +++ Module Filter +++

        /**
         * Filter returns true for modules with the name 'name'
         *
         * @param name - the name of the modules the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(Module*)> module_name_filter(const std::string& name);

        // +++ Gate Filter +++

        /**
         * Filter only returns true, if the Gate is of type 'type' AND has the name 'name'
         *
         * @param type - the type of the gates the filter is searching for
         * @param name - the name of the gates the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(Gate*)> gate_filter(const std::string& type, const std::string& name);

        /**
         * Filter returns true for gates with the name 'name'
         *
         * @param name - the name of the gates the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(Gate*)> gate_name_filter(const std::string& name);

        /**
         * Filter returns true for gates of type 'type'
         *
         * @param type - the type of the gates the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(Gate*)> gate_type_filter(const std::string& type);

        // +++ Net Filter +++

        /**
         * Filter returns true for nets with the name 'name'
         *
         * @param name - the name of the nets the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(Net*)> net_name_filter(const std::string& name);

        // +++ Endpoint Filter +++

        /**
         * Filter returns true, if the type of the Gate, the Endpoint is connected to, is of type 'gate_type'
         *
         * @param gate_type - the type of the gates the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(const Endpoint&)> endpoint_gate_type_filter(const std::string& gate_type);

        /**
         * Filter returns true, if the type of the Gate, the Endpoint is connected to, has the name 'name'
         *
         * @param type - the name of the gates the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(const Endpoint&)> endpoint_gate_name_filter(const std::string& name);

        /**
         * Filter returns true for endpoints of type 'pin_type'
         *
         * @param pin_type - the pin type (i.e. "I1" or "O")
         * @return the std::function object of the filter function
         */
        std::function<bool(const Endpoint&)> endpoint_pin_type_filter(const std::string& pin_type);

        /**
         * Filter returns true, for all connected Endpoints (of adjacent gates) of type 'pin'
         *
         * @param type - the type of the endpoints the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(const std::string&, const Endpoint&)> adjacent_pin_filter(const std::string& pin);

        /**
         * Filter returns true for all endpoints, that are connected to the pin of pintype 'pin' of the calling Gate
         *
         * @param pin - the pin of the Gate, calling the get_predecessors/sucesseors function
         * @return the std::function object of the filter function
         */
        std::function<bool(const std::string&, const Endpoint&)> starting_pin_filter(const std::string& pin);

        /**
         * Filter returns true for all endpoints of adjacent gates of Gate type 'type'
         *
         * @param type - the type of adjacent gates, the filter is searching for
         * @return the std::function object of the filter function
         */
        std::function<bool(const std::string&, const Endpoint&)> adjacent_gate_type_filter(const std::string& type);

    }    // namespace test_utils
}    // namespace hal

//}
