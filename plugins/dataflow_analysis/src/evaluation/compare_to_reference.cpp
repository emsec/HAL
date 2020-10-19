#include "dataflow_analysis/evaluation/compare_to_reference.h"

#include "dataflow_analysis/common/netlist_abstraction.h"
#include "hal_core/utilities/log.h"
#include "igraph/igraph.h"
#include "dataflow_analysis/utils/utils.h"

#include <chrono>
#include <set>

namespace hal
{
    namespace evaluation
    {
        namespace
        {
            double calculate_nmi(const std::map<u32, std::set<u32>>& ref_set, const std::map<u32, std::set<u32>>& candidate_set)
            {
                std::map<u32, u32> gate_to_igraph_id;

                u32 counter = -1;
                for (const auto& ref_group : ref_set)
                {
                    for (const auto& ff : ref_group.second)
                    {
                        gate_to_igraph_id[ff] = ++counter;
                    }
                }

                std::vector<igraph_real_t> reference_membership_c_vector(gate_to_igraph_id.size());
                std::vector<igraph_real_t> candidate_membership_c_vector(gate_to_igraph_id.size());

                igraph_vector_t reference_membership;
                igraph_vector_t candidate_membership;

                for (const auto& [ref_group_id, ref_group] : ref_set)
                {
                    for (const auto& ff : ref_group)
                    {
                        reference_membership_c_vector[gate_to_igraph_id[ff]] = ref_group_id;
                    }
                }

                for (const auto& [candidate_group_id, candidate_group] : candidate_set)
                {
                    for (const auto& ff : candidate_group)
                    {
                        candidate_membership_c_vector[gate_to_igraph_id[ff]] = candidate_group_id;
                    }
                }

                igraph_vector_init_copy(&reference_membership, reference_membership_c_vector.data(), gate_to_igraph_id.size());
                igraph_vector_init_copy(&candidate_membership, candidate_membership_c_vector.data(), gate_to_igraph_id.size());

                igraph_real_t result_comparison_IGRAPH_COMMCMP_VI, result_comparison_IGRAPH_COMMCMP_NMI, result_comparison_IGRAPH_COMMCMP_SPLIT_JOIN, result_comparison_IGRAPH_COMMCMP_RAND,
                    result_comparison_IGRAPH_COMMCMP_ADJUSTED_RAND;

                igraph_compare_communities(&reference_membership, &candidate_membership, &result_comparison_IGRAPH_COMMCMP_VI, IGRAPH_COMMCMP_VI);
                igraph_compare_communities(&reference_membership, &candidate_membership, &result_comparison_IGRAPH_COMMCMP_NMI, IGRAPH_COMMCMP_NMI);
                igraph_compare_communities(&reference_membership, &candidate_membership, &result_comparison_IGRAPH_COMMCMP_SPLIT_JOIN, IGRAPH_COMMCMP_SPLIT_JOIN);
                igraph_compare_communities(&reference_membership, &candidate_membership, &result_comparison_IGRAPH_COMMCMP_RAND, IGRAPH_COMMCMP_ADJUSTED_RAND);
                igraph_compare_communities(&reference_membership, &candidate_membership, &result_comparison_IGRAPH_COMMCMP_ADJUSTED_RAND, IGRAPH_COMMCMP_ADJUSTED_RAND);

                // log_info("dataflow", "result_comparison_IGRAPH_COMMCMP_VI: {}", result_comparison_IGRAPH_COMMCMP_VI);
                // log_info("dataflow", "result_comparison_IGRAPH_COMMCMP_NMI: {}", result_comparison_IGRAPH_COMMCMP_NMI);
                // log_info("dataflow", "result_comparison_IGRAPH_COMMCMP_SPLIT_JOIN: {}", result_comparison_IGRAPH_COMMCMP_SPLIT_JOIN);
                // log_info("dataflow", "result_comparison_IGRAPH_COMMCMP_RAND: {}", IGRAPH_COMMCMP_ADJUSTED_RAND);
                // log_info("dataflow", "result_comparison_IGRAPH_COMMCMP_ADJUSTED_RAND: {}", IGRAPH_COMMCMP_ADJUSTED_RAND);

                igraph_vector_destroy(&candidate_membership);
                igraph_vector_destroy(&reference_membership);

                return result_comparison_IGRAPH_COMMCMP_NMI;
            }

