// Link: https://cses.fi/problemset/task/1673
#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v; long long w; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Edge> edges;
    vector<vector<int>> g(n + 1);
    while (m--) {
        int u, v; long long w; cin >> u >> v >> w;
        edges.push_back({u, v, -w}); g[u].push_back(v);
    }

    vector<long long> dist(n + 1, 1e18);
    dist[1] = 0;
    for (int i = 1; i <= n - 1; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != 1e18 && dist[u] + w < dist[v]) dist[v] = dist[u] + w;
        }
    }

    vector<int> affected(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != 1e18 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; affected[v] = 1;
            }
            if (affected[u]) affected[v] = 1;
        }
    }

    queue<int> q; vector<int> vis(n + 1, 0);
    for (int i = 1; i <= n; i++) if (affected[i]) { q.push(i); vis[i] = 1; }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == n) { cout << -1 << '\n'; return 0; }
        for (int v : g[u]) if (!vis[v]) { vis[v] = 1; q.push(v); }
    }
    cout << -dist[n] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum score path from node 1 to node n in a directed graph (CSES 1673).
// - Approach: Bellman-Ford on Negated Weights + Reachability Check for Positive Cycles.
// - Intuition: Negating edge weights transforms max score into min distance; if an affected positive cycle node can reach node n, output -1.
// - Complexity: Time: O(V \cdot E), Space: O(V + E).