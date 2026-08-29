// Link: https://cses.fi/problemset/task/2143
#include <bits/stdc++.h>
using namespace std;

const int MAXC = 50000;
bitset<MAXC> reach[MAXC];

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, q; cin >> n >> m >> q;

    // Condense into SCC DAG using Kosaraju's
    vector<vector<int>> g(n + 1), rg(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        g[u].push_back(v); rg[v].push_back(u);
    }

    vector<int> order;
    vector<bool> vis(n + 1, false);
    function<void(int)> dfs1 = [&](int u) {
        vis[u] = true;
        for (int v : g[u]) if (!vis[v]) dfs1(v);
        order.push_back(u);
    };
    for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);

    vector<int> comp(n + 1, -1);
    int C = 0;
    function<void(int, int)> dfs2 = [&](int u, int c) {
        comp[u] = c;
        for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c);
    };
    for (int i = n - 1; i >= 0; i--) {
        if (comp[order[i]] == -1) dfs2(order[i], C++);
    }

    // Build condensed DAG + topological sort
    vector<vector<int>> dag(C);
    vector<int> indeg(C, 0);
    for (int u = 1; u <= n; u++)
        for (int v : g[u])
            if (comp[u] != comp[v]) { dag[comp[u]].push_back(comp[v]); indeg[comp[v]]++; }
    for (auto &x : dag) { sort(x.begin(), x.end()); x.erase(unique(x.begin(), x.end()), x.end()); }

    queue<int> bfsq;
    for (int i = 0; i < C; i++) if (indeg[i] == 0) bfsq.push(i);
    vector<int> topo;
    while (!bfsq.empty()) {
        int u = bfsq.front(); bfsq.pop(); topo.push_back(u);
        for (int v : dag[u]) if (--indeg[v] == 0) bfsq.push(v);
    }

    // Reachability bitsets in reverse topological order
    for (int i = C - 1; i >= 0; i--) {
        int u = topo[i]; reach[u].set(u);
        for (int v : dag[u]) reach[u] |= reach[v];
    }

    while (q--) {
        int a, b; cin >> a >> b;
        cout << (reach[comp[a]].test(comp[b]) ? "YES" : "NO") << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Answer q queries: can node a reach node b in a directed graph? (CSES 2143)
// - Approach: SCC condensation (Kosaraju) + Bitset Reachability DP on condensed DAG.
// - Intuition: Condense SCCs to a DAG; propagate reachability bitsets in reverse topological order; each query is a single O(1) bitset lookup.
// - Complexity: Time: O((V + E) + C^2 / 64), Space: O(C^2 / 8).