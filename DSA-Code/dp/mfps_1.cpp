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
    vector<vector<int>> memo;
    vector<vector<int>> mat;

    int minFallingPathSum(vector<vector<int>>& matrix) {
        mat = matrix;
        int n = mat.size();

        memo.assign(n, vector<int>(n, 0));

        // Base case
        for (int col = 0; col < n; col++)
            memo[n - 1][col] = mat[n - 1][col];

        // Fill from bottom to top
        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col < n; col++) {

                int leftDiag = (col > 0) ? memo[row + 1][col - 1] : INT_MAX;
                int down = memo[row + 1][col];
                int rightDiag = (col < n - 1) ? memo[row + 1][col + 1] : INT_MAX;

                memo[row][col] = mat[row][col] +
                               min({leftDiag, down, rightDiag});
            }
        }

        return *min_element(memo[0].begin(), memo[0].end());
    }
};