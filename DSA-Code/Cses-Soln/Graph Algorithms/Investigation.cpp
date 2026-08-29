// Link: https://cses.fi/problemset/task/1202
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18, MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> g(n + 1);
    while (m--) {
        int u, v, w; cin >> u >> v >> w; g[u].push_back({v, w});
    }

    vector<long long> dist(n + 1, INF), ways(n + 1, 0);
    vector<int> minEdges(n + 1, 1e9), maxEdges(n + 1, 0);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[1] = 0; ways[1] = 1; minEdges[1] = 0; maxEdges[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;

        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                ways[v] = ways[u];
                minEdges[v] = minEdges[u] + 1;
                maxEdges[v] = maxEdges[u] + 1;
                pq.push({dist[v], v});
            } else if (dist[v] == dist[u] + w) {
                ways[v] = (ways[v] + ways[u]) % MOD;
                minEdges[v] = min(minEdges[v], minEdges[u] + 1);
                maxEdges[v] = max(maxEdges[v], maxEdges[u] + 1);
            }
        }
    }
    cout << dist[n] << " " << ways[n] << " " << minEdges[n] << " " << maxEdges[n] << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find min price, number of min price paths, min edges in a min price path, max edges in a min price path from 1 to n (CSES 1202).
// - Approach: Multi-attribute Dijkstra Relaxation.
// - Intuition: Update shortest path distances, modulo ways count, min edge count, and max edge count simultaneously during Dijkstra relaxations.
// - Complexity: Time: O((V + E) \log V), Space: O(V + E).