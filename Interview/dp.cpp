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
#endif
using namespace std;

// Standard Binary Tree Node (Used in Tree DP - LC 337)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | LC #   | Core Pattern / Archetype           | Time     | Space    |
 |----|---------------------------------------------|--------|------------------------------------|----------|----------|
 | 01 | Climbing Stairs / Min Cost Climbing Stairs  | 70/746 | 1D Fibonacci State Transition      | O(N)     | O(1)     |
 | 02 | House Robber I & II (Linear & Circular)     | 198/213| 1D Pick vs Skip / Cycle Decoupling  | O(N)     | O(1)     |
 | 03 | Stock with Cooldown / Transaction Fee       | 309/714| Multi-State Machine DP             | O(N)     | O(1)     |
 | 04 | Decode Ways                                 | LC 91  | 1D Suffix/Prefix Branching          | O(N)     | O(1)     |
 | 05 | Maximum Subarray (Kadane's)                 | LC 53  | 1D Local vs Global Optimal Subarray | O(N)     | O(1)     |
 | 06 | Longest Increasing Subsequence (LIS)        | LC 300 | O(N^2) Tabulation & O(N log N) BS  | O(N logN)| O(N)     |
 | 07 | Russian Doll Envelopes                      | LC 354 | 2D Sorting Trick + 1D LIS          | O(N logN)| O(N)     |
 | 08 | Word Break                                  | LC 139 | 1D Substring Partitioning + Trie/Set| O(N * L^2)| O(N)   |
 | 09 | Longest Common Subsequence (LCS)            | LC 1143| 2-Sequence Grid Matching            | O(M * N) | O(min)   |
 | 10 | Edit Distance (Levenshtein)                 | LC 72  | 2-Sequence String Transformation   | O(M * N) | O(min)   |
 | 11 | Distinct Subsequences                       | LC 115 | 2-Sequence Subsequence Counting     | O(M * N) | O(N)     |
 | 12 | Wildcard Matching                           | LC 44  | 2D String Pattern Matching (*, ?)   | O(M * N) | O(N)     |
 | 13 | Regular Expression Matching                 | LC 10  | 2D String Pattern Matching (*, .)   | O(M * N) | O(N)     |
 | 14 | Unique Paths I & II                         | LC 62/63| 2D Grid Path Combinatorics          | O(M * N) | O(N)     |
 | 15 | Minimum Path Sum                            | LC 64  | 2D Grid Additive Cost Minimization  | O(M * N) | O(N)     |
 | 16 | Maximal Square                              | LC 221 | 2D Geometry Local Minimum Extension | O(M * N) | O(N)     |
 | 17 | Dungeon Game                                | LC 174 | 2D Bottom-Up Reverse Survival DP    | O(M * N) | O(N)     |
 | 18 | Coin Change (Min Coins)                     | LC 322 | Unbounded Knapsack (Minimization)   | O(N * A) | O(A)     |
 | 19 | Coin Change II (Total Ways)                 | LC 518 | Unbounded Knapsack (Combinations)   | O(N * A) | O(A)     |
 | 20 | Partition Equal Subset Sum                  | LC 416 | 0/1 Knapsack (Backward Iteration)   | O(N * S) | O(S)     |
 | 21 | Target Sum                                  | LC 494 | Subset Sum Offset Reduction         | O(N * S) | O(S)     |
 | 22 | 0/1 Knapsack (Foundational Classic)         | Classic| 0/1 Capacity Budget Optimization    | O(N * W) | O(W)     |
 | 23 | Longest Palindromic Substring / Subsequence | LC 5/516| 2D Expanding Interval DP            | O(N^2)   | O(N)     |
 | 24 | Burst Balloons                              | LC 312 | Interval DP (Last Element Choice)   | O(N^3)   | O(N^2)   |
 | 25 | Matrix Chain Multiplication (MCM)           | Classic| Interval DP Partition Splitting     | O(N^3)   | O(N^2)   |
 | 26 | House Robber III (Tree DP)                  | LC 337 | Postorder Bottom-Up State Pairs     | O(N)     | O(H)     |
 ====================================================================================================
*/


// ====================================================================================================
// ARCHETYPE 1: 1D ARRAY & STATE MACHINE DP
// ====================================================================================================

// =========================================================
// 1. CLIMBING STAIRS / MIN COST CLIMBING STAIRS (LC 70 / LC 746)
// =========================================================

class Solution1 {
public:
    // --- LC 70: Climbing Stairs ---
    // Recurrence: ways(i) = ways(i-1) + ways(i-2)
    
    // Top-Down Memoization: O(N) Time, O(N) Space
    int climbStairsMemo(int n, vector<int>& memo) {
        if (n <= 2) return n;
        if (memo[n] != -1) return memo[n];
        return memo[n] = climbStairsMemo(n - 1, memo) + climbStairsMemo(n - 2, memo);
    }
    int climbStairsTD(int n) {
        vector<int> memo(n + 1, -1);
        return climbStairsMemo(n, memo);
    }

    // Space-Optimized Tabulation: O(N) Time, O(1) Space
    int climbStairs(int n) {
        if (n <= 2) return n;
        int prev2 = 1, prev1 = 2;
        for (int i = 3; i <= n; ++i) {
            int curr = prev1 + prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    // --- LC 746: Min Cost Climbing Stairs ---
    // Recurrence: dp[i] = cost[i] + min(dp[i-1], dp[i-2])
    // Top step can be reached from n-1 or n-2.
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        int prev2 = 0; // min cost to reach step 0 (can start here for 0 cost)
        int prev1 = 0; // min cost to reach step 1 (can start here for 0 cost)
        
        for (int i = 2; i <= n; ++i) {
            int curr = min(prev1 + cost[i - 1], prev2 + cost[i - 2]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
// Interview Explanation:
// - Problem Statement: Reach step n taking 1 or 2 steps at a time (LC 70 count paths, LC 746 min cost).
// - Core Pattern: 1D Fibonacci recurrence.
// - Intuition & Derivation:
//   * Any step i can only be reached from step i-1 (taking 1 step) or step i-2 (taking 2 steps).
//   * Total ways to reach step i is the sum of ways to reach i-1 and i-2 (Addition rule of disjoint paths).
// - Complexity Progression:
//   * Naive Recursion: O(2^N) Time, O(N) Call stack.
//   * Top-Down Memo:   O(N) Time, O(N) State table + O(N) Stack.
//   * Bottom-Up Table: O(N) Time, O(N) Vector space.
//   * Space-Optimized: O(N) Time, O(1) Auxiliary space (only 2 previous states needed).
// - Edge Cases: n = 1, n = 2.
// - Likely Follow-ups:
//   1. What if we can take up to K steps? -> Generalized to `dp[i] = sum(dp[i-j]) for j in 1..k` in O(N*K) or O(N) sliding window.
//   2. Can we solve in O(log N) time? -> Matrix Exponentiation: [[1, 1], [1, 0]]^(N-1).


// =========================================================
// 2. HOUSE ROBBER I & II (LC 198 / LC 213)
// =========================================================

class Solution2 {
public:
    // --- LC 198: House Robber I (Linear) ---
    // Recurrence: rob[i] = max(rob[i-1], nums[i] + rob[i-2])
    int rob1(vector<int>& nums) {
        int prev2 = 0; // max money if robbing up to i-2
        int prev1 = 0; // max money if robbing up to i-1
        
        for (int num : nums) {
            // Choice: don't rob current (prev1) OR rob current + best from i-2 (num + prev2)
            int curr = max(prev1, num + prev2);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    // --- LC 213: House Robber II (Circular) ---
    // Core Idea: Since house 0 and house n-1 are neighbors, we cannot rob both.
    // Decouple circle into two independent linear subproblems:
    // Subproblem A: Houses [0 ... n-2] (Exclude last)
    // Subproblem B: Houses [1 ... n-1] (Exclude first)
    int robLinear(const vector<int>& nums, int start, int end) {
        int prev2 = 0, prev1 = 0;
        for (int i = start; i <= end; ++i) {
            int curr = max(prev1, nums[i] + prev2);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    int rob2(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }
};
// Interview Explanation:
// - Problem Statement: Maximize stolen wealth without robbing adjacent houses; LC 213 wraps first and last houses into a circle.
// - Core Pattern: Pick vs Skip 1D DP + Cycle Breaking via Range Partitioning.
// - Intuition & Derivation:
//   * At house i: if you rob it, you cannot rob i-1, gaining `nums[i] + dp[i-2]`. If you skip it, you keep `dp[i-1]`.
//   * In a circular street, house 0 and house n-1 cannot both be robbed. Run linear robber twice: once skipping house n-1, once skipping house 0.
// - Complexity Progression:
//   * Memoized: O(N) Time, O(N) Space.
//   * Tabulated: O(N) Time, O(N) Space.
//   * Space-Optimized: O(N) Time, O(1) Space.
// - Edge Cases: Single house (n = 1: return nums[0]), two houses (return max(nums[0], nums[1])), all zeros.
// - Likely Follow-ups:
//   1. How to print the houses chosen? -> Maintain a boolean choice table `took[i]` and trace backwards from n.
//   2. What if houses are arranged as a binary tree? -> Tree DP (House Robber III, LC 337).


// =========================================================
// 3. STOCK WITH COOLDOWN / TRANSACTION FEE (LC 309 / LC 714)
// =========================================================

class Solution3 {
public:
    // --- LC 309: Stock with Cooldown ---
    // States:
    // held: Currently holding a share of stock
    // sold: Just sold today (must cooldown tomorrow)
    // reset: Not holding, free to buy
    int maxProfitCooldown(vector<int>& prices) {
        if (prices.empty()) return 0;
        int held = -prices[0];
        int sold = 0;
        int reset = 0;
        
        for (int i = 1; i < (int)prices.size(); ++i) {
            int prevHeld = held;
            int prevSold = sold;
            int prevReset = reset;
            
            // held: Keep holding OR buy today (must have been in reset state yesterday)
            held = max(prevHeld, prevReset - prices[i]);
            // sold: Must sell from held state
            sold = prevHeld + prices[i];
            // reset: Keep resting OR transition from yesterday's cooldown (sold)
            reset = max(prevReset, prevSold);
        }
        return max(sold, reset);
    }

    // --- LC 714: Stock with Transaction Fee ---
    // Two states: hold and cash
    int maxProfitFee(vector<int>& prices, int fee) {
        int hold = -prices[0]; // Buying incurs stock cost
        int cash = 0;          // Selling pays fee
        
        for (int i = 1; i < (int)prices.size(); ++i) {
            hold = max(hold, cash - prices[i]);          // Hold existing or buy today
            cash = max(cash, hold + prices[i] - fee);    // Keep cash or sell today minus fee
        }
        return cash;
    }
};
// Interview Explanation:
// - Problem Statement: Maximize stock profit with 1-day cooldown after selling (LC 309) or fixed fee per transaction (LC 714).
// - Core Pattern: Finite State Machine DP.
// - Intuition & Derivation:
//   * Model every day as transitions between discrete states.
//   * In Cooldown: `held` -> `sold` -> `reset` -> `held`. The forced cooldown is enforced by only buying from `reset`.
//   * In Fee: Pay fee either on buy or sell (consistently pick sell).
// - Complexity Progression:
//   * Tabulated 2D DP `dp[n][3]`: O(N) Time, O(N) Space.
//   * State Machine Compressed:   O(N) Time, O(1) Auxiliary Space.
// - Edge Cases: Empty prices, monotonically decreasing prices (no trades made, profit = 0), fee > price.
// - Likely Follow-ups:
//   1. What if at most K transactions are allowed (LC 188)? -> 2D DP `dp[k][2]` tracking buy/sell for each transaction count.
//   2. How to reconstruct exact transaction days? -> Record state transitions in an array and backtrack from final max state.


// =========================================================
// 4. DECODE WAYS (LC 91)
// =========================================================

class Solution4 {
public:
    int numDecodings(string s) {
        int n = s.size();
        if (n == 0 || s[0] == '0') return 0;
        
        // prev2 corresponds to dp[i-2], prev1 corresponds to dp[i-1]
        int prev2 = 1; // dp[0] (empty string base case)
        int prev1 = 1; // dp[1] (first character is valid != '0')
        
        for (int i = 2; i <= n; ++i) {
            int curr = 0;
            
            // Single-digit branch: s[i-1] in '1'..'9'
            if (s[i - 1] != '0') {
                curr += prev1;
            }
            
            // Two-digit branch: s[i-2..i-1] in "10".."26"
            int twoDigit = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
            if (twoDigit >= 10 && twoDigit <= 26) {
                curr += prev2;
            }
            
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
// Interview Explanation:
// - Problem Statement: Count ways to decode a digit string where 'A'->1 ... 'Z'->26 (LC 91).
// - Core Pattern: 1D Prefix Branching DP (similar to Fibonacci with conditional validity).
// - Intuition & Derivation:
//   * `dp[i]` = ways to decode prefix `s[0...i-1]`.
//   * Can transition from `i-1` if `s[i-1] != '0'`.
//   * Can transition from `i-2` if `10 <= stoi(s[i-2..i-1]) <= 26`.
// - Complexity Progression:
//   * Memoized: O(N) Time, O(N) Space.
//   * Tabulated: O(N) Time, O(N) Space.
//   * Space-Optimized: O(N) Time, O(1) Space.
// - Edge Cases: Leading zero ("06" -> 0), consecutive invalid zeros ("100" -> 0), valid trailing zeros ("20" -> 1).
// - Likely Follow-ups:
//   1. What if '*' matches any digit 1-9 (LC 639 Decode Ways II)? -> Multiply branch transitions by 9 or count of matching combos.
//   2. How to print all decoded strings? -> Backtracking DFS (output size could be exponential).


// =========================================================
// 5. MAXIMUM SUBARRAY (LC 53 - KADANE'S ALGORITHM)
// =========================================================

class Solution5 {
public:
    int maxSubArray(vector<int>& nums) {
        int currentSum = nums[0];
        int maxSum = nums[0];
        
        for (size_t i = 1; i < nums.size(); ++i) {
            // Choice: extend previous subarray OR start fresh from nums[i]
            currentSum = max(nums[i], currentSum + nums[i]);
            maxSum = max(maxSum, currentSum);
        }
        return maxSum;
    }

    // Follow-up: Reconstructing the Subarray (Start and End Indices)
    pair<int, pair<int, int>> maxSubArrayWithIndices(const vector<int>& nums) {
        int currentSum = nums[0], maxSum = nums[0];
        int start = 0, end = 0, tempStart = 0;
        
        for (int i = 1; i < (int)nums.size(); ++i) {
            if (nums[i] > currentSum + nums[i]) {
                currentSum = nums[i];
                tempStart = i; // Reset start index
            } else {
                currentSum += nums[i];
            }
            if (currentSum > maxSum) {
                maxSum = currentSum;
                start = tempStart;
                end = i;
            }
        }
        return {maxSum, {start, end}};
    }
};
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with largest sum (LC 53).
// - Core Pattern: Kadane's Local vs Global Maximum Subarray DP.
// - Intuition & Derivation:
//   * Let `dp[i]` be max sum of subarray ending strictly at index i.
//   * `dp[i] = max(nums[i], dp[i-1] + nums[i])`. If `dp[i-1] < 0`, it hurts to include it; start fresh.
// - Complexity Progression:
//   * Brute Force:     O(N^2) Time, O(1) Space.
//   * 1D Tabulated:    O(N) Time, O(N) Space.
//   * Space-Optimized: O(N) Time, O(1) Space.
// - Edge Cases: All negative numbers (must return largest single negative number, e.g. [-1] -> -1), single element.
// - Likely Follow-ups:
//   1. Now print the actual subarray indices? -> Track `tempStart` when resetting, update `start` and `end` on new global max.
//   2. What if array is circular (LC 918)? -> Max is `max(Kadane(nums), totalSum - MinKadane(nums))` (unless all negative).


// ====================================================================================================
// ARCHETYPE 2: LONGEST INCREASING SUBSEQUENCE (LIS)
// ====================================================================================================

// =========================================================
// 6. LONGEST INCREASING SUBSEQUENCE (LC 300)
// =========================================================

class Solution6 {
public:
    // Approach 1: Classic DP Tabulation — O(N^2) Time, O(N) Space
    int lengthOfLIS_DP(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, 1); // dp[i] = length of LIS ending at index i
        int maxLen = 1;
        
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    dp[i] = max(dp[i], 1 + dp[j]);
                }
            }
            maxLen = max(maxLen, dp[i]);
        }
        return maxLen;
    }

    // Approach 2: Patience Sorting with Binary Search — O(N log N) Time, O(N) Space
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails; // tails[k] stores smallest tail of all increasing subsequences of length k+1
        
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) {
                tails.push_back(x); // x is greater than all tails; extend LIS
            } else {
                *it = x; // Overwrite to maintain smallest possible tail value
            }
        }
        return tails.size();
    }

    // Follow-up: Reconstructing the actual LIS sequence in O(N log N)
    vector<int> printLIS(vector<int>& nums) {
        int n = nums.size();
        vector<int> tails;
        vector<int> tailIndices;
        vector<int> parent(n, -1);
        
        for (int i = 0; i < n; ++i) {
            int x = nums[i];
            auto it = lower_bound(tails.begin(), tails.end(), x);
            int idx = it - tails.begin();
            
            if (it == tails.end()) {
                tails.push_back(x);
                tailIndices.push_back(i);
            } else {
                *it = x;
                tailIndices[idx] = i;
            }
            
            if (idx > 0) {
                parent[i] = tailIndices[idx - 1]; // Link to predecessor in LIS
            }
        }
        
        // Reconstruct from last element
        vector<int> lis;
        int curr = tailIndices.back();
        while (curr != -1) {
            lis.push_back(nums[curr]);
            curr = parent[curr];
        }
        reverse(lis.begin(), lis.end());
        return lis;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest strictly increasing subsequence (LC 300).
// - Core Pattern: Patience Sorting + Binary Search (std::lower_bound).
// - Intuition & Derivation:
//   * O(N^2): `dp[i] = 1 + max(dp[j])` for all `j < i` where `nums[j] < nums[i]`.
//   * O(N log N): Maintain `tails` array. `tails[len]` is the minimal tail of an increasing subsequence of length `len+1`.
//   * A smaller tail gives a better future chance to append incoming numbers. `tails` is naturally sorted, allowing binary search.
// - Complexity Progression:
//   * Tabulated:       O(N^2) Time, O(N) Space.
//   * Binary Search:   O(N log N) Time, O(N) Space.
// - Edge Cases: Array already strictly sorted, strictly decreasing (length = 1), duplicate elements.
// - Likely Follow-ups:
//   1. How to reconstruct the actual sequence? -> Keep `parent` pointers and backtrack from `tailIndices.back()`.
//   2. What if non-decreasing (<=) is allowed? -> Use `std::upper_bound` instead of `lower_bound`.


// =========================================================
// 7. RUSSIAN DOLL ENVELOPES (LC 354)
// =========================================================

class Solution7 {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        if (envelopes.empty()) return 0;
        
        // CRITICAL SORTING TRICK:
        // 1. Sort width in ASCENDING order.
        // 2. If widths are equal, sort height in DESCENDING order!
        sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] != b[0]) return a[0] < b[0];
            return a[1] > b[1]; // Descending height prevents picking envelopes with same width
        });
        
        // Now find standard LIS on heights in O(N log N)
        vector<int> tails;
        for (const auto& env : envelopes) {
            int h = env[1];
            auto it = lower_bound(tails.begin(), tails.end(), h);
            if (it == tails.end()) {
                tails.push_back(h);
            } else {
                *it = h;
            }
        }
        return tails.size();
    }
};
// Interview Explanation:
// - Problem Statement: Max nested envelopes [w, h]; an envelope fits in another only if both w and h are strictly greater (LC 354).
// - Core Pattern: 2D Geometry Sorting Trick reduced to 1D LIS.
// - Intuition & Derivation:
//   * If we sort widths ascending, we guarantee `w_i <= w_j`.
//   * But if two envelopes have the SAME width, sorting heights DESCENDING ensures `lower_bound` will NEVER pick two envelopes with identical width!
//   * Problem reduces purely to finding 1D LIS on heights in O(N log N).
// - Complexity:
//   * Time: O(N log N) — Sorting takes O(N log N), LIS takes O(N log N).
//   * Space: O(N) — Storage for tails.
// - Edge Cases: Envelopes with identical dimensions, single envelope, all same width.
// - Likely Follow-ups:
//   1. Why descending order on tie? -> If sorted ascending, [3, 4] and [3, 5] would both be picked, violating strict nesting.
//   2. What if 3D boxes [w, h, d] (Box Stacking)? -> Cannot reduce to 1D LIS; requires DAG longest path or 2D Segment Tree / CDQ Divide-and-Conquer.


// =========================================================
// 8. WORD BREAK (LC 139)
// =========================================================

class Solution8 {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();
        
        // Find max word length to prune inner loop search space
        int maxLen = 0;
        for (const string& w : wordDict) {
            maxLen = max(maxLen, (int)w.size());
        }
        
        // dp[i] = true if s[0...i-1] can be segmented into dictionary words
        vector<bool> dp(n + 1, false);
        dp[0] = true; // Base case: empty prefix is valid
        
        for (int i = 1; i <= n; ++i) {
            // Only look back up to maxLen characters
            for (int j = i - 1; j >= max(0, i - maxLen); --j) {
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break; // Early exit once proven valid
                }
            }
        }
        return dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Determine if string s can be segmented into words from wordDict (LC 139).
// - Core Pattern: 1D Substring Partitioning DP with dictionary pruning.
// - Intuition & Derivation:
//   * `dp[i]` is true if there exists some `j < i` such that `dp[j] == true` AND `s[j...i-1] \in dict`.
//   * Pruning: Only check `j >= i - maxWordLength` instead of all `j < i`.
// - Complexity Progression:
//   * Naive Recursion: O(2^N) Time.
//   * Tabulated:       O(N * L^2) Time where L is max word length in dict (string hashing / substr).
//   * Space:           O(N) table + O(Dict) hash set.
// - Edge Cases: Entire string matches a single word, repeated single-character words ("aaaa"), empty string.
// - Likely Follow-ups:
//   1. How to return all valid sentences (LC 140 Word Break II)? -> Memoized DFS with path collection / backtracking.
//   2. Can we optimize substring hashing? -> Use a Trie or Aho-Corasick automaton to match prefixes in O(N * L).


// ====================================================================================================
// ARCHETYPE 3: TWO-SEQUENCE & STRING MATCHING DP
// ====================================================================================================

// =========================================================
// 9. LONGEST COMMON SUBSEQUENCE (LC 1143)
// =========================================================

class Solution9 {
public:
    // Tabulation with O(min(M, N)) Space Optimization
    int longestCommonSubsequence(string text1, string text2) {
        if (text1.size() < text2.size()) swap(text1, text2); // Ensure text2 is smaller for O(min) space
        int m = text1.size(), n = text2.size();
        
        vector<int> dp(n + 1, 0);
        
        for (int i = 1; i <= m; ++i) {
            int prevDiag = 0; // Represents dp[i-1][j-1]
            for (int j = 1; j <= n; ++j) {
                int temp = dp[j]; // Save dp[i-1][j] before overwriting
                if (text1[i - 1] == text2[j - 1]) {
                    dp[j] = 1 + prevDiag;
                } else {
                    dp[j] = max(dp[j], dp[j - 1]);
                }
                prevDiag = temp;
            }
        }
        return dp[n];
    }

    // Follow-up: Reconstructing the actual LCS String
    string printLCS(const string& s1, const string& s2) {
        int m = s1.size(), n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        
        string lcs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (s1[i - 1] == s2[j - 1]) {
                lcs.push_back(s1[i - 1]);
                i--; j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(lcs.begin(), lcs.end());
        return lcs;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest common subsequence between two strings (LC 1143).
// - Core Pattern: 2-Sequence Grid Matching DP.
// - Intuition & Derivation:
//   * If `text1[i-1] == text2[j-1]`: characters match, `dp[i][j] = 1 + dp[i-1][j-1]`.
//   * If mismatch: either drop character from text1 or text2: `max(dp[i-1][j], dp[i][j-1])`.
// - Complexity Progression:
//   * 2D Tabulated:       O(M * N) Time, O(M * N) Space.
//   * 1D Space-Optimized: O(M * N) Time, O(min(M, N)) Space using single row + prevDiag.
// - Edge Cases: Strings share no characters (0), identical strings, single-char strings.
// - Likely Follow-ups:
//   1. Print the LCS string? -> Backtrack from `dp[m][n]` moving diagonally on match, or toward larger adjacent cell.
//   2. Shortest Common Supersequence (LC 1092)? -> Length is `M + N - LCS`.


// =========================================================
// 10. EDIT DISTANCE (LC 72)
// =========================================================

class Solution10 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        // dp[j] stores edit distance between word1[0...i-1] and word2[0...j-1]
        vector<int> dp(n + 1);
        
        // Base case: transforming empty word1 to word2[0...j-1] requires j insertions
        for (int j = 0; j <= n; ++j) dp[j] = j;
        
        for (int i = 1; i <= m; ++i) {
            int prevDiag = dp[0]; // Stores dp[i-1][j-1]
            dp[0] = i;            // Base case: transforming word1[0...i-1] to empty word2 requires i deletions
            
            for (int j = 1; j <= n; ++j) {
                int temp = dp[j];
                if (word1[i - 1] == word2[j - 1]) {
                    dp[j] = prevDiag; // No operation needed
                } else {
                    // 1 + min({Insert, Delete, Replace})
                    dp[j] = 1 + min({dp[j],       // Delete from word1 (dp[i-1][j])
                                     dp[j - 1],   // Insert into word1 (dp[i][j-1])
                                     prevDiag});  // Replace character (dp[i-1][j-1])
                }
                prevDiag = temp;
            }
        }
        return dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Min operations (insert, delete, replace) to convert word1 to word2 (LC 72 - Levenshtein Distance).
// - Core Pattern: 2-Sequence 3-Way Choice DP.
// - Intuition & Derivation:
//   * If `word1[i-1] == word2[j-1]`: cost is `dp[i-1][j-1]`.
//   * Otherwise, try all 3 operations and take minimum:
//     - Insert: `1 + dp[i][j-1]`
//     - Delete: `1 + dp[i-1][j]`
//     - Replace: `1 + dp[i-1][j-1]`
// - Complexity Progression:
//   * 2D Tabulated: O(M * N) Time, O(M * N) Space.
//   * 1D Rolling:   O(M * N) Time, O(min(M, N)) Space.
// - Edge Cases: One or both strings empty, identical strings.
// - Likely Follow-ups:
//   1. What if operations have different costs? -> Replace `1 +` with `costInsert`, `costDelete`, `costReplace`.
//   2. How to print the exact edit transcript? -> Trace 2D parent pointers outputting "Insert 'x'", "Delete 'y'".


// =========================================================
// 11. DISTINCT SUBSEQUENCES (LC 115)
// =========================================================

class Solution11 {
public:
    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();
        // dp[j] = number of distinct subsequences of s[0...i-1] matching t[0...j-1]
        // Use unsigned long long or double to prevent 32-bit integer overflow on large counts
        vector<unsigned long long> dp(n + 1, 0);
        dp[0] = 1; // Empty t is matched by exactly 1 subsequence (the empty one)
        
        for (int i = 1; i <= m; ++i) {
            // Traverse backwards to reuse dp array from previous row without overwrite
            for (int j = n; j >= 1; --j) {
                if (s[i - 1] == t[j - 1]) {
                    // Choice 1: Do not use s[i-1] (dp[j])
                    // Choice 2: Use s[i-1] to match t[j-1] (dp[j-1])
                    dp[j] += dp[j - 1];
                }
            }
        }
        return (int)dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Count distinct subsequences of s that equal t (LC 115).
// - Core Pattern: 2-Sequence Subsequence Counting DP with backward 1D optimization.
// - Intuition & Derivation:
//   * `dp[i][j]` = subsequences of `s[0...i-1]` matching `t[0...j-1]`.
//   * We always have the option to skip `s[i-1]`, giving `dp[i-1][j]`.
//   * If `s[i-1] == t[j-1]`, we can also use `s[i-1]`, adding `dp[i-1][j-1]` ways.
// - Complexity Progression:
//   * Tabulated 2D: O(M * N) Time, O(M * N) Space.
//   * 1D Backward:  O(M * N) Time, O(N) Space.
// - Edge Cases: t longer than s (0), s == t (1), duplicate characters causing large combinatorial numbers.
// - Likely Follow-ups:
//   1. Why iterate j backwards? -> Same as 0/1 knapsack: ensures `dp[j-1]` represents the value from row `i-1`.
//   2. How to handle 64-bit overflow? -> Use `unsigned long long` or modulo arithmetic depending on interviewer spec.


// =========================================================
// 12. WILDCARD MATCHING (LC 44)
// =========================================================

class Solution12 {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        // dp[i][j] = s[0...i-1] matches p[0...j-1]
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        
        // Base case: pattern matching empty string (e.g. "***")
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') dp[j] = dp[j - 1];
        }
        
        for (int i = 1; i <= m; ++i) {
            bool prevDiag = dp[0];
            dp[0] = false; // Non-empty s cannot match empty p
            
            for (int j = 1; j <= n; ++j) {
                bool temp = dp[j];
                if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    dp[j] = prevDiag;
                } else if (p[j - 1] == '*') {
                    // '*' matches empty sequence (dp[j-1]) OR matches 1+ characters (dp[j])
                    dp[j] = dp[j - 1] || dp[j];
                } else {
                    dp[j] = false;
                }
                prevDiag = temp;
            }
        }
        return dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Match string s against pattern p where '?' matches single char, '*' matches any sequence of chars (LC 44).
// - Core Pattern: 2D Grid Pattern Matching DP.
// - Intuition & Derivation:
//   * If `p[j-1] == '?' || p[j-1] == s[i-1]`: direct match, take diagonal `dp[i-1][j-1]`.
//   * If `p[j-1] == '*'`: two choices:
//     1. Match 0 characters: `dp[i][j-1]` (ignore `*`).
//     2. Match 1 or more characters: `dp[i-1][j]` (consume `s[i-1]` while keeping `*` active).
// - Complexity:
//   * Time: O(M * N) Time.
//   * Space: O(N) Space with 1D rolling array.
// - Edge Cases: Pattern is pure "*****", s is empty, pattern empty but s non-empty.
// - Likely Follow-ups:
//   1. Can you solve in O(1) space? -> Yes! Two-pointer greedy backtracking with `lastStar` and `match` pointers in O(M*N) worst, O(M+N) average.


// =========================================================
// 13. REGULAR EXPRESSION MATCHING (LC 10)
// =========================================================

class Solution13 {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;
        
        // Base case: empty s matches patterns like "a*", "a*b*", ".*"
        for (int j = 2; j <= n; j += 2) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (p[j - 1] == '*') {
                    // Choice 1: '*' matches 0 instances of preceding character p[j-2]
                    dp[i][j] = dp[i][j - 2];
                    
                    // Choice 2: '*' matches 1 or more if preceding char matches s[i-1]
                    char prevChar = p[j - 2];
                    if (prevChar == '.' || prevChar == s[i - 1]) {
                        dp[i][j] = dp[i][j] || dp[i - 1][j];
                    }
                } else {
                    // Regular character or '.'
                    if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                        dp[i][j] = dp[i - 1][j - 1];
                    }
                }
            }
        }
        return dp[m][n];
    }
};
// Interview Explanation:
// - Problem Statement: Match string s with pattern p supporting '.' (any char) and '*' (zero or more of preceding element) (LC 10).
// - Core Pattern: 2D Lookback String Matching DP.
// - Intuition & Derivation:
//   * In Regex, `*` never appears alone; it acts as a modifier on `p[j-2]`.
//   * When `p[j-1] == '*'`:
//     - 0 instances: look back 2 columns `dp[i][j-2]`.
//     - 1+ instances: if `p[j-2]` matches `s[i-1]`, consume `s[i-1]` by checking `dp[i-1][j]`.
// - Complexity:
//   * Time: O(M * N) — Every grid cell evaluated in O(1).
//   * Space: O(M * N) Space (can be compressed to O(N)).
// - Edge Cases: Pattern starts with invalid '*', chained patterns like "a*b*c*", empty string with ".*".
// - Likely Follow-ups:
//   1. How does this differ from LC 44 Wildcard? -> In Wildcard, '*' stands alone. In Regex, '*' modifies preceding element.
//   2. Can you do top-down memoization? -> Yes: `unordered_map<string, bool>` or `memo[i][j]` with -1/0/1 flags.


// ====================================================================================================
// ARCHETYPE 4: 2D GRID / MATRIX PATH DP
// ====================================================================================================

// =========================================================
// 14. UNIQUE PATHS I & II (LC 62 / LC 63)
// =========================================================

class Solution14 {
public:
    // --- LC 62: Unique Paths I ---
    // Recurrence: dp[j] = dp[j] (from top) + dp[j-1] (from left)
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1); // 1 way to reach any cell in the first row
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                dp[j] += dp[j - 1]; // dp[j] is top, dp[j-1] is left
            }
        }
        return dp[n - 1];
    }

    // --- LC 63: Unique Paths II (With Obstacles) ---
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1) return 0;
        
        vector<long long> dp(n, 0);
        dp[0] = 1;
        
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (obstacleGrid[i][j] == 1) {
                    dp[j] = 0; // Blocked cell contributes 0 paths
                } else if (j > 0) {
                    dp[j] += dp[j - 1]; // Add paths coming from the left
                }
            }
        }
        return (int)dp[n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Find unique paths from (0, 0) to (m-1, n-1) moving only Down or Right; LC 63 includes obstacles.
// - Core Pattern: 2D Grid Path Additive DP with 1D row compression.
// - Intuition & Derivation:
//   * `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
//   * A single 1D array `dp[j]` holds the value from the row above; adding `dp[j-1]` incorporates paths from the left.
// - Complexity Progression:
//   * Combinatorics (LC 62): O(min(M, N)) Time via C(m+n-2, m-1).
//   * 2D DP Table:           O(M * N) Time, O(M * N) Space.
//   * 1D Space-Optimized:    O(M * N) Time, O(N) Space.
// - Edge Cases: Start or end cell contains obstacle (returns 0), 1x1 grid, 1xN grid with obstacle.
// - Likely Follow-ups:
//   1. Can you solve LC 62 in O(1) space? -> Yes, mathematically: `C(m + n - 2, m - 1)`.
//   2. What if paths can move in 4 directions? -> Becomes Shortest Path in Graph (BFS / Dijkstra), not DAG DP.


// =========================================================
// 15. MINIMUM PATH SUM (LC 64)
// =========================================================

class Solution15 {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<int> dp(n, 0);
        
        // Base case: first row
        dp[0] = grid[0][0];
        for (int j = 1; j < n; ++j) {
            dp[j] = dp[j - 1] + grid[0][j];
        }
        
        for (int i = 1; i < m; ++i) {
            dp[0] += grid[i][0]; // First column can only come from directly above
            for (int j = 1; j < n; ++j) {
                // Min from top (dp[j]) or left (dp[j-1])
                dp[j] = grid[i][j] + min(dp[j], dp[j - 1]);
            }
        }
        return dp[n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Find path from top-left to bottom-right minimizing sum of numbers along path (LC 64).
// - Core Pattern: 2D Grid Minimization DP with 1D Space Optimization.
// - Intuition & Derivation:
//   * Cell (i, j) can only be entered from (i-1, j) or (i, j-1).
//   * `dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])`.
// - Complexity Progression:
//   * 2D Grid In-Place: O(M * N) Time, O(1) Extra Space (mutating input).
//   * 1D Vector DP:     O(M * N) Time, O(N) Auxiliary Space (without mutating input).
// - Edge Cases: Single row, single column, grid with zeros.
// - Likely Follow-ups:
//   1. Can you solve in O(1) space? -> Overwrite `grid[i][j]` directly if input mutation is permitted.
//   2. How to print the path? -> Track whether step came from top or left and trace back.


// =========================================================
// 16. MAXIMAL SQUARE (LC 221)
// =========================================================

class Solution16 {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int m = matrix.size(), n = matrix[0].size();
        vector<int> dp(n + 1, 0);
        int maxSide = 0;
        int prevDiag = 0;
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                int temp = dp[j];
                if (matrix[i - 1][j - 1] == '1') {
                    // Square side is 1 + min of top, left, and top-left diagonal
                    dp[j] = 1 + min({dp[j], dp[j - 1], prevDiag});
                    maxSide = max(maxSide, dp[j]);
                } else {
                    dp[j] = 0;
                }
                prevDiag = temp;
            }
        }
        return maxSide * maxSide; // Return area
    }
};
// Interview Explanation:
// - Problem Statement: Find largest square containing only 1's and return its area (LC 221).
// - Core Pattern: 2D Geometry Local Minimum Extension DP.
// - Intuition & Derivation:
//   * Let `dp[i][j]` be the side length of the largest square whose bottom-right corner is at `(i, j)`.
//   * For a square of side `s` to exist at `(i, j)`, the top, left, and diagonal neighbors must all support a square of side `s - 1`.
//   * Recurrence: `dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]})`.
// - Complexity Progression:
//   * 2D Tabulated: O(M * N) Time, O(M * N) Space.
//   * 1D Rolling:   O(M * N) Time, O(N) Space using `prevDiag`.
// - Edge Cases: No '1's in matrix (area = 0), matrix where entire grid is '1'.
// - Likely Follow-ups:
//   1. What if largest RECTANGLE is requested (LC 85)? -> Monotonic stack on histogram heights in O(M * N) time.
//   2. How to count total number of square submatrices (LC 1277)? -> Sum all `dp[i][j]` values across the grid.


