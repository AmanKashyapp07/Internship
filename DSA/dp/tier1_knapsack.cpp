/**
 * Problem: Knapsack Tier 1 Variants
 * Reference implementations for 8 core 0/1 Knapsack and Unbounded Knapsack variants.
 * Time: O(N * W) time, Space: O(N * W) or O(W) auxiliary space.
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
using namespace std;

// 1. Subset Sum: checks if any subset sums to target K
int subset_sum_memo_helper(int i, int sum, const vector<int>& arr, vector<vector<int>>& dp) {
    if (sum == 0) return 1;
    if (i == 0) return (arr[0] == sum);
    if (dp[i][sum] != -1) return dp[i][sum];
    int notTake = subset_sum_memo_helper(i - 1, sum, arr, dp), take = 0;
    if (arr[i] <= sum) take = subset_sum_memo_helper(i - 1, sum - arr[i], arr, dp);
    return dp[i][sum] = take | notTake; // 0/1 Knapsack: transition checks take vs not-take
}

bool subset_sum_memo(const vector<int>& arr, int k) {
    vector<vector<int>> dp(arr.size(), vector<int>(k + 1, -1));
    return subset_sum_memo_helper(arr.size() - 1, k, arr, dp);
}

bool subset_sum_tab(const vector<int>& arr, int k) {
    int n = arr.size(); vector<vector<bool>> dp(n, vector<bool>(k + 1, false));
    for (int i = 0; i < n; i++) dp[i][0] = true;
    if (arr[0] <= k) dp[0][arr[0]] = true;
    for (int i = 1; i < n; i++) {
        for (int target = 1; target <= k; target++) {
            dp[i][target] = dp[i - 1][target] || (arr[i] <= target && dp[i - 1][target - arr[i]]);
        }
    }
    return dp[n - 1][k];
}

// 2. Partition Equal Subset Sum: can array partition into two subsets of equal sum
bool can_partition(const vector<int>& arr) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    return (total_sum % 2 == 0) && subset_sum_tab(arr, total_sum / 2); // Split target into equivalent subset sum
}

// 3. Count Subsets with Sum K
int count_subsets_memo_helper(int i, int sum, const vector<int>& arr, vector<vector<int>>& dp) {
    if (i < 0) return sum == 0;
    if (dp[i][sum] != -1) return dp[i][sum];
    int notTake = count_subsets_memo_helper(i - 1, sum, arr, dp), take = 0;
    if (arr[i] <= sum) take = count_subsets_memo_helper(i - 1, sum - arr[i], arr, dp);
    return dp[i][sum] = take + notTake;
}

int count_subsets_memo(const vector<int>& arr, int k) {
    vector<vector<int>> dp(arr.size(), vector<int>(k + 1, -1));
    return count_subsets_memo_helper(arr.size() - 1, k, arr, dp);
}

int count_subsets_tab(const vector<int>& arr, int k) {
    int n = arr.size(); vector<vector<int>> dp(n, vector<int>(k + 1, 0));
    dp[0][0] = 1; if (arr[0] <= k) dp[0][arr[0]] += 1;
    for (int i = 1; i < n; i++) {
        for (int target = 0; target <= k; target++) {
            dp[i][target] = dp[i - 1][target] + (arr[i] <= target ? dp[i - 1][target - arr[i]] : 0);
        }
    }
    return dp[n - 1][k];
}

// 4. Target Sum: count ways to assign +/- signs to elements to sum up to target
int target_sum(const vector<int>& arr, int target) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    if (total_sum - target < 0 || (total_sum + target) % 2 != 0) return 0;
    return count_subsets_tab(arr, (total_sum + target) / 2);
}

// 5. 0/1 Knapsack: maximize value under weight limit W
int knapsack_memo_helper(int i, int W, const vector<int>& wt, const vector<int>& val, vector<vector<int>>& dp) {
    if (i == 0) return (wt[0] <= W) ? val[0] : 0;
    if (dp[i][W] != -1) return dp[i][W];
    int notTake = knapsack_memo_helper(i - 1, W, wt, val, dp), take = -1e9;
    if (wt[i] <= W) take = val[i] + knapsack_memo_helper(i - 1, W - wt[i], wt, val, dp);
    return dp[i][W] = max(take, notTake);
}

int knapsack_tab(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size(); vector<vector<int>> dp(n, vector<int>(max_W + 1, 0));
    for (int w = wt[0]; w <= max_W; w++) dp[0][w] = val[0];
    for (int i = 1; i < n; i++) {
        for (int w = 0; w <= max_W; w++) {
            dp[i][w] = max(dp[i - 1][w], (wt[i] <= w ? val[i] + dp[i - 1][w - wt[i]] : -(int)1e9));
        }
    }
    return dp[n - 1][max_W];
}

// 6. Coin Change I (Minimum Coins): infinite coin supply, find min coins to make amount
int min_coins_memo_helper(int i, int amount, const vector<int>& coins, vector<vector<int>>& dp) {
    if (i == 0) return (amount % coins[0] == 0) ? amount / coins[0] : 1e9;
    if (dp[i][amount] != -1) return dp[i][amount];
    int notTake = min_coins_memo_helper(i - 1, amount, coins, dp), take = 1e9;
    if (coins[i] <= amount) take = 1 + min_coins_memo_helper(i, amount - coins[i], coins, dp); // Unbounded: recurse on index i
    return dp[i][amount] = min(take, notTake);
}

int min_coins_tab(const vector<int>& coins, int amount) {
    int n = coins.size(); vector<vector<int>> dp(n, vector<int>(amount + 1, 0));
    for (int a = 0; a <= amount; a++) dp[0][a] = (a % coins[0] == 0) ? a / coins[0] : 1e9;
    for (int i = 1; i < n; i++) {
        for (int a = 0; a <= amount; a++) {
            dp[i][a] = min(dp[i - 1][a], (coins[i] <= a ? 1 + dp[i][a - coins[i]] : (int)1e9));
        }
    }
    return dp[n - 1][amount] >= 1e9 ? -1 : dp[n - 1][amount];
}

// 7. Coin Change II (Total Ways): infinite coin supply, total ways to make amount
int ways_to_make_sum_memo_helper(int i, int amount, const vector<int>& coins, vector<vector<int>>& dp) {
    if (i == 0) return (amount % coins[0] == 0);
    if (dp[i][amount] != -1) return dp[i][amount];
    int notTake = ways_to_make_sum_memo_helper(i - 1, amount, coins, dp), take = 0;
    if (coins[i] <= amount) take = ways_to_make_sum_memo_helper(i, amount - coins[i], coins, dp);
    return dp[i][amount] = take + notTake;
}

int ways_to_make_sum_tab(const vector<int>& coins, int amount) {
    int n = coins.size(); vector<vector<int>> dp(n, vector<int>(amount + 1, 0));
    for (int a = 0; a <= amount; a++) if (a % coins[0] == 0) dp[0][a] = 1;
    for (int i = 1; i < n; i++) {
        for (int a = 0; a <= amount; a++) {
            dp[i][a] = dp[i - 1][a] + (coins[i] <= a ? dp[i][a - coins[i]] : 0);
        }
    }
    return dp[n - 1][amount];
}

// 8. Minimum Subset Sum Difference: partition array into two subsets minimizing sum difference
int min_subset_sum_difference(const vector<int>& arr) {
    int n = arr.size(), total_sum = accumulate(arr.begin(), arr.end(), 0);
    vector<vector<bool>> dp(n, vector<bool>(total_sum + 1, false));
    for (int i = 0; i < n; i++) dp[i][0] = true;
    if (arr[0] <= total_sum) dp[0][arr[0]] = true;
    for (int i = 1; i < n; i++) {
        for (int target = 1; target <= total_sum; target++) {
            dp[i][target] = dp[i - 1][target] || (arr[i] <= target && dp[i - 1][target - arr[i]]);
        }
    }
    int min_diff = 1e9;
    for (int s1 = 0; s1 <= total_sum / 2; s1++) {
        if (dp[n - 1][s1]) min_diff = min(min_diff, total_sum - 2 * s1);
    }
    return min_diff;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
}
