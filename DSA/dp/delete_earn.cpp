/**
 * LeetCode 740 - Delete and Earn
 *
 * Description:
 * Given an integer array nums, you can perform operations to delete an element and earn points.
 * When you delete an element, you must also delete all elements equal to nums[i] - 1 and nums[i] + 1.
 * Return the maximum points you can earn.
 *
 * Approach:
 * - Reduce the problem to House Robber style DP.
 * - Aggregate the values of nums into a frequency / points sum array `points` where `points[i]` represents total points from all occurrences of `i`.
 * - Reuse the House Robber solution (memoized recursive DP) on the `points` array.
 *
 * Time Complexity: O(n + maxVal) where maxVal is the maximum element in nums.
 * Space Complexity: O(maxVal)
 */

#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int robHelper(int i, const vector<int>& nums, vector<int>& dp) {
        // Base cases
        if (i == 0) return nums[0];
        if (i == 1) return max(nums[0], nums[1]);

        // If already computed, return the result
        if (dp[i] != -1) return dp[i];

        // Recursive relation: max of skipping or robbing the current house
        dp[i] = max(robHelper(i - 1, nums, dp), nums[i] + robHelper(i - 2, nums, dp));
        return dp[i];
    }

    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0; // No houses to rob
        if (n == 1) return nums[0]; // Only one house

        // DP array to memoize results
        vector<int> dp(n, -1);

        // Start from the last house
        return robHelper(n - 1, nums, dp);
    }

    int deleteAndEarn(vector<int>& nums) {
        if (nums.empty()) return 0;
        
        // Find the maximum value to know how large our "houses" array needs to be
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // Accumulate total points possible for each number
        vector<int> points(maxVal + 1, 0);
        for (int num : nums) {
            points[num] += num;
        }
        
        // Reuse House Robber solution
        return rob(points);
    }
};