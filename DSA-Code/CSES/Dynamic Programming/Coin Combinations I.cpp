#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    vector<int> dp(x + 1, 0);
    dp[0] = 1;

    for (int sum = 1; sum <= x; sum++) {
        for (int coin : coins) {
            if (sum >= coin) {
                dp[sum] = (dp[sum] + dp[sum - coin]) % MOD;
            }
        }
    }

    cout << dp[x] << '\n';

    return 0;
}

// outer loop - sum from 1 to x
// inner loop - for each coin, check if it can contribute to the current sum
// this is for counting all possible combinations of coins that sum up to x, where the order of coins matters.