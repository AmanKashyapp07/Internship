/**
 * Problem: Knapsack Tier 2 Variants
 * Reference templates for Unbounded Knapsack, Rod Cutting, and Perfect Sum (handling zeros).
 * Time: O(N * W) time, Space: O(N * W) auxiliary space (unoptimized 2D tables).
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

// =============================================================================
// Helper Functions for Memoization
// =============================================================================

int unboundedKnapsackMemoHelper(int idx, int W, const vector<int>& wt, const vector<int>& val, vector<vector<int>>& dp) {
    if (idx == wt.size()) return 0;
    if (dp[idx][W] != -1) return dp[idx][W];
    
    int notTake = unboundedKnapsackMemoHelper(idx + 1, W, wt, val, dp);
    int take = -1e9;
    if (wt[idx] <= W) {
        take = val[idx] + unboundedKnapsackMemoHelper(idx, W - wt[idx], wt, val, dp); // stay at idx for unbounded
    }
    return dp[idx][W] = max(take, notTake);
}

int perfectSumMemoHelper(int idx, int target, const vector<int>& arr, vector<vector<int>>& dp) {
    if (idx == arr.size()) {
        return target == 0 ? 1 : 0;
    }
    if (dp[idx][target] != -1) return dp[idx][target];
    
    int notTake = perfectSumMemoHelper(idx + 1, target, arr, dp);
    int take = 0;
    if (arr[idx] <= target) {
        take = perfectSumMemoHelper(idx + 1, target - arr[idx], arr, dp);
    }
    return dp[idx][target] = take + notTake;
}

// =============================================================================
// 1. Unbounded Knapsack: items can be chosen infinitely
// =============================================================================

int unbounded_knapsack_memo(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size();
    vector<vector<int>> dp(n, vector<int>(max_W + 1, -1));
    return unboundedKnapsackMemoHelper(0, max_W, wt, val, dp);
}

int unbounded_knapsack_tab(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size();
    vector<vector<int>> dp(n + 1, vector<int>(max_W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= max_W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (wt[i - 1] <= w) {
                dp[i][w] = max(dp[i][w], val[i - 1] + dp[i][w - wt[i - 1]]);
            }
        }
    }
    return dp[n][max_W];
}

// =============================================================================
// 2. Rod Cutting: maximize price from cutting a rod of length N
// =============================================================================

int rod_cutting_memo(int n, const vector<int>& prices) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) wt[i] = i + 1;
    return unbounded_knapsack_memo(wt, prices, n);
}

int rod_cutting_tab(int n, const vector<int>& prices) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) wt[i] = i + 1;
    return unbounded_knapsack_tab(wt, prices, n);
}

// =============================================================================
// 3. Perfect Sum: count subsets with sum K (handles zeros properly)
// =============================================================================

int perfect_sum_memo(const vector<int>& arr, int target) {
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));
    return perfectSumMemoHelper(0, target, arr, dp);
}

int perfect_sum_tab(const vector<int>& arr, int target) {
    int n = arr.size();
    vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
    dp[0][0] = 1; // Base case: 0 elements can sum to 0 in 1 way
    
    for (int i = 1; i <= n; i++) {
        for (int t = 0; t <= target; t++) {
            dp[i][t] = dp[i - 1][t];
            if (arr[i - 1] <= t) {
                dp[i][t] += dp[i - 1][t - arr[i - 1]];
            }
        }
    }
    return dp[n][target];
}

// =============================================================================
// Main function for simple local verification
// =============================================================================
int main() {
    // 1. Unbounded Knapsack
    vector<int> wt = {2, 4, 6};
    vector<int> val = {5, 11, 13};
    assert(unbounded_knapsack_memo(wt, val, 10) == 27); // two of wt=4 (val=22) + one of wt=2 (val=5) -> 27
    assert(unbounded_knapsack_tab(wt, val, 10) == 27);
    
    // 2. Rod Cutting
    vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20};
    assert(rod_cutting_memo(8, prices) == 22); // cut into 2 and 6 -> 5 + 17 = 22
    assert(rod_cutting_tab(8, prices) == 22);
    
    // 3. Perfect Sum
    vector<int> arr = {1, 2, 0, 3};
    assert(perfect_sum_memo(arr, 3) == 4); // {1, 2}, {1, 2, 0}, {3}, {3, 0}
    assert(perfect_sum_tab(arr, 3) == 4);
    
    cout << "All knapsack_2 tests passed successfully!" << endl;
    return 0;
}
