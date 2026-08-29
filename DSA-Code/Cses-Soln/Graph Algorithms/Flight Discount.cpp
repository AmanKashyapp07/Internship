// Link: https://cses.fi/problemset/task/1195
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

vector<long long> dijkstra(int src, int n, const vector<vector<pair<int, int>>>& g) {
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist[src] = 0; pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
        }
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> g(n + 1), rg(n + 1);
    vector<array<int, 3>> edges;

    while (m--) {
        int u, v, w; cin >> u >> v >> w;
        g[u].push_back({v, w}); rg[v].push_back({u, w});
        edges.push_back({u, v, w});
    }

    auto d1 = dijkstra(1, n, g), d2 = dijkstra(n, n, rg);
    long long ans = INF;
    for (auto [u, v, w] : edges) {
        if (d1[u] != INF && d2[v] != INF) ans = min(ans, d1[u] + w / 2 + d2[v]);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find shortest route cost from city 1 to city n where exactly one flight cost is halved (CSES 1195).
// - Approach: Dual Dijkstra (Forward from 1 + Reverse from n).
// - Intuition: Total cost using edge $(u, v, w)$ with discount equals $\text{dist1}[u] + \lfloor w / 2 \rfloor + \text{dist2}[v]$.
// - Complexity: Time: O((V + E) \log V), Space: O(V + E).