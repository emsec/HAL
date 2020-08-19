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

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <ostream>
#include <vector>

namespace hal
{
    /**
     * Boolean function class.
     *
     * @ingroup netlist
     */
    class BooleanFunction
    {
    public:
        enum Value
        {
            X    = -1,
            ZERO = 0,
            ONE  = 1
        };

        /**
         * Returns the value as a string.
         *
         * @param[in] v - the value.
         * @returns A string describing the value.
         */
        static std::string to_string(Value v);

        /**
         * ostream operator that forwards to_string of a value.
         *
         * @param[in] os - the stream to write to.
         * @param[in] v - the value.
         * @returns A reference to os.
         */
        friend std::ostream& operator<<(std::ostream& os, Value v);

        /**
         * Constructor for an empty function.
         * Evaluates to X (undefined).
         * Combining a function with an empty function leaves the other one unchanged.
         */
        BooleanFunction();

        /**
         * Constructor for a variable, usable in other functions.
         * Variable name must not be empty.
         *
         * @param[in] variable_name - Name of the variable.
         */
        BooleanFunction(const std::string& variable_name);

        /**
         * Constructor for a constant, usable in other functions.
         * The constant can be either X, Zero, or ONE.
         *
         * @param[in] constant - A constant value.
         */
        BooleanFunction(Value constant);

        /**
         * Substitutes a variable with another variable (i.e., variable renaming).
         * Applies to all instances of the variable in the function.
         *
         * This is just a shorthand for the generic substitute function.
         *
         * @param[in] old_variable_name - The old variable to substitute
         * @param[in] new_variable_name - The new variable name
         * @returns The new boolean function.
         */
        BooleanFunction substitute(const std::string& old_variable_name, const std::string& new_variable_name) const;

        /**
         * Substitutes a variable with another function (can again be a single variable).
         * Applies to all instances of the variable in the function.
         *
         * @param[in] variable_name - The variable to substitute
         * @param[in] function - The function to take the place of the varible
         * @returns The new boolean function.
         */
        BooleanFunction substitute(const std::string& variable_name, const BooleanFunction& function) const;

        /**
         * Evaluates the function on the given inputs and returns the result.
         *
         * @param[in] inputs - A map from variable names to values.
         * @returns The value that the function evaluates to.
         */
        Value evaluate(const std::unordered_map<std::string, Value>& inputs = {}) const;

        /**
         * Evaluates the function on the given inputs and returns the result.
         *
         * @param[in] inputs - A map from variable names to values.
         * @returns The value that the function evaluates to.
         */
        Value operator()(const std::unordered_map<std::string, Value>& inputs = {}) const;

        /**
         * Checks whether the function constantly outputs ONE.
         *
         * @returns True if function is constant ONE, false otherwise.
         */
        bool is_constant_one() const;

        /**
         * Checks whether the function constantly outputs ZERO.
         *
         * @returns True if function is constant ZERO, false otherwise.
         */
        bool is_constant_zero() const;

        /**
         * Checks whether the function is empty.
         *
         * @returns True if function is empty, false otherwise.
         */
        bool is_empty() const;

        /**
         * Get all variable names used in this boolean function.
         *
         * @returns A set of all variable names.
         */
        std::vector<std::string> get_variables() const;

        /**
         * Parse a function from a string representation.
         * Supported operators are  NOT ("!", "'"), AND ("&", "*", " "), OR ("|", "+"), XOR ("^") and brackets ("(", ")").
         * Operator precedence is ! > & > ^ > |
         *
         * Since, for example, '(' is interpreted as a new term, but might also be an intended part of a variable,
         * a vector of known variable names can be supplied, which are extracted before parsing.
         *
         * If there is an error during bracket matching, X is returned for that part.
         *
         * @param[in] expression - String containing a boolean function.
         * @param[in] variable_names - Names of variables to help resolve problematic functions
         * @returns The boolean function extracted from the string.
         */
        static BooleanFunction from_string(std::string expression, const std::vector<std::string>& variable_names = {});

        /**
         * Returns the boolean function as a string.
         *
         * @returns A string describing the boolean function.
         */
        std::string to_string() const;

        /**
         * ostream operator that forwards to_string of a boolean function.
         *
         * @param[in] os - the stream to write to.
         * @param[in] f - the function.
         * @returns A reference to os.
         */
        friend std::ostream& operator<<(std::ostream& os, const BooleanFunction& f);

        /**
         * Combines two boolean functions using an AND operator.
         *
         * @param[in] other - the other function to combine with.
         * @returns The combined boolean function.
         */
        BooleanFunction operator&(const BooleanFunction& other) const;

        /**
         * Combines two boolean functions using an OR operator.
         *
         * @param[in] other - the other function to combine with.
         * @returns The combined boolean function.
         */
        BooleanFunction operator|(const BooleanFunction& other) const;

        /**
         * Combines two boolean functions using an XOR operator.
         *
         * @param[in] other - the other function to combine with.
         * @returns The combined boolean function.
         */
        BooleanFunction operator^(const BooleanFunction& other) const;

