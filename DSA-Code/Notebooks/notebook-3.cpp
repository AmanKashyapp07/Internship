#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Minimum Subset Sum Difference               | Subset Sum DP <= Total / 2        | O(N*Tot) | O(Total) |
 | 2  | Count of LIS (LeetCode 673)                 | 1D DP (Length & Count Arrays)     | O(N^2)   | O(N)     |
 | 3  | SCS Length                                  | Reduction: len(S1) + len(S2) - LCS| O(N * M) | O(N * M) |
 | 4  | Reconstruct SCS (LeetCode 1092)             | 2D LCS Table Backtracking         | O(N * M) | O(N * M) |
 | 5  | Minimum Window Subsequence (LeetCode 727)   | Forward Match + Backward Shrink   | O(N * M) | O(1)     |
 | 6  | Matrix Chain Multiplication                 | Interval DP over Chain Lengths    | O(N^3)   | O(N^2)   |
 | 7  | Bounded Knapsack (Binary Power Split)       | Binary Split + 1D 0/1 Knapsack    | O(W logK)| O(W)     |
 | 8  | LCS Length of 2 Permutations                | Index Map + Patience Sorting LIS  | O(N logN)| O(N)     |
 | 9  | Longest Common Increasing Subseq (LCIS)     | 1D DP with Optimal Prefix Tracker | O(N * M) | O(M)     |
 | 10 | Min Moves to Gather K Consecutive Ones(1703)| Shifted Index + Median Prefix Sum | O(N)     | O(N)     |
 | 11 | 2D Prefix Sum Matrix                        | 2D Inclusion-Exclusion Prefix DP  | O(M * N) | O(M * N) |
 | 12 | Make Array Non-Decreasing (Slope Trick)     | Greedy Max-Heap Slope Inflection  | O(N logN)| O(N)     |
 | 13 | Max Subarray Sum of Length <= K (CSES II)   | Monotonic Deque on Prefix Sums    | O(N)     | O(N)     |
 | 14 | Lexicographical Rank of a Permutation       | Factorial Positional Weighting    | O(N^2)   | O(N)     |
 | 15 | LCM of Array Elements Modulo MOD            | Prime Factorization + Max Powers  | O(N sqrt)| O(U)     |
 | 16 | Permutation Rounds (LCM of Cycles — CSES)   | Disjoint Cycle Decomposition + LCM| O(N+sqrt)| O(N)     |
 | 17 | Min Move-to-Anywhere Operations (N - LIS)   | Longest Increasing Subseq (N-LIS) | O(N logN)| O(N)     |
 | 18 | Min Move-to-Front Operations (N...1)        | Greedy Backwards Suffix Matching  | O(N)     | O(1)     |
 | 19 | Min Subarrays with Sum <= K                 | Greedy Running Sum Partitioning   | O(N)     | O(1)     |
 | 20 | House Robber IV (LeetCode 2560)             | Binary Search on Min Capability   | O(N logM)| O(1)     |
 | 21 | Bounded Coin Change (Limited Supply)        | Binary Splitting + 0/1 Knapsack   | O(A logK)| O(A)     |
 | 22 | Coin Change Path Reconstruction             | DP Predecessor Pointer Traceback  | O(N * A) | O(A)     |
 ====================================================================================================
*/

// ============================================================
// 1. MINIMUM SUBSET SUM DIFFERENCE
// ============================================================

int minSubsetSumDifference(vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    int target = total / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = target; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    for (int s = target; s >= 0; s--) {
        if (dp[s]) return total - 2 * s;
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Partition array into two subsets such that the absolute difference of their sums is minimized.
// - Approach: 0/1 Knapsack Boolean DP targeting Total / 2.
// - Intuition: Finding the closest reachable sum to `total / 2` minimizes `total - 2 * sum`.
// - Complexity: Time: O(N * Total), Space: O(Total).


// ============================================================
// 2. COUNT OF LONGEST INCREASING SUBSEQUENCES — LeetCode 673
// ============================================================

int findNumberOfLIS(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    vector<int> len(n, 1), count(n, 1);
    int maxLen = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    count[i] = count[j];
                } else if (len[j] + 1 == len[i]) {
                    count[i] += count[j];
                }
            }
        }
        maxLen = max(maxLen, len[i]);
    }

    int totalLIS = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == maxLen) totalLIS += count[i];
    }
    return totalLIS;
}
// Interview Explanation:
// - Problem Statement: Find number of longest increasing subsequences in an array (LeetCode 673).
// - Approach: Dual 1D DP arrays (`len[i]` and `count[i]`).
// - Intuition: When a strictly longer LIS ends at i, reset count[i]; if an equal length LIS is found, accumulate counts.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 3. SHORTEST COMMON SUPERSEQUENCE (SCS) LENGTH
// ============================================================

