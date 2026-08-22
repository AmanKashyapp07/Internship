#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MOD = 1e9 + 7;

int n, m;
vector<int> a;
vector<vector<int>> dp;

int solve(int i, int prev)
{
    if (i == n) return 1;
    if (dp[i][prev + 1] != -1) return dp[i][prev + 1];

    ll ans = 0;

    if (a[i]){
        if (prev == -1 || abs(a[i] - prev) <= 1) ans = solve(i + 1, a[i]);
    }
    else{
        int l = (prev == -1 ? 1 : max(1, prev - 1));
        int r = (prev == -1 ? m : min(m, prev + 1));

        for (int cur = l; cur <= r; cur++) ans = (ans + solve(i + 1, cur)) % MOD;
    }

    return dp[i][prev + 1] = ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    a.resize(n);
    for (int &x : a)
        cin >> x;

    dp.assign(n, vector<int>(m + 2, -1));

    cout << solve(0, -1) << "\n";
}