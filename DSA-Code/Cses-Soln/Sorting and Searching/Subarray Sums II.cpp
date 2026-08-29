// Link: https://cses.fi/problemset/task/1661
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    map<long long, long long> mp;
    mp[0] = 1;

    long long ans = 0, pref = 0;
    for (int i = 0; i < n; i++) {
        long long v; cin >> v;
        pref += v;
        if (mp.count(pref - x)) ans += mp[pref - x];
        mp[pref]++;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count total subarrays with sum equal to x (allows negative elements) (CSES 1661).
// - Approach: Prefix Sum Hash Map Frequency Tracking.
// - Intuition: Target subarray sum $pref[r] - pref[l-1] = x \implies pref[l-1] = pref[r] - x$; map lookup counts matching previous prefix sums.
// - Complexity: Time: O(N \log N), Space: O(N).