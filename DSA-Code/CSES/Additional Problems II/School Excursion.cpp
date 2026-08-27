// Link: https://cses.fi/problemset/task/1706
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        if ((a = find(a)) != (b = find(b))) {
            if (sz[a] < sz[b]) swap(a, b);
            p[b] = a; sz[a] += sz[b];
        }
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    DSU dsu(n);
    while (m--) { int u, v; cin >> u >> v; dsu.unite(u, v); }

    bitset<100005> dp;
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        if (dsu.find(i) == i) dp |= (dp << dsu.sz[i]);
    }
    for (int i = 1; i <= n; i++) cout << dp[i];
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Output binary string indicating which total group sizes [1..N] can be formed by union of disjoint student components (CSES 1706).
// - Approach: DSU Component Aggregation + Bitset Subset Sum DP.
// - Intuition: Finding connected component sizes via DSU turns problem into Subset Sum DP; `bitset<100005>` bit-shifts process transitions in $O(N^2 / 64)$.
// - Complexity: Time: O(N^2 / 64), Space: O(N / 64).