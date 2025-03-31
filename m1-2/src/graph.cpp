#include "Graph.hpp"
#include <iostream>
#include <iomanip>

Graph::Graph(bool directed, bool weighted, RepresentationType representation)
    : directed(directed), weighted(weighted), representation(representation) {}

bool Graph::addVertex(std::string label)
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
        matrix.push_back(std::vector<float>(index + 1, 0));
    }
    else
    {
        adjacencyList.emplace_back();
    }

    return true;
}

bool Graph::removeVertex(std::string label)
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

bool Graph::addEdge(int source, int destination, float weight)
{
    if (source >= getVertexCount() || destination >= getVertexCount())
        return false;
    float w = (weighted ? weight : 1.0f);

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

bool Graph::removeEdge(int source, int destination)
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

bool Graph::edgeExists(int source, int destination)
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

float Graph::edgeWeight(int source, int destination)
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

std::vector<int> Graph::getNeighbors(int vertex)
{
    std::vector<int> neighbors;
    if (representation == RepresentationType::MATRIX)
    {
        for (size_t i = 0; i < matrix[vertex].size(); i++)
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

void Graph::printGraph()
{
    if (representation == RepresentationType::MATRIX)
    {
        std::cout << "Adjacency Matrix:\n      ";
        for (size_t i = 0; i < indexToLabel.size(); ++i)
            std::cout << std::setw(8) << indexToLabel[i] + " [" + std::to_string(i) + "]";
        std::cout << std::endl;

        for (size_t i = 0; i < matrix.size(); ++i)
        {
            std::cout << std::setw(8) << indexToLabel[i] + " [" + std::to_string(i) + "]";
            for (float val : matrix[i])
                std::cout << std::setw(8) << val;
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Adjacency List:\n";
        for (size_t i = 0; i < adjacencyList.size(); i++)
        {
            std::cout << indexToLabel[i] << " [" << i << "] -> ";
            for (const auto &e : adjacencyList[i])
                std::cout << "(" << indexToLabel[e.destination] << " [" << e.destination << "], " << e.weight << ") ";
            std::cout << std::endl;
        }
    }
}

int Graph::getVertexCount()
{
    return indexToLabel.size();
}

int Graph::getVertexIndex(std::string label)
{
    return labelToIndex[label];
}

std::string Graph::getVertexLabel(int index)
{
    return indexToLabel[index];
}

bool Graph::isWeighted() const
{
    return weighted;
}
