// Link: https://cses.fi/problemset/task/1688
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;
vector<vector<int>> g;
vector<vector<int>> up;
vector<int> depth;

void dfs(int u, int p, int d) {
    depth[u] = d; up[u][0] = p;
    for (int j = 1; j < LOG; j++) up[u][j] = up[up[u][j - 1]][j - 1];
    for (int v : g[u]) if (v != p) dfs(v, u, d + 1);
}

int getLCA(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int j = LOG - 1; j >= 0; j--) if ((diff >> j) & 1) u = up[u][j];
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
    }
    return up[u][0];
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    g.resize(n + 1); up.assign(n + 1, vector<int>(LOG, 1)); depth.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) { int boss; cin >> boss; g[boss].push_back(i); g[i].push_back(boss); }

    dfs(1, 1, 0);
    while (q--) {
        int a, b; cin >> a >> b;
        cout << getLCA(a, b) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the Lowest Common Ancestor (LCA) of nodes a and b in a tree for q queries (CSES 1688).
// - Approach: Binary Lifting LCA (level depths + jump highest powers of 2 without meeting).
// - Intuition: Lift deeper node to match depths, then simultaneously lift both nodes to right below their common ancestor.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N \log N).