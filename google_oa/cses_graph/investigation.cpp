/**
 * CSES 1202 - Investigation
 *
 * Approach:
 * - Run Dijkstra from node 1.
 * - Along with shortest distance, maintain:
 *     dist  = shortest distance
 *     ways  = number of shortest paths
 *     mn    = minimum edges in a shortest path
 *     mx    = maximum edges in a shortest path
 * - On finding a shorter path, overwrite all values.
 * - On finding another shortest path, accumulate ways and update mn/mx.
 *
 * Time: O((V + E) log V)
 * Space: O(V + E)
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;


using ll = long long;
const ll INF = 1e18;
const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> adj(n + 1);

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    vector<ll> dist(n + 1, INF), ways(n + 1);
    vector<int> mn(n + 1, 1e9), mx(n + 1);

    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;

    dist[1] = 0;
    ways[1] = 1;
    mn[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if(d!=dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                ways[v] = ways[u];
                mn[v] = mn[u] + 1;
                mx[v] = mx[u] + 1;
                pq.push({dist[v], v});
            }
            else if (d + w == dist[v]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
                mn[v] = min(mn[v], mn[u] + 1);
                mx[v] = max(mx[v], mx[u] + 1);
            }
        }
    }

    cout << dist[n] << ' '
         << ways[n] << ' '
         << mn[n] << ' '
         << mx[n] << '\n';
}