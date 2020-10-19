#include "dataflow_analysis/processing/passes/group_by_snn_groups_percent.h"

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
        namespace group_by_snn_groups_percent
        {
            bool snnGreaterThan(std::vector<u32> v1, std::vector<u32> v2, double percent)
            {
                int snn = 0;

                std::vector<u32> small_vec;
                std::vector<u32> large_vec;

                if (v1.size() < v2.size())
                {
                    small_vec = v1;
                    large_vec = v2;
                }
                else
                {
                    small_vec = v2;
                    large_vec = v1;
                }
                int threshold = percent * small_vec.size();

                for (std::vector<u32>::iterator small_it = small_vec.begin(); small_it != small_vec.end(); ++small_it)
                {
                    for (std::vector<u32>::iterator large_it = large_vec.begin(); large_it != large_vec.end(); ++large_it)
                    {
                        if (*small_it == *large_it)
                        {
                            snn++;
                        }
                        else if (*small_it < *large_it)
                        {
                            break;
                        }
                    }
                    if (snn >= threshold)
                        return true;
                }
                return false;
            }

            /* snn algorithm on input groups and neighboured groups for a percentaged threshold */
            std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors, bool predecessors, double percent)
            {
                auto new_state = std::make_shared<Grouping>(state->netlist_abstr);

                /* check characteristics */
                std::map<u32, std::list<u32>> characteristics_map;

                std::vector<std::vector<u32>> sparse;
                std::vector<u32> clustermap;

                // build sparse for groups (get all neighboured groups)
                for (const auto& [group_id, gates] : state->gates_of_group)
                {
                    std::vector<u32> vec;
                    std::set<u32> current_row;

                    std::unordered_set<u32> successor_groups;
                    std::unordered_set<u32> predecessor_groups;

                    if (successors)
                    {
                        successor_groups = state->get_successor_groups_of_group(group_id);
                    }
                    if (predecessors)
                    {
                        predecessor_groups = state->get_predecessor_groups_of_group(group_id);
                    }

                    std::set_union(successor_groups.begin(), successor_groups.end(), predecessor_groups.begin(), predecessor_groups.end(), std::inserter(current_row, current_row.begin()));

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
                    for (j = i - 1; j >= 0; --j)
                    {
                        if (snnGreaterThan(*it, sparse[j], percent))
                        {
                            clustermap[i] = clustermap[j];
                            break;
                        }
                    }
                    if (j == -1)
                    {
                        clustermap[i] = current_cluster++;
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

        }    // namespace group_by_snn_groups_percent
    }        // namespace dataflow
}    // namespace hal