/**
 * CSES 1707 - Graph Girth
 *
 * Description:
 * Find the length of the shortest cycle (girth) in an undirected unweighted graph.
 *
 * Approach:
 * - Run Breadth-First Search (BFS) from every node.
 * - For a starting node, maintain distances from it.
 * - If we encounter an edge between two visited nodes that is not a back-edge to the parent,
 *   a cycle is found.
 * - The cycle length is `dist[u] + dist[v] + 1`.
 * - Keep the minimum cycle length found across all BFS traversals.
 *
 * Time Complexity: O(V * (V + E))
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    int ans = INF;
    // BFS from every node to find the shortest cycle (girth)
    for (int i = 1; i <= n; i++) {
        queue<pair<int,int>> q; q.push({i, -1});
        vector<int> dist(n + 1, INF); dist[i] = 0; // dist[v] = distance from source node i to node v
        while (!q.empty()) {
            auto [u, p] = q.front(); q.pop();
            for (int v : adj[u]) {
                if (v == p) continue;
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1; q.push({v, u});
                } else {
                    ans = min(ans, dist[u] + dist[v] + 1); // Cycle length calculation
                }
            }
        }
    }
    cout << (ans == INF ? -1 : ans) << '\n';
}