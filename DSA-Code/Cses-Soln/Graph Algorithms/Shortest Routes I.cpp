// Link: https://cses.fi/problemset/task/1671
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> g(n + 1);
    while (m--) { int u, v, w; cin >> u >> v >> w; g[u].push_back({v, w}); }

    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist[1] = 0; pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
        }
    }

    for (int i = 1; i <= n; i++) cout << dist[i] << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find shortest path distances from node 1 to all other nodes in a directed weighted graph (CSES 1671).
// - Approach: Standard Single-Source Dijkstra Algorithm with Priority Queue.
// - Intuition: Min-heap pops closest unvisited vertex, relaxing neighbor distances in $O(E \log V)$ time.
// - Complexity: Time: O((V + E) \log V), Space: O(V + E).