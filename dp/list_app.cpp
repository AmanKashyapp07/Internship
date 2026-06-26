/**
 * GeeksforGeeks - Maximum Sum Increasing Subsequence (MSIS)
 *
 * Description:
 * Given an array of n positive integers, find the sum of maximum sum increasing subsequence of the given array.
 *
 * Approach:
 * - Dynamic programming using Memoization or Tabulation.
 * - Let `dp[i]` be the maximum sum of an increasing subsequence ending at index `i`.
 * - Transition: `dp[i] = max(dp[j] + arr[i])` for all `j < i` where `arr[j] < arr[i]`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Recursive memoization version of MSIS
long long solve(int i, int prev, vector<int>& arr, vector<vector<long long>>& dp) {
    if (i == (int)arr.size()) return 0;
    if (dp[i][prev + 1] != -1) return dp[i][prev + 1];
    long long notTake = solve(i + 1, prev, arr, dp), take = 0;
    if (prev == -1 || arr[i] > arr[prev]) take = arr[i] + solve(i + 1, i, arr, dp);
    return dp[i][prev + 1] = max(take, notTake);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    vector<int> arr(n);
    for (int &x : arr) cin >> x;
    vector<long long> dp(n);
    // Iterative DP: dp[i] represents the maximum sum of an increasing subsequence ending at index i
    for (int i = 0; i < n; i++) {
        dp[i] = arr[i];
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) dp[i] = max(dp[i], dp[j] + arr[i]); // Transition
        }
    }
    cout << *max_element(dp.begin(), dp.end()) << '\n';
}