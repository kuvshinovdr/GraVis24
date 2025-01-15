#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Функция для нахождения кратчайшего пути с помощью поиска в ширину
vector<int> bfs(int start, int goal, const vector<vector<int>>& graph) {
    int n = graph.size();  // Определяем количество вершин в графе
    vector<int> parent(n, -1);  // Массив для отслеживания родительских вершин
    vector<bool> visited(n, false);  // Массив для отслеживания посещённых вершин
    queue<int> q;  // Очередь для BFS

    visited[start] = true;  // Отмечаем начальную вершину как посещенную
    q.push(start);  // Добавляем начальную вершину в очередь

    while (!q.empty()) {  // Основной цикл BFS
        int current = q.front();
        q.pop();

        // Если мы нашли целевую вершину, восстанавливаем путь
        if (current == goal) {
            vector<int> path;  // Массив для хранения пути
            for (int v = goal; v != -1; v = parent[v]) {
                path.push_back(v);  // Добавляем вершину в путь
            }
            reverse(path.begin(), path.end());  // Путь восстанавливается в обратном порядке
            return path;
        }
        for (int neighbor : graph[current]) {  // Проходим по всем соседям текущей вершины
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    return {};
}

int main() {
    setlocale(LC_ALL, "RU");
    vector<vector<int>> graph = {{1, 2}, {0, 3, 4}, {0, 4}, {1}, {1, 2}};
    int start = 0;  // Начальная вершина
    int goal = 3;   // Конечная вершина
    
    // Находим кратчайший путь
    vector<int> path = bfs(start, goal, graph);

    // Выводим путь
    if (!path.empty()) {
        cout << "Кратчайший путь: ";
        for (int v : path) {
            cout << v << " ";
        }
        cout << endl;
    }
    else {
        cout << "Путь не найден!" << endl;
    }

    return 0;
}
