cmake_minimum_required(VERSION 3.10)
project(GraphProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(GraphApp
    src/main.cpp
    src/graph.cpp
    src/nav_algorithms.cpp
    src/coloring_algorithms.cpp
    src/max_flow.cpp
    src/utils.cpp
)
