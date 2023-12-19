#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// DFS in the forward direction to find the topological order
void dfsForward(int startNode, const vector<vector<int>>& graph, vector<bool>& visited, stack<int>& st) {
    // stack to keep track of nodes and their index in the adjacency list
    stack<pair<int, vector<int>::size_type>> dfsStack;
    dfsStack.push({startNode, 0});

    while (!dfsStack.empty()) {
        int node = dfsStack.top().first;
        vector<int>::size_type& index = dfsStack.top().second;

        if (index < graph[node].size()) {
            int neighbor = graph[node][index++];
            if (!visited[neighbor]) {
                // mark the neighbor as visited and push it to the stack
                visited[neighbor] = true;
                dfsStack.push({neighbor, 0});
            }
        } else {
            // if all neighbors are visited, push the node to the result stack
            st.push(node);
            dfsStack.pop();
        }
    }
}

// DFS in the backward direction to assign Strongly Connected Components
void dfsBackward(int node, const vector<vector<int>>& reversedGraph, vector<bool>& visited, vector<int>& scc, int sccNumber) {
    // stack to keep track of nodes and their index in the adjacency list 
    stack<int> dfsStack;
    dfsStack.push(node);

    while (!dfsStack.empty()) {
        int currentNode = dfsStack.top();
        dfsStack.pop();

        if (!visited[currentNode]) {
            // mark the node as visited and assign it to the current SCC number
            visited[currentNode] = true;
            scc[currentNode] = sccNumber;

            // explore the neighbors of the current node in the reversed graph
            for (int neighbor : reversedGraph[currentNode]) {
                // if the neighbor is not visited, push it to the stack
                if (!visited[neighbor]) {
                    dfsStack.push(neighbor);
                }
            }
        }
    }
}

// Find the maximum path length between SCCs
int maxPath(int n, const vector<pair<int, int>>& edges, const vector<int>& scc) {   
    // find the number of SCCs
    int numSCCs = *max_element(scc.begin(), scc.end());
    vector<vector<int>> outgoingEdges(n + 1);
    vector<int> maxPathLength(numSCCs + 1, 0);

    // find the outgoing edges between SCCs
    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        int sccX = scc[x];
        int sccY = scc[y];

        // add an edge between SCCs if the endpoints belong to different SCCs
        if (sccX != sccY) {
            outgoingEdges[sccX].push_back(sccY);
        }
    }

    // find the max path length for each SCC by traversing the outgoing edges
    for (int i = 1; i <= n; ++i) {
        for (int toSCC : outgoingEdges[i]) {
            maxPathLength[toSCC] = max(maxPathLength[toSCC], maxPathLength[i] + 1);
        }
    }

    // find the SCC with the maximum path length
    int maxPathSCC = max_element(maxPathLength.begin(), maxPathLength.end()) - maxPathLength.begin();

    // maximum path length i
    return maxPathLength[maxPathSCC];
}

int calculateMaxJumps(int n, const vector<pair<int, int>>& edges, vector<int>& scc) {
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

    // DFS to fill the stack with nodes in the order of their finishing times 
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsForward(i, graph, visited, st);
        }
    }

    fill(visited.begin(), visited.end(), false);
    int sccNumber = 0;

    // DFS on the reversed graph to find SCCs
    while (!st.empty()) {
        int node = st.top();
        st.pop();

        // if the node is not visited, perform DFS to find the SCC it belongs to
        if (!visited[node]) {
            ++sccNumber;
            dfsBackward(node, reversedGraph, visited, scc, sccNumber);
        }
    }

    // calculate the maximum path length between SCCs
    return maxPath(n, edges, scc);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<pair<int, int>> edges(m);

    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &edges[i].first, &edges[i].second);
    }

    if (m == 0 || n == 0) {
        printf("0\n");
        return 0;
    }

    vector<int> scc(n + 1, 0);

    int result = calculateMaxJumps(n, edges, scc);

    printf("%d\n", result);

    return 0;
}

