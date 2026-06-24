/**
 * Problem: Partition to K Equal Sum Subsets (Bitmask DP) (https://leetcode.com/problems/partition-to-k-equal-sum-subsets/)
 * Partition an array into k subsets with equal sum.
 * Time: O(2^N * N) time, O(2^N) space.
 */
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

class Solution {
    int n, target;
    vector<int> nums, dp;
    bool dfs(int mask, int currSum, int k) {
        if (k == 1) return true; // Base case: last remaining bucket must succeed if sum fits
        if (currSum == target) return dfs(mask, 0, k - 1); // Bucket filled, move to next
        if (dp[mask] != -1) return dp[mask];
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i)) && currSum + nums[i] <= target) {
                if (dfs(mask | (1 << i), currSum + nums[i], k)) return dp[mask] = 1;
            }
        }
        return dp[mask] = 0;
    }
public:
    bool canPartitionKSubsets(vector<int>& arr, int k) {
        nums = arr; n = nums.size();
        int total = accumulate(nums.begin(), nums.end(), 0);
        if (total % k != 0) return false;
        target = total / k;
        sort(nums.rbegin(), nums.rend());
        if (nums[0] > target) return false;
        dp.assign(1 << n, -1);
        return dfs(0, 0, k);
    }
};