# Graph Navigation Project

This project implements a flexible graph data structure with support for both **adjacency matrix** and **adjacency list** representations. It allows for **directed/undirected** and **weighted/unweighted** graphs. The implementation includes core operations as well as common graph traversal algorithms like **Breadth-First Search (BFS)**, **Depth-First Search (DFS)**, and **Dijkstra’s algorithm**.

---

## 📂 Features

- Dual representation: adjacency list or matrix
- Vertex and edge manipulation:
  - Add/remove vertices
  - Add/remove edges
  - Check for edge existence
  - Retrieve edge weights and neighbors
- Graph printing
- Graph loading from file (with custom format)
- Graph traversal algorithms:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra’s shortest path (for weighted graphs)

---

## 📄 Input File Format

Graph data is loaded from a file with the following format:

Where:

- `V`: number of vertices
- `A`: number of edges
- `D`: 1 for directed, 0 for undirected
- `P`: 1 for weighted, 0 for unweighted

Each edge line includes source and destination labels. If the graph is weighted (`P = 1`), a weight must also be provided.

### Example

4 5 1 1
0 1 2.5
0 2 1.2
1 2 1.8
2 3 2.0
3 0 3.3

## Usage

Compile the code

```bash
g++ main.cpp -o main
```

Run the executable

```bash
./main
```

The `main.cpp` file demonstrates the usage of both adjacency matrix and adjacency list graph implementations. It includes adding vertices and edges, removing edges, and printing the graph.

## Contributing

If you would like to contribute, please fork the repository and use a feature branch. Pull requests are warmly welcome.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
