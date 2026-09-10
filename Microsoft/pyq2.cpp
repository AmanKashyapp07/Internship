#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#endif
using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/*
 ====================================================================================================
                             MICROSOFT OA DSA PROBLEM SHEET (PYQ 2)
                                 SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern to Remember               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 01 | Longest Palindromic Subsequence Length      | Interval DP (2-Row Rolling Array) | O(N²)    | O(N)     |
 | 02 | Number of Longest Increasing Subsequences   | DP + Count Pair Tracking          | O(N²)    | O(N)     |
 | 03 | Count Substrings Containing All Three Letters| Last Seen Pointers Min-Offset    | O(N)     | O(1)     |
 | 04 | Lexicographically Smallest Encrypted String | Monotonic Stack + Suffix Minima   | O(N)     | O(N)     |
 | 05 | Asteroid Belt Survivors (LC 735)            | Directional Stack Collision       | O(N)     | O(N)     |
 | 06 | Sum of Every Student's Region Length        | Monotonic Stack (Next Greater)    | O(N)     | O(N)     |
 | 07 | Minimum Operations to Reduce Sum by Half    | Greedy Max-Heap Halving           | O(K logN)| O(N)     |
 | 08 | Count Palindromic Subsequences              | Inclusion-Exclusion Interval DP   | O(N²)    | O(N²)    |
 | 09 | Maximum Products Alex Can Buy               | Prefix Sum + Binary Search        | O(N+QlgN)| O(N)     |
 | 10 | Count Intervals Covering a Point            | Endpoint Sorting + Two Searches   | O((N+Q)lg| O(N)     |
 | 11 | Light Panel Toggle Sum                      | Difference Array (Prefix XOR)     | O(N + M) | O(M)     |
 | 12 | Minimum Cost to Combine Array Into One      | Huffman Greedy Min-Heap Merge     | O(N logN)| O(N)     |
 | 13 | Palindromic Pair Count                      | 26-Bit Parity Mask + HashMap      | O(26N)   | O(N)     |
 | 14 | Worth Taking on a Trip                      | Substring Pattern Matching         | O(N)     | O(1)     |
 | 15 | Minimum Total Complexity Over Days (LC 1335)| Partition DP with Suffix Range Max| O(D × N²)| O(D × N) |
 | 16 | Minimum Flips to Remove Every '10' Pair     | 2-State Dynamic Programming       | O(N)     | O(1)     |
 | 17 | Valid Bracket Sequence With At Most 1 Swap  | Prefix Balance & Min Deficit Check| O(N)     | O(1)     |
 | 18 | Maximum Minimum Magnetic Force (LC 1552)    | Binary Search on Answer + Greedy  | O(N logR)| O(1)     |
 | 19 | Minimum Array Cost With Unlimited Insertions| Convexity Property (Unit Steps)   | O(N)     | O(1)     |
 | 20 | Weighted Manhattan Distance City            | 1D Weighted Median Decomposition  | O(N logN)| O(N)     |
 | 21 | Maximum XOR with the Same Bit Length        | Prefix XOR Periodicity (Mod 4)    | O(1)     | O(1)     |
 | 22 | Minimum Array Cost After One Insertion      | Maximum Neighbor Variance Drop    | O(N)     | O(1)     |
 | 23 | Count Arrays Reachable by Threshold Decrement| Independent Coordinate Gaps       | O(N logN)| O(N)     |
 | 24 | Maximize Ones in a Binary String            | Greedy Left Propagation Reach     | O(N)     | O(1)     |
 | 25 | Minimize Array Sum After K Operations       | Max-Heap Halving (LC 1962)        | O((N+K)lg| O(N)     |
 | 26 | Longest Square Chain Subset (LC 2501)       | Hash Set Sequence Traversal (x²)  | O(N)     | O(N)     |
 | 27 | Authentication Token System (LC 1797)       | Hash Map Expiry Cache             | O(Q)     | O(Tokens)|
 | 28 | Minimum Cost to Reach Point N (Jump Game VI)| DP + Monotonic Deque Window Min   | O(N)     | O(N)     |
 | 29 | Ideal Numbers (3^x * 5^y)                   | Double Geometric Power Traversal  | O(1)     | O(1)     |
 | 30 | Maximum Bitwise OR-Sum (LC 2680)            | Prefix & Suffix Bitwise OR Sweep  | O(N)     | O(N)     |
 | 31 | Shopkeeper Sale – Final Price of Items      | Next Smaller/Equal Monotonic Stack| O(N)     | O(N)     |
 | 32 | Count Palindromic Subsequences of Length 5  | Prefix/Suffix Pair Frequencies    | O(N)     | O(1)     |
 | 33 | Binary Cardinality Sort (LC 1356)           | Popcount Custom Sort Comparator   | O(N logN)| O(log N) |
 | 34 | Ball Collision Around Pivot Point           | Single-Pass Speed Threshold       | O(N)     | O(1)     |
 | 35 | Maximum Weighted Difference (1-based)       | Rearrangement Inequality          | O(N logN)| O(1)     |
 | 36 | Delete Odd-Valued Nodes from Linked List    | Sentinel Pointer Traversal        | O(N)     | O(1)     |
 | 37 | Shortest Substring Deletion for Distinct    | Prefix & Suffix Distinct Sets     | O(N)     | O(Σ)     |
 | 38 | Message Duplicate Detection Rate Limiter    | Hash Map Timestamp Cache (LC 359) | O(N)     | O(U)     |
 | 39 | Maximize XOR by Reordering One Binary String| Greedy Most Significant Bit XOR   | O(N)     | O(N)     |
 | 40 | Evaluate Signed Integer Arithmetic Expr     | Recursive Descent Parser (AST)    | O(N)     | O(N)     |
 | 41 | Validate Org Hierarchy & Report Depth       | Kahn's Topological Sort + BFS Level| O(N)     | O(N)     |
 | 42 | Most Visited Sectors of a Circular Track    | Circular Difference Array (Sweep) | O(N + M) | O(N)     |
 ====================================================================================================
*/

// ====================================================================================================
// 1. LONGEST PALINDROMIC SUBSEQUENCE LENGTH [LC 516]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s`, find the maximum length of a subsequence that reads the same forward and backward.

  PATTERN TO REMEMBER:
  Interval DP (2-Row Rolling Array Optimization)

  CORE INTUITION & STEPS:
  - Let dp[i][j] = longest palindromic subsequence in substring s[i..j].
  - If s[i] == s[j]: dp[i][j] = 2 + dp[i+1][j-1].
  - If s[i] != s[j]: dp[i][j] = max(dp[i+1][j], dp[i][j-1]).
  - Only two rows (or 1D array) are needed since dp[i] depends only on row i+1.

  COMPLEXITY:
  - Time:  O(N²)
  - Space: O(N) space optimized

  CONCLUSION / TAKEAWAY:
  Match outer endpoints: if equal, add 2 and peel inwards; if different, take max of dropping either endpoint.
*/
class Solution1 {
  public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for(int i=0;i<n;i++) dp[i][i] = 1; // single character is a palindrome of length 1
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i + len - 1 < n; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j])
                    dp[i][j] = 2 + (len == 2 ? 0 : dp[i + 1][j - 1]);
                else
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }

        return dp[0][n - 1];
    }
};

// ====================================================================================================
// 2. NUMBER OF LONGEST STRICTLY INCREASING SUBSEQUENCES [LC 673]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an integer array `nums`, return the total count of longest strictly increasing subsequences.

  PATTERN TO REMEMBER:
  Dynamic Programming with Length + Count Tracking

  CORE INTUITION & STEPS:
  - Maintain `len[i]` (length of LIS ending at index i) and `cnt[i]` (number of such LIS ending at i).
  - For each j < i where nums[j] < nums[i]:
    * If len[j] + 1 > len[i]: new strictly longer length found, update len[i] = len[j] + 1, cnt[i] = cnt[j].
    * If len[j] + 1 == len[i]: alternative path of identical length found, accumulate cnt[i] += cnt[j].
  - Sum `cnt[i]` across all indices achieving the global maximum length.

  COMPLEXITY:
  - Time:  O(N²)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Whenever counting optimal DP solutions, maintain a parallel `count` array alongside the optimal value array.
*/
class Solution2 {
public:
    int findNumberOfLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return n;
        vector<int> len(n, 1), cnt(n, 1);
        int maxLen = 1;
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] > nums[j]) {
                    if (len[j] + 1 > len[i]) {
                        len[i] = len[j] + 1;
                        cnt[i] = cnt[j];
                    } else if (len[j] + 1 == len[i]) {
                        cnt[i] += cnt[j];
                    }
                }
            }
            maxLen = max(maxLen, len[i]);
        }
        int total = 0;
        for (int i = 0; i < n; ++i) {
            if (len[i] == maxLen) total += cnt[i];
        }
        return total;
    }
};

