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
    vector<int> memo;
    vector<vector<int>> tri;

    int minimumTotal(vector<vector<int>>& triangle) {
        tri = triangle;
        int n = tri.size();

        memo = tri[n - 1];

        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col <= row; col++) {
                memo[col] = tri[row][col] + min(memo[col], memo[col + 1]);
            }
        }

        return memo[0];
    }
};