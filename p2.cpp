#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// DFS in the forward direction to find the topological order
void dfsForward(int startNode, const vector<vector<int>>& graph,
                vector<bool>& visited, stack<int>& st) {
    // stack to keep track of nodes and their index in the adjacency list
    stack<pair<int, vector<int>::size_type>> dfsStack;
    dfsStack.push({startNode, 0});

    while (!dfsStack.empty()) {
        int node = dfsStack.top().first;
        vector<int>::size_type& index = dfsStack.top().second;

        // check if the node has unvisited neighbors
        if (index < graph[node].size()) {
            // retrieve the next unvisited neighbor
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
void dfsBackward(int node, const vector<vector<int>>& reversedGraph,
                 vector<bool>& visited, vector<int>& scc, int sccNumber) {
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

// Compress SCCs into a new graph
vector<vector<int>> compressSCCs(int n, const vector<pair<int, int>>& edges,
                                 const vector<int>& scc) {
    // initialize graph as an empty adjacency list
    vector<vector<int>> compressedGraph(n + 1);

    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        // retrieve the SCCs numbers of the nodes
        int sccX = scc[x];
        int sccY = scc[y];

        // if the nodes are in different SCCs, add an edge between the SCCs
        if (sccX != sccY) {
            compressedGraph[sccX].push_back(sccY);
        }
    }

    return compressedGraph;
}

// Find SCCs in the graph
vector<vector<int>> findSCCs(int n, const vector<pair<int, int>>& edges,
                             vector<int>& scc) {
    // initialize graph and reversed graph as empty adjacency lists
    vector<vector<int>> graph(n + 1);
    vector<vector<int>> reversedGraph(n + 1);

    // build the graph and the reversed graph
    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        graph[x].push_back(y);
        reversedGraph[y].push_back(x);
    }

    // dfs traversal in the forward direction to find the topological order
    vector<bool> visited(n + 1, false);
    stack<int> st;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsForward(i, graph, visited, st);
        }
    }

    // reset visited vector
    fill(visited.begin(), visited.end(), false);
    int sccNumber = 0;

    //dfs traversal on the reversed graph to find SCCs
    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            ++sccNumber;
            dfsBackward(node, reversedGraph, visited, scc, sccNumber);
        }
    }

    // return the compressed graph of SCCs
    return compressSCCs(n, edges, scc);
}

// Calculates maximum jumps from a given SCC
int dfs(int startNode, const vector<vector<int>>& graph, vector<bool>& visited) {
    // stack to keep track of nodes and their jumps
    stack<pair<int, int>> nodeStack;  // Pair: (node, jumps)
    nodeStack.push({startNode, 0});
    visited[startNode] = true;

    int maxJumps = 0;

    while (!nodeStack.empty()) {
        // retrieve the current node and its jumps
        auto currentPair = nodeStack.top();
        nodeStack.pop();

        int currentNode = currentPair.first;
        int jumps = currentPair.second;

        maxJumps = max(maxJumps, jumps);

        // flag to check if there are unvisited neighbors
        bool unvisitedNeighborFound = false;

        for (int neighbor : graph[currentNode]) {
            // if the neighbor has not been visited
            if (!visited[neighbor]) {
                // mark the neighbor as visited
                visited[neighbor] = true; 
                // push the neighbor to the stack and update its jumps
                nodeStack.push({neighbor, jumps + 1}); 
                unvisitedNeighborFound = true;
                // break the loop after finding the first unvisited neighbor 
                break;  
            }
        }

         // update maxJumps only if there are no unvisited neighbors
        if (!unvisitedNeighborFound) {
            maxJumps = max(maxJumps, jumps); 
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

    // dfs traversal on the SCC graph to find the maximum jumps
    for (int startSCC = 1; startSCC <= sccCount; ++startSCC) {
        // keeps track of visited SCCs
        vector<bool> visited(sccCount + 1, false);
        // update maxJumps with the maximum jumps from the current SCC
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

    if (m == 0 || n == 0) {
        printf("0\n");
        return 0;
    }

    int result = calculateMaxJumps(n, edges);

    printf("%d\n", result);

    return 0;
}
