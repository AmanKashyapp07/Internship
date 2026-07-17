#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree (Binary Indexed Tree) for point updates and dynamic range sums
struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n): n(n), bit(n + 1) {}
    void upd(int i, long long x){ for(; i <= n; i += i & -i) bit[i] += x; }
    long long qry(int i){ long long s = 0; for(; i; i -= i & -i) s += bit[i]; return s; }
    // Range sum query: yields the sum within the interval [l, r]
    long long qry(int l, int r){ return qry(r) - qry(l - 1); }
};

// Euler Tour technique variant to flatten subtrees into contiguous array segments
struct Euler {
    int t = 0; vector<int> in, out;
    Euler(int n): in(n + 1), out(n + 1) {}
    // Discovers tree layout; the entire subtree of u lives exactly inside [in[u], out[u]]
    void dfs(int u, int p, vector<vector<int>> &g){
        in[u] = ++t;
        for(int v : g[u]) if(v != p) dfs(v, u, g);
        out[u] = t; // The time tracking ends when all child subtrees are completely visited
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q; cin >> n >> q;
    vector<long long> val(n + 1);
    for(int i = 1; i <= n; i++) cin >> val[i];
    vector<vector<int>> g(n + 1);
    for(int i = 1, u, v; i < n; i++){
        cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    Euler et(n); et.dfs(1, 0, g);
    Fenwick ft(n);
    // Populate the Fenwick tree using each node's flattened 'in' time index
    for(int i = 1; i <= n; i++) ft.upd(et.in[i], val[i]);
    while(q--){
        int t, s; cin >> t >> s;
        if(t == 1){ // Type 1: Point update to change node s's value to x
            long long x; cin >> x;
            ft.upd(et.in[s], x - val[s]); // Apply the net difference at the node's position
            val[s] = x;
        } else { // Type 2: Query the sum of values within the entire subtree of s
            // Subtree query converts to a clean range sum query on the interval [in[s], out[s]]
            cout << ft.qry(et.in[s], et.out[s]) << '\n';
        }
    }
}