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
 | 16 | Permutation Rounds (LCM of Cycles — CSES)   | Disjoint Cycle Decomposition + LCM| O(N)     | O(N)     |
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

int minSubsetSumDifference(vi& a) {
    int tot = accumulate(a.begin(), a.end(), 0);
    int t = tot / 2;
    vector<bool> dp(t + 1, false);
    dp[0] = true;

    for (int x : a) {
        for (int j = t; j >= x; j--) { // starting from t down to x to avoid reusing the same element
            dp[j] = dp[j] || dp[j - x];
        }
    }

    for (int s = t; s >= 0; s--) {
        if (dp[s]) return tot - 2 * s;
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Partition array into two subsets such that the absolute difference of their sums is minimized.
// - Approach: 0/1 Knapsack Boolean DP targeting Total / 2.
// - Intuition:
//   * Let subset sums be S1 and S2 with S1 + S2 = Total. Difference is |S1 - S2| = Total - 2 * S1 (assuming S1 <= S2).
//   * Minimizing the difference is mathematically equivalent to maximizing S1 such that S1 <= Total / 2.
//   * Standard 0/1 knapsack evaluates boolean reachability of every sum up to Total / 2 in reverse order to prevent re-using elements.
//   * The largest reachable sum s closest to Total / 2 yields the minimal possible discrepancy Total - 2 * s.
// - Complexity: Time: O(N * Total), Space: O(Total).


// ============================================================
// 2. COUNT OF LONGEST INCREASING SUBSEQUENCES — LeetCode 673
// ============================================================

int findNumberOfLIS(vi& a) {
    int n = a.size();
    if (n == 0) return 0;
    vi len(n, 1), cnt(n, 1);
    int mx = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    cnt[i] = cnt[j];
                } else if (len[j] + 1 == len[i]) {
                    cnt[i] += cnt[j];
                }
            }
        }
        mx = max(mx, len[i]);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == mx) ans += cnt[i];
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find number of longest increasing subsequences in an array (LeetCode 673).
// - Approach: Dual 1D DP arrays (len[i] and cnt[i]).
// - Intuition:
//   * Tracking LIS length alone is insufficient; we must simultaneously track how many distinct paths achieve that maximal length ending at each index.
//   * For each pair (j, i) with j < i and a[j] < a[i]:
//     - If len[j] + 1 > len[i], a strictly longer sequence ending at i is found, resetting cnt[i] = cnt[j].
//     - If len[j] + 1 == len[i], an alternative path of equal maximal length is discovered, accumulating cnt[i] += cnt[j].
//   * Global answer is the sum of cnt[i] across all indices i where len[i] matches the global maximum mx.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 3. SHORTEST COMMON SUPERSEQUENCE (SCS) LENGTH
// ============================================================
// SCS means the shortest string that has both s and t as subsequences. The length of SCS can be derived from the lengths of the two strings and their longest common subsequence (LCS).
// formula = len(s) + len(t) - len(LCS(s, t))
int scsLength(string s, string t) {
    int m = s.size(), n = t.size();
    vvi dp(m + 1, vi(n + 1, 0)); // dp[i][j] = length of LCS of s[0..i-1] and t[0..j-1]
    int i=n, j=m;
    while(i>0 && j>0){
        if(s[i-1]==t[j-1]){
            dp[i][j] = 1 + dp[i-1][j-1];
            i--; j--;
        }else{
            dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            if(dp[i-1][j]>dp[i][j-1]) i--;
            else j--;
        }
    }
    return m + n - dp[m][n];
}
// Interview Explanation:
// - Problem Statement: Find length of shortest common supersequence of two strings.
// - Approach: Mathematical reduction to Longest Common Subsequence (LCS).
// - Intuition:
//   * Any common supersequence must contain all characters of s and all characters of t while preserving their order.
//   * Overlapping characters that appear in both strings in the same order need only be written once.
//   * The maximum number of characters that can be shared without conflict is precisely the LCS(s, t).
//   * By Inclusion-Exclusion on character multiset positions: Length(SCS) = len(s) + len(t) - LCS(s, t).
// - Complexity: Time: O(N * M), Space: O(N * M).


