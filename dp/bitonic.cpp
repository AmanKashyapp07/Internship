/**
 * GeeksforGeeks - Longest Bitonic Subsequence
 *
 * Description:
 * Given an array of positive integers, find the length of the longest subsequence
 * that is first increasing, then decreasing (bitonic).
 *
 * Approach:
 * - Use dynamic programming / memoization.
 * - `solve(idx, prev, turned)` computes LBS from `idx` with predecessor index `prev` and `turned` boolean flag (0 if still increasing, 1 if decreasing).
 * - Transition:
 *   - If not turned, we can transition to turned = 1 (start decreasing) or stay turned = 0 (continue increasing) if `nums[idx] > nums[prev]`.
 *   - If turned, we can only append if `nums[idx] < nums[prev]`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <vector>
#include <algorithm>
using namespace std;

class Solution {
    int n;
    vector<int> arr;
    vector<vector<vector<int>>> dp;

    int solve(int idx, int prev, int turned) {
        if (idx == n) return turned ? 0 : -1e9; // Base case: sequence must have a decreasing part
        if (dp[idx][prev + 1][turned] != -1) return dp[idx][prev + 1][turned];
        int ans = solve(idx + 1, prev, turned); // Option 1: Skip current element
        if (prev == -1) {
            ans = max(ans, 1 + solve(idx + 1, idx, 0)); // Option 2: Start sequence
        } else {
            if (!turned) {
                if (arr[idx] > arr[prev]) ans = max(ans, 1 + solve(idx + 1, idx, 0)); // Continue increasing
                if (arr[idx] < arr[prev]) ans = max(ans, 1 + solve(idx + 1, idx, 1)); // Transition to decreasing
            } else if (arr[idx] < arr[prev]) {
                ans = max(ans, 1 + solve(idx + 1, idx, 1)); // Continue decreasing
            }
        }
        return dp[idx][prev + 1][turned] = ans;
    }
public:
    int LongestBitonicSequence(vector<int>& nums) {
        arr = nums; n = nums.size();
        dp.assign(n, vector<vector<int>>(n + 1, vector<int>(2, -1)));
        return max(0, solve(0, -1, 0));
    }
};