/**
 * Problem: Round Trip (https://cses.fi/problemset/task/1669)
 * Find a cycle of length at least 3 (4 vertices in path) in an undirected graph.
 * Time: O(V + E) time, O(V + E) space.
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m;
vector<vector<int>> adj;
vector<int> parent, vis, cycle;

bool dfs(int u, int par) {
    vis[u] = 1;
    for (int v : adj[u]) {
        if (v == par) continue;
        if (!vis[v]) {
            parent[v] = u;
            if (dfs(v, u)) return true;
        } else {
            // Cycle found: backtracking using parent pointers
            cycle.push_back(v);
            for (int cur = u; cur != v; cur = parent[cur]) cycle.push_back(cur);
            cycle.push_back(v);
            reverse(cycle.begin(), cycle.end());
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    cin >> n >> m;
    adj.resize(n + 1); parent.assign(n + 1, -1); vis.assign(n + 1, 0);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    for (int i = 1; i <= n; i++) {
        if (!vis[i] && dfs(i, -1)) {
            cout << cycle.size() << '\n';
            for (int x : cycle) cout << x << ' ';
            cout << '\n'; return 0;
        }
    }
    cout << "IMPOSSIBLE\n";
}
