#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

class Solution {
public:
    vector<int> input;
    vector<int> weights;
    vector<int> values;
    vector<int> coinValues;
    vector<vector<int>> memo;

    // =============================================================================
    // RECURSIVE DP CORE ENGINE (Top-Down Helpers)
    // =============================================================================

    bool subsetSumMemoHelper(int idx, int target) {
        if (target == 0) return true;
        if (idx == input.size()) return false;
        if (memo[idx][target] != -1) return memo[idx][target];
        
        bool notTake = subsetSumMemoHelper(idx + 1, target);
        bool take = false;
        if (input[idx] <= target) {
            take = subsetSumMemoHelper(idx + 1, target - input[idx]);
        }
        return memo[idx][target] = (take || notTake);
    }

    int countSubsetsMemoHelper(int idx, int target) {
        if (idx == input.size()) return target == 0 ? 1 : 0;
        if (memo[idx][target] != -1) return memo[idx][target];
        
        int notTake = countSubsetsMemoHelper(idx + 1, target);
        int take = 0;
        if (input[idx] <= target) {
            take = countSubsetsMemoHelper(idx + 1, target - input[idx]);
        }
        return memo[idx][target] = take + notTake;
    }

    int knapsackMemoHelper(int idx, int W) {
        if (idx == weights.size()) return 0;
        if (memo[idx][W] != -1) return memo[idx][W];
        
        int notTake = knapsackMemoHelper(idx + 1, W);
        int take = -1e9;
        if (weights[idx] <= W) {
            take = values[idx] + knapsackMemoHelper(idx + 1, W - weights[idx]);
        }
        return memo[idx][W] = max(take, notTake);
    }

    int minCoinsMemoHelper(int idx, int amount) {
        if (idx == coinValues.size()) return amount == 0 ? 0 : 1e9;
        if (memo[idx][amount] != -1) return memo[idx][amount];
        
        int notTake = minCoinsMemoHelper(idx + 1, amount);
        int take = 1e9;
        if (coinValues[idx] <= amount) {
            take = 1 + minCoinsMemoHelper(idx, amount - coinValues[idx]);
        }
        return memo[idx][amount] = min(take, notTake);
    }

    int waysToMakeSumMemoHelper(int idx, int amount) {
        if (idx == coinValues.size()) return amount == 0 ? 1 : 0;
        if (memo[idx][amount] != -1) return memo[idx][amount];
        
        int notTake = waysToMakeSumMemoHelper(idx + 1, amount);
        int take = 0;
        if (coinValues[idx] <= amount) {
            take = waysToMakeSumMemoHelper(idx, amount - coinValues[idx]);
        }
        return memo[idx][amount] = take + notTake;
    }

    // =============================================================================
    // INTERFACE ARCHITECTURE: MEMOIZATION (Top-Down)
    // =============================================================================

    bool subset_sum_memo(const vector<int>& arr, int k) {
        input = arr;
        memo.assign(arr.size(), vector<int>(k + 1, -1));
        return subsetSumMemoHelper(0, k);
    }

    bool can_partition_memo(const vector<int>& arr) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        if (totalSum % 2 != 0) return false;
        return subset_sum_memo(arr, totalSum / 2);
    }

    int count_subsets_memo(const vector<int>& arr, int k) {
        input = arr;
        memo.assign(arr.size(), vector<int>(k + 1, -1));
        return countSubsetsMemoHelper(0, k);
    }

    int target_sum_memo(const vector<int>& arr, int target) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        if (totalSum - target < 0 || (totalSum + target) % 2 != 0) return 0;
        return count_subsets_memo(arr, (totalSum + target) / 2);
    }

    int knapsack_memo(const vector<int>& wt, const vector<int>& val, int max_W) {
        weights = wt;
        values = val;
        memo.assign(wt.size(), vector<int>(max_W + 1, -1));
        return knapsackMemoHelper(0, max_W);
    }

    int min_coins_memo(const vector<int>& coins, int amount) {
        coinValues = coins;
        memo.assign(coins.size(), vector<int>(amount + 1, -1));
        int ans = minCoinsMemoHelper(0, amount);
        return ans >= 1e9 ? -1 : ans;
    }

    int ways_to_make_sum_memo(const vector<int>& coins, int amount) {
        coinValues = coins;
        memo.assign(coins.size(), vector<int>(amount + 1, -1));
        return waysToMakeSumMemoHelper(0, amount);
    }

    int min_subset_sum_difference_memo(const vector<int>& arr) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        int n = arr.size();
        int k = totalSum / 2;
        input = arr;
        memo.assign(n, vector<int>(k + 1, -1));
        
        subsetSumMemoHelper(0, k);
        
        int maxPossibleSum = 0;
        for (int s = k; s >= 0; s--) {
            if (subsetSumMemoHelper(0, s)) {
                maxPossibleSum = s;
                break;
            }
        }
        return totalSum - 2 * maxPossibleSum;
    }

    // =============================================================================
    // INTERFACE ARCHITECTURE: TABULATION (Bottom-Up)
    // =============================================================================

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

    bool can_partition_tab(const vector<int>& arr) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        if (totalSum % 2 != 0) return false;
        return subset_sum_tab(arr, totalSum / 2);
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

    int target_sum_tab(const vector<int>& arr, int target) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        if (totalSum - target < 0 || (totalSum + target) % 2 != 0) return 0;
        return count_subsets_tab(arr, (totalSum + target) / 2);
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

    int min_coins_tab(const vector<int>& coins, int amount) {
        int n = coins.size();
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 1e9));
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

    int ways_to_make_sum_tab(const vector<int>& coins, int amount) {
        int n = coins.size();
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));
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

    int min_subset_sum_difference_tab(const vector<int>& arr) {
        int totalSum = accumulate(arr.begin(), arr.end(), 0);
        int n = arr.size();
        int k = totalSum / 2;
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
        return totalSum - 2 * maxPossibleSum;
    }
};

// =============================================================================
// VERIFICATION SANDBOX
// =============================================================================
int main() {
    Solution solver;

    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};
    int max_W = 7;
    assert(solver.knapsack_memo(weights, values, max_W) == 9);
    assert(solver.knapsack_tab(weights, values, max_W) == 9);

    vector<int> coins = {1, 2, 3};
    int amount = 5;
    assert(solver.min_coins_memo(coins, amount) == 2);
    assert(solver.min_coins_tab(coins, amount) == 2);
    assert(solver.ways_to_make_sum_memo(coins, amount) == 5);
    assert(solver.ways_to_make_sum_tab(coins, amount) == 5);

    vector<int> arr = {1, 6, 11, 5};
    assert(solver.min_subset_sum_difference_memo(arr) == 1);
    assert(solver.min_subset_sum_difference_tab(arr) == 1);

    cout << "All Tier-1 Knapsack and Unbounded variants compiled and verified successfully!" << endl;
    return 0;
}