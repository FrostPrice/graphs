#include <algorithm>
#include <vector>
#include <limits>
#include <random>

#include "max_flow.hpp"
#include <time.h>

using namespace std;

// BEGIN: Depth-First Search (DFS) utility functions
bool dfsUtil(Graph &graph, int v, int goal, std::vector<bool> &visited, std::vector<int> &parent)
{
    visited[v] = true;
    if (v == goal)
        return true;

    for (int neighbor : graph.getNeighbors(v))
    {
        float capacity = graph.edgeWeight(v, neighbor);
        if (!visited[neighbor] && capacity > 0)
        {
            parent[neighbor] = v;
            if (dfsUtil(graph, neighbor, goal, visited, parent))
                return true;
        }
    }
    return false;
}

bool dfs(Graph &graph, int start, int goal, std::vector<bool> &visited, std::vector<int> &parent)
{
    visited.assign(graph.getVertexCount(), false);
    parent.assign(graph.getVertexCount(), -1);
    return dfsUtil(graph, start, goal, visited, parent);
}
// END: Depth-First Search (DFS) utility functions

int fordFulkerson(Graph original, int source, int sink)
{
    Graph residual = original.copy();
    int max_flow = 0;
    vector<int> parent(residual.getVertexCount());

    while (true)
    {
        vector<bool> visited(residual.getVertexCount(), false);
        fill(parent.begin(), parent.end(), -1);

        if (!dfs(residual, source, sink, visited, parent))
            break;

        float path_flow = numeric_limits<float>::max();
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, residual.getCapacity(u, v));
        }

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residual.addFlow(u, v, -path_flow);
            residual.addFlow(v, u, path_flow);
        }

        max_flow += path_flow;
    }

    return max_flow;
}

int localSearch(Graph &graph, int source, int sink, int iterations)
{
    srand(static_cast<unsigned>(time(nullptr)));

    int best_flow = fordFulkerson(graph, source, sink);
    Graph best_graph = graph;
    // Generate random number based on a seed
    mt19937 rng(random_device{}());

    for (int i = 0; i < iterations; ++i)
    {
        Graph neighbor = best_graph;
        auto edges = neighbor.getEdges();
        if (edges.empty())
            break;

        uniform_int_distribution<size_t> dist(0, edges.size() - 1);
        auto [u, v, c] = edges[dist(rng)];

        neighbor.removeEdge(u, v);
        neighbor.addEdge(v, u, c); // Invert the edge direction

        int new_flow = fordFulkerson(neighbor, source, sink);
        if (new_flow > best_flow)
        {
            best_flow = new_flow;
            best_graph = neighbor;
        }
    }
    return best_flow;
}
