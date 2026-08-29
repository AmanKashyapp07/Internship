// Link: https://cses.fi/problemset/task/3302
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int OFFSET = 250000;
const int MAX_SUM = 500000;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, a;
    if (!(cin >> n >> a)) return 0;
    vector<int> x(n);
    for (int i = 0; i < n; i++) { int val; cin >> val; x[i] = val - a; }

    vector<int> dp(MAX_SUM + 1, 0), next_dp(MAX_SUM + 1, 0);
    dp[OFFSET] = 1;

    for (int i = 0; i < n; i++) {
        int y = x[i];
        next_dp = dp;
        for (int sum = 0; sum <= MAX_SUM; sum++) {
            int prev_sum = sum - y;
            if (prev_sum >= 0 && prev_sum <= MAX_SUM) {
                next_dp[sum] = (next_dp[sum] + dp[prev_sum]) % MOD;
            }
        }
        dp = next_dp;
    }
    long long ans = (dp[OFFSET] - 1 + MOD) % MOD;
    cout << ans << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count non-empty subsets of an array whose arithmetic mean equals a (CSES 3302).
// - Approach: Zero-Sum Balancing Trick (`x[i] = val - a`) + Subset Sum DP with offset.
// - Intuition: Average of subset is `a` iff $\sum (x_i - a) = 0$; shift DP indices by OFFSET = 250,000 to support negative intermediate sums.
// - Complexity: Time: O(N \cdot \text{MAX\_SUM}), Space: O(\text{MAX\_SUM}).