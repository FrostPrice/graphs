#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

class Graph
{
protected:
    bool directed;
    bool weighted;
    unordered_map<string, int> labelToIndex;
    vector<string> indexToLabel;

public:
    Graph(bool directed, bool weighted) : directed(directed), weighted(weighted) {}
    virtual bool addVertex(string label) = 0;
    virtual bool removeVertex(string label) = 0;
    virtual bool addEdge(int source, int destination, float weight = 1.0) = 0;
    virtual bool removeEdge(int source, int destination) = 0;
    virtual bool edgeExists(int source, int destination) = 0;
    virtual float edgeWeight(int source, int destination) = 0;
    virtual vector<int> getNeighbors(int vertex) = 0;
    virtual void printGraph() = 0;

    string getVertexLabel(int index)
    {
        return indexToLabel[index];
    }
    int getVertexIndex(string label)
    {
        return labelToIndex[label];
    }
};

class AdjacencyMatrixGraph : public Graph
{
private:
    vector<vector<float>> matrix;

public:
    AdjacencyMatrixGraph(bool directed, bool weighted) : Graph(directed, weighted) {}

    bool addVertex(string label) override
    {
        if (labelToIndex.count(label))
            return false;
        int index = indexToLabel.size();
        labelToIndex[label] = index;
        indexToLabel.push_back(label);
        for (auto &row : matrix)
        {
            row.push_back(0);
        }
        matrix.push_back(vector<float>(index + 1, 0));
        return true;
    }

    bool removeVertex(string label) override
    {
        if (!labelToIndex.count(label))
            return false;

        int index = labelToIndex[label];

        // Remove label from mappings
        labelToIndex.erase(label);
        indexToLabel.erase(indexToLabel.begin() + index);

        // Remove row and column from adjacency matrix
        matrix.erase(matrix.begin() + index);
        for (auto &row : matrix)
        {
            row.erase(row.begin() + index);
        }

        // Update indices in labelToIndex map
        for (auto &pair : labelToIndex)
        {
            if (pair.second > index)
            {
                pair.second--; // Shift index down
            }
        }

        return true;
    }

    bool addEdge(int source, int destination, float weight = 1.0) override
    {
        if (source >= matrix.size() || destination >= matrix.size())
            return false;
        matrix[source][destination] = weight;
        if (!directed)
            matrix[destination][source] = weight;
        return true;
    }

    bool removeEdge(int source, int destination) override
    {
        if (source >= matrix.size() || destination >= matrix.size())
            return false;
        matrix[source][destination] = 0;
        if (!directed)
            matrix[destination][source] = 0;
        return true;
    }

    bool edgeExists(int source, int destination) override
    {
        return matrix[source][destination] != 0;
    }

    float edgeWeight(int source, int destination) override
    {
        return matrix[source][destination];
    }

    vector<int> getNeighbors(int vertex) override
    {
        vector<int> neighbors;
        for (int i = 0; i < matrix[vertex].size(); i++)
        {
            if (matrix[vertex][i] != 0)
                neighbors.push_back(i);
        }
        return neighbors;
    }

    void printGraph() override
    {
        // TODO: Fix the length of each column
        cout << "Adjacency Matrix:" << endl;
        cout << "  ";
        for (const auto &label : indexToLabel)
        {
            cout << label << " ";
        }
        cout << endl;
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            cout << indexToLabel[i] << " ";
            for (float val : matrix[i])
            {
                cout << val << " ";
            }
            cout << endl;
        }
    }
};

struct Edge
{
    int destination;
    float weight;
};

class AdjacencyListGraph : public Graph
{
private:
    vector<list<Edge>> adjacencyList;

public:
    AdjacencyListGraph(bool directed, bool weighted) : Graph(directed, weighted) {}

    bool addVertex(string label) override
    {
        if (labelToIndex.count(label))
            return false;
        int index = indexToLabel.size();
        labelToIndex[label] = index;
        indexToLabel.push_back(label);
        adjacencyList.push_back(list<Edge>());
        return true;
    }

    bool removeVertex(string label) override
    {
        if (!labelToIndex.count(label))
            return false;

        int index = labelToIndex[label];

        // Remove vertex from mappings
        labelToIndex.erase(label);
        indexToLabel.erase(indexToLabel.begin() + index);

        // Remove vertex from adjacency list
        adjacencyList.erase(adjacencyList.begin() + index);

        // Update indices in labelToIndex map
        for (auto &pair : labelToIndex)
        {
            if (pair.second > index)
            {
                pair.second--; // Shift index down
            }
        }

        // Update adjacency lists to remove references to the deleted index
        for (auto &edges : adjacencyList)
        {
            edges.remove_if([index](Edge e)
                            { return e.destination == index; });

            // Adjust indices in edges
            for (auto &e : edges)
            {
                if (e.destination > index)
                {
                    e.destination--; // Shift indices down
                }
            }
        }

        return true;
    }

