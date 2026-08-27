// Link: https://cses.fi/problemset/task/1633
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> dp(n + 1, 0);
    dp[0] = 1;

    for (int sum = 1; sum <= n; sum++) {
        for (int d = 1; d <= 6; d++) {
            if (sum >= d) dp[sum] = (dp[sum] + dp[sum - d]) % MOD;
        }
    }
    cout << dp[n] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count total ways to construct sum n by rolling a 6-sided die (CSES 1633).
// - Approach: 1D Dynamic Programming (`dp[sum] = \sum_{d=1}^6 dp[sum - d]`).
// - Intuition: Transition sums options from the previous 6 possible die outcomes.
// - Complexity: Time: O(N), Space: O(N).