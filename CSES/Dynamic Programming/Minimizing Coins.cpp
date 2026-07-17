#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int minimumCoins(int sum, vector<int>& coins, vector<int>& dp) {
    if (sum == 0) {
        return 0;
    }

    if (sum < 0) {
        return INF;
    }

    if (dp[sum] != -1) {
        return dp[sum];
    }

    int answer = INF;

    for (int coin : coins) {
        answer = min(answer, 1 + minimumCoins(sum - coin, coins, dp));
    }

    return dp[sum] = answer;
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

    int answer = minimumCoins(x, coins, dp);

    cout << (answer == INF ? -1 : answer) << '\n';

    return 0;
}