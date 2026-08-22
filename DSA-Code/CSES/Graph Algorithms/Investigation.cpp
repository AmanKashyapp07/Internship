// Link: https://cses.fi/problemset/task/1202


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

const long long INF = 1e18;
const long long MOD = 1e9 + 7;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> graph(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
    }

    vector<long long> dist(n + 1, INF);
    vector<long long> ways(n + 1, 0); // ways[i] = number of shortest paths from node 1 to node i
    // we are applying Dijkstra's algorithm to find the shortest path from node 1 to node n, and at the same time counting the number of shortest paths to each node using the ways array. Whenever we find a shorter path to a node, we update its distance and set the number of ways to reach it equal to the number of ways to reach the previous node. If we find another path with the same distance, we add the number of ways to reach the previous node to the current number of ways for that node.
    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;

    dist[1] = 0;
    ways[1] = 1;
    pq.push({0, 1});
    vector<int> min_edges(n + 1, INF); // min_edges[i] = minimum number of edges in a shortest path from node 1 to node i
    min_edges[1] = 0;
    vector<int> max_edges(n + 1, 0); // max_edges[i] = maximum number of edges in a shortest path from node 1 to node i
    max_edges[1] = 0;
    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u])
            continue;

        for (auto [v, w] : graph[u])
        {
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                ways[v] = ways[u];
                min_edges[v] = min_edges[u] + 1;
                max_edges[v] = max_edges[u] + 1;
                pq.push({dist[v], v});
            }
            else if (dist[v] == dist[u] + w)
            {
                ways[v] = (ways[v] + ways[u]) % MOD;
                min_edges[v] = min(min_edges[v], min_edges[u] + 1);
                max_edges[v] = max(max_edges[v], max_edges[u] + 1);
            }
        }
    }

    cout<<dist[n]<<" "<<ways[n]<<" "<<min_edges[n]<<" "<<max_edges[n]<<"\n";

    return 0;
}