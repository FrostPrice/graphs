#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Graph *loadGraphFromFile(const std::string &filename, RepresentationType reprType)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return nullptr;
    }

    int V, A, D, P;
    std::string headerLine;
    std::getline(file, headerLine);
    std::stringstream ss(headerLine);
    if (!(ss >> V >> A >> D >> P))
    {
        std::cerr << "Erro: cabeçalho inválido. Esperado: V A D P\n";
        return nullptr;
    }

    Graph *graph = new Graph(D == 1, P == 1, reprType);

    for (int i = 0; i < V; ++i)
        graph->addVertex(std::to_string(i));

    std::string line;
    int edgeCount = 0;
    while (std::getline(file, line) && edgeCount < A)
    {
        std::string origem, destino;
        float peso = 1.0;

        std::stringstream ss(line);
        if (!(ss >> origem >> destino))
        {
            std::cerr << "Erro na linha de aresta: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (P == 1 && !(ss >> peso))
        {
            std::cerr << "Erro: peso da aresta esperado, mas ausente em: '" << line << "'\n";
            delete graph;
            return nullptr;
        }

        if (!graph->addEdge(graph->getVertexIndex(origem), graph->getVertexIndex(destino), peso))
        {
            std::cerr << "Erro ao adicionar aresta: " << origem << " -> " << destino << std::endl;
            delete graph;
            return nullptr;
        }

        edgeCount++;
    }

    if (edgeCount != A)
        std::cerr << "Aviso: número de arestas lidas (" << edgeCount << ") difere do especificado (" << A << ")\n";

    file.close();
    return graph;
}
