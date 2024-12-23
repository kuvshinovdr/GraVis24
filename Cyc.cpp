#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <fstream>
#include <algorithm>

using namespace std;

// Граф, представленный списком смежности
class Graph {
public:
    int vertices; // Количество вершин
    vector<vector<int>> adjList; // Список смежности

    // Конструктор для инициализации графа с заданным числом вершин
    Graph(int v) : vertices(v), adjList(v) {}

    // Метод для добавления ребра между двумя вершинами
    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
    }

    // Метод для визуализации графа в формате DOT
    void visualize(const string &filename, const vector<int> &cycle = {}) {
        ofstream file(filename); // Открываем файл для записи
        file << "digraph G {\n";

        // Проходим по всем вершинам графа
        for (int i = 0; i < vertices; i++) {
            for (int j : adjList[i]) {
                file << "  " << i << " -> " << j;
                // Если ребро является частью цикла, выделяем его цветом
                if (!cycle.empty() && isInCycle(i, j, cycle)) {
                    file << " [color=red]";
                }
                file << ";\n";
            }
        }

        file << "}";
        file.close(); // Закрываем файл
        cout << "Graph saved to " << filename << "\n";
        cout << "Use Graphviz to visualize it: dot -Tpng " << filename << " -o graph.png\n";
    }

private:
    // Метод для проверки, принадлежит ли ребро (src, dest) найденному циклу
    bool isInCycle(int src, int dest, const vector<int> &cycle) {
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            if (cycle[i] == src && cycle[i + 1] == dest) {
                return true;
            }
        }
        return false;
    }
};

// Рекурсивный метод для поиска цикла в графе
bool dfs(int node, vector<bool> &visited, vector<bool> &recStack, const Graph &graph, vector<int> &cycle) {
    visited[node] = true; // Отмечаем вершину как посещенную
    recStack[node] = true; // Добавляем вершину в текущий стек вызовов
    cycle.push_back(node); // Добавляем вершину в потенциальный цикл

    // Проходим по всем соседям текущей вершины
    for (int neighbor : graph.adjList[node]) {
        if (!visited[neighbor]) { // Если соседняя вершина не посещена
            if (dfs(neighbor, visited, recStack, graph, cycle)) {
                return true; // Если найден цикл, возвращаем true
            }
        } else if (recStack[neighbor]) { // Если сосед в стеке, найден цикл
            cycle.push_back(neighbor);
            return true;
        }
    }

    cycle.pop_back(); // Убираем вершину из потенциального цикла
    recStack[node] = false; // Убираем вершину из стека вызовов
    return false;
}

// Метод для поиска цикла в графе
vector<int> findCycle(const Graph &graph) {
    vector<bool> visited(graph.vertices, false); // Вектор для отслеживания посещенных вершин
    vector<bool> recStack(graph.vertices, false); // Вектор для отслеживания стека вызовов
    vector<int> cycle; // Вектор для хранения найденного цикла

    // Проходим по всем вершинам графа
    for (int i = 0; i < graph.vertices; i++) {
        if (!visited[i]) { // Если вершина еще не посещена
            if (dfs(i, visited, recStack, graph, cycle)) {
                // Если найден цикл, обрабатываем его
                auto it = find(cycle.begin(), cycle.end(), cycle.back());
                if (it != cycle.end()) {
                    cycle.erase(cycle.begin(), it); // Убираем лишние элементы из цикла
                }
                return cycle; // Возвращаем найденный цикл
            }
        }
    }
    return {}; // Если циклы не найдены, возвращаем пустой вектор
}

int main() {
    int vertices, edges;
    cout << "Enter the number of vertices: ";
    cin >> vertices;

    Graph graph(vertices); // Создаем граф

    cout << "Enter the number of edges: ";
    cin >> edges;

    cout << "Enter the edges :\n";
    for (int i = 0; i < edges; i++) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    // Ищем цикл в графе
    vector<int> cycle = findCycle(graph);

    if (!cycle.empty()) {
        cout << "Cycle detected: ";
        for (int node : cycle) {
            cout << node << " "; // Выводим вершины, входящие в цикл
        }
        cout << endl;
    } else {
        cout << "No cycle detected." << endl; // Если цикл не найден
    }

    // Визуализация графа
    graph.visualize("graph.dot", cycle); // Сохраняем граф в формате DOT

    return 0;
}


// Пример ввода 
// Введите количество вершин : 6
// Введите количество ребер : 6
// Введите границы :
// 0 1
// 1 2
// 2 3
// 3 1
// 3 4
// 4 5
