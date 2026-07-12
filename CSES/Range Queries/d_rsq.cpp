#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct SegTree {
    int n; vector<long long> t;
    SegTree(vector<int>& a) : n(a.size()), t(2 * n) {
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
    }
    void update(int p, int val) {
        for (t[p += n] = val; p > 1; p >>= 1) t[p >> 1] = t[p] + t[p ^ 1];
    }
    long long query(int l, int r) {
        long long s = 0;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) s += t[l++];
            if (r & 1) s += t[--r];
        }
        return s;
    }
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
        SegTree st(a);

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
