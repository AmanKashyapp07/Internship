#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, ptr;

    Dinic(int n): n(n), g(n + 1), level(n + 1), ptr(n + 1) {}

    void addEdge(int u, int v, ll cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;

        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto &e : g[v]) {
                if (e.cap && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(int v, int t, ll f) {
        if (v == t || f == 0) return f;

        for (int &i = ptr[v]; i < (int)g[v].size(); i++) {
            auto &e = g[v][i];

            if (level[e.to] != level[v] + 1 || e.cap == 0)
                continue;

            ll pushed = dfs(e.to, t, min(f, e.cap));

            if (pushed) {
                e.cap -= pushed;
                g[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        ll flow = 0;

        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(s, t, LLONG_MAX))
                flow += pushed;
        }

        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    Dinic mf(n);

    vector<pair<int,int>> edges;

    while (m--) {
        int u, v;
        cin >> u >> v;

        edges.push_back({u, v});

        // undirected edge of capacity 1
        mf.addEdge(u, v, 1);
        mf.addEdge(v, u, 1);
    }

    cout << mf.maxflow(1, n) << '\n';

    vector<int> vis(n + 1);

    function<void(int)> dfs = [&](int v) {
        vis[v] = 1;
        for (auto &e : mf.g[v])
            if (e.cap && !vis[e.to])
                dfs(e.to);
    };

    dfs(1);

    for (auto [u, v] : edges) {
        if (vis[u] != vis[v])
            cout << u << ' ' << v << '\n';
    }
}