// =========================================================
// 17. DUNGEON GAME (LC 174)
// =========================================================

class Solution17 {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int m = dungeon.size(), n = dungeon[0].size();
        // dp[j] stores min HP needed when entering cell (i, j)
        vector<int> dp(n + 1, INT_MAX);
        
        // Base cases: Princess cell needs at least 1 HP upon exit
        dp[n - 1] = 1;
        dp[n] = 1;
        
        // Bottom-up reverse traversal: from bottom-right back to top-left
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                // Min HP required after leaving cell (i, j)
                int minHpNext = min(dp[j], dp[j + 1]);
                if (i == m - 1 && j == n - 1) minHpNext = 1;
                
                // HP needed entering cell (i, j): must be at least 1
                dp[j] = max(1, minHpNext - dungeon[i][j]);
            }
            dp[n] = INT_MAX; // Reset boundary for subsequent rows
        }
        return dp[0];
    }
};
// Interview Explanation:
// - Problem Statement: Find knight's minimum initial health to reach bottom-right room without health dropping <= 0 (LC 174).
// - Core Pattern: 2D Bottom-Up Reverse Survival DP.
// - Intuition & Derivation:
//   * Forward DP fails because optimal choice depends on future path requirements, not past accumulated health.
//   * Start at princess cell (m-1, n-1) with requirement: must exit with at least 1 HP.
//   * Working backwards: `hpNeeded = min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j]`.
//   * Since knight must always remain alive: `dp[i][j] = max(1, hpNeeded)`.
// - Complexity:
//   * Time: O(M * N) — Single reverse pass.
//   * Space: O(N) — 1D row array.
// - Edge Cases: All positive rooms (knight needs 1 HP), all negative rooms, single-cell dungeon.
// - Likely Follow-ups:
//   1. Why does forward DP fail? -> State requires tracking both "min health seen so far" and "current health"; lacks optimal substructure forward.
//   2. Can we solve using Binary Search + BFS? -> Binary search on initial HP in [1, 10^9] + reachability check in O(M*N log(Range)).


