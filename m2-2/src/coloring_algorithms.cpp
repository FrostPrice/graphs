#include <vector>
#include <algorithm>
#include <unordered_set>

#include "graph.hpp"
#include "coloring_algorithms.hpp"

using namespace std;

// Helper: check if current partial coloring is valid (no neighbors vertices share the same color).
bool isValidColoring(Graph &g, vector<int> &colors)
{
    int n = g.getVertexCount();
    for (int u = 0; u < n; ++u)
    {
        for (int v : g.getNeighbors(u))
        {
            if (u < v && colors[u] != -1 && colors[u] == colors[v])
                return false;
        }
    }
    return true;
}

// Brute-force coloring: tries all color assignments until a valid one is found.
pair<int, vector<int>> bruteForce(Graph &g)
{
    int n = g.getVertexCount();
    vector<int> colors(n, -1);

    // Try k colors, starting from 2 up to n
    for (int k = 2; k <= n; ++k)
    {
        // Initialize to all zeros (color indices 0..k-1)
        fill(colors.begin(), colors.end(), 0);
        while (true)
        {
            if (isValidColoring(g, colors))
            {
                // Found valid assignment; remap colors to 1..k
                for (int &c : colors)
                    c += 1;
                return {k, colors};
            }
            // Increment "base-k" counter over the colors vector
            int idx = n - 1;
            while (idx >= 0)
            {
                if (colors[idx] < k - 1)
                {
                    colors[idx]++;
                    break;
                }
                else
                {
                    colors[idx] = 0;
                    --idx;
                }
            }
            if (idx < 0)
                break; // overflow: tried all assignments for this k
        }
    }
    // Fallback: every vertex gets its own color
    vector<int> fallback(n);
    for (int i = 0; i < n; ++i)
        fallback[i] = i + 1;
    return {n, fallback};
}

// Simple greedy coloring: assigns the smallest available color to each vertex in arbitrary order.
pair<int, vector<int>> greedy(Graph &g)
{
    int n = g.getVertexCount();
    vector<int> colors(n, -1);
    int maxColor = 0;

    // Assign colors in natural order 0..n-1
    for (int u = 0; u < n; ++u)
    {
        unordered_set<int> forbidden;
        for (int v : g.getNeighbors(u))
        {
            if (colors[v] != -1)
                forbidden.insert(colors[v]);
        }
        // Find smallest color not in forbidden
        int c = 0;
        while (forbidden.count(c))
            ++c;
        colors[u] = c;
        maxColor = max(maxColor, c);
    }
    // Remap to 1..maxColor+1
    for (int &c : colors)
        c += 1;
    return {maxColor + 1, colors};
}

// Welsh-Powell heuristic: orders vertices by descending degree before greedy coloring.
pair<int, vector<int>> welshPowell(Graph &g)
{
    int n = g.getVertexCount();
    vector<int> colors(n, -1);
    vector<int> order(n);
    // Create list of vertices [0..n-1]
    for (int i = 0; i < n; ++i)
        order[i] = i;
    // Sort by descending degree
    sort(order.begin(), order.end(), [&](int a, int b)
         { return g.getNeighbors(a).size() > g.getNeighbors(b).size(); });
    int maxColor = 0;

    for (int u : order)
    {
        unordered_set<int> forbidden;
        for (int v : g.getNeighbors(u))
        {
            if (colors[v] != -1)
                forbidden.insert(colors[v]);
        }
        int c = 0;
        while (forbidden.count(c))
            ++c;
        colors[u] = c;
        maxColor = max(maxColor, c);
    }
    for (int &c : colors)
        c += 1;
    return {maxColor + 1, colors};
}

// DSATUR heuristic: selects next vertex based on saturation (number of distinct neighbor colors).
pair<int, vector<int>> dsatur(Graph &g)
{
    int n = g.getVertexCount();
    vector<int> colors(n, -1);
    vector<unordered_set<int>> neighborColors(n);
    vector<int> degree(n);

    // Initialize degrees
    for (int u = 0; u < n; ++u)
        degree[u] = g.getNeighbors(u).size();

    int coloredCount = 0;
    int maxColor = 0;

    while (coloredCount < n)
    {
        // Select next vertex: highest saturation; break ties by degree
        int best = -1;
        int bestSat = -1;
        int bestDeg = -1;
        for (int u = 0; u < n; ++u)
        {
            if (colors[u] != -1)
                continue;
            int sat = neighborColors[u].size();
            if (sat > bestSat || (sat == bestSat && degree[u] > bestDeg))
            {
                best = u;
                bestSat = sat;
                bestDeg = degree[u];
            }
        }
        // Assign smallest permissible color
        unordered_set<int> &forb = neighborColors[best];
        int c = 0;
        while (forb.count(c))
            ++c;
        colors[best] = c;
        maxColor = max(maxColor, c);
        ++coloredCount;
        // Update neighborColors for neighbors
        for (int v : g.getNeighbors(best))
        {
            neighborColors[v].insert(c);
        }
    }
    // Remap to 1..maxColor+1
    for (int &c : colors)
        c += 1;
    return {maxColor + 1, colors};
}
