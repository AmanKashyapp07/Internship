#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> coins(n);
    int totalSum = 0;

    for (int i = 0; i < n; i++) {
        cin >> coins[i];
        totalSum += coins[i];
    }

    vector<bool> dp(totalSum + 1, false);
    dp[0] = true;

    for (int coin : coins) {
        for (int sum = totalSum; sum >= coin; sum--) {
            dp[sum] = dp[sum] || dp[sum - coin];
        }
    }

    vector<int> ans;

    for (int sum = 1; sum <= totalSum; sum++) {
        if (dp[sum]) {
            ans.push_back(sum);
        }
    }

    cout << ans.size() << '\n';

    for (int x : ans) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}