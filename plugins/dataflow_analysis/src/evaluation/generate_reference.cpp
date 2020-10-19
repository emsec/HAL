#include "dataflow_analysis/evaluation/generate_reference.h"

#include "dataflow_analysis/common/netlist_abstraction.h"
#include "dataflow_analysis/utils/utils.h"
#include "hal_core/netlist/gate.h"
#include "hal_core/netlist/net.h"
#include "hal_core/netlist/netlist.h"
#include "hal_core/utilities/log.h"

#include <chrono>
#include <list>

namespace hal
{
    namespace dataflow
    {
        namespace evaluation
        {
            namespace
            {
                void find_and_replace(std::string& data, const std::string& toSearch, const std::string& replaceStr)
                {
                    // Get the first occurrence
                    size_t pos = data.find(toSearch);

                    // Repeat till end is reached
                    while (pos != std::string::npos)
                    {
                        // Replace this occurrence of Sub String
                        data.replace(pos, toSearch.size(), replaceStr);
                        // Get the next occurrence from the current position
                        pos = data.find(toSearch, pos + replaceStr.size());
                    }
                }
            }    // namespace

            std::shared_ptr<Grouping> generate_reference(const NetlistAbstraction& netlist_abstr)
            {
                auto netlist  = netlist_abstr.nl;
                auto& all_ffs = netlist_abstr.all_sequential_gates;

                auto new_state = std::make_shared<Grouping>(netlist_abstr);

                // create groups by name
                std::map<std::string, std::unordered_set<u32>> reference_groups;
                std::string design_name = netlist_abstr.nl->get_design_name();

                if (design_name.find("vivado") != std::string::npos)
                {
                    if (design_name == "tiny_aes_128_vivado")
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            find_and_replace(group_name, "\\", "");
                            group_name = group_name.substr(0, group_name.find_last_of("["));

                            // fix AES grouping
                            if (group_name.at(0) == 'a')
                            {
                                if (group_name.find("k") != std::string::npos)
                                {
                                    group_name = group_name.substr(0, group_name.find_last_of("k") + 1);
                                }
                                else if (group_name.find("/S") != std::string::npos)
                                {
                                    group_name = group_name.substr(0, group_name.find_first_of("/S") + 1);
                                }
                            }
                            else if (group_name.at(0) == 'r')
                            {
                                if (group_name.find("/t") != std::string::npos)
                                {
                                    group_name = group_name.substr(0, group_name.find_first_of("/t") + 1);
                                }
                                else if (group_name.find("/S") != std::string::npos)
                                {
                                    group_name = group_name.substr(0, group_name.find_first_of("/S") + 1);
                                }
                            }
                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                    else
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            find_and_replace(group_name, "\\", "");
                            group_name = group_name.substr(0, group_name.find_last_of("["));
                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                }
                else if (design_name.find("synopsys") != std::string::npos)
                {
                    if (design_name == "tiny_aes_128_synopsys")
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));
                            if (group_name.find("_k") != std::string::npos)
                            {
                                if (group_name.at(0) == 'a')
                                {
                                    std::string round(1, group_name.at(1));
                                    if (group_name.at(3) == '_')
                                    {
                                        round += group_name.at(2);
                                    }
                                    group_name = "a_" + round + "_k";
                                }
                            }
                            else if (group_name.find("_S") != std::string::npos)
                            {
                                if (group_name.at(0) == 'a')
                                {
                                    std::string round(1, group_name.at(1));
                                    if (group_name.at(3) == '_')
                                    {
                                        round += group_name.at(2);
                                    }
                                    group_name = "a_" + round + "_S";
                                }
                            }
                            // fix AES grouping
                            if (group_name.find("state") == std::string::npos)
                            {
                                if (group_name.at(0) == 'r')
                                {
                                    if (group_name.at(1) == 'f')
                                    {
                                        group_name = "rf_S4";
                                    }
                                    else
                                    {
                                        std::string round(1, group_name.at(1));
                                        if (group_name.at(3) == '_')
                                        {
                                            round += group_name.at(2);
                                        }
                                        group_name = "r_" + round;
                                    }
                                }
                                if (group_name.at(0) == 'a')
                                {
                                    std::string round(1, group_name.at(1));
                                    if (group_name.at(3) == '_')
                                    {
                                        round += group_name.at(2);
                                    }
                                    group_name = "a_" + round;
                                }
                            }
                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                    else if (design_name == "opentitan_synopsys")
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));

