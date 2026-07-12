#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct SparseTable {
    int n, K;
    vector<vector<int>> st;
    SparseTable(vector<int> &a) {
        n = a.size(); K = __lg(n) + 1;
        st.assign(K, vector<int>(n));
        st[0] = a;
        for (int j = 1; j < K; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
    }
    int query(int l, int r) {
        int j = __lg(r - l + 1);
        return min(st[j][l], st[j][r - (1 << j) + 1]);
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
