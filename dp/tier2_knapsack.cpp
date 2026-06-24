/**
 * Problem: Knapsack Tier 2 Variants
 * Reference templates for Unbounded Knapsack, Rod Cutting, and Perfect Sum (handling zeros).
 * Time: O(N * W) time, Space: O(N * W) or O(W) auxiliary space.
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

// 1. Unbounded Knapsack: items can be chosen infinitely
int unbounded_knapsack_memo(int i, int W, const vector<int>& wt, const vector<int>& val, vector<vector<int>>& dp) {
    if (i == 0) return (W / wt[0]) * val[0];
    if (dp[i][W] != -1) return dp[i][W];
    int notTake = unbounded_knapsack_memo(i - 1, W, wt, val, dp), take = -1e9;
    if (wt[i] <= W) take = val[i] + unbounded_knapsack_memo(i, W - wt[i], wt, val, dp); // Unbounded: stay at index i
    return dp[i][W] = max(take, notTake);
}

int unbounded_knapsack_tab(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size(); vector<vector<int>> dp(n, vector<int>(max_W + 1, 0));
    for (int w = 0; w <= max_W; w++) dp[0][w] = (w / wt[0]) * val[0];
    for (int i = 1; i < n; i++) {
        for (int w = 0; w <= max_W; w++) {
            dp[i][w] = max(dp[i - 1][w], (wt[i] <= w ? val[i] + dp[i][w - wt[i]] : -(int)1e9));
        }
    }
    return dp[n - 1][max_W];
}

// 2. Rod Cutting: maximize price from cutting a rod of length N (equivalent to Unbounded Knapsack)
int rod_cutting(int n, const vector<int>& prices) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) wt[i] = i + 1;
    return unbounded_knapsack_tab(wt, prices, n);
}

int rod_cutting_1D(int n, const vector<int>& prices) {
    vector<int> dp(n + 1, 0);
    for (int i = 0; i < n; i++) {
        int len = i + 1, price = prices[i];
        for (int w = len; w <= n; w++) dp[w] = max(dp[w], price + dp[w - len]);
    }
    return dp[n];
}

// 3. Perfect Sum: count subsets with sum K (handles zeros properly)
int perfect_sum_memo_helper(int i, int sum, const vector<int>& arr, vector<vector<int>>& dp) {
    if (i == 0) {
        if (sum == 0 && arr[0] == 0) return 2; // Perfect Sum: include or exclude 0 when target is 0
        if (sum == 0 || sum == arr[0]) return 1;
        return 0;
    }
    if (dp[i][sum] != -1) return dp[i][sum];
    int notTake = perfect_sum_memo_helper(i - 1, sum, arr, dp), take = 0;
    if (arr[i] <= sum) take = perfect_sum_memo_helper(i - 1, sum - arr[i], arr, dp);
    return dp[i][sum] = take + notTake;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
}
