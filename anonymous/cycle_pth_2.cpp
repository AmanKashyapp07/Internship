/**
 * Problem: Cycle Path Reconstruct
 * Link: N/A
 * Category: Graph / DFS
 * 
 * Description:
 * Detects and prints cycle path vertices.
 * 
 * Logic/Approach:
 * DFS tracking parent pointers to backtrack and reconstruct cycle path.
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> parent, state, cycle;
int startNode = -1, endNode = -1;

bool dfs(int u, vector<vector<int>>& adj) {
    state[u] = 1; // currently in recursion stack

    for (int v : adj[u]) {
        if (state[v] == 0) {
            parent[v] = u;

            if (dfs(v, adj))
                return true;
        }
        else if (state[v] == 1) { // back edge => cycle found
            startNode = v;
            endNode = u;
            return true;
        }
    }

    state[u] = 2; // fully processed
    return false;
}

vector<int> findCycle(int n, vector<vector<int>>& adj) {
    parent.assign(n + 1, -1);
    state.assign(n + 1, 0);

    startNode = endNode = -1;

    for (int i = 1; i <= n; i++) {
        if (state[i] == 0 && dfs(i, adj))
            break;
    }

    if (startNode == -1)
        return {};

    vector<int> cycle;

    cycle.push_back(startNode);

    for (int v = endNode; v != startNode; v = parent[v])
        cycle.push_back(v);

    cycle.push_back(startNode);

    reverse(cycle.begin(), cycle.end());

    return cycle;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    vector<int> cycle = findCycle(n, adj);

    if (cycle.empty()) {
        cout << "No cycle found\n";
    } else {
        cout << "Cycle: ";
        for (int x : cycle)
            cout << x << ' ';
        cout << '\n';
    }

    return 0;
}