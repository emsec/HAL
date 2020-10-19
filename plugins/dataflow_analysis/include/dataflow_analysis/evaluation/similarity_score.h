#pragma once

namespace hal
{
    namespace dataflow
    {
        namespace evaluation
        {
            struct SimilarityScore
            {
                double nmi;
                double purity;
            };
        }    // namespace evaluation
    }        // namespace dataflow
}    // namespace hal