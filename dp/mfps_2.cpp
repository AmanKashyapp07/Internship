/**
 * LeetCode 1289 - Minimum Falling Path Sum II
 *
 * Description:
 * Given an n x n integer grid, return the minimum sum of a falling path with non-zero shifts.
 * A falling path with non-zero shifts chooses one element from each row such that no two elements in adjacent rows are in the same column.
 *
 * Approach:
 * - Dynamic programming using tabulation (bottom-up).
 * - In each row, to optimize transitions from the row below, we identify the minimum value `min1` at column `col1` and the second minimum value `min2` from the row below.
 * - Transition:
 *   - If current column `col == col1`, we add the second minimum `min2`: `dp[row][col] = grid[row][col] + min2`.
 *   - Otherwise, we add the first minimum `min1`: `dp[row][col] = grid[row][col] + min1`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& grid) {
        int n = grid.size();

        vector<vector<int>> dp(n, vector<int>(n));

        // Base case
        for (int col = 0; col < n; col++)
            dp[n - 1][col] = grid[n - 1][col];

        // Build DP
        for (int row = n - 2; row >= 0; row--) {

            // Find minimum value
            int min1 = *min_element(dp[row + 1].begin(), dp[row + 1].end());

            // Find column of minimum value
            int col1 = -1;
            for (int col = 0; col < n; col++) {
                if (dp[row + 1][col] == min1) {
                    col1 = col;
                    break;
                }
            }

            // Find second minimum value
            int min2 = INT_MAX;
            for (int col = 0; col < n; col++) {
                if (col != col1)
                    min2 = min(min2, dp[row + 1][col]);
            }

            // Fill current row
            for (int col = 0; col < n; col++) {
                if (col == col1)
                    dp[row][col] = grid[row][col] + min2;
                else
                    dp[row][col] = grid[row][col] + min1;
            }
        }

        return *min_element(dp[0].begin(), dp[0].end());
    }
};