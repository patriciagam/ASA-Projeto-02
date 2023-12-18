#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <climits>
#include <algorithm>

using namespace std;

struct Node {
    int id;
    int scc;
    int close_time;
};

void dfsForward(int startNode, const vector<vector<int>>& graph, vector<bool>& visited, stack<int>& st) {
    stack<pair<int, vector<int>::size_type>> dfsStack;
    dfsStack.push({startNode, 0});

    while (!dfsStack.empty()) {
        int node = dfsStack.top().first;
        vector<int>::size_type& index = dfsStack.top().second;

        if (index < graph[node].size()) {
            int neighbor = graph[node][index++];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                dfsStack.push({neighbor, 0});
            }
        } else {
            st.push(node);
            dfsStack.pop();
        }
    }
}

void findSCCs(int n, const vector<pair<Node*, Node*>>& edges) {
    vector<vector<Node*>> graph(n + 1);

    for (const auto& edge : edges) {
        Node* x = edge.first;
        Node* y = edge.second;
        graph[x->id].push_back(y);
    }

    printf("graph[1][0]->id = %d\n", graph[1][0]->id);
    vector<bool> visited(n + 1, false);
    stack<int> st;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            //dfsForward(i, graph, visited, st);
        }
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<pair<Node*, Node*>> edges(m);

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        Node* x_node = new Node;
        *x_node = {x, 0, 0};
        Node* y_node = new Node;
        *y_node = {y, 0, 0};
        edges[i] = {x_node, y_node};
    }

    // free memory

    if (m == 0 || n == 0) {
        printf("0\n");
        return 0;
    }

    findSCCs(n, edges);

    return 0;
}
