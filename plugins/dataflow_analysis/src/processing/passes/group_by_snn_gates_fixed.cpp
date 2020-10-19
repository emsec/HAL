#include "dataflow_analysis/processing/passes/group_by_snn_gates_fixed.h"

#include "dataflow_analysis/common/grouping.h"
#include "dataflow_analysis/common/netlist_abstraction.h"
#include "hal_core/netlist/gate.h"
#include "hal_core/netlist/netlist.h"

#include <list>
#include <map>
#include <set>

namespace hal
{
    namespace dataflow
    {
        namespace group_by_snn_gates_fixed
        {
            bool snnGreaterThan(std::vector<u32> v1, std::vector<u32> v2, int threshold)
            {
                int snn = 0;
                for (std::vector<u32>::iterator it1 = v1.begin(); it1 != v1.end(); ++it1)
                {
                    for (std::vector<u32>::iterator it2 = v2.begin(); it2 != v2.end(); ++it2)
                    {
                        if (*it1 == *it2)
                        {
                            snn++;
                        }
                        else if (*it1 < *it2)
                        {
                            break;
                        }
                    }
                    if (snn >= threshold)
                        return true;
                }
                return false;
            }

            /* snn algorithm on input groups and neighboured gates for a fixed threshold */
            std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors, bool predecessors, int threshold)
            {
                auto new_state = std::make_shared<Grouping>(state->netlist_abstr);

                /* check characteristics */
                std::map<u32, std::list<u32>> characteristics_map;

                std::vector<std::vector<u32>> sparse;
                std::vector<u32> clustermap;

                // build sparse for groups (get all neighboured gates)
                for (const auto& [group_id, gates] : state->gates_of_group)
                {
                    std::vector<u32> vec;
                    std::unordered_set<u32> current_row;
                    for (const auto& gate_id : gates)
                    {
                        Gate* gate = state->netlist_abstr.nl->get_gate_by_id(gate_id);
                        if (successors)
                        {
                            for (const auto& successor : gate->get_unique_successors())
                            {
                                current_row.insert(successor->get_id());
                            }
                        }
                        if (predecessors)
                        {
                            for (const auto& predecessor : gate->get_unique_predecessors())
                            {
                                current_row.insert(predecessor->get_id());
                            }
                        }
                    }
                    vec.assign(current_row.begin(), current_row.end());
                    sort(vec.begin(), vec.end());
                    sparse.push_back(vec);
                }

                // clustering
                int i = 0, j = 0;
                int current_cluster = 0;
                clustermap.resize(sparse.size(), 0);
                for (std::vector<std::vector<u32>>::iterator it = sparse.begin(); it != sparse.end(); ++it, ++i)
                {
                    if (it->size() < threshold)
                    {
                        clustermap[i] = current_cluster++;
                    }
                    else
                    {
                        for (j = i - 1; j >= 0; --j)
                        {
                            if (snnGreaterThan(*it, sparse[j], threshold))
                            {
                                clustermap[i] = clustermap[j];
                                break;
                            }
                        }
                        if (j == -1)
                        {
                            clustermap[i] = current_cluster++;
                        }
                    }
                    characteristics_map[clustermap[i]].push_back(i);
                }

                /* check if merge is allowed */
                std::vector<std::vector<u32>> merge_sets;
                for (auto& merge_candidates : characteristics_map)
                {
                    auto& work_list = merge_candidates.second;
                    while (!work_list.empty())
                    {
                        auto it       = work_list.begin();
                        auto group_id = *it;
                        it            = work_list.erase(it);

                        std::vector<u32> merge_set = {group_id};

                        while (it != work_list.end())
                        {
                            auto test_group_id = *it;

                            if (!state->are_groups_allowed_to_merge(group_id, test_group_id))
                            {
                                ++it;
                                continue;
                            }

                            merge_set.push_back(test_group_id);
                            it = work_list.erase(it);
                        }
                        merge_sets.push_back(merge_set);
                    }
                }

                /* merge groups */
                u32 id_counter = -1;
                for (const auto& groups_to_merge : merge_sets)
                {
                    u32 new_group_id = ++id_counter;

                    for (const auto& old_group : groups_to_merge)
                    {
                        auto gates                                             = state->gates_of_group.at(old_group);
                        new_state->group_control_fingerprint_map[new_group_id] = new_state->netlist_abstr.gate_to_fingerprint.at(*gates.begin());

                        new_state->gates_of_group[new_group_id].insert(gates.begin(), gates.end());
                        for (const auto& sg : gates)
                        {
                            new_state->parent_group_of_gate[sg] = new_group_id;
                        }
                    }
                }

                return new_state;
            }

        }    // namespace group_by_snn_gates_fixed
    }        // namespace dataflow
}    // namespace hal