/**
 * CSES 1667 - Message Route
 *
 * Description:
 * Find the shortest path (minimum number of computers) from node 1 to node n in an undirected graph.
 * If a path exists, print the number of computers and the path itself.
 *
 * Approach:
 * - Use Breadth-First Search (BFS) to find the shortest path in an unweighted graph.
 * - Maintain a parent array to reconstruct the path after reaching the destination node n.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    vector<int> parent(n + 1, -1); queue<int> q;
    q.push(1); parent[1] = 0;
    // Standard BFS for finding the shortest path in an unweighted graph
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (parent[v] == -1) { parent[v] = u; q.push(v); }
        }
    }
    if (parent[n] == -1) { cout << "IMPOSSIBLE\n"; return 0; }
    vector<int> path;
    // Reconstruct the path from destination to source using parent pointers
    for (int curr = n; curr != 0; curr = parent[curr]) path.push_back(curr);
    reverse(path.begin(), path.end());
    cout << path.size() << '\n';
    for (int node : path) cout << node << ' ';
    cout << '\n';
}