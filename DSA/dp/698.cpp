/**
 * LeetCode 698 - Partition to K Equal Sum Subsets
 *
 * Description:
 * Given an integer array nums, you can perform operations to delete an element and earn points.
 * When you delete an element, you must also delete all elements equal to nums[i] - 1 and nums[i] + 1.
 * Return the maximum points you can earn.
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
public:
    vector<int> nums_;
    vector<int> memo_;
    int target_;
    int n_;

    bool solve(int mask) {
        if (mask == (1 << n_) - 1) {
            return true;
        }
        if (memo_[mask] != -1) {
            return memo_[mask] == 1;
        }
        int current_sum = 0;
        for (int i = 0; i < n_; ++i) {
            if (mask & (1 << i)) {
                current_sum += nums_[i];
            }
        }
        current_sum %= target_; 
        
        // Try to add an unused number to the current bucket
        for (int j = 0; j < n_; ++j) {
            // Check if the j-th bit is NOT set (nums[j] is free)
            if ((mask & (1 << j)) == 0) {
                if (current_sum + nums_[j] <= target_) {
                    
                    int next_mask = mask | (1 << j); // Set the j-th bit
                    
                    // Recursively check if taking this path leads to a solution
                    if (solve(next_mask)) {
                        memo_[mask] = 1; // Cache as true
                        return true;
                    }
                }
            }
        }
        
        // If we tried all available numbers and none worked, cache as false
        memo_[mask] = 0;
        return false;
    }

    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int total_sum = 0;
        for (int num : nums) {
            total_sum += num;
        }
        if (total_sum % k != 0) {
            return false;
        }
        
        target_ = total_sum / k;
        nums_ = nums;
        n_ = nums.size();
        memo_.assign(1 << n_, -1);
        return solve(0);
    }
};