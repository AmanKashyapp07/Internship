/**
 * LeetCode 329 - Longest Increasing Path in a Matrix
 *
 * Description:
 * Given an m x n integers matrix, return the length of the longest increasing path in matrix.
 * From each cell, you can either move in four directions: left, right, up, or down.
 * You may not move diagonally or move outside the boundary.
 *
 * Approach:
 * - Use Depth First Search (DFS) with Memoization.
 * - For each cell, explore all four directions recursively if the neighbor has a strictly larger value.
 * - Cache the longest path length starting from each cell in a DP table to avoid redundant calculations.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

class Solution {
public:
    int m, n;
    vector<vector<int>> dp;
    vector<int> dx = {1, -1, 0, 0};
    vector<int> dy = {0, 0, 1, -1};

    int dfs(int i, int j, vector<vector<int>>& matrix) {
        if (dp[i][j] != -1) return dp[i][j];

        int best = 1;

        for (int k = 0; k < 4; k++) {
            int ni = i + dx[k];
            int nj = j + dy[k];
            if (ni >= 0 && ni < m && nj >= 0 && nj < n &&
                matrix[ni][nj] > matrix[i][j]) {
                best = max(best, 1 + dfs(ni, nj, matrix));
            }
        }

        return dp[i][j] = best;
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        dp.assign(m, vector<int>(n, -1));

        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                ans = max(ans, dfs(i, j, matrix));
            }
        }

        return ans;
    }
};