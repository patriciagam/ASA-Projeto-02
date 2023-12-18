#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <climits>
#include <algorithm>

using namespace std;

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

void dfsBackward(int node, const vector<vector<int>>& reversedGraph, vector<bool>& visited, vector<int>& scc, int sccNumber) {
    stack<int> dfsStack;
    dfsStack.push(node);

    while (!dfsStack.empty()) {
        int currentNode = dfsStack.top();
        dfsStack.pop();

        if (!visited[currentNode]) {
            visited[currentNode] = true;
            scc[currentNode] = sccNumber;

            for (int neighbor : reversedGraph[currentNode]) {
                if (!visited[neighbor]) {
                    dfsStack.push(neighbor);
                }
            }
        }
    }
}

vector<vector<int>> compressSCCs(int n, const vector<pair<int, int>>& edges, const vector<int>& scc) {
    vector<vector<int>> compressedGraph(n + 1);

    vector<vector<int>> outgoingEdges(n + 1);

    for (const auto& edge : edges) {
        int x = edge.first;
        int y = edge.second;
        int sccX = scc[x];
        int sccY = scc[y];

        if (sccX != sccY) {
            outgoingEdges[sccX].push_back(sccY);
        }
    }

    for (int i = 1; i <= n; ++i) {
        for (int toSCC : outgoingEdges[i]) {
            compressedGraph[i].push_back(toSCC);
        }
    }

    return compressedGraph;
}

void printGraph(const vector<vector<int>>& graph) {
    cout << "Compressed Graph:" << endl;
    for (std::vector<int>::size_type i = 1; i < graph.size(); ++i) {
        cout << "SCC " << i << ": ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

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

    return compressSCCs(n, edges, scc);
}

int dfs(int startNode, const vector<vector<int>>& graph) {
    int numNodes = graph.size();
    vector<int> distance(numNodes, INT_MIN);

    distance[startNode] = 0;

    for (int i = startNode; i < numNodes; ++i) {
        if (distance[i] != INT_MIN) {
            for (int neighbor : graph[i]) {
                if (distance[i] + 1 > distance[neighbor]) {
                    distance[neighbor] = distance[i] + 1;
                }
            }
        }
    }

    return *max_element(distance.begin(), distance.end());
}

int calculateMaxJumps(int n, const vector<pair<int, int>>& edges) {
    vector<int> scc(n + 1, 0);
    vector<vector<int>> sccGraph = findSCCs(n, edges, scc);

    int sccCount = *max_element(scc.begin(), scc.end());
    int maxJumps = 0;

    for (int startSCC = 1; startSCC <= sccCount; ++startSCC) {
        vector<bool> visited(sccCount + 1, false);
        maxJumps = max(maxJumps, dfs(startSCC, sccGraph));
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
