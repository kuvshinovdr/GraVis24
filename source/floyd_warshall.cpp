/// @file floyd_warshall.cpp
#include "../include/graph.hpp"
#include <vector>
#include <limits>

using namespace gravis24;

std::vector<std::vector<float>> dist(vertexCount, std::vector<float>(n, INF)); floyd_warshall(const DefaultGraphImplementation graph) {

    const float INF = std::numeric_limits<float>::infinity();
    int n = matrix.getVertexCount();
    std::vector<std::vector<float>> dist(vertexCount, std::vector<float>(n, INF));

    if (graph.hasAdjacencyMatrixView()){
        DenseAdjacencyMatrixView adjency_matrix = graph.getAdjacencyMatrixView();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) {
                    dist[i][j] = 0.0f; // Расстояние до себя равно 0
                } 
                else if(adjency_matrix[i][j]){
                    dist[i][j] = 1.0f; // Вес ребра
                } 
                else{
                    dist[i][j] = INF;
                }
            }
        }

        for (int k = 1; k <= n){
            for (int i = 1; k <= n){
                for (int j = 1; k <= n){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

    }

    return dist;
}