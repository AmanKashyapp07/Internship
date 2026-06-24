/**
 * Problem: Graph Girth (https://cses.fi/problemset/task/1707)
 * Find the length of the shortest cycle in an undirected unweighted graph.
 * Logic - Run BFS from every node to find the shortest cycle (girth).
 * Running from every node is necessary because the shortest cycle may not include the starting node of the BFS.
 * Time: O(V * (V + E)) time, O(V + E) space.
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
