#pragma once

#include "Graph.hpp"
#include <string>

void bfs(Graph &graph, const std::string &startLabel);
void dfs(Graph &graph, const std::string &startLabel);
void dijkstra(Graph &graph, const std::string &startLabel);
