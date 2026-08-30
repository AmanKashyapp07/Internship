// Link: https://cses.fi/problemset/task/2422
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    if (!(cin >> n)) return 0;

    ll k = (n * n + 1) / 2; // Target 1-based median index
    ll lo = 1, hi = n * n, ans = lo;

    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        ll count = 0;
        for (ll i = 1; i <= n; i++) {
            count += min(n, mid / i);
        }
        if (count >= k) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    cout << ans << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the median element in an N x N multiplication table where cell (i, j) = i * j (CSES 2422 / LC 668).
// - Approach: Binary Search on Value Range [1, N^2].
// - Intuition: In row i, the count of elements <= mid is min(N, floor(mid / i)). Summing across all N rows computes total numbers <= mid in O(N) to drive monotonic binary search.
// - Complexity: Time: O(N log(N^2)), Space: O(1).
