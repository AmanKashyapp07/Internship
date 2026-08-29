// Link: https://cses.fi/problemset/task/1190
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
    void upd(int v, int l, int r, int i, int x) {
        if (l == r) { t[v] = leaf(x); return; }
        int m = (l+r)/2;
        if (i <= m) upd(2*v, l, m, i, x); else upd(2*v+1, m+1, r, i, x);
        t[v] = merge(t[2*v], t[2*v+1]);
    }
    void upd(int i, int x) { upd(1, 0, n-1, i, x); }
    long long ans() { return t[1].ans; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SegTree st(n, a);
    while (q--) {
        int i, x; cin >> i >> x;
        st.upd(i - 1, x);
        cout << st.ans() << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: After each point update, output the maximum subarray sum of the whole array (CSES 1190).
// - Approach: Segment Tree with augmented node (sum, pref, suff, ans) — Kadane's algorithm over segment tree.
// - Intuition: Each node stores best prefix/suffix/subarray sum; merge computes these in O(1); root's `ans` is the global max subarray sum after each update.
// - Complexity: Time: O((N + Q) log N), Space: O(N).