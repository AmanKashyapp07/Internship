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
 * - Track current subset sum as `currentSum = (sum of elements in mask) % target`.
 * - Transition: For each unused element `j`, if `currentSum + nums[j] <= target`, transition to `mask | (1 << j)`.
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
    vector<int> arr;
    vector<int> memo;
    int targetSum;
    int size;

    bool solve(int mask) {
        if (mask == (1 << size) - 1) {
            return true;
        }
        if (memo[mask] != -1) {
            return memo[mask] == 1;
        }
        int currentSum = 0;
        for (int i = 0; i < size; ++i) {
            if (mask & (1 << i)) {
                currentSum += arr[i];
            }
        }
        currentSum %= targetSum; 
        
        // Try to add an unused number to the current bucket
        for (int j = 0; j < size; ++j) {
            // Check if the j-th bit is NOT set (nums[j] is free)
            if ((mask & (1 << j)) == 0) {
                if (currentSum + arr[j] <= targetSum) {
                    
                    int nextMask = mask | (1 << j); // Set the j-th bit
                    
                    // Recursively check if taking this path leads to a solution
                    if (solve(nextMask)) {
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

    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int totalSum = 0;
        for (int num : nums) {
            totalSum += num;
        }
        if (totalSum % k != 0) {
            return false;
        }
        
        targetSum = totalSum / k;
        arr = nums;
        size = nums.size();
        memo.assign(1 << size, -1);
        return solve(0);
    }
};