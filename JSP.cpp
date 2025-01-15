#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

// Определения для удобства
using Graph = unordered_map<int, vector<pair<int, int>>>; // Вершина -> [(сосед, вес)]
const int INF = numeric_limits<int>::max();

// Алгоритм Беллмана-Форда
bool bellmanFord(const Graph& graph, int source, unordered_map<int, int>& distances) {
    distances[source] = 0;

    for (int i = 0; i < graph.size(); ++i) {
        bool updated = false;
        for (const auto& [u, edges] : graph) {
            for (const auto& [v, weight] : edges) {
                if (distances[u] != INF && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    updated = true;
                }
            }
        }
        if (!updated) break;
    }

    // Проверка на отрицательный цикл
    for (const auto& [u, edges] : graph) {
        for (const auto& [v, weight] : edges) {
            if (distances[u] != INF && distances[u] + weight < distances[v]) {
                return false; // Обнаружен отрицательный цикл
            }
        }
    }

    return true;
}

// Алгоритм Дейкстры
unordered_map<int, int> dijkstra(const Graph& graph, int source) {
    unordered_map<int, int> distances;
    for (const auto& [vertex, _] : graph) {
        distances[vertex] = INF;
    }
    distances[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();

        if (currentDist > distances[u]) continue;

        for (const auto& [v, weight] : graph.at(u)) {
            int newDist = currentDist + weight;
            if (newDist < distances[v]) {
                distances[v] = newDist;
                pq.push({newDist, v});
            }
        }
    }

    return distances;
}

// Алгоритм Джонсона
unordered_map<int, unordered_map<int, int>> johnson(Graph graph) {
    int newVertex = -1; // Дополнительная вершина
    graph[newVertex] = {};

    for (const auto& [vertex, _] : graph) {
        if (vertex != newVertex) {
            graph[newVertex].push_back({vertex, 0});
        }
    }

    unordered_map<int, int> potentials;
    if (!bellmanFord(graph, newVertex, potentials)) {
        throw runtime_error("Граф содержит отрицательный цикл!");
    }

    // Перевзвешивание рёбер
    Graph reweightedGraph;
    for (const auto& [u, edges] : graph) {
        if (u == newVertex) continue;
        for (const auto& [v, weight] : edges) {
            reweightedGraph[u].push_back({v, weight + potentials[u] - potentials[v]});
        }
    }

    // Нахождение кратчайших путей с использованием Дейкстры
    unordered_map<int, unordered_map<int, int>> allPairsDistances;
    for (const auto& [vertex, _] : graph) {
        if (vertex == newVertex) continue;
        auto distances = dijkstra(reweightedGraph, vertex);
        for (const auto& [target, dist] : distances) {
            allPairsDistances[vertex][target] = dist - potentials[vertex] + potentials[target];
        }
    }

    return allPairsDistances;
}

// Пример использования
int main() {
    // Граф в формате: вершина -> [(сосед, вес)]
    Graph graph = {
        {0, {{1, 1}, {2, 4}}},
        {1, {{2, 2}, {3, 2}}},
        {2, {{3, 3}}},
        {3, {}}
    };

    try {
        auto result = johnson(graph);
        for (const auto& [source, distances] : result) {
            cout << "Кратчайшие пути из вершины " << source << ":\n";
            for (const auto& [target, distance] : distances) {
                cout << "  -> " << target << ": " << distance << "\n";
            }
        }
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }

    return 0;
}
