#include "netlist/gate_library/gate_library_manager.h"
#include "netlist/netlist.h"
#include "netlist_test_utils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <netlist/gate.h>
#include <netlist/net.h>

namespace hal {

    class EndpointTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            test_utils::init_log_channels();
        }

        virtual void TearDown() {
        }
    };

    /**
     * Testing the access on the endpoints content
     *
     * Functions: get_gate, get_pin, is_destination_pin, is_source_pin
     */
    TEST_F(EndpointTest, check_set_get_gate) {
        TEST_START
            auto nl = test_utils::create_empty_netlist(0);
            Gate* test_gate = nl->create_gate(123, test_utils::get_gate_type_by_name("gate_1_to_1"), "test_gate");
            Net* test_net = nl->create_net("test_net");
            Endpoint ep(test_gate, "I", test_net,true);
            EXPECT_EQ(ep.get_gate(), test_gate);
            EXPECT_EQ(ep.get_pin(), "I");
            EXPECT_EQ(ep.get_net(), test_net);
            EXPECT_TRUE(ep.is_destination_pin());
            EXPECT_FALSE(ep.is_source_pin());
        TEST_END
    }

    /**
     * Testing the copy operator
     *
     * Functions: operator=
     */
    TEST_F(EndpointTest, check_copy_operator) {
        TEST_START
            auto nl = test_utils::create_empty_netlist(0);
            Gate* test_gate = nl->create_gate(123,test_utils::get_gate_type_by_name("gate_1_to_1"), "test_gate");
            Net* test_net = nl->create_net("test_net");
            Endpoint ep(test_gate, "I", test_net, true);

            Endpoint other_ep = ep;
            EXPECT_EQ(other_ep.get_gate(), test_gate);
            EXPECT_EQ(other_ep.get_pin(), "I");
            EXPECT_EQ(ep.get_net(), test_net);
            EXPECT_TRUE(other_ep.is_destination_pin());
        TEST_END
    }

    /**
     * Testing the operator< and operator== by create a set an search in it (the set uses this
     * operators internally)
     *
     * Functions: operator<, operator==
     */
    TEST_F(EndpointTest, check_comparison_operators) {
        TEST_START
            auto nl = test_utils::create_empty_netlist(0);
            Gate* test_gate_0 = nl->create_gate(1, test_utils::get_gate_type_by_name("gate_1_to_1"), "test_gate_0");
            Gate* test_gate_1 = nl->create_gate(2, test_utils::get_gate_type_by_name("gate_1_to_1"), "test_gate_1");

            // Create some endpoints
            Endpoint ep_0 = test_utils::get_endpoint(test_gate_0, "I");
            Endpoint ep_1 = test_utils::get_endpoint(test_gate_0, "0");
            Endpoint ep_2 = test_utils::get_endpoint(nullptr, "I");
            Endpoint ep_3 = test_utils::get_endpoint(nullptr, "0");
            Endpoint ep_4 = test_utils::get_endpoint(nullptr, "");
            Endpoint ep_5 = test_utils::get_endpoint(test_gate_1, "");

            // Add them to a set
            std::set<Endpoint> ep_set = {ep_0, ep_1, ep_2, ep_3, ep_4, ep_5};

            // Search them in the set
            EXPECT_NE(ep_set.find(ep_0), ep_set.end());
            EXPECT_NE(ep_set.find(ep_1), ep_set.end());
            EXPECT_NE(ep_set.find(ep_2), ep_set.end());
            EXPECT_NE(ep_set.find(ep_3), ep_set.end());
            EXPECT_NE(ep_set.find(ep_4), ep_set.end());
            EXPECT_NE(ep_set.find(ep_5), ep_set.end());

            // Search an Endpoint which isn't part of the set
            Endpoint ep_not_in_set = test_utils::get_endpoint(test_gate_1, "O");

            EXPECT_EQ(ep_set.find(ep_not_in_set), ep_set.end());

        TEST_END
    }

    /**
     * Testing the unequal operator
     *
     * Functions: operator!=
     */
    TEST_F(EndpointTest, check_unequal_operator) {
        TEST_START
            auto nl = test_utils::create_empty_netlist(0);
            Gate* test_gate = nl->create_gate(123, test_utils::get_gate_type_by_name("gate_1_to_1"), "test_gate");

            Endpoint ep = test_utils::get_endpoint(test_gate, "O");
            Endpoint other_ep = test_utils::get_endpoint(test_gate, "O");
            EXPECT_FALSE(ep != other_ep);
            other_ep = test_utils::get_endpoint(test_gate, "Other_Pin");
            EXPECT_TRUE(ep != other_ep);

        TEST_END
    }
} //namespace hal
