#include "netlist_test_utils.h"
#include "gtest/gtest.h"
#include <netlist/boolean_function.h>
#include <iostream>


using namespace test_utils;


class boolean_function_test : public ::testing::Test
{
protected:

    const boolean_function::value X = boolean_function::value::X;
    const boolean_function::value ZERO = boolean_function::value::ZERO;
    const boolean_function::value ONE = boolean_function::value::ONE;

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }



    // Test Debug only
    void print_bf(boolean_function bf){
        std::cout << "\n-------------\n" << bf << "\n-------------\n";
    }

    void printTruthTable(boolean_function bf, std::vector<std::string> vars){
        std::cout << std::endl;
        for (auto i : vars){
            std::cout << i;
        }
        std::cout << "|O" << std::endl;
        std::vector<boolean_function::value> t_table = bf.get_truth_table(vars);
        for (unsigned int i = 0; i < vars.size() + 2; i++) std::cout << "-";
        std::cout << std::endl;
        for (unsigned int i = 0; i < t_table.size(); i++){
            for (unsigned int j = 0; j < vars.size(); j++){
                std::cout << ((((i>>j)&1)>0)?"1":"0");
            }
            std::cout << "|";
            switch(t_table[i]){
                case boolean_function::value::ONE:
                    std::cout << "1";
                    break;
                case boolean_function::value::ZERO:
                    std::cout << "0";
                    break;
                default:
                    std::cout << "X";
                    break;
            }
            std::cout << std::endl;
        }

    }

    /**
     * Create a string to value map, that can be used by the evaluate function. Each variable MUST be one character long.
     * I.e: ("ABC","10X") creates the map: ("A" -> ONE, "B" -> ZERO, "C" -> X).
     *
     * If the inputs are invalid, an empty map s returned.
     *
     * @param variables - the names of the variables next to each others
     * @param values - the values the variables should be set to
     * @returns a variables to values map, that can be interpreted by the boolean funcitons evaluate function.
     */
    std::map<std::string, boolean_function::value> create_input_map(std::string variables, std::string values)
    {
        std::map<std::string, boolean_function::value> res;
        // Booth strings must be equal in length
        if (variables.size() != values.size()){
            return res;
        }
        for (int c = 0; c < variables.size(); c++)
        {
            std::string var = std::string(1, variables.at(c));
            std::string val = std::string(1, values.at(c));
            // Can't set the same variable twice
            if (res.find(var) != res.end())
            {
                return std::map<std::string, boolean_function::value>();
            }
            if (val == "0")
            {
                res.insert(std::pair<std::string, boolean_function::value>(var, boolean_function::value::ZERO));
            }
            else if (val == "1")
            {
                res.insert(std::pair<std::string, boolean_function::value>(var, boolean_function::value::ONE));
            }
            else if (val == "x" || val == "X")
            {
                res.insert(std::pair<std::string, boolean_function::value>(var, boolean_function::value::X));
            }
            // If the values string contains an illegal character, exit
            else
            {
                return std::map<std::string, boolean_function::value>();
            }
        }
        return res;
    }

};

/**
 * Testing template
 *
 * Functions: <functions>
 */
TEST_F(boolean_function_test, check_){
    TEST_START
        {
            // Set multiple data with different keys and categories
            boolean_function a("A");
            boolean_function b("B");

            boolean_function::from_string("1 & 1 & 1").to_dnf();

            EXPECT_TRUE(true);
        }

    TEST_END
}

/**
 * Testing the different constructors and the main functionality, by implement the following boolean function:
 *
 *  f(A,B,C) = ( (A AND B) OR C ) XOR 1
 *
 * Functions: constructor, evaluate, get_truth_table, AND, XOR, OR
 */
TEST_F(boolean_function_test, check_main_example){
    TEST_START
        {
            // Constuctor with variables
            boolean_function a("A");
            boolean_function b("B");
            boolean_function c("C");
            // Constructor with constant
            boolean_function _1(ONE);

            // Combining them
            boolean_function r = ( (a & b) | c ) ^ _1;

            EXPECT_EQ(r(create_input_map("ABC", "000")), ONE );
            EXPECT_EQ(r(create_input_map("ABC", "001")), ZERO);
            EXPECT_EQ(r(create_input_map("ABC", "010")), ONE );
            EXPECT_EQ(r(create_input_map("ABC", "011")), ZERO);

            EXPECT_EQ(r(create_input_map("ABC", "100")), ONE );
            EXPECT_EQ(r(create_input_map("ABC", "101")), ZERO);
            EXPECT_EQ(r(create_input_map("ABC", "110")), ZERO);
            EXPECT_EQ(r(create_input_map("ABC", "111")), ZERO);

            std::vector<boolean_function::value> truth_table = r.get_truth_table(std::vector<std::string>({"C","B","A"}));

            EXPECT_EQ(truth_table, std::vector<boolean_function::value>({ONE, ZERO, ONE, ZERO, ONE, ZERO, ZERO, ZERO}));
        }

    TEST_END
}

