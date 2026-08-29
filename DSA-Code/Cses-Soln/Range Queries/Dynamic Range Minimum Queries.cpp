// Link: https://cses.fi/problemset/task/1649
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n; vector<int> t;
    SegTree(int n, vector<int>& a) : n(n), t(4*n) { build(1, 0, n-1, a); }
    void build(int v, int l, int r, vector<int>& a) {
        if (l == r) { t[v] = a[l]; return; }
        int m = (l+r)/2;
        build(2*v, l, m, a); build(2*v+1, m+1, r, a);
        t[v] = min(t[2*v], t[2*v+1]);
    }
    void upd(int v, int l, int r, int i, int x) {
        if (l == r) { t[v] = x; return; }
        int m = (l+r)/2;
        if (i <= m) upd(2*v, l, m, i, x); else upd(2*v+1, m+1, r, i, x);
        t[v] = min(t[2*v], t[2*v+1]);
    }
    int qry(int v, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return INT_MAX;
        if (ql <= l && r <= qr) return t[v];
        int m = (l+r)/2;
        return min(qry(2*v, l, m, ql, qr), qry(2*v+1, m+1, r, ql, qr));
    }
    void upd(int i, int x) { upd(1, 0, n-1, i, x); }
    int qry(int l, int r) { return qry(1, 0, n-1, l, r); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SegTree st(n, a);
    while (q--) {
        int t; cin >> t;
        if (t == 1) { int k, u; cin >> k >> u; st.upd(k-1, u); }
        else        { int l, r; cin >> l >> r; cout << st.qry(l-1, r-1) << '\n'; }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Support point updates and range minimum queries on an array (CSES 1649).
// - Approach: Segment Tree with point update + range min query.
// - Intuition: Each node stores the minimum of its subtree; update touches O(log N) nodes bottom-up; query merges O(log N) nodes top-down.
// - Complexity: Time: O((N + Q) log N), Space: O(N).
