#pragma once

#include "dataflow_analysis/evaluation/similarity_score.h"
#include "hal_core/defines.h"

namespace hal
{
    namespace dataflow
    {
        /* forward declaration */
        struct Grouping;

        namespace textual_output
        {
            void write_register_output(const std::shared_ptr<Grouping>& state, const evaluation::SimilarityScore& score, const std::string m_path, const std::string file_name);
        }    // namespace textual_output
    }        // namespace dataflow
}    // namespace hal
