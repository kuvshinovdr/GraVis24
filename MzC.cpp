#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <chrono>
#include <thread>

using namespace std;

struct Edge
{
  int source;
  int target;
};

void MzCround(
    vector<vector<int>> &graph,
    vector<bool> &visited,
    vector<bool> &inStack,
    vector<int> &parent,
    int s,
    mt19937 &randGen,
    int &cycleCount)
{
  visited[s] = true;
  inStack[s] = true;

  shuffle(graph[s].begin(), graph[s].end(), randGen);

  for (int t : graph[s])
  {
    uniform_int_distribution<> dis(0, 1);
    bool random_bool = dis(randGen);
    Edge edge = {s, t};
    if (!visited[t] && !inStack[t])
    {
      parent[t] = s;
      cout << "Edge: " << edge.source << " -> " << edge.target << endl;
      MzCround(graph, visited, inStack, parent, t, randGen, cycleCount);
    }
    else if (parent[s] != t && cycleCount > 0 && random_bool)
    {
      cout << "Cycle: " << edge.source << " -> " << edge.target << endl;
      cycleCount--;
    }
  }

  inStack[s] = false;
}

void MzC(vector<vector<int>> &graph, int startNode, int size, int &cycleCount)
{
  vector<bool> visited(size * size, false);
  vector<bool> inStack(size * size, false);
  vector<int> parent(size * size, -1);

  random_device rd;
  mt19937 randGen(rd());

  MzCround(graph, visited, inStack, parent, startNode, randGen, cycleCount);
}

int main()
{
  int size;
  cout << "Enter the size of the square maze wall: ";
  cin >> size;

  int totalNodes = size * size;
  vector<vector<int>> graph(totalNodes);
  int cycleCount = totalNodes / 16;
  for (int i = 0; i < totalNodes; i++)
  {
    if (i % size != 0)
    { // left edge of the wall
      graph[i].push_back(i - 1);
    }
    if (i % size != size - 1)
    { // right edge of the wall
      graph[i].push_back(i + 1);
    }
    if (i >= size)
    { // top edge of the wall
      graph[i].push_back(i - size);
    }
    if (i < totalNodes - size)
    { // bottom edge of the wall
      graph[i].push_back(i + size);
    }
  }

  int startNode = 0; // start traversal from top-left corner

  MzC(graph, startNode, size, cycleCount);

  return 0;
}