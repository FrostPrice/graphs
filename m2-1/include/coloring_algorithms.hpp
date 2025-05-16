#pragma once

#include <vector>
#include "graph.hpp"

using namespace std;

// All algorithms are going to return {number_of_colors_used, color_assignment_vector}.

// Brute-force coloring: tries all color assignments until a valid one is found.
pair<int, vector<int>> bruteForce(Graph &g);

// Welsh-Powell heuristic: orders vertices by descending degree before greedy coloring.
pair<int, vector<int>> welshPowell(Graph &g);

// DSATUR heuristic: selects next vertex based on saturation (number of distinct neighbor colors).
pair<int, vector<int>> dsatur(Graph &g);

// Simple greedy coloring: assigns the smallest available color to each vertex in arbitrary order.
pair<int, vector<int>> greedy(Graph &g);
