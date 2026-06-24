/**
 * Problem: Longest Bitonic Sequence (https://practice.geeksforgeeks.org/problems/longest-bitonic-subsequence0824/1)
 * Find the length of the longest subsequence that first increases then decreases.
 * Time: O(N^2) time, O(N^2) space.
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