// ====================================================================================================
// 3. COUNT SUBSTRINGS CONTAINING ALL THREE LETTERS [LC 1358]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given string `s` containing only 'a', 'b', and 'c', return the number of substrings containing
  at least one occurrence of all three characters.

  PATTERN TO REMEMBER:
  Last Occurrence Tracking / Sliding Window Min-Offset

  CORE INTUITION & STEPS:
  - Track the last seen 0-based index of 'a', 'b', and 'c': `last[3]`.
  - For each index `i`, update `last[s[i] - 'a'] = i`.
  - Any valid substring ending at `i` can start at any index from 0 up to min(last['a'], last['b'], last['c']).
  - Add `min({last[0], last[1], last[2]}) + 1` to answer at each step.

  COMPLEXITY:
  - Time:  O(N) single pass
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  A window ending at `i` contains all required characters if its start index is <= min(last_positions).
  Count of valid prefixes ending at `i` is min(last) + 1.
*/
class Solution3 {
public:
    long long countCompleteSubstrings(const string& s) {
        vector<int> last(3, -1);
        long long ans = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            last[s[i] - 'a'] = i;
            ans += min({last[0], last[1], last[2]}) + 1;
        }
        return ans;
    }
};

// ====================================================================================================
// 4. LEXICOGRAPHICALLY SMALLEST ENCRYPTED STRING [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Move characters from `original` to `temp` stack, or pop from `temp` to output `encrypted`.
  Return the lexicographically smallest output string possible.

  PATTERN TO REMEMBER:
  Monotonic Stack + Suffix Minima Greedy Flushing

  CORE INTUITION & STEPS:
  - Precompute suffix minimums `mn[i] = min(s[i], mn[i+1])`.
  - Push each char to temporary stack.
  - While stack top <= suffix minimum of remaining characters, pop stack top to output greedily.
  - Flush leftover stack elements at the end.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Simulate the stack; greedily flush characters from the stack whenever no smaller character exists
  ahead in the unread suffix.
*/
class Solution4 {
public:
    string getEncryptedString(const string& s) {
        int n = s.size();
        vector<char> mn(n + 1, '{');
        for (int i = n - 1; i >= 0; --i) {
            mn[i] = min(s[i], mn[i + 1]); // for each index, store the minimum character in the suffix starting at i
        }
        string st, ans;
        for (int i = 0; i < n; ++i) {
            st += s[i];
            while (!st.empty() && st.back() <= mn[i + 1]) { // while the top of the stack is less than or equal to the minimum character in the remaining suffix, it means we can pop it to the output, because it is lexicographically smaller than any character that will come later
                ans += st.back();
                st.pop_back();
            }
        }
        while (!st.empty()) {
            ans += st.back();
            st.pop_back();
        }
        return ans;
    }
};

// ====================================================================================================
// 5. ASTEROID BELT SURVIVORS [LC 735]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Asteroids move right (> 0) or left (< 0) with speeds proportional to magnitude. When they meet,
  the smaller explodes; if equal, both explode. Find surviving asteroids.

  PATTERN TO REMEMBER:
  Directional Stack Collision Simulation

  CORE INTUITION & STEPS:
  - Collisions only occur when a left-moving asteroid (< 0) meets a preceding right-moving asteroid (> 0).
  - Use stack: for each asteroid `x`:
    * While `x < 0` and `st.top() > 0`: compare magnitudes. Pop if smaller, destroy both if equal, stop if larger.
    * If `x` survives all collisions, push to stack.

  COMPLEXITY:
  - Time:  O(N) amortized
  - Space: O(N) for stack

  CONCLUSION / TAKEAWAY:
  Stack simulates physical collisions cleanly: only opposite-direction encounters `(>0, <0)` trigger elimination.
*/
class Solution5 {
public:
    vector<long long> asteroidCollision(const vector<long long>& a) {
        vector<long long> st;
        for (long long x : a) {
            bool alive = true;
            while (alive && x < 0 && !st.empty() && st.back() > 0) {
                if (st.back() < -x) { // asteroid in stack is smaller, it explodes
                    st.pop_back();
                } else if (st.back() == -x) { // both asteroids are equal, both explode
                    st.pop_back();
                    alive = false;
                } else { // asteroid in stack is larger, current asteroid explodes
                    alive = false;
                }
            }
            if (alive) st.push_back(x); // if current asteroid survived all collisions, add it to the stack
        }
        return st;
    }
};

// ====================================================================================================
// 6. SUM OF EVERY STUDENT'S REGION LENGTH [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Student `i` has height `h[i]`. Their region is the maximal contiguous subarray containing `i` where
  `h[i]` is the strictly maximum height. Return sum of region lengths over all students.

  PATTERN TO REMEMBER:
  Monotonic Stack (Previous & Next Greater Element)

  CORE INTUITION & STEPS:
  - Region of index i is bounded by the nearest strictly greater element on the left `L[i]`
    and nearest strictly greater element on the right `R[i]`.
  - Region length for student i is `(R[i] - 1) - (L[i] + 1) + 1 = R[i] - L[i] - 1`.
  - Monotonic decreasing stack computes both boundaries in a single pass.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Each element's dominance interval extends strictly between its nearest strictly greater neighbors
  `[L+1, R-1]`; resolve all boundaries via monotonic stack.
*/
class Solution6 {
  public:
    long long sumOfRegionLengths(const vector<int> &heights) {
        int n = heights.size();
        long long total = 0;
        stack<int> st;
        for (int i = 0; i <= n; ++i) {
            while (!st.empty() && (i == n || heights[st.top()] < heights[i])) { // < because we want strictly greater, not equal or less
                int mid = st.top(); st.pop();
                int left = st.empty() ? -1 : st.top();
                int right = i;
                total += right - left - 1;
            }
            if (i < n)
                st.push(i);
        }
        return total;
    }
};

// ====================================================================================================
// 7. MINIMUM OPERATIONS TO REDUCE ARRAY SUM BY HALF [LC 2208]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Pick any element and replace it with half its value. Find minimum operations to reduce total array sum by at least 50%.

  PATTERN TO REMEMBER:
  Greedy Max-Heap Halving

  CORE INTUITION & STEPS:
  - To minimize operation count, maximize reduction per operation.
  - Halving value `x` saves `x / 2.0`. Always greedily halve the largest current element.
  - Store elements in a max priority queue, repeatedly halve top, until reduction >= total_sum / 2.0.

  COMPLEXITY:
  - Time:  O(K log N) where K is number of operations
  - Space: O(N) for priority queue

  CONCLUSION / TAKEAWAY:
  Always greedily halve the current maximal element using a priority queue to harvest the largest absolute sum drop.
*/
class Solution7 {
public:
    long long minOperations(const vector<long long>& nums) {
        priority_queue<double> pq;
        double totalSum = 0, reduced = 0;
        for (long long x : nums) {
            totalSum += x;
            pq.push(x);
        }
        double target = totalSum / 2.0;
        long long ops = 0;
        while (reduced < target) {
            double top = pq.top(); pq.pop();
            double half = top / 2.0;
            reduced += half;
            pq.push(half);
            ops++;
        }
        return ops;
    }
};
// ====================================================================================================
// 8A. COUNT ALL PALINDROMIC SUBSEQUENCES
// Different index selections count separately.
//
// dp[i][j] = number of palindromic subsequences in s[i..j]
//
// Time:  O(N²)
// Space: O(N²)
// ====================================================================================================

long long countPalindromicSubsequences(string s) {
    const long long MOD = 1e9 + 7;
    int n = s.size();
    if (!n) return 0;

    vector<vector<long long>> dp(n, vector<long long>(n)); // dp[i][j] = number of palindromic subsequences in s[i..j]

    for (int i = 0; i < n; i++)
        dp[i][i] = 1;

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;

            if (s[i] == s[j])
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] + 1; // if both match, add 1 for the new palindromic subsequence formed by s[i] and s[j]=
            else
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1]; // if they don't match, we take the union of the two ranges and subtract the intersection

            dp[i][j] = (dp[i][j] % MOD + MOD) % MOD;
        }
    }

    return dp[0][n - 1];
}

// ====================================================================================================
// 8B. COUNT DISTINCT PALINDROMIC SUBSEQUENCES [LC 730]
//
// dp[i][j] = number of DISTINCT palindromic subsequences in s[i..j]
//
// Time:  O(N²)
// Space: O(N²)
// ====================================================================================================

