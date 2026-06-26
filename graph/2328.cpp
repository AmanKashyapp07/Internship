/**
 * LeetCode 2328 - Number of Increasing Paths in a Grid
 *
 * Description:
 * Given an m x n grid, count the total number of strictly increasing paths.
 * A path may start and end at any cell and moves are allowed in the four
 * adjacent directions.
 *
 * Return the answer modulo 1e9 + 7.
 *
 * Approach:
 * - Let dp[r][c] denote the number of increasing paths starting from cell (r, c).
 * - Every cell itself forms one valid path.
 * - Use DFS + Memoization.
 * - From the current cell, recursively visit all neighbors having a larger value.
 * - Since values must strictly increase, cycles are impossible.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

class Solution {
public:
    static constexpr int MOD = 1e9 + 7;

    int rows, cols;

    vector<vector<int>> dp;

    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};

    // Returns the number of increasing paths starting from (r, c)
    int dfs(int r, int c, vector<vector<int>>& grid) {

        // Already computed
        if (dp[r][c] != -1)
            return dp[r][c];

        // Path containing only the current cell
        long long paths = 1;

        // Try moving to all larger neighboring cells
        for (int k = 0; k < 4; k++) {

            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols)
                continue;

            if (grid[nr][nc] <= grid[r][c])
                continue;

            paths = (paths + dfs(nr, nc, grid)) % MOD;
        }

        return dp[r][c] = paths;
    }

    int countPaths(vector<vector<int>>& grid) {

        rows = grid.size();
        cols = grid[0].size();

        dp.assign(rows, vector<int>(cols, -1)); // dp[i][j] = number of increasing paths starting from cell (i, j)

        long long ans = 0;

        // Count increasing paths starting from every cell
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                ans = (ans + dfs(r, c, grid)) % MOD;
            }
        }

        return ans;
    }
};