#pragma once

#include "hal_core/defines.h"

namespace hal
{
    namespace dataflow
    {
        /* forward declaration */
        struct Grouping;

        namespace group_by_snn_groups_percent
        {
            std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors, bool predecessors, double percent);

        }    // namespace group_by_snn_groups_percent
    }        // namespace dataflow
}    // namespace hal