class Solution8B {
  public:
    int countPalindromicSubsequences(string s) {
        const int MOD = 1e9 + 7;
        int n = s.size();
        if (!n) return 0;

        vector<int> next(n, n), prev(n, -1), last(256, -1); // next[i] = next occurrence of s[i] after i, prev[i] = previous occurrence of s[i] before i, last[i] = last occurrence of character i

        for (int i = 0; i < n; i++) {
            prev[i] = last[s[i]]; // store the last occurrence of s[i] before index i
            last[s[i]] = i; // update the last occurrence of s[i] to be index i
        }

        fill(last.begin(), last.end(), n); // initialize last to n

        for (int i = n - 1; i >= 0; i--) {
            next[i] = last[s[i]]; // store the next occurrence of s[i] after index i
            last[s[i]] = i; // update the last occurrence of s[i] to be index i
        }

        vector<vector<long long>> dp(n, vector<long long>(n)); // dp[i][j] = number of distinct palindromic subsequences in s[i..j]

        for (int i = 0; i < n; i++) dp[i][i] = 1;

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                if (s[i] != s[j]) {
                    dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
                } else {
                    int l = next[i], r = prev[j]; // l = next occurrence of s[i] after i, r = previous occurrence of s[j] before j
                    long long mid = dp[i + 1][j - 1]; // number of distinct palindromic subsequences in s[i+1..j-1]

                    if (l > r) dp[i][j] = 2 * mid + 2; 
                    else if (l == r) dp[i][j] = 2 * mid + 1;
                    else dp[i][j] = 2 * mid - dp[l + 1][r - 1];
                }
                dp[i][j] = (dp[i][j] % MOD + MOD) % MOD;
            }
        }

        return dp[0][n - 1];
    }
};

