#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Graph *loadGraphFromFile(const std::string &filename, RepresentationType reprType)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }

    int V, A, D, P;
    std::string headerLine;
    std::getline(file, headerLine);
    std::stringstream ss(headerLine);
    if (!(ss >> V >> A >> D >> P))
    {
        std::cerr << "Error: invalid header format. Expected: V A D P\n";
        return nullptr;
    }

    Graph *graph = new Graph(D == 1, P == 1, reprType);

    for (int i = 0; i < V; ++i)
        graph->addVertex(std::to_string(i));

    std::string line;
    int edgeCount = 0;
    while (std::getline(file, line) && edgeCount < A)
    {
        std::string source, destination;
        float weight = 1.0;

        std::stringstream ss(line);
        if (!(ss >> source >> destination))
        {
            std::cerr << "Error reading edge line: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (P == 1 && !(ss >> weight))
        {
            std::cerr << "Error: edge weight expected but missing in line: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (!graph->addEdge(graph->getVertexIndex(source), graph->getVertexIndex(destination), weight))
        {
            std::cerr << "Error adding edge: " << source << " -> " << destination << std::endl;
            delete graph;
            return nullptr;
        }

        edgeCount++;
    }

    if (edgeCount != A)
        std::cerr << "Warning: number of edges read (" << edgeCount << ") differs from expected (" << A << ")\n";

    file.close();
    return graph;
}
