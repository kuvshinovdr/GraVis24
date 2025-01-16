#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

class Graph {
public:
    Graph(int V); 
    void add_edge(int v, int w); 
    void print_Euler();

private:
    int V;
    list<int>* adjacency;
    bool check_ct();
    int deg(int v);
};

Graph::Graph(int V) {
    this->V = V;
    adjacency = new list<int>[V];
}

void Graph::add_edge(int v, int w) {
    adjacency[v].push_back(w); 
}

bool Graph::check_ct() {
    //ищем первую ненулевую вершину
    int i;
    for (i = 0; i < V; i++)
        if (adjacency[i].size() > 0)
            break;
    if (i == V)
        return true;

    // проверка связности
    vector<bool> visited(V, false);
    stack<int> stack;
    stack.push(i);
    visited[i] = true;

    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();

        for (auto u : adjacency[v]) {
            if (!visited[u]) {
                visited[u] = true;
                stack.push(u);
            }
        }
    }

    // все ли ненулевые вершины посещены
    for (i = 0; i < V; i++) {
        if (visited[i] == false && adjacency[i].size() > 0)
            return false;
    }
    return true;
}

int Graph::deg(int v) {
    return adjacency[v].size();
}

void Graph::print_Euler() {
    // проверка на существование эйлерова цикла
    for (int i = 0; i < V; i++)
        if (deg(i) % 2 != 0) {
            cout << "The Eulerian cycle does not exist." << endl;
            return;
        }

    if (!check_ct()) {
        cout << "The Eulerian cycle does not exist." << endl;
        return;
    }

    stack<int> current;
    list<int> s;
    current.push(0); 
    while (!current.empty()) {
        int v = current.top();

        if (deg(v) > 0) {
            current.push(adjacency[v].front()); 
            adjacency[v].pop_front(); 
        }
        else {
            s.push_front(v); 
            current.pop();
        }
    }

    cout << "The Eulerian cycle: ";
    for (auto vertex : s) {
        cout << vertex << " ";
    }
    cout << endl;
}

int main() {
    Graph g(5); 
    g.add_edge(0, 1);  g.add_edge(1, 2);
    g.add_edge(2, 0);  g.add_edge(2, 3);
    g.add_edge(3, 4);  g.add_edge(4, 2);
    g.add_edge(1, 4);
   
    g.print_Euler(); 

    return 0;
}
