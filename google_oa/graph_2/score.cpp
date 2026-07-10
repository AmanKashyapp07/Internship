/**
 * CSES 1673 - High Score
 *
 * Description:
 * Find the maximum score possible from room 1 to room n, detecting positive-weight cycles.
 *
 * Approach:
 * - Negate weights and run the Bellman-Ford algorithm.
 * - Reachability checking to confirm cycle-to-destination paths.
 *
 * Time Complexity: O(V * E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge { int u, v, w; };

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<Edge> edges; vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w; edges.push_back({u, v, -w}); adj[u].push_back(v); // Negate weights for longest path
    }
    vector<long long> dist(n + 1, LLONG_MAX); dist[1] = 0;
    // Bellman-Ford: relax edges N-1 times
    for (int i = 1; i < n; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != LLONG_MAX && dist[u] + w < dist[v]) dist[v] = dist[u] + w;
        }
    }
    vector<int> affected(n + 1, 0);
    // Propagate cycle updates to mark nodes affected by negative cycles
    for (int i = 1; i <= n; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != LLONG_MAX) {
                if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; affected[v] = 1; }
                if (affected[u]) affected[v] = 1;
            }
        }
    }
    // BFS to see if any cycle-affected node can reach destination n
    queue<int> q; vector<int> vis(n + 1, 0);
    for (int i = 1; i <= n; i++) if (affected[i]) { q.push(i); vis[i] = 1; }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == n) { cout << -1 << '\n'; return 0; }
        for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; q.push(v); }
    }
    cout << -dist[n] << '\n';
}