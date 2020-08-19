#include "core/log.h"
#include "core/plugin_manager.h"
#include "netlist/gate.h"
#include "netlist/net.h"
#include "netlist/netlist.h"
#include "plugin_graph_algorithm.h"

#include <igraph/igraph.h>
#include <tuple>

namespace hal
{
    std::map<int, std::set<Gate*>> plugin_graph_algorithm::get_communities_fast_greedy(Netlist* nl)
    {
        if (nl == nullptr)
        {
            log_error(this->get_name(), "{}", "parameter 'nl' is nullptr");
            return std::map<int, std::set<Gate*>>();
        }

        std::tuple<igraph_t, std::map<int, Gate*>> igraph_tuple = get_igraph_directed(nl);
        igraph_t graph                                                          = std::get<0>(igraph_tuple);
        std::map<int, Gate*> vertex_to_gate                     = std::get<1>(igraph_tuple);

        igraph_vector_t membership, modularity;
        igraph_matrix_t merges;

        igraph_to_undirected(&graph, IGRAPH_TO_UNDIRECTED_MUTUAL, 0);

        igraph_vector_init(&membership, 1);
        igraph_vector_init(&modularity, 1);
        igraph_matrix_init(&merges, 1, 1);

        igraph_community_fastgreedy(&graph,
                                    0, /* no weights */
                                    &merges,
                                    &modularity,
                                    &membership);

        // map back to HAL structures
        std::map<int, std::set<Gate*>> community_sets;
        for (int i = 0; i < igraph_vector_size(&membership); i++)
        {
            community_sets[(int)VECTOR(membership)[i]].insert(vertex_to_gate[i]);
        }
        igraph_vector_destroy(&membership);

        igraph_destroy(&graph);
        igraph_vector_destroy(&membership);
        igraph_vector_destroy(&modularity);
        igraph_matrix_destroy(&merges);

        return community_sets;
    }
}    // namespace hal
