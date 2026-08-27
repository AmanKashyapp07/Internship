// Link: https://cses.fi/problemset/task/1669
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g;
vector<int> parent, vis, cycle;

bool dfs(int u, int p) {
    vis[u] = 1;
    for (int v : g[u]) {
        if (v == p) continue;
        if (!vis[v]) {
            parent[v] = u;
            if (dfs(v, u)) return true;
        } else {
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
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); parent.assign(n + 1, -1); vis.assign(n + 1, 0);
    while (m--) { int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u); }

    for (int i = 1; i <= n; i++) {
        if (!vis[i] && dfs(i, -1)) {
            cout << cycle.size() << '\n';
            for (int x : cycle) cout << x << ' ';
            cout << '\n';
            return 0;
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find any simple cycle of length $\ge 3$ in an undirected graph (CSES 1669).
// - Approach: Undirected DFS Cycle Detection via Parent Pointers.
// - Intuition: Visiting an already visited neighbor $v \neq p$ identifies a simple cycle; back-tracking parent pointers reconstructs the cycle path.
// - Complexity: Time: O(V + E), Space: O(V + E).