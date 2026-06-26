/**
 * LeetCode 120 - Triangle
 *
 * Description:
 * Given a triangle array, return the minimum path sum from top to bottom.
 * For each step, you may move to adjacent numbers on the row below.
 *
 * Approach:
 * - Bottom-up 1D dynamic programming.
 * - Initialize the DP state with the bottom row of the triangle.
 * - Traverse rows upwards: `dp[col] = triangle[row][col] + min(dp[col], dp[col+1])`.
 * - The answer will accumulate at `dp[0]`.
 *
 * Time Complexity: O(n^2) where n is the number of rows.
 * Space Complexity: O(n)
 */



#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();

        vector<int> dp = triangle[n - 1];

        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col <= row; col++) {
                dp[col] = triangle[row][col] + min(dp[col], dp[col + 1]);
            }
        }

        return dp[0];
    }
};