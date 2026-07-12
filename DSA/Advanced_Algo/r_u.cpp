#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// 0-Indexed Fenwick Tree (BIT) configured for Difference Array / Range Updates
struct FenwickTree {
    int n;
    vector<long long> bit;

    FenwickTree(int n) : n(n), bit(n, 0) {}

    // Point Update: Add val to the difference array at 0-based index idx
    void update(int idx, long long val) {
        while (idx < n) {
            bit[idx] += val;
            idx = idx | (idx + 1);
        }
    }

    // Prefix Query: Returns the value at 0-based index idx (Sum of differences up to idx)
    long long query(int idx) {
        long long sum = 0;
        while (idx >= 0) {
            sum += bit[idx];
            idx = (idx & (idx + 1)) - 1;
        }
        return sum;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (cin >> n >> q) {
        vector<int> a(n);
        FenwickTree ft(n);

        // Build the Difference Array representation
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            if (i == 0) {
                ft.update(i, a[i]);
            } else {
                ft.update(i, a[i] - a[i - 1]);
            }
        } // difference array is arrays in which ft.bit[i] = a[i] - a[i-1] for i > 0 and ft.bit[0] = a[0]

        // Process Queries
        for (int i = 0; i < q; i++) {
            int type;
            cin >> type;
            if (type == 1) {
                int l, r, u;
                cin >> l >> r >> u;
                // Convert 1-based bounds to 0-based
                l--; r--;
                // Range update [l, r] with +u
                ft.update(l, u);
                ft.update(r + 1, -u);
            } else if (type == 2) {
                int k;
                cin >> k;
                // Convert 1-based index to 0-based point query
                cout << ft.query(k - 1) << "\n";
            }
        }
    }

    return 0;
}
