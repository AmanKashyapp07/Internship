// Link: https://cses.fi/problemset/task/2177
#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> g;
vector<int> tin, low, visited;
vector<pair<int, int>> ans;
int timer = 0;
bool hasBridge = false;

void dfs(int u, int pEdge) {
    visited[u] = 1;
    tin[u] = low[u] = ++timer;
    for (auto [v, id] : g[u]) {
        if (id == pEdge) continue;
        if (!visited[v]) {
            ans[id] = {u, v};
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) hasBridge = true;
        } else {
            low[u] = min(low[u], tin[v]);
            if (tin[v] < tin[u]) ans[id] = {u, v};
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.assign(n + 1, {}); tin.assign(n + 1, 0); low.assign(n + 1, 0);
    visited.assign(n + 1, 0); ans.resize(m);

    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        g[u].push_back({v, i}); g[v].push_back({u, i});
    }

    dfs(1, -1);
    for (int i = 1; i <= n; i++) if (!visited[i]) { cout << "IMPOSSIBLE\n"; return 0; }
    if (hasBridge) { cout << "IMPOSSIBLE\n"; return 0; }

    for (auto [u, v] : ans) cout << u << ' ' << v << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Orient edges of an undirected graph to make it strongly connected (CSES 2177).
// - Approach: DFS Tree Orientation (Tree edges parent $\to$ child, Back edges descendant $\to$ ancestor) + Tarjan Bridge Check.
// - Intuition: If graph is connected and has no bridges (`low[v] <= tin[u]`), orienting tree edges forward and back-edges backward creates strong connectivity.
// - Complexity: Time: O(V + E), Space: O(V + E).