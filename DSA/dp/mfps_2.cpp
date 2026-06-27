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
    vector<vector<int>> memo;
    vector<vector<int>> matrix;

    int minFallingPathSum(vector<vector<int>>& grid) {
        matrix = grid;
        int n = matrix.size();

        memo.assign(n, vector<int>(n, 0));

        // Base case
        for (int col = 0; col < n; col++)
            memo[n - 1][col] = matrix[n - 1][col];

        // Build DP
        for (int row = n - 2; row >= 0; row--) {

            // Find minimum value
            int min1 = *min_element(memo[row + 1].begin(), memo[row + 1].end());

            // Find column of minimum value
            int col1 = -1;
            for (int col = 0; col < n; col++) {
                if (memo[row + 1][col] == min1) {
                    col1 = col;
                    break;
                }
            }

            // Find second minimum value
            int min2 = INT_MAX;
            for (int col = 0; col < n; col++) {
                if (col != col1)
                    min2 = min(min2, memo[row + 1][col]);
            }

            // Fill current row
            for (int col = 0; col < n; col++) {
                if (col == col1)
                    memo[row][col] = matrix[row][col] + min2;
                else
                    memo[row][col] = matrix[row][col] + min1;
            }
        }

        return *min_element(memo[0].begin(), memo[0].end());
    }
};