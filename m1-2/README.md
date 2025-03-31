# 🚀 Graph Navigation Project

This project implements a flexible and modular **graph data structure** with support for both **adjacency matrix** and **adjacency list** representations. It allows for **directed/undirected** and **weighted/unweighted** graphs, with key features like vertex/edge manipulation, graph loading from files, and traversal algorithms including **BFS**, **DFS**, and **Dijkstra's algorithm**.

---

## 📂 Features

- ✅ Dual representation: adjacency **list** or **matrix**
- ✅ Directed and undirected graphs
- ✅ Weighted and unweighted graphs
- ✅ Vertex and edge manipulation:
  - Add/remove vertices
  - Add/remove edges
  - Check for edge existence
  - Retrieve edge weights and neighbors
- ✅ Graph visualization via printing
- ✅ Graph file loading with flexible format
- ✅ Graph traversal algorithms:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra’s algorithm (for weighted graphs)

---

## 📄 Input File Format

Graph data is loaded from a plain text file with the following structure:

```txt
V A D P
source destination [weight]
```

Where:

    V: number of vertices

    A: number of edges

    D: 1 for directed graph, 0 for undirected

    P: 1 if graph is weighted, 0 if unweighted

    Each edge line includes two vertex labels. If the graph is weighted (P = 1), a weight value must also be provided.

### Example

| 4   | 5   | 1   | 1   |
| --- | --- | --- | --- |
| 0   | 1   | 2.5 |     |
| 0   | 2   | 1.2 |     |
| 1   | 2   | 1.8 |     |
| 2   | 3   | 2.0 |     |
| 3   | 0   | 3.3 |     |

## Usage

Compile the code with CMake

```bash
mkdir build
cd build
cmake ..
make
```

Run the executable

```bash
./GraphApp
```

If you want to compile without CMake, you can use the following command:

```bash
g++ -std=c++17 src/*.cpp -I include -o graph_app
```

## Contributing

If you would like to contribute, please fork the repository and use a feature branch. Pull requests are warmly welcome.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
