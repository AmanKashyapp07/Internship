// Link: https://cses.fi/problemset/task/1695
#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge { int to, rev; long long cap; };
    int n; vector<vector<Edge>> g; vector<int> level, ptr;
    Dinic(int n) : n(n), g(n + 1), level(n + 1), ptr(n + 1) {}

    void addEdge(int u, int v, long long cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q; q.push(s); level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                if (e.cap && level[e.to] == -1) { level[e.to] = level[u] + 1; q.push(e.to); }
            }
        }
        return level[t] != -1;
    }

    long long dfs(int u, int t, long long f) {
        if (u == t || f == 0) return f;
        for (int &i = ptr[u]; i < (int)g[u].size(); i++) {
            auto &e = g[u][i];
            if (level[e.to] != level[u] + 1 || e.cap == 0) continue;
            long long pushed = dfs(e.to, t, min(f, e.cap));
            if (pushed) { e.cap -= pushed; g[e.to][e.rev].cap += pushed; return pushed; }
        }
        return 0;
    }

    long long maxflow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, LLONG_MAX)) flow += pushed;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    Dinic mf(n);
    vector<pair<int, int>> edges;
    while (m--) {
        int u, v; cin >> u >> v; edges.push_back({u, v});
        mf.addEdge(u, v, 1); mf.addEdge(v, u, 1);
    }

    cout << mf.maxflow(1, n) << '\n';
    vector<int> vis(n + 1, 0);
    auto dfsReach = [&](auto &self, int u) -> void {
        vis[u] = 1;
        for (auto &e : mf.g[u]) if (e.cap && !vis[e.to]) self(self, e.to);
    };
    dfsReach(dfsReach, 1);

    for (auto [u, v] : edges) {
        if (vis[u] != vis[v]) cout << u << ' ' << v << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of streets to block so no path exists between city 1 and city n (CSES 1695).
// - Approach: Max-Flow Min-Cut Theorem (Dinic's Algorithm).
// - Intuition: Min-cut equals max-flow; reachable vertices in residual graph from source 1 determine cut edges where `vis[u] != vis[v]`.
// - Complexity: Time: O(V^2 E), Space: O(V + E).