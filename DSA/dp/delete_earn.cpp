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
 * - Aggregate the values of nums into a frequency / points sum array `sum` where `sum[i]` represents total points from all occurrences of `i`.
 * - Perform 1D DP: `dp[i] = max(dp[i-1], dp[i-2] + sum[i])`.
 *
 * Time Complexity: O(n + maxVal) where maxVal is the maximum element in nums.
 * Space Complexity: O(maxVal)
 */



#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        if (nums.empty()) return 0;
        
        // Find the maximum value to know how large our "houses" array needs to be
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // Accumulate total points possible for each number
        vector<int> points(maxVal + 1, 0);
        for (int num : nums) {
            points[num] += num;
        }
        
        // Standard House Robber space-optimized DP
        int prev2 = 0; // Represents dp[i-2]
        int prev1 = 0; // Represents dp[i-1]
        
        for (int i = 0; i <= maxVal; ++i) {
            int current = max(prev1, prev2 + points[i]);
            prev2 = prev1;
            prev1 = current;
        }
        
        return prev1;
    }
};