int scsLength(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return m + n - dp[m][n];
}
// Interview Explanation:
// - Problem Statement: Find length of shortest common supersequence of two strings.
// - Approach: Mathematical reduction to LCS: `len(S1) + len(S2) - LCS(S1, S2)`.
// - Intuition: Common characters are merged once; remainder are included sequentially.
// - Complexity: Time: O(N * M), Space: O(N * M).


// ============================================================
// 4. RECONSTRUCT SHORTEST COMMON SUPERSEQUENCE — LeetCode 1092
// ============================================================

string shortestCommonSupersequence(string str1, string str2) {
    int m = str1.size(), n = str2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    int i = m, j = n;
    string res = "";
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            res += str1[i - 1]; i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            res += str1[i - 1]; i--;
        } else {
            res += str2[j - 1]; j--;
        }
    }
    while (i > 0) res += str1[--i];
    while (j > 0) res += str2[--j];
    reverse(res.begin(), res.end());
    return res;
}
// Interview Explanation:
// - Problem Statement: Construct the shortest string that has both str1 and str2 as subsequences (LeetCode 1092).
// - Approach: 2D LCS Table Backtracking.
// - Intuition: Walk backwards through LCS table, taking matching characters once and non-matching characters from the dominating path.
// - Complexity: Time: O(N * M), Space: O(N * M).


// ============================================================
// 5. MINIMUM WINDOW SUBSEQUENCE — LeetCode 727
// ============================================================

string minWindowSubsequence(string s, string t) {
    int m = s.size(), n = t.size();
    int minLen = INT_MAX, startIdx = -1;
    int sIdx = 0, tIdx = 0;

    while (sIdx < m) {
        if (s[sIdx] == t[tIdx]) {
            tIdx++;
            if (tIdx == n) {
                // Reverse match to find the tightest left boundary
                int end = sIdx;
                tIdx--;
                while (tIdx >= 0) {
                    if (s[sIdx] == t[tIdx]) tIdx--;
                    sIdx--;
                }
                sIdx++;
                tIdx = 0;
                if (end - sIdx + 1 < minLen) {
                    minLen = end - sIdx + 1;
                    startIdx = sIdx;
                }
            }
        }
        sIdx++;
    }
    return startIdx == -1 ? "" : s.substr(startIdx, minLen);
}
// Interview Explanation:
// - Problem Statement: Find shortest contiguous substring of S containing T as a subsequence (LeetCode 727).
// - Approach: Two-Pointer Forward Search + Backwards Window Shrink.
// - Intuition: Find candidate end position moving right, then backtrack left to find the tightest starting index.
// - Complexity: Time: O(M * N), Space: O(1).


// ============================================================
// 6. MATRIX CHAIN MULTIPLICATION
// ============================================================

