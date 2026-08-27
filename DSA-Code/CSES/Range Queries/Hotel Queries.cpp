// Link: https://cses.fi/problemset/task/1143
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n; vector<int> t;
    SegTree(int n, vector<int>& a) : n(n), t(4*n) { build(1, 0, n-1, a); }
    void build(int v, int l, int r, vector<int>& a) {
        if (l == r) { t[v] = a[l]; return; }
        int m = (l+r)/2;
        build(2*v, l, m, a); build(2*v+1, m+1, r, a);
        t[v] = max(t[2*v], t[2*v+1]);
    }
    void upd(int v, int l, int r, int i, int x) {
        if (l == r) { t[v] = x; return; }
        int m = (l+r)/2;
        if (i <= m) upd(2*v, l, m, i, x); else upd(2*v+1, m+1, r, i, x);
        t[v] = max(t[2*v], t[2*v+1]);
    }
    // Find first index in [0, n-1] with value >= val (walk segtree)
    int first_ge(int v, int l, int r, int val) {
        if (t[v] < val) return -1;
        if (l == r) return l;
        int m = (l+r)/2;
        int res = first_ge(2*v, l, m, val);
        return res != -1 ? res : first_ge(2*v+1, m+1, r, val);
    }
    void upd(int i, int x) { upd(1, 0, n-1, i, x); }
    int first_ge(int val) { return first_ge(1, 0, n-1, val); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    SegTree st(n, a);
    for (int i = 0; i < q; i++) {
        int x; cin >> x;
        int idx = st.first_ge(x);
        if (idx == -1) { cout << 0; }
        else { cout << idx + 1; a[idx] -= x; st.upd(idx, a[idx]); }
        cout << " \n"[i == q - 1];
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Assign each group to the first hotel with enough free rooms; update room count (CSES 1143).
// - Approach: Segment Tree Range Max + Binary Walk to find first index with value >= x.
// - Intuition: Walk the segtree from the root: go left if left subtree max >= x, else go right; O(log N) per query.
// - Complexity: Time: O((N + Q) log N), Space: O(N).
