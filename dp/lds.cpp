/**
 * Problem: Longest Decreasing Subsequence (LDS)
 *
 * Description:
 * Given an array of integers, find the length of the longest decreasing subsequence.
 *
 * Approach:
 * - 1D dynamic programming.
 * - Let `dp[i]` represent the length of the LDS ending at index `i`.
 * - Transition: `dp[i] = max(dp[j] + 1)` for all `j < i` where `arr[j] > arr[i]`.
 * - Initialize all `dp[i] = 1`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int lds(const vector<int>& arr) {
    int n = arr.size(); if (n == 0) return 0;
    vector<int> dp(n, 1); int max_len = 1;
    // DP state: dp[i] is the length of LDS ending at index i
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] < arr[j]) dp[i] = max(dp[i], dp[j] + 1);
        }
        max_len = max(max_len, dp[i]);
    }
    return max_len;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (cin >> n) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) cin >> arr[i];
        cout << lds(arr) << "\n";
    }
}