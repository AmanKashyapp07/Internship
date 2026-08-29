// Link: https://cses.fi/problemset/task/1677
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    int comp;
    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--; return true;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<pair<int, int>> edges(m);
    set<pair<int, int>> removed;
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v; if (u > v) swap(u, v); edges[i] = {u, v};
    }

    vector<pair<int, int>> queries(k);
    for (int i = 0; i < k; i++) {
        int u, v; cin >> u >> v; if (u > v) swap(u, v);
        queries[i] = {u, v}; removed.insert({u, v});
    }

    DSU dsu(n);
    for (auto [u, v] : edges) if (!removed.count({u, v})) dsu.unite(u, v);

    vector<int> ans;
    for (int i = k - 1; i >= 0; i--) {
        ans.push_back(dsu.comp);
        dsu.unite(queries[i].first, queries[i].second);
    }
    reverse(ans.begin(), ans.end());

    for (int x : ans) cout << x << " ";
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Output component counts after each edge breakdown query (CSES 1677).
// - Approach: Offline Processing via Reverse DSU Edge Re-insertion.
// - Intuition: Reversing edge deletions transforms graph destruction into incremental DSU unions, tracking component count `comp`.
// - Complexity: Time: O((M + K) \cdot \alpha(N)), Space: O(N + M + K).