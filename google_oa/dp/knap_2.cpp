#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

class Solution {
public:
    vector<int> weights;
    vector<int> values;
    vector<int> input;
    vector<vector<int>> memo;

    // =============================================================================
    // RECURSIVE DP CORE ENGINE (Top-Down Helpers)
    // =============================================================================

    int unboundedKnapsackMemoHelper(int idx, int W) {
        if (idx == weights.size()) return 0;
        if (memo[idx][W] != -1) return memo[idx][W];
        
        int notTake = unboundedKnapsackMemoHelper(idx + 1, W);
        int take = -1e9;
        if (weights[idx] <= W) {
            // Unbounded track: stay at the same index 'idx' upon selection
            take = values[idx] + unboundedKnapsackMemoHelper(idx, W - weights[idx]);
        }
        return memo[idx][W] = max(take, notTake);
    }

    int perfectSumMemoHelper(int idx, int target) {
        if (idx == input.size()) return target == 0 ? 1 : 0;
        if (memo[idx][target] != -1) return memo[idx][target];
        
        int notTake = perfectSumMemoHelper(idx + 1, target);
        int take = 0;
        if (input[idx] <= target) {
            take = perfectSumMemoHelper(idx + 1, target - input[idx]);
        }
        return memo[idx][target] = take + notTake;
    }

    // =============================================================================
    // INTERFACE ARCHITECTURE: MEMOIZATION (Top-Down)
    // =============================================================================

    int unbounded_knapsack_memo(const vector<int>& wt, const vector<int>& val, int max_W) {
        weights = wt;
        values = val;
        memo.assign(wt.size(), vector<int>(max_W + 1, -1));
        return unboundedKnapsackMemoHelper(0, max_W);
    }

    int rod_cutting_memo(int n, const vector<int>& prices) {
        vector<int> wt(n);
        for (int i = 0; i < n; ++i) wt[i] = i + 1;
        return unbounded_knapsack_memo(wt, prices, n);
    }

    int perfect_sum_memo(const vector<int>& arr, int target) {
        input = arr;
        memo.assign(arr.size(), vector<int>(target + 1, -1));
        return perfectSumMemoHelper(0, target);
    }

    // =============================================================================
    // INTERFACE ARCHITECTURE: TABULATION (Bottom-Up)
    // =============================================================================

    int unbounded_knapsack_tab(const vector<int>& wt, const vector<int>& val, int max_W) {
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

    int rod_cutting_tab(int n, const vector<int>& prices) {
        vector<int> wt(n);
        for (int i = 0; i < n; ++i) wt[i] = i + 1;
        return unbounded_knapsack_tab(wt, prices, n);
    }

    int perfect_sum_tab(const vector<int>& arr, int target) {
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
};

// =============================================================================
// VERIFICATION SANDBOX
// =============================================================================
int main() {
    Solution solver;

    // 1. Unbounded Knapsack
    vector<int> wt = {2, 4, 6};
    vector<int> val = {5, 11, 13};
    assert(solver.unbounded_knapsack_memo(wt, val, 10) == 27);
    assert(solver.unbounded_knapsack_tab(wt, val, 10) == 27);
    
    // 2. Rod Cutting
    vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20};
    assert(solver.rod_cutting_memo(8, prices) == 22);
    assert(solver.rod_cutting_tab(8, prices) == 22);
    
    // 3. Perfect Sum (Robust validation against zeroes)
    vector<int> arr = {1, 2, 0, 3};
    assert(solver.perfect_sum_memo(arr, 3) == 4);
    assert(solver.perfect_sum_tab(arr, 3) == 4);
    
    cout << "All clean Tier-2 Knapsack variants passed successfully!" << endl;
    return 0;
}