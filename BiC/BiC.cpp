#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>


// Функция для проверки, является ли граф двудольным
bool isBipartite(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();  // Количество вершин
    std::vector<int> color(n, -1);  // Массив цветов, -1 означает, что вершина не окрашена

    // Перебираем все вершины (граф может быть не связным)
    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {  // Если вершина ещё не посещена
            std::queue<int> q;
            q.push(i);
            color[i] = 0;  // Начинаем с окрашивания вершины в цвет 0 (синий)

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                // Обрабатываем всех соседей текущей вершины
                for (int neighbor : graph[node]) {
                    if (color[neighbor] == -1) {  // Если сосед ещё не окрашен
                        color[neighbor] = 1 - color[node];  // Окрасить в противоположный цвет
                        q.push(neighbor);
                    }
                    else if (color[neighbor] == color[node]) {  // Если сосед имеет тот же цвет
                        return false;  // Граф не двудольный
                    }
                }
            }
        }
    }

    return true;  // Граф двудольный
}

int main() {
    // Пример графа: 4 вершины, рёбра (0, 1), (0, 2), (1, 3)
    setlocale(LC_ALL, "RU");
    std::vector<std::vector<int>> graph = {
        {1, 2},  // Вершина 0 соединена с вершинами 1 и 2
        {0, 3},  // Вершина 1 соединена с вершинами 0 и 3
        {0},      // Вершина 2 соединена с вершиной 0
        {1}       // Вершина 3 соединена с вершиной 1
    };

    // Проверка, является ли граф двудольным
    if (isBipartite(graph)) {
        std::cout << "Граф двудольный.\n";
    }
    else {
        std::cout << "Граф не двудольный.\n";
    }

    return 0;
}