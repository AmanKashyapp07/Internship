/**
 * CSES 1202 - Investigation
 *
 * Description:
 * Find the minimum cost, the number of minimum-cost paths, and the min/max number of edges in a minimum-cost path from node 1 to node n.
 *
 * Approach:
 * - Use Dijkstra's algorithm.
 * - Maintain arrays: `dist` (min cost), `ways` (count of min cost paths), `min_edges` (min edges on a min cost path), and `max_edges` (max edges on a min cost path).
 * - During edge relaxation, if a shorter path is found, reset all values. If a path of equal cost is found, add to `ways` and update the min/max edges.
 *
 * Time Complexity: O(E * log(V))
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const long long INF = 1e18, MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0, u, v, w; i < m; i++) { cin >> u >> v >> w; adj[u].push_back({v, w}); }
    vector<long long> dist(n + 1, INF), ways(n + 1, 0);
    vector<int> min_edges(n + 1, 1e9), max_edges(n + 1, 0);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq; 
    dist[1] = 0; ways[1] = 1; min_edges[1] = 0; pq.push({0, 1});
    // Modified Dijkstra updating DP tables for shortest path stats 
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) { // Case 1: strictly shorter path found. Just copy ways, min_edges, max_edges from u to v
                dist[v] = dist[u] + w; ways[v] = ways[u];
                min_edges[v] = min_edges[u] + 1; max_edges[v] = max_edges[u] + 1;
                pq.push({dist[v], v});
            } else if (dist[u] + w == dist[v]) { // Case 2: alternative shortest path found, no need to push to pq since dist[v] is not updated. Just update ways, min_edges, max_edges, even if you push to pq, it won't affect correctness since dist[v] is not updated
                ways[v] = (ways[v] + ways[u]) % MOD;
                min_edges[v] = min(min_edges[v], min_edges[u] + 1);
                max_edges[v] = max(max_edges[v], max_edges[u] + 1);
            }
        }
    }
    cout << dist[n] << " " << ways[n] << " " << min_edges[n] << " " << max_edges[n] << "\n";
}