// ====================================================================================================
// 9. MAXIMUM PRODUCTS ALEX CAN BUY [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given sorted prices[1..n], each query gives (pos, budget). Alex can buy products from cubicle `pos`
  onwards up to budget. Find the maximum products he can buy.

  PATTERN TO REMEMBER:
  Prefix Sum + Binary Search (upper_bound)

  CORE INTUITION & STEPS:
  - Since prices are non-decreasing and positive, buying greedily from `pos` forward maximizes total count.
  - Range cost from `l` to `r` is `prefix[r+1] - prefix[l] <= budget` <=> `prefix[r+1] <= prefix[l] + budget`.
  - Use `upper_bound` on prefix sums to find rightmost affordable endpoint in O(log N).

  COMPLEXITY:
  - Time:  O(N + Q log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Monotonic positive costs allow greedy forward acquisition; evaluate max range reach using prefix sum upper_bound.
*/
class Solution9 {
public:
    vector<int> maximumProducts(const vector<long long>& prices, const vector<pair<int, long long>>& queries) {
        int n = prices.size();
        vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + prices[i];
        vector<int> results;
        results.reserve(queries.size());
        for (const auto& [pos, money] : queries) {
            int l = pos - 1; // Convert 1-based to 0-based index
            long long limit = pre[l] + money; // Maximum prefix sum we can afford
            int r = upper_bound(pre.begin() + l, pre.end(), limit) - pre.begin() - 1; // find the rightmost index where prefix sum <= limit, startinf from l to end of pre
            results.push_back(r - l);
        }
        return results;
    }
};

// ====================================================================================================
// 10. COUNT INTERVALS COVERING A POINT [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given `N` closed intervals [L[i], R[i]], answer for each query point `x` how many intervals cover `x`.

  PATTERN TO REMEMBER:
  Endpoint Sorting + Binary Search Difference

  CORE INTUITION & STEPS:
  - An interval covers x iff L[i] <= x and R[i] >= x.
  - Count = (Intervals that started on or before x) - (Intervals that ended strictly before x).
  - Sort L and R independently: started = upper_bound(L, x), ended = lower_bound(R, x).
  - Answer = started - ended.

  COMPLEXITY:
  - Time:  O((N + Q) log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Point-in-interval counts decompose into: (starts <= x) minus (ends < x), each answered via independent binary searches.
*/
class Solution10 {
public:
    vector<int> countContainingIntervals(vector<int> L, vector<int> R, const vector<int>& queries) {
        sort(L.begin(), L.end());
        sort(R.begin(), R.end());
        vector<int> ans;
        ans.reserve(queries.size());
        for (int x : queries) {
            int started = upper_bound(L.begin(), L.end(), x) - L.begin();
            int ended = lower_bound(R.begin(), R.end(), x) - R.begin();
            ans.push_back(started - ended);
        }
        return ans;
    }
};

// ====================================================================================================
// 11. LIGHT PANEL TOGGLE SUM [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given `m` lamps initially OFF and `n` range flip operations [l, r], return the sum of 1-based indices
  of all lamps that end in the ON state.

  PATTERN TO REMEMBER:
  Difference Array (Prefix XOR Sweep)

  CORE INTUITION & STEPS:
  - Range toggle [l, r] is recorded at boundaries: diff[l] ^= 1, diff[r + 1] ^= 1.
  - Prefix XOR sweep yields the final boolean state of lamp `i`.
  - Add `i` to total if state is 1 (ON).

  COMPLEXITY:
  - Time:  O(N + M)
  - Space: O(M)

  CONCLUSION / TAKEAWAY:
  Boolean range toggle operations map to difference array with XOR; prefix XOR sweep reveals final state in O(M).
*/
class Solution11 {
public:
    long long lampToggleSum(int m, const vector<pair<int, int>>& ops) {
        vector<int> diff(m + 2, 0);
        for (const auto& [l, r] : ops) {
            diff[l] ^= 1;
            diff[r + 1] ^= 1;
        }
        long long total = 0;
        int state = 0;
        for (int i = 1; i <= m; ++i) {
            state ^= diff[i];
            if (state) total += i;
        }
        return total;
    }
};

// ====================================================================================================
// 12. MINIMUM COST TO COMBINE ARRAY INTO ONE [Huffman Min-Heap]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Repeatedly pop two elements, pay their sum, and push sum back. Minimize total payment.

  PATTERN TO REMEMBER:
  Huffman Greedy Min-Heap Pair Merge

  CORE INTUITION & STEPS:
  - Smaller values should be merged earlier so their cost multiplies less in subsequent merges.
  - Maintain priority queue, pop two smallest, accumulate sum, push back.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Always merge the two smallest available values first using a min-heap to minimize cumulative merge cost.
*/
class Solution12 {
public:
    long long min_merge_cost(const vector<long long>& arr) {
        priority_queue<long long, vector<long long>, greater<long long>> pq(arr.begin(), arr.end());
        long long total = 0;
        while (pq.size() > 1) {
            long long a = pq.top(); pq.pop();
            long long b = pq.top(); pq.pop();
            total += a + b;
            pq.push(a + b);
        }
        return total;
    }
};

// ====================================================================================================
// 13. PALINDROMIC PAIR COUNT [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given strings, count pairs (i, j) with i < j whose concatenated characters can form a palindrome.

  PATTERN TO REMEMBER:
  26-Bit Parity Mask + HashMap Frequency

  CORE INTUITION & STEPS:
  - Character parity mask toggles bit `c - 'a'`.
  - Combined mask is `m1 ^ m2`. Valid if equal to 0 or power of 2.
  - Match current mask against seen frequencies for mask and all 1-bit toggles.

  COMPLEXITY:
  - Time:  O(26 * N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Palindrome condition = Hamming distance <= 1 between XOR parity masks. Query hash map in O(26) per word.
*/
class Solution13 {
public:
    long long count_palindrome_pairs(const vector<string>& strings) {
        unordered_map<int, long long> cnt;
        long long pairs = 0;
        for (const string& s : strings) {
            int mask = 0;
            for (char c : s) mask ^= (1 << (c - 'a'));
            pairs += cnt[mask];
            for (int b = 0; b < 26; ++b) {
                pairs += cnt[mask ^ (1 << b)];
            }
            cnt[mask]++;
        }
        return pairs;
    }
};

// ====================================================================================================
// 14. WORTH TAKING ON A TRIP [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Return true if item string `s` contains any of markers: {"co", "f", "ha", "he", "ke", "ok", "po", "sc"}.

  PATTERN TO REMEMBER:
  Constant Substring Pattern Matching

  CORE INTUITION & STEPS:
  - Iterate through the fixed set of target markers and test `s.find(marker) != string::npos`.

  COMPLEXITY:
  - Time:  O(K * N) where K = 8
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Check small constant substring dictionaries directly in linear time.
*/
class Solution14 {
public:
    bool isWorthTaking(const string& s) {
        static const vector<string> marks = {"co", "f", "ha", "he", "ke", "ok", "po", "sc"};
        for (const string& m : marks) {
            if (s.find(m) != string::npos) return true; // found a marker in the string
            // for checking substring existence, string::find returns npos if not found
        }
        return false;
    }
};

// ====================================================================================================
// 15. MINIMUM TOTAL COMPLEXITY OVER DAYS [LC 1335]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Partition `n` ordered tasks into `D` days. Each day's cost is max task complexity that day.
  Find the minimum sum of day costs.

  PATTERN TO REMEMBER:
  Partition Dynamic Programming with Suffix Range Maxima

  CORE INTUITION & STEPS:
  - dp[d][i] = min complexity for scheduling first i tasks across d days.
  - Transition: dp[d][i] = min_{j} (dp[d-1][j] + max_{k=j}^{i-1} a[k]).
  - Iterating j backwards from i-1 down to d-1 maintains range max in O(1).

  COMPLEXITY:
  - Time:  O(D * N²)
  - Space: O(D * N)

  CONCLUSION / TAKEAWAY:
  Partition DP with running backwards max computes optimal day intervals without auxiliary RMQ structures.
*/
class Solution {
    int n;
    vector<int> a;
    vector<vector<int>> dp;

    int solve(int i, int d) {
        if (d == 1) return *max_element(a.begin() + i, a.end());

        int &ans = dp[i][d];
        if (ans != -1) return ans;

        ans = INT_MAX;
        int mx = 0;

        for (int j = i; j <= n - d; j++) {
            mx = max(mx, a[j]);
            ans = min(ans, mx + solve(j + 1, d - 1));
        }
        return ans;
    }

public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        n = jobDifficulty.size();
        if (n < d) return -1;

        a = jobDifficulty;
        dp.assign(n, vector<int>(d + 1, -1));
        return solve(0, d);
    }
};
// ====================================================================================================
// 16. MINIMUM FLIPS TO REMOVE EVERY '10' PAIR [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given binary string `s`, find minimum bit flips so that "10" never appears (all '0's must precede all '1's).

  PATTERN TO REMEMBER:
  2-State Dynamic Programming

  CORE INTUITION & STEPS:
  - The target string must have form 00...011...1.
  - Maintain:
    * dp0: min flips to make prefix all '0's (s[i] != '0').
    * dp1: min flips to make prefix valid with transition to '1's (min(dp0, dp1) + (s[i] != '1')).

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Monotonic string requirements map to a 2-state DP: track cost to keep building '0's vs transitioning to '1's.
*/
class Solution16 {
public:
    long long minFlips(int n, const string& s) {
        long long dp0 = (s[0] != '0');
        long long dp1 = (s[0] != '1');
        // dp0: cost to make prefix all '0's
        // dp1: cost to make prefix valid with transition to '1's, simply, it means the cost to make prefix valid with at least one '1' after all '0's
        for (int i = 1; i < n; ++i) {
            long long next0 = dp0 + (s[i] != '0'); // continue all '0's
            long long next1 = min(dp0, dp1) + (s[i] != '1'); // transition to '1's or continue '1's
            dp0 = next0; // update dp0 for next iteration
            dp1 = next1; // update dp1 for next iteration
        }
        return min(dp0, dp1); // return the minimum flips needed to make the entire string valid
    }
};

// ====================================================================================================
// 17. VALID BRACKET SEQUENCE WITH AT MOST 1 SWAP [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a bracket string `s`, determine if at most one swap can make it a valid bracket sequence.

  PATTERN TO REMEMBER:
  Prefix Balance & Minimum Deficit Check

  CORE INTUITION & STEPS:
  - Valid sequence requires total balance == 0 and all prefix balances >= 0 (count of '(' minus count of ')' at each prefix).
  - Swapping a ')' at index i with a '(' at index j (i < j) adds exactly +2 to prefix balances in between.
  - Therefore, at most one swap can remedy a prefix balance deficit of at most -2.
  - Condition: balance == 0 && min(prefix_balance) >= -2.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  One swap of ')' with '(' boosts intermediate prefix balances by exactly +2; minimum prefix balance cannot fall below -2.
*/
class Solution17 {
public:
    int canBeValid(const string& s) {
        int balance = 0, minBalance = 0;
        for (char c : s) {
            balance += (c == '(' ? 1 : -1);
            minBalance = min(minBalance, balance);
        }
        return (balance == 0 && minBalance >= -2) ? 1 : 0;
    }
};

// ====================================================================================================
// 18. MAXIMUM MINIMUM MAGNETIC FORCE [LC 1552]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Place `m` balls into `n` baskets to maximize the minimum distance between any two balls.

  PATTERN TO REMEMBER:
  Binary Search on Answer + Greedy Placement Check

  CORE INTUITION & STEPS:
  - Sort basket positions.
  - Binary search the answer `dist` in [1, max_pos - min_pos].
  - Feasibility check: place first ball at basket 0; place next ball at earliest basket with gap >= dist.
  - If placed count >= m, search higher; else search lower.

  COMPLEXITY:
  - Time:  O(N log(Range))
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Max-min / min-max bottleneck spacing problems always reduce to binary search on the distance with greedy check.
*/
class Solution18 {
    bool canPlace(vector<int>& pos, int m, int dist) {
        int balls = 1;
        int last = pos[0];

        for (int i = 1; i < pos.size(); i++) {
            if (pos[i] - last >= dist) { // if the current basket is far enough from the last placed ball, we can place another ball here
                balls++;
                last = pos[i];
            }
        }

        return balls >= m; // return true if we can place at least m balls with the given minimum distance
    }

public:
    int maxDistance(vector<int>& pos, int m) {
        sort(pos.begin(), pos.end());

        int low = 1;
        int high = pos.back() - pos[0];
        int ans = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            // Check if it's possible to place m balls with minimum distance mid
            if (canPlace(pos, m, mid)) {
                ans = mid;
                low = mid + 1;   // Try bigger distance
            } else {
                high = mid - 1;  // Try smaller distance
            }
        }

        return ans;
    }
};

// ====================================================================================================
// 19. MINIMUM ARRAY COST WITH UNLIMITED INSERTIONS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Array cost is sum of adjacent squared differences: sum((a[i] - a[i-1])²). You may insert any
  integers between original elements. Find minimum possible cost.

  PATTERN TO REMEMBER:
  Convexity Property (Unit Step Decompositions)

  CORE INTUITION & STEPS:
  - By convexity of squares, splitting difference `d = |x - y|` into `d` unit steps of 1 produces
    cost d * (1)² = d = |x - y|.
  - Total minimum cost is simply Σ |a[i] - a[i - 1]|.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Unit steps of size 1 minimize the sum of squared differences, collapsing the quadratic cost to linear absolute differences.
*/
class Solution19 {
public:
    long long minInsertionCost(const vector<long long>& a) {
        long long cost = 0;
        for (size_t i = 1; i < a.size(); ++i) {
            cost += abs(a[i] - a[i - 1]);
        }
        return cost;
    }
};

// ====================================================================================================
// 20. WEIGHTED MANHATTAN DISTANCE CITY [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Find coordinates (X, Y) minimizing weighted Manhattan distance to all cities: sum(p * (|X - x| + |Y - y|)).
  Break ties with lexicographically smallest (X, Y).

  PATTERN TO REMEMBER:
  1D Weighted Median (Independent Dimensions)

  CORE INTUITION & STEPS:
  - Manhattan distance splits into independent 1D problems for X and Y.
  - In 1D, sum(w * |X - x|) is minimized at the weighted median where cumulative weight >= total / 2.
  - Sort coordinates, find first point where prefix weight >= (total + 1) / 2.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Manhattan distance dimensions are completely decoupled; solve independently along X and Y via 1D weighted medians.
*/
class Solution20 {
public:
    pair<ll, ll> solve(const vector<array<ll, 3>>& cities) {
        vector<pair<ll, ll>> xs, ys;
        ll totalWeight = 0;
        for (const auto& [x, y, p] : cities) {
            xs.push_back({x, p});
            ys.push_back({y, p});
            totalWeight += p;
        }
        sort(xs.begin(), xs.end());
        sort(ys.begin(), ys.end());

        auto getMedian = [&](const vector<pair<ll, ll>>& pts) {
            ll running = 0;
            for (const auto& [coord, w] : pts) {
                running += w;
                if (2 * running >= totalWeight) return coord; // this is weight median
            }
            return pts.back().first; // in case all weights are zero, return last coordinate
        };
        return {getMedian(xs), getMedian(ys)};
    }
};

// ====================================================================================================
// 21. MAXIMUM XOR WITH THE SAME BIT LENGTH [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given `n`, choose integer `x >= n` with the same bit length as `n` to maximize n XOR (n+1) ... XOR x.
  Break ties with smallest `x`.

  PATTERN TO REMEMBER:
  Prefix XOR Periodicity (Modulo 4)

  CORE INTUITION & STEPS:
  - Prefix XOR P(x) = 0 ^ 1 ^ ... ^ x has period 4:
    * x % 4 == 0 -> x
    * x % 4 == 1 -> 1
    * x % 4 == 2 -> x + 1
    * x % 4 == 3 -> 0
  - Range XOR(n..x) = P(x) ^ P(n - 1).
  - Test candidates among upper boundary values having the same bit length.

  COMPLEXITY:
  - Time:  O(1)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Prefix XOR repeats every 4 integers; evaluate range XOR in O(1) using the 4-periodic closed formula.
*/
class Solution21 {
    ull prefXor(ull x) {
        ull rem = x % 4;
        if (rem == 0) return x;
        if (rem == 1) return 1;
        if (rem == 2) return x + 1;
        return 0;
    }
public:
    ull solve(ull n) {
        int bits = 64 - __builtin_clzll(n); // count bits in n
        // __builtin_clzll counts leading zeros; 64 - leading zeros gives bit length
        ull hi = (bits == 64 ? ULLONG_MAX : (1ULL << bits) - 1); // maximum number with same bit length
        ull bestX = n, maxVal = prefXor(n) ^ prefXor(n - 1); // initial value for x = n
        for (int i = 0; i < 4 && hi-i>=n; ++i) { // hi-i must be >= n to ensure x >= n
            ull x = hi - i;
            ull val = prefXor(x) ^ prefXor(n - 1);
            if (val > maxVal || (val == maxVal && x < bestX)) {
                maxVal = val;
                bestX = x;
            }
        }
        return bestX;
    }
};

// ====================================================================================================
// 22. MINIMUM ARRAY COST AFTER ONE INSERTION [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Array cost is sum((a[i] - a[i-1])²). Insert at most ONE integer `x` between an adjacent pair to minimize cost.

  PATTERN TO REMEMBER:
  Maximum Variance Reduction via Midpoint Insertion

  CORE INTUITION & STEPS:
  - Inserting midpoint between u and v splits difference `d` into floor(d/2) and ceil(d/2).
  - Cost reduction = d² - (floor(d/2)² + ceil(d/2)²) = floor(d² / 2).
  - Find the adjacent pair yielding the maximum reduction `floor(d² / 2)` and subtract it.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Inserting a midpoint splits difference `d` into halves, netting a variance savings of `floor(d² / 2)`.
*/
class Solution22 {
public:
    long long solve(const vector<long long>& a) {
        long long total = 0, maxSave = 0;
        for (size_t i = 1; i < a.size(); ++i) {
            long long d = a[i] - a[i - 1];
            total += d * d;
            maxSave = max(maxSave, (d * d) / 2);
        }
        return total - maxSave;
    }
};

// ====================================================================================================
// 23. COUNT ARRAYS REACHABLE BY THRESHOLD DECREMENTS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  In each step, choose threshold `T` and decrement all elements >= T by 1. Count distinct reachable arrays modulo 10^9 + 7.

  PATTERN TO REMEMBER:
  Independent Gap Coordinates Combinatorics

  CORE INTUITION & STEPS:
  - Sort and deduplicate array `a`.
  - Operations preserve relative order; every adjacent gap (a[i] - a[i-1]) can be decremented
    independently to any value in [0, a[i] - a[i-1]], because we can choose thresholds to decrement each coordinate separately.
  - Total combinations = (a[0] + 1) * Π (a[i] - a[i-1] + 1) % MOD.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Threshold decrements preserve coordinate gaps independently; answer is product of (gap + 1) choices.
*/
class Solution23 {
public:
    long long solve(vector<long long> a) {
        const long long MOD = 1e9 + 7;
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        long long ans = (a[0] + 1) % MOD;
        for (size_t i = 1; i < a.size(); ++i) {
            long long gap = (a[i] - a[i - 1] + 1) % MOD;
            ans = (ans * gap) % MOD;
        }
        return ans;
    }
};

// ====================================================================================================
// 24. MAXIMIZE ONES IN A BINARY STRING [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Operation sets s[i] = max(s[i], s[i+1]). A '1' propagates leftward. Find maximum '1's after <= k operations.

  PATTERN TO REMEMBER:
  Greedy Left Propagation Reach

  CORE INTUITION & STEPS:
  - '1's can only propagate leftward to preceding positions.
  - Any '0' preceding the rightmost '1' can be turned into a '1'.
  - If no '1' exists, answer is 0.
  - Count available zeros before rightmost '1': convertible = (last_one_idx + 1) - total_ones.
  - Max ones = total_ones + min((long long)k, convertible).

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Leftward propagation can convert any preceding zeros up to operation budget `k`.
*/
class Solution24 {
public:
    long long maxOnes(const string& s, long long k) {
        int lastOne = -1;
        long long ones = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '1') {
                ones++;
                lastOne = i;
            }
        }
        if (lastOne == -1) return 0;
        int total = lastOne + 1;
        long long zerosBeforeLast = total - ones;
        return ones + min(k, zerosBeforeLast);
    }
};

