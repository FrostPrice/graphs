#include "NavAlgorithms.hpp"
#include <iostream>
#include <queue>
#include <limits>

void bfs(Graph &graph, const std::string &startLabel)
{
    std::cout << "\n[BFS] Visiting from: " << startLabel << std::endl;

    int start = graph.getVertexIndex(startLabel);
    std::vector<bool> visited(graph.getVertexCount(), false);
    std::list<int> queue;

    visited[start] = true;
    queue.push_back(start);

    while (!queue.empty())
    {
        int v = queue.front();
        queue.pop_front();
        std::cout << graph.getVertexLabel(v) << " ";

        for (int neighbor : graph.getNeighbors(v))
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }
    std::cout << std::endl;
}

void dfsUtil(Graph &graph, int v, std::vector<bool> &visited)
{
    visited[v] = true;
    std::cout << graph.getVertexLabel(v) << " ";

    for (int neighbor : graph.getNeighbors(v))
    {
        if (!visited[neighbor])
            dfsUtil(graph, neighbor, visited);
    }
}

void dfs(Graph &graph, const std::string &startLabel)
{
    std::cout << "\n[DFS] Visiting from: " << startLabel << std::endl;

    int start = graph.getVertexIndex(startLabel);
    std::vector<bool> visited(graph.getVertexCount(), false);
    dfsUtil(graph, start, visited);
    std::cout << std::endl;
}

void dijkstra(Graph &graph, const std::string &startLabel)
{
    if (!graph.isWeighted())
    {
        std::cout << "\n[Dijkstra] Warning: Graph is not weighted. Skipping Dijkstra.\n";
        return;
    }

    int V = graph.getVertexCount();
    int start = graph.getVertexIndex(startLabel);

    std::vector<float> dist(V, std::numeric_limits<float>::infinity());
    std::vector<int> prev(V, -1);
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        for (int v : graph.getNeighbors(u))
        {
            float weight = graph.edgeWeight(u, v);
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    std::cout << "\n[Dijkstra] Shortest distances from " << startLabel << ":\n";
    for (int i = 0; i < V; ++i)
    {
        std::cout << "To " << graph.getVertexLabel(i) << " [" << i << "]: ";
        if (dist[i] == std::numeric_limits<float>::infinity())
        {
            std::cout << "unreachable\n";
            continue;
        }

        std::cout << "Distance = " << dist[i] << ", Path = ";

        std::vector<int> path;
        for (int at = i; at != -1; at = prev[at])
            path.push_back(at);
        for (int j = path.size() - 1; j >= 0; --j)
        {
            std::cout << graph.getVertexLabel(path[j]);
            if (j > 0)
                std::cout << " -> ";
        }
        std::cout << std::endl;
    }
}
