#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>


// ������� ��� ��������, �������� �� ���� ����������
bool isBipartite(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();  // ���������� ������
    std::vector<int> color(n, -1);  // ������ ������, -1 ��������, ��� ������� �� ��������

    // ���������� ��� ������� (���� ����� ���� �� �������)
    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {  // ���� ������� ��� �� ��������
            std::queue<int> q;
            q.push(i);
            color[i] = 0;  // �������� � ����������� ������� � ���� 0 (�����)

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                // ������������ ���� ������� ������� �������
                for (int neighbor : graph[node]) {
                    if (color[neighbor] == -1) {  // ���� ����� ��� �� �������
                        color[neighbor] = 1 - color[node];  // �������� � ��������������� ����
                        q.push(neighbor);
                    }
                    else if (color[neighbor] == color[node]) {  // ���� ����� ����� ��� �� ����
                        return false;  // ���� �� ����������
                    }
                }
            }
        }
    }

    return true;  // ���� ����������
}

int main() {
    // ������ �����: 4 �������, ���� (0, 1), (0, 2), (1, 3)
    setlocale(LC_ALL, "RU");
    std::vector<std::vector<int>> graph = {
        {1, 2},  // ������� 0 ��������� � ��������� 1 � 2
        {0, 3},  // ������� 1 ��������� � ��������� 0 � 3
        {0},      // ������� 2 ��������� � �������� 0
        {1}       // ������� 3 ��������� � �������� 1
    };

    // ��������, �������� �� ���� ����������
    if (isBipartite(graph)) {
        std::cout << "���� ����������.\n";
    }
    else {
        std::cout << "���� �� ����������.\n";
    }

    return 0;
}