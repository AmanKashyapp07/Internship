// Link: https://cses.fi/problemset/task/1635
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x; cin >> n >> x;
    vector<int> coins(n);
    for (int &c : coins) cin >> c;

    vector<int> dp(x + 1, 0);
    dp[0] = 1;
    for (int sum = 1; sum <= x; sum++) {
        for (int c : coins) {
            if (sum >= c) dp[sum] = (dp[sum] + dp[sum - c]) % MOD;
        }
    }
    cout << dp[x] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count coin sequences summing to x where order of coins matters (CSES 1635).
// - Approach: Unbounded Knapsack Permutation DP (Outer loop: sum, Inner loop: coins).
// - Intuition: Processing `sum` in outer loop allows counting ordered coin arrangements ($1+2 \neq 2+1$).
// - Complexity: Time: O(N \cdot X), Space: O(X).