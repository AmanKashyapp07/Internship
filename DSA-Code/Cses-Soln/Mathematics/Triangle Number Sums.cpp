// Link: https://cses.fi/problemset/task/3406
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    vector<long long> tri;
    for (long long k = 1;; k++) {
        long long x = k * (k + 1) / 2;
        tri.push_back(x);
        if (x > 1e12) break;
    }

    int t; cin >> t;
    while (t--) {
        long long n; cin >> n;
        if (binary_search(tri.begin(), tri.end(), n)) { cout << 1 << '\n'; continue; }

        bool two = false;
        int left = 0, right = upper_bound(tri.begin(), tri.end(), n) - tri.begin() - 1;
        while (left <= right) {
            long long s = tri[left] + tri[right];
            if (s == n) { two = true; break; }
            else if (s < n) left++;
            else right--;
        }

        cout << (two ? 2 : 3) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of triangular numbers needed to sum to n (CSES 3406).
// - Approach: Precomputed Triangular Numbers + Two Pointers + Gauss's Triangular Number Theorem.
// - Intuition: Gauss proved every integer is sum of at most 3 triangular numbers; test 1 (binary search), test 2 (two pointers), default to 3.
// - Complexity: Time: O(√N precompute + T · √N), Space: O(√N).