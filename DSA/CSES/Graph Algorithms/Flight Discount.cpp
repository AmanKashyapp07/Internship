// Link: https://cses.fi/problemset/task/1195

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

vector<ll> dijkstra(int src, vector<vector<pair<int,int>>>& graph)
{
    int n = graph.size() - 1;

    vector<ll> dist(n + 1, INF);
    priority_queue<
        pair<ll,int>,
        vector<pair<ll,int>>,
        greater<pair<ll,int>>
    > pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue;

        for (auto [v, w] : graph[u])
        {
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}
// this algorithm finds the shortest path from a source node to all other nodes in a weighted graph with non-negative weights. It uses a priority queue to efficiently get the next node with the smallest distance. The algorithm maintains a distance array that keeps track of the shortest known distance from the source to each node. When a shorter path to a node is found, it updates the distance and adds that node back into the priority queue for further exploration.
struct Edge
{
    int u, v, w;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> g(n + 1), rg(n + 1);
    vector<Edge> edges;

    while (m--)
    {
        int a, b, c;
        cin >> a >> b >> c;

        g[a].push_back({b, c});
        rg[b].push_back({a, c});

        edges.push_back({a, b, c});
    }

    auto dist1 = dijkstra(1, g);   // from source to all nodes
    auto dist2 = dijkstra(n, rg);  // from destination to all nodes in the reversed graph
    // after 2nd dijkstra, dist2[v] will give the shortest distance from node v to node n in the original graph, because we ran dijkstra on the reversed graph starting from node n.

    ll ans = INF;

    for (auto [u, v, w] : edges)
    {
        ans = min(ans,
                  dist1[u] +
                  w / 2 +
                  dist2[v]); // dist1[u] is the shortest distance from source to u, w/2 is the halved weight of the edge (u,v), and dist2[v] is the shortest distance from v to destination. The sum gives the total distance from source to destination using the edge (u,v) with halved weight.
    }

    cout << ans << '\n';
}