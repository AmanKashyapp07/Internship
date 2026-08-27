// Link: https://cses.fi/problemset/task/1675
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
        return true;
    }
};

struct Edge {
    int u, v; long long w;
    bool operator<(const Edge& o) const { return w < o.w; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Edge> edges(m);
    for (auto &[u, v, w] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end());

    DSU dsu(n);
    long long cost = 0; int used = 0;
    for (auto &[u, v, w] : edges) {
        if (dsu.unite(u, v)) { cost += w; used++; }
    }

    if (used != n - 1) cout << "IMPOSSIBLE\n";
    else cout << cost << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum total cost to repair roads so all cities are connected (CSES 1675).
// - Approach: Minimum Spanning Tree (Kruskal's Algorithm + DSU).
// - Intuition: Sorting edges by weight and greedily adding non-cycle edges until $N-1$ edges are used yields minimum total cost.
// - Complexity: Time: O(E \log E), Space: O(V + E).