            double calculate_purity(const std::map<u32, std::set<u32>>& ref_set, const std::map<u32, std::set<u32>>& candidate_set)
            {
                std::map<u32, u32> max_value_of_classes;

                for (const auto& [candidate_group_id, candidate_group] : candidate_set)
                {
                    std::map<u32, u32> classes_counter;
                    for (const auto& ref_group_id : ref_set)
                    {
                        classes_counter[ref_group_id.first] = 0;
                    }

                    for (const auto& ff : candidate_group)
                    {
                        for (const auto& [ref_group_id, ref_group] : ref_set)
                        {
                            if (ref_group.find(ff) != ref_group.end())
                            {
                                classes_counter[ref_group_id]++;
                                break;
                            }
                        }
                    }
                    u32 highest_number = 0;
                    for (const auto& count : classes_counter)
                    {
                        if (highest_number < count.second)
                        {
                            highest_number = count.second;
                        }
                    }

                    max_value_of_classes[candidate_group_id] = highest_number;
                }

                u32 purity_total = 0;
                for (const auto& count : max_value_of_classes)
                {
                    purity_total += count.second;
                }

                u32 gate_count = 0;
                for (const auto& candidate_group : candidate_set)
                {
                    gate_count += candidate_group.second.size();
                }

                double purity = (double)purity_total / gate_count;

                //log_info("dataflow", "purity: {}", purity);

                return purity;
            }
        }    // namespace

        SimilarityScore compare_to_reference(const std::shared_ptr<Grouping>& reference_state, const std::shared_ptr<Grouping>& candidate_state)
        {
            // auto begin_time = std::chrono::high_resolution_clock::now();

            // log_info("dataflow", "start compare_states");

            std::map<u32, std::set<u32>> ref_group_set;
            std::map<u32, std::set<u32>> candidate_group_set;

            u32 counter = -1;

            for (const auto& it : reference_state->gates_of_group)
            {
                counter++;
                ref_group_set[counter].insert(it.second.begin(), it.second.end());
            }

            counter = -1;
            for (const auto& it : candidate_state->gates_of_group)
            {
                counter++;
                candidate_group_set[counter].insert(it.second.begin(), it.second.end());
            }

            // u32 match_count = 0;

            // for (const auto& candidate_group : candidate_group_set)
            // {
            //     for (const auto& ref_group : ref_group_set)
            //     {
            //         if (ref_group.second == candidate_group.second)
            //         {
            //             match_count++;
            //             continue;
            //         }
            //     }
            // }

            if (ref_group_set.size() == 0)
            {
                log_error("dataflow", "can't compare to golden model, ref_set empty");
                return {0, 0};
            }

            //log_info("dataflow", "matches: {}/{}", match_count, ref_group_set.size());

            SimilarityScore result;
            result.nmi    = calculate_nmi(ref_group_set, candidate_group_set);
            result.purity = calculate_purity(ref_group_set, candidate_group_set);
            return result;

            // log_info(
            //     "dataflow", "compare_states executed in {:3.2f}s", (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin_time).count() / 1000);
        }

        std::unordered_map<std::shared_ptr<Grouping>, SimilarityScore>
            compare_to_reference(const processing::Result& processing_result, const evaluation::Result& eval_result, const std::shared_ptr<Grouping>& ref_state)
        {
            std::unordered_map<std::shared_ptr<Grouping>, SimilarityScore> output;
            auto score = compare_to_reference(ref_state, eval_result.merged_result);
            output.emplace(eval_result.merged_result, score);

            log_info("dataflow", "  nmi: {:.4f}, purity: {:.4f}", score.nmi, score.purity);
            log_info("dataflow", "");

            /*
        for (const auto& group : processing_result.unique_groupings)
        {
            output[group] = compare_to_reference(group, ref_state);
        }
        */

            return output;
        }
    }    // namespace evaluation
}    // namespace hal