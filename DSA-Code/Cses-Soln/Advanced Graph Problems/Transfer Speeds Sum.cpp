// Link: https://cses.fi/problemset/task/3111
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, sz;
    DSU(int n) : parent(n + 1), sz(n + 1, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    long long unite(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return 0;
        long long pairs = 1LL * sz[u] * sz[v];
        if (sz[u] < sz[v]) swap(u, v);
        parent[v] = u; sz[u] += sz[v];
        return pairs;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<tuple<long long, int, int>> edges;
    for (int i = 0; i < n - 1; i++) {
        int u, v; long long w; cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    sort(edges.rbegin(), edges.rend());
    DSU dsu(n);
    long long ans = 0;
    for (auto &[w, u, v] : edges) {
        long long pairs = dsu.unite(u, v);
        ans += pairs * w;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find sum of bottleneck transfer speeds over all pairs of nodes in a tree (CSES 3111).
// - Approach: Reverse Kruskal DSU Component Product.
// - Intuition: Sorting tree edges in descending order of weight guarantees that joining components of size $S_u$ and $S_v$ via edge $w$ contributes $S_u \cdot S_v \cdot w$ to total sum.
// - Complexity: Time: O(N \log N), Space: O(N).