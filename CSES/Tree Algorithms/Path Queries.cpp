#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Fenwick Tree (Binary Indexed Tree) for dynamic range/point queries
struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n): n(n), bit(n + 1, 0) {}
    void upd(int i, long long x){ for(; i <= n; i += i & -i) bit[i] += x; }
    long long qry(int i){ long long s = 0; for(; i; i -= i & -i) s += bit[i]; return s; }
    long long qry(int l, int r){ return qry(r) - qry(l - 1); }
};

// Modulated Euler Tour template as requested
struct EulerTour {
    int n, timer;
    vector<int> tin, tout, depth, first_occ, euler_lca;

    EulerTour(int n) {
        this->n = n; timer = 0;
        tin.assign(n + 1, 0); tout.assign(n + 1, 0);
        depth.assign(n + 1, 0); first_occ.assign(n + 1, 0);
    }

    // 1. Subtree flattening (Size N)
    void dfs_subtree(int u, int p, const vector<vector<int>>& adj) {
        tin[u] = ++timer;
        for (int v : adj[u]) if (v != p) dfs_subtree(v, u, adj);
        tout[u] = timer;
    }

    // 2. Path flattening (Size 2N)
    void dfs_path(int u, int p, const vector<vector<int>>& adj, vector<long long>& flat_val, const vector<long long>& val) {
        tin[u] = ++timer;
        flat_val[timer] = val[u];
        for (int v : adj[u]) if (v != p) dfs_path(v, u, adj, flat_val, val);
        tout[u] = ++timer;
        flat_val[timer] = -val[u];
    }

    // 3. LCA flattening (Size 2N-1)
    void dfs_lca(int u, int p, int d, const vector<vector<int>>& adj) {
        first_occ[u] = euler_lca.size(); euler_lca.push_back(u); depth[u] = d;
        for (int v : adj[u]) {
            if (v != p) { dfs_lca(v, u, d + 1, adj); euler_lca.push_back(u); }
        }
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q; cin >> n >> q;
    vector<long long> val(n + 1);
    for(int i = 1; i <= n; i++) cin >> val[i];
    vector<vector<int>> g(n + 1);
    for(int i = 1, u, v; i < n; i++){ cin >> u >> v; g[u].push_back(v); g[v].push_back(u); }

    // --- MODE 1: UNCOMMENT THIS BLOCK FOR SUBTREE QUERIES ---
    /*
    EulerTour et(n); et.dfs_subtree(1, 0, g);
    Fenwick ft(n);
    for(int i = 1; i <= n; i++) ft.upd(et.tin[i], val[i]);

    while(q--){
        int t, s; cin >> t >> s;
        if(t == 1){
            long long x; cin >> x;
            ft.upd(et.tin[s], x - val[s]); val[s] = x;
        } else cout << ft.qry(et.tin[s], et.tout[s]) << '\n';
    }
    */

    // --- MODE 2: PATH QUERIES FROM ROOT (DEFAULT) ---
    EulerTour et(n);
    vector<long long> flat_val(2 * n + 1, 0);
    et.dfs_path(1, 0, g, flat_val, val);

    Fenwick ft(2 * n);
    for(int i = 1; i <= 2 * n; i++) ft.upd(i, flat_val[i]);

    while(q--){
        int t, s; cin >> t >> s;
        if(t == 1){
            long long x; cin >> x;
            ft.upd(et.tin[s], x - val[s]);
            ft.upd(et.tout[s], val[s] - x);
            val[s] = x;
        } else cout << ft.qry(et.tin[s]) << '\n';
    }
}

// For path queries 2 -> for finding max value on path from root to node u, we will use a segment tree instead of a Fenwick tree, as max queries are non-commutative and cannot be handled by a Fenwick tree. The segment tree will allow us to efficiently update node values and query the maximum value along the path from the root to any given node.

