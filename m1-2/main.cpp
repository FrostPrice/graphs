#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>

using namespace std;

enum class RepresentationType
{
    MATRIX,
    LIST
};

struct Edge
{
    int destination;
    float weight;
};

class Graph
{
private:
    bool directed;
    bool weighted;
    RepresentationType representation;

    unordered_map<string, int> labelToIndex;
    vector<string> indexToLabel;

    vector<vector<float>> matrix;
    vector<list<Edge>> adjacencyList;

public:
    Graph(bool directed, bool weighted, RepresentationType representation)
        : directed(directed), weighted(weighted), representation(representation) {}

    bool addVertex(string label)
    {
        if (labelToIndex.count(label))
            return false;
        int index = indexToLabel.size();
        labelToIndex[label] = index;
        indexToLabel.push_back(label);

        if (representation == RepresentationType::MATRIX)
        {
            for (auto &row : matrix)
                row.push_back(0);
            matrix.push_back(vector<float>(index + 1, 0));
        }
        else
        {
            adjacencyList.emplace_back();
        }

        return true;
    }

    bool removeVertex(string label)
    {
        if (!labelToIndex.count(label))
            return false;

        int index = labelToIndex[label];
        labelToIndex.erase(label);
        indexToLabel.erase(indexToLabel.begin() + index);

        if (representation == RepresentationType::MATRIX)
        {
            matrix.erase(matrix.begin() + index);
            for (auto &row : matrix)
                row.erase(row.begin() + index);
        }
        else
        {
            adjacencyList.erase(adjacencyList.begin() + index);
            for (auto &edges : adjacencyList)
            {
                edges.remove_if([index](Edge e)
                                { return e.destination == index; });
                for (auto &e : edges)
                    if (e.destination > index)
                        e.destination--;
            }
        }

        for (auto &pair : labelToIndex)
            if (pair.second > index)
                pair.second--;

        return true;
    }

    bool addEdge(int source, int destination, float weight = 1.0)
    {
        if (source >= getVertexCount() || destination >= getVertexCount())
            return false;

        float w = (weighted ? weight : 1.0);

        if (representation == RepresentationType::MATRIX)
        {
            matrix[source][destination] = w;
            if (!directed)
                matrix[destination][source] = w;
        }
        else
        {
            adjacencyList[source].push_back({destination, w});
            if (!directed)
                adjacencyList[destination].push_back({source, w});
        }

        return true;
    }

    bool removeEdge(int source, int destination)
    {
        if (source >= getVertexCount() || destination >= getVertexCount())
            return false;

        if (representation == RepresentationType::MATRIX)
        {
            matrix[source][destination] = 0;
            if (!directed)
                matrix[destination][source] = 0;
        }
        else
        {
            adjacencyList[source].remove_if([destination](Edge e)
                                            { return e.destination == destination; });
            if (!directed)
                adjacencyList[destination].remove_if([source](Edge e)
                                                     { return e.destination == source; });
        }

        return true;
    }

    bool edgeExists(int source, int destination)
    {
        if (representation == RepresentationType::MATRIX)
            return matrix[source][destination] != 0;
        else
        {
            for (const auto &e : adjacencyList[source])
                if (e.destination == destination)
                    return true;
            return false;
        }
    }

    float edgeWeight(int source, int destination)
    {
        if (representation == RepresentationType::MATRIX)
            return matrix[source][destination];
        else
        {
            for (const auto &e : adjacencyList[source])
                if (e.destination == destination)
                    return e.weight;
            return 0;
        }
    }

    vector<int> getNeighbors(int vertex)
    {
        vector<int> neighbors;
        if (representation == RepresentationType::MATRIX)
        {
            for (int i = 0; i < matrix[vertex].size(); i++)
                if (matrix[vertex][i] != 0)
                    neighbors.push_back(i);
        }
        else
        {
            for (const auto &e : adjacencyList[vertex])
                neighbors.push_back(e.destination);
        }
        return neighbors;
    }

    void printGraph()
    {
        if (representation == RepresentationType::MATRIX)
        {
            cout << "Adjacency Matrix:\n      ";
            for (size_t i = 0; i < indexToLabel.size(); ++i)
                cout << setw(8) << indexToLabel[i] + " [" + to_string(i) + "]";
            cout << endl;

            for (size_t i = 0; i < matrix.size(); ++i)
            {
                cout << setw(8) << indexToLabel[i] + " [" + to_string(i) + "]";
                for (float val : matrix[i])
                    cout << setw(8) << val;
                cout << endl;
            }
        }
        else
        {
            cout << "Adjacency List:" << endl;
            for (int i = 0; i < adjacencyList.size(); i++)
            {
                cout << indexToLabel[i] << " [" << i << "] -> ";
                for (const auto &e : adjacencyList[i])
                    cout << "(" << indexToLabel[e.destination] << " [" << e.destination << "], " << e.weight << ") ";
                cout << endl;
            }
        }
    }

