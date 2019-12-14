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
            EXPECT_TRUE(( !_1 ).is_constant_zero());    // <- fails
            EXPECT_TRUE(( a^a ).is_constant_zero());    // <- fails
            EXPECT_TRUE(( a&(!a) ).is_constant_zero());
            EXPECT_TRUE(( _0|_0 ).is_constant_zero());  // <- fails
        }
        {
            // Some samples that are constant one
            EXPECT_TRUE(( _1 ).is_constant_one());
            EXPECT_TRUE(( !_0 ).is_constant_one());    // <- fails
            EXPECT_TRUE(( a^(!a) ).is_constant_one());
            EXPECT_TRUE(( a|(!a) ).is_constant_one()); // <- fails
            EXPECT_TRUE(( _1&_1 ).is_constant_one());  // <- fails
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
