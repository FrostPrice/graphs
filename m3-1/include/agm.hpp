#pragma once

#include <queue>
#include <vector>
#include <tuple>
#include <numeric>

#include "graph.hpp"

using namespace std;

// Prim's algorithm
pair<int, vector<tuple<int, int, int>>> prim_mst(Graph &graph);

// Kruskal's algorithm
pair<int, vector<tuple<int, int, int>>> kruskal_mst(Graph &graph);

// Disjoint Set Union for Kruskal
struct DSU
{
    vector<int> parent;
    DSU(int n) : parent(n)
    {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x)
    {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b)
    {
        int ra = find(a), rb = find(b);
        if (ra == rb)
            return false;
        parent[rb] = ra;
        return true;
    }
};