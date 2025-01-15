#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
private:
    int v; // Количество вершин
    vector<vector<int>> adj; // Список смежности
    vector<bool> visited; // Информация о посещении 
    vector<int> disc; // Время входа в вершину
    vector<int> low;  // Минимальное время входа, достижимое из поддерева
    vector<int> parent; // Родительская вершина
    vector<bool> articulationPoints; // Является ли вершина шарниром

    void DFS(int u) {
        int time = 0;
        visited[u] = true;
        disc[u] = low[u] = ++time;
        int children = 0; // Количество дочерних вершин

        for (int v : adj[u]) {
            if (!visited[v]) {
                children++;
                parent[v] = u;
                DFS(v);

                //low[u] позволяет понять, можно ли добраться до вершины, предшествующей u, без прохождения через u
                low[u] = min(low[u], low[v]);

                if (parent[u] == -1 && children > 1) // Корень и больше одного ребенка
                    articulationPoints[u] = true;

                if (parent[u] != -1 && low[v] >= disc[u]) // Не корень и низкое значение >= времени входа
                    articulationPoints[u] = true;
            }
            else if (v != parent[u]) {
                low[u] = min(low[u], disc[v]);
            }
        }
    }

public:
    Graph(int V) : v(V), adj(V), visited(V, false), disc(V, 0), low(V, 0), parent(V, -1), articulationPoints(V, false) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void findArticulationPoints() {
        for (int i = 0; i < v; i++) {
            if (!visited[i])
                DFS(i);
        }

        cout << "Точки разрыва графа:" << "\n";
        for (int i = 0; i < v; i++) {
            if (articulationPoints[i])
                cout << i << " ";
        }
        cout << "\n";
    }
};

int main() {
    Graph g(5);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    g.findArticulationPoints();

    return 0;
}