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

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #   | Problem Name                               | Pattern / Technique               | Time     | Space    |
 |-----|--------------------------------------------|-----------------------------------|----------|----------|
 | 94  | Next Greater Element II                    | Circular Monotonic Stack (2N)     | O(N)     | O(N)     |
 | 95  | Kth Largest Element in a Stream            | Min-Heap Size-K Priority Queue    | O(log K) | O(K)     |
 | 96  | Final Prices With Special Discount         | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 97  | Jump Game VI                               | Sliding Window Monotonic Deque DP | O(N)     | O(K)     |
 | 98  | Regular Expression Matching                | 2D Dynamic Programming (., *)     | O(M * N) | O(M * N) |
 | 99  | Maximum Subarray                           | Kadane's Running Prefix DP        | O(N)     | O(1)     |
 | 100 | Unique Paths                               | Combinatorics / 1D DP Space       | O(M * N) | O(N)     |
 | 101 | Edit Distance                              | 2D Levenshtein Distance DP         | O(M * N) | O(M * N) |
 | 102 | Triangle                                   | Bottom-Up In-Place DP             | O(N^2)   | O(1)     |
 | 103 | Best Time to Buy and Sell Stock            | Running Minimum Price Single-Pass | O(N)     | O(1)     |
 | 104 | House Robber                               | 2-State Constant Space DP         | O(N)     | O(1)     |
 | 105 | Longest Increasing Subsequence             | Patience Sorting + Binary Search  | O(N logN)| O(N)     |
 | 106 | Partition Equal Subset Sum & Knapsacks     | 0/1 & Unbounded Knapsack DP       | O(N * S) | O(S)     |
 | 107 | Reverse Pairs                              | Divide-and-Conquer Merge Sort     | O(N logN)| O(N)     |
 | 108 | Longest Palindromic Subsequence            | Interval 2D Dynamic Programming   | O(N^2)   | O(N)     |
 | 109 | Coin Change II & Coin Change I             | Unbounded Knapsack Combinations DP| O(N * A) | O(A)     |
 | 110 | Beautiful Arrangement                      | Bitmask Dynamic Programming       | O(N * 2^N)| O(2^N)  |
 | 111 | K-th Symbol in Grammar                     | Binary Tree Symmetry / Popcount   | O(1)     | O(1)     |
 | 112 | Number of ZigZag Arrays I                  | Directional DP + Prefix Sums      | O(N * M) | O(N * M) |
 | 113 | Combination Sum                            | Backtracking with Element Reuse   | O(2^T)   | O(T)     |
 | 114 | N-Queens                                   | Bitmask Constrained Backtracking  | O(N!)    | O(N)     |
 | 115 | Subsets                                    | Cascading / Backtracking Power Set| O(2^N * N)| O(N)    |
 | 116 | Subsets II                                 | Sorting + Duplicate Skip Pruning  | O(2^N * N)| O(N)    |
 | 117 | Concatenated Words / Word Break II         | Memoized DFS Backtracking Trie    | O(N * L^2)| O(N * L) |
 | 118 | Reverse Integer                            | Overflow-Safe Modular Reversal    | O(log X) | O(1)     |
 | 119 | Pow(x, n)                                  | Fast Binary Exponentiation        | O(log N) | O(1)     |
 | 120 | Single Number II                           | Bitwise FSM Modulo-3 Counters     | O(N)     | O(1)     |
 | 121 | Count Primes                               | Sieve of Eratosthenes             | O(N loglogN)| O(N)     |
 | 122 | Rectangle Area                             | Coordinate Geometry Overlap Area  | O(1)     | O(1)     |
 | 123 | H-Index                                    | Linear Bucket Counting Sort       | O(N)     | O(N)     |
 | 124 | Consecutive Numbers Sum                    | Arithmetic Progression Math Check | O(√N)    | O(1)     |
 ====================================================================================================
*/

// =========================================================
// 94. NEXT GREATER ELEMENT II (LC 503)
// =========================================================

class Solution94 {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = (int)nums.size();
        vector<int> result(n, -1);
        stack<int> st; // stores indices

