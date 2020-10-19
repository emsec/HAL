#pragma once

#include "hal_core/defines.h"

namespace hal
{
    /* forward declaration */
    struct Grouping;

    namespace group_by_snn_groups_fixed
    {
        std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors, bool predecessors, int threshold);

    }    // namespace group_by_snn
}    // namespace hal