                            if (group_name.find("top_earlgrey_usbdev_u_reg_u_stall") != std::string::npos)
                            {
                                group_name = "top_earlgrey_usbdev_u_reg_u_stall";
                            }

                            if (group_name.find("top_earlgrey_usbdev_u_reg_u_rxenable_setup") != std::string::npos)
                            {
                                group_name = "top_earlgrey_usbdev_u_reg_u_rxenable_setup";
                            }

                            if (group_name.find("top_earlgrey_usbdev_u_reg_u_iso") != std::string::npos)
                            {
                                group_name = "top_earlgrey_usbdev_u_reg_u_iso";
                            }

                            if (group_name.find("top_earlgrey_usbdev_u_reg_u_in_sent") != std::string::npos)
                            {
                                group_name = "top_earlgrey_usbdev_u_reg_u_in_sent";
                            }
                            if (group_name.find("top_earlgrey_usbdev_u_reg_u_data_toggle_clear") != std::string::npos)
                            {
                                group_name = "top_earlgrey_usbdev_u_reg_u_data_toggle_clear";
                            }
                            if (group_name.find("top_earlgrey_rv_plic_u_reg") != std::string::npos)
                            {
                                group_name = "top_earlgrey_rv_plic_u_reg";
                            }
                            if (group_name.find("top_earlgrey_alert_handler_i_reg_wrap_i_reg_u_loc_alert_cause") != std::string::npos)
                            {
                                group_name = "top_earlgrey_alert_handler_i_reg_wrap_i_reg_u_loc_alert_cause";
                            }
                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                    else if (design_name == "SHA3_synopsys")
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            if (group_name.find("i_reg_") != std::string::npos && group_name.at(0) == 'i')
                            {
                                // dont change!
                            }
                            else
                            {
                                group_name = group_name.substr(0, group_name.find_last_of("_"));
                                group_name = group_name.substr(0, group_name.find_last_of("_"));
                            }

                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                    else
                    {
                        for (const auto& ff : all_ffs)
                        {
                            std::string group_name = ff->get_name();
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));
                            group_name             = group_name.substr(0, group_name.find_last_of("_"));
                            reference_groups[group_name].insert(ff->get_id());
                        }
                    }
                }
                else
                {
                    log_warning("dataflow", "could not create reference for design: {}, using standard naming", netlist_abstr.nl->get_design_name());
                    for (const auto& ff : all_ffs)
                    {
                        std::string group_name = ff->get_name();
                        find_and_replace(group_name, "\\", "");
                        group_name = group_name.substr(0, group_name.find_last_of("["));
                        reference_groups[group_name].insert(ff->get_id());
                    }
                    return new_state;
                }
                // for (auto const& [group_str, group_set] : reference_groups)
                // {
                //     log_info("dataflow", "{} - size: {}", group_str, group_set.size());
                // }

                // create state
                u32 id_counter = -1;
                for (auto const& [group_str, group_set] : reference_groups)
                {
                    u32 new_group_id = ++id_counter;
                    new_state->gates_of_group.emplace(new_group_id, group_set);
                    for (const auto& sg : group_set)
                    {
                        new_state->parent_group_of_gate.emplace(sg, new_group_id);
                    }

                    // log_info("dataflow", "group: {} - {}", group_str, group_set.size());
                }

                // log_info("dataflow", "ref has {} groups", new_state->candidate_set.size());

                return new_state;
            }

        }    // namespace evaluation
    }        // namespace dataflow
}    // namespace hal