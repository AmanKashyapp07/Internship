// Link: https://cses.fi/problemset/task/2078

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MOD = 1e9 + 7;

/*
Proof:
Let xe = 1 if edge e is chosen, else 0.
Each vertex must have even degree, giving one linear equation over GF(2).
Hence Eulerian subgraphs are solutions of Ax = 0, where A is the incidence matrix.
#solutions = 2^(m - rank(A)), and rank(A) = n - c for an undirected graph.
Therefore, answer = 2^(m - n + c).
*/

ll power(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> vis(n, false);

    auto dfs = [&](auto&& self, int u) -> void {
        vis[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) self(self, v);
        }
    };

    int components = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(dfs, i);
            components++;
        }
    }

    cout << power(2, m - n + components) << '\n';
}