// ============================================================
// 4. RECONSTRUCT SHORTEST COMMON SUPERSEQUENCE — LeetCode 1092
// ============================================================

string shortestCommonSupersequence(string s, string t) {
    int m = s.size(), n = t.size();
    vvi dp(m + 1, vi(n + 1, 0));
    int i = m, j = n;
    while(i > 0 && j > 0) {
        if (s[i - 1] == t[j - 1]) {
            dp[i][j] = 1 + dp[i - 1][j - 1];
            i--; j--;
        } else {
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            if (dp[i - 1][j] > dp[i][j - 1]) i--;
            else j--;
        }
    }
    int i = m, j = n;
    string ans = "";
    while (i > 0 && j > 0) {
        if (s[i - 1] == t[j - 1]) {
            ans += s[i - 1]; i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            ans += s[i - 1]; i--;
        } else {
            ans += t[j - 1]; j--;
        }
    }
    while (i > 0) ans += s[--i]; // append remaining characters from s if any
    while (j > 0) ans += t[--j]; // append remaining characters from t if any
    reverse(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Construct the shortest string that has both s and t as subsequences (LeetCode 1092).
// - Approach: 2D LCS Table Backtracking.
// - Intuition:
//   * Compute standard 2D LCS table dp[m+1][n+1].
//   * Backtrack from (m, n) toward (0, 0):
//     - If s[i-1] == t[j-1], this character belongs to the LCS; append it once and move diagonally (i--, j--).
//     - Otherwise, follow the optimal path: if dp[i-1][j] > dp[i][j-1], take s[i-1] and move up; else take t[j-1] and move left.
//   * Append remaining characters from whichever string has unconsumed prefix, then reverse the collected string.
// - Complexity: Time: O(N * M), Space: O(N * M).


// ============================================================
// 5. MINIMUM WINDOW SUBSEQUENCE — LeetCode 727
// ============================================================

string minWindowSubsequence(string s, string t) {
    int m = s.size(), n = t.size();
    int minL = INT_MAX, st = -1;
    int i = 0, j = 0;

    while (i < m) {
        if (s[i] == t[j]) {
            j++;
            if (j == n) {
                int r = i;
                j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                i++;
                j = 0;
                if (r - i + 1 < minL) {
                    minL = r - i + 1;
                    st = i;
                }
            }
        }
        i++;
    }
    return st == -1 ? "" : s.substr(st, minL);
}
// Interview Explanation:
// - Problem Statement: Find shortest contiguous substring of s containing t as a subsequence (LeetCode 727).
// - Approach: Forward Scan Match + Backward Greedy Contraction.
// - Intuition:
//   * Scanning s forward, when t[n-1] is matched at index r, we have identified a candidate valid window ending at r.
//   * To minimize the window length, backtrack right-to-left from r to locate the latest possible starting index that still covers all characters of t in reverse.
//   * This contraction establishes the tightest valid window ending at r.
//   * Reset t's pointer to 0 and resume the forward scan from the compacted start index + 1 to discover shorter subsequent windows.
// - Complexity: Time: O(N * M), Space: O(1).


// ============================================================
// 6. MATRIX CHAIN MULTIPLICATION
// ============================================================

int matrixChainMultiplication(const vi& a) {
    int n = a.size() - 1;
    vvi dp(n, vi(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) { // why till j-1 , because we are splitting the chain into two parts, and k is the last matrix of the first part, so it should be less than j
                int c = dp[i][k] + dp[k + 1][j] + a[i] * a[k + 1] * a[j + 1];
                dp[i][j] = min(dp[i][j], c);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find optimal parenthesization of matrix chain to minimize scalar multiplications.
// - Approach: Interval DP iterating over chain lengths.
// - Intuition:
//   * Multiplying matrices of dimensions (r x k) and (k x c) costs r * k * c scalar multiplications.
//   * For chain from matrix i to j, consider partitioning into two subchains (i..k) and (k+1..j) at split point k.
//   * Total cost is dp[i][k] + dp[k+1][j] + cost of combining the two resulting matrices (a[i] * a[k+1] * a[j+1]).
//   * Solving smaller chain lengths len = 2..n first guarantees optimal subproblems are ready when evaluating larger intervals.
// - Complexity: Time: O(N^3), Space: O(N^2).


// ============================================================
// 7. BOUNDED KNAPSACK (BINARY POWER SPLITTING)
// ============================================================

int boundedKnapsack(int W, const vi& wt, const vi& val, const vi& cnt) {
    vi dp(W + 1, 0);
    int n = wt.size();

    for (int i = 0; i < n; i++) {
        int c = cnt[i];
        for (int k = 1; c > 0; k <<= 1) {
            int take = min(k, c);
            int itemW = take * wt[i];
            int itemV = take * val[i];
            for (int w = W; w >= itemW; w--) {
                dp[w] = max(dp[w], dp[w - itemW] + itemV);
            }
            c -= take;
        }
    }
    return dp[W];
}
// Interview Explanation:
// - Problem Statement: 0/1 Knapsack with bounded item counts cnt[i].
// - Approach: Binary Decomposition (1, 2, 4, ..., remainder) + 0/1 Knapsack.
// - Intuition:
//   * Naively unpacking each count into individual items yields O(W * sum(cnt)) time, which is too slow.
//   * Any integer C can be uniquely partitioned into powers of two: {1, 2, 4, ..., 2^p, R}.
//   * Any subset of items from 1 to C can be represented as a subset combination of these binary bundled items.
//   * Bundling reduces the number of items per type from C to O(log C), allowing standard 1D reverse 0/1 knapsack DP.
// - Complexity: Time: O(W * sum(log K)), Space: O(W).


// ============================================================
// 8. LCS LENGTH OF 2 PERMUTATIONS (REDUCTION TO LIS)
// ============================================================

int lcsOfPermutations(const vi& a, const vi& b) {
    int n = a.size();
    unordered_map<int, int> pos;
    for (int i = 0; i < n; i++) pos[a[i]] = i;

    vi mapped;
    for (int x : b) {
        if (pos.count(x)) mapped.push_back(pos[x]);
    }

    vi lis;
    for (int x : mapped) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return lis.size();
}
// Interview Explanation:
// - Problem Statement: Find LCS of two permutations of numbers 1..N in O(N log N) time.
// - Approach: Permutation Index Mapping + Patience Sorting LIS.
// - Intuition:
//   * Standard LCS takes O(N^2), but permutations contain unique elements.
//   * Record the index of each element in a into hash map pos. Replace each element in b with its index in a.
//   * A common subsequence corresponds to indices in a that appear in strictly increasing order in b.
//   * The problem reduces directly to finding the Longest Increasing Subsequence (LIS) on the transformed array via binary search (patience sorting) in O(N log N).
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 9. LONGEST COMMON INCREASING SUBSEQUENCE (LCIS)
// ============================================================

int LCIS(vi& a, vi& b) {
    int n = a.size(), m = b.size();
    vi dp(m, 0); // dp[j] = length of LCIS ending at b[j]
    int i=0;
    while(i<n){
        int cur = 0;
        int j=0;
        while(j<m){
            if(a[i]==b[j]){
                dp[j] = cur + 1;
            }else if(a[i]>b[j]){
                cur = max(cur, dp[j]);
            }
            j++;
        }
        i++;
    }
    return *max_element(dp.begin(), dp.end());
}
// Interview Explanation:
// - Problem Statement: Find the longest common subsequence of two arrays that is also strictly increasing.
// - Approach: 1D Dynamic Programming with running optimal prefix tracker.
// - Intuition:
//   * Let dp[j] be the length of the LCIS ending at b[j].
//   * When processing a[i], as j increases, we track cur = max(dp[k]) for all k < j where b[k] < a[i].
//   * If a[i] == b[j], we can append b[j] to the best previously seen increasing subsequence, updating dp[j] = cur + 1.
//   * If a[i] > b[j], b[j] is a valid candidate predecessor for a subsequent match, so update cur = max(cur, dp[j]).
// - Complexity: Time: O(N * M), Space: O(M).


// ============================================================
// 10. MIN MOVES TO GATHER K CONSECUTIVE ONES — LeetCode 1703
// ============================================================
int minMoves(vector<int>& nums, int k) {
        // Store positions of all 1's.
        vector<long long> pos;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == 1)
                pos.push_back(i);
        }

        int m = pos.size();

        // adjusted[i] = pos[i] - i
        //
        // If the target consecutive block starts at x, then
        // desired positions are:
        //      x, x+1, x+2, ...
        //
        // Cost:
        //      |pos[i] - (x+i)|
        //
        // Rearranging:
        //      |(pos[i]-i) - x|
        //
        // Thus, after subtracting the index, every element wants
        // to move to the SAME value x, whose optimum is the median.
        vector<long long> adjusted(m);
        for (int i = 0; i < m; i++)
            adjusted[i] = pos[i] - i;

        // Prefix sums of adjusted[] for O(1) range sum queries.
        vector<long long> prefix(m + 1, 0);
        for (int i = 0; i < m; i++)
            prefix[i + 1] = prefix[i] + adjusted[i];

        long long ans = LLONG_MAX;

        // Try every consecutive group of k ones.
        for (int left = 0; left + k <= m; left++) {
            int right = left + k - 1;
            int mid = (left + right) / 2;

            long long median = adjusted[mid];

            // Cost to move all elements on the left of median.
            // Σ (median - adjusted[i])
            long long leftCost =
                median * (mid - left) -
                (prefix[mid] - prefix[left]);

            // Cost to move all elements on the right of median.
            //
            // Σ (adjusted[i] - median)
            long long rightCost =
                (prefix[right + 1] - prefix[mid + 1]) -
                median * (right - mid);

            ans = min(ans, leftCost + rightCost);
        }

        return (int)ans;
}
// Interview Explanation:
// - Problem Statement: Minimum moves to gather any k ones together consecutively (LeetCode 1703).
// - Approach: Index Shift Transformation + Median Cost using Prefix Sums.
// - Intuition:
//   * Gathering k ones at consecutive indices [x, x+1, ..., x+k-1] means original indices pos[j] move to x + j.
//   * Substituting p[j] = pos[j] - j simplifies condition: all p[j] must be gathered to the same value x.
//   * Minimizing sum of absolute distances |p[j] - x| for a window of size k is minimized when x is the median of p[i..i+k-1].
//   * Sliding window prefix sums evaluate left and right deviations from the median in O(1) per window.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 11. 2D PREFIX SUM MATRIX
// ============================================================

struct NumMatrix {
    vvi pref;
    NumMatrix(vvi& m) {
        int r = m.size(), c = m[0].size();
        pref.assign(r + 1, vi(c + 1, 0)); // pref[i][j] = sum of rectangle [0..i-1, 0..j-1]
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                pref[i + 1][j + 1] = m[i][j] + pref[i][j + 1] + pref[i + 1][j] - pref[i][j];
            }
        }
    }
    int sumRegion(int r1, int c1, int r2, int c2) {
        return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
    } // sum of rectangle [r1..r2, c1..c2] where r1 <= r2 and c1 <= c2 and are 0-indexed
};
// Interview Explanation:
// - Problem Statement: Query sum of submatrix [r1, c1] to [r2, c2] in O(1) time.
// - Approach: 2D Inclusion-Exclusion Prefix Sum Table.
// - Intuition:
//   * Precompute 2D cumulative prefix matrix where pref[i+1][j+1] stores sum of rectangle [0..i, 0..j].
//   * Build recurrence: pref[i+1][j+1] = m[i][j] + pref[i][j+1] + pref[i+1][j] - pref[i][j].
//   * Query recurrence: pref[r2+1][c2+1] - pref[r1][c2+1] - pref[r2+1][c1] + pref[r1][c1].
//   * Subtracts top and left outside regions while adding back the double-subtracted top-left intersection.
// - Complexity: Time: Build O(R * C), Query O(1); Space: O(R * C).


// ============================================================
// 12. MAKE ARRAY NON-DECREASING (SLOPE TRICK)
// ============================================================

long long minOperationsNonDecreasing(vi& a) {
    long long ans = 0;
    priority_queue<int> pq;
    for (int x : a) {
        if (!pq.empty() && pq.top() > x) {
            ans += pq.top() - x;
            pq.pop();
            pq.push(x);
        }
        pq.push(x);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Minimum operations (+1/-1) to make array non-decreasing.
// - Approach: Slope Trick with Max-Heap.
// - Intuition:
//   * Let f_i(v) be the minimum cost to make prefix non-decreasing ending with value <= v. f_i is a convex piecewise-linear function.
//   * Adding |x - v| shifts the slope by +1. The inflection points where slope transitions represent optimal choices.
//   * A max-heap tracks inflection points to the left of the minimum.
//   * If current element x is strictly smaller than pq.top(), the cost increases by pq.top() - x, and pq.top() is adjusted down to x.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 13. MAX SUBARRAY SUM OF LENGTH <= K (CSES II)
// ============================================================

ll maxSubarraySumAtMostK(int n, int k, const vl& a) {
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
    deque<int> dq;
    dq.push_back(0);
    ll ans = LLONG_MIN;
    for (int r = 1; r <= n; r++) {
        int l = r - k;
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        if (!dq.empty()) ans = max(ans, pref[r] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[r])dq.pop_back();
        dq.push_back(r);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum subarray sum with length at most K (CSES Max Subarray Sum II).
// - Approach: Monotonic Deque on Prefix Sums.
// - Intuition:
//   * Any subarray sum ending at index i has sum pref[i] - pref[j] where i - k <= j < i.
//   * To maximize pref[i] - pref[j], we need the minimum pref[j] in the sliding window [i - k, i - 1].
//   * A monotonic deque maintains candidate indices j in increasing order of their pref[j] values.
//   * Pop stale indices j < i - k from front, read optimal pref[dq.front()], and maintain monotonicity by popping larger back values before inserting i.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 14. LEXICOGRAPHICAL RANK OF A PERMUTATION
// ============================================================

ll permutationRank(const vi& a) {
    int n = a.size();
    vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = (fact[i - 1] * i) % MOD;

    ll ans = 1;
    for (int i = 0; i < n; i++) {
        int cnt = 0;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[i]) cnt++;
        }
        ans = (ans + cnt * fact[n - 1 - i]) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find 1-based lexicographical rank of a permutation among all permutations of length N.
// - Approach: Factorial Number System (Lehmer code conversion).
// - Intuition:
//   * For position i, any remaining available element strictly smaller than a[i] could be placed at index i to create a lexicographically smaller permutation.
//   * If cnt elements after index i are smaller than a[i], each such choice prefixes (n - 1 - i)! full permutations that appear before the current one.
//   * Multiply cnt by fact[n - 1 - i] and accumulate modulo MOD.
//   * Add 1 at the end to convert from 0-based count of strictly preceding permutations to 1-based rank.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 15. LCM OF ARRAY ELEMENTS MODULO MOD
// ============================================================

ll arrayLCMMod(const vi& a) {
    map<int, int> mxP;
    for (int x : a) {
        int tmp = x;
        for (int p = 2; p * p <= tmp; p++) {
            if (tmp % p == 0) {
                int cnt = 0;
                while (tmp % p == 0) { cnt++; tmp /= p; }
                mxP[p] = max(mxP[p], cnt);
            }
        }
        if (tmp > 1) mxP[tmp] = max(mxP[tmp], 1);
    }
    ll ans = 1;
    for (auto& [p, pw] : mxP) {
        for (int i = 0; i < pw; i++) ans = (ans * p) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Compute LCM of all array elements modulo (1e9 + 7).
// - Approach: Prime factorization and max-exponent tracking.
// - Intuition:
//   * Computing pairwise LCM via (a * b) / gcd(a, b) fails under modulo because division is not compatible with intermediate modulo operations.
//   * The true mathematical definition of LCM(a_1, ..., a_n) is the product of p^{max_k(v_p(a_k))} across all primes p.
//   * Factorize each number in O(sqrt(A)), tracking the maximum exponent observed for each prime factor globally.
//   * Multiply each prime p to its maximum exponent modulo (1e9 + 7).
// - Complexity: Time: O(N * sqrt(max_A)), Space: O(Unique Primes).


// ============================================================
// 16. PERMUTATION ROUNDS (LCM OF CYCLES — CSES 3398)
// ============================================================

ll permutationRounds(const vi& a) {
    int n = a.size();
    vector<bool> vis(n, false);
    map<int, int> mxP;

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            int len = 0, cur = i;
            while (!vis[cur]) { vis[cur] = true; cur = a[cur] - 1; len++; }
            int tmp = len;
            for (int p = 2; p * p <= tmp; p++) {
                if (tmp % p == 0) {
                    int cnt = 0;
                    while (tmp % p == 0) { cnt++; tmp /= p; }
                    mxP[p] = max(mxP[p], cnt);
                }
            }
            if (tmp > 1) mxP[tmp] = max(mxP[tmp], 1);
        }
    }
    ll ans = 1;
    for (auto& [p, pw] : mxP) {
        for (int i = 0; i < pw; i++) ans = (ans * p) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find number of rounds until permutation returns to identity (CSES 3398).
// - Approach: Disjoint cycle decomposition + modular LCM.
// - Intuition:
//   * Every permutation decomposes uniquely into a set of disjoint cyclic orbits.
//   * All elements in a cycle of length L return to their start positions in exactly L steps (and every multiple of L).
//   * Therefore, the entire permutation returns to its identity state after a number of rounds equal to the LCM of all cycle lengths.
//   * Prime factorize each cycle length and track global maximum prime powers to compute LCM modulo 1e9+7.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 17. MIN MOVE-TO-ANYWHERE OPERATIONS (N - LIS)
// ============================================================

int minMoveToAnywhereToSort(const vi& a) {
    vi lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return a.size() - lis.size();
}
// Interview Explanation:
// - Problem Statement: Find minimum operations to sort array if any element can be moved to any position.
// - Approach: Compute Longest Increasing Subsequence (LIS); answer is N - LIS.size().
// - Intuition:
//   * Any elements that are already in strictly increasing relative order can remain fixed in place.
//   * All other elements must be moved at least once to fit into their correct sorted slots.
//   * To minimize moved elements, we must maximize the number of stationary elements.
//   * Stationary elements form an increasing subsequence, so the optimal choice is the Longest Increasing Subsequence (LIS).
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 18. MIN MOVE-TO-FRONT OPERATIONS (N...1)
// ============================================================

int minMoveToFrontToSort(const vi& a) {
    int n = a.size();
    int exp = n;
    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == exp) exp--;
    }
    return exp;
}
// Interview Explanation:
// - Problem Statement: Minimum operations to sort permutation [1...N] if elements can only be moved to the front.
// - Approach: Greedy backwards suffix match: search for N, N-1, N-2... from right to left.
// - Intuition:
//   * Moving an element to front places it before all currently sorted elements.
//   * If we move elements in descending order (e.g., 3, then 2, then 1), any element moved can be properly positioned.
//   * Any suffix of numbers that are already in correct descending relative order (N, N-1, ...) right-to-left never need to be moved.
//   * All other elements must be picked and moved to the front.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 19. MIN SUBARRAYS WITH SUM <= K
// ============================================================

int minSubarraysSumAtMostK(const vi& a, ll k) {
    int ans = 1;
    ll sum = 0;
    for (int x : a) {
        if (sum + x <= k) {
            sum += x;
        } else {
            ans++;
            sum = x;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Greedy partition of non-negative array into minimum contiguous subarrays each with sum <= k.
// - Approach: Greedy linear accumulation.
// - Intuition:
//   * Since all array elements are non-negative, extending the current subarray is always optimal until the sum exceeds k.
//   * Stopping early would only force subsequent subarrays to start earlier, never reducing total count.
//   * When adding x exceeds k, close the current subarray, increment count, and start a new subarray with x.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 20. HOUSE ROBBER IV — LeetCode 2560
// ============================================================
bool canRob(vector<int>& nums, int k, int capability) {
        int robbed = 0;
        int n = nums.size();

        for (int i = 0; i < n; ) {
            // If current house can be robbed under this capability,
            // greedily rob it and skip the adjacent house.
            if (nums[i] <= capability) {
                robbed++;
                i += 2;
            } else {
                i++;
            }
        }

        return robbed >= k; // if we can rob at least k houses, return true.
    }

    int minCapability(vector<int>& nums, int k) {
        int left = *min_element(nums.begin(), nums.end());
        int right = *max_element(nums.begin(), nums.end());

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (canRob(nums, k, mid)) {
                right = mid;      // Try a smaller capability.
            } else {
                left = mid + 1;   // Capability too small.
            }
        }

        return left;
    }
// Interview Explanation:
// - Problem Statement: Find minimum capability to rob at least k non-adjacent houses (LeetCode 2560).
// - Approach: Binary Search on Answer + Greedy Non-Adjacent Count.
// - Intuition:
//   * Capability is the maximum money stolen from any single house. Lower capability is harder to achieve.
//   * The predicate "can we rob >= k non-adjacent houses with individual values <= mid" is monotonic with respect to mid.
//   * Verification runs greedily: rob house i if a[i] <= mid, then skip house i + 1 (i++), which maximizes houses robbed.
//   * Binary search narrows the capability range [min(a), max(a)] in O(log(range)) iterations.
// - Complexity: Time: O(N log(max - min)), Space: O(1).


// ============================================================
// 21. BOUNDED COIN CHANGE (LIMITED SUPPLY)
// ============================================================

int coinChangeBounded(vi& c, vi& lim, int t) {
    vi dp(t + 1, 1e9);
    dp[0] = 0;
    for (int i = 0; i < (int)c.size(); i++) {
        int val = c[i], cnt = lim[i];
        for (int k = 1; cnt > 0; k <<= 1) {
            int take = min(k, cnt);
            int wt = take * val, cost = take;
            for (int x = t; x >= wt; x--) dp[x] = min(dp[x], dp[x - wt] + cost);
            cnt -= take;
        }
    }
    return dp[t] >= 1e9 ? -1 : dp[t];
}
// Interview Explanation:
// - Problem Statement: Find minimum coins to make amount where coin[i] has limited count lim[i].
// - Approach: Binary Power Splitting + 0/1 Knapsack Backwards DP.
// - Intuition:
//   * An unbounded supply allows forward 1D DP, but limited supply would require 2D state or deque optimization.
//   * Decomposing each limit into binary bundles {1, 2, 4, ...} transforms the problem into standard 0/1 knapsack.
//   * Bundle of size take has coin weight = take * val and cost = take coins.
//   * Iterating x backwards from amount down to weight avoids multiple usage of the same bundle.
// - Complexity: Time: O(amount * sum(log limit)), Space: O(amount).


// ============================================================
// 22. COIN CHANGE PATH RECONSTRUCTION (PRINT COINS)
// ============================================================

vi reconstructCoins(vi& c, int t) {
    vi dp(t + 1, 1e9), par(t + 1, -1);
    dp[0] = 0;
    for (int coin : c) {
        for (int x = coin; x <= t; x++) { 
            if (dp[x - coin] + 1 < dp[x]) {
                dp[x] = dp[x - coin] + 1;
                par[x] = coin;
            }
        }
    }
    if (dp[t] >= 1e9) return {};
    vi ans;
    for (int cur = t; cur > 0; cur -= par[cur]) ans.push_back(par[cur]);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Reconstruct and return the exact coin denominations used to make amount with fewest coins.
// - Approach: DP Predecessor Tracking + Backtracking Path Recovery.
// - Intuition:
//   * Standard coin change computes minimum coins to reach amount t using unbounded knapsack.
//   * Maintain par[x] recording the denomination of the coin that yielded the optimal transition into state x.
//   * If dp[t] is unreachable (>= 1e9), return empty vector.
//   * Otherwise, trace backwards from t: repeatedly append par[cur] and subtract it from cur until cur reaches 0.
// - Complexity: Time: O(N * amount), Space: O(amount).