/**
 * Testing the functions is_constant_one and is_constant_zero, by passing some sample inputs
 *
 * Functions: is_constant_one, is_constant_zero
 */
TEST_F(boolean_function_test, check_is_constant){
    TEST_START
        boolean_function a("A");
        boolean_function b("B");
        boolean_function c("C");
        boolean_function _0(ZERO);
        boolean_function _1(ONE);
        {
            // Some samples that are constant zero
            EXPECT_TRUE(( _0 ).is_constant_zero());
            EXPECT_TRUE(( !_1 ).is_constant_zero());
            EXPECT_TRUE(( a^a ).is_constant_zero());
            EXPECT_TRUE(( a&(!a) ).is_constant_zero());
            EXPECT_TRUE(( _0|_0 ).is_constant_zero());
        }
        {
            // Some samples that are constant one
            EXPECT_TRUE(( _1 ).is_constant_one());
            EXPECT_TRUE(( !_0 ).is_constant_one());
            EXPECT_TRUE(( a^(!a) ).is_constant_one());
            EXPECT_TRUE(( a|(!a) ).is_constant_one());
            EXPECT_TRUE(( _1&_1 ).is_constant_one());
        }
        {
            // Some samples that are NOT constant zero
            EXPECT_FALSE(( _1 ).is_constant_zero());
            EXPECT_FALSE(( a ).is_constant_zero());
            EXPECT_FALSE(( a^a^b ).is_constant_zero());
            EXPECT_FALSE(( a&b ).is_constant_zero());
            EXPECT_FALSE(( _0|_1 ).is_constant_zero());
        }
        {
            // Some samples that are NOT constant one
            EXPECT_FALSE(( _0 ).is_constant_one());
            EXPECT_FALSE(( a ).is_constant_one());
            EXPECT_FALSE(( a^b^c ).is_constant_one());
            EXPECT_FALSE(( a&b ).is_constant_one());
            EXPECT_FALSE(( _0&_1 ).is_constant_one());
        }

    TEST_END
}

/**
 * Testing the is_empty function
 *
 * Functions: is_empty
 */
TEST_F(boolean_function_test, check_is_empty){
    TEST_START
        {
            // The boolean function is not empty
            boolean_function not_empty("A");
            EXPECT_FALSE(not_empty.is_empty());
        }
        {
            // The boolean function is empty
            boolean_function empty;
            EXPECT_TRUE(empty.is_empty());
        }
    TEST_END
}

/**
 * Testing the get_variables function
 *
 * Functions: get_variables
 */
TEST_F(boolean_function_test, check_get_variables){
    TEST_START
        {
            // Get variables
            boolean_function a("A");
            boolean_function b("B");
            boolean_function c("C");
            boolean_function a_2("A");
            EXPECT_EQ((a|b|c|a_2).get_variables(), std::set<std::string>({"A","B","C"}));
        }
    TEST_END
}

/**
 * Testing comparation operator
 *
 * Functions: operator==, operator!=
 */
TEST_F(boolean_function_test, check_compare_operator){
    TEST_START
        // Tests for ==
        {
            // Compare the same object
            boolean_function a("A");
            EXPECT_TRUE((a == a));
        }
        {
            // The boolean functions are equivalent in syntax
            boolean_function a("A");
            boolean_function b("B");
            EXPECT_TRUE(((a|b) == (a|b)));
        }
        {
            // The boolean functions are equivalent in semantic (but not in syntax)
            boolean_function a("A");
            boolean_function b("B");
            // EXPECT_TRUE(((a|b|b) == (a|b)));
        }
        // Tests for !=
        {
            // The boolean function are equivalent in semantic, but do not share the same variable
            boolean_function a("A");
            boolean_function b("B");
            EXPECT_TRUE((a != b));
        }
        {
            // Compare boolean functions of different types (constant, variable, expression)
            boolean_function a("A");
            boolean_function b("B");
            boolean_function _1(ONE);
            EXPECT_TRUE((a != (a|(b&_1)))); // variable - expression
            EXPECT_TRUE((a != _1 )); // variable - constant
            EXPECT_TRUE(((a|(b&_1)) != _1 )); // expression - constant
        }
        {
            // Compare semantically different expressions
            boolean_function a("A");
            boolean_function b("B");
            EXPECT_TRUE(((a&b) != (a|b)));
            EXPECT_TRUE(((a^b) != (a&b)));
            EXPECT_TRUE(((a^b) != ((!a)&b)));
        }
    TEST_END
}