// ====================================================================================================
// 25. MINIMIZE ARRAY SUM AFTER K OPERATIONS [LC 1962]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  In each operation, replace element x with ceil(x / 2). Minimize array sum after <= k operations.

  PATTERN TO REMEMBER:
  Greedy Max-Heap Halving

  CORE INTUITION & STEPS:
  - Replacing x with ceil(x / 2) reduces sum by floor(x / 2).
  - Greedily pick the largest element via max-heap to maximize each subtraction.

  COMPLEXITY:
  - Time:  O((N + K) log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Max-heap repeatedly extracts the largest value to maximize floor(x / 2) reduction at each step.
*/
class Solution25 {
public:
    long long minimumSum(const vector<long long>& nums, long long k) {
        priority_queue<long long> pq;
        long long sum = 0;
        for (long long x : nums) {
            pq.push(x);
            sum += x;
        }
        while (k-- && !pq.empty() && pq.top() > 1) {
            long long x = pq.top();
            pq.pop();
            long long nextVal = (x + 1) / 2; // ceil(x / 2)
            sum -= (x - nextVal); // reduce sum by floor(x / 2)
            pq.push(nextVal);
        }
        return sum;
    }
};

// ====================================================================================================
// 26. LONGEST SQUARE CHAIN SUBSET [LC 2501]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Find the maximum size of a subset where every element after the first equals the square of the previous.

  PATTERN TO REMEMBER:
  Hash Set Sequence Traversal (x -> x²)

  CORE INTUITION & STEPS:
  - Multiple 1's can form a chain of 1's (1² = 1).
  - Store all numbers in an unordered_set.
  - For each number x > 1: repeatedly check if x² exists in set, guarding against 64-bit integer overflow.

  COMPLEXITY:
  - Time:  O(N) average
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Hash set allows constant-time verification of geometric square chains x -> x²; guard bounds against overflow.
*/
class Solution26 {
public:
    int solution(int n, const vector<int>& B) {
        unordered_set<long long> st;
        int ones = 0, best = 0;
        for (int x : B) {
            st.insert(x);
            if (x == 1) ones++;
        }
        best = ones;
        for (long long x : st) {
            if (x == 1) continue;
            long long cur = x;
            int len = 0;
            while (st.count(cur)) {
                len++;
                if (cur > 1000000000LL / cur) break;
                cur *= cur;
            }
            best = max(best, len);
        }
        return best;
    }
};

// ====================================================================================================
// 27. AUTHENTICATION TOKEN SYSTEM [LC 1797]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Manage authentication tokens with `time_to_live`. Queries: generate, renew, and count unexpired tokens.

  PATTERN TO REMEMBER:
  Hash Map Expiry Tracking / Time-based Cache

  CORE INTUITION & STEPS:
  - Map `token -> expiryTime`.
  - generate: `expiry[token] = time + ttl`.
  - renew: if token exists and `expiry[token] > time`, update `expiry[token] = time + ttl`.
  - count: count tokens where `expiry[token] > time`.

  COMPLEXITY:
  - Time:  O(1) generate/renew, O(Tokens) count
  - Space: O(Tokens)

  CONCLUSION / TAKEAWAY:
  Store absolute expiration timestamps; renew is valid strictly when `expiry > current_time`.
*/
class Solution27 {
public:
    vector<int> getUnexpiredTokens(int time_to_live, const vector<string>& queries) {
        unordered_map<string, int> expiry;
        vector<int> counts;
        for (const string& q : queries) {
            stringstream ss(q); // stringstream to parse the query
            string type, token;
            int time;
            ss >> type;
            if (type == "generate") {
                ss >> token >> time;
                expiry[token] = time + time_to_live; // update expiry time for the generated token
            } else if (type == "renew") {
                ss >> token >> time;
                if (expiry.count(token) && expiry[token] > time) { // only renew if token exists and is unexpired
                    expiry[token] = time + time_to_live;
                }
            } else if (type == "count") {
                ss >> time;
                int active = 0;
                // Count the number of active tokens at the current time
                for (const auto& [_, exp] : expiry) {
                    if (exp > time) active++;
                }
                counts.push_back(active);
            }
        }
        return counts;
    }
};

// ====================================================================================================
// 28. MINIMUM COST TO REACH POINT N [LC 1696 / Jump Game VI]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Start at 0, jump at most `k` steps forward, paying cost[i] at each landing. Minimize cost to reach n - 1.

  PATTERN TO REMEMBER:
  DP + Monotonic Deque Sliding Window Minimum

  CORE INTUITION & STEPS:
  - dp[i] = cost[i] + min_{j in [i-k, i-1]} dp[j].
  - Maintain indices in a monotonic increasing deque: front always holds minimum dp in sliding window.
  - Evict front indices older than `i - k` in O(1) amortized time.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Monotonic deque maintains sliding window minimums for 1D DP transitions in O(1) amortized time.
*/
class Solution28 {
public:
    long long getMinimumCost(int n, const vector<int>& cost, int k) {
        vector<long long> dp(n);
        deque<int> dq;
        dp[0] = cost[0];
        dq.push_back(0); // initialize deque with index 0
        for (int i = 1; i < n; ++i) {
            while (!dq.empty() && dq.front() < i - k) dq.pop_front(); // evict all indices outside the window
            int index = dq.front(); // index of the minimum dp value in the last k indices
            dp[i] = cost[i] + dp[index]; // compute dp[i] using the minimum dp in the window
            while (!dq.empty() && dp[dq.back()] >= dp[i]) dq.pop_back(); // maintain monotonicity: remove indices with higher or equal dp value
            dq.push_back(i); // add current index to deque for future minimum queries
        }
        return dp[n - 1];
    }
};

// ====================================================================================================
// 29. IDEAL NUMBERS (3^x * 5^y) [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Count ideal numbers (3^x * 5^y with x, y >= 0) in inclusive range [low, high].

  PATTERN TO REMEMBER:
  Double Geometric Power Traversal / Bounded Enumeration

  CORE INTUITION & STEPS:
  - Total pairs (x, y) with 3^x * 5^y <= high is under 1,000 for 64-bit limits.
  - Nested loops iterate powers of 3 and powers of 5, guarding against overflow.
  - Count products falling in [low, high].

  COMPLEXITY:
  - Time:  O(log3(high) * log5(high)) ≈ O(1)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Exponential growth limits candidate search space to < 1000 combinations; iterate powers directly with multiplication guards.
*/
class Solution29 {
public:
    long long getIdealNums(long long low, long long high) {
        long long count = 0;
        int a=1;
        int b=1;
        while(a <= high) {
            b = 1;
            while (a * b <= high) {
                if (a * b >= low) count++;
                if (b > high / 5) break; // prevent overflow
                b *= 5;
            }
            if (a > high / 3) break; // prevent overflow
            a *= 3;
        }
        return count;
    }
};

// ====================================================================================================
// 30. MAXIMUM BITWISE OR-SUM [LC 2680]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  At most `k` times, choose an element and multiply it by 2. Maximize the bitwise OR sum of the array.

  PATTERN TO REMEMBER:
  Prefix & Suffix Bitwise OR Sweep

  CORE INTUITION & STEPS:
  - Bit shifts create higher-order bits. To maximize OR, all k multiplications should be applied
    to a single chosen element: `arr[i] << k`, because OR is monotonic with respect to bit shifts, it means, applying all shifts to one number will yield the highest OR sum, because it will contribute the most significant bits to the overall OR
  - For each candidate index `i`, total OR is `pref[i] | ((long long)arr[i] << k) | suff[i + 1]`.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(N) for prefix/suffix arrays

  CONCLUSION / TAKEAWAY:
  Greedily concentrate all bit shifts on one optimal number; compute the rest in O(1) via prefix/suffix OR sweeps.
*/
class Solution30 {
public:
    long long getMaxOrSum(const vector<int>& arr, int k) {
        int n = arr.size();
        vector<long long> pre(n + 1, 0), suf(n + 1, 0);
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] | arr[i];
        for (int i = n - 1; i >= 0; --i) suf[i] = suf[i + 1] | arr[i];
        long long maxOr = 0;
        for (int i = 0; i < n; ++i) {
            maxOr = max(maxOr, pre[i] | ((long long)arr[i] << k) | suf[i + 1]);
        }
        return maxOr;
    }
};

