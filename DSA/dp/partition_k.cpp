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
    vector<int> nums;
    vector<int> memo;
    int target, n;
    
    bool dfs(int mask, int currSum) {
        if (mask == (1 << n) - 1) return true;
        if (memo[mask] != -1) return memo[mask];
        
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i)) && currSum + nums[i] <= target) {
                int newSum = (currSum + nums[i]) % target;
                if (dfs(mask | (1 << i), newSum)) {
                    return memo[mask] = 1;
                }
            }
        }
        return memo[mask] = 0;
    }
    
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int total = accumulate(nums.begin(), nums.end(), 0);
        if (total % k) return false;
        
        this->nums = nums;
        this->target = total / k;
        this->n = nums.size();
        memo.assign(1 << n, -1);
        
        return dfs(0, 0);
    }
};