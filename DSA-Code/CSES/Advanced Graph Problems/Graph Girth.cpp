// Link: https://cses.fi/problemset/task/1707
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    int ans = INF;
    for (int i = 1; i <= n; i++) {
        queue<pair<int, int>> q; q.push({i, -1});
        vector<int> dist(n + 1, INF); dist[i] = 0;

        while (!q.empty()) {
            auto [u, p] = q.front(); q.pop();
            for (int v : g[u]) {
                if (v == p) continue;
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1; q.push({v, u});
                } else {
                    ans = min(ans, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    cout << (ans == INF ? -1 : ans) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the girth (length of the shortest cycle) in an unweighted undirected graph (CSES 1707).
// - Approach: BFS from Every Vertex ($O(V \cdot (V + E))$).
// - Intuition: Running unweighted BFS from each vertex i finds shortest cycle containing i via cross-edges `dist[u] + dist[v] + 1`.
// - Complexity: Time: O(V(V + E)), Space: O(V + E).
