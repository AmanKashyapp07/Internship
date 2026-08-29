// Link: https://cses.fi/problemset/task/1636
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
    for (int c : coins) {
        for (int sum = c; sum <= x; sum++) {
            dp[sum] = (dp[sum] + dp[sum - c]) % MOD;
        }
    }
    cout << dp[x] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count distinct coin combinations summing to x where order does NOT matter (CSES 1636).
// - Approach: Unbounded Knapsack Combination DP (Outer loop: coins, Inner loop: sum).
// - Intuition: Processing `coin` in outer loop avoids counting duplicate coin order permutations ($1+2 \equiv 2+1$).
// - Complexity: Time: O(N \cdot X), Space: O(X).