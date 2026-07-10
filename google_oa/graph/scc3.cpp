/**
 * CSES 1686 - Coin Collector
 *
 * Problem Statement:
 * You are given a directed graph where each vertex contains some coins.
 * You may start and end at any vertex while following directed edges.
 * Find the maximum number of coins that can be collected.
 *
 * Approach:
 * - Use Kosaraju's algorithm to find all Strongly Connected Components (SCCs).
 * - Every SCC becomes a single node whose weight is the sum of coins in that SCC.
 * - Build the condensed graph, which is a Directed Acyclic Graph (DAG).
 * - Run DP on the DAG to find the maximum-weight path.
 *
 * Time Complexity: O(n + m)
 * Space Complexity: O(n + m)
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;
vector<int> comp;
stack<int>s;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u])
        if (!vis[v])
            dfs1(v);

    s.push(u);
}

void dfs2(int u, int id) {
    comp[u] = id;

    for (int v : rg[u]) if (!comp[v]) dfs2(v, id);
}

long long dfs(int u, vector<vector<int>> &dag, vector<long long> &weight, vector<long long> &dp) {
    if (dp[u] != -1) return dp[u];

    long long ans = weight[u];

    for (int v : dag[u])
        ans = max(ans, weight[u] + dfs(v, dag, weight, dp));

    return dp[u] = ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<long long> coins(n + 1);

    for (int i = 1; i <= n; i++)
        cin >> coins[i];

    g.resize(n + 1);
    rg.resize(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        rg[v].push_back(u);
    }

    vis.assign(n + 1, false);

    for (int i = 1; i <= n; i++)
        if (!vis[i])
            dfs1(i);

    
    comp.assign(n + 1, 0);
    int scc = 0;

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        if (comp[u]) continue;

        scc++;
        dfs2(u, scc);
    }

    vector<long long> weight(scc + 1); // weight[i] = sum of coins in component i

    for (int i = 1; i <= n; i++)  weight[comp[i]] += coins[i];

    vector<vector<int>> dag(scc + 1);

    for (int u = 1; u <= n; u++) for (int v : g[u]) if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);

    vector<long long> dp(scc + 1, -1);

    long long ans = 0;
    for (int i = 1; i <= scc; i++) ans = max(ans, dfs(i, dag, weight, dp));

    cout << ans << '\n';
}