// Link: https://cses.fi/problemset/task/1093

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const long long INV2 = 500000004;

int main() {
    int n;
    cin >> n;

    long long total = 1LL * n * (n + 1) / 2;

    if (total & 1) {
        cout << 0 << '\n';
        return 0;
    }

    int target = total / 2;

    vector<long long> dp(target + 1, 0);
    dp[0] = 1;

    for (int x = 1; x <= n; x++) {
        for (int s = target; s >= x; s--) {
            dp[s] = (dp[s] + dp[s - x]) % MOD;
        }
    }

    cout << dp[target] * INV2 % MOD << '\n';
}