    int getVertexCount()
    {
        return indexToLabel.size();
    }

    int getVertexIndex(string label)
    {
        return labelToIndex[label];
    }

    string getVertexLabel(int index)
    {
        return indexToLabel[index];
    }
    bool isWeighted() const
    {
        return weighted;
    }
};

Graph *loadGraphFromFile(const string &filename, RepresentationType reprType)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return nullptr;
    }

    // Reading header line
    int V, A, D, P;
    string headerLine;
    getline(file, headerLine);
    stringstream ss(headerLine);
    if (!(ss >> V >> A >> D >> P))
    {
        cerr << "Error: invalid header. Expected format: V A D P\n";
        return nullptr;
    }

    // Create the graph
    Graph *graph = new Graph(D == 1, P == 1, reprType);

    // Add vertices (names from 0 to V-1)
    for (int i = 0; i < V; ++i)
    {
        graph->addVertex(to_string(i));
    }

    // Read edges
    string line;
    int edgeCount = 0;
    while (getline(file, line) && edgeCount < A)
    {
        string origin, destination;
        float weight = 1.0;

        stringstream ss(line);
        if (!(ss >> origin >> destination))
        {
            cerr << "Error in edge line: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (P == 1)
        {
            if (!(ss >> weight))
            {
                cerr << "Error: expected edge weight missing in line: '" << line << "'\n";
                delete graph;
                return nullptr;
            }
        }

        if (!graph->addEdge(graph->getVertexIndex(origin), graph->getVertexIndex(destination), weight))
        {
            cerr << "Error adding edge: " << origin << " -> " << destination << endl;
            delete graph;
            return nullptr;
        }

        edgeCount++;
    }

    if (edgeCount != A)
    {
        cerr << "Warning: number of edges read (" << edgeCount << ") differs from expected (" << A << ")\n";
    }

    file.close();
    return graph;
}

void bfs(Graph &graph, const string &startLabel)
{
    cout << "\n[BFS] Visiting from: " << startLabel << endl;

    int start = graph.getVertexIndex(startLabel);
    vector<bool> visited(graph.getVertexCount(), false);
    list<int> queue;

    visited[start] = true;
    queue.push_back(start);

    while (!queue.empty())
    {
        int v = queue.front();
        queue.pop_front();
        cout << graph.getVertexLabel(v) << " ";

        for (int neighbor : graph.getNeighbors(v))
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }
    cout << endl;
}

void dfsUtil(Graph &graph, int v, vector<bool> &visited)
{
    visited[v] = true;
    cout << graph.getVertexLabel(v) << " ";

    for (int neighbor : graph.getNeighbors(v))
    {
        if (!visited[neighbor])
            dfsUtil(graph, neighbor, visited);
    }
}

void dfs(Graph &graph, const string &startLabel)
{
    cout << "\n[DFS] Visiting from: " << startLabel << endl;

    int start = graph.getVertexIndex(startLabel);
    vector<bool> visited(graph.getVertexCount(), false);
    dfsUtil(graph, start, visited);
    cout << endl;
}

void dijkstra(Graph &graph, const string &startLabel)
{
    if (!graph.isWeighted())
    {
        cout << "\n[Dijkstra] Warning: Graph is not weighted. Dijkstra will not be executed.\n";
        return;
    }

    int V = graph.getVertexCount();
    int start = graph.getVertexIndex(startLabel);

    vector<float> dist(V, std::numeric_limits<float>::infinity());
    vector<int> prev(V, -1);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;

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

    // Print shortest paths
    cout << "\n[Dijkstra] Shortest distances from " << startLabel << ":\n";
    for (int i = 0; i < V; ++i)
    {
        cout << "To " << graph.getVertexLabel(i) << " [" << i << "]: ";
        if (dist[i] == std::numeric_limits<float>::infinity())
        {
            cout << "unreachable\n";
            continue;
        }

        cout << "Distance = " << dist[i] << ", Path = ";

        vector<int> path;
        for (int at = i; at != -1; at = prev[at])
            path.push_back(at);
        for (int j = path.size() - 1; j >= 0; --j)
        {
            cout << graph.getVertexLabel(path[j]);
            if (j > 0)
                cout << " -> ";
        }
        cout << endl;
    }
}

int main()
{

    string filename = "graph.txt";
    Graph *g = loadGraphFromFile(filename, RepresentationType::LIST); // ou MATRIX

    if (g)
    {
        g->printGraph();
        bfs(*g, "0");
        dfs(*g, "0");
        dijkstra(*g, "0");
        delete g;
    }

    return 0;
}
