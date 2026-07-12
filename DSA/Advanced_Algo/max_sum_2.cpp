#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long sum, pref, suff, ans;
};

Node merge(Node L, Node R) {
    return {
        L.sum + R.sum,
        max(L.pref, L.sum + R.pref),
        max(R.suff, R.sum + L.suff),
        max({L.ans, R.ans, L.suff + R.pref})
    };
}

Node make_node(long long x) {
    return {x, max(0LL, x), max(0LL, x), max(0LL, x)};
}

struct SegTree {
    int n;
    vector<Node> tree;

    SegTree(vector<int>& a) {
        n = a.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, a);
    }

    // Build
    void build(int node, int lo, int hi, vector<int>& a) {
        if (lo == hi) {
            tree[node] = make_node(a[lo]);
            return;
        }

        int mid = (lo + hi) / 2;

        build(2 * node, lo, mid, a);
        build(2 * node + 1, mid + 1, hi, a);

        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    // Query
    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    Node query(int node, int lo, int hi, int l, int r) {
        // No overlap
        if (hi < l || r < lo)
            return {0, 0, 0, 0};

        // Complete overlap
        if (l <= lo && hi <= r)
            return tree[node];

        // Partial overlap
        int mid = (lo + hi) / 2;

        return merge(
            query(2 * node, lo, mid, l, r),
            query(2 * node + 1, mid + 1, hi, l, r)
        );
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (int &x : a)
        cin >> x;

    SegTree st(a);

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l - 1, r - 1).ans << '\n';
    }
}