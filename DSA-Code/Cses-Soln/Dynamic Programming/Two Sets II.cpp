// Link: https://cses.fi/problemset/task/1093
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const long long INV2 = 500000004; // Modular inverse of 2

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    long long total = 1LL * n * (n + 1) / 2;

    if (total & 1) { cout << 0 << '\n'; return 0; }

    int target = total / 2;
    vector<long long> dp(target + 1, 0);
    dp[0] = 1;

    for (int x = 1; x <= n; x++) {
        for (int s = target; s >= x; s--) {
            dp[s] = (dp[s] + dp[s - x]) % MOD;
        }
    }
    cout << dp[target] * INV2 % MOD << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count ways to split $1 \dots n$ into two equal-sum sets modulo $10^9+7$ (CSES 1093).
// - Approach: 0/1 Subset Sum DP to count ways to reach `totalSum / 2` + divide by 2 (INV2).
// - Intuition: `dp[s]` counts subsets summing to $s$; dividing by 2 removes double-counting since each partition pair is counted twice.
// - Complexity: Time: O(N \cdot \text{totalSum}/2), Space: O(\text{totalSum}/2).