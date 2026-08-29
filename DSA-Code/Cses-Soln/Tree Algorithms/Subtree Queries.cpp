// Link: https://cses.fi/problemset/task/1137
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, long long val) { for (i++; i <= n; i += i & -i) bit[i] += val; }
    long long query(int i) { long long s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }
    long long rangeQuery(int l, int r) { return query(r) - (l > 0 ? query(l - 1) : 0); }
};

int timer = 0;
vector<int> tin, tout;
void dfsSubtree(int u, int p, const vector<vector<int>>& g, vector<int>& flat, const vector<int>& val) {
    tin[u] = ++timer; flat[timer - 1] = val[u];
    for (int v : g[u]) if (v != p) dfsSubtree(v, u, g, flat, val);
    tout[u] = timer;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; if (!(cin >> n >> q)) return 0;
    vector<int> val(n + 1);
    for (int i = 1; i <= n; i++) cin >> val[i];
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    tin.assign(n + 1, 0); tout.assign(n + 1, 0);
    vector<int> flat(n);
    dfsSubtree(1, 0, g, flat, val);

    Fenwick ft(n);
    for (int i = 0; i < n; i++) ft.add(i, flat[i]);

    while (q--) {
        int type; cin >> type;
        if (type == 1) {
            int s; long long x; cin >> s >> x;
            long long delta = x - val[s];
            ft.add(tin[s] - 1, delta);
            val[s] = x;
        } else {
            int s; cin >> s;
            cout << ft.rangeQuery(tin[s] - 1, tout[s] - 1) << '\n';
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process point updates and subtree sum queries on a tree (CSES 1137).
// - Approach: Euler Tour Subtree Range Mapping (`tin[u]..tout[u]`) + Fenwick Tree range sum queries.
// - Intuition: Flattening subtrees into contiguous 1D array ranges allows $O(\log N)$ range queries and updates via Binary Indexed Tree.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N).
