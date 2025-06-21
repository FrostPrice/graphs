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
#include "agm.hpp"

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

void printAGMResult(const string &name, const pair<int, vector<tuple<int, int, int>>> &result)
{

    if (result.second.size() <= 20)
    {
        cout << "Arestas da AGM:" << endl;
        for (const auto &[w, u, v] : result.second)
        {
            cout << u << " - " << v << " (peso: " << w << ")\n";
        }
    }

    cout << "\nÁrvore Geradora Mínima - " << name << endl;
    cout << "Peso total: " << result.first << endl;
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

    // --- AGM with Prim ---
    cout << "\nINFO: Calculating AGM with Prim" << endl;
    auto start = chrono::high_resolution_clock::now();
    auto primResult = prim_mst(*g);
    auto end = chrono::high_resolution_clock::now();
    printAGMResult("Prim", primResult);
    cout << "Execution time: " << formatDuration(chrono::duration_cast<chrono::nanoseconds>(end - start).count());

    // --- AGM with Kruskal ---
    cout << "\nINFO: Calculating AGM with Kruskal" << endl;
    start = chrono::high_resolution_clock::now();
    auto kruskalResult = kruskal_mst(*g);
    end = chrono::high_resolution_clock::now();
    printAGMResult("Kruskal", kruskalResult);
    cout << "Execution time: " << formatDuration(chrono::duration_cast<chrono::nanoseconds>(end - start).count());

    delete g;
    return 0;
}