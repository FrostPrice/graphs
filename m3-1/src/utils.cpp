#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Graph *loadGraphFromFile(const string &filename, RepresentationType reprType)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return nullptr;
    }

    int V, A, D, P;
    string headerLine;
    getline(file, headerLine);
    stringstream ss(headerLine);
    if (!(ss >> V >> A >> D >> P))
    {
        cerr << "Error: invalid header format. Expected: V A D P\n";
        return nullptr;
    }

    Graph *graph = new Graph(D == 1, P == 1, reprType);

    for (int i = 0; i < V; ++i)
        graph->addVertex(to_string(i));

    string line;
    int edgeCount = 0;
    while (getline(file, line) && edgeCount < A)
    {
        string source, destination;
        float weight = 1.0;

        stringstream ss(line);
        if (!(ss >> source >> destination))
        {
            cerr << "Error reading edge line: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (P == 1 && !(ss >> weight))
        {
            cerr << "Error: edge weight expected but missing in line: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (!graph->addEdge(graph->getVertexIndex(source), graph->getVertexIndex(destination), weight))
        {
            cerr << "Error adding edge: " << source << " -> " << destination << endl;
            delete graph;
            return nullptr;
        }

        edgeCount++;
    }

    if (edgeCount != A)
        cerr << "Warning: number of edges read (" << edgeCount << ") differs from expected (" << A << ")\n";

    file.close();
    return graph;
}