// ====================================================================================================
// ARCHETYPE 5: KNAPSACK & SUBSET SUM ARCHETYPES
// ====================================================================================================

// =========================================================
// 18. COIN CHANGE - MINIMUM COINS (LC 322)
// =========================================================

class Solution18 {
public:
    int coinChange(vector<int>& coins, int amount) {
        // dp[i] = minimum coins needed to make amount i
        // Initialize with amount + 1 (acts as infinity)
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0; // Base case: 0 coins to make amount 0
        
        for (int coin : coins) {
            // Forward iteration because coins can be reused unlimited times (Unbounded Knapsack)
            for (int a = coin; a <= amount; ++a) {
                dp[a] = min(dp[a], 1 + dp[a - coin]);
            }
        }
        return dp[amount] > amount ? -1 : dp[amount];
    }

    // Follow-up: Print the actual coins chosen
    vector<int> printCoins(const vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        vector<int> lastCoin(amount + 1, -1);
        dp[0] = 0;
        
        for (int coin : coins) {
            for (int a = coin; a <= amount; ++a) {
                if (1 + dp[a - coin] < dp[a]) {
                    dp[a] = 1 + dp[a - coin];
                    lastCoin[a] = coin;
                }
            }
        }
        if (dp[amount] > amount) return {};
        
        vector<int> result;
        int curr = amount;
        while (curr > 0) {
            result.push_back(lastCoin[curr]);
            curr -= lastCoin[curr];
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return fewest number of coins needed to make up amount; return -1 if impossible (LC 322).
// - Core Pattern: Unbounded Knapsack (Minimization).
// - Intuition & Derivation:
//   * `dp[a] = min(dp[a], 1 + dp[a - coin])`.
//   * Forward iteration `a = coin...amount` allows a coin to be used multiple times.
// - Complexity Progression:
//   * Naive Recursion: O(Coins^Amount) Time.
//   * Memoized/Table:  O(N * Amount) Time, O(Amount) Space.
// - Edge Cases: amount = 0 (returns 0), amount cannot be formed (returns -1), coin > amount.
// - Likely Follow-ups:
//   1. Print the coins used? -> Maintain `lastCoin[a]` parent array and trace back.
//   2. Can you solve with BFS? -> Yes: treat amount as graph node, coins as edges; BFS finds shortest path in O(N * Amount).


// =========================================================
// 19. COIN CHANGE II - TOTAL COMBINATIONS (LC 518)
// =========================================================

class Solution19 {
public:
    // Combinations: Order DOES NOT matter ([1, 2] == [2, 1])
    // OUTER LOOP = COINS, INNER LOOP = AMOUNT
    int change(int amount, vector<int>& coins) {
        vector<unsigned long long> dp(amount + 1, 0);
        dp[0] = 1; // 1 way to make 0 amount
        
        for (int coin : coins) {
            for (int a = coin; a <= amount; ++a) {
                dp[a] += dp[a - coin];
            }
        }
        return (int)dp[amount];
    }

    // Permutations: Order DOES matter ([1, 2] != [2, 1]) -> LC 377 Combination Sum IV
    // OUTER LOOP = AMOUNT, INNER LOOP = COINS
    int combinationSum4(vector<int>& nums, int target) {
        vector<unsigned int> dp(target + 1, 0);
        dp[0] = 1;
        
        for (int a = 1; a <= target; ++a) {
            for (int num : nums) {
                if (a >= num) {
                    dp[a] += dp[a - num];
                }
            }
        }
        return dp[target];
    }
};
// Interview Explanation:
// - Problem Statement: Return number of combinations that make up amount (LC 518).
// - Core Pattern: Combinations vs Permutations Loop Ordering in Unbounded Knapsack.
// - Intuition & Derivation:
//   * COMBINATIONS (LC 518): Outer loop = Coins. We consider coin 1 fully, then coin 2, etc. Never generates [2, 1] after [1, 2].
//   * PERMUTATIONS (LC 377): Outer loop = Amount. At each amount, every coin is tried, generating all orderings.
// - Complexity:
//   * Time: O(Coins * Amount).
//   * Space: O(Amount) 1D vector.
// - Edge Cases: amount = 0 (returns 1), no combinations possible (returns 0).
// - Likely Follow-ups:
//   1. What causes the difference between combinations and permutations? -> Loop nesting order (Outer coin vs Outer amount).
//   2. What if coins can only be used ONCE? -> 0/1 Knapsack: inner loop must run BACKWARDS (LC 416 / LC 494).


// =========================================================
// 20. PARTITION EQUAL SUBSET SUM (LC 416)
// =========================================================

class Solution20 {
public:
    bool canPartition(vector<int>& nums) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if (totalSum % 2 != 0) return false; // Odd sum cannot be partitioned equally
        
        int target = totalSum / 2;
        vector<bool> dp(target + 1, false);
        dp[0] = true; // Base case: sum 0 is always achievable
        
        for (int num : nums) {
            // CRITICAL: Traverse BACKWARDS to ensure each number is used AT MOST ONCE
            for (int s = target; s >= num; --s) {
                dp[s] = dp[s] || dp[s - num];
            }
            if (dp[target]) return true; // Early exit
        }
        return dp[target];
    }
};
// Interview Explanation:
// - Problem Statement: Determine if array can be partitioned into two subsets with equal sum (LC 416).
// - Core Pattern: 0/1 Knapsack with Backward Iteration.
// - Intuition & Derivation:
//   * Equal partition means finding a subset summing strictly to `totalSum / 2`.
//   * `dp[s] = dp[s] || dp[s - num]`.
//   * Backward iteration `s = target...num` prevents using the same number multiple times in the same round.
// - Complexity Progression:
//   * 2D DP Table:     O(N * Target) Time, O(N * Target) Space.
//   * 1D Space-Optim:  O(N * Target) Time, O(Target) Space where Target = Sum / 2.
// - Edge Cases: Odd total sum (immediately false), maximum element > totalSum / 2 (false).
// - Likely Follow-ups:
//   1. Why iterate backwards? -> Forward iteration reuses `dp[s - num]` updated in the current iteration (unbounded knapsack behavior).
//   2. Can bitset optimize this? -> Yes! `bitset<10001> bits(1); for (int x : nums) bits |= (bits << x); return bits[target];` — 64x faster!


// =========================================================
// 21. TARGET SUM (LC 494)
// =========================================================

class Solution21 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        // Mathematical reduction:
        // P - N = target  AND  P + N = totalSum
        // Adding: 2P = target + totalSum => P = (target + totalSum) / 2
        if (abs(target) > totalSum || (target + totalSum) % 2 != 0) return 0;
        
        int subsetTarget = (target + totalSum) / 2;
        vector<int> dp(subsetTarget + 1, 0);
        dp[0] = 1;
        
        for (int num : nums) {
            for (int s = subsetTarget; s >= num; --s) {
                dp[s] += dp[s - num];
            }
        }
        return dp[subsetTarget];
    }
};
// Interview Explanation:
// - Problem Statement: Assign '+' or '-' to each integer so that sum evaluates to target; return number of ways (LC 494).
// - Core Pattern: Algebraic Reduction to 0/1 Subset Sum.
// - Intuition & Derivation:
//   * Split nums into positive set P and negative set N.
//   * `sum(P) - sum(N) = target` and `sum(P) + sum(N) = totalSum`.
//   * `2 * sum(P) = target + totalSum` -> `sum(P) = (target + totalSum) / 2`.
//   * Problem is mathematically identical to finding number of subsets summing to `sum(P)`.
// - Complexity:
//   * Time: O(N * TargetSum).
//   * Space: O(TargetSum) 1D array.
// - Edge Cases: target > totalSum (0), (target + totalSum) is odd (0), zeros in nums (must be counted; backward iteration handles them).
// - Likely Follow-ups:
//   1. How do zeros affect the count? -> Each zero doubles ways: `dp[0]` becomes 2 after first zero, 4 after second.
//   2. What if target is negative? -> Symmetrical: `target = abs(target)`.


