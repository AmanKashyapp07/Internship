// Link: https://cses.fi/problemset/task/2078
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

long long power(long long a, long long b) {
    long long res = 1;
    for (; b; b >>= 1) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--) {
        int u, v; cin >> u >> v; u--; v--;
        g[u].push_back(v); g[v].push_back(u);
    }

    vector<bool> vis(n, false);
    int components = 0;
    auto dfs = [&](auto& self, int u) -> void {
        vis[u] = true;
        for (int v : g[u]) if (!vis[v]) self(self, v);
    };
    for (int i = 0; i < n; i++) {
        if (!vis[i]) { dfs(dfs, i); components++; }
    }

    // Answer = 2^(m - n + components): from linear algebra over GF(2)
    cout << power(2, m - n + components) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count Eulerian subgraphs (subsets of edges where every vertex has even degree) modulo 10^9+7 (CSES 2078).
// - Approach: Linear Algebra over GF(2) — answer is $2^{m - n + c}$ where $c$ = connected components.
// - Intuition: Eulerian subgraph solutions satisfy $Ax = 0 \pmod{2}$; free variables count equals $m - \text{rank}(A) = m - (n - c)$.
// - Complexity: Time: O(V + E), Space: O(V + E).