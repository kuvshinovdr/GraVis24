#include "CCS.hpp"
#include <iostream>

const int maxn = 1e5, MAXV = 1000;
int component[maxn]; // тут будут номера компонент
using namespace std;

struct Edgenode {
int y;
int weight;
Edgenode *next;
};

struct Graph {
Edgenode *edges[MAXV+1];
int degree[MAXV+1];
int nvertices;
int nedges;
} g;

void dfs(Graph g, int v, int num) {
    component[v] = num;
    for (int u = 0; u < g.degree[v]; u++)
        if (!component[g.edges[v][u].y]) // если номер не присвоен, то мы там ещё не были
            dfs(g, u, num);
}

int main () {
int num = 0;
for (int v = 0; v < n; v++)
    if (!component[v])
        dfs(g, v, ++num);
} //Конец псевдокода наверное, уточните, если нужно делать дальнейшую реализацию в плане визуализации.
