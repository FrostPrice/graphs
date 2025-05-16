#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

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
    float edgeWeight(int source, int destination);
    vector<int> getNeighbors(int vertex);
    void printGraph();
    int getVertexCount();
    int getVertexIndex(string label);
    string getVertexLabel(int index);
    bool isWeighted() const;
};
