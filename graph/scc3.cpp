/**
 * Problem: Coin Collector (https://cses.fi/problemset/task/1686)
 * Condense directed graph into a DAG of SCC components, then find the maximum weight path.
 * It means we can use Kosaraju's algorithm to find the strongly connected components (SCCs) of the graph, then we can build a new graph where each SCC is a single node, and there is an edge from one SCC to another if there is an edge in the original graph from any node in the first SCC to any node in the second SCC. This new graph will be a Directed Acyclic Graph (DAG). We can then perform a topological sort on this DAG and use dynamic programming to find the maximum weight path.
 * Time: O(V + E) time, O(V + E) space.

 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;
vector<int> comp, order;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
    order.push_back(u);
}

void dfs2(int u, int id) {
    comp[u] = id;
    for (int v : rg[u]) if (!comp[v]) dfs2(v, id);
}

long long solveDAG(int u, const vector<long long>& weight, const vector<vector<int>>& dag, vector<long long>& dp) {
    if (dp[u] != -1) return dp[u];
    long long mx = 0;
    for (int v : dag[u]) mx = max(mx, solveDAG(v, weight, dag, dp));
    return dp[u] = weight[u] + mx; // DP state transition: dp[u] = weight[u] + max(dp[v])
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<long long> coins(n + 1);
    for (int i = 1; i <= n; i++) cin >> coins[i];
    g.resize(n + 1); rg.resize(n + 1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; g[u].push_back(v); rg[v].push_back(u); }
    vis.assign(n + 1, false);
    for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
    comp.assign(n + 1, 0); reverse(order.begin(), order.end());
    int scc = 0;
    for (int u : order) if (!comp[u]) dfs2(u, ++scc); // Condense graph into SCCs
    vector<long long> weight(scc + 1, 0);
    for (int i = 1; i <= n; i++) weight[comp[i]] += coins[i];
    vector<vector<int>> dag(scc + 1);
    for (int u = 1; u <= n; u++) {
        for (int v : g[u]) if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]); // Build DAG of components
    }
    vector<long long> dp(scc + 1, -1); long long ans = 0;
    for (int i = 1; i <= scc; i++) ans = max(ans, solveDAG(i, weight, dag, dp)); // Find max path in DAG
    cout << ans << '\n';
}
