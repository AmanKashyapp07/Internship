// Link: https://cses.fi/problemset/task/1686
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g, rg, dag;
vector<bool> vis;
vector<int> comp, order;
vector<long long> coins, sccWeight, dp;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
    order.push_back(u);
}

void dfs2(int u, int id) {
    comp[u] = id;
    for (int v : rg[u]) if (!comp[v]) dfs2(v, id);
}

long long getDP(int u) {
    if (dp[u] != -1) return dp[u];
    dp[u] = sccWeight[u];
    for (int v : dag[u]) dp[u] = max(dp[u], sccWeight[u] + getDP(v));
    return dp[u];
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    coins.resize(n + 1);
    for (int i = 1; i <= n; i++) cin >> coins[i];

    g.resize(n + 1); rg.resize(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); rg[v].push_back(u);
    }

    vis.assign(n + 1, false);
    for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);

    comp.assign(n + 1, 0);
    reverse(order.begin(), order.end());
    int scc = 0;
    for (int u : order) if (!comp[u]) dfs2(u, ++scc);

    sccWeight.assign(scc + 1, 0);
    for (int i = 1; i <= n; i++) sccWeight[comp[i]] += coins[i];

    dag.resize(scc + 1);
    for (int u = 1; u <= n; u++) {
        for (int v : g[u]) {
            if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        }
    }

    dp.assign(scc + 1, -1);
    long long ans = 0;
    for (int i = 1; i <= scc; i++) ans = max(ans, getDP(i));
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum coins collected by traversing a directed graph (CSES 1686).
// - Approach: Kosaraju's SCC + Condensation DAG + Dynamic Programming.
// - Intuition: Contract strongly connected components into a single DAG node containing total component coins; run DP for longest path.
// - Complexity: Time: O(V + E), Space: O(V + E).