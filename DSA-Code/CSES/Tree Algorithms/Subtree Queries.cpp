// Link: https://cses.fi/problemset/task/1137

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Standard Fenwick Tree (BIT) for 0-indexed queries externally
struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, long long val) {
        i++; // Convert 0-indexed parameter to 1-indexed BIT internally
        while (i <= n) {
            bit[i] += val;
            i += i & -i; 
        }
    }

    long long query(int i) {
        long long s = 0;
        i++;
        while (i >= 1) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }

    long long query(int l, int r) {
        if (l > r) return 0;
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};

// Euler Tour Technique (Variant A) to map subtree of u to contiguous range [tin[u], tout[u]]
struct EulerTour {
    int n, timer;
    vector<int> tin, tout;

    EulerTour(int n) : n(n), timer(0), tin(n + 1, 0), tout(n + 1, 0) {}

    // Subtree flattening (Size N) -> Range [tin[u], tout[u]]
    // Maps node values directly to a 0-indexed array
    void dfs_subtree(int u, int p, const vector<vector<int>>& g, vector<int>& flat, const vector<int>& val) {
        tin[u] = ++timer; 
        flat[timer - 1] = val[u];
        
        for (int v : g[u]) {
            if (v != p) dfs_subtree(v, u, g, flat, val);
        }
        tout[u] = timer;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> values(n + 1);
        for (int i = 1; i <= n; i++) cin >> values[i];

        vector<vector<int>> graph(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        // Build subtree Euler Tour (populating the flat vector directly in DFS)
        EulerTour et(n);
        vector<int> flat(n);
        et.dfs_subtree(1, 0, graph, flat, values);

        // Populate BIT (directly from the flat vector)
        Fenwick ft(n);
        for (int i = 0; i < n; i++) {
            ft.update(i, flat[i]);
        }

        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                int s;
                long long x;
                cin >> s >> x;
                long long delta = x - values[s];
                ft.update(et.tin[s] - 1, delta);
                values[s] = x;
            } else {
                int s;
                cin >> s;
                // Contiguous range query over [tin[s] - 1, tout[s] - 1]
                cout << ft.query(et.tin[s] - 1, et.tout[s] - 1) << "\n";
            }
        }
    }
    return 0;
}
