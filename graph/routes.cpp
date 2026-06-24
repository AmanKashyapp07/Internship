/**
 * Problem: Investigation (https://cses.fi/problemset/task/1202)
 * Find shortest path cost, ways, and min/max edge counts from node 1 to n in a weighted graph.
 * Logic: initiate ways, min_edges, max_edges arrays and update them during Dijkstra's algorithm. Dijkstra will be normal except that when we find a new shortest path to a node, we update the ways, min_edges, and max_edges arrays accordingly. If we find an alternative path with the same cost, we update the ways, min_edges, and max_edges arrays accordingly.
 * Time: O(E log V) time, O(V + E) space.
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
