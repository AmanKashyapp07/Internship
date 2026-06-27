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

class Solution {
public:
    vector<vector<int>> matrix;
    vector<vector<int>> memo;

    /* ---------------- Memoization ---------------- */

    int solve(int i, int j) {
        if (i == 0 && j == 0) return matrix[0][0];
        if (i < 0 || j < 0) return INF;
        if (memo[i][j] != -1) return memo[i][j];

        int up = solve(i - 1, j);
        int left = solve(i, j - 1);

        return memo[i][j] = matrix[i][j] + min(up, left);
    }

    int minPathSumMemo(vector<vector<int>> &grid) {
        matrix = grid;
        int m = matrix.size();
        int n = matrix[0].size();
        memo.assign(m, vector<int>(n, -1));
        return solve(m - 1, n - 1);
    }

    /* ---------------- Tabulation ---------------- */

    int minPathSumTab(vector<vector<int>> &grid) {
        matrix = grid;
        int m = matrix.size();
        int n = matrix[0].size();
        memo.assign(m, vector<int>(n, 0));

        memo[0][0] = matrix[0][0];

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 && j == 0) continue;

                int up = INF;
                int left = INF;

                if (i > 0) up = memo[i - 1][j];
                if (j > 0) left = memo[i][j - 1];

                memo[i][j] = matrix[i][j] + min(up, left);
            }
        }

        return memo[m - 1][n - 1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    Solution solver;
    cout << "Memoization: " << solver.minPathSumMemo(grid) << '\n';
    cout << "Tabulation: " << solver.minPathSumTab(grid) << '\n';

    return 0;
}
