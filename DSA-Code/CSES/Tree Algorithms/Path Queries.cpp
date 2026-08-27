// Link: https://cses.fi/problemset/task/1138
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, long long delta) { for (i++; i <= n; i += i & -i) bit[i] += delta; }
    long long query(int i) { long long sum = 0; for (i++; i > 0; i -= i & -i) sum += bit[i]; return sum; }
};

int timer = 0;
vector<int> tin, tout;
void dfsPath(int u, int p, const vector<vector<int>>& g, vector<long long>& flat, const vector<int>& val) {
    tin[u] = ++timer; flat[timer - 1] = val[u];
    for (int v : g[u]) if (v != p) dfsPath(v, u, g, flat, val);
    tout[u] = ++timer; flat[timer - 1] = -val[u];
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
    vector<long long> flat(2 * n, 0);
    dfsPath(1, 0, g, flat, val);

    Fenwick ft(2 * n);
    for (int i = 0; i < 2 * n; i++) ft.add(i, flat[i]);
    vector<long long> curVal = vector<long long>(val.begin(), val.end());

    while (q--) {
        int type; cin >> type;
        if (type == 1) {
            int s; long long x; cin >> s >> x;
            long long delta = x - curVal[s];
            ft.add(tin[s] - 1, delta);
            ft.add(tout[s] - 1, -delta);
            curVal[s] = x;
        } else {
            int s; cin >> s;
            cout << ft.query(tin[s] - 1) << '\n';
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process point updates and path sum queries from root to node s (CSES 1138).
// - Approach: Euler Tour Flattening of size 2N + Fenwick Tree prefix queries.
// - Intuition: Assigning +val[u] at entry time `tin[u]` and -val[u] at exit time `tout[u]` ensures ancestor values sum to root-to-node path sum.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N).
