#pragma once

#include "def.h"

namespace hal
{
    /* forward declaration */
    struct Grouping;

    namespace group_by_input_output_size
    {
        std::shared_ptr<Grouping> process(const std::shared_ptr<Grouping>& state, bool successors);

    }    // namespace group_by_input_output_size
}    // namespace hal