// ====================================================================================================
// 31. SHOPKEEPER SALE – FINAL PRICE OF ITEMS [LC 1475]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Item `i` receives a discount equal to the first subsequent price `prices[j] <= prices[i]` (j > i).
  Return total discounted price and the sorted indices of items that received NO discount.

  PATTERN TO REMEMBER:
  Next Smaller or Equal Element via Monotonic Stack

  CORE INTUITION & STEPS:
  - Monotonic stack stores indices waiting for a discount.
  - When prices[i] <= prices[st.top()], pop top and subtract discount prices[i].
  - Indices remaining in stack at the end received no discount.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Monotonic stack pairs each item with the next smaller-or-equal element on its right in O(N).
*/
class Solution31 {
public:
    pair<long long, vector<int>> finalPrice(const vector<int>& prices) {
        int n = prices.size();
        vector<int> finalCost = prices;
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && prices[i] <= prices[st.top()]) { // there exist a previous item that can be discounted by prices[i]
                finalCost[st.top()] -= prices[i];
                st.pop();
            }
            st.push(i);
        }
        long long total = 0;
        for (int x : finalCost) total += x;
        vector<int> noDiscount;
        while (!st.empty()) {
            noDiscount.push_back(st.top());
            st.pop();
        }
        reverse(noDiscount.begin(), noDiscount.end());
        return {total, noDiscount};
    }
};

// ====================================================================================================
// 32. COUNT PALINDROMIC SUBSEQUENCES OF LENGTH 5 [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given binary string `s`, count palindromic subsequences of length 5 (form: `a b c b a`).

  PATTERN TO REMEMBER:
  Prefix / Suffix Pair Frequency Counting Around Center

  CORE INTUITION & STEPS:
  - Character `c` is the center at index `i`.
  - Prefix must supply subsequence `a b`, suffix must supply `b a`.
  - Since alphabet is {0, 1}, evaluate all 4 binary pair configurations (a, b) in {0, 1}².
  - Sum `left(a b) * right(b a)` across all center indices.

  COMPLEXITY:
  - Time:  O(4 * N) = O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Fix the palindrome center and iterate constant 2² boundary configurations; combine prefix and suffix frequencies in O(N).
*/
class Solution32 {
public:
    int getPalindromesCount(const string& s) {
        const long long MOD = 1e9 + 7;
        int n = s.size();
        if (n < 5) return 0;

 
        vector<vector<vector<long long>>> pref(n, vector<vector<long long>>(2, vector<long long>(2, 0)));
        // pref[i][j][k] = count of subsequence "jk" in s[0...i-1]
        vector<vector<vector<long long>>> suff(n, vector<vector<long long>>(2, vector<long long>(2, 0)));
        // suff[i][j][k] = count of subsequence "jk" in s[i+1...n-1]

        // Pass 1: Build Prefix Subsequence Counts
        vector<long long> cnt(2, 0);
        for (int i = 0; i < n; ++i) {
            if (i > 0) pref[i] = pref[i - 1]; // carry forward previous counts
            int cur = s[i] - '0';
            for (int prev = 0; prev < 2; ++prev) {
                pref[i][prev][cur] += cnt[prev]; // "prev" followed by "cur" forms "prev cur"
            }
            cnt[cur]++;
        }

        // Pass 2: Build Suffix Subsequence Counts
        cnt.assign(2, 0);
        for (int i = n - 1; i >= 0; --i) {
            if (i < n - 1) suff[i] = suff[i + 1]; // carry forward previous counts
            int cur = s[i] - '0';
            for (int nxt = 0; nxt < 2; ++nxt) {
                suff[i][cur][nxt] += cnt[nxt]; // "cur" followed by "nxt" forms "cur nxt"
            }
            cnt[cur]++;
        }

        // Pass 3: Fix middle character 'c' at index i (1 <= i <= n-2)
        long long total = 0;
        for (int i = 2; i < n - 2; ++i) {
            for(int j=0; j<2; ++j) {
                for(int k=0; k<2; ++k) {
                    long long leftCount = pref[i - 1][j][k]; // count of "jk" in prefix
                    long long rightCount = suff[i + 1][k][j]; // count of "kj" in suffix
                    total = (total + (leftCount * rightCount) % MOD) % MOD;
                }
            }
        }

        return total;
    }
};

