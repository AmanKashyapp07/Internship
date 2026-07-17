#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int n;
vector<string> grid;
vector<vector<int>> dp;

int solve(int i, int j) {
    if (i >= n || j >= n) return 0;
    if (grid[i][j] == '*') return 0;

    if (i == n - 1 && j == n - 1) return 1;

    if (dp[i][j] != -1) return dp[i][j];

    long long down = solve(i + 1, j);
    long long right = solve(i, j + 1);

    return dp[i][j] = (down + right) % MOD;
}

int main() {
    cin >> n;

    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    dp.assign(n, vector<int>(n, -1));

    cout << solve(0, 0) << '\n';
}