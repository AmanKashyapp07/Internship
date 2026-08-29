// Link: https://cses.fi/problemset/task/1133
#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> g;
vector<long long> sz, ans;

void dfs1(int u, int p, int d) {
    ans[1] += d; sz[u] = 1;
    for (int v : g[u]) {
        if (v != p) {
            dfs1(v, u, d + 1);
            sz[u] += sz[v];
        }
    }
}

void dfs2(int u, int p) {
    for (int v : g[u]) {
        if (v != p) {
            ans[v] = ans[u] + n - 2 * sz[v];
            dfs2(v, u);
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    g.resize(n + 1); sz.resize(n + 1); ans.assign(n + 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    dfs1(1, 0, 0);
    dfs2(1, 0);
    for (int i = 1; i <= n; i++) cout << ans[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each node, find the sum of distances from that node to all other nodes in the tree (CSES 1133).
// - Approach: Tree Rerooting DP (2-Pass DFS).
// - Intuition: Rerooting from parent u to child v increases distance by +1 for $(N - \text{sz}[v])$ nodes and decreases distance by -1 for $\text{sz}[v]$ nodes: $\text{ans}[v] = \text{ans}[u] + N - 2 \cdot \text{sz}[v]$.
// - Complexity: Time: O(N), Space: O(N).