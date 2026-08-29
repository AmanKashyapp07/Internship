// Link: https://cses.fi/problemset/task/2138
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50005;
// Global to avoid stack overflow (~312 MB if local)
bitset<MAXN> reachable[MAXN];

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    vector<int> indeg(n, 0);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v; u--; v--;
        g[u].push_back(v); indeg[v]++;
    }

    // Kahn's BFS topological sort
    queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    vector<int> topo;
    while (!q.empty()) {
        int u = q.front(); q.pop(); topo.push_back(u);
        for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
    }

    // Propagate reachability in reverse topological order
    for (int i = n - 1; i >= 0; i--) {
        int u = topo[i]; reachable[u].set(u);
        for (int v : g[u]) reachable[u] |= reachable[v];
    }

    for (int i = 0; i < n; i++)
        cout << reachable[i].count() << " \n"[i == n - 1];
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each node in a DAG, count the number of nodes reachable from it (CSES 2138).
// - Approach: Topological Sort (Kahn's BFS) + Bitset Reachability DP propagated in reverse order.
// - Intuition: By processing in reverse topological order, a node's reachable set = self ∪ union of all children's bitsets; bitwise OR over bitsets is fast with hardware SIMD.
// - Complexity: Time: O((V + E) \cdot V / 64), Space: O(V^2 / 8).