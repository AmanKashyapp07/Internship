// Link: https://cses.fi/problemset/task/1672
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, q; cin >> n >> m >> q;
    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));
    for (int i = 1; i <= n; i++) dist[i][i] = 0;

    while (m--) {
        int u, v; long long w; cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w); dist[v][u] = min(dist[v][u], w);
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    while (q--) {
        int u, v; cin >> u >> v;
        cout << (dist[u][v] == INF ? -1 : dist[u][v]) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Answer q shortest path queries between any pair of nodes (CSES 1672).
// - Approach: Floyd-Warshall All-Pairs Shortest Path ($O(V^3)$).
// - Intuition: Dynamic programming relaxation `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])` considers intermediate node k.
// - Complexity: Time: O(V^3 + Q), Space: O(V^2).