#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class Solution {
public:
    // Subset Sum
    bool subset_sum_tab(vector<int> arr, int k) {
        int n = arr.size();
        vector<vector<bool>> dp(n + 1, vector<bool>(k + 1, false));
        dp[0][0] = true;

        for (int i = 1; i <= n; ++i)
            for (int t = 0; t <= k; ++t) {
                dp[i][t] = dp[i - 1][t];
                if (arr[i - 1] <= t)
                    dp[i][t] = dp[i][t] || dp[i - 1][t - arr[i - 1]];
            }
        return dp[n][k];
    }

    // Count subsets with given sum
    int count_subsets_tab(vector<int>& arr, int k) {
        int n = arr.size();
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i <= n; ++i)
            for (int t = 0; t <= k; ++t) {
                dp[i][t] = dp[i - 1][t];
                if (arr[i - 1] <= t)
                    dp[i][t] += dp[i - 1][t - arr[i - 1]];
            }
        return dp[n][k];
    }

    // 0/1 Knapsack
    int knapsack_tab(vector<int> wt, vector<int> val, int W) {
        int n = wt.size();
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

        for (int i = 1; i <= n; ++i)
            for (int w = 0; w <= W; ++w) {
                dp[i][w] = dp[i - 1][w];
                if (wt[i - 1] <= w)
                    dp[i][w] = max(dp[i][w], val[i - 1] + dp[i - 1][w - wt[i - 1]]);
            }
        return dp[n][W];
    }

    // Minimum Coins (Unbounded Knapsack)
    int min_coins_tab(vector<int> coins, int amount) {
        int n = coins.size();
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 1000000000));
        dp[0][0] = 0;

        for (int i = 1; i <= n; ++i) {
            dp[i][0] = 0;
            for (int a = 0; a <= amount; ++a) {
                dp[i][a] = dp[i - 1][a];
                if (coins[i - 1] <= a)
                    dp[i][a] = min(dp[i][a], 1 + dp[i][a - coins[i - 1]]);
            }
        }
        return dp[n][amount] >= 1000000000 ? -1 : dp[n][amount];
    }

    // Coin Change - Number of Ways (Unlimited)
    int ways_to_make_sum_tab(vector<int> coins, int amount) {
        int n = coins.size();
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i <= n; ++i)
            for (int a = 0; a <= amount; ++a) {
                dp[i][a] = dp[i - 1][a];
                if (coins[i - 1] <= a)
                    dp[i][a] += dp[i][a - coins[i - 1]];
            }
        return dp[n][amount];
    }

    // Minimum Subset Sum Difference
    int min_subset_sum_difference_tab(vector<int> arr) {
        int total = accumulate(arr.begin(), arr.end(), 0);
        int n = arr.size();
        int half = total / 2;

        vector<vector<bool>> dp(n + 1, vector<bool>(half + 1, false));
        dp[0][0] = true;

        for (int i = 1; i <= n; ++i)
            for (int t = 0; t <= half; ++t) {
                dp[i][t] = dp[i - 1][t];
                if (arr[i - 1] <= t)
                    dp[i][t] = dp[i][t] || dp[i - 1][t - arr[i - 1]];
            }

        int best = 0;
        for (int s = half; s >= 0; --s)
            if (dp[n][s]) {
                best = s;
                break;
            }
        return total - 2 * best;
    }
};

int main() {
    return 0;
}