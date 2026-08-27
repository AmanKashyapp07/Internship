// Link: https://cses.fi/problemset/task/1666
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g;
vector<bool> vis;

void dfs(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs(v);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); vis.assign(n + 1, false);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    vector<int> reps;
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) { reps.push_back(i); dfs(i); }
    }

    cout << reps.size() - 1 << '\n';
    for (size_t i = 1; i < reps.size(); i++) {
        cout << reps[i - 1] << ' ' << reps[i] << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of new roads needed to connect all cities into one component (CSES 1666).
// - Approach: Connected Components DFS / Disjoint Set Union (DSU).
// - Intuition: Finding representative nodes for each connected component allows connecting component $i-1$ to component $i$ with $C-1$ edges.
// - Complexity: Time: O(V + E), Space: O(V + E).