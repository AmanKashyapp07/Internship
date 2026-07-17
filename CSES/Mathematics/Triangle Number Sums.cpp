#include <bits/stdc++.h>
#define int long long
using namespace std;

signed main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. Precompute ALL triangular numbers up to 10^12 ONCE
    vector<long long> tri;
    for (long long k = 1;; k++) {
        long long x = k * (k + 1) / 2;
        tri.push_back(x);
        if (x > 1e12) break; 
    }

    int t;
    cin >> t;

    while (t--) {
        long long n;
        cin >> n;

        // Case 1: Can it be represented by 1 triangular number?
        if (binary_search(tri.begin(), tri.end(), n)) {
            cout << 1 << '\n';
            continue;
        }

        // Case 2: Can it be represented by 2 triangular numbers?
        // FIX: Use Two Pointers instead of a loop + binary search
        bool two = false;
        int left = 0;
        
        // Start the right pointer at the largest triangular number <= n
        int right = upper_bound(tri.begin(), tri.end(), n) - tri.begin() - 1;

        while (left <= right) {
            long long sum = tri[left] + tri[right];
            
            if (sum == n) {
                two = true;
                break;
            } else if (sum < n) {
                left++;  // We need a bigger sum
            } else {
                right--; // We need a smaller sum
            }
        }

        // Case 3: Gauss's theorem
        if (two) {
            cout << 2 << '\n';
        } else {
            cout << 3 << '\n';
        }
    }

    return 0;
}