        // Iterate through array twice to simulate circular traversal
        for (int i = 0; i < 2 * n; ++i) {
            int idx = i % n;
            while (!st.empty() && nums[st.top()] < nums[idx]) {
                result[st.top()] = nums[idx];
                st.pop();
            }
            if (i < n) {
                st.push(idx);
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find next greater element for each element in circular array nums.
// - Approach: Monotonic Decreasing Stack over Virtual 2N Array.
// - Intuition:
//   * A circular array of length N is equivalent to an array traversed up to 2N - 1 indices using `i % n`.
//   * Only push indices during the first pass (0 to n - 1) to avoid redundant entries.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 95. KTH LARGEST ELEMENT IN A STREAM (LC 703)
// =========================================================

class Solution95 {
public:
    class KthLargest {
        priority_queue<int, vector<int>, greater<int>> minHeap;
        int k;

    public:
        KthLargest(int k, vector<int>& nums) : k(k) {
            for (int num : nums) {
                add(num);
            }
        }

        int add(int val) {
            if ((int)minHeap.size() < k) {
                minHeap.push(val);
            } else if (val > minHeap.top()) {
                minHeap.pop();
                minHeap.push(val);
            }
            return minHeap.top();
        }
    };
};
using KthLargest = Solution95::KthLargest;

// Interview Explanation:
// - Problem Statement: Design a class to find the k-th largest element in a stream of numbers.
// - Approach: Maintain a min-heap of capacity k. The heap top always holds the k-th largest value.
// - Intuition: Elements smaller than top cannot be in top-k; pushing larger elements and evicting smallest keeps top at k-th largest.
// - Complexity: Time: O(log K) per add operation, Space: O(K).


// =========================================================
// 96. FINAL PRICES WITH A SPECIAL DISCOUNT IN A SHOP (LC 1475)
// =========================================================

class Solution96 {
public:
    vector<int> finalPrices(vector<int>& prices) {
        int n = prices.size();
        vector<int> ans = prices;
        stack<int> st; // Stores indices of prices awaiting discount

        for (int i = 0; i < n; i++) {
            while (!st.empty() && prices[st.top()] >= prices[i]) {
                ans[st.top()] -= prices[i];
                st.pop();
            }
            st.push(i);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Apply discount to prices[i] equal to first subsequent price prices[j] <= prices[i].
// - Approach: Monotonic Stack. Maintain stack of indices whose next smaller/equal element hasn't been found.
// - Intuition: When encountering prices[i] <= prices[st.top()], prices[i] acts as discount for all stack elements >= prices[i].
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 97. JUMP GAME VI (LC 1696)
// =========================================================

class Solution97 {
public:
    int maxResult(vector<int>& nums, int k) {
        int n = (int)nums.size();

        // dp[i] = maximum score possible when reaching index i
        vector<int> dp(n);

        // Starting position
        dp[0] = nums[0];

        // Stores indices.
        // dp values of these indices are in decreasing order.
        deque<int> dq;

        dq.push_back(0); // Starting index

        for (int i = 1; i < n; ++i) {

            // Remove indices that are more than k positions behind.
            while (!dq.empty() && dq.front() < i - k) {
                dq.pop_front();
            }

            // Front contains the index with maximum dp value
            // among the last k reachable positions.
            dp[i] = nums[i] + dp[dq.front()];

            // Remove indices having smaller or equal dp values.
            // Current index is better because it has a >= score
            // and is more recent.
            while (!dq.empty() && dp[dq.back()] <= dp[i]) {
                dq.pop_back();
            }

            // Add current index.
            dq.push_back(i);
        }

        return dp[n - 1];
    }
};


// =========================================================
// 98. REGULAR EXPRESSION MATCHING (LC 10)
// RECURSION + MEMOIZATION
// =========================================================

class Solution98 {
public:
    vector<vector<int>> memo;

    bool solve(int i, int j, string& s, string& p) {
        // Pattern finished: string must also be finished
        if (j == p.size()) return i == s.size();
        // Return cached result
        if (memo[i][j] != -1) return memo[i][j];

        // Check if current characters match
        bool firstMatch = i < s.size() && (p[j] == s[i] || p[j] == '.');
        bool ans;
        // Next character is '*'
        if (j + 1 < p.size() && p[j + 1] == '*') {
            // Skip '*' OR use it to match current character
            ans = solve(i, j + 2, s, p) || (firstMatch && solve(i + 1, j, s, p));
        }
        else {
            // Match current characters and move forward
            ans = firstMatch && solve(i + 1, j + 1, s, p);
        }

        return memo[i][j] = ans;
    }

    bool isMatch(string s, string p) {
        int m = s.size();
        int n = p.size();

        // -1 means not computed
        memo.assign(m + 1, vector<int>(n + 1, -1));

        return solve(0, 0, s, p);
    }
};



// =========================================================
// 99. MAXIMUM SUBARRAY (LC 53)
// =========================================================

class Solution99 {
public:
    int maxSubArray(vector<int>& nums) {
        int maxSum = nums[0];
        int currentSum = 0;

        for (int x : nums) {
            currentSum = max(x, currentSum + x);
            maxSum = max(maxSum, currentSum);
        }
        return maxSum;
    }
};
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with the largest sum and return its sum.
// - Approach: Kadane's Algorithm. Maintain running sum, resetting to current element if running sum becomes negative.
// - Intuition: A negative prefix degrades any subsequent subarray; start fresh whenever current sum dips below the current element.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 100. UNIQUE PATHS (LC 62)
// =========================================================

class Solution100 {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[j] += dp[j - 1];
            }
        }
        return dp[n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Find number of unique paths from top-left to bottom-right in an m x n grid moving only right or down.
// - Approach: 1D Space-Optimized Dynamic Programming.
// - Intuition:
//   * A 1D array rolling row by row accumulates paths: ways to reach (i, j) is ways from above (dp[j]) + ways from left (dp[j-1]).
// - Complexity: Time: O(M * N), Space: O(N).


// =========================================================
// 101. EDIT DISTANCE (LC 72)
// =========================================================

class Solution101 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0)); // dp[i][j] = min operations to convert word1[0..i-1] to word2[0..j-1]

        for (int i = 1; i <= m; ++i) dp[i][0] = i; // deleting all characters from word1
        for (int j = 1; j <= n; ++j) dp[0][j] = j; // inserting all characters to word1 to form word2

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1]; // if characters match, no operation needed
                } else { // if they don't match, consider all three operations and take the minimum
                    dp[i][j] = 1 + min({
                        dp[i - 1][j],    // Delete
                        dp[i][j - 1],    // Insert
                        dp[i - 1][j - 1] // Replace
                    });
                }
            }
        }
        return dp[m][n]; // minimum operations to convert word1 to word2
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum operations (insert, delete, replace) to convert word1 into word2.
// - Approach: Classic 2D Levenshtein Dynamic Programming.
// - Intuition:
//   * If characters match, no operation needed (`dp[i-1][j-1]`).
//   * Otherwise, take 1 + minimum of deletion, insertion, or substitution.
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 102. TRIANGLE (LC 120)
// =========================================================

