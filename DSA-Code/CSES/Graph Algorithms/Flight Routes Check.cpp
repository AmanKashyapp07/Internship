// Link: https://cses.fi/problemset/task/1682
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
}

void dfs2(int u) {
    vis[u] = true;
    for (int v : rg[u]) if (!vis[v]) dfs2(v);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); rg.resize(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); rg[v].push_back(u);
    }

    vis.assign(n + 1, false); dfs1(1);
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) { cout << "NO\n1 " << i << '\n'; return 0; }
    }

    vis.assign(n + 1, false); dfs2(1);
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) { cout << "NO\n" << i << " 1\n"; return 0; }
    }

    cout << "YES\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Check if it is possible to travel between any pair of cities (CSES 1682).
// - Approach: 2-DFS Connectivity Check (Forward & Transpose Graph from node 1).
// - Intuition: If node 1 can reach all nodes and all nodes can reach node 1, the graph is strongly connected.
// - Complexity: Time: O(V + E), Space: O(V + E).