int matrixChainMultiplication(const vector<int>& p) {
    int n = p.size() - 1;
    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find optimal parenthesization of matrix chain to minimize scalar multiplications.
// - Approach: Interval DP iterating over chain lengths.
// - Intuition: Split chain into two sub-chains at index k; `dp[i][j] = min(dp[i][k] + dp[k+1][j] + cost)`.
// - Complexity: Time: O(N^3), Space: O(N^2).


// ============================================================
// 7. BOUNDED KNAPSACK (BINARY POWER SPLITTING)
// ============================================================

int boundedKnapsack(int W, const vector<int>& weights, const vector<int>& values, const vector<int>& counts) {
    vector<int> dp(W + 1, 0);
    int n = weights.size();

    for (int i = 0; i < n; i++) {
        int count = counts[i];
        for (int k = 1; count > 0; k <<= 1) {
            int take = min(k, count);
            int itemW = take * weights[i];
            int itemV = take * values[i];
            for (int w = W; w >= itemW; w--) {
                dp[w] = max(dp[w], dp[w - itemW] + itemV);
            }
            count -= take;
        }
    }
    return dp[W];
}
// Interview Explanation:
// - Problem Statement: 0/1 Knapsack with bounded item counts.
// - Approach: Binary Decomposition (1, 2, 4, ..., remainder) + 0/1 Knapsack.
// - Intuition: Reduces O(count) transitions to O(log count) items.
// - Complexity: Time: O(W * sum(log K)), Space: O(W).


// ============================================================
// 8. LCS LENGTH OF 2 PERMUTATIONS (REDUCTION TO LIS)
// ============================================================

int lcsOfPermutations(const vector<int>& A, const vector<int>& B) {
    int n = A.size();
    unordered_map<int, int> posA;
    for (int i = 0; i < n; i++) posA[A[i]] = i;

    vector<int> mapped;
    for (int x : B) {
        if (posA.count(x)) mapped.push_back(posA[x]);
    }

    // LIS on mapped indices in O(N log N)
    vector<int> lis;
    for (int x : mapped) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return lis.size();
}
// Interview Explanation:
// - Problem Statement: Find LCS of two permutations of numbers 1..N in O(N log N) time.
// - Approach: Map elements of B to their index in A, then compute LIS.
// - Intuition: A common subsequence corresponds to indices in A appearing in increasing order in B.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 9. LONGEST COMMON INCREASING SUBSEQUENCE (LCIS)
// ============================================================

int LCIS(vector<int>& a, vector<int>& b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m, 0);

    for (int i = 0; i < n; i++) {
        int currentOptimal = 0;
        for (int j = 0; j < m; j++) {
            if (a[i] == b[j]) {
                dp[j] = max(dp[j], currentOptimal + 1);
            } else if (a[i] > b[j]) {
                currentOptimal = max(currentOptimal, dp[j]);
            }
        }
    }
    return *max_element(dp.begin(), dp.end());
}
// Interview Explanation:
// - Problem Statement: Find the longest common subsequence of two arrays that is also strictly increasing.
// - Approach: 1D Dynamic Programming with running optimal prefix tracker.
// - Intuition: For each a[i], track the maximum LCIS length for values < a[i], then update dp[j] on match.
// - Complexity: Time: O(N * M), Space: O(M).


// ============================================================
// 10. MIN MOVES TO GATHER K CONSECUTIVE ONES — LeetCode 1703
// ============================================================

int minMoves(vector<int>& nums, int k) {
    vector<int> p;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (nums[i] == 1) p.push_back(i - p.size()); // Shifted indices
    }
    int m = p.size();
    vector<long long> prefix(m + 1, 0);
    for (int i = 0; i < m; i++) prefix[i + 1] = prefix[i] + p[i];

    long long minCost = LLONG_MAX;
    for (int i = 0; i <= m - k; i++) {
        int mid = i + k / 2;
        long long median = p[mid];
        long long leftCost = median * (mid - i) - (prefix[mid] - prefix[i]);
        long long rightCost = (prefix[i + k] - prefix[mid + 1]) - median * (i + k - 1 - mid);
        minCost = min(minCost, leftCost + rightCost);
    }
    return minCost;
}
// Interview Explanation:
// - Problem Statement: Minimum moves to gather any k ones together consecutively (LeetCode 1703).
// - Approach: Index Shift Transformation + Median Cost using Prefix Sums.
// - Intuition: Transforming index `p[j] = orig[j] - j` converts the problem to gathering elements to a single median point.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 11. 2D PREFIX SUM MATRIX
// ============================================================

struct NumMatrix {
    vector<vector<int>> pref;
    NumMatrix(vector<vector<int>>& matrix) {
        int r = matrix.size(), c = matrix[0].size();
        pref.assign(r + 1, vector<int>(c + 1, 0));
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                pref[i + 1][j + 1] = matrix[i][j] + pref[i][j + 1] + pref[i + 1][j] - pref[i][j];
            }
        }
    }
    int sumRegion(int row1, int col1, int row2, int col2) {
        return pref[row2 + 1][col2 + 1] - pref[row1][col2 + 1] - pref[row2 + 1][col1] + pref[row1][col1];
    }
};
// Interview Explanation:
// - Problem Statement: Query sum of submatrix [row1, col1] to [row2, col2] in O(1) time.
// - Approach: 2D Inclusion-Exclusion Prefix Sum Table.
// - Intuition: Build 2D cumulative prefix matrix in O(R * C); query subgrid in O(1).
// - Complexity: Time: Build O(R * C), Query O(1); Space: O(R * C).