class Solution102 {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<int> dp = triangle[n - 1];

        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col <= row; col++) {
                dp[col] = triangle[row][col] + min(dp[col], dp[col + 1]);
            }
        }

        return dp[0];
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum path sum from top to bottom of a triangle.
// - Approach: Bottom-Up 1D Dynamic Programming.
// - Intuition:
//   * Bottom-up eliminates boundary edge cases: each cell (r, c) transitions to min of (r + 1, c) and (r + 1, c + 1).
// - Complexity: Time: O(N^2), Space: O(N).


// =========================================================
// 103. BEST TIME TO BUY AND SELL STOCK (LC 121)
// =========================================================

class Solution103 {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = INT_MAX;
        int maxProfitVal = 0;

        for (int p : prices) {
            minPrice = min(minPrice, p);
            maxProfitVal = max(maxProfitVal, p - minPrice);
        }
        return maxProfitVal;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum profit achievable from buying and selling a single stock share.
// - Approach: Running Minimum Price Single-Pass Sweep.
// - Intuition:
//   * At each day, selling yields `p - minPriceSoFar`. Update running minimum and maximum profit.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 104. HOUSE ROBBER (LC 198)
// =========================================================

class Solution104 {
public:
    int rob(vector<int>& nums) {
        int prev2 = 0; // dp[i-2]
        int prev1 = 0; // dp[i-1]
        // prev2 and prev1 represent the maximum amount of money that can be robbed from the previous two houses.

        for (int x : nums) {
            int curr = max(prev1, prev2 + x); // either skip or rob current house
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1; // the maximum amount of money that can be robbed from all houses
    }
};
// Interview Explanation:
// - Problem Statement: Find max money stolen without robbing adjacent houses.
// - Approach: Constant-Space Dynamic Programming.
// - Intuition:
//   * Choice at house i: rob it (`prev2 + x`) or skip it (`prev1`).
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 105. LONGEST INCREASING SUBSEQUENCE (LC 300)
// =========================================================

class Solution105 {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;

        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) {
                tails.push_back(x);
            } else {
                *it = x;
            }
        }
        return (int)tails.size();
    }
};
// Interview Explanation:
// - Problem Statement: Find length of the longest strictly increasing subsequence.
// - Approach: Patience Sorting + Binary Search (`std::lower_bound`).
// - Intuition:
//   * Maintain `tails[len - 1]`: smallest tail of all increasing subsequences of length `len`.
//   * Binary search finds the smallest tail >= x and updates it; if x is strictly larger than all tails, extend length by 1.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 106. PARTITION EQUAL SUBSET SUM (LC 416) & KNAPSACK FUNDAMENTALS
// =========================================================

