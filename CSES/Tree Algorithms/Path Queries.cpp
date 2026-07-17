// Link: https://cses.fi/problemset/task/1138

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
};

// Euler Tour Technique (Variant B) to map path sums
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

    void dfs_path(int u, int p, const vector<vector<int>>& adj, vector<long long>& flat_val, const vector<int>& val) {
        tin[u] = ++timer;
        flat_val[timer] = val[u];
        for (int v : adj[u]) {
            if (v != p) {
                dfs_path(v, u, adj, flat_val, val);
            }
        }
        tout[u] = ++timer;
        flat_val[timer] = -val[u];
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

    // Build path Euler Tour (Variant B) of size 2N
    EulerTour et(n);
    vector<long long> flat_val(2 * n + 1, 0);
    et.dfs_path(1, 0, adj, flat_val, values);

    // Initialize Fenwick tree (sizes correspond to 2N flattened indices)
    Fenwick ft(2 * n);
    for (int i = 1; i <= 2 * n; i++) {
        ft.update(i, flat_val[i]);
    }

    // Keep track of current node values
    vector<long long> current_values(n + 1);
    for (int i = 1; i <= n; i++) {
        current_values[i] = values[i];
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) { // Update value of node s to x
            int s;
            long long x;
            cin >> s >> x;
            long long delta = x - current_values[s];
            
            // In Variant B, update +delta at entry and -delta at exit
            ft.update(et.tin[s], delta);
            ft.update(et.tout[s], -delta);
            current_values[s] = x;
        } else { // Query path sum from root to node s
            int s;
            cin >> s;
            cout << ft.query(et.tin[s]) << "\n";
        }
    }

    return 0;
}
