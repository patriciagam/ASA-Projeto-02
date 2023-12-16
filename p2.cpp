#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void dfs1(int node, const vector<vector<int>>& graph, vector<bool>& visited, stack<int>& st) {
    visited[node] = true;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs1(neighbor, graph, visited, st);
        }
    }

    st.push(node);
}

void dfs2(int node, const vector<vector<int>>& reversedGraph, vector<bool>& visited, vector<int>& scc, int sccNumber) {
    visited[node] = true;
    scc[node] = sccNumber;

    for (int neighbor : reversedGraph[node]) {
        if (!visited[neighbor]) {
            dfs2(neighbor, reversedGraph, visited, scc, sccNumber);
        }
    }
}

vector<vector<int>> compressSCCs(int n, const vector<pair<int, int>>& relationships, const vector<int>& scc) {
    vector<vector<int>> compressedGraph(n + 1);

    for (const auto& rel : relationships) {
        int x = rel.first;
        int y = rel.second;
        int sccX = scc[x];
        int sccY = scc[y];

        if (sccX != sccY) {
            compressedGraph[sccX].push_back(sccY);
        }
    }

    return compressedGraph;
}

vector<vector<int>> findSCCs(int n, const vector<pair<int, int>>& relationships, vector<int>& scc) {
    vector<vector<int>> graph(n + 1);
    vector<vector<int>> reversedGraph(n + 1);

    for (const auto& rel : relationships) {
        int x = rel.first;
        int y = rel.second;
        graph[x].push_back(y);
        reversedGraph[y].push_back(x);
    }

    vector<bool> visited(n + 1, false);
    stack<int> st;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs1(i, graph, visited, st);
        }
    }

    fill(visited.begin(), visited.end(), false);
    int sccNumber = 0;

    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            ++sccNumber;
            dfs2(node, reversedGraph, visited, scc, sccNumber);
        }
    }

    // Return the compressed graph of SCCs
    return compressSCCs(n, relationships, scc);
}


int dfs(int node, const vector<vector<int>>& graph, vector<bool>& visited) {
    visited[node] = true;

    int max_jumps = 0;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            int jumps = 1 + dfs(neighbor, graph, visited);
            max_jumps = max(max_jumps, jumps);
        } else {
            max_jumps = max(max_jumps, 1);
        }
    }

    return max_jumps;
}

int calculateMaxJumps(int n, const vector<pair<int, int>>& relationships) {
    vector<int> scc(n + 1, 0);
    vector<vector<int>> sccGraph = findSCCs(n, relationships, scc);

    int sccCount = *max_element(scc.begin(), scc.end());

    int max_jumps = 0;

    for (int start_scc = 1; start_scc <= sccCount; ++start_scc) {
        vector<bool> visited(sccCount + 1, false);
        max_jumps = max(max_jumps, dfs(start_scc, sccGraph, visited));
    }

    return max_jumps;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<pair<int, int>> relationships(m);

    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &relationships[i].first, &relationships[i].second);
    }

    int result = calculateMaxJumps(n, relationships);

    printf("%d\n", result);

    return 0;
}
