/**
 * Problem: High Score (https://cses.fi/problemset/task/1673)
 * Find the maximum score (longest path) from node 1 to node n, detecting positive cycles that affect the path.
 * Time: O(V * E) time, O(V + E) space.
 */
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Edge { int u, v; long long w; };

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<Edge> edges; vector<vector<int>> rev(n + 1);
    for (int i = 0, a, b; i < m; i++) {
        long long w; cin >> a >> b >> w;
        edges.push_back({a, b, -w}); // Negate weights to find longest path using Bellman-Ford
        rev[b].push_back(a);
    }
    // BFS on reverse graph to find all nodes that can reach destination n
    vector<bool> reach(n + 1, false); queue<int> q;
    q.push(n); reach[n] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : rev[u]) if (!reach[v]) { reach[v] = true; q.push(v); }
    }
    const long long INF = 1e18;
    vector<long long> dist(n + 1, INF); dist[1] = 0;
    int x = -1;
    for (int i = 1; i <= n; i++) {
        x = -1;
        for (auto &[u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (reach[v]) x = v; // Update node if it can reach n
            }
        }
    }
    // If a node that can reach n gets updated on the N-th iteration, an infinite loop is detected
    if (x != -1 && dist[x] != INF) cout << -1 << '\n';
    else cout << -dist[n] << '\n';
}