        /**
         * Combines two boolean functions using an AND operator in place.
         *
         * @param[in] other - the other function to combine with.
         * @returns Self-reference.
         */
        BooleanFunction& operator&=(const BooleanFunction& other);

        /**
         * Combines two boolean functions using an OR operator in place.
         *
         * @param[in] other - the other function to combine with.
         * @returns Self-reference.
         */
        BooleanFunction& operator|=(const BooleanFunction& other);

        /**
         * Combines two boolean functions using an XOR operator in place.
         *
         * @param[in] other - the other function to combine with.
         * @returns Self-reference.
         */
        BooleanFunction& operator^=(const BooleanFunction& other);

        /**
         * Negates the boolean function.
         *
         * @returns The negated boolean function.
         */
        BooleanFunction operator!() const;

        /**
         * Tests whether two boolean functions are equal.
         *
         * @param[in] other - Boolean function to compare to.
         * @returns True when both boolean functions are equal, false otherwise.
         */
        bool operator==(const BooleanFunction& other) const;

        /**
         * Tests whether two boolean functions are unequal.
         *
         * @param[in] other - Boolean function to compare to.
         * @returns True when both boolean functions are unequal, false otherwise.
         */
        bool operator!=(const BooleanFunction& other) const;

        /**
         * Tests whether the function is in DNF.
         *
         * @returns True if in DNF, false otherwise.
         */
        bool is_dnf() const;

        /**
         * Gets the plain DNF representation of the function.
         *
         * @returns The DNF as a boolean function.
         */
        BooleanFunction to_dnf() const;

        /**
         * Gets the DNF clauses of the function.
         * Useful for parsing into other formats etc.
         *
         * Returns all clauses that are OR-ed together.
         * Each clause is a vector of pairs <variable name, boolean value>.
         * Example:
         * [[("a", true), ("b", false)], [("c", true)]] -> (a & !b) | c
         *
         * If the function is empty, this returns an empty vector.
         *
         * @returns The DNF clauses as a vector of vectors of pairs <string, bool>.
         */
        std::vector<std::vector<std::pair<std::string, bool>>> get_dnf_clauses() const;

        /**
         * Optimizes the function by first converting it to DNF and then applying the Quine-McCluskey algorithm.
         *
         * @returns The optimized boolean function.
         */
        BooleanFunction optimize() const;

        /**
         * Get the truth table outputs of the function.
         * WARNING: Exponential runtime in the number of variables!
         *
         * Output is the vector of output values when walking the truth table in ascending order.
         * The variable values are changed in order of appearance, i.e.:
         * first_var second_var | output_vector_index
         *     0         0      |   0
         *     1         0      |   1
         *     0         1      |   2
         *     1         1      |   3
         *
         * If ordered_variables is empty, all included variables are used and ordered alphabetically.
         *
         * @param[in] ordered_variables - Specific order in which the inputs shall be structured in the truth table.
         * @param[in] remove_unknown_variables - If true, all given variables that are not found in the function are removed from the truth table.
         * @returns The vector of output values.
         */
        std::vector<Value> get_truth_table(std::vector<std::string> ordered_variables = {}, bool remove_unknown_variables = false) const;

    protected:
        enum class operation
        {
            AND,
            OR,
            XOR
        };
        static std::string to_string(const operation& op);
        friend std::ostream& operator<<(std::ostream& os, const operation& op);

        static BooleanFunction from_string_internal(std::string expression, const std::vector<std::string>& variable_names);

        /*
        * Constructor for a function of the form "term1 op term2 op term3 op ..."
        * Empty terms behaves like constant X.
        * If there is only a single term, this constructor simply copies said term.
        */
        BooleanFunction(operation op, const std::vector<BooleanFunction>& operands, bool invert_result = false);

        BooleanFunction combine(operation op, const BooleanFunction& other) const;

        std::string to_string_internal() const;

        // replaces a^b with (a & !b | (!a & b)
        BooleanFunction replace_xors() const;

        // propagates negations down to the variables
        BooleanFunction propagate_negations(bool negate_term = false) const;

        // expands ands, i.e., a & (b | c) -> a&b | a&c
        BooleanFunction expand_ands() const;
        // helper function 1
        std::vector<BooleanFunction> expand_ands_internal(const std::vector<std::vector<BooleanFunction>>& sub_primitives) const;
        // helper function 2
        std::vector<BooleanFunction> get_primitives() const;

        // merges constants if possible and resolves duplicates
        BooleanFunction optimize_constants() const;

        // merges nested expressions of the same operands
        BooleanFunction flatten() const;

        // merges nested expressions of the same operands
        static std::vector<std::vector<Value>> qmc(const std::vector<std::vector<Value>>& terms);

        // helper to allow for substitution with reduced amount of copies
        static void substitute_helper(BooleanFunction& f, const std::string& v, const BooleanFunction& s);

        bool m_invert;

        enum class content_type
        {
            VARIABLE,
            CONSTANT,
            TERMS
        } m_content;

        std::string m_variable;

        Value m_constant;

        operation m_op;
        std::vector<BooleanFunction> m_operands;
    };
}    // namespace hal
