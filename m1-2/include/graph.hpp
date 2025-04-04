#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

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

    std::unordered_map<std::string, int> labelToIndex;
    std::vector<std::string> indexToLabel;

    std::vector<std::vector<float>> matrix;
    std::vector<std::list<Edge>> adjacencyList;

public:
    Graph(bool directed, bool weighted, RepresentationType representation);

    bool addVertex(std::string label);
    bool removeVertex(std::string label);
    bool addEdge(int source, int destination, float weight = 1.0);
    bool removeEdge(int source, int destination);
    bool edgeExists(int source, int destination);
    float edgeWeight(int source, int destination);
    std::vector<int> getNeighbors(int vertex);
    void printGraph();
    int getVertexCount();
    int getVertexIndex(std::string label);
    std::string getVertexLabel(int index);
    bool isWeighted() const;
};
