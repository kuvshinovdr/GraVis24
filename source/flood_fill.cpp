/// @file flood_fill.cpp
#include "../include/dense_adjacency_matrix.hpp"
#include <stack>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

using namespace gravis24;

void flood_fill(const DenseAdjacencyMatrixView& matrix, int startVertex, std::vector<bool>& visited) {
    std::stack<int> stack;    // Стек вершин для обработки
    stack.push(startVertex);

    while (!stack.empty()) {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex]) {
            visited[vertex] = true;

            // Visualize the process in terminal
            std::cout << "Visiting vertex: " << vertex << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pause for visualization

            auto row = matrix.getRow(vertex);
            for (int i = 0; i < matrix.getVertexCount(); ++i) {
                if (row.getBit(i) && !visited[i]) {
                    stack.push(i);
                }
            }
        }
    }
}