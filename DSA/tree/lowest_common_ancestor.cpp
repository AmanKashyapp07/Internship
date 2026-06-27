/**
 * Problem: Company Queries II (Lowest Common Ancestor)
 * Link: https://cses.fi/problemset/task/1688
 * Category: Tree
 * 
 * Description:
 * Find LCA using binary lifting.
 * 
 * Logic/Approach:
 * DP transitions tracking 2^k-th parents.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int LOG = 20;
vector<vector<int>> adj, up;
vector<int> depth;

void dfs(int u, int p) {
    up[u][0] = p;
    for (int j = 1; j < LOG; j++) {
        up[u][j] = (up[u][j - 1] != -1) ? up[up[u][j - 1]][j - 1] : -1;
    }
    for (int v : adj[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

int kthAncestor(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            u = up[u][j];
            if (u == -1) return -1;
        }
    }
    return u;
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    u = kthAncestor(u, depth[u] - depth[v]);
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    adj.resize(n + 1);
    depth.assign(n + 1, 0);
    up.assign(n + 1, vector<int>(LOG, -1));
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }
    dfs(1, -1);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << "\n";
    }
    return 0;
}
