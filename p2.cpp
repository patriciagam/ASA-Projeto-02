#include <algorithm>
#include <iostream>
#include <list>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Graph {
public:
    unordered_map<int, bool> visited;
    unordered_map<int, list<int>> adj;
    unordered_set<int> inCurrentPath;  // To detect cycles
    unordered_set<int> cycleNodes;     // To mark nodes in a cycle

    void addEdge(int x, int y) { adj[x].push_back(y); }
    void dfs(int node, stack<int>& s);
    bool hasCycle();

    // Create a new graph with cycles collapsed
    Graph createGraphWithCollapsedCycles();
};

void Graph::dfs(int node, stack<int>& s) {
    visited[node] = true;
    inCurrentPath.insert(node);

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, s);
        } else if (inCurrentPath.count(neighbor)) {
            cycleNodes.insert(node);  // Mark node as part of the cycle
        }
    }

    inCurrentPath.erase(node);
    s.push(node);
}

bool Graph::hasCycle() {
    for (const auto& entry : adj) {
        int node = entry.first;
        if (!visited[node] && !cycleNodes.count(node)) {
            stack<int> s;
            dfs(node, s);
        }
    }

    return !cycleNodes.empty();
}

Graph Graph::createGraphWithCollapsedCycles() {
    Graph newGraph;

    for (const auto& entry : adj) {
        int node = entry.first;
        if (!cycleNodes.count(node)) {
            for (int neighbor : adj[node]) {
                if (!cycleNodes.count(neighbor)) {
                    newGraph.addEdge(node, neighbor);
                }
            }
        }
    }

    return newGraph;
}

int maxJumps(Graph& graph, int n) {
    vector<int> distances(n + 1, 0);
    stack<int> s;

    for (int i = 1; i <= n; ++i) {
        if (!graph.visited[i]) {
            graph.dfs(i, s);
        }
    }

    // Create a new graph with cycles collapsed
    Graph newGraph = graph.createGraphWithCollapsedCycles();

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        for (int neighbor : newGraph.adj[current]) {
            distances[neighbor] = max(distances[neighbor], distances[current] + 1);
        }
    }

    return *max_element(distances.begin(), distances.end());
}


int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    Graph graph;

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        graph.addEdge(x, y);
    }

    // Create a new graph with cycles collapsed
    Graph collapsedGraph = graph.createGraphWithCollapsedCycles();

    int result = maxJumps(collapsedGraph, n);
    printf("%d\n", result);

    return 0;
}

