#pragma once

#include "hal_core/defines.h"

namespace hal
{
    namespace dataflow
    {
        /* forward declaration */
        struct Grouping;

        namespace group_by_snn_gates_fixed
        {
            std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors, bool predecessors, int threshold);

        }    // namespace group_by_snn_gates_fixed
    }        // namespace dataflow
}    // namespace hal
