#include <queue>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "agm.hpp"
#include "graph.hpp"

using namespace std;

// Prim's algorithm implementation
pair<int, vector<tuple<int, int, int>>> prim_mst(Graph &graph)
{
    using Edge = tuple<int, int, int>; // weight, from, to
    priority_queue<Edge, vector<Edge>, greater<>> pq;
    vector<bool> visited(graph.getVertexCount(), false);
    vector<Edge> mst;
    int total_weight = 0;

    visited[0] = true; // start at node 0
    for (int neighbor : graph.getNeighbors(0))
    {
        int weight = graph.edgeWeight(0, neighbor);
        pq.emplace(weight, 0, neighbor);
    }

    while (!pq.empty())
    {
        auto [weight, from, to] = pq.top();
        pq.pop();
        if (visited[to])
            continue;
        visited[to] = true;
        mst.emplace_back(weight, from, to);
        total_weight += weight;

        for (int next : graph.getNeighbors(to))
        {
            if (!visited[next])
            {
                int w = graph.edgeWeight(to, next);
                pq.emplace(w, to, next);
            }
        }
    }

    return {total_weight, mst};
}

// Kruskal's algorithm implementation
pair<int, vector<tuple<int, int, int>>> kruskal_mst(Graph &graph)
{
    using Edge = tuple<int, int, int>; // weight, u, v
    vector<Edge> edges;

    // Get all edges from the graph
    auto graph_edges = graph.getEdges();
    for (const auto &[u, v, w] : graph_edges)
    {
        if (u < v) // avoid duplicates for undirected graphs
            edges.emplace_back(w, u, v);
    }

    sort(edges.begin(), edges.end());
    DSU dsu(graph.getVertexCount());

    vector<Edge> mst;
    int total_weight = 0;

    for (auto [w, u, v] : edges)
    {
        if (dsu.unite(u, v))
        {
            mst.emplace_back(w, u, v);
            total_weight += w;
        }
    }

    return {total_weight, mst};
}
