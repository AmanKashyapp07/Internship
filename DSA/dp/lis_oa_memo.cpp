#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 1. Longest Increasing Subsequence (LIS)
// ============================================================================

int solve(int idx, int prev, vector<int> &arr, vector<vector<int>> &dp) {
  if (idx == arr.size())
    return 0;

  if (dp[idx][prev + 1] != -1)
    return dp[idx][prev + 1];

  int skip = solve(idx + 1, prev, arr, dp);

  int take = 0;
  if (prev == -1 || arr[idx] > arr[prev])
    take = 1 + solve(idx + 1, idx, arr, dp);

  return dp[idx][prev + 1] = max(take, skip);
}

int lengthOfLISMemo(vector<int> &arr) {
  int n = arr.size();
  vector<vector<int>> dp(n, vector<int>(n + 1, -1));
  return solve(0, -1, arr, dp);
}

// ============================================================================
// 2. Maximum Sum Increasing Subsequence
// ============================================================================

int solve(int idx, int prev, vector<int> &arr, vector<vector<int>> &dp) {
  if (idx == arr.size())
    return 0;

  if (dp[idx][prev + 1] != -1)
    return dp[idx][prev + 1];

  int skip = solve(idx + 1, prev, arr, dp);

  int take = 0;
  if (prev == -1 || arr[idx] > arr[prev])
    take = arr[idx] + solve(idx + 1, idx, arr, dp);

  return dp[idx][prev + 1] = max(take, skip);
}

int maxSumISMemo(vector<int> &arr) {
  int n = arr.size();
  vector<vector<int>> dp(n, vector<int>(n + 1, -1));
  return solve(0, -1, arr, dp);
}

// ============================================================================
// 3. Longest Non-Decreasing Subsequence
// ============================================================================

int solve(int idx, int prev, vector<int> &arr, vector<vector<int>> &dp) {
  if (idx == arr.size())
    return 0;

  if (dp[idx][prev + 1] != -1)
    return dp[idx][prev + 1];

  int skip = solve(idx + 1, prev, arr, dp);

  int take = 0;
  if (prev == -1 || arr[idx] >= arr[prev])
    take = 1 + solve(idx + 1, idx, arr, dp);

  return dp[idx][prev + 1] = max(take, skip);
}

int lengthOfLNDSMemo(vector<int> &arr) {
  int n = arr.size();
  vector<vector<int>> dp(n, vector<int>(n + 1, -1));
  return solve(0, -1, arr, dp);
}

// ============================================================================
// 4. Longest Chain of Pairs
// ============================================================================

int solve(int idx, int prev, vector<vector<int>> &arr,
          vector<vector<int>> &dp) {
  if (idx == arr.size())
    return 0;

  if (dp[idx][prev + 1] != -1)
    return dp[idx][prev + 1];

  int skip = solve(idx + 1, prev, arr, dp);

  int take = 0;
  if (prev == -1 || arr[idx][0] > arr[prev][1])
    take = 1 + solve(idx + 1, idx, arr, dp);

  return dp[idx][prev + 1] = max(take, skip);
}

int findLongestChainMemo(vector<vector<int>> &arr) {
  sort(arr.begin(), arr.end());

  int n = arr.size();
  vector<vector<int>> dp(n, vector<int>(n + 1, -1));

  return solve(0, -1, arr, dp);
}