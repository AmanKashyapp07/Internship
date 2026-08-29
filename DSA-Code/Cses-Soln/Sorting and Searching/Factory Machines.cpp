// Link: https://cses.fi/problemset/task/1620
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    ll n, t; cin >> n >> t;
    vector<ll> a(n);
    for (ll &x : a) cin >> x;

    ll low = 0, high = 1e18, ans = -1;
    while (low <= high) {
        ll mid = low + (high - low) / 2;
        i128 total = 0;
        for (ll x : a) total += (i128)mid / x;

        if (total >= (i128)t) { ans = mid; high = mid - 1; }
        else low = mid + 1;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum time to produce t products using n machines with times $a_i$ (CSES 1620).
// - Approach: Binary Search on Answer predicate `totalProducts(time) >= t`.
// - Intuition: Total products produced in time $T$ equals $\sum \lfloor T / a_i \rfloor$, which is monotonically non-decreasing.
// - Complexity: Time: O(N \log(10^{18})), Space: O(N).