// ====================================================================================================
// 33. BINARY CARDINALITY SORT [LC 1356]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Sort integers by number of 1-bits ascending; break ties by numerical value ascending.

  PATTERN TO REMEMBER:
  Popcount Multi-Key Sort Comparator

  CORE INTUITION & STEPS:
  - Custom comparator: compare `__builtin_popcount(a)` vs `__builtin_popcount(b)`.
  - If equal, fallback to `a < b`.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(log N) auxiliary space for sort

  CONCLUSION / TAKEAWAY:
  Use `__builtin_popcount` inside standard comparator for hardware-accelerated bit-cardinality sorting.
*/
class Solution33 {
public:
    static bool comparator(int a, int b) {
        int popA = __builtin_popcount(a);
        int popB = __builtin_popcount(b);
        if (popA != popB) return popA < popB;
        return a < b;
    }
    vector<int> cardinalitySort(vector<int> nums) {
        sort(nums.begin(), nums.end(), comparator);
        return nums;
    }
};

// ====================================================================================================
// 34. BALL COLLISION AROUND PIVOT POINT [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given speeds of balls moving right and pivot index `x`, count collisions involving ball `x`.

  PATTERN TO REMEMBER:
  Single-Pass Bidirectional Threshold Comparison

  CORE INTUITION & STEPS:
  - Behind ball `x`: collides if speed > pivot speed.
  - Ahead of ball `x`: collides if speed < pivot speed.
  - Count both in a single linear pass.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  A follower overtakes if faster; a leader gets hit if slower. Count directly around the pivot speed threshold.
*/
class Solution34 {
public:
    long long countCollisions(const vector<long long>& a, int x) {
        int n = a.size(), p = x - 1;
        long long collisions = 0;
        for (int i = 0; i < p; ++i) {
            if (a[i] > a[p]) collisions++;
        }
        for (int i = p + 1; i < n; ++i) {
            if (a[i] < a[p]) collisions++;
        }
        return collisions;
    }
};

// ====================================================================================================
// 35. MAXIMUM WEIGHTED DIFFERENCE (1-BASED INDEX) [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Rearrange `arr1` and `arr2` to maximize Σ_{i=1}^{n} i * (arr2[i] - arr1[i]).

  PATTERN TO REMEMBER:
  Rearrangement Inequality

  CORE INTUITION & STEPS:
  - Sort arr2 ascending (match largest elements with largest weights).
  - Sort arr1 descending (match largest elements with smallest weights).

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Sort positive term ascending and negative term descending to maximize weighted difference.
*/
class Solution35 {
public:
    long long maxWeightedDifference(vector<long long> arr1, vector<long long> arr2) {
        int n = arr1.size();
        sort(arr1.rbegin(), arr1.rend());
        sort(arr2.begin(), arr2.end());
        long long total = 0;
        for (int i = 0; i < n; ++i) {
            total += 1LL * (i + 1) * (arr2[i] - arr1[i]);
        }
        return total;
    }
};

// ====================================================================================================
// 36. DELETE ODD-VALUED NODES FROM A LINKED LIST [LC 3217 Variant]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Delete every node in a singly linked list with odd value (`val % 2 != 0`).

  PATTERN TO REMEMBER:
  Sentinel Pointer / Dummy Head Traversal

  CORE INTUITION & STEPS:
  - Advance `head` while it is odd, because the first node may be odd, so we need to find the first even node to be the new head.
  - Traverse with pointer `cur`: if `cur->next->val % 2 != 0`, bypass `cur->next = cur->next->next`; else advance.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Filter leading matches by advancing head, then stitch around matching subsequent nodes using single-lookahead pointers.
*/
class Solution36 {
public:
    ListNode* deleteOddNodes(ListNode* head) {
        while (head && head->val % 2 != 0) { // skip leading odd nodes
            head = head->next;
        }
        ListNode* cur = head;
        while (cur && cur->next) { // traverse and remove odd nodes
            if (cur->next->val % 2 != 0) { // if next node is odd, bypass it
                cur->next = cur->next->next;
            } else {
                cur = cur->next; // advance if next node is even
            }
        }
        return head; // return new head of the filtered list
    }
};


// ====================================================================================================
// 37. SHORTEST SUBSTRING DELETION FOR DISTINCT CHARACTERS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s`, find the minimum length of a contiguous substring that must be deleted
  so that all remaining characters in the string are unique/distinct.
  (Deleting 0 characters is allowed if the string already has all distinct characters).

  PATTERN TO REMEMBER:
  Prefix & Suffix Distinct Sets / Two Pointers

  CORE INTUITION & STEPS:
  - The kept characters form a prefix s[0...l] and a suffix s[r...n-1] (either can be empty).
  - Both prefix and suffix must contain pairwise distinct characters, and their intersection must be empty.
  - Step 1: Scan right-to-left to find the largest suffix s[r...n-1] with distinct characters. Initial ans = r.
  - Step 2: Scan left-to-right to grow prefix s[0...l] of distinct characters.
    For each character s[l], shrink the suffix (advance r) until s[l] is not in the suffix.
  - ans = min(ans, r - l - 1). Stop if prefix itself sees a duplicate character.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(Σ) where Σ <= 26 (alphabet size)

  CONCLUSION / TAKEAWAY:
  Fixing a distinct prefix and sliding a distinct suffix shrinks deletions to the minimal gap between them.
*/
class Solution37 {
public:
    int findShortestSubstring(const string& s) {
        int n = s.size(), r = n, ans = INT_MAX;
        unordered_set<char> suf, pre;

        // Largest distinct suffix
        int j = n - 1;
        while (j >= 0 && !suf.count(s[j]))
            suf.insert(s[j--]);

        r = j + 1; // r is pointer to the first character of the suffix which is distinct
        ans = r; // initial answer is the length of the suffix

        for (int i = 0; i < n; i++) {
            if (pre.count(s[i])) break;
            pre.insert(s[i]);

            while (r < n && pre.count(s[r])){
                r++;
                suf.erase(s[r - 1]); // remove from suffix as we move r forward
            }

            ans = min(ans, r - i - 1); // update answer with the length of the substring to delete
        }

        return ans;
    }
};

// ====================================================================================================
// 38. MESSAGE DUPLICATE DETECTION RATE LIMITER [LC 359]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Message is delivered (true) unless exact same message was delivered within the last `k` seconds.
  Dropped messages do not update delivery time. Return boolean outcomes.

  PATTERN TO REMEMBER:
  Hash Map Message Timestamp Cache

  CORE INTUITION & STEPS:
  - Map message -> lastDeliveryTimestamp.
  - If `last.count(msg) && timestamp - last[msg] <= k`: duplicate, record false.
  - Else: successful delivery, record true, update `last[msg] = timestamp`.

  COMPLEXITY:
  - Time:  O(N)
  - Space: O(Unique Messages)

  CONCLUSION / TAKEAWAY:
  Cache the most recent successful timestamp per message key; dropped messages never advance the clock.
*/
class Solution38 {
public:
    vector<bool> messageDelivery(const vector<int>& timestamps, const vector<string>& messages, int k) {
        unordered_map<string, int> lastDelivered;
        vector<bool> ans;
        ans.reserve(messages.size());
        for (size_t i = 0; i < messages.size(); ++i) {
            const string& msg = messages[i];
            if (lastDelivered.count(msg) && timestamps[i] - lastDelivered[msg] <= k) {
                ans.push_back(false);
            } else {
                ans.push_back(true);
                lastDelivered[msg] = timestamps[i];
            }
        }
        return ans;
    }
};

