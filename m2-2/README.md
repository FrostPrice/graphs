# 🎨 Graph Coloring Project

This project implements a modular and efficient **graph coloring system** in C++, supporting multiple coloring strategies over undirected and unweighted graphs. It includes both **exact** and **heuristic** algorithms, designed for performance benchmarking and visualization on graphs of various sizes.

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

### General

- ✅ Dual graph representation: **Adjacency List** and **Matrix**
- ✅ Graph input from structured text files
- ✅ Outputs:
  - Execution time
  - Number of colors (coloring)
  - Maximum flow values
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
- **D**: 0 (only undirected supported)
- **P**: 0 (only unweighted supported)

### Example

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
- Only undirected, unweighted graphs are supported in this version.
- Results for small graphs print the coloring directly.

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
