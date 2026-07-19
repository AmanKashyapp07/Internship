// Link: https://cses.fi/problemset/task/1143

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Segment Tree for Hotel Queries (Range Max with binary search query_first)
struct SegTree {
    int n; 
    vector<int> tree;

    SegTree(int n) : n(n), tree(4 * n, 0) {}

    void build(int node, int lo, int hi, const vector<int>& a) {
        if (lo == hi) { 
            tree[node] = a[lo]; 
            return; 
        }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid, a); 
        build(2 * node + 1, mid + 1, hi, a);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int idx, int val) { 
        if (n > 0) update(1, 0, n - 1, idx, val); 
    }

    void update(int node, int lo, int hi, int idx, int val) {
        if (lo == hi) { 
            tree[node] = val; 
            return; 
        }
        int mid = (lo + hi) / 2;
        if (idx <= mid) update(2 * node, lo, mid, idx, val);
        else update(2 * node + 1, mid + 1, hi, idx, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    // Binary Search on Segment Tree: Find first index in [0, n-1] with value >= val
    int query_first(int val) { 
        return n <= 0 ? -1 : query_first(1, 0, n - 1, val); 
    }

    int query_first(int node, int lo, int hi, int val) {
        if (tree[node] < val) return -1;
        if (lo == hi) return lo;
        int mid = (lo + hi) / 2;
        if (tree[2 * node] >= val) return query_first(2 * node, lo, mid, val);
        return query_first(2 * node + 1, mid + 1, hi, val);
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        SegTree st(n);
        st.build(1, 0, n - 1, a);

        for (int i = 0; i < q; i++) {
            int x;
            cin >> x;
            int idx = st.query_first(x);
            if (idx == -1) {
                cout << 0 << (i == q - 1 ? "" : " ");
            } else {
                cout << idx + 1 << (i == q - 1 ? "" : " ");
                a[idx] -= x;
                st.update(idx, a[idx]);
            }
        }
        cout << "\n";
    }
    return 0;
}
