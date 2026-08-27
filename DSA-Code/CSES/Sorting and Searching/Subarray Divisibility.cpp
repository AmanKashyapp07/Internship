// Link: https://cses.fi/problemset/task/1662
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> freq(n, 0);
    freq[0] = 1;

    long long pref = 0, ans = 0;
    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        pref = ((pref + x) % n + n) % n;
        ans += freq[pref];
        freq[pref]++;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count total subarrays whose sum is divisible by n (CSES 1662).
// - Approach: Prefix Sum Remainder Modulo Map (`(pref % n + n) % n`).
// - Intuition: Subarray sum $pref[r] - pref[l-1]$ is divisible by n iff $pref[r] \equiv pref[l-1] \pmod n$.
// - Complexity: Time: O(N), Space: O(N).