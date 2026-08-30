#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Climbing Stairs (LC 70)                     | Fibonacci 1D Space DP             | O(N)     | O(1)     |
 | 2  | Min Cost Climbing Stairs (LC 746)           | Rolling 2-State Minimum DP        | O(N)     | O(1)     |
 | 3  | House Robber (LC 198)                       | Rob / Skip Alternating DP         | O(N)     | O(1)     |
 | 4  | House Robber II - Circular (LC 213)         | Dual Pass: [0..N-2] & [1..N-1]    | O(N)     | O(1)     |
 | 5  | Decode Ways (LC 91)                         | 1-Digit / 2-Digit Branching DP    | O(N)     | O(1)     |
 | 6  | Delete and Earn (LC 740)                    | Frequency Transform + House Robber| O(N + Max| O(MaxVal)|
 | 7  | Partition Equal Subset Sum (LC 416)         | 1D 0/1 Knapsack (Backwards Loop)  | O(N*Sum) | O(Sum)   |
 | 8  | Target Sum (LC 494)                         | Subset Sum Partition Reduction    | O(N*Sum) | O(Sum)   |
 | 9  | Coin Change (LC 322)                        | 1D Unbounded Knapsack (Min Coins) | O(N*Amt) | O(Amt)   |
 | 10 | Coin Change II (LC 518)                     | 1D Unbounded (Combinations Outer) | O(N*Amt) | O(Amt)   |
 | 11 | Combination Sum IV (LC 377)                 | 1D Unbounded (Permutations Outer) | O(N*Amt) | O(Amt)   |
 | 12 | Longest Common Subsequence (LC 1143)        | 2D DP Table / Rolling Row DP      | O(N * M) | O(min M) |
 | 13 | Edit Distance (LC 72)                       | 2D Levenshtein Distance Matrix    | O(N * M) | O(N * M) |
 | 14 | Distinct Subsequences (LC 115)              | 1D Space-Optimized DP (Backwards) | O(N * M) | O(M)     |
 | 15 | Word Break (LC 139)                         | 1D Prefix DP + Trie/Set Lookup    | O(N^2 * L)| O(N + D) |
 | 16 | Interleaving String (LC 97)                 | 2D Grid DP Match Propagation      | O(N * M) | O(M)     |
 | 17 | Longest Palindromic Subsequence (LC 516)    | 2D Interval DP / LCS with Reverse | O(N^2)   | O(N)     |
 | 18 | Longest Increasing Subsequence (LC 300)     | Patience Sorting (lower_bound)    | O(N logN)| O(N)     |
 | 19 | Longest String Chain (LC 1048)              | Length-Sorted DP + Predecessor Map| O(N*L^2) | O(N)     |
 | 20 | Largest Divisible Subset (LC 368)           | Sorted 1D LIS Variant + Parent DP | O(N^2)   | O(N)     |
 | 21 | Job Scheduling Max Profit (LC 1235)         | End-Time Sort + DP + Binary Search| O(N logN)| O(N)     |
 | 22 | Unique Paths (LC 62)                        | Combinatorics / 1D Grid DP Roll   | O(M * N) | O(N)     |
 | 23 | Unique Paths II - Obstacles (LC 63)         | 1D Space-Optimized Grid DP        | O(M * N) | O(N)     |
 | 24 | Minimum Path Sum (LC 64)                    | 1D Grid DP In-Place Accumulation  | O(M * N) | O(N)     |
 | 25 | Maximal Square (LC 221)                     | 2D DP: min(up, left, diag) + 1    | O(M * N) | O(N)     |
 | 26 | Maximal Rectangle in Binary Grid (LC 85)    | Row Heights Histogram + MonoStack | O(M * N) | O(N)     |
 | 27 | Burst Balloons (LC 312)                     | Interval DP (Last Balloon Picked) | O(N^3)   | O(N^2)   |
 | 28 | Minimum Cost to Cut a Stick (LC 1547)       | Interval DP on Sorted Cut Points  | O(C^3)   | O(C^2)   |
 | 29 | Palindrome Partitioning II (LC 132)         | Precomputed Palindrome + 1D Min Cut| O(N^2)  | O(N^2)   |
 | 30 | Binary Tree Maximum Path Sum (LC 124)       | Post-Order DFS Gain Calculation   | O(N)     | O(H)     |
 | 31 | House Robber III - Tree (LC 337)            | Post-Order DFS pair{rob, notRob}  | O(N)     | O(H)     |
 | 32 | Best Time to Buy Stock Cooldown (LC 309)    | 3-State FSM (hold, sold, rest)    | O(N)     | O(1)     |
 | 33 | Stock with Transaction Fee (LC 714)         | 2-State FSM (hold, cash)          | O(N)     | O(1)     |
 | 34 | Stock IV - At Most K Trans (LC 188)         | 2K-State FSM / Min-Cost DP        | O(N * K) | O(K)     |
 | 35 | Count Digit One (LC 233)                    | Positional Power-of-10 Math       | O(log10 N| O(1)     |
 | 36 | Smallest Sufficient Team (LC 1125)          | Bitmask DP (Subset Skill Cover)   | O(M 2^N) | O(2^N)   |
 | 37 | Removal Game / Stone Game I (CSES / LC 877) | 2D Interval Minimax DP            | O(N^2)   | O(N)     |
 | 38 | Predict the Winner (LC 486)                 | 1D Interval Minimax Net Score DP  | O(N^2)   | O(N)     |
 | 39 | Can I Win (LC 464)                          | Bitmask Memoization Minimax       | O(2^N)   | O(2^N)   |
 | 40 | Guess Number Higher or Lower II (LC 375)    | Interval Minimax Cost DP          | O(N^3)   | O(N^2)   |
 | 41 | Stone Game II (LC 1140)                     | Memoized Suffix Minimax (i, M)    | O(N^3)   | O(N^2)   |
 | 42 | Stone Game III (LC 1406)                    | 1D Suffix Minimax 3-Step Choice   | O(N)     | O(1)     |
 | 43 | Divisor Game & Nim Game (LC 1025 / 292)     | Mathematical Parity Invariant     | O(1)     | O(1)     |
 | 44 | Super Egg Drop (LC 887)                     | Inverse DP: moves & eggs          | O(K logN)| O(K)     |
 | 45 | Decode Ways II with Wildcards (LC 639)      | 1D DP Multi-Case Transition Mod   | O(N)     | O(1)     |
 | 46 | Minimum Cost For Tickets (LC 983)           | 1D Travel Day Reachability DP     | O(365)   | O(365)   |
 | 47 | Dungeon Game (LC 174)                       | Bottom-Up Reverse Knight Health DP| O(M * N) | O(M * N) |
 | 48 | Cherry Pickup (LC 741)                      | Synchronous 2-Agent Manhattan DP  | O(N^3)   | O(N^2)   |
 | 49 | Maximum Product Subarray (LC 152)           | 2-State Max/Min Running DP        | O(N)     | O(1)     |
 | 50 | Ninja's Training (GeeksforGeeks)            | 2D Activity Transitions O(1) Space| O(N)     | O(1)     |
 | 51 | Removing Digits (CSES 1637)                 | Top-Down Memoized Digit DP        | O(N logN)| O(N)     |
 | 52 | Array Description (CSES 1746)               | 2D Value Continuity DP            | O(N * M) | O(N * M) |
 | 53 | Counting Numbers - No Adj Equal (CSES 2220) | Digit DP with Prev Digit & Tight  | O(log B) | O(1)     |
 | 54 | Rectangle Cutting (CSES 1744)               | 2D Rectangle Interval DP          | O(A*B(A+B))| O(A * B)|
 | 55 | Elevator Rides (CSES 1653)                  | Bitmask DP (rides, last_weight)   | O(2^N * N)| O(2^N)   |
 | 56 | Increasing Subsequence II (CSES 1748)       | Fenwick Tree Prefix DP            | O(N logN)| O(N)     |
 | 57 | Minimal Grid Path (CSES 3359)               | Level BFS Greedy Frontier Step    | O(N^2)   | O(N^2)   |
 ====================================================================================================
*/




struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

class Solution {
public:

    // =========================================================
    // 1. BASIC 1D DP
    // =========================================================

    // Climbing Stairs (LeetCode 70)
    int climbStairs(int n) {
        if (n <= 2) return n;
        vector<int> dp(n + 1, 0);
        dp[1] = 1; dp[2] = 2;
        for (int i = 3; i <= n; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Find total distinct ways to climb n steps taking 1 or 2 steps at a time (LeetCode 70).
    // - Approach: 1D DP tabulation storing total ways to reach step i.
    // - Intuition: Reaching step i requires stepping from step i-1 or step i-2; `dp[i] = dp[i-1] + dp[i-2]`.
    // - Complexity: Time: O(N), Space: O(N).

    // Min Cost Climbing Stairs (LeetCode 746)
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        vector<int> dp(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            dp[i] = min(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cost to reach the top of the floor starting at index 0 or 1 (LeetCode 746).
    // - Approach: 1D DP tabulation tracking minimum cost to reach step i.
    // - Intuition: Step i can be reached from step i-1 or i-2; `dp[i] = min(dp[i-1] + cost[i-1], dp[i-2] + cost[i-2])`.
    // - Complexity: Time: O(N), Space: O(N).

    // House Robber (LeetCode 198)
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        vector<int> dp(n + 1, 0);
        dp[1] = nums[0];
        for (int i = 2; i <= n; i++) {
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[i - 1]);
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum money robbed from non-adjacent houses (LeetCode 198).
    // - Approach: 1D DP tabulation array `dp[i]` storing max robbed from first i houses.
    // - Intuition: At house i, choose max of skipping house i-1 (`dp[i-1]`) or robbing it (`dp[i-2] + nums[i-1]`).
    // - Complexity: Time: O(N), Space: O(N).

    // House Robber II (LeetCode 213)
    int robCircular(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];

        auto robRange = [&](int l, int r) {
            vector<int> dp(r - l + 2, 0);
            dp[1] = nums[l];
            for (int i = l + 1; i <= r; i++) {
                int idx = i - l + 1;
                dp[idx] = max(dp[idx - 1], dp[idx - 2] + nums[i]);
            }
            return dp.back();
        };

        return max(robRange(0, n - 2), robRange(1, n - 1));
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum money robbed when houses are arranged in a circle (LeetCode 213).
    // - Approach: Split into two linear subproblems: [0..N-2] and [1..N-1].
    // - Intuition: First and last houses cannot both be robbed; evaluate linear DP on both ranges and take maximum.
    // - Complexity: Time: O(N), Space: O(N).

    // Decode Ways (LeetCode 91)
    int numDecodings(string s) {
        int n = s.size();
        if (n == 0) return 0;
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= n; i++) {
            if (s[i - 1] != '0') dp[i] += dp[i - 1];
            if (i >= 2) {
                int x = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
                if (10 <= x && x <= 26) dp[i] += dp[i - 2];
            }
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Count total ways to decode a digit string into letter combinations (LeetCode 91).
    // - Approach: 1D DP tabulation `dp[i]` representing total decoding ways for first i characters.
    // - Intuition: Single digit `s[i-1] != '0'` adds `dp[i-1]`; two-digit `s[i-2..i-1]` in [10..26] adds `dp[i-2]`.
    // - Complexity: Time: O(N), Space: O(N).

    // Delete and Earn (LeetCode 740)
    int deleteAndEarn(vector<int>& nums) {
        if (nums.empty()) return 0;
        int mx = *max_element(nums.begin(), nums.end());
        vector<int> sum(mx + 1, 0);
        for (int x : nums) sum[x] += x;
        vector<int> dp(mx + 1, 0);
        for (int x = 1; x <= mx; x++) {
            dp[x] = dp[x - 1];
            if (x >= 2) dp[x] = max(dp[x], dp[x - 2] + sum[x]);
            else dp[x] = max(dp[x], sum[x]);
        }
        return dp[mx];
    }
    // Interview Explanation:
    // - Problem Statement: Maximize points gained by taking nums[i] and deleting all occurrences of nums[i]-1 and nums[i]+1 (LeetCode 740).
    // - Approach: Frequency sum mapping transformed into House Robber DP.
    // - Intuition: Precompute `sum[x] = x * count`; choosing x forbids x-1, matching House Robber state transitions.
    // - Complexity: Time: O(N + \max(\text{nums})), Space: O(\max(\text{nums})).


    // =========================================================
    // 2. KNAPSACK / SUBSET SUM
    // =========================================================

    // 0/1 Knapsack
    int knapsack01(int W, const vector<int>& wt, const vector<int>& val) {
        int n = wt.size();
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= W; w++) {
                dp[i][w] = dp[i - 1][w];
                if (wt[i - 1] <= w) {
                    dp[i][w] = max(dp[i][w], dp[i - 1][w - wt[i - 1]] + val[i - 1]);
                }
            }
        }
        return dp[n][W];
    }
    // Interview Explanation:
    // - Problem Statement: 0/1 Knapsack — maximize total value without exceeding weight capacity W.
    // - Approach: 2D DP Tabulation table `dp[i][w]` using first i items.
    // - Intuition: Item i-1 can either be excluded (`dp[i-1][w]`) or included (`dp[i-1][w-wt[i-1]] + val[i-1]`).
    // - Complexity: Time: O(N \cdot W), Space: O(N \cdot W).

    // Partition Equal Subset Sum (LeetCode 416)
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % 2 != 0) return false;
        int target = sum / 2, n = nums.size();
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        for (int i = 0; i <= n; i++) dp[i][0] = true;
        for (int i = 1; i <= n; i++) {
            for (int t = 1; t <= target; t++) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t) dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
            }
        }
        return dp[n][target];
    }
    // Interview Explanation:
    // - Problem Statement: Determine if an array can be partitioned into two subsets with equal sum (LeetCode 416).
    // - Approach: 0/1 Knapsack subset sum DP matching target = totalSum / 2.
    // - Intuition: Problem reduces to finding if a subset sums exactly to sum/2 using 2D boolean DP table.
    // - Complexity: Time: O(N \cdot \text{sum}), Space: O(N \cdot \text{sum}).

    // Target Sum (LeetCode 494)
    int findTargetSumWays(vector<int>& nums, int target) {
        int total = accumulate(nums.begin(), nums.end(), 0);
        if (abs(target) > total || (total + target) % 2 != 0) return 0;
        int subsetSum = (total + target) / 2, n = nums.size();
        vector<vector<long long>> dp(n + 1, vector<long long>(subsetSum + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int s = 0; s <= subsetSum; s++) {
                dp[i][s] = dp[i - 1][s];
                if (nums[i - 1] <= s) dp[i][s] += dp[i - 1][s - nums[i - 1]];
            }
        }
        return dp[n][subsetSum];
    }
    // Interview Explanation:
    // - Problem Statement: Assign '+' and '-' signs to elements to evaluate to target (LeetCode 494).
    // - Approach: Mathematical transformation to subset sum DP: `P = (total + target) / 2`.
    // - Intuition: Finding positive subset P reduces target evaluation to counting subsets summing to (total + target) / 2.
    // - Complexity: Time: O(N \cdot \text{sum}), Space: O(N \cdot \text{sum}).

    // Coin Change (LeetCode 322)
    int coinChange(vector<int>& coins, int amount) {
        const int INF = amount + 1;
        vector<int> dp(amount + 1, INF);
        dp[0] = 0;
        for (int a = 1; a <= amount; a++) {
            for (int coin : coins) {
                if (coin <= a && dp[a - coin] != INF) {
                    dp[a] = min(dp[a], dp[a - coin] + 1);
                }
            }
        }
        return dp[amount] == INF ? -1 : dp[amount];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum number of coins needed to make up target amount (LeetCode 322).
    // - Approach: Unbounded Knapsack 1D DP tabulation array `dp[a]`.
    // - Intuition: `dp[a] = min(dp[a], dp[a - coin] + 1)` testing each coin denomination for amount a.
    // - Complexity: Time: O(N \cdot \text{amount}), Space: O(\text{amount}).

    // Coin Change II (LeetCode 518)
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        vector<vector<long long>> dp(n + 1, vector<long long>(amount + 1, 0));
        for (int i = 0; i <= n; i++) dp[i][0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int a = 1; a <= amount; a++) {
                dp[i][a] = dp[i - 1][a];
                if (coins[i - 1] <= a) dp[i][a] += dp[i][a - coins[i - 1]];
            }
        }
        return (int)dp[n][amount];
    }
    // Interview Explanation:
    // - Problem Statement: Count total unique combinations to make up target amount (LeetCode 518).
    // - Approach: Unbounded Knapsack 2D DP tabulation `dp[i][a]`.
    // - Intuition: Outer loop over coin types ensures combinations are ordered, preventing duplicate permutations.
    // - Complexity: Time: O(N \cdot \text{amount}), Space: O(N \cdot \text{amount}).

    // Combination Sum IV (LeetCode 377)
    int combinationSum4(vector<int>& nums, int target) {
        vector<unsigned int> dp(target + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= target; i++) {
            for (int x : nums) {
                if (x <= i) dp[i] += dp[i - x];
            }
        }
        return dp[target];
    }
    // Interview Explanation:
    // - Problem Statement: Count total permutations of numbers that sum up to target (LeetCode 377).
    // - Approach: Unbounded Knapsack permutation 1D DP (`dp[i] += dp[i - x]` with target loop outer).
    // - Intuition: Target loop outer allows different orderings of numbers to be counted as distinct permutations.
    // - Complexity: Time: O(N \cdot \text{target}), Space: O(\text{target}).


    // =========================================================
    // 3. STRING DP
    // =========================================================

    // Longest Common Subsequence (LeetCode 1143)
    int longestCommonSubsequence(string text1, string text2) {
        int n = text1.size(), m = text2.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (text1[i - 1] == text2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        return dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Find length of longest common subsequence between text1 and text2 (LeetCode 1143).
    // - Approach: 2D DP tabulation table `dp[i][j]` comparing prefixes.
    // - Intuition: If `text1[i-1] == text2[j-1]`, extend LCS by `1 + dp[i-1][j-1]`; else take `max(dp[i-1][j], dp[i][j-1])`.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Edit Distance (LeetCode 72)
    int minDistance(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i <= n; i++) dp[i][0] = i;
        for (int j = 0; j <= m; j++) dp[0][j] = j;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (word1[i - 1] == word2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
                else dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
        return dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum operations (insert, delete, replace) to convert word1 to word2 (LeetCode 72).
    // - Approach: 2D Edit Distance DP table `dp[i][j]`.
    // - Intuition: If characters match, `dp[i][j] = dp[i-1][j-1]`; else 1 + min(delete: `dp[i-1][j]`, insert: `dp[i][j-1]`, replace: `dp[i-1][j-1]`).
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Distinct Subsequences (LeetCode 115)
    int numDistinct(string s, string t) {
        int n = s.size(), m = t.size();
        vector<vector<unsigned long long>> dp(n + 1, vector<unsigned long long>(m + 1, 0));
        for (int i = 0; i <= n; i++) dp[i][0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                dp[i][j] = dp[i - 1][j];
                if (s[i - 1] == t[j - 1]) dp[i][j] += dp[i - 1][j - 1];
            }
        }
        return (int)dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Count number of unique subsequences of s that equal t (LeetCode 115).
    // - Approach: 2D DP table `dp[i][j]` matching prefix s[0..i-1] with t[0..j-1].
    // - Intuition: Always add `dp[i-1][j]` (skipping s[i-1]); if `s[i-1] == t[j-1]`, also add `dp[i-1][j-1]`.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Word Break (LeetCode 139)
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Check if string s can be segmented into dictionary words (LeetCode 139).
    // - Approach: 1D DP `dp[i]` tracking segmentability of prefix s[0..i-1].
    // - Intuition: Prefix s[0..i-1] is valid if there exists split point j where s[0..j-1] is valid (`dp[j] == true`) and substring s[j..i-1] is in dict.
    // - Complexity: Time: O(N^2 \cdot L) where L is max word length, Space: O(N + \text{dict}).

    // Interleaving String (LeetCode 97)
    bool isInterleave(string s1, string s2, string s3) {
        int n = s1.size(), m = s2.size();
        if (n + m != (int)s3.size()) return false;
        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
        dp[0][0] = true;
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= m; j++) {
                if (i > 0 && s1[i - 1] == s3[i + j - 1]) dp[i][j] = dp[i][j] || dp[i - 1][j];
                if (j > 0 && s2[j - 1] == s3[i + j - 1]) dp[i][j] = dp[i][j] || dp[i][j - 1];
            }
        }
        return dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Check if s3 is formed by interleaving s1 and s2 preserving character order (LeetCode 97).
    // - Approach: 2D Grid DP `dp[i][j]` matching s1[0..i-1] and s2[0..j-1] against s3[0..i+j-1].
    // - Intuition: Transition matches `s1[i-1] == s3[i+j-1]` from top OR `s2[j-1] == s3[i+j-1]` from left.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Longest Palindromic Subsequence (LeetCode 516)
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) dp[i][i] = 1;
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) dp[i][j] = 2 + (len == 2 ? 0 : dp[i + 1][j - 1]);
                else dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
        return dp[0][n - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Find length of longest palindromic subsequence in string s (LeetCode 516).
    // - Approach: Interval DP `dp[i][j]` over substring range s[i..j].
    // - Intuition: If `s[i] == s[j]`, `dp[i][j] = 2 + dp[i+1][j-1]`; else take `max(dp[i+1][j], dp[i][j-1])`.
    // - Complexity: Time: O(N^2), Space: O(N^2).


    // =========================================================
    // 4. LIS / SEQUENCE
    // =========================================================

    // Longest Increasing Subsequence (LeetCode 300)
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
    // - Problem Statement: Find length of longest strictly increasing subsequence (LeetCode 300).
    // - Approach: Binary Search + Patience Sorting (`tails` array storing smallest tail per length).
    // - Intuition: `lower_bound` finds first tail >= x and replaces it to maintain smallest possible tail values for future extensions.
    // - Complexity: Time: O(N \log N), Space: O(N).

    // Longest String Chain (LeetCode 1048)
    int longestStrChain(vector<string>& words) {
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });
        unordered_map<string, int> dp;
        int ans = 0;
        for (const string& word : words) {
            int best = 1;
            for (int i = 0; i < (int)word.size(); i++) {
                string prev = word.substr(0, i) + word.substr(i + 1);
                if (dp.count(prev)) best = max(best, dp[prev] + 1);
            }
            dp[word] = best;
            ans = max(ans, best);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find longest string chain where each word is formed by inserting 1 letter into previous word (LeetCode 1048).
    // - Approach: Sort words by length + Hash Map LIS DP.
    // - Intuition: For each word, remove 1 character at position i to generate predecessor `prev` and update `dp[word] = max(dp[prev] + 1)`.
    // - Complexity: Time: O(N \log N + N \cdot L^2) where L is word length, Space: O(N).

    // Largest Divisible Subset (LeetCode 368)
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        if (nums.empty()) return {};
        sort(nums.begin(), nums.end());
        int n = nums.size(), bestLen = 1, bestIdx = 0;
        vector<int> dp(n, 1), parent(n, -1);
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (dp[i] > bestLen) { bestLen = dp[i]; bestIdx = i; }
        }
        vector<int> ans;
        for (int cur = bestIdx; cur != -1; cur = parent[cur]) ans.push_back(nums[cur]);
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find largest subset where every pair (a, b) satisfies `a % b == 0` (LeetCode 368).
    // - Approach: Sorting + LIS DP with parent pointer array for subset path reconstruction.
    // - Intuition: Sorting guarantees if `nums[i] % nums[j] == 0`, then `nums[i]` is divisible by all elements in `nums[j]`'s chain.
    // - Complexity: Time: O(N^2), Space: O(N).

    // Maximum Sum Increasing Subsequence (MSIS - GFG / Striver SDE)
    int maxSumIS(vector<int>& arr) {
        int n = arr.size();
        vector<int> dp = arr; // dp[i] stores max sum increasing subsequence ending at index i
        int maxSum = *max_element(arr.begin(), arr.end());
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (arr[j] < arr[i] && dp[j] + arr[i] > dp[i]) {
                    dp[i] = dp[j] + arr[i];
                }
            }
            maxSum = max(maxSum, dp[i]);
        }
        return maxSum;
    }
    // Interview Explanation:
    // - Problem Statement: Find the maximum sum subsequence such that elements are strictly increasing (MSIS).
    // - Approach: LIS DP variation tracking sum instead of length.
    // - Intuition: For element `arr[i]`, transition: `dp[i] = max(arr[i], dp[j] + arr[i])` for all `j < i` where `arr[j] < arr[i]`.
    // - Complexity: Time: O(N^2), Space: O(N).

    // Maximum Profit in Job Scheduling (LeetCode 1235 / Striver SDE)
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<tuple<int, int, int>> jobs(n);
        for (int i = 0; i < n; i++) {
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        }
        sort(jobs.begin(), jobs.end()); // sort by end time

        vector<int> endTimes(n);
        for (int i = 0; i < n; i++) endTimes[i] = get<0>(jobs[i]);

        vector<int> dp(n + 1, 0); // dp[i] = max profit considering first i jobs
        for (int i = 1; i <= n; i++) {
            auto [e, s, p] = jobs[i - 1];
            // Binary search for latest non-conflicting job ending <= s
            int idx = upper_bound(endTimes.begin(), endTimes.end(), s) - endTimes.begin();
            dp[i] = max(dp[i - 1], dp[idx] + p);
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum profit scheduling non-overlapping jobs with given start, end times and profits (LeetCode 1235).
    // - Approach: Weighted Interval Scheduling with DP + Binary Search (`upper_bound`).
    // - Intuition: Sort jobs by end time. For job i, either skip it (`dp[i-1]`) or take it (`profit + dp[idx]`), where `idx` is the latest job finishing before job i starts.
    // - Complexity: Time: O(N \log N), Space: O(N).


    // =========================================================
    // 5. GRID DP
    // =========================================================

    // Unique Paths (LeetCode 62)
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n, 1));
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
        return dp[m - 1][n - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Count total unique paths from top-left (0,0) to bottom-right (m-1, n-1) in an m x n grid (LeetCode 62).
    // - Approach: 2D Grid DP table `dp[i][j]`.
    // - Intuition: Cell (i, j) can only be reached from top `dp[i-1][j]` or left `dp[i][j-1]`; `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
    // - Complexity: Time: O(M \cdot N), Space: O(M \cdot N).

    // Unique Paths II (LeetCode 63)
    long long uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int n = obstacleGrid.size(), m = obstacleGrid[0].size();
        vector<vector<long long>> dp(n, vector<long long>(m, 0));
        if (obstacleGrid[0][0] == 1 || obstacleGrid[n - 1][m - 1] == 1) return 0;
        dp[0][0] = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (obstacleGrid[i][j] == 1) { dp[i][j] = 0; continue; }
                if (i > 0) dp[i][j] += dp[i - 1][j];
                if (j > 0) dp[i][j] += dp[i][j - 1];
            }
        }
        return dp[n - 1][m - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Count unique paths from top-left to bottom-right avoiding obstacles (LeetCode 63).
    // - Approach: 2D Grid DP table `dp[i][j]`.
    // - Intuition: Obstacle sets `dp[i][j] = 0`; otherwise sum paths coming from top (`dp[i-1][j]`) and left (`dp[i][j-1]`).
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Minimum Path Sum (LeetCode 64)
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        vector<vector<int>> dp(n, vector<int>(m, 0));
        dp[0][0] = grid[0][0];
        for (int i = 1; i < n; i++) dp[i][0] = dp[i - 1][0] + grid[i][0];
        for (int j = 1; j < m; j++) dp[0][j] = dp[0][j - 1] + grid[0][j];
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                dp[i][j] = grid[i][j] + min(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        return dp[n - 1][m - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Find path from top-left to bottom-right minimizing sum of cell numbers (LeetCode 64).
    // - Approach: 2D Grid DP table `dp[i][j]`.
    // - Intuition: `dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])` taking minimum incoming path cost.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Maximal Square (LeetCode 221)
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int n = matrix.size(), m = matrix[0].size(), best = 0;
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (matrix[i - 1][j - 1] == '1') {
                    dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                    best = max(best, dp[i][j]);
                }
            }
        }
        return best * best;
    }
    // Interview Explanation:
    // - Problem Statement: Find largest square containing only 1s and return its area (LeetCode 221).
    // - Approach: 2D Grid DP using 2x2 local minimum square extension rule.
    // - Intuition: `dp[i][j] = 1 + min(top, left, top-left)`; a square of side k can only be formed if all 3 neighbor sub-squares are at least side k-1.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Maximal Rectangle (LeetCode 85)
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int n = matrix.size(), m = matrix[0].size(), maxArea = 0;
        vector<int> heights(m, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0;
            }
            stack<int> stk;
            for (int j = 0; j <= m; j++) {
                int h = (j == m) ? 0 : heights[j];
                while (!stk.empty() && heights[stk.top()] >= h) {
                    int height = heights[stk.top()]; stk.pop();
                    int width = stk.empty() ? j : (j - stk.top() - 1);
                    maxArea = max(maxArea, height * width);
                }
                stk.push(j);
            }
        }
        return maxArea;
    }
    // Interview Explanation:
    // - Problem Statement: Find largest rectangle containing only 1s in a 2D binary grid (LeetCode 85).
    // - Approach: Dynamic row histogram construction + Monotonic Stack Largest Rectangle in Histogram solver.
    // - Intuition: Each row forms a histogram of contiguous '1' heights; running histogram max area per row finds global maximum rectangle.
    // - Complexity: Time: O(N \cdot M), Space: O(M).


    // =========================================================
    // 6. PARTITION / INTERVAL DP
    // =========================================================

    // Matrix Chain Multiplication (MCM Template)
    int matrixChainMultiplication(const vector<int>& p) {
        int n = p.size() - 1;
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        for (int len = 2; len <= n; len++) {
            for (int i = 1; i <= n - len + 1; i++) {
                int j = i + len - 1;
                dp[i][j] = INT_MAX;
                for (int k = i; k < j; k++) {
                    int cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        return dp[1][n];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum scalar multiplications needed to multiply n matrices with dimensions array p.
    // - Approach: Classic Interval MCM DP over subsegment range [i..j].
    // - Intuition: Split matrix multiplication range [i..j] at position k; `dp[i][j] = min(dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j])`.
    // - Complexity: Time: O(N^3), Space: O(N^2).

    // Burst Balloons (LeetCode 312)
    int maxCoinsBurstBalloons(vector<int>& nums) {
        int n = nums.size();
        vector<int> b(n + 2, 1);
        for (int i = 0; i < n; i++) b[i + 1] = nums[i];
        int sz = n + 2;
        vector<vector<int>> dp(sz, vector<int>(sz, 0));
        for (int len = 2; len < sz; len++) {
            for (int i = 0; i + len < sz; i++) {
                int j = i + len;
                for (int k = i + 1; k < j; k++) {
                    dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] + b[i] * b[k] * b[j]);
                }
            }
        }
        return dp[0][sz - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Maximize coins collected by bursting balloons (LeetCode 312).
    // - Approach: Reverse Interval MCM DP (thinking of balloon k as the LAST balloon burst in interval (i, j)).
    // - Intuition: If k is burst last in (i, j), its remaining neighbors are b[i] and b[j], yielding coins `b[i]*b[k]*b[j] + dp[i][k] + dp[k][j]`.
    // - Complexity: Time: O(N^3), Space: O(N^2).

    // Minimum Cost to Cut a Stick (LeetCode 1547)
    int minCostCutStick(int n, vector<int>& cuts) {
        vector<int> c = cuts;
        c.push_back(0); c.push_back(n);
        sort(c.begin(), c.end());
        int m = c.size();
        vector<vector<int>> dp(m, vector<int>(m, 0));
        for (int len = 2; len < m; len++) {
            for (int i = 0; i + len < m; i++) {
                int j = i + len;
                dp[i][j] = INT_MAX;
                for (int k = i + 1; k < j; k++) {
                    dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j] + c[j] - c[i]);
                }
            }
        }
        return dp[0][m - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cost to cut a stick of length n at given cut positions (LeetCode 1547).
    // - Approach: Interval MCM DP over sorted cut positions including endpoints 0 and n.
    // - Intuition: `dp[i][j]` is min cost to cut segment between cuts[i] and cuts[j]; test all intermediate cut choices k.
    // - Complexity: Time: O(M^3) where M is number of cuts, Space: O(M^2).

    // Palindrome Partitioning II (LeetCode 132)
    int minCutPalindromePartitioning(string s) {
        int n = s.size();
        if (n <= 1) return 0;
        vector<vector<bool>> pal(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i <= 2 || pal[i + 1][j - 1])) pal[i][j] = true;
            }
        }
        vector<int> dp(n, INT_MAX);
        for (int i = 0; i < n; i++) {
            if (pal[0][i]) { dp[i] = 0; continue; }
            for (int j = 0; j < i; j++) {
                if (pal[j + 1][i]) dp[i] = min(dp[i], dp[j] + 1);
            }
        }
        return dp[n - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cuts needed for palindrome partitioning of string s (LeetCode 132).
    // - Approach: 2D DP Palindrome table precomputation + 1D Cut Minimization DP.
    // - Intuition: Precompute `pal[i][j]` in O(N^2); `dp[i]` stores min cuts for prefix s[0..i]; transition checks if s[j+1..i] is a palindrome.
    // - Complexity: Time: O(N^2), Space: O(N^2).


    // =========================================================
    // 7. TREE DP
    // =========================================================

    // Maximum Path Sum (LeetCode 124)
    pair<int, int> maxPathSumHelper(TreeNode* root) {
        if (!root) return {0, INT_MIN};
        auto left = maxPathSumHelper(root->left);
        auto right = maxPathSumHelper(root->right);
        int leftGain = max(0, left.first);
        int rightGain = max(0, right.first);
        int bestDown = root->val + max(leftGain, rightGain);
        int bestThrough = root->val + leftGain + rightGain;
        int bestSubtree = max({left.second, right.second, bestThrough});
        return {bestDown, bestSubtree};
    }

    int maxPathSum(TreeNode* root) {
        return maxPathSumHelper(root).second;
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum path sum in a binary tree along any node-to-node path (LeetCode 124).
    // - Approach: Post-order Tree DP recursion returning `{maxDownwardBranch, maxSubtreePath}`.
    // - Intuition: At each node, compute max gain from left and right subtrees (pruning negative gains); update best path through root (`val + left + right`).
    // - Complexity: Time: O(N), Space: O(H) call stack.

    // House Robber III (LeetCode 337)
    pair<int, int> robTreeHelper(TreeNode* root) {
        if (!root) return {0, 0};
        auto left = robTreeHelper(root->left);
        auto right = robTreeHelper(root->right);
        int robNode = root->val + left.second + right.second;
        int skipNode = max(left.first, left.second) + max(right.first, right.second);
        return {robNode, skipNode};
    }

    int robTree(TreeNode* root) {
        auto [rob, skip] = robTreeHelper(root);
        return max(rob, skip);
    }
    // Interview Explanation:
    // - Problem Statement: House Robber III on binary tree nodes (LeetCode 337).
    // - Approach: Post-order Tree DP returning `{robThisNode, skipThisNode}` pair.
    // - Intuition: If robbing current node, cannot rob children (`val + left.skip + right.skip`); if skipping, pick max choice for each child.
    // - Complexity: Time: O(N), Space: O(H).


    // =========================================================
    // 8. STATE MACHINE DP
    // =========================================================

    // Stock with Cooldown (LeetCode 309)
    int maxProfitWithCooldown(vector<int>& prices) {
        int n = prices.size();
        if (n == 0) return 0;
        vector<vector<int>> dp(n, vector<int>(3, 0));
        dp[0][0] = -prices[0];
        dp[0][1] = INT_MIN / 2;
        dp[0][2] = 0;
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][2] - prices[i]);
            dp[i][1] = dp[i - 1][0] + prices[i];
            dp[i][2] = max(dp[i - 1][2], dp[i - 1][1]);
        }
        return max(dp[n - 1][1], dp[n - 1][2]);
    }
    // Interview Explanation:
    // - Problem Statement: Best Time to Buy and Sell Stock with 1-day cooldown after selling (LeetCode 309).
    // - Approach: 3-State Machine DP (`holding`, `just_sold`, `resting`).
    // - Intuition: Holding transitions from prev hold or rest - price; just sold transitions from hold + price; resting transitions from prev sold or rest.
    // - Complexity: Time: O(N), Space: O(N).

    // Stock with Transaction Fee (LeetCode 714)
    int maxProfitWithFee(vector<int>& prices, int fee) {
        int n = prices.size();
        if (n == 0) return 0;
        vector<vector<int>> dp(n, vector<int>(2, 0));
        dp[0][0] = -prices[0];
        dp[0][1] = 0;
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][1] - prices[i]);
            dp[i][1] = max(dp[i - 1][1], dp[i - 1][0] + prices[i] - fee);
        }
        return dp[n - 1][1];
    }
    // Interview Explanation:
    // - Problem Statement: Best Time to Buy and Sell Stock with transaction fee per trade (LeetCode 714).
    // - Approach: 2-State Machine DP (`holding`, `cash`).
    // - Intuition: Cash (not holding) updates by selling `hold + price - fee`; holding updates by buying `cash - price`.
    // - Complexity: Time: O(N), Space: O(N).

    // Stock IV - At Most K Transactions (LeetCode 188)
    int maxProfitK(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++) if (prices[i] > prices[i - 1]) profit += prices[i] - prices[i - 1];
            return profit;
        }
        vector<vector<int>> dp(k + 1, vector<int>(n, 0));
        for (int t = 1; t <= k; t++) {
            int maxDiff = -prices[0];
            for (int i = 1; i < n; i++) {
                dp[t][i] = max(dp[t][i - 1], prices[i] + maxDiff);
                maxDiff = max(maxDiff, dp[t - 1][i] - prices[i]);
            }
        }
        return dp[k][n - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum profit completing at most k stock transactions (LeetCode 188).
    // - Approach: 2D State Machine DP `dp[t][i]` tracking transaction count t and day i with `maxDiff` optimization.
    // - Intuition: `dp[t][i] = max(dp[t][i-1], prices[i] + maxDiff)` where `maxDiff = max(dp[t-1][j] - prices[j])` avoids an inner O(N) loop.
    // - Complexity: Time: O(K \cdot N), Space: O(K \cdot N).


    // =========================================================
    // 9. DIGIT DP
    // =========================================================

    // Count Digit One (LeetCode 233)
    string digitStr;
    long long digitMemo[20][20][2][2];

    long long countDigitOneDFS(int pos, int count, bool started, bool tight) {
        if (pos == (int)digitStr.size()) return count;
        long long &memo = digitMemo[pos][count][started][tight];
        if (memo != -1) return memo;
        int limit = tight ? digitStr[pos] - '0' : 9;
        long long ans = 0;
        for (int d = 0; d <= limit; d++) {
            bool nextStarted = started || d != 0;
            int nextCount = count + (nextStarted && d == 1);
            bool nextTight = tight && (d == limit);
            ans += countDigitOneDFS(pos + 1, nextCount, nextStarted, nextTight);
        }
        return memo = ans;
    }

    int countDigitOne(int n) {
        if (n <= 0) return 0;
        digitStr = to_string(n);
        memset(digitMemo, -1, sizeof(digitMemo));
        return (int)countDigitOneDFS(0, 0, false, true);
    }
    // Interview Explanation:
    // - Problem Statement: Count total occurrences of digit '1' in all non-negative integers <= n (LeetCode 233).
    // - Approach: Digit DP Memoization over state `(pos, count1s, started, tight)`.
    // - Intuition: `tight` restricts digits to prefix limit; `started` distinguishes leading zeros from valid digit 0.
    // - Complexity: Time: O(\log_{10} N \cdot \text{digits}), Space: O(\log_{10} N).


    // =========================================================
    // 10. BITMASK DP
    // =========================================================

    // Smallest Sufficient Team (LeetCode 1125)
    vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
        int m = req_skills.size(), n = people.size();
        unordered_map<string, int> skillId;
        for (int i = 0; i < m; i++) skillId[req_skills[i]] = i;
        vector<int> personMask(n, 0);
        for (int i = 0; i < n; i++) {
            for (const string& skill : people[i]) {
                if (skillId.count(skill)) personMask[i] |= 1 << skillId[skill];
            }
        }
        int states = 1 << m, target = states - 1;
        vector<vector<int>> dp(states);
        vector<bool> reachable(states, false);
        reachable[0] = true;
        for (int mask = 0; mask < states; mask++) {
            if (!reachable[mask]) continue;
            for (int i = 0; i < n; i++) {
                int nextMask = mask | personMask[i];
                if (nextMask == mask) continue;
                if (!reachable[nextMask] || dp[nextMask].size() > dp[mask].size() + 1) {
                    reachable[nextMask] = true;
                    dp[nextMask] = dp[mask];
                    dp[nextMask].push_back(i);
                }
            }
        }
        return dp[target];
    }
    // Interview Explanation:
    // - Problem Statement: Find smallest subset of people covering all required skills (LeetCode 1125).
    // - Approach: Bitmask DP over skill states `dp[mask]`.
    // - Intuition: Bitmask represents covered skills (up to 16 skills => 2^16 states); state transition adds person i to expand skill mask.
    // - Complexity: Time: O(2^M \cdot N), Space: O(2^M).


    // =========================================================
    // 11. GAME / MINIMAX DP
    // =========================================================

    // Removal Game / Stone Game I (CSES / LeetCode 877)
    long long removalGame(vector<int>& nums) {
        int n = nums.size();
        vector<vector<long long>> dp(n, vector<long long>(n, 0));
        for (int i = 0; i < n; i++) dp[i][i] = nums[i];
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                dp[i][j] = max(nums[i] - dp[i + 1][j], nums[j] - dp[i][j - 1]);
            }
        }
        long long total = accumulate(nums.begin(), nums.end(), 0LL);
        return (total + dp[0][n - 1]) / 2;
    }
    // Interview Explanation:
    // - Problem Statement: Removal Game / Stone Game — Two players pick numbers from ends of array; maximize player 1's total (CSES / LeetCode 877).
    // - Approach: Minimax Interval DP calculating relative score difference `player1 - player2`.
    // - Intuition: `dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])`; absolute score for player 1 is `(totalSum + maxDiff) / 2`.
    // - Complexity: Time: O(N^2), Space: O(N^2).

    // Predict the Winner (LeetCode 486)
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp = nums;
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                dp[i] = max(nums[i] - dp[i + 1], nums[j] - dp[i]);
            }
        }
        return dp[0] >= 0;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if Player 1 can win picking numbers from ends of array (LeetCode 486).
    // - Approach: 1D Space-Optimized Interval Minimax DP tracking relative score advantage.
    // - Intuition: At interval [i, j], current player can take nums[i] leaving -dp[i+1][j], or nums[j] leaving -dp[i][j-1]. Player 1 wins if final relative score >= 0.
    // - Complexity: Time: O(N^2), Space: O(N).

    // Can I Win (LeetCode 464 - Minimax with Bitmask Memoization)
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        int totalSum = (maxChoosableInteger * (maxChoosableInteger + 1)) / 2;
        if (totalSum < desiredTotal) return false;
        if (desiredTotal <= 0) return true;

        unordered_map<int, bool> memo;
        function<bool(int, int)> dfs = [&](int mask, int total) -> bool {
            if (memo.count(mask)) return memo[mask];
            for (int i = 1; i <= maxChoosableInteger; i++) {
                int bit = 1 << i;
                if (!(mask & bit)) {
                    if (total + i >= desiredTotal || !dfs(mask | bit, total + i)) {
                        return memo[mask] = true;
                    }
                }
            }
            return memo[mask] = false;
        };

        return dfs(0, 0);
    }
    // Interview Explanation:
    // - Problem Statement: Determine if first player can force a win reaching desiredTotal picking unique numbers 1..maxChoosableInteger (LeetCode 464).
    // - Approach: Minimax Game DP with Bitmask State Memoization.
    // - Intuition: Bitmask tracks which numbers 1..N have been used. A player wins if choosing number `i` immediately reaches desiredTotal, or forces opponent into a losing state (`!dfs(next_mask)`).
    // - Complexity: Time: O(2^N \cdot N), Space: O(2^N) memoization table where N = maxChoosableInteger <= 20.

    // Guess Number Higher or Lower II (LeetCode 375 - Interval Minimax DP)
    int getMoneyAmount(int n) {
        vvi dp(n + 2, vi(n + 2, 0));
        for (int len = 2; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                int minCost = INT_MAX;
                for (int k = i; k <= j; k++) {
                    int cost = k + max(dp[i][k - 1], dp[k + 1][j]);
                    minCost = min(minCost, cost);
                }
                dp[i][j] = minCost;
            }
        }
        return dp[1][n];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum money needed to guarantee a win guessing number between 1 and n (LeetCode 375).
    // - Approach: Interval Minimax DP (`min` of worst-case `max`).
    // - Intuition: Guessing number k costs $k and splits range into [i, k-1] (too high) and [k+1, j] (too low). Worst case payoff is `k + max(dp[i][k-1], dp[k+1][j])`; we minimize this cost over all choices k in [i, j].
    // - Complexity: Time: O(N^3), Space: O(N^2).

    // Stone Game II (LeetCode 1140 - Game DP with Dynamic M Parameter)
    int stoneGameII(vector<int>& piles) {
        int n = piles.size();
        vi suffixSum(n + 1, 0);
        for (int i = n - 1; i >= 0; i--) suffixSum[i] = suffixSum[i + 1] + piles[i];

        vvi dp(n + 1, vi(n + 1, 0)); // dp[i][M] = max stones obtainable from pile i with parameter M

        for (int i = n - 1; i >= 0; i--) {
            for (int m = 1; m <= n; m++) {
                if (i + 2 * m >= n) {
                    dp[i][m] = suffixSum[i]; // can take all remaining stones
                } else {
                    for (int x = 1; x <= 2 * m; x++) {
                        dp[i][m] = max(dp[i][m], suffixSum[i] - dp[i + x][max(m, x)]);
                    }
                }
            }
        }

        return dp[0][1];
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum stones Alice can get starting with M = 1 where player can take 1 <= X <= 2M piles (LeetCode 1140).
    // - Approach: 2D Suffix Sum Minimax DP on `(index, M)`.
    // - Intuition: Total stones taken from index i is `suffixSum[i] - dp[i + X][max(M, X)]` (total remaining minus opponent's best score from next state).
    // - Complexity: Time: O(N^3), Space: O(N^2).

    // Stone Game III (LeetCode 1406 - 1D Suffix Minimax DP)
    string stoneGameIII(vector<int>& stoneValue) {
        int n = stoneValue.size();
        vi dp(n + 1, 0); // dp[i] = max relative score difference player can get from index i

        for (int i = n - 1; i >= 0; i--) {
            int take = 0, best = INT_MIN;
            for (int k = 0; k < 3 && i + k < n; k++) {
                take += stoneValue[i + k];
                best = max(best, take - dp[i + k + 1]);
            }
            dp[i] = best;
        }

        if (dp[0] > 0) return "Alice";
        if (dp[0] < 0) return "Bob";
        return "Tie";
    }
    // Interview Explanation:
    // - Problem Statement: Return winner ("Alice", "Bob", "Tie") when players take 1, 2, or 3 stones from start (LeetCode 1406).
    // - Approach: 1D Suffix DP tracking relative score difference `(Alice - Bob)`.
    // - Intuition: Current player takes sum of first `k+1` stones (`k in {0, 1, 2}`) minus opponent's optimal advantage from `i + k + 1`: `dp[i] = max(take - dp[i + k + 1])`.
    // - Complexity: Time: O(N), Space: O(N) or O(1) keeping last 3 states.

    // Divisor Game (LeetCode 1025) & Nim Game (LeetCode 292)
    bool divisorGame(int n) {
        return n % 2 == 0;
    }
    bool canWinNim(int n) {
        return n % 4 != 0;
    }
    // Interview Explanation:
    // - Problem Statement: Mathematical Game Theory / Subtraction Games (LeetCode 1025 & 292).
    // - Approach: Parity / Modulo State Analysis or 1D Boolean DP.
    // - Intuition: In Divisor Game, even numbers always have odd divisors (like 1), allowing player to force opponent to an odd number (which only has odd divisors, always returning even). In Nim, multiples of 4 are losing positions.
    // - Complexity: Time: O(1), Space: O(1).


    // =========================================================
    // 12. ADVANCED DP
    // =========================================================

    // Super Egg Drop (LeetCode 887)
    int superEggDrop(int k, int n) {
        vector<int> dp(k + 1, 0);
        int moves = 0;
        while (dp[k] < n) {
            moves++;
            for (int i = k; i >= 1; i--) {
                dp[i] = dp[i] + dp[i - 1] + 1;
            }
        }
        return moves;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum moves to determine critical floor with k eggs and n floors (LeetCode 887).
    // - Approach: Dual inversion DP: `dp[moves][eggs]` = max floors checkable with given moves and eggs.
    // - Intuition: Dropping an egg at move m splits floors into egg-broken (`dp[i-1]`) and egg-intact (`dp[i]`); total floors checkable is `1 + dp[i-1] + dp[i]`.
    // - Complexity: Time: O(K \log N), Space: O(K).

    // Decode Ways II with Wildcards '*' (LeetCode 639)
    int numDecodings2(string s) {
        int n = s.size();
        if (n == 0) return 0;
        const int MOD = 1e9 + 7;
        vector<long long> dp(n + 1, 0);
        dp[0] = 1;
        dp[1] = (s[0] == '*') ? 9 : (s[0] == '0' ? 0 : 1);
        for (int i = 2; i <= n; i++) {
            char c1 = s[i - 1], c2 = s[i - 2];
            if (c1 == '*') dp[i] = (dp[i] + 9 * dp[i - 1]) % MOD;
            else if (c1 != '0') dp[i] = (dp[i] + dp[i - 1]) % MOD;

            if (c2 == '*') {
                if (c1 == '*') dp[i] = (dp[i] + 15 * dp[i - 2]) % MOD;
                else if (c1 <= '6') dp[i] = (dp[i] + 2 * dp[i - 2]) % MOD;
                else dp[i] = (dp[i] + dp[i - 2]) % MOD;
            } else if (c2 == '1') {
                if (c1 == '*') dp[i] = (dp[i] + 9 * dp[i - 2]) % MOD;
                else dp[i] = (dp[i] + dp[i - 2]) % MOD;
            } else if (c2 == '2') {
                if (c1 == '*') dp[i] = (dp[i] + 6 * dp[i - 2]) % MOD;
                else if (c1 <= '6') dp[i] = (dp[i] + dp[i - 2]) % MOD;
            }
        }
        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Decode digit string containing wildcard '*' matching '1'-'9' (LeetCode 639).
    // - Approach: 1D DP tabulation considering single-character and double-character wildcard expansion cases modulo 10^9+7.
    // - Intuition: '*' contributes 9 single-digit choices, 15 choices for '**' (11-19, 21-26), 9 choices for '1*', and 6 choices for '2*'.
    // - Complexity: Time: O(N), Space: O(N).

    // Minimum Cost For Tickets / Travel Tickets (LeetCode 983)
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        int lastDay = days.back();
        vector<int> dp(lastDay + 1, 0);
        unordered_set<int> travel(days.begin(), days.end());
        for (int i = 1; i <= lastDay; i++) {
            if (!travel.count(i)) {
                dp[i] = dp[i - 1];
            } else {
                dp[i] = min({
                    dp[i - 1] + costs[0],
                    dp[max(0, i - 7)] + costs[1],
                    dp[max(0, i - 30)] + costs[2]
                });
            }
        }
        return dp[lastDay];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cost to cover travel on given days using 1-day, 7-day, and 30-day passes (LeetCode 983).
    // - Approach: 1D Calendar DP up to `lastDay`.
    // - Intuition: Non-travel days carry forward `dp[i-1]`; travel days test purchasing 1-day, 7-day, or 30-day pass ending on day i.
    // - Complexity: Time: O(\text{lastDay}), Space: O(\text{lastDay}).

    // Dungeon Game (LeetCode 174 - Bottom-Up Health Calculation)
    int calculateMinimumHP(vvi& dungeon) {
        int m = dungeon.size(), n = dungeon[0].size();
        vvi dp(m + 1, vi(n + 1, 1e9));
        dp[m][n - 1] = dp[m - 1][n] = 1;

        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                int minHealth = min(dp[i + 1][j], dp[i][j + 1]) - dungeon[i][j];
                dp[i][j] = max(1, minHealth);
            }
        }
        return dp[0][0];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum initial health to rescue princess (LeetCode 174).
    // - Approach: Backwards Grid DP from destination (m-1, n-1) to start (0, 0).
    // - Intuition: `minHealth = min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j]`; health at any point cannot drop below 1.
    // - Complexity: Time: O(M * N), Space: O(M * N).

    // Cherry Pickup (LeetCode 741 - Simultaneous 2-Path DP)
    int cherryPickup(vvi& grid) {
        int n = grid.size();
        vvi dp(n, vi(n, -1));
        dp[0][0] = grid[0][0];

        for (int step = 1; step <= 2 * n - 2; ++step) {
            vvi nextDP(n, vi(n, -1));
            for (int r1 = max(0, step - (n - 1)); r1 <= min(n - 1, step); ++r1) {
                int c1 = step - r1;
                for (int r2 = max(0, step - (n - 1)); r2 <= min(n - 1, step); ++r2) {
                    int c2 = step - r2;
                    if (grid[r1][c1] == -1 || grid[r2][c2] == -1) continue;

                    int maxPrev = -1;
                    for (int dr1 : {0, -1}) {
                        for (int dr2 : {0, -1}) {
                            int pr1 = r1 + dr1, pr2 = r2 + dr2;
                            if (pr1 >= 0 && pr2 >= 0 && dp[pr1][pr2] != -1) {
                                maxPrev = max(maxPrev, dp[pr1][pr2]);
                            }
                        }
                    }
                    if (maxPrev == -1) continue;

                    int cherries = maxPrev + grid[r1][c1];
                    if (r1 != r2) cherries += grid[r2][c2];
                    nextDP[r1][r2] = cherries;
                }
            }
            dp = std::move(nextDP);
        }
        return max(0, dp[n - 1][n - 1]);
    }
    // Interview Explanation:
    // - Problem Statement: Collect maximum cherries going from (0,0) to (n-1,n-1) and returning back (LeetCode 741).
    // - Approach: Synchronous 2-Agent Manhattan Distance Step DP ($r_1 + c_1 = r_2 + c_2 = 	ext{step}$).
    // - Complexity: Time: O(N^3), Space: O(N^2).

    // 49. Maximum Product Subarray (LeetCode 152)
    int maxProduct(vi& nums) {
        int n = nums.size();
        int max_prod = nums[0], min_prod = nums[0], ans = nums[0];
        for (int i = 1; i < n; i++) {
            if (nums[i] < 0) swap(max_prod, min_prod);
            max_prod = max(nums[i], max_prod * nums[i]);
            min_prod = min(nums[i], min_prod * nums[i]);
            ans = max(ans, max_prod);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find a contiguous non-empty subarray that has the largest product (LeetCode 152).
    // - Approach: Dynamic Programming tracking running maximum and minimum products.
    // - Intuition: A negative number flips max and min; swap max_prod and min_prod on negative elements.
    // - Complexity: Time: O(N), Space: O(1).

    // 50. Ninja's Training (GeeksforGeeks)
    int ninjaTraining(int n, vvi& points) {
        vi prev(4, 0);
        for (int last = 0; last < 4; last++) {
            for (int task = 0; task < 3; task++) {
                if (task != last) prev[last] = max(prev[last], points[0][task]);
            }
        }
        for (int day = 1; day < n; day++) {
            vi curr(4, 0);
            for (int last = 0; last < 4; last++) {
                for (int task = 0; task < 3; task++) {
                    if (task != last) curr[last] = max(curr[last], points[day][task] + prev[task]);
                }
            }
            prev = curr;
        }
        return prev[3];
    }
    // Interview Explanation:
    // - Problem Statement: Maximize total points over N days with 3 daily activities without performing the same activity consecutively.
    // - Approach: 2D Dynamic Programming with space compression to O(1) size-4 array.
    // - Complexity: Time: O(N), Space: O(1).



    // =========================================================
    // 51. REMOVING DIGITS (CSES 1637)
    // =========================================================

    int minStepsRemovingDigits(int n) {
        vi dp(n + 1, -1);
        function<int(int)> solve = [&](int x) -> int {
            if (x == 0) return 0;
            if (dp[x] != -1) return dp[x];
            int ans = INT_MAX, tmp = x;
            while (tmp > 0) {
                int d = tmp % 10;
                tmp /= 10;
                if (d != 0) ans = min(ans, 1 + solve(x - d));
            }
            return dp[x] = ans;
        };
        return solve(n);
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum steps to reduce n to 0 by subtracting one of its digits (CSES 1637).
    // - Approach: Top-Down Memoized DP (`dp[n] = 1 + min over digits`).
    // - Complexity: Time: O(N log10 N), Space: O(N).


    // =========================================================
    // 52. ARRAY DESCRIPTION (CSES 1746)
    // =========================================================

    int countArrayDescriptions(int n, int m, const vi& a) {
        vvl dp(n, vl(m + 2, 0));
        if (a[0] == 0) {
            for (int val = 1; val <= m; val++) dp[0][val] = 1;
        } else {
            dp[0][a[0]] = 1;
        }

        for (int i = 1; i < n; i++) {
            if (a[i] == 0) {
                for (int val = 1; val <= m; val++) {
                    dp[i][val] = (dp[i - 1][val - 1] + dp[i - 1][val] + dp[i - 1][val + 1]) % MOD;
                }
            } else {
                int val = a[i];
                dp[i][val] = (dp[i - 1][val - 1] + dp[i - 1][val] + dp[i - 1][val + 1]) % MOD;
            }
        }

        ll ans = 0;
        for (int val = 1; val <= m; val++) ans = (ans + dp[n - 1][val]) % MOD;
        return (int)ans;
    }
    // Interview Explanation:
    // - Problem Statement: Count array fills with adjacent absolute differences <= 1 (CSES 1746).
    // - Approach: 2D DP (`dp[i][v] = dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]`).
    // - Complexity: Time: O(N * M), Space: O(N * M).


    // =========================================================
    // 53. COUNTING NUMBERS - NO ADJACENT EQUAL DIGITS (CSES 2220)
    // =========================================================

    long long countNumbersNoAdjacentEqual(long long a, long long b) {
        auto countUpTo = [](long long x) -> long long {
            if (x < 0) return 0;
            string num = to_string(x);
            long long memo[20][11][2][2];
            memset(memo, -1, sizeof(memo));

            function<long long(int, int, bool, bool)> solve = [&](int pos, int prev, bool started, bool tight) -> long long {
                if (pos == (int)num.size()) return 1;
                if (memo[pos][prev][started][tight] != -1) return memo[pos][prev][started][tight];

                int lim = tight ? num[pos] - '0' : 9;
                long long ans = 0;
                if (!started) ans += solve(pos + 1, 10, false, tight && !lim);

                for (int d = started ? 0 : 1; d <= lim; d++) {
                    if (!started || d != prev) {
                        ans += solve(pos + 1, d, true, tight && (d == lim));
                    }
                }
                return memo[pos][prev][started][tight] = ans;
            };

            return solve(0, 10, false, true);
        };

        auto isValid = [](long long x) -> bool {
            string s = to_string(x);
            for (size_t i = 1; i < s.size(); i++) if (s[i] == s[i - 1]) return false;
            return true;
        };

        return countUpTo(b) - countUpTo(a) + isValid(a);
    }
    // Interview Explanation:
    // - Problem Statement: Count integers in range [a, b] where no two adjacent digits are equal (CSES 2220).
    // - Approach: Digit DP with state (pos, prev_digit, started, tight).
    // - Complexity: Time: O(digits * 10), Space: O(1).


    // =========================================================
    // 54. RECTANGLE CUTTING (CSES 1744)
    // =========================================================

    int minCutsRectangle(int a, int b) {
        vvi dp(a + 1, vi(b + 1, 0));
        for (int i = 1; i <= a; i++) {
            for (int j = 1; j <= b; j++) {
                if (i == j) { dp[i][j] = 1; continue; }
                int ans = INT_MAX;
                for (int k = 1; k < i; k++) ans = min(ans, dp[k][j] + dp[i - k][j]);
                for (int k = 1; k < j; k++) ans = min(ans, dp[i][k] + dp[i][j - k]);
                dp[i][j] = ans;
            }
        }
        return dp[a][b] - 1;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cuts to partition an a x b rectangle into squares (CSES 1744).
    // - Approach: 2D Interval DP over all horizontal and vertical split lines.
    // - Complexity: Time: O(A * B * (A + B)), Space: O(A * B).


    // =========================================================
    // 55. ELEVATOR RIDES (CSES 1653)
    // =========================================================

    int minElevatorRides(int n, int maxWeight, const vi& w) {
        vector<pair<int, int>> dp(1 << n, {-1, -1});
        function<pair<int, int>(int)> solve = [&](int mask) -> pair<int, int> {
            if (!mask) return {1, 0};
            if (dp[mask].first != -1) return dp[mask];

            pair<int, int> best = {n + 1, 0};
            for (int i = 0; i < n; i++) {
                if (!(mask & (1 << i))) continue;
                auto prev = solve(mask ^ (1 << i));
                best = min(best, prev.second + w[i] <= maxWeight
                    ? pair<int, int>(prev.first, prev.second + w[i])
                    : pair<int, int>(prev.first + 1, w[i]));
            }
            return dp[mask] = best;
        };
        return solve((1 << n) - 1).first;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum elevator rides to transport n people under weight limit (CSES 1653).
    // - Approach: Bitmask DP storing pair state {rides, weight_of_last_ride}.
    // - Complexity: Time: O(2^N * N), Space: O(2^N).


    // =========================================================
    // 56. INCREASING SUBSEQUENCE II (CSES 1748)
    // =========================================================

    int countIncreasingSubsequences(int n, const vi& a) {
        vi vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        struct Fenwick {
            int sz; vi bit;
            Fenwick(int sz) : sz(sz), bit(sz + 1, 0) {}
            void add(int i, int val) {
                for (i++; i <= sz; i += i & -i) bit[i] = (bit[i] + val) % MOD;
            }
            int query(int i) {
                int s = 0;
                for (i++; i > 0; i -= i & -i) s = (s + bit[i]) % MOD;
                return s;
            }
        };

        Fenwick ft(vals.size());
        ll ans = 0;
        for (int x : a) {
            int idx = lower_bound(vals.begin(), vals.end(), x) - vals.begin();
            ll cur = (1 + (idx > 0 ? ft.query(idx - 1) : 0)) % MOD;
            ft.add(idx, cur);
            ans = (ans + cur) % MOD;
        }
        return (int)ans;
    }
    // Interview Explanation:
    // - Problem Statement: Count total increasing subsequences modulo 10^9+7 (CSES 1748).
    // - Approach: Coordinate Compression + Fenwick Tree DP.
    // - Complexity: Time: O(N log N), Space: O(N).


    // =========================================================
    // 57. MINIMAL GRID PATH (CSES 3359)
    // =========================================================

    string minLexicographicalGridPath(int n, const vector<string>& grid) {
        string ans = "";
        ans += grid[0][0];
        vector<pair<int, int>> cur = {{0, 0}};
        vector<vector<bool>> vis(n, vector<bool>(n, false));
        vis[0][0] = true;

        for (int step = 0; step < 2 * n - 2; step++) {
            char best = 'z' + 1;
            for (auto [x, y] : cur) {
                if (x + 1 < n) best = min(best, grid[x + 1][y]);
                if (y + 1 < n) best = min(best, grid[x][y + 1]);
            }
            ans += best;

            vector<pair<int, int>> nxt;
            for (auto [x, y] : cur) {
                if (x + 1 < n && grid[x + 1][y] == best && !vis[x + 1][y]) {
                    vis[x + 1][y] = true;
                    nxt.push_back({x + 1, y});
                }
                if (y + 1 < n && grid[x][y + 1] == best && !vis[x][y + 1]) {
                    vis[x][y + 1] = true;
                    nxt.push_back({x, y + 1});
                }
            }
            cur = move(nxt);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find lexicographically smallest path from (0,0) to (n-1, n-1) in grid (CSES 3359).
    // - Approach: Step-by-Step Level BFS / Greedy Selection.
    // - Complexity: Time: O(N^2), Space: O(N^2).

};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}