    bool addEdge(int source, int destination, float weight = 1) override
    {
        if (source >= adjacencyList.size() || destination >= adjacencyList.size())
            return false;
        adjacencyList[source].push_back({destination, weight});
        if (!directed)
            adjacencyList[destination].push_back({source, weight});
        return true;
    }

    bool removeEdge(int source, int destination) override
    {
        if (source >= adjacencyList.size() || destination >= adjacencyList.size())
            return false;
        adjacencyList[source].remove_if([destination](Edge e)
                                        { return e.destination == destination; });
        if (!directed)
        {
            adjacencyList[destination].remove_if([source](Edge e)
                                                 { return e.destination == source; });
        }
        return true;
    }

    bool edgeExists(int source, int destination) override
    {
        for (const auto &e : adjacencyList[source])
        {
            if (e.destination == destination)
                return true;
        }
        return false;
    }

    float edgeWeight(int source, int destination) override
    {
        for (const auto &e : adjacencyList[source])
        {
            if (e.destination == destination)
                return e.weight;
        }
        return 0;
    }

    vector<int> getNeighbors(int vertex) override
    {
        vector<int> neighbors;
        for (const auto &e : adjacencyList[vertex])
        {
            neighbors.push_back(e.destination);
        }
        return neighbors;
    }

    void printGraph() override
    {
        cout << "Adjacency List:" << endl;
        for (int i = 0; i < adjacencyList.size(); i++)
        {
            cout << indexToLabel[i] << " -> ";
            for (const auto &e : adjacencyList[i])
            {
                cout << "(" << indexToLabel[e.destination] << ", " << e.weight << ") ";
            }
            cout << endl;
        }
    }
};

int main()
{
    cout << "=== Adjacency Matrix Graph (Non-Directed) ===" << endl;
    AdjacencyMatrixGraph matrixGraph(false, true);
    matrixGraph.addVertex("A");
    matrixGraph.addVertex("B");
    matrixGraph.addVertex("C");
    matrixGraph.addEdge(0, 1, 5.5);
    matrixGraph.addEdge(1, 2, -3);
    matrixGraph.addEdge(2, 0, 2);
    matrixGraph.printGraph();

    // Remove edge from matrixGraph
    matrixGraph.removeEdge(0, 1);
    matrixGraph.removeEdge(0, 2);

    // Remove Vertex from matrixGraph
    matrixGraph.removeVertex("A");

    matrixGraph.printGraph();

    cout << "\n=== Adjacency List Graph (Non-Directed) ===" << endl;
    AdjacencyListGraph listGraph(false, true);
    listGraph.addVertex("A");
    listGraph.addVertex("B");
    listGraph.addVertex("C");
    listGraph.addEdge(0, 1, 5.5);
    listGraph.addEdge(1, 2, -3);
    listGraph.addEdge(2, 0, 2);
    listGraph.printGraph();

    // Remove edge from listGraph
    listGraph.removeEdge(0, 1);
    listGraph.removeEdge(0, 2);

    // Remove Vertex from listGraph
    listGraph.removeVertex("A");

    listGraph.printGraph();

    cout << "=== Adjacency Matrix Graph (Directed) ===" << endl;
    AdjacencyMatrixGraph matrixGraph(true, true); // Directed, Weighted Graph

    // Add vertices
    matrixGraph.addVertex("A");
    matrixGraph.addVertex("B");
    matrixGraph.addVertex("C");
    matrixGraph.addVertex("D");

    // Add directed edges
    matrixGraph.addEdge(0, 1, 5.5);
    matrixGraph.addEdge(1, 2, -3);
    matrixGraph.addEdge(2, 3, 2);
    matrixGraph.addEdge(3, 0, 4.1);

    cout << "Before removing vertex 'B':" << endl;
    matrixGraph.printGraph();

    // Remove a vertex
    matrixGraph.removeVertex("B");

    cout << "After removing vertex 'B':" << endl;
    matrixGraph.printGraph();

    cout << "\n=== Adjacency List Graph (Directed) ===" << endl;
    AdjacencyListGraph listGraph(true, true); // Directed, Weighted Graph

    // Add vertices
    listGraph.addVertex("A");
    listGraph.addVertex("B");
    listGraph.addVertex("C");
    listGraph.addVertex("D");

    // Add directed edges
    listGraph.addEdge(0, 1, 5.5);
    listGraph.addEdge(1, 2, -3);
    listGraph.addEdge(2, 3, 2);
    listGraph.addEdge(3, 0, 4.1);

    cout << "Before removing vertex 'B':" << endl;
    listGraph.printGraph();

    // Remove a vertex
    listGraph.removeVertex("B");

    cout << "After removing vertex 'B':" << endl;
    listGraph.printGraph();

    return 0;
}