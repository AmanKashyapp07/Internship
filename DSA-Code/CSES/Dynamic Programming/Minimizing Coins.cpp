#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    vector<int> dp(x + 1, INF);
    dp[0] = 0;

    for (int sum = 1; sum <= x; sum++) {
        for (int coin : coins) {
            if (sum >= coin) {
                dp[sum] = min(dp[sum], 1 + dp[sum - coin]);
            }
        }
    }

    cout << (dp[x] == INF ? -1 : dp[x]) << '\n';

    return 0;
}