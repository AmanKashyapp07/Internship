// Link: https://cses.fi/problemset/task/1135
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;
vector<vector<int>> g, up;
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

    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    dfs(1, 1, 0);

    while (q--) {
        int a, b; cin >> a >> b;
        int L = getLCA(a, b);
        cout << (depth[a] + depth[b] - 2 * depth[L]) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find shortest path distance between nodes a and b in an unweighted tree for q queries (CSES 1135).
// - Approach: Binary Lifting LCA Distance Formula `dist(a, b) = depth[a] + depth[b] - 2 * depth[LCA(a, b)]`.
// - Intuition: Path distance equals total depth from root minus twice the depth of their lowest common ancestor.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N \log N).