// ============================================================
// 12. MAKE ARRAY NON-DECREASING (SLOPE TRICK)
// ============================================================

long long minOperationsNonDecreasing(vector<int>& nums) {
    long long ans = 0;
    priority_queue<int> maxHeap;
    for (int x : nums) {
        if (!maxHeap.empty() && maxHeap.top() > x) {
            ans += maxHeap.top() - x;
            maxHeap.pop();
            maxHeap.push(x);
        }
        maxHeap.push(x);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Minimum operations (+1/-1) to make array non-decreasing.
// - Approach: Slope Trick with Max-Heap.
// - Intuition: Represents derivative of convex cost function; inflection points tracked by max-heap.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 13. MAX SUBARRAY SUM OF LENGTH <= K (CSES II)
// ============================================================

ll maxSubarraySumAtMostK(int n, int k, const vl& arr) {
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + arr[i];

    deque<int> dq;
    dq.push_back(0);
    ll maxSum = -1e18;

    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();
        if (!dq.empty()) maxSum = max(maxSum, pref[i] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[i]) dq.pop_back();
        dq.push_back(i);
    }
    return maxSum;
}
// Interview Explanation:
// - Problem Statement: Find maximum subarray sum with length at most K (CSES Max Subarray Sum II).
// - Approach: Monotonic Deque on Prefix Sums.
// - Intuition: Maximize `pref[i] - pref[j]` for `i - k <= j < i` by maintaining minimum `pref[j]` in a sliding window.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 14. LEXICOGRAPHICAL RANK OF A PERMUTATION
// ============================================================

ll permutationRank(const vi& perm) {
    int n = perm.size();
    vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = (fact[i - 1] * i) % MOD;

    ll rank = 1;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = i + 1; j < n; j++) {
            if (perm[j] < perm[i]) count++;
        }
        rank = (rank + count * fact[n - 1 - i]) % MOD;
    }
    return rank;
}
// Interview Explanation:
// - Problem Statement: Find 1-based lexicographical rank of a permutation among all permutations of length N.
// - Approach: Factorial Number System (Lehmer code conversion).
// - Intuition: At position i, each smaller remaining element contributes `fact[N - 1 - i]` permutations.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 15. LCM OF ARRAY ELEMENTS MODULO MOD
// ============================================================

ll arrayLCMMod(const vi& nums) {
    map<int, int> maxPrimePower;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int count = 0;
                while (temp % p == 0) { count++; temp /= p; }
                maxPrimePower[p] = max(maxPrimePower[p], count);
            }
        }
        if (temp > 1) maxPrimePower[temp] = max(maxPrimePower[temp], 1);
    }
    ll lcm = 1;
    for (auto& [p, power] : maxPrimePower) {
        for (int i = 0; i < power; i++) lcm = (lcm * p) % MOD;
    }
    return lcm;
}
// Interview Explanation:
// - Problem Statement: Compute LCM of all array elements modulo (1e9 + 7).
// - Approach: Prime factorization and max-exponent tracking.
// - Intuition: LCM is product of all primes raised to their maximum power across all numbers.
// - Complexity: Time: O(N * sqrt(max_A)), Space: O(Unique Primes).


// ============================================================
// 16. PERMUTATION ROUNDS (LCM OF CYCLES — CSES 3398)
// ============================================================

