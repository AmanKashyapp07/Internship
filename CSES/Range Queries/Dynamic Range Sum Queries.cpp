#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Segment Tree template configured for Range Sum and Point Update
struct SegmentTree {
    int n;
    vector<long long> tree;

    SegmentTree(vector<int>& a) {
        n = a.size();
        tree.assign(4 * n, 0);
        build(1, 0, n - 1, a);
    }

    void build(int node, int l, int r, vector<int>& a) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long query(int node, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return 0;           // No overlap
        if (ql <= l && r <= qr) return tree[node]; // Complete overlap
        int mid = (l + r) / 2;                     // Partial overlap
        return query(2 * node, l, mid, ql, qr) +
               query(2 * node + 1, mid + 1, r, ql, qr);
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            update(2 * node, l, mid, idx, val);
        else
            update(2 * node + 1, mid + 1, r, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Public functions (0-based indexing)
    long long query(int l, int r) { return query(1, 0, n - 1, l, r); }
    void update(int idx, int val) { update(1, 0, n - 1, idx, val); }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        // Initialize Segment Tree
        SegmentTree st(a);

        // Process Queries
        for (int i = 0; i < q; i++) {
            int type;
            cin >> type;
            if (type == 1) {
                int k, u;
                cin >> k >> u;
                // Convert 1-based index to 0-based
                st.update(k - 1, u);
            } else if (type == 2) {
                int l, r;
                cin >> l >> r;
                // Convert 1-based index to 0-based
                cout << st.query(l - 1, r - 1) << "\n";
            }
        }
    }

    return 0;
}
