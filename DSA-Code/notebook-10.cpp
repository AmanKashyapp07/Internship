#include <bits/stdc++.h>
using namespace std;

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
    // 1. 1D DP / LINEAR DP
    // =========================================================

    // House Robber
    // dp[i] = maximum money that can be robbed from houses [0..i-1].
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> dp(n + 1, 0);
        dp[1] = nums[0];

        for (int i = 2; i <= n; i++) {
            // Skip house i-1 OR rob it and skip i-2.
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[i - 1]);
        }

        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum money robbed from non-adjacent houses (LeetCode 198).
    // - Approach: 1D DP tabulation array `dp[i]` storing max robbed from first i houses.
    // - Intuition: At house i, choose max of skipping house i-1 (`dp[i-1]`) or robbing it (`dp[i-2] + nums[i-1]`).
    // - Complexity: Time: O(N), Space: O(N).

    // House Robber II
    // First and last house cannot both be selected.
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

    // Decode Ways
    // dp[i] = number of ways to decode the first i characters.
    int numDecodings(string s) {
        int n = s.size();
        if (n == 0) return 0;

        vector<int> dp(n + 1, 0);
        dp[0] = 1;

        for (int i = 1; i <= n; i++) {
            // Decode s[i-1] as a single digit.
            if (s[i - 1] != '0')
                dp[i] += dp[i - 1];

            // Decode s[i-2..i-1] as a two-digit number.
            if (i >= 2) {
                int x = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');

                if (10 <= x && x <= 26)
                    dp[i] += dp[i - 2];
            }
        }

        return dp[n];
    }
    // Interview Explanation:
    // - Problem Statement: Count total ways to decode a digit string into letter combinations (LeetCode 91).
    // - Approach: 1D DP tabulation `dp[i]` representing total decoding ways for first i characters.
    // - Intuition: Single digit `s[i-1] != '0'` adds `dp[i-1]`; two-digit `s[i-2..i-1]` in [10..26] adds `dp[i-2]`.
    // - Complexity: Time: O(N), Space: O(N).

    // Delete and Earn
    // Transform into House Robber:
    // sum[x] = total value obtained by taking all occurrences of x.
    int deleteAndEarn(vector<int>& nums) {
        if (nums.empty()) return 0;

        int mx = *max_element(nums.begin(), nums.end());
        vector<int> sum(mx + 1, 0);

        for (int x : nums)
            sum[x] += x;

        vector<int> dp(mx + 1, 0);

        for (int x = 1; x <= mx; x++) {
            dp[x] = dp[x - 1];

            if (x >= 2)
                dp[x] = max(dp[x], dp[x - 2] + sum[x]);
            else
                dp[x] = max(dp[x], sum[x]);
        }

        return dp[mx];
    }
    // Interview Explanation:
    // - Problem Statement: Maximize points gained by taking nums[i] and deleting all occurrences of nums[i]-1 and nums[i]+1 (LeetCode 740).
    // - Approach: Frequency sum mapping transformed into House Robber DP.
    // - Intuition: Precompute `sum[x] = x * count`; choosing x forbids x-1, matching House Robber state transitions.
    // - Complexity: Time: O(N + \max(\text{nums})), Space: O(\max(\text{nums})).


    // =========================================================
    // 2. KNAPSACK / SUBSET SUM DP
    // =========================================================

    // 0/1 Knapsack
    // dp[i][w] = maximum value using first i items with capacity w.
    int knapsack01(int W, const vector<int>& wt, const vector<int>& val) {
        int n = wt.size();

        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= W; w++) {
                // Do not take item i-1.
                dp[i][w] = dp[i - 1][w];

                // Take item i-1 if it fits.
                if (wt[i - 1] <= w) {
                    dp[i][w] = max(
                        dp[i][w],
                        dp[i - 1][w - wt[i - 1]] + val[i - 1]
                    );
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

    // Partition Equal Subset Sum
    // dp[i][target] = whether target can be formed using first i numbers.
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);

        if (sum % 2 != 0)
            return false;

        int target = sum / 2;
        int n = nums.size();

        vector<vector<bool>> dp(
            n + 1,
            vector<bool>(target + 1, false)
        );

        for (int i = 0; i <= n; i++)
            dp[i][0] = true;

        for (int i = 1; i <= n; i++) {
            for (int t = 1; t <= target; t++) {
                dp[i][t] = dp[i - 1][t];

                if (nums[i - 1] <= t)
                    dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
            }
        }

        return dp[n][target];
    }
    // Interview Explanation:
    // - Problem Statement: Determine if an array can be partitioned into two subsets with equal sum (LeetCode 416).
    // - Approach: 0/1 Knapsack subset sum DP matching target = totalSum / 2.
    // - Intuition: Problem reduces to finding if a subset sums exactly to sum/2 using 2D boolean DP table.
    // - Complexity: Time: O(N \cdot \text{sum}), Space: O(N \cdot \text{sum}).

    // Target Sum
    // Convert: P - N = target, P + N = total => P = (total + target) / 2
    int findTargetSumWays(vector<int>& nums, int target) {
        int total = accumulate(nums.begin(), nums.end(), 0);

        if (abs(target) > total)
            return 0;

        if ((total + target) % 2 != 0)
            return 0;

        int subsetSum = (total + target) / 2;
        int n = nums.size();

        vector<vector<long long>> dp(
            n + 1,
            vector<long long>(subsetSum + 1, 0)
        );

        dp[0][0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int s = 0; s <= subsetSum; s++) {
                // Do not take nums[i-1].
                dp[i][s] = dp[i - 1][s];

                // Take nums[i-1].
                if (nums[i - 1] <= s)
                    dp[i][s] += dp[i - 1][s - nums[i - 1]];
            }
        }

        return dp[n][subsetSum];
    }
    // Interview Explanation:
    // - Problem Statement: Assign '+' and '-' signs to elements to evaluate to target (LeetCode 494).
    // - Approach: Mathematical transformation to subset sum DP: `P = (total + target) / 2`.
    // - Intuition: Finding positive subset P reduces target evaluation to counting subsets summing to (total + target) / 2.
    // - Complexity: Time: O(N \cdot \text{sum}), Space: O(N \cdot \text{sum}).

    // Coin Change
    // dp[a] = minimum number of coins required to make amount a.
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

    // Coin Change II
    // dp[i][a] = number of combinations to make amount a using the first i coin types.
    int change(int amount, vector<int>& coins) {
        int n = coins.size();

        vector<vector<long long>> dp(
            n + 1,
            vector<long long>(amount + 1, 0)
        );

        for (int i = 0; i <= n; i++)
            dp[i][0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int a = 1; a <= amount; a++) {
                // Do not use this coin.
                dp[i][a] = dp[i - 1][a];

                // Use this coin; it can be reused.
                if (coins[i - 1] <= a)
                    dp[i][a] += dp[i][a - coins[i - 1]];
            }
        }

        return (int)dp[n][amount];
    }
    // Interview Explanation:
    // - Problem Statement: Count total unique combinations to make up target amount (LeetCode 518).
    // - Approach: Unbounded Knapsack 2D DP tabulation `dp[i][a]`.
    // - Intuition: Outer loop over coin types ensures combinations are ordered, preventing duplicate permutations.
    // - Complexity: Time: O(N \cdot \text{amount}), Space: O(N \cdot \text{amount}).


    // =========================================================
    // 3. LCS / STRING DP
    // =========================================================

    // Longest Common Subsequence
    // dp[i][j] = LCS length of first i chars of text1 and first j chars of text2.
    int longestCommonSubsequence(string text1, string text2) {
        int n = text1.size();
        int m = text2.size();

        vector<vector<int>> dp(
            n + 1,
            vector<int>(m + 1, 0)
        );

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        return dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Find length of longest common subsequence between text1 and text2 (LeetCode 1143).
    // - Approach: 2D DP tabulation table `dp[i][j]` comparing prefixes.
    // - Intuition: If `text1[i-1] == text2[j-1]`, extend LCS by `1 + dp[i-1][j-1]`; else take `max(dp[i-1][j], dp[i][j-1])`.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Edit Distance
    // dp[i][j] = minimum operations to convert first i chars of word1 into first j chars of word2.
    int minDistance(string word1, string word2) {
        int n = word1.size();
        int m = word2.size();

        vector<vector<int>> dp(
            n + 1,
            vector<int>(m + 1, 0)
        );

        for (int i = 0; i <= n; i++)
            dp[i][0] = i;

        for (int j = 0; j <= m; j++)
            dp[0][j] = j;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + min({
                        dp[i - 1][j],     // delete
                        dp[i][j - 1],     // insert
                        dp[i - 1][j - 1]  // replace
                    });
                }
            }
        }

        return dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum operations (insert, delete, replace) to convert word1 to word2 (LeetCode 72).
    // - Approach: 2D Edit Distance DP table `dp[i][j]`.
    // - Intuition: If characters match, `dp[i][j] = dp[i-1][j-1]`; else 1 + min(delete: `dp[i-1][j]`, insert: `dp[i][j-1]`, replace: `dp[i-1][j-1]`).
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Distinct Subsequences
    // dp[i][j] = number of ways to form first j chars of t from first i chars of s.
    int numDistinct(string s, string t) {
        int n = s.size();
        int m = t.size();

        vector<vector<unsigned long long>> dp(
            n + 1,
            vector<unsigned long long>(m + 1, 0)
        );

        for (int i = 0; i <= n; i++)
            dp[i][0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // Skip s[i-1].
                dp[i][j] = dp[i - 1][j];

                // Match s[i-1] with t[j-1].
                if (s[i - 1] == t[j - 1])
                    dp[i][j] += dp[i - 1][j - 1];
            }
        }

        return (int)dp[n][m];
    }
    // Interview Explanation:
    // - Problem Statement: Count number of unique subsequences of s that equal t (LeetCode 115).
    // - Approach: 2D DP table `dp[i][j]` matching prefix s[0..i-1] with t[0..j-1].
    // - Intuition: Always add `dp[i-1][j]` (skipping s[i-1]); if `s[i-1] == t[j-1]`, also add `dp[i-1][j-1]`.
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).


    // =========================================================
    // 4. LIS / SEQUENCE DP
    // =========================================================

    // O(N log N) LIS.
    // tails[len-1] = smallest possible tail for an increasing subsequence of length len.
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;

        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);

            if (it == tails.end())
                tails.push_back(x);
            else
                *it = x;
        }

        return tails.size();
    }
    // Interview Explanation:
    // - Problem Statement: Find length of longest strictly increasing subsequence (LeetCode 300).
    // - Approach: Binary Search + Patience Sorting (`tails` array storing smallest tail per length).
    // - Intuition: `lower_bound` finds first tail >= x and replaces it to maintain smallest possible tail values for future extensions.
    // - Complexity: Time: O(N \log N), Space: O(N).

    // Longest String Chain
    int longestStrChain(vector<string>& words) {
        sort(words.begin(), words.end(),
             [](const string& a, const string& b) {
                 return a.size() < b.size();
             });

        unordered_map<string, int> dp;
        int ans = 0;

        for (const string& word : words) {
            int best = 1;

            for (int i = 0; i < (int)word.size(); i++) {
                string prev = word.substr(0, i) + word.substr(i + 1);

                if (dp.count(prev))
                    best = max(best, dp[prev] + 1);
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

    // Largest Divisible Subset
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        if (nums.empty()) return {};

        sort(nums.begin(), nums.end());

        int n = nums.size();
        int bestLen = 1;
        int bestIdx = 0;

        vector<int> dp(n, 1);
        vector<int> parent(n, -1);

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }

            if (dp[i] > bestLen) {
                bestLen = dp[i];
                bestIdx = i;
            }
        }

        vector<int> ans;

        for (int cur = bestIdx; cur != -1; cur = parent[cur])
            ans.push_back(nums[cur]);

        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find largest subset where every pair (a, b) satisfies `a % b == 0` (LeetCode 368).
    // - Approach: Sorting + LIS DP with parent pointer array for subset path reconstruction.
    // - Intuition: Sorting guarantees if `nums[i] % nums[j] == 0`, then `nums[i]` is divisible by all elements in `nums[j]`'s chain.
    // - Complexity: Time: O(N^2), Space: O(N).


    // =========================================================
    // 5. GRID / MATRIX DP
    // =========================================================

    // Unique Paths II
    // dp[i][j] = number of ways to reach cell (i,j).
    long long uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int n = obstacleGrid.size();
        int m = obstacleGrid[0].size();

        vector<vector<long long>> dp(n, vector<long long>(m, 0));

        if (obstacleGrid[0][0] == 1 || obstacleGrid[n - 1][m - 1] == 1)
            return 0;

        dp[0][0] = 1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (obstacleGrid[i][j] == 1) {
                    dp[i][j] = 0;
                    continue;
                }

                if (i > 0)
                    dp[i][j] += dp[i - 1][j];

                if (j > 0)
                    dp[i][j] += dp[i][j - 1];
            }
        }

        return dp[n - 1][m - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Count unique paths from top-left to bottom-right avoiding obstacles (LeetCode 63).
    // - Approach: 2D Grid DP table `dp[i][j]`.
    // - Intuition: Obstacle sets `dp[i][j] = 0`; otherwise sum paths coming from top (`dp[i-1][j]`) and left (`dp[i][j-1]`).
    // - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).

    // Minimum Path Sum
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<vector<int>> dp(n, vector<int>(m, 0));

        dp[0][0] = grid[0][0];

        for (int i = 1; i < n; i++)
            dp[i][0] = dp[i - 1][0] + grid[i][0];

        for (int j = 1; j < m; j++)
            dp[0][j] = dp[0][j - 1] + grid[0][j];

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

    // Maximal Square
    // dp[i][j] = largest square side ending at (i-1,j-1).
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty())
            return 0;

        int n = matrix.size();
        int m = matrix[0].size();
        int best = 0;

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


    // =========================================================
    // 6. INTERVAL / PARTITION DP
    // =========================================================

    // Minimum Cost to Cut a Stick
    // dp[i][j] = minimum cost to perform all cuts between cuts[i] and cuts[j].
    int minCostCutStick(int n, vector<int>& cuts) {
        vector<int> c = cuts;
        c.push_back(0);
        c.push_back(n);

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

    // Burst Balloons
    // Think of k as the LAST balloon burst in interval (i,j).
    int maxCoinsBurstBalloons(vector<int>& nums) {
        int n = nums.size();

        vector<int> b(n + 2, 1);

        for (int i = 0; i < n; i++)
            b[i + 1] = nums[i];

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

    // Palindrome Partitioning II
    int minCutPalindromePartitioning(string s) {
        int n = s.size();
        if (n <= 1) return 0;

        // pal[i][j] = whether s[i..j] is a palindrome.
        vector<vector<bool>> pal(n, vector<bool>(n, false));

        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i <= 2 || pal[i + 1][j - 1])) {
                    pal[i][j] = true;
                }
            }
        }

        // dp[i] = minimum cuts needed for prefix s[0..i].
        vector<int> dp(n, INT_MAX);

        for (int i = 0; i < n; i++) {
            if (pal[0][i]) {
                dp[i] = 0;
                continue;
            }

            for (int j = 0; j < i; j++) {
                if (pal[j + 1][i]) {
                    dp[i] = min(dp[i], dp[j] + 1);
                }
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

    // Maximum Path Sum
    // Returns maximum downward path starting from root.
    // Updates answer with a path passing through current node.
    pair<int, int> maxPathSumHelper(TreeNode* root) {
        if (!root)
            return {0, INT_MIN};

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

    // House Robber III
    // returns {rob current node, skip current node}.
    pair<int, int> robTreeHelper(TreeNode* root) {
        if (!root)
            return {0, 0};

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

    // Stock with Cooldown
    // State: 0 = holding, 1 = just sold, 2 = resting / not holding.
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

    // Stock with Transaction Fee
    // State: 0 = holding, 1 = cash / not holding.
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


    // =========================================================
    // 9. DIGIT DP / BITMASK DP
    // =========================================================

    // Digit DP: Count total occurrences of digit '1' in [0..n].
    // State: pos = position, count = 1s chosen, started = non-leading zero appeared, tight = prefix limit.
    string digitStr;
    long long digitMemo[20][20][2][2];

    long long countDigitOneDFS(int pos, int count, bool started, bool tight) {
        if (pos == (int)digitStr.size())
            return count;

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

    // Smallest Sufficient Team
    // dp[mask] = minimum team covering exactly the skills in mask.
    vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
        int m = req_skills.size();
        int n = people.size();

        unordered_map<string, int> skillId;
        for (int i = 0; i < m; i++) skillId[req_skills[i]] = i;

        vector<int> personMask(n, 0);
        for (int i = 0; i < n; i++) {
            for (const string& skill : people[i]) {
                if (skillId.count(skill))
                    personMask[i] |= 1 << skillId[skill];
            }
        }

        int states = 1 << m;
        int target = states - 1;

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
    // 10. GAME THEORY / MINIMAX DP
    // =========================================================

    // Removal Game / Stone Game
    // dp[i][j] = maximum score difference current player can obtain from nums[i..j].
    long long removalGame(vector<int>& nums) {
        int n = nums.size();

        vector<vector<long long>> dp(n, vector<long long>(n, 0));

        for (int i = 0; i < n; i++)
            dp[i][i] = nums[i];

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
};

/*
================================================================================
                    DYNAMIC PROGRAMMING — INTERVIEW CHEAT SHEET
================================================================================

1. DP RECOGNITION

| Pattern | Typical State | Key Idea |
|--------|---------------|----------|
| Pick / Skip | dp[i] | take current vs skip current |
| 0/1 Knapsack | dp[i][w] | item used at most once |
| Unbounded Knapsack | dp[i][w] | item can be reused |
| Subset Sum | dp[i][sum] | can we form this sum? |
| Count Ways | dp[i][state] | number of valid ways |
| LCS | dp[i][j] | compare two prefixes |
| Edit Distance | dp[i][j] | insert/delete/replace |
| LIS | dp[i] / tails | best increasing sequence |
| Grid DP | dp[i][j] | transition from neighbors |
| Interval DP | dp[l][r] | split interval at k |
| Tree DP | return state from child | combine subtree states |
| State Machine | dp[i][state] | model current condition |
| Digit DP | dp[pos][...] | build number digit by digit |
| Bitmask DP | dp[mask] | subset of small universe |
| Minimax DP | dp[l][r] | best relative score |

2. THE INTERVIEW DP PROTOCOL

Step 1:
Define exactly what dp[state] means.

Step 2:
Identify the choices/transitions.

Step 3:
Write the recurrence.

Step 4:
Identify base cases.

Step 5:
Choose:
    - Memoization when recursive choices are natural.
    - Tabulation when dependency order is obvious.

Step 6:
State:
    Time = number of states × transitions per state
    Space = number of stored states + recursion stack (if memoized)

3. KNAPSACK LOOP RULE

For 0/1 Knapsack:
    process each item once
    capacity goes backwards in 1D optimization

For Unbounded Knapsack:
    same item may be reused
    capacity goes forwards in 1D optimization

4. INTERVAL DP TEMPLATE

for (int len = smallest; len <= n; len++) {
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1;

        for (int k = l; k < r; k++) {
            // combine left interval + right interval
        }
    }
}

5. COMMON INTERVIEW TRAPS

- 0/1 vs unbounded knapsack: loop direction matters in 1D DP.
- Coin Change II counts combinations, not permutations.
- LCS is subsequence; substring must be contiguous.
- LIS uses lower_bound for strictly increasing subsequence.
- Interval DP usually becomes easy after deciding what k represents.
- Tree DP often returns multiple states from each subtree.
- State-machine DP: define the meaning of every state before coding.
- Digit DP: `tight` and `started` are different concepts.
- Target Sum requires checking feasibility before converting to subset sum.
- Always check integer overflow when DP counts ways.

6. GOLDEN RULE

Do not start by writing `dp`.

First answer:
    "What does dp[state] represent?"

If that sentence is precise, the transition usually becomes straightforward.
================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}
