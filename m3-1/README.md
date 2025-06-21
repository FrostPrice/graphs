# � Graph Algorithms Project

This project implements a modular and efficient **graph algorithms system** in C++, supporting multiple algorithmic strategies over graphs. It includes **exact** and **heuristic** algorithms for various graph problems, designed for performance benchmarking and analysis on graphs of various sizes.

---

## 📂 Features

### 🎨 Graph Coloring

- ✅ **Brute-force** (exact)
- ✅ **Welsh-Powell** heuristic
- ✅ **DSATUR** heuristic
- ✅ **Simple greedy** heuristic

### 🔁 Maximum Flow

- ✅ **Ford-Fulkerson** algorithm using DFS
- ✅ **Local Search optimization** (inverting edge directions to maximize flow)

### 🌳 Minimum Spanning Tree (AGM)

- ✅ **Prim's** algorithm
- ✅ **Kruskal's** algorithm with Disjoint Set Union (DSU)

### General

- ✅ Dual graph representation: **Adjacency List** and **Matrix**
- ✅ Graph input from structured text files
- ✅ Outputs:
  - Execution time
  - Number of colors (coloring)
  - Maximum flow values
  - MST total weight and edges (for small graphs ≤20 edges)
  - Colored vertex list for small graphs (≤10 vertices)
- ✅ Modular codebase

---

## 📄 Input File Format

Graph data is read from plain text files using the following structure:

```txt
V A D P
source destination weight
...
```

Where:

- **V**: number of vertices
- **A**: number of edges
- **D**: 0 or 1 (indicating undirected or directed graph)
- **P**: 0 or 1 (indicating if the graph is unweighted or weighted)

**Note**: MST algorithms require weighted graphs (P = 1).

### Example

For a weighted graph suitable for MST algorithms:

```txt
4 5 0 1
0 1 2.5
0 2 1.2
1 2 1.8
2 3 2.0
3 0 3.3
```

For an unweighted graph suitable for coloring:

```txt
4 4 0 0
0 1
0 2
1 2
2 3
```

---

## ⚙️ Usage

Compile the code with CMake:

```bash
mkdir build
cd build
cmake ..
make
```

Run the executable:

```bash
./GraphApp <path_to_graph_file> [LIST|MATRIX]
```

If you prefer compiling manually without CMake:

```bash
g++ -std=c++17 src/*.cpp -I include -o GraphApp
```

---

## 🧪 Notes

- Algorithms are benchmarked using `std::chrono` for accurate timing.
- **Graph Coloring**: Only undirected, unweighted graphs are supported.
- **Maximum Flow**: Supports directed, weighted graphs.
- **MST (AGM)**: Requires undirected, weighted graphs for meaningful results.
- Results for small graphs print detailed information (edges, colorings, etc.).
- Output messages for MST algorithms are displayed in Portuguese ("Árvore Geradora Mínima").
- The project includes implementations of classic algorithms:
  - **Prim's algorithm**: Grows the MST from a starting vertex
  - **Kruskal's algorithm**: Uses Union-Find (DSU) to detect cycles

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
