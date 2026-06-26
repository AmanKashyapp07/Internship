/**
 * LeetCode 698 - Partition to K Equal Sum Subsets
 *
 * Description:
 * Given an integer array nums and an integer k, return true if it is possible to partition this array
 * into k subsets whose sums are all equal.
 *
 * Approach:
 * - Bitmask Dynamic Programming with Memoization.
 * - Let `solve(mask)` return if we can partition the remaining elements (defined by `mask`) into subsets of size `target`.
 * - Track current subset sum as `current_sum = (sum of elements in mask) % target`.
 * - Transition: For each unused element `j`, if `current_sum + nums[j] <= target`, transition to `mask | (1 << j)`.
 *
 * Time Complexity: O(2^n * n)
 * Space Complexity: O(2^n)
 */

#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
private:

    bool solve(int mask, const vector<int>& nums, vector<int>& memo, int target) {
        int n = nums.size();
        if (mask == (1 << n) - 1) {
            return true;
        }
        if (memo[mask] != -1) {
            return memo[mask] == 1;
        }
        int current_sum = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                current_sum += nums[i];
            }
        }
        current_sum %= target; 
        
        // Try to add an unused number to the current bucket
        for (int j = 0; j < n; ++j) {
            // Check if the j-th bit is NOT set (nums[j] is free)
            if ((mask & (1 << j)) == 0) {
                if (current_sum + nums[j] <= target) {
                    
                    int next_mask = mask | (1 << j); // Set the j-th bit
                    
                    // Recursively check if taking this path leads to a solution
                    if (solve(next_mask, nums, memo, target)) {
                        memo[mask] = 1; // Cache as true
                        return true;
                    }
                }
            }
        }
        
        // If we tried all available numbers and none worked, cache as false
        memo[mask] = 0;
        return false;
    }

public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int total_sum = 0;
        for (int num : nums) {
            total_sum += num;
        }
        if (total_sum % k != 0) {
            return false;
        }
        
        int target = total_sum / k;
        int n = nums.size();
        vector<int> memo(1 << n, -1);
        return solve(0, nums, memo, target);
    }
};