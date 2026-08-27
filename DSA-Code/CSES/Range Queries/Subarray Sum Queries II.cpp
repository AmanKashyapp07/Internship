// Link: https://cses.fi/problemset/task/3226
#include <bits/stdc++.h>
using namespace std;

struct Node { long long sum, pref, suff, ans; };
Node ZERO = {0, 0, 0, 0};
Node leaf(long long x) { long long v = max(0LL, x); return {x, v, v, v}; }
Node merge(Node L, Node R) {
    return {L.sum + R.sum,
            max(L.pref, L.sum + R.pref),
            max(R.suff, R.sum + L.suff),
            max({L.ans, R.ans, L.suff + R.pref})};
}

struct SegTree {
    int n; vector<Node> t;
    SegTree(int n, vector<int>& a) : n(n), t(4*n) { build(1, 0, n-1, a); }
    void build(int v, int l, int r, vector<int>& a) {
        if (l == r) { t[v] = leaf(a[l]); return; }
        int m = (l+r)/2;
        build(2*v, l, m, a); build(2*v+1, m+1, r, a);
        t[v] = merge(t[2*v], t[2*v+1]);
    }
    Node qry(int v, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return ZERO;
        if (ql <= l && r <= qr) return t[v];
        int m = (l+r)/2;
        return merge(qry(2*v, l, m, ql, qr), qry(2*v+1, m+1, r, ql, qr));
    }
    long long qry(int l, int r) { return qry(1, 0, n-1, l, r).ans; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SegTree st(n, a);
    while (q--) {
        int l, r; cin >> l >> r;
        cout << st.qry(l-1, r-1) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each query [l, r], find the maximum subarray sum within that range (CSES 3226).
// - Approach: Segment Tree with augmented node (sum, pref, suff, ans) — range query version of Kadane's.
// - Intuition: Same merge as Subarray Sum Queries, but now we query arbitrary [l, r] ranges instead of always [0, n-1].
// - Complexity: Time: O((N + Q) log N), Space: O(N).