ll permutationRounds(const vi& perm) {
    int n = perm.size();
    vector<bool> vis(n, false);
    map<int, int> primeMaxPower;

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            int len = 0, cur = i;
            while (!vis[cur]) { vis[cur] = true; cur = perm[cur] - 1; len++; }
            int temp = len;
            for (int p = 2; p * p <= temp; p++) {
                if (temp % p == 0) {
                    int count = 0;
                    while (temp % p == 0) { count++; temp /= p; }
                    primeMaxPower[p] = max(primeMaxPower[p], count);
                }
            }
            if (temp > 1) primeMaxPower[temp] = max(primeMaxPower[temp], 1);
        }
    }
    ll ans = 1;
    for (auto& [p, power] : primeMaxPower) {
        for (int i = 0; i < power; i++) ans = (ans * p) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find number of rounds until permutation returns to identity (CSES 3398).
// - Approach: Permutation cycle decomposition + modular LCM.
// - Intuition: Overall cycle period is LCM of individual disjoint cycle lengths.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 17. MIN MOVE-TO-ANYWHERE OPERATIONS (N - LIS)
// ============================================================

int minMoveToAnywhereToSort(const vi& nums) {
    vi lis;
    for (int x : nums) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return nums.size() - lis.size();
}
// Interview Explanation:
// - Problem Statement: Find minimum operations to sort array if any element can be moved to any position.
// - Approach: Compute Longest Increasing Subsequence (LIS); answer is `N - LIS.size()`.
// - Intuition: Elements in LIS stay in place; all remaining elements are inserted around them.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 18. MIN MOVE-TO-FRONT OPERATIONS (N...1)
// ============================================================

int minMoveToFrontToSort(const vi& perm) {
    int n = perm.size();
    int expected = n;
    for (int i = n - 1; i >= 0; i--) {
        if (perm[i] == expected) expected--;
    }
    return expected;
}
// Interview Explanation:
// - Problem Statement: Minimum operations to sort permutation [1...N] if elements can only be moved to the front.
// - Approach: Greedy backwards suffix match: search for N, N-1, N-2... from right to left.
// - Intuition: The longest suffix of elements already in relative order 1..k can remain; others must be moved.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 19. MIN SUBARRAYS WITH SUM <= K
// ============================================================

int minSubarraysSumAtMostK(const vi& nums, ll k) {
    int count = 1;
    ll curSum = 0;
    for (int x : nums) {
        if (curSum + x <= k) {
            curSum += x;
        } else {
            count++;
            curSum = x;
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Greedy partition of non-negative array into minimum contiguous subarrays each with sum <= k.
// - Approach: Greedy linear accumulation.
// - Intuition: Greedily extend each subarray until adding next element exceeds limit k.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 20. HOUSE ROBBER IV — LeetCode 2560
// ============================================================

int minCapability(vector<int>& nums, int k) {
    int lo = *min_element(nums.begin(), nums.end()), hi = *max_element(nums.begin(), nums.end());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2, count = 0, n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] <= mid) { count++; i++; }
        }
        if (count >= k) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
// Interview Explanation:
// - Problem Statement: Find minimum capability to rob at least k non-adjacent houses (LeetCode 2560).
// - Approach: Binary Search on Answer + Greedy Non-Adjacent Count.
// - Intuition: The predicate "can we steal >= k non-adjacent houses each <= mid" is monotonic.
// - Complexity: Time: O(N log(max - min)), Space: O(1).


// ============================================================
// 21. BOUNDED COIN CHANGE (LIMITED SUPPLY)
// ============================================================

int coinChangeBounded(vector<int>& coins, vector<int>& limits, int amount) {
    vector<int> dp(amount + 1, 1e9);
    dp[0] = 0;
    for (int i = 0; i < (int)coins.size(); i++) {
        int c = coins[i], lim = limits[i];
        for (int k = 1; lim > 0; k <<= 1) {
            int take = min(k, lim);
            int weight = take * c, cost = take;
            for (int x = amount; x >= weight; x--) dp[x] = min(dp[x], dp[x - weight] + cost);
            lim -= take;
        }
    }
    return dp[amount] >= 1e9 ? -1 : dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find minimum coins to make amount where coin[i] has limited count limits[i].
// - Approach: Binary Power Splitting + 0/1 Knapsack Backwards DP.
// - Intuition: Split item counts into powers of 2 to reduce transitions from O(limit) to O(log limit).
// - Complexity: Time: O(amount * sum(log limit)), Space: O(amount).


// ============================================================
// 22. COIN CHANGE PATH RECONSTRUCTION (PRINT COINS)
// ============================================================

vector<int> reconstructCoins(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, 1e9), parent(amount + 1, -1);
    dp[0] = 0;
    for (int c : coins) {
        for (int x = c; x <= amount; x++) {
            if (dp[x - c] + 1 < dp[x]) {
                dp[x] = dp[x - c] + 1;
                parent[x] = c;
            }
        }
    }
    if (dp[amount] >= 1e9) return {};
    vector<int> res;
    for (int curr = amount; curr > 0; curr -= parent[curr]) res.push_back(parent[curr]);
    return res;
}
// Interview Explanation:
// - Problem Statement: Reconstruct and return the exact coin denominations used to make amount with fewest coins.
// - Approach: DP Predecessor Tracking + Backtracking Path Recovery.
// - Intuition: `parent[x]` stores the last coin denomination used to achieve optimal state dp[x].
// - Complexity: Time: O(N * amount), Space: O(amount).
