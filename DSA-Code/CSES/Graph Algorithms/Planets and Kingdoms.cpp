// Link: https://cses.fi/problemset/task/1683
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;
vector<int> order, kingdom;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
    order.push_back(u);
}

void dfs2(int u, int k) {
    kingdom[u] = k;
    for (int v : rg[u]) if (!kingdom[v]) dfs2(v, k);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); rg.resize(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); rg[v].push_back(u);
    }

    vis.assign(n + 1, false);
    for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);

    kingdom.assign(n + 1, 0);
    reverse(order.begin(), order.end());
    int kCount = 0;
    for (int u : order) if (!kingdom[u]) dfs2(u, ++kCount);

    cout << kCount << '\n';
    for (int i = 1; i <= n; i++) cout << kingdom[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Group planets into kingdoms such that two planets are in the same kingdom iff they can reach each other (CSES 1683).
// - Approach: Kosaraju's Algorithm for Strongly Connected Components (SCCs).
// - Intuition: 2-Pass DFS (forward finishing order stack + reverse transpose DFS) partitions directed graph into component IDs.
// - Complexity: Time: O(V + E), Space: O(V + E).