#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> g; // Список смежности графа
vector<bool> used; // Массив для проверки посещенных вершин
vector<int> tin, fup; // Временные метки и массивы для алгоритма
int timer;

void IS_BRIDGE(int u, int v) {
    cout << "Bridge found between vertices: " << u << " and " << v << endl;
}

void dfs(int v, int p = -1) {
    used[v] = true;
    tin[v] = fup[v] = timer++;

    for (int to : g[v]) {
        if (to == p) continue; // Пропускаем ребро, по которому пришли

        if (used[to]) {
            fup[v] = min(fup[v], tin[to]); // Если вершина уже посещена
        }
        else {
            dfs(to, v);
            fup[v] = min(fup[v], fup[to]);
            if (fup[to] > tin[v]) {
                IS_BRIDGE(v, to);
            }
        }
    }
}

void find_bridges(int n) {
    timer = 0;
    used.assign(n, false);
    tin.assign(n, -1);
    fup.assign(n, -1);

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }
}

int main() {
    int n, m; // Количество вершин и рёбер
    cout << "Enter number of vertices and edges: ";
    cin >> n >> m;

    g.resize(n);

    cout << "Enter the edges (u v):" << endl;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    cout << "Finding bridges in the graph...\n";
    find_bridges(n);

    return 0;
}
//Сначала вводится количество вершин и ребер через пробел
//затем перечисляются ребра(две вершины через пробел)