// ====================================================================================================
// 39. MAXIMIZE THE XOR OF TWO BINARY STRINGS BY REORDERING ONE [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given two binary strings `s` and `t` of the same length, reorder the characters of `t` such that
  the bitwise XOR of `s` and the reordered `t` is maximized. Return the resulting maximum XOR string.

  PATTERN TO REMEMBER:
  Greedy Most Significant Bit XOR Maximization

  CORE INTUITION & STEPS:
  - Higher-order bits contribute exponentially more than lower-order bits (2^k > sum_{j=0}^{k-1} 2^j).
  - Therefore, we must greedily make the leftmost bits of the XOR result '1' whenever possible.
  - Count available '1's and '0's in string `t`.
  - For each bit `c` in `s` from left to right:
    * If `c == '0'`: to get a '1' in XOR, we need a '1' from `t`. If `ones > 0`, pick '1' (ans += '1', ones--);
      otherwise we are forced to pick '0' (ans += '0', zeros--).
    * If `c == '1'`: to get a '1' in XOR, we need a '0' from `t`. If `zeros > 0`, pick '0' (ans += '1', zeros--);
      otherwise we are forced to pick '1' (ans += '0', ones--).
  - Return the resulting string `ans`.

  COMPLEXITY:
  - Time:  O(N) single linear pass over the strings
  - Space: O(N) for output string

  CONCLUSION / TAKEAWAY:
  Greedily prioritize setting most significant XOR bits to '1' using complementary bits from the character counts.
*/
class Solution39 {
public:
    string maximizeXor(const string& s, const string& t) {
        int n = t.size();
        int m = s.size();
        int ones = count(t.begin(), t.end(), '1');
        int zeros = n - ones;

        string ans;
        for (char c : s) {
            if (c == '0') {
                if (ones > 0) {
                    ans.push_back('1');
                    ones--;
                } else {
                    ans.push_back('0');
                    zeros--;
                }
            } else {
                if (zeros > 0) {
                    ans.push_back('1');
                    zeros--;
                } else {
                    ans.push_back('0');
                    ones--;
                }
            }
        }

        return ans;
    }
};

// ====================================================================================================
// 40. EVALUATE A SIGNED INTEGER ARITHMETIC EXPRESSION [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an arithmetic expression string containing non-negative integers, operators ('+', '-', '*', '/'),
  parentheses ('(', ')'), unary signs (+, -), and whitespace, evaluate and return its integer result.

  PATTERN TO REMEMBER:
  Recursive Descent Parsing (Grammar Hierarchy: Expr -> Term -> Factor)

  CORE INTUITION & STEPS:
  - Standard grammar handling operator precedence and associativity:
    * expr   := term (('+' | '-') term)*
    * term   := factor (('*' | '/') factor)*
    * factor := ('+' | '-') factor | '(' expr ')' | number
  - Maintain a pointer index `i` through the string `s`.
  - `factor()` parses unary signs recursively, sub-expressions enclosed in '(', ')', or multi-digit numbers.
  - `term()` evaluates higher-precedence multiplicative operators ('*', '/').
  - `expr()` evaluates lower-precedence additive operators ('+', '-').
  - `skip()` cleanly bypasses whitespace characters between tokens.

  COMPLEXITY:
  - Time:  O(N) single pass over the expression tokens
  - Space: O(N) recursion stack in the worst case (nested parentheses or unary chains)

  CONCLUSION / TAKEAWAY:
  Recursive descent parsing cleanly decouples operator precedence, parenthesization, and unary signs without complex stack state machines.
*/
class Solution40Recursive {
    int i = 0;

    void skipWhitespace(const string& s) {
        while (i < s.size() && s[i] == ' ') i++;
    }

    // Handles numbers, unary operators (+/-), and parenthesized sub-expressions
    long long factor(const string& s) {
        skipWhitespace(s);
        if (s[i] == '+' || s[i] == '-') {
            char op = s[i++];
            return (op == '+') ? factor(s) : -factor(s);
        }
        if (s[i] == '(') {
            i++; // skip '('
            long long val = expr(s);
            i++; // skip ')'
            return val;
        }
        long long num = 0;
        while (i < s.size() && isdigit(s[i])) {
            num = num * 10 + (s[i++] - '0');
        }
        return num;
    }

    // Handles multiplication and division (higher precedence)
    long long term(const string& s) {
        long long left = factor(s);
        while (true) {
            skipWhitespace(s);
            if (i < s.size() && (s[i] == '*' || s[i] == '/')) {
                char op = s[i++];
                long long right = factor(s);
                left = (op == '*') ? (left * right) : (left / right);
            } else {
                break;
            }
        }
        return left;
    }

    // Handles addition and subtraction (lower precedence)
    long long expr(const string& s) {
        long long left = term(s);
        while (true) {
            skipWhitespace(s);
            if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
                char op = s[i++];
                long long right = term(s);
                left = (op == '+') ? (left + right) : (left - right);
            } else {
                break;
            }
        }
        return left;
    }

public:
    long long evaluateExpression(const string& s) {
        i = 0;
        return expr(s);
    }
};

class Solution41 {
    vector<int> state, depth;

    int dfs(int u, const vector<int>& manager) {
        if (state[u] == 1) return -1;  // cycle
        if (state[u] == 2) return depth[u];

        state[u] = 1;

        if (manager[u] == -1)
            depth[u] = 0;
        else {
            int d = dfs(manager[u], manager);
            if (d == -1) return -1;
            depth[u] = d + 1;
        }

        state[u] = 2;
        return depth[u];
    }

public:
    vector<int> analyzeHierarchy(const vector<int>& manager) {
        int n = manager.size(), root = -1, roots = 0;

        state.assign(n, 0);
        depth.assign(n, 0);

        for (int i = 0; i < n; i++) {
            if (manager[i] == -1)
                root = i, roots++;
            else if (manager[i] < 0 || manager[i] >= n)
                return {1, -1};
        }

        if (roots != 1) return {1, -1};

        int ans = 0;
        for (int i = 0; i < n; i++) {
            int d = dfs(i, manager);
            if (d == -1) return {1, -1};
            ans = max(ans, d);
        }

        return {0, ans};
    }
};

// ====================================================================================================
// 42. MOST VISITED SECTORS OF A CIRCULAR TRACK [MS OA / LC 1560 Variant]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a circular track divided into named `sectors`, and a list of sector checkpoints visited in order `rounds`:
  A runner moves around the track in a fixed circular direction from rounds[i-1] to rounds[i].
  Find the sector(s) visited the maximum number of times. Return the most visited sectors in their original track order.

  PATTERN TO REMEMBER:
  Circular Difference Array (Prefix Sum Sweep)

  CORE INTUITION & STEPS:
  - Mapping sector names to indices [0..n-1] turns the circular track into an index interval.
  - Instead of point-by-point circular simulation (which could be O(Track * Rounds)), use a difference array `diff` of size n+1.
  - Start position: increment `diff[pos[rounds[0]]]++`.
  - For each leg from index `a` to `b`:
    * If `a < b`: simple interval update on [a + 1, b].
    * If `a > b`: circular wrap-around split into two intervals: [a + 1, n - 1] and [0, b].
  - Running prefix sum computes the total visits per sector in O(N).
  - Collect all sectors attaining the maximum visit count.

  COMPLEXITY:
  - Time:  O(N + M) where N = sectors.size(), M = rounds.size()
  - Space: O(N) for index map, difference array, and visit counts

  CONCLUSION / TAKEAWAY:
  Convert circular boundary transitions into at most two linear interval updates using a difference array to avoid full step simulation.
*/
class Solution42 {
public:
    vector<string> mostVisitedSectors(const vector<string>& sectors,
                                       const vector<string>& rounds) {
        int n = sectors.size();
        if (!n || rounds.empty()) return {};

        unordered_map<string, int> pos;
        for (int i = 0; i < n; i++) pos[sectors[i]] = i;

        vector<int> diff(n + 1), cnt(n);
        diff[pos[rounds[0]]]++;

        for (int i = 1; i < rounds.size(); i++) {
            int a = pos[rounds[i - 1]], b = pos[rounds[i]];

            if (a < b) {
                diff[a + 1]++;
                diff[b + 1]--;
            } else if (a > b) {
                diff[a + 1]++;
                diff[n]--;
                diff[0]++;
                diff[b + 1]--;
            }
        }

        int cur = 0, mx = 0;
        for (int i = 0; i < n; i++) {
            cur += diff[i];
            cnt[i] = cur;
            mx = max(mx, cur);
        }

        vector<string> ans;
        for (int i = 0; i < n; i++)
            if (cnt[i] == mx)
                ans.push_back(sectors[i]);

        return ans;
    }
};