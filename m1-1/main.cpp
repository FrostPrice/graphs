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
    matrixGraph.printGraph();

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

    listGraph.printGraph();

    return 0;
}