// =========================================================
// 22. 0/1 KNAPSACK (FOUNDATIONAL CLASSIC)
// =========================================================

class Solution22 {
public:
    int knapsack01(int W, const vector<int>& weights, const vector<int>& values) {
        int n = weights.size();
        // dp[w] stores max value achievable with capacity w
        vector<int> dp(W + 1, 0);
        
        for (int i = 0; i < n; ++i) {
            // Backward iteration ensures each item is evaluated at most once
            for (int w = W; w >= weights[i]; --w) {
                dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
            }
        }
        return dp[W];
    }

    // Follow-up: Item Reconstruction (Which items were chosen?)
    vector<int> knapsack01WithItems(int W, const vector<int>& weights, const vector<int>& values) {
        int n = weights.size();
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
        
        for (int i = 1; i <= n; ++i) {
            for (int w = 0; w <= W; ++w) {
                dp[i][w] = dp[i - 1][w]; // Skip item i-1
                if (w >= weights[i - 1]) {
                    dp[i][w] = max(dp[i][w], values[i - 1] + dp[i - 1][w - weights[i - 1]]);
                }
            }
        }
        
        // Backtrack to find items
        vector<int> chosen;
        int w = W;
        for (int i = n; i >= 1; --i) {
            if (dp[i][w] != dp[i - 1][w]) {
                chosen.push_back(i - 1); // Item i-1 was included
                w -= weights[i - 1];
            }
        }
        reverse(chosen.begin(), chosen.end());
        return chosen;
    }
};
// Interview Explanation:
// - Problem Statement: Given weights, values, and capacity W, maximize value without exceeding capacity; each item used <= 1 time.
// - Core Pattern: 0/1 Knapsack Capacity Budgeting.
// - Intuition & Derivation:
//   * At item i: either leave it (`dp[i-1][w]`) or take it (`values[i] + dp[i-1][w - weights[i]]`).
// - Complexity Progression:
//   * Naive Recursion: O(2^N) Time.
//   * 2D DP Table:     O(N * W) Time, O(N * W) Space.
//   * 1D Space-Optim:  O(N * W) Time, O(W) Space (backward iteration).
// - Edge Cases: Item weight > W, zero capacity W = 0, items with 0 weight.
// - Likely Follow-ups:
//   1. How to reconstruct items? -> Must preserve 2D table or bitmasks to backtrack transitions.
//   2. What if W is very large (e.g. 10^9) but N is small (N <= 40)? -> Meet-in-the-middle binary search in O(2^(N/2)).


