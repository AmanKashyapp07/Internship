// Link: https://cses.fi/problemset/task/1203
#include <bits/stdc++.h>
using namespace std;

const long long INF = 4e18;
mt19937_64 rng(1337);
const long long MOD1 = 1000000007, MOD2 = 1000000009;

void dijkstra(const vector<vector<pair<int, long long>>>& g, vector<long long>& dist, vector<pair<long long, long long>>& ways, int src) {
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist[src] = 0; ways[src] = {1, 1}; pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w; ways[v] = ways[u]; pq.push({dist[v], v});
            } else if (d + w == dist[v]) {
                ways[v].first = (ways[v].first + ways[u].first) % MOD1;
                ways[v].second = (ways[v].second + ways[u].second) % MOD2;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pair<int, long long>>> g(n + 1), rg(n + 1);
    while (m--) {
        int u, v; long long w; cin >> u >> v >> w;
        g[u].push_back({v, w}); rg[v].push_back({u, w});
    }

    vector<long long> distF(n + 1, INF), distR(n + 1, INF);
    vector<pair<long long, long long>> waysF(n + 1, {0, 0}), waysR(n + 1, {0, 0});

    dijkstra(g, distF, waysF, 1);
    dijkstra(rg, distR, waysR, n);

    long long tot1 = waysF[n].first, tot2 = waysF[n].second;
    vector<int> ans;

    for (int u = 1; u <= n; u++) {
        if (distF[u] + distR[u] != distF[n]) continue;
        long long h1 = (waysF[u].first * waysR[u].first) % MOD1;
        long long h2 = (waysF[u].second * waysR[u].second) % MOD2;
        if (h1 == tot1 && h2 == tot2) ans.push_back(u);
    }

    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << (i + 1 == (int)ans.size() ? '\n' : ' ');
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all cities that lie on EVERY shortest path from city 1 to city n (CSES 1203).
// - Approach: Dual Dijkstra (Forward & Reverse) + Dual Hash Way Counting (`waysF[u] * waysR[u] == totalWays`).
// - Intuition: A node u lies on all shortest paths iff $distF[u] + distR[u] == distF[n]$ AND total paths through u equals total shortest paths from 1 to n.
// - Complexity: Time: O((V + E) \log V), Space: O(V + E).