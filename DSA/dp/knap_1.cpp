/**
 * Problem: Knapsack Tier 1 Variants
 * Reference implementations for 8 core 0/1 Knapsack and Unbounded Knapsack variants.
 * Time: O(N * W) time, Space: O(N * W) auxiliary space (unoptimized 2D tables).
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

// =============================================================================
// Helper Functions for Memoization
// =============================================================================

bool subsetSumMemoHelper(int idx, int target, const vector<int>& arr, vector<vector<int>>& dp) {
    if (target == 0) return true;
    if (idx == arr.size()) return false;
    if (dp[idx][target] != -1) return dp[idx][target];
    
    bool notTake = subsetSumMemoHelper(idx + 1, target, arr, dp);
    bool take = false;
    if (arr[idx] <= target) {
        take = subsetSumMemoHelper(idx + 1, target - arr[idx], arr, dp);
    }
    return dp[idx][target] = (take || notTake);
}

int countSubsetsMemoHelper(int idx, int target, const vector<int>& arr, vector<vector<int>>& dp) {
    if (idx == arr.size()) return target == 0 ? 1 : 0;
    if (dp[idx][target] != -1) return dp[idx][target];
    
    int notTake = countSubsetsMemoHelper(idx + 1, target, arr, dp);
    int take = 0;
    if (arr[idx] <= target) {
        take = countSubsetsMemoHelper(idx + 1, target - arr[idx], arr, dp);
    }
    return dp[idx][target] = take + notTake;
}

int knapsackMemoHelper(int idx, int W, const vector<int>& wt, const vector<int>& val, vector<vector<int>>& dp) {
    if (idx == wt.size()) return 0;
    if (dp[idx][W] != -1) return dp[idx][W];
    
    int notTake = knapsackMemoHelper(idx + 1, W, wt, val, dp);
    int take = -1e9;
    if (wt[idx] <= W) {
        take = val[idx] + knapsackMemoHelper(idx + 1, W - wt[idx], wt, val, dp);
    }
    return dp[idx][W] = max(take, notTake);
}

int minCoinsMemoHelper(int idx, int amount, const vector<int>& coins, vector<vector<int>>& dp) {
    if (idx == coins.size()) return amount == 0 ? 0 : 1e9;
    if (dp[idx][amount] != -1) return dp[idx][amount];
    
    int notTake = minCoinsMemoHelper(idx + 1, amount, coins, dp);
    int take = 1e9;
    if (coins[idx] <= amount) {
        take = 1 + minCoinsMemoHelper(idx, amount - coins[idx], coins, dp); // Unbounded Knapsack
    }
    return dp[idx][amount] = min(take, notTake);
}

int waysToMakeSumMemoHelper(int idx, int amount, const vector<int>& coins, vector<vector<int>>& dp) {
    if (idx == coins.size()) return amount == 0 ? 1 : 0;
    if (dp[idx][amount] != -1) return dp[idx][amount];
    
    int notTake = waysToMakeSumMemoHelper(idx + 1, amount, coins, dp);
    int take = 0;
    if (coins[idx] <= amount) {
        take = waysToMakeSumMemoHelper(idx, amount - coins[idx], coins, dp); // Unbounded Knapsack
    }
    return dp[idx][amount] = take + notTake;
}

// =============================================================================
// 1. Subset Sum: checks if any subset sums to target K
// =============================================================================

bool subset_sum_memo(const vector<int>& arr, int k) {
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(k + 1, -1));
    return subsetSumMemoHelper(0, k, arr, dp);
}

bool subset_sum_tab(const vector<int>& arr, int k) {
    int n = arr.size();
    vector<vector<bool>> dp(n + 1, vector<bool>(k + 1, false));
    for (int i = 0; i <= n; i++) dp[i][0] = true;
    
    for (int i = 1; i <= n; i++) {
        for (int target = 1; target <= k; target++) {
            dp[i][target] = dp[i - 1][target];
            if (arr[i - 1] <= target) {
                dp[i][target] = dp[i][target] || dp[i - 1][target - arr[i - 1]];
            }
        }
    }
    return dp[n][k];
}

// =============================================================================
// 2. Partition Equal Subset Sum
// =============================================================================

bool can_partition_memo(const vector<int>& arr) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    if (total_sum % 2 != 0) return false;
    return subset_sum_memo(arr, total_sum / 2);
}

bool can_partition_tab(const vector<int>& arr) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    if (total_sum % 2 != 0) return false;
    return subset_sum_tab(arr, total_sum / 2);
}

// =============================================================================
// 3. Count Subsets with Sum K
// =============================================================================

int count_subsets_memo(const vector<int>& arr, int k) {
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(k + 1, -1));
    return countSubsetsMemoHelper(0, k, arr, dp);
}

int count_subsets_tab(const vector<int>& arr, int k) {
    int n = arr.size();
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    dp[0][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        for (int target = 0; target <= k; target++) {
            dp[i][target] = dp[i - 1][target];
            if (arr[i - 1] <= target) {
                dp[i][target] += dp[i - 1][target - arr[i - 1]];
            }
        }
    }
    return dp[n][k];
}

// =============================================================================
// 4. Target Sum: count ways to assign +/- signs to elements to sum up to target
// =============================================================================

int target_sum_memo(const vector<int>& arr, int target) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    if (total_sum - target < 0 || (total_sum + target) % 2 != 0) return 0;
    return count_subsets_memo(arr, (total_sum + target) / 2);
}

int target_sum_tab(const vector<int>& arr, int target) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    if (total_sum - target < 0 || (total_sum + target) % 2 != 0) return 0;
    return count_subsets_tab(arr, (total_sum + target) / 2);
}

// =============================================================================
// 5. 0/1 Knapsack: maximize value under weight limit W
// =============================================================================

int knapsack_memo(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size();
    vector<vector<int>> dp(n, vector<int>(max_W + 1, -1));
    return knapsackMemoHelper(0, max_W, wt, val, dp);
}

int knapsack_tab(const vector<int>& wt, const vector<int>& val, int max_W) {
    int n = wt.size();
    vector<vector<int>> dp(n + 1, vector<int>(max_W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= max_W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (wt[i - 1] <= w) {
                dp[i][w] = max(dp[i][w], val[i - 1] + dp[i - 1][w - wt[i - 1]]);
            }
        }
    }
    return dp[n][max_W];
}

// =============================================================================
// 6. Coin Change I (Minimum Coins)
// =============================================================================

int min_coins_memo(const vector<int>& coins, int amount) {
    int n = coins.size();
    vector<vector<int>> dp(n, vector<int>(amount + 1, -1));
    int ans = minCoinsMemoHelper(0, amount, coins, dp);
    return ans >= 1e9 ? -1 : ans;
}

int min_coins_tab(const vector<int>& coins, int amount) {
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 1e9));
    dp[0][0] = 0;
    for (int i = 0; i <= n; i++) dp[i][0] = 0;
    
    for (int i = 1; i <= n; i++) {
        for (int a = 0; a <= amount; a++) {
            dp[i][a] = dp[i - 1][a];
            if (coins[i - 1] <= a) {
                dp[i][a] = min(dp[i][a], 1 + dp[i][a - coins[i - 1]]);
            }
        }
    }
    return dp[n][amount] >= 1e9 ? -1 : dp[n][amount];
}

// =============================================================================
// 7. Coin Change II (Total Ways)
// =============================================================================

int ways_to_make_sum_memo(const vector<int>& coins, int amount) {
    int n = coins.size();
    vector<vector<int>> dp(n, vector<int>(amount + 1, -1));
    return waysToMakeSumMemoHelper(0, amount, coins, dp);
}

int ways_to_make_sum_tab(const vector<int>& coins, int amount) {
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));
    dp[0][0] = 1;
    for (int i = 0; i <= n; i++) dp[i][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        for (int a = 0; a <= amount; a++) {
            dp[i][a] = dp[i - 1][a];
            if (coins[i - 1] <= a) {
                dp[i][a] += dp[i][a - coins[i - 1]];
            }
        }
    }
    return dp[n][amount];
}

// =============================================================================
// 8. Minimum Subset Sum Difference
// =============================================================================

int min_subset_sum_difference_memo(const vector<int>& arr) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(total_sum / 2 + 1, -1));
    
    int maxPossibleSum = 0;
    for (int s = total_sum / 2; s >= 0; s--) {
        if (subsetSumMemoHelper(0, s, arr, dp)) {
            maxPossibleSum = s;
            break;
        }
    }
    return total_sum - 2 * maxPossibleSum;
}

int min_subset_sum_difference_tab(const vector<int>& arr) {
    int total_sum = accumulate(arr.begin(), arr.end(), 0);
    int n = arr.size();
    int k = total_sum / 2;
    vector<vector<bool>> dp(n + 1, vector<bool>(k + 1, false));
    for (int i = 0; i <= n; i++) dp[i][0] = true;
    
    for (int i = 1; i <= n; i++) {
        for (int target = 1; target <= k; target++) {
            dp[i][target] = dp[i - 1][target];
            if (arr[i - 1] <= target) {
                dp[i][target] = dp[i][target] || dp[i - 1][target - arr[i - 1]];
            }
        }
    }
    
    int maxPossibleSum = 0;
    for (int s = k; s >= 0; s--) {
        if (dp[n][s]) {
            maxPossibleSum = s;
            break;
        }
    }
    return total_sum - 2 * maxPossibleSum;
}

// =============================================================================
// Main function for simple local verification
// =============================================================================
int main() {
    // 1. Subset Sum
    vector<int> arr = {3, 34, 4, 12, 5, 2};
    assert(subset_sum_memo(arr, 9) == true);
    assert(subset_sum_tab(arr, 9) == true);
    
    // 2. Partition
    assert(can_partition_memo(arr) == false);
    assert(can_partition_tab(arr) == false);
    
    // 3. Count Subsets
    assert(count_subsets_memo(arr, 9) == 2); // {3, 4, 2} and {4, 5}
    assert(count_subsets_tab(arr, 9) == 2);
    
    // 4. Target Sum
    vector<int> arrTS = {1, 1, 1, 1, 1};
    assert(target_sum_memo(arrTS, 3) == 5);
    assert(target_sum_tab(arrTS, 3) == 5);
    
    // 5. 0/1 Knapsack
    vector<int> wt = {1, 2, 4, 5};
    vector<int> val = {5, 4, 8, 6};
    assert(knapsack_memo(wt, val, 5) == 13); // {1, 4} -> wt = 5, val = 13
    assert(knapsack_tab(wt, val, 5) == 13);
    
    // 6. Coin Change I
    vector<int> coins = {1, 2, 5};
    assert(min_coins_memo(coins, 11) == 3); // 5 + 5 + 1
    assert(min_coins_tab(coins, 11) == 3);
    
    // 7. Coin Change II
    assert(ways_to_make_sum_memo(coins, 5) == 4); // 5, 2+2+1, 2+1+1+1, 1+1+1+1+1
    assert(ways_to_make_sum_tab(coins, 5) == 4);
    
    // 8. Min Difference
    vector<int> arrDiff = {1, 6, 11, 5};
    assert(min_subset_sum_difference_memo(arrDiff) == 1);
    assert(min_subset_sum_difference_tab(arrDiff) == 1);
    
    cout << "All knap_1 tests passed successfully!" << endl;
    return 0;
}
