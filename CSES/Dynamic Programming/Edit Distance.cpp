// Link: https://cses.fi/problemset/task/1639

#include <bits/stdc++.h>
using namespace std;

string a, b;
vector<vector<int>> dp;

int solve(int i, int j) {
    if (i == (int)a.size()) return b.size() - j;
    if (j == (int)b.size()) return a.size() - i;

    int &ans = dp[i][j];
    if (ans != -1) return ans;

    if (a[i] == b[j]) {
        return ans = solve(i + 1, j + 1);
    }

    return ans = 1 + min({
        solve(i + 1, j),     // remove
        solve(i, j + 1),     // add
        solve(i + 1, j + 1)  // replace
    });
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> a >> b;

    dp.assign(a.size(), vector<int>(b.size(), -1));

    cout << solve(0, 0) << '\n';
    return 0;
}