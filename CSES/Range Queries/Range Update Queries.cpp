// Link: https://cses.fi/problemset/task/1651

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Fenwick Tree (BIT) configured for Difference Array / Range Updates
struct FenwickTree {
    int n;
    vector<long long> bit;

    FenwickTree(int n) {
        this->n = n;
        // Size n + 2 to safely handle (r + 1) updates on the boundary
        bit.assign(n + 2, 0); 
    }

    // Point Update: Add val to the difference array at 0-based index idx
    void update(int idx, long long val) {
        idx++; // Convert to 1-based index
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += val;
        }
    }

    // Prefix Query: Returns the value at 0-based index idx (Sum of differences up to idx)
    long long query(int idx) {
        idx++; // Convert to 1-based index
        long long sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += bit[idx];
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
        }

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