// --- Companion Fundamental 1: Standard 0/1 Knapsack ---
int knapsack01(vector<int>& weight, vector<int>& value, int capacity) {
    // dp[w] = maximum value we can get with capacity w
    vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < (int)weight.size(); i++) {
        // Go BACKWARD because each item can be used only once
        for (int w = capacity; w >= weight[i]; w--) {
            dp[w] = max(dp[w], value[i] + dp[w - weight[i]]);
        }
    }
    return dp[capacity];
}
// Time: O(N * capacity), Space: O(capacity)


// --- Companion Fundamental 2: Unbounded Knapsack ---
int unboundedKnapsack(vector<int>& weight, vector<int>& value, int capacity) {
    // dp[w] = maximum value we can get with capacity w
    vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < (int)weight.size(); i++) {
        // Go FORWARD because the same item can be used unlimited times
        for (int w = weight[i]; w <= capacity; w++) {
            dp[w] = max(dp[w], value[i] + dp[w - weight[i]]);
        }
    }
    return dp[capacity];
}
// Time: O(N * capacity), Space: O(capacity)


// --- Companion Fundamental 3: Coin Change 1 (Min Coins) ---
int coinChange(vector<int>& coins, int amount) {
    // dp[x] = minimum coins needed to make amount x
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0; // Base case: 0 coins needed to make amount 0

    for (int x = 1; x <= amount; x++) {
        for (int coin : coins) {
            if (coin <= x) {
                dp[x] = min(dp[x], dp[x - coin] + 1);
            }
        }
    }
    return dp[amount] == amount + 1 ? -1 : dp[amount];
}
// Time: O(N * amount), Space: O(amount)


// --- Companion Fundamental 4: Coin Change 2 (Combinations) ---
int coinChangeWays(int amount, vector<int>& coins) {
    // dp[x] = number of combinations to make amount x
    vector<int> dp(amount + 1, 0);
    dp[0] = 1; // One way to make amount 0: choose no coins

    for (int coin : coins) {
        // Go FORWARD because each coin can be used unlimited times
        for (int x = coin; x <= amount; x++) {
            dp[x] += dp[x - coin];
        }
    }
    return dp[amount];
}
// Time: O(N * amount), Space: O(amount)


class Solution106 {
public:
    bool canPartition(vector<int>& nums) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        // If total sum is odd, it cannot be divided into two equal subsets
        if (totalSum % 2 != 0) return false;
        int target = totalSum / 2;

        // dp[sum] = can we make this sum using the elements processed so far?
        vector<bool> dp(target + 1, false);
        dp[0] = true; // We can always make sum 0 by choosing no elements

        for (int x : nums) {
            // Backward loop -> 0/1 Knapsack -> use each element once
            // Forward loop  -> Unbounded Knapsack -> can reuse elements
            for (int sum = target; sum >= x; sum--) {
                dp[sum] = dp[sum] || dp[sum - x];
            }
            // Early exit if target sum is reachable
            if (dp[target]) return true;
        }
        return dp[target];
    }
};
// Interview Explanation:
// - Problem: Determine whether the array can be partitioned into two subsets with equal sum.
// - Approach: 0/1 Subset Sum / 0/1 Knapsack feasibility DP.
// - Intuition:
//   * If totalSum is even, check whether a subset with sum totalSum / 2 exists.
//   * dp[sum] tells whether that sum can be formed using the elements processed so far.
//   * Iterate backwards so the current element is used at most once.
// - Complexity: Time: O(N * target), Space: O(target).


// =========================================================
// 107. REVERSE PAIRS (LC 493)
// =========================================================

class Solution107 {
    int count = 0;

