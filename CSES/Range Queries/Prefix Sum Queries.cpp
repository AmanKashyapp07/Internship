// Link: https://cses.fi/problemset/task/2166

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    long long sum, pref;
};

Node merge(Node L, Node R) {
    return {L.sum + R.sum, max(L.pref, L.sum + R.pref)};
}

Node make_node(long long x) {
    return {x, max(0LL, x)};
}

struct SegTree {
    int n;
    vector<Node> tree;

    SegTree(vector<int>& a) : n(a.size()), tree(4 * a.size()) {
        build(1, 0, n - 1, a);
    }

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

    void update(int idx, int val) {
        update(1, 0, n - 1, idx, val);
    }

    void update(int node, int lo, int hi, int idx, int val) {
        if (lo == hi) {
            tree[node] = make_node(val);
            return;
        }
        int mid = (lo + hi) / 2;
        if (idx <= mid) update(2 * node, lo, mid, idx, val);
        else update(2 * node + 1, mid + 1, hi, idx, val);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    Node query(int node, int lo, int hi, int l, int r) {
        if (hi < l || r < lo) return {0, 0};
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return merge(query(2 * node, lo, mid, l, r), query(2 * node + 1, mid + 1, hi, l, r));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        SegTree st(a);

        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                int idx, val;
                cin >> idx >> val;
                st.update(idx - 1, val);
            } else {
                int l, r;
                cin >> l >> r;
                cout << st.query(l - 1, r - 1).pref << '\n';
            }
        }
    }
    return 0;
}