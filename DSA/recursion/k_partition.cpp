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
 * Time Complexity: O(2^N * N)
 * Space Complexity: O(2^N)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

class Solution {
private:
    int target;
    vector<int> memo;

    bool solve(int mask, const vector<int>& nums) {
        if (mask == (1 << nums.size()) - 1) {
            return true;
        }
        if (memo[mask] != -1) {
            return memo[mask];
        }

        // Calculate the sum accumulated in the current bucket
        int current_sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (mask & (1 << i)) {
                current_sum += nums[i];
            }
        }
        current_sum %= target;

        for (int j = 0; j < nums.size(); j++) {
            if (!(mask & (1 << j))) { // if j-th element is not used yet
                if (current_sum + nums[j] <= target) {
                    if (solve(mask | (1 << j), nums)) {
                        return memo[mask] = 1;
                    }
                }
            }
        }

        return memo[mask] = 0;
    }
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % k != 0) return false;
        target = sum / k;
        
        // Sorting in descending order optimizes search space pruning
        sort(nums.rbegin(), nums.rend());
        if (nums[0] > target) return false;

        memo.assign(1 << nums.size(), -1);
        return solve(0, nums);
    }
};

int main() {
    Solution solver;
    vector<int> nums = {4, 3, 2, 3, 5, 2, 1};
    int k = 4;
    if (solver.canPartitionKSubsets(nums, k)) {
        cout << "Possible\n";
    } else {
        cout << "Not Possible\n";
    }
    return 0;
}
