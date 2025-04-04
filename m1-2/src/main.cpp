#include "graph.hpp"
#include "utils.hpp"
#include "nav_algorithms.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <arquivo_grafo>\n";
        return 1;
    }

    std::string filename = argv[1];

    Graph *g = loadGraphFromFile(filename, RepresentationType::LIST); // or MATRIX

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
