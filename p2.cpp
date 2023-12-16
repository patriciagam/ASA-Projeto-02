#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// Depth-First Search in the forward direction
void dfsForward(int node, const vector<vector<int>>& graph, vector<bool>& visited, stack<int>& st) {
    visited[node] = true;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsForward(neighbor, graph, visited, st);
        }
    }

    st.push(node);
}

// DFS in the backward direction to assign Strongly Connected Components
void dfsBackward(int node, const vector<vector<int>>& reversedGraph, vector<bool>& visited, vector<int>& scc, int sccNumber) {
    visited[node] = true;
    scc[node] = sccNumber;

    for (int neighbor : reversedGraph[node]) {
        if (!visited[neighbor]) {
            dfsBackward(neighbor, reversedGraph, visited, scc, sccNumber);
        }
    }
}

// Compress SCCs into a new graph
vector<vector<int>> compressSCCs(int n, const vector<pair<int, int>>& edges, const vector<int>& scc) {
    vector<vector<int>> compressedGraph(n + 1);

    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        int sccX = scc[x];
        int sccY = scc[y];

        if (sccX != sccY) {
            compressedGraph[sccX].push_back(sccY);
        }
    }

    return compressedGraph;
}

// Find SCCs in the graph
vector<vector<int>> findSCCs(int n, const vector<pair<int, int>>& edges, vector<int>& scc) {
    vector<vector<int>> graph(n + 1);
    vector<vector<int>> reversedGraph(n + 1);

    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        graph[x].push_back(y);
        reversedGraph[y].push_back(x);
    }

    vector<bool> visited(n + 1, false);
    stack<int> st;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsForward(i, graph, visited, st);
        }
    }

    fill(visited.begin(), visited.end(), false);
    int sccNumber = 0;

    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            ++sccNumber;
            dfsBackward(node, reversedGraph, visited, scc, sccNumber);
        }
    }

    // Return the compressed graph of SCCs
    return compressSCCs(n, edges, scc);
}

// Recursive function to calculate maximum jumps from a given SCC
int dfs(int node, const vector<vector<int>>& graph, vector<bool>& visited) {
    visited[node] = true;

    int maxJumps = 0;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            int jumps = 1 + dfs(neighbor, graph, visited);
            maxJumps = max(maxJumps, jumps);
        } else {
            maxJumps = max(maxJumps, 1);
        }
    }

    return maxJumps;
}

// Calculate the maximum jumps over all SCCs
int calculateMaxJumps(int n, const vector<pair<int, int>>& edges) {
    vector<int> scc(n + 1, 0);
    vector<vector<int>> sccGraph = findSCCs(n, edges, scc);

    int sccCount = *max_element(scc.begin(), scc.end());
    int maxJumps = 0;

    for (int startSCC = 1; startSCC <= sccCount; ++startSCC) {
        vector<bool> visited(sccCount + 1, false);
        maxJumps = max(maxJumps, dfs(startSCC, sccGraph, visited));
    }

    return maxJumps;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<pair<int, int>> edges(m);

    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &edges[i].first, &edges[i].second);
    }

    int result = calculateMaxJumps(n, edges);

    printf("%d\n", result);
    
    return 0;
}
