/**
 * LeetCode 368 - Largest Divisible Subset
 *
 * Description:
 * Given a set of distinct positive integers nums, return the largest subset such that every pair (si, sj)
 * in this subset satisfies: si % sj == 0 or sj % si == 0.
 *
 * Approach:
 * - Sort the array first so we only need to check one-way divisibility.
 * - Let `dp[i]` denote the size of the largest divisible subset ending at index `i`.
 * - Transition: `dp[i] = max(dp[j] + 1)` for all `j < i` where `nums[i] % nums[j] == 0`.
 * - Maintain a parent pointer array to reconstruct the actual subset.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */

#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> dp_;
    vector<int> parent_;
    vector<int> nums_;

    vector<int> largestDivisibleSubset(vector<int>& nums) {
        int n = nums.size(); 
        if (n == 0) return {};
        nums_ = nums;
        sort(nums_.begin(), nums_.end()); // Sort to simplify divisibility checking
        
        dp_.assign(n, 1);
        parent_.assign(n, -1);
        int maxLen = 1, lastIdx = 0;
        
        // DP state: dp[i] is the size of the largest divisible subset ending at index i
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums_[i] % nums_[j] == 0 && dp_[j] + 1 > dp_[i]) {
                    dp_[i] = dp_[j] + 1; 
                    parent_[i] = j;
                }
            }
            if (dp_[i] > maxLen) { 
                maxLen = dp_[i]; 
                lastIdx = i; 
            }
        }
        
        vector<int> ans;
        for (int curr = lastIdx; curr != -1; curr = parent_[curr]) {
            ans.push_back(nums_[curr]); // Reconstruct path
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};