/**
 * Testing the integrity of the optimize function
 *
 * Functions: optimize
 */
TEST_F(boolean_function_test, check_optimize){
    TEST_START
        boolean_function a("A");
        boolean_function b("B");
        boolean_function c("C");
        boolean_function _0(ZERO);
        boolean_function _1(ONE);
        {
            // Optimize some boolean functions and compare their truth_table
            boolean_function bf = (!(a^b&c)|(b|c&_1))^((a&b) | (a|b|c));
            EXPECT_EQ(bf.get_truth_table(std::vector<std::string>({"C","B","A"})), bf.optimize().get_truth_table(std::vector<std::string>({"C","B","A"}))); // <- fails
        }
        {
            // Optimize some boolean functions and compare their truth_table
            boolean_function bf = (a|b|c);
            EXPECT_EQ(bf.get_truth_table(std::vector<std::string>({"C","B","A"})), bf.optimize().get_truth_table(std::vector<std::string>({"C","B","A"}))); // <- fails

            // +++ DEBUG OUTPUT +++
            //printTruthTable(bf, std::vector<std::string>({"C","B","A"}));
            //printTruthTable(bf.optimize(), std::vector<std::string>({"C","B","A"}));
            //std::cout << "Variables: amount = " << bf.optimize().get_variables().size() << std::endl;
            //for (auto v : bf.optimize().get_variables())
                //std::cout << v << ",";
            //std::cout << std::endl;
        }

    TEST_END
}



/**
 * Test string parsing, dnf, and optimization for a collection of functions
 *
 * Functions: from_string, to_dnf, optimize
 */
TEST_F(boolean_function_test, check_test_vectors)
{
    TEST_START
    {
        /* clang-format off */
        std::vector<std::string> test_cases = {
            "0",
            "1",
            "a",
            "a ^ a",
            "a | a",
            "a & a",
            "a ^ b",
            "a | b",
            "a & b",
            "!(((!8 & !(!(!19 | !20) & 26)) | (8 & !((!26 & !(!19 | !20)) | (26 & !(!19 | 20))))) & !(((((((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & !((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & !27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (!((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19))))) & !26 & !((8 & (!20 | 19)) | (!8 & !(!20 | 19)))) | (!((((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & !((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & !27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (!((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19))))) & 26 & !((8 & (!20 | 19)) | (!8 & !(!20 | 19)))) | (!((((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & !((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & !27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (!((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19))))) & !26 & ((8 & (!20 | 19)) | (!8 & !(!20 | 19)))) | (((((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & !((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & !27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (!((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19)))) | (((!(!20 | 19) & 28 & !((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & !28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | ((!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19)))) | (!(!20 | 19) & 28 & ((10 & (!20 | 19)) | (!10 & !(!20 | 19))))) & 27 & ((9 & (!20 | 19)) | (!9 & !(!20 | 19))))) & 26 & ((8 & (!20 | 19)) | (!8 & !(!20 | 19))))) & !19))",
        };
        /* clang-format on */

        for (const auto& f_str : test_cases)
        {
            auto f        = boolean_function::from_string(f_str);
            auto tmp_vars = f.get_variables();
            std::vector<std::string> ordered_variables(tmp_vars.begin(), tmp_vars.end());
            auto original_truth_table  = f.get_truth_table(ordered_variables);
            auto dnf                   = f.to_dnf();
            auto dnf_truth_table       = dnf.get_truth_table(ordered_variables);
            auto optimized             = f.optimize();
            auto optimized_truth_table = optimized.get_truth_table(ordered_variables);

            if (original_truth_table != dnf_truth_table)
            {
                EXPECT_TRUE(false) << "ERROR: DNF function does not match original function" << std::endl
                                   << "  original function:  " << f << std::endl
                                   << "  DNF of function:    " << dnf << std::endl
                                   << "  optimized function: " << optimized << std::endl;
            }

            if (original_truth_table != optimized_truth_table)
            {
                EXPECT_TRUE(false) << "ERROR: optimized function does not match original function" << std::endl
                                   << "  original function:  " << f << std::endl
                                   << "  DNF of function:    " << dnf << std::endl
                                   << "  optimized function: " << optimized << std::endl;
            }
        }
    }

    TEST_END
}
