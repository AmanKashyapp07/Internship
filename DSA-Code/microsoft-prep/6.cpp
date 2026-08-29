#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <climits>
#endif
using namespace std;

// =========================================================
// 78. BEST TIME TO BUY AND SELL STOCK
// =========================================================

int maxProfit(vector<int>& prices) {
    int minPrice = INT_MAX, maxProf = 0;
    for (int p : prices) {
        minPrice = min(minPrice, p);
        maxProf = max(maxProf, p - minPrice);
    }
    return maxProf;
}
// Interview Explanation:
// - Problem Statement: Find max profit from buying and selling stock on a single day.
// - Approach: Single-pass Running Minimum Price Tracking.
// - Intuition: Best selling day for today is current price minus minimum buying price seen so far.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 79. MAXIMUM PRODUCT SUBARRAY
// =========================================================

int maxProduct(vector<int>& nums) {
    if (nums.empty()) return 0;
    int globalMax = nums[0], currMax = nums[0], currMin = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        int x = nums[i];
        if (x < 0) swap(currMax, currMin);
        currMax = max(x, currMax * x);
        currMin = min(x, currMin * x);
        globalMax = max(globalMax, currMax);
    }
    return globalMax;
}
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with largest product.
// - Approach: Dual Min/Max Tracking Dynamic Programming.
// - Intuition: A negative number turns the minimum product into a potential maximum product; swap on negatives.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 80. CLIMBING STAIRS
// =========================================================

int climbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
// Interview Explanation:
// - Problem Statement: Count distinct ways to climb n stairs taking 1 or 2 steps.
// - Approach: 1D Fibonacci Dynamic Programming.
// - Intuition: ways(n) = ways(n-1) + ways(n-2).
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 81. HOUSE ROBBER
// =========================================================

int rob(vector<int>& nums) {
    int robPrev = 0, notRobPrev = 0;
    for (int x : nums) {
        int newRob = notRobPrev + x;
        int newNotRob = max(robPrev, notRobPrev);
        robPrev = newRob;
        notRobPrev = newNotRob;
    }
    return max(robPrev, notRobPrev);
}
// Interview Explanation:
// - Problem Statement: Max money robbed without robbing adjacent houses.
// - Approach: 1D Linear State DP max(rob, skip).
// - Intuition: dp[i] = max(dp[i-1], dp[i-2] + nums[i]).
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 82. COIN CHANGE
// =========================================================

int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (i - coin >= 0) {
                dp[i] = min(dp[i], 1 + dp[i - coin]);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}
// Interview Explanation:
// - Problem Statement: Minimum coins needed to make up given amount.
// - Approach: 1D DP (Unbounded Knapsack / Minimum Steps).
// - Intuition: dp[i] = min(1 + dp[i - coin]) across all valid coin choices.
// - Complexity: Time: O(Amount * Coins), Space: O(Amount).


// =========================================================
// 83. UNIQUE PATHS
// =========================================================

int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, 1));
    for (int r = 1; r < m; ++r) {
        for (int c = 1; c < n; ++c) {
            dp[r][c] = dp[r - 1][c] + dp[r][c - 1];
        }
    }
    return dp[m - 1][n - 1];
}
// Interview Explanation:
// - Problem Statement: Total unique paths from top-left to bottom-right moving only down/right.
// - Approach: 2D Grid Dynamic Programming.
// - Intuition: dp[r][c] = dp[r-1][c] + dp[r][c-1].
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 84. MINIMUM PATH SUM
// =========================================================

