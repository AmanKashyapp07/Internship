// Link: https://cses.fi/problemset/task/1137

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Fenwick tree implementation for 1-based indexing
struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, long long val) {
        for (; i <= n; i += i & -i) bit[i] += val;
    }

    long long query(int i) {
        long long s = 0;
        for (; i > 0; i -= i & -i) s += bit[i];
        return s;
    }

    long long query(int l, int r) {
        return query(r) - query(l - 1);
    }
};

// Euler Tour Technique (Variant A) to map subtree of u to contiguous range [tin[u], tout[u]]
struct EulerTour {
    int n;
    int timer;
    vector<int> tin, tout;

    EulerTour(int n) {
        this->n = n;
        timer = 0;
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
    }

    void dfs_subtree(int u, int p, const vector<vector<int>>& adj) {
        tin[u] = ++timer;
        for (int v : adj[u]) {
            if (v != p) {
                dfs_subtree(v, u, adj);
            }
        }
        tout[u] = timer;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> values(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> values[i];
    }

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Build subtree Euler Tour (Variant A)
    EulerTour et(n);
    et.dfs_subtree(1, 0, adj);

    // Initialize Fenwick tree (sizes correspond to flattened index)
    Fenwick ft(n);
    for (int i = 1; i <= n; i++) {
        ft.update(et.tin[i], values[i]);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) { // Update value of node s to x
            int s;
            long long x;
            cin >> s >> x;
            long long delta = x - values[s];
            ft.update(et.tin[s], delta);
            values[s] = x;
        } else { // Query subtree sum of node s
            int s;
            cin >> s;
            cout << ft.query(et.tin[s], et.tout[s]) << "\n";
        }
    }

    return 0;
}
