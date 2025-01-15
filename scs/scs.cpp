#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

class TarjanSCC {
private:
    vector<vector<int>> graph;
    vector<int> indices, lowLinks;
    vector<bool> onStack;
    stack<int> stk;  // Переименовали переменную, чтобы избежать конфликта
    vector<vector<int>> sccs;
    int index = 0;

    void dfs(int node) {
        indices[node] = lowLinks[node] = index++;
        stk.push(node);
        onStack[node] = true;

        for (int neighbor : graph[node]) {
            if (indices[neighbor] == -1) {
                dfs(neighbor);
                lowLinks[node] = min(lowLinks[node], lowLinks[neighbor]);
            } else if (onStack[neighbor]) {
                lowLinks[node] = min(lowLinks[node], indices[neighbor]);
            }
        }

        if (lowLinks[node] == indices[node]) {
            vector<int> scc;
            int v;
            do {
                v = stk.top(); stk.pop();
                onStack[v] = false;
                scc.push_back(v);
            } while (v != node);
            sccs.push_back(scc);
        }
    }

public:
    TarjanSCC(int n) : graph(n), indices(n, -1), lowLinks(n, -1), onStack(n, false) {}

    void addEdge(int u, int v) {
        graph[u].push_back(v);
    }

    vector<vector<int>> findSCCs() {
        for (int i = 0; i < graph.size(); ++i) {
            if (indices[i] == -1) dfs(i);
        }
        return sccs;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    TarjanSCC tarjan(n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        tarjan.addEdge(u, v);
    }

    vector<vector<int>> sccs = tarjan.findSCCs();

    for (const auto& scc : sccs) {
        for (int node : scc) {
            cout << node << " ";
        }
        cout << "\n";
    }

    return 0;
}