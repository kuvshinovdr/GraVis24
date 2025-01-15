#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
    int to;     // вершина, в которую ведет ребро
    int weight; // вес ребра
};

vector<int> dijkstra(int start, const vector<vector<Edge>>& graph) { // Функция для поиска кратчайшего пути с использованием алгоритма Дейкстры
    int n = graph.size();
    vector<int> dist(n, numeric_limits<int>::max()); // Инициализация расстояний
    dist[start] = 0;
    // Приоритетная очередь для хранения вершин и их расстояний
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start }); // Добавляем стартовую вершину в очередь
    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_vertex = pq.top().second;
        pq.pop();
        // Если текущее расстояние больше, чем уже найденное, пропускаем
        if (current_dist > dist[current_vertex]) {
            continue;
        }
        // Обходим все соседние вершины
        for (const Edge& edge : graph[current_vertex]) {
            int new_dist = current_dist + edge.weight;
            // Если найдено более короткое расстояние, обновляем его
            if (new_dist < dist[edge.to]) {
                dist[edge.to] = new_dist;
                pq.push({ new_dist, edge.to });
            }
        }
    }
    return dist; 
}

int main() {
    setlocale(LC_ALL, "RU");
    int n, m; // n - кол-во вершин, m - кол-во рёбер
    cout << "Введите количество вершин и рёбер: ";
    cin >> n >> m;
    vector<vector<Edge>> graph(n);
    cout << "Введите рёбра (формат: вершина1 вершина2 вес):" << endl;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({ v, w }); // Добавляем ребро в граф
        graph[v].push_back({ u, w }); // Для неориентированного графа
    }
    int start;
    cout << "Введите стартовую вершину: ";
    cin >> start;
    vector<int> distances = dijkstra(start, graph);
    cout << "Кратчайшие расстояния от вершины " << start << ":" << endl;
    for (int i = 0; i < n; ++i) {
        if (distances[i] == numeric_limits<int>::max()) {
            cout << "До вершины " << i << " нет пути." << endl;
        }
        else {
            cout << "До вершины " << i << " расстояние: " << distances[i] << endl;
        }
    }
    return 0;
}