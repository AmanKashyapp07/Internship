// Link: https://cses.fi/problemset/task/1673

#include <bits/stdc++.h>
using namespace std;

#define int long long

struct Edge
{
    int u, v, w;
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges;
    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;

        edges.push_back({u, v, -w}); // negate weights, because we want to find the longest path, but Bellman-Ford finds the shortest path, so we negate the weights to find the longest path
        adj[u].push_back(v);
    }

    vector<long long> dist(n + 1, LLONG_MAX);
    dist[1] = 0;

    // Bellman-Ford
    for (int i = 1; i <= n - 1; i++)
    {
        for (auto [u, v, w] : edges)
        {
            if (dist[u] == LLONG_MAX)
                continue;

            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
            }
        }
    }

    vector<int> affected(n + 1, 0); // affected means the node is affected by a negative cycle, i.e. it can reach a negative cycle or is part of a negative cycle

    // nth relaxation => node affected by a negative cycle
    for (int i = 1; i <= n; i++)
    {
        for (auto [u, v, w] : edges)
        {
            if (dist[u] == LLONG_MAX)
                continue;

            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                affected[v] = 1;
            }

            if (affected[u])
                affected[v] = 1;
        }
    }

    // If any affected node can reach n => answer is infinite
    queue<int> q;
    vector<int> vis(n + 1, 0);

    for (int i = 1; i <= n; i++)
    {
        if (affected[i])
        {
            q.push(i);
            vis[i] = 1;
        }
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        if (u == n)
        {
            cout << -1 << '\n';
            return 0;
        }

        for (int v : adj[u])
        {
            if (!vis[v])
            {
                vis[v] = 1;
                q.push(v);
            }
        }
    }

    cout << -dist[n] << '\n';
    return 0;
}