// ====================================================================================================
// ARCHETYPE 6: INTERVAL DP & TREE DP
// ====================================================================================================

// =========================================================
// 23. LONGEST PALINDROMIC SUBSTRING / SUBSEQUENCE (LC 5 / LC 516)
// =========================================================

class Solution23 {
public:
    // --- LC 5: Longest Palindromic Substring ---
    // Recurrence: isPalin[i][j] = (s[i] == s[j]) && (j - i <= 2 || isPalin[i+1][j-1])
    string longestPalindrome(string s) {
        int n = s.size();
        if (n <= 1) return s;
        
        int start = 0, maxLen = 1;
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        
        // Every single character is a palindrome
        for (int i = 0; i < n; ++i) dp[i][i] = true;
        
        // Expand interval length from 2 to n
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    if (len <= 3 || dp[i + 1][j - 1]) {
                        dp[i][j] = true;
                        if (len > maxLen) {
                            start = i;
                            maxLen = len;
                        }
                    }
                }
            }
        }
        return s.substr(start, maxLen);
    }

    // --- LC 516: Longest Palindromic Subsequence ---
    // Recurrence:
    // If s[i] == s[j]: dp[i][j] = 2 + dp[i+1][j-1]
    // Else:            dp[i][j] = max(dp[i+1][j], dp[i][j-1])
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<int> dp(n, 0); // 1D space optimized
        
        for (int i = n - 1; i >= 0; --i) {
            dp[i] = 1;
            int prevDiag = 0;
            for (int j = i + 1; j < n; ++j) {
                int temp = dp[j];
                if (s[i] == s[j]) {
                    dp[j] = 2 + prevDiag;
                } else {
                    dp[j] = max(dp[j], dp[j - 1]);
                }
                prevDiag = temp;
            }
        }
        return dp[n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: LC 5 finds longest contiguous palindromic substring; LC 516 finds longest palindromic subsequence.
// - Core Pattern: 2D Expanding Interval DP.
// - Intuition & Derivation:
//   * Substring (LC 5): A substring `s[i...j]` is palindrome if outer chars match AND inner substring `s[i+1...j-1]` is palindrome.
//   * Subsequence (LC 516): If outer match, add 2 to inner; if mismatch, take max skipping either i or j. (Also equivalent to LCS(s, reverse(s))).
// - Complexity:
//   * Substring (LC 5):   O(N^2) Time, O(N^2) Space (can be O(1) space via Expand Around Center).
//   * Subsequence (LC 516): O(N^2) Time, O(N) Space.
// - Edge Cases: Single character, all identical characters ("aaaa"), no palindromes > 1.
// - Likely Follow-ups:
//   1. Can LC 5 be solved in O(N) time? -> Manacher's Algorithm.
//   2. Can LC 516 be solved using LCS? -> Yes: `longestCommonSubsequence(s, string(s.rbegin(), s.rend()))`.


// =========================================================
// 24. BURST BALLOONS (LC 312)
// =========================================================

class Solution24 {
public:
    int maxCoins(vector<int>& nums) {
        // Pad array with boundary 1s: [1, ...nums, 1]
        vector<int> balloons;
        balloons.reserve(nums.size() + 2);
        balloons.push_back(1);
        for (int x : nums) balloons.push_back(x);
        balloons.push_back(1);
        
        int n = balloons.size();
        // dp[i][j] = max coins from bursting all balloons strictly BETWEEN i and j
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // len is the distance between boundaries i and j
        for (int len = 2; len < n; ++len) {
            for (int i = 0; i < n - len; ++i) {
                int j = i + len;
                // Try choosing balloon k as the LAST balloon to burst in (i, j)
                for (int k = i + 1; k < j; ++k) {
                    int coins = balloons[i] * balloons[k] * balloons[j] + dp[i][k] + dp[k][j];
                    dp[i][j] = max(dp[i][j], coins);
                }
            }
        }
        return dp[0][n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Burst balloons to maximize coins; bursting k yields `nums[k-1] * nums[k] * nums[k+1]` (LC 312).
// - Core Pattern: Interval DP (Think of the LAST balloon burst, not the first!).
// - Intuition & Derivation:
//   * Thinking forward (which to burst first) fails because bursting changes the neighbors of remaining balloons, creating dependency leaks.
//   * INVERSE THINKING: Choose which balloon `k` bursts LAST in open interval `(i, j)`.
//   * Since `k` is last, balloons `i` and `j` are still present to serve as adjacent multipliers!
//   * Recurrence: `dp[i][j] = max_{i < k < j} (dp[i][k] + dp[k][j] + nums[i] * nums[k] * nums[j])`.
// - Complexity:
//   * Time: O(N^3) — O(N^2) intervals, each taking O(N) partition loop.
//   * Space: O(N^2) — 2D interval memoization grid.
// - Edge Cases: Single balloon, all balloons 0, empty array.
// - Likely Follow-ups:
//   1. Why does choosing the first balloon burst fail? -> Subproblems become interdependent because boundaries collapse.
//   2. How is this related to Matrix Chain Multiplication? -> Structurally identical recurrence; both partition open intervals.


// =========================================================
// 25. MATRIX CHAIN MULTIPLICATION (MCM - FOUNDATIONAL CLASSIC)
// =========================================================

class Solution25 {
public:
    // dimensions: matrix i has dimension p[i-1] x p[i]
    int matrixChainOrder(const vector<int>& p) {
        int n = p.size() - 1; // Number of matrices
        // dp[i][j] = min scalar multiplications to multiply matrices A_i through A_j
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        
        // len is chain length (from 2 matrices up to n matrices)
        for (int len = 2; len <= n; ++len) {
            for (int i = 1; i <= n - len + 1; ++i) {
                int j = i + len - 1;
                dp[i][j] = INT_MAX;
                
                for (int k = i; k < j; ++k) {
                    int cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        return dp[1][n];
    }

    // Follow-up: Print Optimal Parenthesization
    void printParenthesis(int i, int j, const vector<vector<int>>& bracket, char& name, string& out) {
        if (i == j) {
            out += name++;
            return;
        }
        out += '(';
        printParenthesis(i, bracket[i][j], bracket, name, out);
        printParenthesis(bracket[i][j] + 1, j, bracket, name, out);
        out += ')';
    }
};
// Interview Explanation:
// - Problem Statement: Find optimal order to parenthesize matrix product A_1 * A_2 * ... * A_n to minimize scalar multiplications.
// - Core Pattern: Classical Interval DP with Partition Point Splitting.
// - Intuition & Derivation:
//   * Let `dp[i][j]` be min cost to multiply chain $A_i \dots A_j$.
//   * Split chain at position $k$ where $i \le k < j$:
//     `cost = dp[i][k] + dp[k+1][j] + p[i-1] * p[k] * p[j]`.
//   * Take minimum over all valid split points $k$.
// - Complexity:
//   * Time: O(N^3) — N^2 intervals times N partition splits.
//   * Space: O(N^2) — 2D DP matrix.
// - Edge Cases: Single matrix (cost = 0), chain of 2 matrices.
// - Likely Follow-ups:
//   1. How to print the optimal parentheses? -> Record `bracket[i][j] = k` when min cost updates, then print recursively.
//   2. Related problems? -> Burst Balloons (LC 312), Minimum Cost Tree From Leaf Values (LC 1130).


// =========================================================
// 26. HOUSE ROBBER III - TREE DP (LC 337)
// =========================================================

class Solution26 {
    // Returns pair<int, int>: {robThisNode, skipThisNode}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};
        
        auto left = dfs(node->left);
        auto right = dfs(node->right);
        
        // 1. If we ROB current node, we CANNOT rob its direct children
        int robThis = node->val + left.second + right.second;
        
        // 2. If we SKIP current node, children are free to be robbed or skipped
        int skipThis = max(left.first, left.second) + max(right.first, right.second);
        
        return {robThis, skipThis};
    }

public:
    int rob(TreeNode* root) {
        auto [robRoot, skipRoot] = dfs(root);
        return max(robRoot, skipRoot);
    }
};
// Interview Explanation:
// - Problem Statement: House robber on a binary tree; cannot rob parent and child nodes directly linked (LC 337).
// - Core Pattern: Postorder Bottom-Up Tree DP returning state tuples {rob, not_rob}.
// - Intuition & Derivation:
//   * Naive tree recursion recalculates grandchild states repeatedly, causing exponential explosion.
//   * Bottom-up postorder calculates two numbers per node in a single pass:
//     - `rob`: node->val + left.skip + right.skip
//     - `skip`: max(left.rob, left.skip) + max(right.rob, right.skip)
// - Complexity Progression:
//   * Naive Recursion: O(2^N) Time.
//   * Memoized (Map):  O(N) Time, O(N) Hash map space + O(H) stack.
//   * Pair Tree DP:    O(N) Time, O(H) Call Stack (strictly O(1) auxiliary heap space).
// - Edge Cases: Empty tree (0), single node, linear tree (skewed linked-list).
// - Likely Follow-ups:
//   1. Why is returning a pair better than memoization with `unordered_map<TreeNode*, int>`? -> Zero heap allocations, zero hashing overhead, cache local.
//   2. What if tree is a generic N-ary tree? -> Sum `child.skip` for robThis, sum `max(child.rob, child.skip)` for skipThis.


/*
 ====================================================================================================
                                  A. CORE PATTERNS QUICK-GLANCE TABLE
 ====================================================================================================
 | Archetype / Pattern              | Problems Covered                  | Trigger / Signal in Interviews                                        |
 | :--------------------------------| :---------------------------------| :---------------------------------------------------------------------|
 | 1D Fibonacci / Local Choice      | LC 70, LC 746, LC 198, LC 213, 91 | "Ways to reach step N", "no two adjacent elements chosen", "decode string" |
 | State Machine DP                 | LC 309, LC 714                    | Explicit state constraints (cooldown day, transaction fee, hold/cash) |
 | Kadane's Running Maximum         | LC 53, LC 918                     | "Maximum sum contiguous subarray" (local choice: extend or restart)    |
 | Longest Increasing Subseq (LIS)  | LC 300, LC 354, LC 139            | Strictly increasing order, nesting intervals, sorting trick + BS     |
 | 2-Sequence String Grid DP        | LC 1143, LC 72, LC 115, LC 44, 10 | Two strings + match/subsequence/transform (insert, delete, replace)   |
 | 2D Grid Path DP                  | LC 62, LC 63, LC 64, LC 221, 174  | "Grid moves down/right only", "min path sum", "maximal square"        |
 | 0/1 Knapsack (Backward Iter)     | LC 416, LC 494, Classic Knapsack  | "Partition into equal subsets", "each item used AT MOST once"          |
 | Unbounded Knapsack (Forward)     | LC 322, LC 518                    | "Unlimited supply of coins/items", Combinations vs Permutations order  |
 | Interval DP (Contracting/Expand) | LC 5, LC 516, LC 312, Classic MCM | "Palindromes", "burst balloons", "matrix chain product", O(N^3) splits|
 | Tree DP (Bottom-Up Postorder)    | LC 337                            | Hierarchical graph / tree where parent decision impacts children      |
 ====================================================================================================

 ====================================================================================================
                               B. MICROSOFT-SPECIFIC INTERVIEW CHECKLIST
 ====================================================================================================
 1. STATE SPACE REDUCTION (THE #1 FOLLOW-UP):
    - Always state the full 2D / 1D tabular solution first: "We can define dp[i][j] in O(M*N) space."
    - Immediately follow up verbally: "Notice dp[i][j] only depends on row i-1 and the diagonal. We can
      compress this to a single rolling 1D vector of size O(N) using a temporary `prevDiag` variable."

 2. TOP-DOWN VS BOTTOM-UP (HOW TO START):
    - If the recurrence relation feels non-obvious (e.g. Wildcard or Regex), write top-down memoization
      with a 2D `vector<vector<int>> memo(m, vector<int>(n, -1))` first.
    - If the problem is grid/knapsack-based, go straight to bottom-up tabulation; Microsoft interviewers
      prefer iterative tabulation to avoid stack overflow risks.

 3. RECONSTRUCTING THE SOLUTION (DON'T JUST RETURN THE NUMBER):
    - Interviewers at Microsoft love to ask: "Great, now how do we print the actual path / string / coins?"
    - Always explain the backtracking technique:
      * LCS / Edit Distance: Walk backwards from dp[m][n], comparing whether current cell came from top, left, or diagonal.
      * Knapsack / Coins: Store a parent decision array (`lastCoin[a]` or `parent[i]`) to emit chosen elements.

 4. BASE CASE & OVERFLOW PITFALLS:
    - Base Case [0][0]: Empty string / 0 capacity / 0 amount base cases must be initialized explicitly.
    - Infinity Initialization: For minimization (Coin Change), never use `INT_MAX` if you add `1 + dp[...]`
      (causes signed integer overflow). Use `amount + 1` or `1e9`.
    - Counting Overflows: In Distinct Subsequences (LC 115) or Combinations, counts explode; use `unsigned long long`.
 ====================================================================================================
*/
