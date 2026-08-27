// Link: https://cses.fi/problemset/task/1638
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    vector<vector<int>> dp(n, vector<int>(n, 0));
    if (grid[0][0] == '.') dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '#') { dp[i][j] = 0; continue; }
            if (i > 0) dp[i][j] = (dp[i][j] + dp[i - 1][j]) % MOD;
            if (j > 0) dp[i][j] = (dp[i][j] + dp[i][j - 1]) % MOD;
        }
    }
    cout << dp[n - 1][n - 1] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count paths from top-left to bottom-right in an n x n grid with trap cells `#` (CSES 1638).
// - Approach: 2D Grid Path Dynamic Programming (`dp[i][j] = dp[i-1][j] + dp[i][j-1]`).
// - Intuition: Valid moves are only down and right; adding incoming top and left cell path counts modulo $10^9+7$ solves cell counts.
// - Complexity: Time: O(N^2), Space: O(N^2).