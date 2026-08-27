// Link: https://cses.fi/problemset/task/1136
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;
vector<vector<int>> g, up;
vector<int> depth;
vector<long long> val;

void dfs(int u, int p, int d) {
    depth[u] = d; up[u][0] = p;
    for (int j = 1; j < LOG; j++) up[u][j] = (up[u][j - 1] == -1) ? -1 : up[up[u][j - 1]][j - 1];
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

void dfsAccumulate(int u, int p) {
    for (int v : g[u]) {
        if (v != p) {
            dfsAccumulate(v, u);
            val[u] += val[v];
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); up.assign(n + 1, vector<int>(LOG, -1)); depth.assign(n + 1, 0); val.assign(n + 1, 0);

    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    dfs(1, -1, 0);

    while (m--) {
        int a, b; cin >> a >> b;
        int L = getLCA(a, b);
        val[a]++; val[b]++; val[L]--;
        int p = up[L][0];
        if (p != -1) val[p]--;
    }

    dfsAccumulate(1, -1);
    for (int i = 1; i <= n; i++) cout << val[i] << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process m path queries (a, b) and count how many paths pass through each node (CSES 1136).
// - Approach: Tree Difference Array on Subtrees (`val[a]++, val[b]++, val[L]--, val[parent(L)]--`).
// - Intuition: Accumulating subtree sums bottom-up via post-order DFS computes total path counts across all nodes in linear time.
// - Complexity: Time: O(N \log N + M \log N), Space: O(N \log N).