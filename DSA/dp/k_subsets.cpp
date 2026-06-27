/**
 * LeetCode 698 - Partition to K Equal Sum Subsets (Backtracking)
 *
 * Description:
 * Given an integer array nums and an integer k, return true if it is possible to partition this array
 * into k subsets whose sums are all equal.
 *
 * Approach:
 * - Solve using recursive backtracking.
 * - Sort `nums` in descending order to optimize pruning.
 * - Maintain an array of bucket sums. Try placing the current element in each of the k buckets.
 * - Prune redundant searches (e.g. skip duplicate empty buckets).
 *
 * Time Complexity: O(k^n) worst-case.
 * Space Complexity: O(n) recursion stack.
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