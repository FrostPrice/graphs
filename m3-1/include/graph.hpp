#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

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
    Graph(bool directed, bool weighted, RepresentationType representation);

    bool addVertex(string label);
    bool removeVertex(string label);
    bool addEdge(int source, int destination, float weight = 1.0);
    bool removeEdge(int source, int destination);
    bool edgeExists(int source, int destination);
    float edgeWeight(int source, int destination) const;
    vector<int> getNeighbors(int vertex);
    void printGraph();
    int getVertexCount();
    int getVertexIndex(string label);
    string getVertexLabel(int index);
    bool isWeighted() const;
    Graph copy() const;
    float getCapacity(int u, int v) const;
    void addFlow(int u, int v, float delta);
    vector<tuple<int, int, float>> getEdges() const;
};