    void mergeSort(vector<int>& a, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(a, left, mid);
        mergeSort(a, mid + 1, right);

        int j = mid + 1;
        int i = left;
        while (i <= mid) {
            while (j <= right && (long long)a[i] > 2LL * a[j]) j++;
            count += (j - (mid + 1)); // add all numbers from mid+1 to j-1 that satisfy condition
            i++;
        }

        vector<int> temp;
        int p1 = left, p2 = mid + 1;
        while (p1 <= mid && p2 <= right) {
            if (a[p1] <= a[p2]) temp.push_back(a[p1++]);
            else temp.push_back(a[p2++]);
        }
        while (p1 <= mid) temp.push_back(a[p1++]);
        while (p2 <= right) temp.push_back(a[p2++]);
        copy(temp.begin(), temp.end(), a.begin() + left);
    }

public:
    int reversePairs(vector<int>& nums) {
        count = 0;
        int n = (int)nums.size();
        mergeSort(nums, 0, n - 1);
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Count pairs (i, j) where i < j and nums[i] > 2 * nums[j].
// - Approach: Divide-and-Conquer Merge Sort Counting.
// - Intuition:
//   * When merging two sorted halves [left..mid] and [mid+1..right], pointer j advances monotonically because elements in the left half are sorted.
//   * Yields optimal O(N log N) time without expensive balanced tree lookups.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 108. LONGEST PALINDROMIC SUBSEQUENCE (LC 516)
// =========================================================

// Fundamental Relation: lps(s) = lcs(s, reverse(s))

class Solution108 {
public:
    int longestPalindromeSubseq(string s) {
        int n = (int)s.size();
        vector<int> dp(n, 0);

        for (int i = n - 1; i >= 0; --i) {
            vector<int> newDp(n, 0);
            newDp[i] = 1;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) {
                    newDp[j] = dp[j - 1] + 2;
                } else {
                    newDp[j] = max(dp[j], newDp[j - 1]);
                }
            }
            dp = std::move(newDp);
        }
        return dp[n - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest palindromic subsequence in s.
// - Approach: Interval Dynamic Programming with Space Optimization (or LCS of s and reverse(s)).
// - Intuition:
//   * If s[i] == s[j], dp[i][j] = dp[i+1][j-1] + 2.
//   * Otherwise, dp[i][j] = max(dp[i+1][j], dp[i][j-1]).
// - Complexity: Time: O(N^2), Space: O(N).


// =========================================================
// 109. COIN CHANGE II (LC 518) & COIN CHANGE I (LC 322)
// =========================================================

// Companion Fundamental: Coin Change 1 (Minimum coins to make amount)
class Solution1100 {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0; // 0 coins needed to make amount 0

        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (i - coin >= 0 && dp[i - coin] != INT_MAX) {
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};

// Coin Change 2 (Number of combinations to make amount)
class Solution109 {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1; // One way to make amount 0: use no coins

        // Loop coin on outer level to ensure combinations (not permutations)
        for (int c : coins) {
            for (int a = c; a <= amount; ++a) {
                dp[a] += dp[a - c];
            }
        }
        return dp[amount];
    }
};
// Interview Explanation:
// - Problem Statement: Return number of combinations that make up amount using given coins.
// - Approach: 1D Unbounded Knapsack Dynamic Programming.
// - Intuition:
//   * Coin Change 1 asks for MINIMUM number of coins (min over valid sub-amounts).
//   * Coin Change 2 asks for NUMBER OF COMBINATIONS (sum over valid combinations).
//   * Iterating coins on the outer loop guarantees combinations rather than ordered permutations.
// - Complexity: Time: O(N * amount), Space: O(amount).


// =========================================================
// 110. BEAUTIFUL ARRANGEMENT (LC 526)
// =========================================================

class Solution110 {
public:
    int countArrangement(int n) {
        // dp[mask] = number of valid permutations of first __builtin_popcount(mask) elements
        vector<int> dp(1 << n, 0); // dp[mask] represents the number of valid arrangements for the subset of numbers represented by 'mask'
        dp[0] = 1; // Base case: one way to arrange zero numbers

        for (int mask = 0; mask < (1 << n); ++mask) {
            if (dp[mask] == 0) continue; // Skip if no valid arrangements for this mask
            int pos = __builtin_popcount(mask) + 1; // 1-based index to place next number // 

            for (int i = 0; i < n; ++i) {
                if (!(mask & (1 << i))) {
                    int val = i + 1;
                    if (val % pos == 0 || pos % val == 0) {
                        dp[mask | (1 << i)] += dp[mask];
                    }
                }
            }
        }
        return dp[(1 << n) - 1];
    }
};
// Interview Explanation:
// - Problem Statement: Count beautiful permutations where perm[i] is divisible by i or i is divisible by perm[i] (1-indexed).
// - Approach: Bitmask Dynamic Programming.
// - Intuition:
//   * Mask state represents set of numbers placed so far; count of set bits directly gives current position `pos`.
// - Complexity: Time: O(N * 2^N), Space: O(2^N).


// =========================================================
// 111. K-TH SYMBOL IN GRAMMAR (LC 779)
// =========================================================

class Solution111 {
public:
    int kthGrammar(int n, int k) {
        // Each row doubles in length; the bit at (k - 1) is flipped for each 1 in binary representation
        return __builtin_popcount(k - 1) & 1;
    }
};
// Interview Explanation:
// - Problem Statement: Generate grammar row by row ('0' -> "01", '1' -> "10") and return kth symbol in nth row.
// - Approach: Binary Parity / Popcount Formula.
// - Intuition:
//   * The tree is an infinite Thue-Morse sequence. Symbol at position k (1-indexed) is equivalent to the parity of set bits in `k - 1`.
// - Complexity: Time: O(1), Space: O(1).


// =========================================================
// 112. NUMBER OF ZIGZAG ARRAYS I (LC 3699)
// =========================================================

class Solution112 {
public:
    static const int MOD = 1e9 + 7;

    int zigZagArrays(int n, int l, int r) {
        int m = r - l + 1;

        // up[x]   = arrays ending at x where last step was increasing
        // down[x] = arrays ending at x where last step was decreasing
        vector<long long> up(m, 1), down(m, 1);

        for (int len = 2; len <= n; len++) { // Iterate through each length from 2 to n
            vector<long long> newUp(m), newDown(m); // New arrays for the next length

            long long sum = 0;

            // Previous value < current value
            for (int x = 0; x <= m-1; x++) {
                newUp[x] = sum;
                sum = (sum + down[x]) % MOD;
            }

            sum = 0;

            // Previous value > current value
            for (int x = m - 1; x >= 0; x--) {
                newDown[x] = sum;
                sum = (sum + up[x]) % MOD;
            }

            up = newUp;
            down = newDown;
        }

        long long ans = 0;

        for (int x = 0; x < m; x++)
            ans = (ans + up[x] + down[x]) % MOD;

        // For n = 1, every value was counted twice
        if (n == 1) return m;

        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Count length-n arrays with elements in [1, m] where adjacent elements strictly alternate direction (< > < > ... or > < > < ...).
// - Approach: Dynamic Programming with Prefix/Suffix Sum Accumulation.
// - Intuition:
//   * Separate transitions into increasing (`next_inc[v] = sum_{u < v} dp_dec[u]`) and decreasing (`next_dec[v] = sum_{u > v} dp_inc[u]`).
//   * Prefix sums reduce step transitions from O(M^2) to O(M).
// - Complexity: Time: O(N * M), Space: O(M).


// =========================================================
// 113. COMBINATION SUM (LC 39)
// =========================================================

class Solution113 {
    vector<vector<int>> ans;

    void dfs(const vector<int>& a, int start, int rem, vector<int>& cur) {
        if (rem == 0) {
            ans.push_back(cur);
            return;
        }

        for (int i = start; i < (int)a.size() && a[i] <= rem; i++) {
            cur.push_back(a[i]);
            dfs(a, i, rem - a[i], cur); // reuse allowed
            cur.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& a, int target) {
        sort(a.begin(), a.end());
        vector<int> cur;
        dfs(a, 0, target, cur);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all unique combinations of candidates that sum to target, allowing unlimited reuse of numbers.
// - Approach: Backtracking with Early Pruning on Sorted Array.
// - Intuition:
//   * Sort candidates to break early when `candidates[i] > rem`.
//   * Pass index `i` (not `i + 1`) to recursive call to enable unlimited reuse.
// - Complexity: Time: O(2^(target / min_val)), Space: O(target / min_val).


// =========================================================
// 114. N-QUEENS (LC 51)
// =========================================================

class Solution114 {
    vector<vector<string>> result;
    vector<string> board;
    vector<bool> cols, diag1, diag2;
    int n;

    void backtrack(int r) {
        if (r == n) {
            result.push_back(board);
            return;
        }
        for (int c = 0; c < n; ++c) {
            if (cols[c] || diag1[r - c + n] || diag2[r + c]) continue;
            board[r][c] = 'Q';
            cols[c] = diag1[r - c + n] = diag2[r + c] = true;
            backtrack(r + 1);
            board[r][c] = '.';
            cols[c] = diag1[r - c + n] = diag2[r + c] = false;
        }
    }

public:
    vector<vector<string>> solveNQueens(int nVal) {
        n = nVal;
        result.clear();
        board.assign(n, string(n, '.'));
        cols.assign(n, false);
        diag1.assign(2 * n, false);
        diag2.assign(2 * n, false);
        backtrack(0);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Place n queens on an n x n chessboard such that no two queens attack each other.
// - Approach: Row-by-Row Backtracking with Column & Diagonal Bitsets.
// - Intuition:
//   * Each row must have exactly one queen.
//   * Diagonals are indexed by `r - c + n` and `r + c`. O(1) conflict verification.
// - Complexity: Time: O(N!), Space: O(N).


// =========================================================
// 115. SUBSETS (LC 78)
// =========================================================

class Solution115 {
    vector<vector<int>> result;
    vector<int> current;

    void backtrack(const vector<int>& nums, int start) {
        result.push_back(current);
        for (int i = start; i < (int)nums.size(); ++i) {
            current.push_back(nums[i]);
            backtrack(nums, i + 1);
            current.pop_back();
        }
    }

public:
    vector<vector<int>> subsets(vector<int>& nums) {
        result.clear();
        current.clear();
        backtrack(nums, 0);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return power set of unique integers nums.
// - Approach: Recursive Cascading Backtracking.
// - Intuition:
//   * At each recursive step, snapshot `current` into result, then iterate through remaining choices.
// - Complexity: Time: O(2^N * N), Space: O(N).


// =========================================================
// 116. SUBSETS II (LC 90)
// =========================================================

class Solution116 {
    vector<vector<int>> result;
    vector<int> current;

    void backtrack(const vector<int>& nums, int start) {
        result.push_back(current);
        for (int i = start; i < (int)nums.size(); ++i) {
            if (i > start && nums[i] == nums[i - 1]) continue;
            current.push_back(nums[i]);
            backtrack(nums, i + 1);
            current.pop_back();
        }
    }

public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        result.clear();
        current.clear();
        sort(nums.begin(), nums.end());
        backtrack(nums, 0);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return power set containing duplicate elements without duplicate subsets in output.
// - Approach: Sorting + Sibling Duplicate Pruning.
// - Intuition:
//   * Sort array. If `i > start && nums[i] == nums[i-1]`, skip to avoid creating duplicate branches.
// - Complexity: Time: O(2^N * N), Space: O(N).


// =========================================================
// 117. CONCATENATED WORDS (LC 472) & WORD BREAK II (LC 140)
// =========================================================

class Solution117 {
    unordered_set<string> st;

    bool dfsConcat(const string& s, int i, vector<int>& memo) {
        if (i == (int)s.size()) return true;
        if (memo[i] != -1) return memo[i];

        string cur;
        for (int j = i; j < (int)s.size(); j++) {
            cur += s[j];
            if (st.count(cur) && dfsConcat(s, j + 1, memo)) {
                return memo[i] = 1;
            }
        }
        return memo[i] = 0;
    }

    unordered_set<string> dict;
    unordered_map<string, vector<string>> memoWB;

    vector<string> dfsWordBreak(const string& rem) {
        if (memoWB.count(rem)) return memoWB[rem];
        if (rem.empty()) return {""};

        vector<string> subList;
        for (int len = 1; len <= (int)rem.size(); ++len) {
            string word = rem.substr(0, len);
            if (dict.count(word)) {
                vector<string> subSentences = dfsWordBreak(rem.substr(len));
                for (const string& sub : subSentences) {
                    subList.push_back(word + (sub.empty() ? "" : " ") + sub);
                }
            }
        }
        return memoWB[rem] = subList;
    }

public:
    // LC 472: Concatenated Words
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        st = unordered_set<string>(words.begin(), words.end());
        vector<string> ans;
        for (const string& word : words) {
            st.erase(word);
            vector<int> memo(word.size(), -1);
            if (dfsConcat(word, 0, memo)) ans.push_back(word);
            st.insert(word);
        }
        return ans;
    }

    // LC 140: Word Break II
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        dict.clear();
        memoWB.clear();
        for (const string& w : wordDict) dict.insert(w);
        return dfsWordBreak(s);
    }
};
// Interview Explanation:
// - Problem Statement:
//   * Concatenated Words: Return all words that can be formed by concatenating at least two other words from dictionary.
//   * Word Break II: Add spaces in s to construct all valid sentences where each word is in wordDict.
// - Approach: Top-Down DFS with Memoization + Hash Set Dictionary.
// - Intuition:
//   * For Concatenated Words: remove word from set, recursively test if substrings exist in set.
//   * For Word Break II: explore dictionary prefixes, solve suffixes recursively, memoize paths.
// - Complexity: Time: O(N * L^2), Space: O(N * L).


// =========================================================
// 118. REVERSE INTEGER (LC 7)
// =========================================================

class Solution118 {
public:
    int reverse(int x) {
        int rev = 0;
        while (x != 0) {
            int pop = x % 10;
            x /= 10;

            // Check for 32-bit signed integer overflow before multiplying by 10
            if (rev > INT_MAX / 10 || (rev == INT_MAX / 10 && pop > 7)) return 0;
            if (rev < INT_MIN / 10 || (rev == INT_MIN / 10 && pop < -8)) return 0;

            rev = rev * 10 + pop;
        }
        return rev;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse digits of a 32-bit signed integer, returning 0 if reversing causes overflow.
// - Approach: Mathematical Digit Extraction with Pre-Multiplication Bounds Check.
// - Intuition:
//   * Verify `rev > INT_MAX / 10` before multiplying to avoid runtime undefined behavior overflow.
// - Complexity: Time: O(log10 |X|), Space: O(1).


// =========================================================
// 119. POW(X, N) (LC 50)
// =========================================================

class Solution119 {
public:
    double myPow(double x, int n) {
        long long exp = n;
        if (exp < 0) {
            x = 1.0 / x;
            exp = -exp;
        }

        double result = 1.0;
        while (exp > 0) {
            if (exp & 1) {
                result *= x;
            }
            x *= x;
            exp >>= 1;
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Calculate x raised to power n (x^n).
// - Approach: Binary Exponentiation (Exponentiation by Squaring).
// - Intuition:
//   * Cast n to long long to prevent overflow when negating INT_MIN.
//   * Square base and halve exponent every step in O(log N).
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 120. SINGLE NUMBER II (LC 137)
// =========================================================

class Solution120 {
public:
    int singleNumber(vector<int>& nums) {
        int ones = 0, twos = 0;

        for (int x : nums) {
            ones = (ones ^ x) & ~twos;
            twos = (twos ^ x) & ~ones;
        }
        return ones;
    }
};
// Interview Explanation:
// - Problem Statement: Every element appears three times except for one which appears exactly once. Find it.
// - Approach: Bitwise Modulo-3 Finite State Machine.
// - Intuition:
//   * Two variables `ones` and `twos` track bits that have appeared 1 time and 2 times.
//   * When a bit appears a third time, both reset to 0.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 121. COUNT PRIMES (LC 204)
// =========================================================

class Solution121 {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;

        vector<bool> isPrime(n, true);
        isPrime[0] = isPrime[1] = false;

        for (int i = 2; i * i < n; ++i) {
            if (isPrime[i]) {
                for (int j = i * i; j < n; j += i) {
                    isPrime[j] = false;
                }
            }
        }
        return count(isPrime.begin(), isPrime.end(), true);
    }
};
// Interview Explanation:
// - Problem Statement: Count number of prime numbers strictly less than n.
// - Approach: Sieve of Eratosthenes.
// - Intuition:
//   * Mark multiples of each prime starting from i * i up to n.
// - Complexity: Time: O(N log log N), Space: O(N).


// =========================================================
// 122. RECTANGLE AREA (LC 223)
// =========================================================

class Solution122 {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        int areaA = (ax2 - ax1) * (ay2 - ay1);
        int areaB = (bx2 - bx1) * (by2 - by1);

        // Compute overlapping rectangle dimensions
        int overlapX = max(0, min(ax2, bx2) - max(ax1, bx1));
        int overlapY = max(0, min(ay2, by2) - max(ay1, by1));
        int overlapArea = overlapX * overlapY;

        return areaA + areaB - overlapArea;
    }
};
// Interview Explanation:
// - Problem Statement: Find total area covered by two rectilinear rectangles.
// - Approach: Principle of Inclusion-Exclusion.
// - Intuition:
//   * Total Area = Area(A) + Area(B) - Overlap(A, B).
//   * Overlap is determined by max(0, min(x2) - max(x1)) * max(0, min(y2) - max(y1)).
// - Complexity: Time: O(1), Space: O(1).


// =========================================================
// 123. H-INDEX (LC 274)
// =========================================================

class Solution123 {
public:
    int hIndex(vector<int>& citations) {
        int n = (int)citations.size();
        vector<int> buckets(n + 1, 0);

        for (int c : citations) {
            if (c >= n) {
                buckets[n]++;
            } else {
                buckets[c]++;
            }
        }

        int count = 0;
        for (int i = n; i >= 0; --i) {
            count += buckets[i];
            if (count >= i) {
                return i;
            }
        }
        return 0;
    }
};
// Interview Explanation:
// - Problem Statement: Compute researcher's h-index (h papers having at least h citations).
// - Approach: Counting / Bucket Sort in O(N) Time.
// - Intuition:
//   * Any citation >= n can be bucketed at index n because h cannot exceed n.
//   * Accumulate citations from n down to 0; return first index where cumulative count >= i.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 124. CONSECUTIVE NUMBERS SUM (LC 829)
// =========================================================

class Solution124 {
public:
    int consecutiveNumbersSum(int n) {
        // n = k * x + k * (k - 1) / 2 where k is number of terms, x >= 1 is first term
        int count = 0;
        for (long long k = 1; k * (k - 1) / 2 < n; ++k) {
            if ((n - k * (k - 1) / 2) % k == 0) {
                count++;
            }
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Return number of ways to write n as a sum of two or more consecutive positive integers + 1 (single number).
// - Approach: Arithmetic Progression Algebra.
// - Intuition:
//   * A sum of k consecutive integers starting at x is `k * x + k*(k-1)/2 = n`.
//   * `k * x = n - k*(k-1)/2`. A valid integer x exists iff `(n - k*(k-1)/2) % k == 0`.
// - Complexity: Time: O(sqrt(N)), Space: O(1).\n