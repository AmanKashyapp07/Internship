/**
 * Problem: Distance Queries
 * Link: https://cses.fi/problemset/task/1135
 * Category: Tree Algorithms
 * 
 * Description:
 * Find the number of edges between two tree nodes.
 * 
 * Logic/Approach:
 * LCA distance: depth[u] + depth[v] - 2 * depth[LCA(u, v)].
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int LOG = 20;

vector<vector<int>> adj;
vector<vector<int>> up;
vector<int> depth;
vector<long long> cnt, ans;

void dfs(int u, int p) {
    up[u][0] = p;

    for (int j = 1; j < LOG; j++) {
        if (up[u][j - 1] == -1)
            up[u][j] = -1;
        else
            up[u][j] = up[up[u][j - 1]][j - 1];
    }

    for (int v : adj[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        dfs(v, u);
    }
}

int lift(int x, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            x = up[x][j];
            if (x == -1) return -1;
        }
    }
    return x;
}

int lca(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);

    a = lift(a, depth[a] - depth[b]);

    if (a == b) return a;

    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }

    return up[a][0];
}

void dfs2(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs2(v, u);
        cnt[u] += cnt[v];
    }
    ans[u] = cnt[u];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    adj.resize(n + 1);
    up.assign(n + 1, vector<int>(LOG, -1));
    depth.assign(n + 1, 0);
    cnt.assign(n + 1, 0);
    ans.assign(n + 1, 0);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    dfs(1, -1);

    while (m--) {
        int a, b;
        cin >> a >> b;

        int L = lca(a, b);

        cnt[a]++;
        cnt[b]++;
        cnt[L]--;

        int p = up[L][0];
        if (p != -1) cnt[p]--; // if p is not -1, we need to decrement cnt[p] because L is the ancestor of a and b, so we need to remove the count from its parent as well, but if p is -1 it means L is the root and has no parent, so we don't need to do anything in that case
    }

    dfs2(1, -1);

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << ' ';
    }
    cout << '\n';

    return 0;
}