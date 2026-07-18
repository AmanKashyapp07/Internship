// Link: https://cses.fi/problemset/task/1203

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using Edge = pair<int, ll>;

const ll INF = 4e18;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
const ll MOD1 = uniform_int_distribution<ll>(1e9, 2e9)(rng);
const ll MOD2 = uniform_int_distribution<ll>(1e9, 2e9)(rng);

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<Edge>> graph(N + 1), revGraph(N + 1);

    while (M--) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        revGraph[v].push_back({u, w});
    }

    vector<ll> distF(N + 1, INF), distR(N + 1, INF);
    vector<pair<ll, ll>> waysF(N + 1, {0, 0}), waysR(N + 1, {0, 0});

    auto dijkstra = [&](vector<vector<Edge>> &g,
                        vector<ll> &dist,
                        vector<pair<ll, ll>> &ways,
                        int src) {

        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;

        dist[src] = 0;
        ways[src] = {1, 1};
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d != dist[u]) continue;

            for (auto [v, w] : g[u]) {
                if (d + w < dist[v]) {
                    dist[v] = d + w;
                    ways[v] = ways[u];
                    pq.push({dist[v], v});
                }
                else if (d + w == dist[v]) {
                    ways[v].first = (ways[v].first + ways[u].first) % MOD1;
                    ways[v].second = (ways[v].second + ways[u].second) % MOD2;
                }
            }
        }
    };

    dijkstra(graph, distF, waysF, 1);
    dijkstra(revGraph, distR, waysR, N);

    ll total1 = waysF[N].first;
    ll total2 = waysF[N].second;

    vector<int> ans;

    for (int u = 1; u <= N; u++) {
        if (distF[u] + distR[u] != distF[N]) continue;

        ll hash1 = waysF[u].first * waysR[u].first % MOD1;
        ll hash2 = waysF[u].second * waysR[u].second % MOD2;

        if (hash1 == total1 && hash2 == total2)
            ans.push_back(u);
    }

    cout << ans.size() << '\n';
    for (int x : ans)
        cout << x << ' ';
}