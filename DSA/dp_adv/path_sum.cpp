/**
 * LeetCode 64 - Minimum Path Sum
 *
 * Description:
 * Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right,
 * which minimizes the sum of all numbers along its path (can only move down or right).
 *
 * Approach:
 * - Dynamic Programming (both Memoization and Tabulation styles).
 * - Let `dp[i][j]` be the minimum path sum to reach `(i, j)` from `(0, 0)`.
 * - Transition: `dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])`.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int INF = INT_MAX;

/* ---------------- Memoization ---------------- */

int solveMemo(int i, int j, vector<vector<int>> &grid, vector<vector<int>> &dp) {
    if (i == 0 && j == 0) return grid[0][0];
    if (i < 0 || j < 0) return INF;
    if (dp[i][j] != -1) return dp[i][j];

    int up = solveMemo(i - 1, j, grid, dp);
    int left = solveMemo(i, j - 1, grid, dp);

    return dp[i][j] = grid[i][j] + min(up, left);
}

int minPathSumMemo(vector<vector<int>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    vector<vector<int>> dp(m, vector<int>(n, -1));
    return solveMemo(m - 1, n - 1, grid, dp);
}

/* ---------------- Tabulation ---------------- */

int minPathSumTab(vector<vector<int>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    vector<vector<int>> dp(m, vector<int>(n, 0));

    dp[0][0] = grid[0][0];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) continue;

            int up = INF;
            int left = INF;

            if (i > 0) up = dp[i - 1][j];
            if (j > 0) left = dp[i][j - 1];

            dp[i][j] = grid[i][j] + min(up, left);
        }
    }

    return dp[m - 1][n - 1];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    cout << "Memoization: " << minPathSumMemo(grid) << '\n';
    cout << "Tabulation: " << minPathSumTab(grid) << '\n';

    return 0;
}
