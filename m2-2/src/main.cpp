#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

#include "graph.hpp"
#include "utils.hpp"
#include "nav_algorithms.hpp"
#include "coloring_algorithms.hpp"
#include "max_flow.hpp"

using namespace std;

void printColoringResult(string name, pair<int, vector<int>> &result, Graph &g)
{
    cout << "\nAlgorithm: " << name << endl;
    cout << "Number of colors used: " << result.first << endl;

    if (g.getVertexCount() < 10)
    {
        cout << "Vertex colors:\n";
        for (int i = 0; i < result.second.size(); ++i)
        {
            cout << g.getVertexLabel(i) << ": Color " << result.second[i] << endl;
        }
    }
}

// Function to format duration in a human-readable way
string formatDuration(long long nanoseconds)
{
    double time = nanoseconds;
    ostringstream out;

    out << fixed << setprecision(3);

    if (time < 1'000)
    {
        out << time << " ns (Nanosegundo)";
    }
    else if (time < 1'000'000)
    {
        out << time / 1'000 << " µs (Microsegundo)";
    }
    else if (time < 1'000'000'000)
    {
        out << time / 1'000'000 << " ms (Milisegundo)";
    }
    else if (time < 60LL * 1'000'000'000)
    {
        out << time / 1'000'000'000 << " s (Segundo)";
    }
    else if (time < 3600LL * 1'000'000'000)
    {
        out << time / (60.0 * 1'000'000'000) << " min (Minuto)";
    }
    else
    {
        out << time / (3600.0 * 1'000'000'000) << " h (hora)";
    }

    out << "\n";

    return out.str();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <graph_file> [LIST|MATRIX]\n";
        return 1;
    }

    string filename = argv[1];

    // Default is LIST if not specified
    RepresentationType type = RepresentationType::LIST;

    if (argc >= 3)
    {
        string mode = argv[2];
        if (mode == "MATRIX")
        {
            type = RepresentationType::MATRIX;
        }
        else if (mode != "LIST")
        {
            cerr << "Invalid representation type. Use LIST or MATRIX.\n";
            return 1;
        }
    }

    Graph *g = loadGraphFromFile(filename, type);
    if (!g)
    {
        cerr << "Failed to load graph.\n";
        return 1;
    }

    g->printGraph();

    // --- Fluxo Máximo ---
    cout << "\nINFO: Calculando Fluxo Máximo com Ford-Fulkerson" << endl;

    int source = 0;
    int destination = g->getVertexCount() - 1;

    auto start = chrono::high_resolution_clock::now();
    int maxFlow = fordFulkerson(*g, source, destination);
    auto end = chrono::high_resolution_clock::now();
    cout << "Ford-Fulkerson: Fluxo Máximo = " << maxFlow << endl;
    cout << "Tempo de execução: " << formatDuration(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) << endl;

    // --- Busca Local ---
    cout << "\nINFO: Otimizando com Busca Local" << endl;
    start = chrono::high_resolution_clock::now();
    int improvedFlow = localSearch(*g, source, destination, 1000);
    end = chrono::high_resolution_clock::now();
    cout << "Busca Local: Fluxo Máximo Otimizado = " << improvedFlow << endl;
    cout << "Tempo de execução: " << formatDuration(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) << endl;

    delete g;
    return 0;
}