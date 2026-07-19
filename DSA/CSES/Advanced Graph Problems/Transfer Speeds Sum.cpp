// Link: https://cses.fi/problemset/task/3111

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    ll unite(int u, int v) {
        u = find(u);
        v = find(v);

        if (u == v)
            return 0;

        ll pairs = 1LL * sz[u] * sz[v];

        if (sz[u] < sz[v])
            swap(u, v);

        parent[v] = u;
        sz[u] += sz[v];

        return pairs;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<ll, int, int>> edges;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    sort(edges.rbegin(), edges.rend());

    DSU dsu(n);

    ll ans = 0;

    for (auto &[w, u, v] : edges) {
        ll pairs = dsu.unite(u, v);
        ans += pairs * w;
    }

    cout << ans << '\n';

    return 0;
}