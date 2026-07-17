// Link: https://cses.fi/problemset/task/1633

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int countWays(int sum, vector<int>& dp) {
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

    for (int dice = 1; dice <= 6; dice++) {
        ways = (ways + countWays(sum - dice, dp)) % MOD;
    }

    return dp[sum] = ways;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> dp(n + 1, -1);

    cout << countWays(n, dp) << '\n';

    return 0;
}