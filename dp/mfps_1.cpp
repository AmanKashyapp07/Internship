/**
 * LeetCode 931 - Minimum Falling Path Sum
 *
 * Description:
 * Given an n x n array of integers matrix, return the minimum sum of any falling path through matrix.
 * A falling path starts at any element in the first row and chooses the element in the next row that is
 * either directly below, or diagonally left/right.
 *
 * Approach:
 * - Dynamic programming using tabulation (bottom-up).
 * - Let `dp[row][col]` be the minimum path sum to reach `(row, col)` from the bottom.
 * - Transition: `dp[row][col] = matrix[row][col] + min({dp[row+1][col-1], dp[row+1][col], dp[row+1][col+1]})`.
 * - Base case: the last row of DP is initialized to the last row of matrix.
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
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();

        vector<vector<int>> dp(n, vector<int>(n));

        // Base case
        for (int col = 0; col < n; col++)
            dp[n - 1][col] = matrix[n - 1][col];

        // Fill from bottom to top
        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col < n; col++) {

                int leftDiag = (col > 0) ? dp[row + 1][col - 1] : INT_MAX;
                int down = dp[row + 1][col];
                int rightDiag = (col < n - 1) ? dp[row + 1][col + 1] : INT_MAX;

                dp[row][col] = matrix[row][col] +
                               min({leftDiag, down, rightDiag});
            }
        }

        return *min_element(dp[0].begin(), dp[0].end());
    }
};