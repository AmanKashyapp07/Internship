/**
 * Problem: Largest Divisible Subset (https://leetcode.com/problems/largest-divisible-subset/)
 * Find the largest subset of numbers such that every pair of elements satisfies divisibility.
 * Time: O(N^2) time, O(N) space.
 */
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        int n = nums.size(); if (n == 0) return {};
        sort(nums.begin(), nums.end()); // Sort to simplify divisibility checking
        vector<int> dp(n, 1), parent(n, -1);
        int maxLen = 1, lastIdx = 0;
        // DP state: dp[i] is the size of the largest divisible subset ending at index i
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1; parent[i] = j;
                }
            }
            if (dp[i] > maxLen) { maxLen = dp[i]; lastIdx = i; }
        }
        vector<int> ans;
        for (int curr = lastIdx; curr != -1; curr = parent[curr]) ans.push_back(nums[curr]); // Reconstruct path
        reverse(ans.begin(), ans.end());
        return ans;
    }
};