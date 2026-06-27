#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// =============================================================================
// RECURSIVE DP CORE ENGINE (Top-Down Helpers)
// =============================================================================

int unboundedKnapsackMemoHelper(int idx, int W, const vector<int> &wt,
                                const vector<int> &val,
                                vector<vector<int>> &dp) {
  if (idx == wt.size())
    return 0;
  if (dp[idx][W] != -1)
    return dp[idx][W];

  int notTake = unboundedKnapsackMemoHelper(idx + 1, W, wt, val, dp);
  int take = -1e9;
  if (wt[idx] <= W) {
    // Unbounded track: stay at the same index 'idx' upon selection
    take =
        val[idx] + unboundedKnapsackMemoHelper(idx, W - wt[idx], wt, val, dp);
  }
  return dp[idx][W] = max(take, notTake);
}

int perfectSumMemoHelper(int idx, int target, const vector<int> &arr,
                         vector<vector<int>> &dp) {
  if (idx == arr.size())
    return target == 0 ? 1 : 0;
  if (dp[idx][target] != -1)
    return dp[idx][target];

  int notTake = perfectSumMemoHelper(idx + 1, target, arr, dp);
  int take = 0;
  if (arr[idx] <= target) {
    take = perfectSumMemoHelper(idx + 1, target - arr[idx], arr, dp);
  }
  return dp[idx][target] = take + notTake;
}

// =============================================================================
// INTERFACE ARCHITECTURE: MEMOIZATION (Top-Down)
// =============================================================================

int unbounded_knapsack_memo(const vector<int> &wt, const vector<int> &val,
                            int max_W) {
  int n = wt.size();
  vector<vector<int>> dp(n, vector<int>(max_W + 1, -1));
  return unboundedKnapsackMemoHelper(0, max_W, wt, val, dp);
}

int rod_cutting_memo(int n, const vector<int> &prices) {
  vector<int> wt(n);
  for (int i = 0; i < n; ++i)
    wt[i] = i + 1;
  return unbounded_knapsack_memo(wt, prices, n);
}

int perfect_sum_memo(const vector<int> &arr, int target) {
  int n = arr.size();
  vector<vector<int>> dp(n, vector<int>(target + 1, -1));
  return perfectSumMemoHelper(0, target, arr, dp);
}

// =============================================================================
// INTERFACE ARCHITECTURE: TABULATION (Bottom-Up)
// =============================================================================

int unbounded_knapsack_tab(const vector<int> &wt, const vector<int> &val,
                           int max_W) {
  int n = wt.size();
  vector<vector<int>> dp(n + 1, vector<int>(max_W + 1, 0));

  for (int i = 1; i <= n; ++i) {
    for (int w = 0; w <= max_W; ++w) {
      dp[i][w] = dp[i - 1][w];
      if (wt[i - 1] <= w) {
        // Unbounded link: references row 'i' directly rather than 'i - 1'
        dp[i][w] = max(dp[i][w], val[i - 1] + dp[i][w - wt[i - 1]]);
      }
    }
  }
  return dp[n][max_W];
}

int rod_cutting_tab(int n, const vector<int> &prices) {
  vector<int> wt(n);
  for (int i = 0; i < n; ++i)
    wt[i] = i + 1;
  return unbounded_knapsack_tab(wt, prices, n);
}

int perfect_sum_tab(const vector<int> &arr, int target) {
  int n = arr.size();
  vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
  dp[0][0] = 1;

  for (int i = 1; i <= n; ++i) {
    for (int t = 0; t <= target; ++t) {
      dp[i][t] = dp[i - 1][t];
      if (arr[i - 1] <= t) {
        dp[i][t] += dp[i - 1][t - arr[i - 1]];
      }
    }
  }
  return dp[n][target];
}

// =============================================================================
// VERIFICATION SANDBOX
// =============================================================================
int main() {
  // 1. Unbounded Knapsack
  vector<int> wt = {2, 4, 6};
  vector<int> val = {5, 11, 13};
  assert(unbounded_knapsack_memo(wt, val, 10) == 27);
  assert(unbounded_knapsack_tab(wt, val, 10) == 27);

  // 2. Rod Cutting
  vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20};
  assert(rod_cutting_memo(8, prices) == 22);
  assert(rod_cutting_tab(8, prices) == 22);

  // 3. Perfect Sum (Robust validation against zeroes)
  vector<int> arr = {1, 2, 0, 3};
  assert(perfect_sum_memo(arr, 3) == 4);
  assert(perfect_sum_tab(arr, 3) == 4);

  cout << "All clean Tier-2 Knapsack variants passed successfully!" << endl;
  return 0;
}