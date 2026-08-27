// Link: https://cses.fi/problemset/task/1694
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
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1; q.push(e.to);
                }
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
            if (pushed > 0) { e.cap -= pushed; g[e.to][e.rev].cap += pushed; return pushed; }
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
    while (m--) { int u, v; long long c; cin >> u >> v >> c; mf.addEdge(u, v, c); }
    cout << mf.maxflow(1, n) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum network flow from source 1 to sink n (CSES 1694).
// - Approach: Dinic's Maximum Flow Algorithm ($O(V^2 E)$).
// - Intuition: Level graph BFS construct + DFS augmenting block flow using dynamic edge pointer `ptr` optimizes Ford-Fulkerson augmentation.
// - Complexity: Time: O(V^2 E), Space: O(V + E).