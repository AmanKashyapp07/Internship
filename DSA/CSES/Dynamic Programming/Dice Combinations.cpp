#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> dp(n + 1, 0);
    dp[0] = 1;

    for (int sum = 1; sum <= n; sum++) {
        for (int dice = 1; dice <= 6; dice++) {
            if (sum >= dice) {
                dp[sum] = (dp[sum] + dp[sum - dice]) % MOD;
            }
        }
    }

    cout << dp[n] << '\n';

    return 0;
}