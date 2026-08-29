// Link: https://cses.fi/problemset/task/2166
#include <bits/stdc++.h>
using namespace std;

// Segment tree node: sum of range + max prefix sum
struct Node { long long sum, pref; };
Node merge(Node L, Node R) { return {L.sum + R.sum, max(L.pref, L.sum + R.pref)}; }

struct SegTree {
    int n; vector<Node> t;
    SegTree(int n, vector<int>& a) : n(n), t(4*n) { build(1, 0, n-1, a); }
    void build(int v, int l, int r, vector<int>& a) {
        if (l == r) { t[v] = {a[l], max(0, a[l])}; return; }
        int m = (l+r)/2;
        build(2*v, l, m, a); build(2*v+1, m+1, r, a);
        t[v] = merge(t[2*v], t[2*v+1]);
    }
    void upd(int v, int l, int r, int i, int x) {
        if (l == r) { t[v] = {x, max(0, x)}; return; }
        int m = (l+r)/2;
        if (i <= m) upd(2*v, l, m, i, x); else upd(2*v+1, m+1, r, i, x);
        t[v] = merge(t[2*v], t[2*v+1]);
    }
    Node qry(int v, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return {0, 0};
        if (ql <= l && r <= qr) return t[v];
        int m = (l+r)/2;
        return merge(qry(2*v, l, m, ql, qr), qry(2*v+1, m+1, r, ql, qr));
    }
    void upd(int i, int x) { upd(1, 0, n-1, i, x); }
    long long qry(int l, int r) { return qry(1, 0, n-1, l, r).pref; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SegTree st(n, a);
    while (q--) {
        int t; cin >> t;
        if (t == 1) { int i, x; cin >> i >> x; st.upd(i-1, x); }
        else        { int l, r; cin >> l >> r; cout << st.qry(l-1, r-1) << '\n'; }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Support point updates; query max prefix sum of subarray [l, r] (CSES 2166).
// - Approach: Segment Tree with augmented node storing (sum, max_prefix_sum).
// - Intuition: `merge(L, R).pref = max(L.pref, L.sum + R.pref)`; enables range max-prefix queries in O(log N) by composing partial results.
// - Complexity: Time: O((N + Q) log N), Space: O(N).