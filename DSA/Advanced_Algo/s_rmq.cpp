#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// High-performance, O(1) query Sparse Table template
struct SparseTable {
    int n;
    int LOG;
    vector<vector<int>> st;
    vector<int> lg;

    SparseTable(const vector<int>& a) {
        n = a.size();
        LOG = __lg(n) + 1;

        st.assign(LOG, vector<int>(n));
        lg.assign(n + 1, 0);

        // Precompute log values
        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;

        // Base level
        for (int i = 0; i < n; i++)
            st[0][i] = a[i];

        // Build Sparse Table
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                int leftInterval = st[j - 1][i];
                int rightInterval = st[j - 1][i + (1 << (j - 1))];
                st[j][i] = min(leftInterval, rightInterval);
            }
        }
    }

    // Range Minimum Query [l, r] in O(1)
    int query(int l, int r) {
        int j = lg[r - l + 1];
        int leftInterval = st[j][l];
        int rightInterval = st[j][r - (1 << j) + 1];
        return min(leftInterval, rightInterval);
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

        // Initialize Sparse Table
        SparseTable st(a);

        // Process Queries
        for (int i = 0; i < q; i++) {
            int l, r;
            cin >> l >> r;
            // Convert 1-based indexing to 0-based
            cout << st.query(l - 1, r - 1) << "\n";
        }
    }

    return 0;
}
