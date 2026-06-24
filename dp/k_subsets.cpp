/**
 * Problem: Partition to K Equal Sum Subsets (Backtracking) (https://leetcode.com/problems/partition-to-k-equal-sum-subsets/)
 * Partition an array into k subsets with equal sum.
 * Time: O(K^N) worst-case, O(N) space.
 */
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

class Solution_Backtracking {
    bool dfs(int idx, vector<int>& nums, vector<int>& bucket, int target) {
        if (idx == nums.size()) return true;
        for (size_t i = 0; i < bucket.size(); i++) {
            if (bucket[i] + nums[idx] > target) continue;
            bucket[i] += nums[idx];
            if (dfs(idx + 1, nums, bucket, target)) return true;
            bucket[i] -= nums[idx];
            if (bucket[i] == 0) break; // Pruning: if empty bucket fails, subsequent identical empty buckets will also fail
        }
        return false;
    }
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int total = accumulate(nums.begin(), nums.end(), 0);
        if (total % k != 0) return false;
        int target = total / k;
        sort(nums.rbegin(), nums.rend()); // Sort descending to fail early on large values
        if (nums[0] > target) return false;
        vector<int> bucket(k, 0);
        return dfs(0, nums, bucket, target);
    }
};