int minPathSum(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> dp(m, vector<int>(n, 0));
    dp[0][0] = grid[0][0];

    for (int c = 1; c < n; ++c) dp[0][c] = dp[0][c - 1] + grid[0][c];
    for (int r = 1; r < m; ++r) dp[r][0] = dp[r - 1][0] + grid[r][0];

    for (int r = 1; r < m; ++r) {
        for (int c = 1; c < n; ++c) {
            dp[r][c] = grid[r][c] + min(dp[r - 1][c], dp[r][c - 1]);
        }
    }
    return dp[m - 1][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find path from top-left to bottom-right minimizing sum of numbers.
// - Approach: 2D Grid Path Accumulation DP.
// - Intuition: dp[r][c] = grid[r][c] + min(dp[r-1][c], dp[r][c-1]).
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 85. LONGEST INCREASING SUBSEQUENCE (LIS)
// =========================================================

int lengthOfLIS(vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
// Interview Explanation:
// - Problem Statement: Find length of longest strictly increasing subsequence.
// - Approach: Patience Sorting with Binary Search (std::lower_bound).
// - Intuition: tails[i] stores smallest tail among all increasing subsequences of length i+1.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 86. LONGEST COMMON SUBSEQUENCE (LCS)
// =========================================================

int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}
// Interview Explanation:
// - Problem Statement: Find length of longest common subsequence between two strings.
// - Approach: 2D String Matching Dynamic Programming.
// - Intuition: If chars match, dp[i][j] = 1 + dp[i-1][j-1]; else max(dp[i-1][j], dp[i][j-1]).
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 87. WORD BREAK
// =========================================================

bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }
    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Check if string can be segmented into words from dictionary.
// - Approach: 1D DP String Prefix Partitioning.
// - Intuition: dp[i] is true if prefix s[0..j-1] is valid and remaining substring s[j..i-1] exists in dictionary.
// - Complexity: Time: O(N^2 * L), Space: O(N).


// =========================================================
// 88. 0/1 KNAPSACK PROBLEM
// =========================================================

int knapsack01(int W, const vector<int>& val, const vector<int>& wt) {
    int n = val.size();
    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; ++i) {
        for (int w = W; w >= wt[i]; --w) {
            dp[w] = max(dp[w], val[i] + dp[w - wt[i]]);
        }
    }
    return dp[W];
}
// Interview Explanation:
// - Problem Statement: Maximize total value in knapsack of capacity W with 0/1 item choices.
// - Approach: 1D Space-Optimized Dynamic Programming.
// - Intuition: Backward iteration prevents using the same item multiple times.
// - Complexity: Time: O(N * W), Space: O(W).


// =========================================================
// 89. DECODE WAYS
// =========================================================

int numDecodings(string s) {
    if (s.empty() || s[0] == '0') return 0;
    int n = s.size();
    vector<int> dp(n + 1, 0);
    dp[0] = 1;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        int oneDigit = s[i - 1] - '0';
        if (oneDigit >= 1 && oneDigit <= 9) dp[i] += dp[i - 1];

        int twoDigits = stoi(s.substr(i - 2, 2));
        if (twoDigits >= 10 && twoDigits <= 26) dp[i] += dp[i - 2];
    }
    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Total number of ways to decode numeric string to letters ('A'..'Z').
// - Approach: 1D Linear Transition DP (Single and Double Digit States).
// - Intuition: dp[i] transitions from dp[i-1] (valid single digit) and dp[i-2] (valid 10-26 double digit).
// - Complexity: Time: O(N), Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 78 | Best Time to Buy and Sell Stock             | Running Min & Max Profit Sweep    | O(N)     | O(1)     |
 | 79 | Maximum Product Subarray                    | Dual Min/Max Tracking DP          | O(N)     | O(1)     |
 | 80 | Climbing Stairs                             | 1D Fibonacci Dynamic Programming  | O(N)     | O(1)     |
 | 81 | House Robber                                | 1D Linear State DP max(rob, skip) | O(N)     | O(1)     |
 | 82 | Coin Change                                 | 1D DP (Unbounded Knapsack)        | O(Amount)| O(Amount)|
 | 83 | Unique Paths                                | 2D Grid Dynamic Programming       | O(M * N) | O(M * N) |
 | 84 | Minimum Path Sum                            | 2D Grid Path Accumulation DP      | O(M * N) | O(M * N) |
 | 85 | Longest Increasing Subsequence (LIS)        | Patience Sorting Binary Search    | O(N logN)| O(N)     |
 | 86 | Longest Common Subsequence (LCS)            | 2D String Matching DP Matrix      | O(M * N) | O(M * N) |
 | 87 | Word Break                                  | 1D DP String Prefix Partitioning  | O(N^2 *L)| O(N)     |
 | 88 | 0/1 Knapsack Problem                        | 1D Space-Optimized DP             | O(N * W) | O(W)     |
 | 89 | Decode Ways                                 | 1D Linear Transition DP           | O(N)     | O(N)     |
 ====================================================================================================
*/
