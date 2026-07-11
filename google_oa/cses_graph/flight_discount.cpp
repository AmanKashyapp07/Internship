/**
 * CSES 1195 - Flight Discount
 *
 * Problem:
 * Find the minimum cost from city 1 to city n if exactly one flight
 * can be taken at half price.
 *
 * Approach:
 * - Run Dijkstra twice:
 *      1. From city 1 on the original graph -> dist1.
 *      2. From city n on the reversed graph -> dist2.
 * - Try applying the discount on every edge:
 *      dist1[u] + w/2 + dist2[v]
 * - The minimum over all edges is the answer.
 *
 * Time:  O(E log V)
 * Space: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

using ll = long long;
const ll INF = 1e18;

struct Edge {
    int u, v, w;
};

vector<ll> dijkstra(int src, vector<vector<pair<int,int>>>& adj) {
    int n = adj.size() - 1;
    vector<ll> dist(n + 1, INF);
    using T = pair<ll,int>; // {distance, node}
    priority_queue<T, vector<T>, greater<T>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> graph(n + 1), revGraph(n + 1);
    vector<Edge> edges;

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        graph[u].push_back({v, w});
        revGraph[v].push_back({u, w});
        edges.push_back({u, v, w});
    }

    auto dist1 = dijkstra(1, graph);
    auto dist2 = dijkstra(n, revGraph);

    ll ans = INF;

    for (auto [u, v, w] : edges) {
        if (dist1[u] == INF || dist2[v] == INF) continue;
        ans = min(ans, dist1[u] + w / 2 + dist2[v]);
    }

    cout << ans << '\n';
}