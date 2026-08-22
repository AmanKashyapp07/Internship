// Link: https://cses.fi/problemset/task/3226

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Recursive Segment Tree for Range Maximum Subarray Sum Queries (CSES Subarray Sum Queries II)
struct SegTree {
    struct Node {
        long long sum, pref, suff, ans;
    };

    int n;
    vector<Node> tree;

    Node identity() {
        return {0LL, 0LL, 0LL, 0LL};
    }

    Node make_node(long long x) {
        long long v = max(0LL, x); // Subarray can be empty (sum = 0)
        return {x, v, v, v};
    }

    Node merge(Node L, Node R) {
        Node res;
        res.sum = L.sum + R.sum;
        res.pref = max(L.pref, L.sum + R.pref);
        res.suff = max(R.suff, R.sum + L.suff);
        res.ans = max({L.ans, R.ans, L.suff + R.pref});
        return res;
    }

    SegTree(vector<int>& a) : n(a.size()), tree(4 * a.size()) {
        build(1, 0, n - 1, a);
    }

    void build(int node, int lo, int hi, const vector<int>& a) {
        if (lo == hi) {
            tree[node] = make_node(a[lo]);
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid, a);
        build(2 * node + 1, mid + 1, hi, a);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    Node query(int node, int lo, int hi, int l, int r) {
        if (hi < l || r < lo) return identity();
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return merge(query(2 * node, lo, mid, l, r), query(2 * node + 1, mid + 1, hi, l, r));
    }
};

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        SegTree st(a);

        while (q--) {
            int l, r;
            cin >> l >> r;
            // O(log N) Range Query
            cout << st.query(l - 1, r - 1).ans << '\n';
        }
    }
    return 0;
}