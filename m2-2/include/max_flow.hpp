#pragma once

#include <vector>
#include "graph.hpp"

int fordFulkerson(Graph original, int source, int sink);
int localSearch(Graph &graph, int source, int sink, int iterations);