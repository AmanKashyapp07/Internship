/**
 * LeetCode 416 - Partition Equal Subset Sum
 *
 * Description:
 * Given an integer array nums, return true if you can partition the array into two subsets
 * such that the sum of the elements in both subsets is equal.
 *
 * Approach:
 * - 0/1 Knapsack dynamic programming.
 * - Compute `totalSum`. If it is odd, partition is impossible (return false). Otherwise, target sum is `totalSum / 2`.
 * - Let `dp[i][j]` be a boolean indicating if a subset of the first `i` elements can sum up to `j`.
 * - Transition: `dp[i][j] = dp[i-1][j] || dp[i-1][j - nums[i]]` (if `nums[i] <= j`).
 *
 * Time Complexity: O(n * target)
 * Space Complexity: O(n * target)
 */

#include <vector>
#include <numeric>

using namespace std;

class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        if (totalSum % 2 != 0)
            return false;

        int n = nums.size();
        int target = totalSum / 2;

        vector<vector<bool>> dp(n, vector<bool>(target + 1, false));

        // Base case: target = 0 is always possible
        for (int i = 0; i < n; i++)
            dp[i][0] = true;

        // Base case: using only the first element
        if (nums[0] <= target)
            dp[0][nums[0]] = true;

        // Fill the table
        for (int index = 1; index < n; index++) {
            for (int t = 1; t <= target; t++) {

                bool exclude = dp[index - 1][t];

                bool include = false;
                if (nums[index] <= t)
                    include = dp[index - 1][t - nums[index]];

                dp[index][t] = exclude || include;
            }
        }

        return dp[n - 1][target];
    }
};