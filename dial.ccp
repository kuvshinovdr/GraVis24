#include<bits/stdc++.h>
using namespace std;
# define INF 0x3f3f3f3f
 
// Этот класс представляет собой ориентированный граф, использующий представление списка смежности
class Graph
{
    int V; // Количество вершин
    // Во взвешенном графе нам нужно сохранить вершину и весоую пару для каждого ребра
    list< pair<int, int> > *adj;
 
public:
    Graph(int V); 
 
    // функция для добавления ребра к графику
    void addEdge(int u, int v, int w);
 
    // выводит кратчайший путь из s
    void shortestPath(int s, int W);
};
 
// Выделяет память для списка смежности
Graph::Graph(int V)
{
    this->V = V;
    adj = new list< pair<int, int> >[V];
}
 
// добавляет преимущество между u и v в весе w
void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}
 
// Выводит кратчайший путь от src ко всем остальным вершинам.
// W - максимальный вес ребра
void Graph::shortestPath(int src, int W)
{
    /* С каждым расстоянием итератор до этой вершины в 
    его сегмент сохраняется, чтобы вершину можно было удалить
    в O (1) во время обновления. Итак, 
    dist[i].first = расстояние i-й вершины от вершины src
    dist[i].second = итератор до вершины i в номере сегмента */
    vector<pair<int, list<int>::iterator> > dist(V);
 
    // Инициализировать все расстояния как бесконечные (INF)
    for (int i = 0; i < V; i++)
        dist[i].first = INF;
 
    // Создайте сегменты B[].
    // B[i] сохранить вершину метки расстояния i
    list<int> B[W * V + 1];
 
    B[0].push_back(src);
    dist[src].first = 0;
 
    //
    int idx = 0;
    while (1)
    {
        // Последовательно перебирайте ведра, пока одно не станет непустым
        // ведро найдено
        while (B[idx].size() == 0 && idx < W*V)
            idx++;
 
        // If all buckets are empty, we are done.
        if (idx == W * V)
            break;
 
        // Возьмите верхнюю вершину из корзины и вытащите ее
        int u = B[idx].front();
        B[idx].pop_front();
 
        // Обработайте все смежные элементы извлеченной вершины 'u' и 
        // обновите их расстояние, если требуется.
        for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            int v = (*i).first;
            int weight = (*i).second;
 
            int du = dist[u].first;
            int dv = dist[v].first;
 
            // Если есть короткий путь к v через u .
            if (dv > du + weight)
            {
                // Если dv не является INF, то он должен быть в B [dv]
                // bucket , поэтому удалите его запись с помощью итератора
                // в O(1)
                if (dv != INF)
                    B[dv].erase(dist[v].second);
 
                // обновление расстояния
                dist[v].first = du + weight;
                dv = dist[v].first;
 
                // перемещение вершины v в корзину обновленного расстояния
                B[dv].push_front(v);
 
                // сохранение обновленного итератора в dist[v].второй
                dist[v].second = B[dv].begin();
            }
        }
    }
 
    // Вывести кратчайшие расстояния, сохраненные в dist[]
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d     %d\n", i, dist[i].first);
}
 
// Программа-драйвер для тестирования методов класса graph
int main()
{
    // создайте график, приведенный на рисунке выше
    int V = 9;
    Graph g(V);
 
    // построение приведенного выше графика
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);
 
    // максимальное взвешенное ребро - 14
    g.shortestPath(0, 14);
 
    return 0;
}
