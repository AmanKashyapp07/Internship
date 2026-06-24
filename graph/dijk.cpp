/**
 * Problem: Flight Discount (https://cses.fi/problemset/task/1195)
 * Find the cheapest flight route from 1 to n with the option to discount one flight to half price.
 * Time: O(E log V) time, O(V + E) space.
 * Trick is to run Dijkstra's algorithm twice: once from the source and once from the destination on the reversed graph, then check all edges for the minimum cost using the discount.
 */
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

using ll = long long;
const ll INF = 1e18;
struct Edge { int u, v, w; };

vector<ll> dijkstra(int src, vector<vector<pair<int,int>>>& adj) {
    int n = adj.size() - 1;
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[src] = 0; pq.push({0, src});
    // Standard Dijkstra implementation using min-heap priority queue
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
        }
    }
    return dist;
} // returns vector holding shortest distances from src to all other nodes in the graph

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<pair<int,int>>> g(n + 1), rg(n + 1); vector<Edge> edges;
    for (int i = 0, a, b, c; i < m; i++) {
        cin >> a >> b >> c;
        g[a].push_back({b, c}); rg[b].push_back({a, c}); edges.push_back({a, b, c});
    }
    auto dist1 = dijkstra(1, g);  // Forward Dijkstra from source
    auto dist2 = dijkstra(n, rg); // Reverse Dijkstra from destination
    ll ans = INF;
    // Minimize dist1[u] + w/2 + dist2[v] for all edges
    for (auto [u, v, w] : edges) {
        if (dist1[u] != INF && dist2[v] != INF) ans = min(ans, dist1[u] + w / 2 + dist2[v]);
    }
    cout << ans << '\n';
}
