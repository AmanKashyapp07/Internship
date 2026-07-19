// Link: https://cses.fi/problemset/task/1637

#include <bits/stdc++.h>
using namespace std;

vector<int> dp;

int solve(int n) {
    if (n == 0) return 0;

    if (dp[n] != -1) return dp[n];

    int ans = INT_MAX;
    int x = n;

    while (x > 0) {
        int digit = x % 10;
        x /= 10;

        if (digit != 0) {
            ans = min(ans, 1 + solve(n - digit));
        }
    }

    return dp[n] = ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    dp.assign(n + 1, -1);

    cout << solve(n) << '\n';

    return 0;
}