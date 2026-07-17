#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int countWays(int sum, vector<int>& coins, vector<int>& dp) {
    if (sum == 0) {
        return 1;
    }

    if (sum < 0) {
        return 0;
    }

    if (dp[sum] != -1) {
        return dp[sum];
    }

    long long ways = 0;

    for (int coin : coins) {
        ways = (ways + countWays(sum - coin, coins, dp)) % MOD;
    }

    return dp[sum] = ways;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    vector<int> dp(x + 1, -1);

    cout << countWays(x, coins, dp) << '\n';

    return 0;
}