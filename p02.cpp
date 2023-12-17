#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>

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

void printSCC(const vector<int>& scc) {
    cout << "Strongly Connected Components:" << endl;
    int maxScc = *max_element(scc.begin(), scc.end());

    for (int i = 1; i <= maxScc; ++i) {
        cout << "SCC " << i << ": ";
        for (size_t j = 0; j < scc.size(); ++j) {
            if (scc[j] == i) {
                cout << j << " ";
            }
        }
        cout << endl;
    }
}

// DFS to calculate the max jumps without counting jumps within the same SCC
int dfs(int startNode, const vector<vector<int>>& graph, vector<bool>& visited, const vector<int>& scc) {
    // stack to keep track of nodes and their jumps
    stack<pair<int, int>> nodeStack;  // pair: (node, jumps)
    nodeStack.push({startNode, 0});
    visited[startNode] = true;

    int maxJumps = 0;

    printf("\n");
    printf("Debugging dfs dos jumps\n");

    while (!nodeStack.empty()) {
        // retrieve the current node and its jumps
        auto currentPair = nodeStack.top();
        nodeStack.pop();

        int currentNode = currentPair.first;
        int jumps = currentPair.second;

        // flag to check if the current node has unvisited neighbors
        bool unvisitedNeighbors = false;

        printf("currentNode: %d, jumps: %d\n", currentNode, jumps);

        for (int neighbor : graph[currentNode]) {
            printf("visited[neighbor]: %s\n", visited[neighbor] ? "true" : "false");
            printf("unvisitedNeighbors: %s\n", unvisitedNeighbors ? "true" : "false");
            if (!visited[neighbor]) {
                printf("Aquui .... neighbor: %d\n", neighbor);
                visited[neighbor] = true;
                unvisitedNeighbors = true;
                if (scc[neighbor] != scc[currentNode]) {
                    nodeStack.push({neighbor, jumps + 1});
                } else {
                    nodeStack.push({neighbor, jumps});
                }
                break;
            }
        }
        if (!unvisitedNeighbors) {
        maxJumps = max(maxJumps, jumps);
        }
    }
    
    return maxJumps;
}

// Function to build graphs from edges
void buildGraphs(const vector<pair<int, int>>& edges, int n,
                 vector<vector<int>>& graph, vector<vector<int>>& reversedGraph) {
    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        graph[x].push_back(y);
        reversedGraph[y].push_back(x);
    }
}

// Finds SCCs and calculate max jumps without counting jumps within the same SCC
int calculateMaxJumps(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> graph(n + 1);
    vector<vector<int>> reversedGraph(n + 1);

    buildGraphs(edges, n, graph, reversedGraph);

    vector<bool> visited(n + 1, false);
    stack<int> st;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsForward(i, graph, visited, st);
        }
    }

    fill(visited.begin(), visited.end(), false);
    int sccNumber = 0;
    vector<int> scc(n + 1, 0);

    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            ++sccNumber;
            dfsBackward(node, reversedGraph, visited, scc, sccNumber);
        }
    }

    printSCC(scc);

    int maxJumps = 0;
    
    for (int i = 1; i <= n; ++i) {
        fill(visited.begin(), visited.end(), false);
        if (!visited[i]) {
            int jumps = dfs(i, graph, visited, scc);
            maxJumps = max(maxJumps, jumps);
        }
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
