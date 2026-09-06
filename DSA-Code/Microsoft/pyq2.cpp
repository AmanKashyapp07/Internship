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
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 01 | Longest Palindromic Subsequence Length      | Interval DP                        | O(N²)    | O(N²)    |
 | 02 | Number of Longest Strictly Increasing Subseq| DP + Counting                      | O(N²)    | O(N)     |
 | 03 | Count Substrings Containing All Three Letter| Last Occurrence / Sliding Window  | O(N)     | O(1)     |
 | 04 | Lexicographically Smallest Encrypted String | Stack + Suffix Minimum             | O(N)     | O(N)     |
 | 05 | Asteroid Belt Survivors                     | Stack Collision Simulation         | O(N)     | O(N)     |
 | 06 | Sum of Every Student's Region Length        | Monotonic Stack (Nearest Greater)  | O(N)     | O(N)     |
 | 07 | Minimum Operations to Reduce Sum by Half    | Greedy Max Heap                    | O(K logN)| O(N)     |
 | 08 | Count Palindromic Subsequences              | Interval DP (Modulo Arithmetic)    | O(N²)    | O(N²)    |
 | 09 | Maximum Products Alex Can Buy               | Prefix Sum + Binary Search         | O(N+QlgN)| O(N)     |
 | 10 | Count Intervals Covering a Point            | Endpoint Sorting + Binary Search   | O((N+Q)lg| O(N)     |
 | 11 | Light Panel Toggle Sum                      | Difference Array / Prefix XOR      | O(N + M) | O(M)     |
 | 12 | Minimum Cost to Combine Array Into One      | Huffman Min-Heap Pair Summation    | O(N logN)| O(N)     |
 | 13 | Palindromic Pair Count                      | 26-Bit Parity Mask + HashMap       | O(26N)   | O(N)     |
 | 14 | Worth Taking on a Trip                      | Substring Pattern Matching         | O(N)     | O(1)     |
 | 15 | Minimum Total Complexity Over Days          | Partition Dynamic Programming      | O(D × N²)| O(D × N) |
 | 16 | Minimum Flips to Remove Every 10 Pair       | 2-State Dynamic Programming        | O(N)     | O(1)     |
 | 17 | Valid Bracket Sequence With At Most 1 Swap  | Prefix Balance & Min Deficit Check | O(N)     | O(1)     |
 | 18 | Maximum Minimum Magnetic Force              | Binary Search on Answer + Greedy   | O(N logR)| O(1)     |
 | 19 | Minimum Array Cost With Unlimited Insertions| Mathematical Observation (Unit)   | O(N)     | O(1)     |
 | 20 | Weighted Manhattan Distance City            | 1D Weighted Median (Independent X,Y| O(N logN)| O(N)     |
 | 21 | Maximum XOR with the Same Bit Length        | Prefix XOR Periodicity (Mod 4)     | O(1)     | O(1)     |
 | 22 | Minimum Array Cost After One Insertion      | Maximum Variance Reduction         | O(N)     | O(1)     |
 | 23 | Count Arrays Reachable by Threshold Decremen| Independent Coordinate Gaps        | O(N logN)| O(N)     |
 | 24 | Maximize Ones in a Binary String            | Greedy Left Propagation Reach      | O(N)     | O(1)     |
 | 25 | Minimize Array Sum After K Operations       | Greedy Max Heap Reduction          | O((N+K)lg| O(N)     |
 | 26 | Longest Square Chain Subset                 | Hash Set Exploration (x -> x²)     | O(N) avg | O(N)     |
 | 27 | Authentication Token System (Unexpired)     | Hash Map Expiry State Simulation   | O(Q)     | O(Q)     |
 | 28 | Minimum Cost to Reach Point N               | DP + Monotonic Deque Sliding Window| O(N)     | O(N)     |
 | 29 | Ideal Numbers (3^x * 5^y)                   | Double Geometric Power Traversal   | O(lg3*lg5| O(1)     |
 | 30 | Maximum Bitwise OR-Sum                      | Prefix & Suffix Bitwise OR Sweep   | O(N)     | O(N)     |
 | 31 | Shopkeeper Sale – Final Price of Items      | Next Smaller/Equal Monotonic Stack | O(N)     | O(N)     |
 | 32 | Count Palindromic Subsequences of Length 5  | Left/Right Pair Frequency Matches  | O(N)     | O(1)     |
 | 33 | Binary Cardinality Sort                     | Popcount + Value Multi-Key Sorting | O(N logN)| O(log N) |
 | 34 | Ball Collision Around Pivot Point           | Single-Pass Speed Comparison       | O(N)     | O(1)     |
 | 35 | Maximum Weighted Difference (1-based)       | Rearrangement Inequality (A2 asc)  | O(N logN)| O(1)     |
 | 36 | Delete Odd-Valued Nodes from Linked List    | Linked List Pointer Traversal      | O(N)     | O(1)     |
 | 37 | Shortest Substring Deletion for Distinct    | Prefix & Suffix Distinct Sets      | O(N)     | O(Σ)     |
 | 38 | Message Duplicate Detection Rate Limiter    | Hash Map Last-Delivery Timestamp   | O(N)     | O(U)     |
 ====================================================================================================
*/

// ====================================================================================================
// 1. LONGEST PALINDROMIC SUBSEQUENCE LENGTH
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a lowercase string `s`, return the length of its longest subsequence that is a palindrome.
  A subsequence is formed by deleting zero or more characters without changing the order.

  PATTERN:
  Interval DP

  INTUITION & KEY OBSERVATIONS:
  - Let dp[i][j] be the length of the longest palindromic subsequence in substring s[i...j].
  - Base case: dp[i][i] = 1 for all single characters.
  - Transition:
    * If s[i] == s[j]: dp[i][j] = 2 + dp[i + 1][j - 1].
    * If s[i] != s[j]: dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]).
  - Process substrings in decreasing order of start index i, increasing order of end index j.

  COMPLEXITY:
  - Time: O(N²)
  - Space: O(N²)
*/
class Solution1 {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        if (n == 0) return 0;
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) dp[i][j] = 2 + dp[i + 1][j - 1];
                else dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
        return dp[0][n - 1];
    }
};

// ====================================================================================================
// 2. NUMBER OF LONGEST STRICTLY INCREASING SUBSEQUENCES
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an integer array `nums`, find the maximum length `L` of a strictly increasing subsequence
  and return how many subsequences have length exactly `L`.
  Subsequences using different indices are counted separately.

  PATTERN:
  Dynamic Programming + Counting

  INTUITION & KEY OBSERVATIONS:
  - For each index i, maintain:
    * len[i]: length of longest increasing subsequence ending at index i.
    * cnt[i]: count of such subsequences of length len[i] ending at index i.
  - For each j < i where nums[j] < nums[i]:
    * If len[j] + 1 > len[i]: len[i] = len[j] + 1, cnt[i] = cnt[j].
    * Else if len[j] + 1 == len[i]: cnt[i] += cnt[j].
  - Find max_len = max(len[i]), then sum cnt[i] for all i with len[i] == max_len.

  COMPLEXITY:
  - Time: O(N²)
  - Space: O(N)
*/
class Solution2 {
public:
    long long countLongestIncreasing(const vector<int>& nums) {
        int n = nums.size(), best = 0;
        if (n == 0) return 0;
        vector<int> len(n, 1);
        vector<long long> cnt(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    if (len[j] + 1 > len[i]) len[i] = len[j] + 1, cnt[i] = cnt[j];
                    else if (len[j] + 1 == len[i]) cnt[i] += cnt[j];
                }
            }
            best = max(best, len[i]);
        }
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            if (len[i] == best) ans += cnt[i];
        }
        return ans;
    }
};

// ====================================================================================================
// 3. COUNT SUBSTRINGS CONTAINING ALL THREE LETTERS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s` containing only `'a'`, `'b'`, and `'c'`, count the number of substrings
  containing at least one `'a'`, one `'b'`, and one `'c'`.

  PATTERN:
  Last Occurrence Tracking / Sliding Window

  INTUITION & KEY OBSERVATIONS:
  - Maintain an array `last[3]` storing the most recent 0-based index of `'a'`, `'b'`, and `'c'`.
  - For each character s[i], update last[s[i] - 'a'] = i.
  - Any substring starting at index `start` and ending at `i` contains all 3 characters iff
    `start <= min({last[0], last[1], last[2]})`.
  - The number of valid starting positions is min({last[0], last[1], last[2]}) + 1.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
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
// 4. LEXICOGRAPHICALLY SMALLEST ENCRYPTED STRING
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given `originalString`.
  You have two strings initially empty: `temporaryString` and `encryptedString`.
  You may repeatedly:
  1. Move the first character of `originalString` to the end of `temporaryString`.
  2. Move the last character of `temporaryString` to the end of `encryptedString`.
  Continue until all characters are moved into `encryptedString`.
  Return the lexicographically smallest possible `encryptedString`.

  PATTERN:
  Stack + Suffix Minimum

  INTUITION & KEY OBSERVATIONS:
  - This simulates a stack where temporaryString is the stack.
  - To make encryptedString lexicographically minimal, we should greedily output characters
    that are smaller than or equal to anything remaining in originalString.
  - Compute suffix minimums mn[i] = min(s[i], mn[i + 1]).
  - Push s[i] to stack. While stack top <= mn[i + 1], pop to result.
  - Flush remaining characters from stack.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution4 {
public:
    string getEncryptedString(const string& s) {
        int n = s.size();
        vector<char> mn(n + 1, '{');
        for (int i = n - 1; i >= 0; --i) mn[i] = min(s[i], mn[i + 1]);
        string st, ans;
        for (int i = 0; i < n; ++i) {
            st += s[i];
            while (!st.empty() && st.back() <= mn[i + 1]) {
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
// 5. ASTEROID BELT SURVIVORS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array `asteroids`.
  * Absolute value = asteroid size.
  * Positive = moving right.
  * Negative = moving left.
  When a right-moving asteroid meets a left-moving asteroid:
  * smaller asteroid is destroyed,
  * if equal, both are destroyed.
  Return the asteroids remaining after all collisions.

  PATTERN:
  Stack Collision Simulation

  INTUITION & KEY OBSERVATIONS:
  - Asteroids moving left only collide with preceding asteroids moving right.
  - Use a vector as a stack.
  - For each asteroid x:
    * If x > 0: push to stack (moving right, no collision with earlier left-movers).
    * If x < 0: simulate collisions while stack top > 0 and asteroid is still alive.
      - If stack.back() < -x: stack top destroyed, pop and continue.
      - If stack.back() == -x: both destroyed, pop and mark alive = false.
      - If stack.back() > -x: current asteroid destroyed, alive = false.
    * If alive after collisions, push x.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution5 {
public:
    vector<long long> asteroidCollision(const vector<long long>& a) {
        vector<long long> st;
        for (long long x : a) {
            bool alive = true;
            while (alive && x < 0 && !st.empty() && st.back() > 0) {
                if (st.back() < -x) st.pop_back();
                else {
                    if (st.back() == -x) st.pop_back();
                    alive = false;
                }
            }
            if (alive) st.push_back(x);
        }
        return st;
    }
};

// ====================================================================================================
// 6. SUM OF EVERY STUDENT'S REGION LENGTH
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  `n` students stand in a line with heights `heights[i]`.
  The region of student `i` is the longest contiguous block containing `i` such that
  the maximum height in that block is exactly `heights[i]`.
  Return the sum of region lengths of all students.

  PATTERN:
  Monotonic Stack (Nearest Strictly Greater Elements)

  INTUITION & KEY OBSERVATIONS:
  - The block containing student i where heights[i] is the maximum must not contain
    any student with height > heights[i].
  - Find the nearest strictly greater element to the left L[i] and to the right R[i].
  - The valid region is index range (L[i], R[i]), so its length is R[i] - L[i] - 1.
  - Sum lengths over all students i.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution6 {
public:
    long long calculateTotalRegion(vector<int> h) {
        int n = h.size();
        vector<int> L(n), R(n), st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && h[st.back()] <= h[i]) st.pop_back();
            L[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        st.clear();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && h[st.back()] <= h[i]) st.pop_back();
            R[i] = st.empty() ? n : st.back();
            st.push_back(i);
        }
        long long ans = 0;
        for (int i = 0; i < n; ++i) ans += R[i] - L[i] - 1;
        return ans;
    }
};

// ====================================================================================================
// 7. MINIMUM OPERATIONS TO REDUCE ARRAY SUM BY HALF
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array of positive integers `nums`.
  In one operation, choose any element and replace it with exactly half its current value.
  Find the minimum operations required so that the total array sum becomes at most half of its original sum.

  PATTERN:
  Greedy + Max Heap

  INTUITION & KEY OBSERVATIONS:
  - To minimize operations, each operation should achieve the maximum possible sum reduction.
  - Halving value x reduces the sum by x / 2. Therefore, always choose the currently largest element.
  - Maintain a max-heap of values. Pop the largest element, halve it, accumulate the reduction,
    and push it back until total reduction >= original_sum / 2.

  COMPLEXITY:
  - Time: O(K log N) where K is number of operations.
  - Space: O(N)
*/
class Solution7 {
public:
    long long minOperations(const vector<long long>& nums) {
        priority_queue<long double> pq;
        long double sum = 0, reduced = 0;
        for (auto x : nums) sum += x, pq.push(x);
        long long ans = 0;
        while (reduced < sum / 2.0) {
            long double x = pq.top(); pq.pop();
            x /= 2.0;
            reduced += x;
            pq.push(x);
            ans++;
        }
        return ans;
    }
};

// ====================================================================================================
// 8. COUNT PALINDROMIC SUBSEQUENCES
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a lowercase string `s`, count all non-empty palindromic subsequences.
  Two subsequences are different if they use different sets of positions, even if they produce the same string.
  Return the answer modulo 1000000007.

  PATTERN:
  Interval DP (Inclusion-Exclusion)

  INTUITION & KEY OBSERVATIONS:
  - dp[i][j] = number of palindromic subsequences in substring s[i...j].
  - If s[i] == s[j]:
    dp[i][j] = dp[i + 1][j] + dp[i][j - 1] + 1 (the +1 is for subsequence "s[i]s[j]" without inner chars).
  - If s[i] != s[j]:
    dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1] (subtract duplicate middle part).
  - All additions and subtractions modulo 10^9 + 7.

  COMPLEXITY:
  - Time: O(N²)
  - Space: O(N²)
*/
class Solution8 {
    static const long long MOD = 1000000007;
public:
    long long countPalindromicSubsequences(const string& s) {
        int n = s.size();
        if (n == 0) return 0;
        vector<vector<long long>> dp(n, vector<long long>(n, 0));
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) dp[i][j] = 2 + dp[i + 1][j - 1];
                else dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                dp[i][j] = (dp[i][j] % MOD + MOD) % MOD;
            }
        }
        return dp[0][n - 1];
    }
};

// ====================================================================================================
// 9. MAXIMUM PRODUCTS ALEX CAN BUY
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  There are `n` cubicles with non-decreasing prices: prices[1] <= prices[2] <= ... <= prices[n].
  For every query `(pos, amount)`:
  * Alex starts at cubicle `pos` (1-based).
  * He can visit cubicles from `pos` to `n`.
  * He may buy at most one product from each cubicle.
  * Total cost must not exceed `amount`.
  Return the maximum number of products he can buy for each query.

  PATTERN:
  Prefix Sum + Binary Search (upper_bound)

  INTUITION & KEY OBSERVATIONS:
  - Since prices are sorted ascending and positive, buying the cheapest available items
    greedily from `pos` onward maximizes the count of purchased products.
  - Build prefix sum array `pre`. Cost of buying from index `l = pos - 1` to `r` is `pre[r + 1] - pre[l]`.
  - We require pre[r + 1] - pre[l] <= money <=> pre[r + 1] <= pre[l] + money.
  - Use `upper_bound` on pre to find largest valid index in O(log N).

  COMPLEXITY:
  - Time: O(N + Q log N)
  - Space: O(N)
*/
class Solution9 {
public:
    vector<int> maximumProducts(const vector<long long>& prices, const vector<pair<int, long long>>& queries) {
        int n = prices.size();
        vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + prices[i];
        vector<int> ans;
        for (auto [pos, money] : queries) {
            int l = pos - 1;
            long long limit = pre[l] + money;
            int r = upper_bound(pre.begin() + l, pre.end(), limit) - pre.begin() - 1;
            ans.push_back(r - l);
        }
        return ans;
    }
};

// ====================================================================================================
// 10. COUNT INTERVALS COVERING A POINT
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given `n` closed intervals `[l[i], r[i]]` and several query points.
  For every query `x`, return how many intervals satisfy: `l[i] <= x <= r[i]`.
  Intervals with identical endpoints are still counted separately.

  PATTERN:
  Endpoint Sorting + Binary Search

  INTUITION & KEY OBSERVATIONS:
  - An interval [L_i, R_i] covers query point x iff L_i <= x AND R_i >= x.
  - Total covering intervals = (Intervals with L_i <= x) - (Intervals with R_i < x).
  - Sort L array and R array independently.
  - Count of L_i <= x: upper_bound(L, x) - L.begin().
  - Count of R_i < x: lower_bound(R, x) - R.begin().
  - Difference gives exact count in O(log N) per query.

  COMPLEXITY:
  - Time: O((N + Q) log N)
  - Space: O(N)
*/
class Solution10 {
public:
    vector<int> countContainingIntervals(const vector<int>& l, const vector<int>& r, const vector<int>& queries) {
        vector<int> L = l, R = r;
        sort(L.begin(), L.end());
        sort(R.begin(), R.end());
        vector<int> ans;
        for (int x : queries) {
            int started = upper_bound(L.begin(), L.end(), x) - L.begin();
            int ended = lower_bound(R.begin(), R.end(), x) - R.begin();
            ans.push_back(started - ended);
        }
        return ans;
    }
};

// ====================================================================================================
// 11. LIGHT PANEL TOGGLE SUM
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  There are `m` lamps numbered `1` to `m`, initially all OFF.
  You are given `n` operations `[l, r]`. Each operation flips every lamp from `l` to `r` (OFF <-> ON).
  After all operations, return the sum of the numbers of all lamps that are ON.

  PATTERN:
  Difference Array / Prefix XOR

  INTUITION & KEY OBSERVATIONS:
  - Flipping a range [l, r] is equivalent to XOR-toggling boundary markers:
    diff[l] ^= 1 and diff[r + 1] ^= 1.
  - Maintain running prefix XOR `state`. At lamp i: state ^= diff[i].
  - If state == 1 (lamp is ON), add lamp index i to total sum.

  COMPLEXITY:
  - Time: O(N + M)
  - Space: O(M)
*/
class Solution11 {
public:
    long long lampToggleSum(int m, const vector<pair<int, int>>& ops) {
        vector<int> diff(m + 2, 0);
        for (auto [l, r] : ops) {
            diff[l] ^= 1;
            diff[r + 1] ^= 1;
        }
        long long ans = 0;
        int state = 0;
        for (int i = 1; i <= m; ++i) {
            state ^= diff[i];
            if (state) ans += i;
        }
        return ans;
    }
};

// ====================================================================================================
// 12. MINIMUM COST TO COMBINE THE ARRAY INTO ONE ELEMENT
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array of positive integers.
  Repeatedly choose any two elements `x` and `y`:
  * Remove them.
  * Insert `x + y`.
  * Pay a cost of `x + y`.
  Return the minimum total cost required to reduce the array to one element.

  PATTERN:
  Min-Heap / Huffman Greedy Optimal Merge

  INTUITION & KEY OBSERVATIONS:
  - Earlier merged values participate in subsequent operations, multiplying their impact.
  - To minimize the total sum, always greedily merge the two smallest available numbers.
  - Store numbers in a min-heap, pop two smallest, add sum to total cost, and push sum back.

  COMPLEXITY:
  - Time: O(N log N)
  - Space: O(N)
*/
class Solution12 {
public:
    long long min_merge_cost(vector<long long> arr) {
        priority_queue<long long, vector<long long>, greater<long long>> pq(arr.begin(), arr.end());
        long long ans = 0;
        while (pq.size() > 1) {
            long long a = pq.top(); pq.pop();
            long long b = pq.top(); pq.pop();
            ans += a + b;
            pq.push(a + b);
        }
        return ans;
    }
};

// ====================================================================================================
// 13. PALINDROMIC PAIR COUNT
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given `n` lowercase strings.
  For every pair `(i, j)` where `i < j`, combine all characters of both strings.
  The pair is called palindromic if the combined characters can be rearranged to form a palindrome.
  Return the number of such pairs.

  PATTERN:
  26-Bit Parity Mask + HashMap

  INTUITION & KEY OBSERVATIONS:
  - A concatenated string can form a palindrome iff at most one character has an odd count.
  - Character parity mask: bit c is 1 if ('a' + c) count is odd, 0 if even.
  - Concatenating two strings XORs their masks: m1 ^ m2.
  - Valid iff m1 ^ m2 == 0 or m1 ^ m2 == (1 << b) for some 0 <= b < 26.
  - Query hash map for matching masks, then insert current mask.

  COMPLEXITY:
  - Time: O(26N)
  - Space: O(N)
*/
class Solution13 {
public:
    long long count_palindrome_pairs(const vector<string>& strings) {
        unordered_map<int, long long> cnt;
        long long ans = 0;
        for (const string& s : strings) {
            int mask = 0;
            for (char c : s) mask ^= 1 << (c - 'a');
            ans += cnt[mask];
            for (int b = 0; b < 26; ++b) ans += cnt[mask ^ (1 << b)];
            cnt[mask]++;
        }
        return ans;
    }
};

// ====================================================================================================
// 14. WORTH TAKING ON A TRIP
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an item's lowercase name `s`.
  The item is worth taking if its name contains at least one of these substrings:
  `co`, `f`, `ha`, `he`, `ke`, `ok`, `po`, `sc`.
  Return `true` if at least one marker occurs, `false` otherwise.

  PATTERN:
  String Matching

  INTUITION & KEY OBSERVATIONS:
  - Check each of the 8 fixed markers using string::find.
  - As soon as any marker is found as a substring, return true. If none match, return false.

  COMPLEXITY:
  - Time: O(8N) ≈ O(N)
  - Space: O(1)
*/
class Solution14 {
public:
    bool isWorthTaking(const string& s) {
        static const vector<string> mark = {"co", "f", "ha", "he", "ke", "ok", "po", "sc"};
        for (const string& x : mark) {
            if (s.find(x) != string::npos) return true;
        }
        return false;
    }
};

// ====================================================================================================
// 15. MINIMUM TOTAL COMPLEXITY OVER DAYS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given `n` tasks in a fixed order and must complete them in exactly `D` days.
  Tasks done on each day must form a contiguous block, and every day must receive at least one task.
  The cost of a day is the maximum complexity among tasks done that day.
  Return the minimum possible total cost.

  PATTERN:
  Partition Dynamic Programming

  INTUITION & KEY OBSERVATIONS:
  - dp[d][i] = minimum total complexity for first i tasks scheduled across d days.
  - Base case: dp[0][0] = 0, all other dp entries initialized to INF.
  - Transition: For day d, try all possible starts j for the last day:
    dp[d][i] = min_{j} (dp[d - 1][j] + max_{k=j}^{i-1} a[k]).
  - Iterating j backwards from i - 1 down to d - 1 allows maintaining running max in O(1).

  COMPLEXITY:
  - Time: O(D × N²)
  - Space: O(D × N)
*/
class Solution15 {
public:
    long long minTotalComplexity(int n, int days, const vector<long long>& a) {
        const long long INF = 4e18;
        vector<vector<long long>> dp(days + 1, vector<long long>(n + 1, INF));
        dp[0][0] = 0;
        for (int d = 1; d <= days; ++d) {
            for (int i = d; i <= n; ++i) {
                long long mx = 0;
                for (int j = i - 1; j >= d - 1; --j) {
                    mx = max(mx, a[j]);
                    dp[d][i] = min(dp[d][i], dp[d - 1][j] + mx);
                }
            }
        }
        return dp[days][n];
    }
};

// ====================================================================================================
// 16. MINIMUM FLIPS TO REMOVE EVERY "10" PAIR
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given a binary string `s`.
  A string is bad if it contains the substring `"10"`.
  In one operation, you may flip any one character (`0 -> 1` or `1 -> 0`).
  Return the minimum number of flips needed so that `"10"` does not occur anywhere.

  PATTERN:
  2-State Dynamic Programming

  INTUITION & KEY OBSERVATIONS:
  - A string without "10" must have all '0's before any '1's (shape: 00...011...1).
  - Track dp0 (min flips where prefix ends in '0') and dp1 (min flips where prefix ends in '1').
  - At index i:
    * ndp0 = dp0 + (s[i] != '0')
    * ndp1 = min(dp0, dp1) + (s[i] != '1')

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution16 {
public:
    long long minFlips(int n, const string& s) {
        long long dp0 = (s[0] != '0');
        long long dp1 = (s[0] != '1');
        for (int i = 1; i < n; ++i) {
            long long ndp0 = dp0 + (s[i] != '0');
            long long ndp1 = min(dp0, dp1) + (s[i] != '1');
            dp0 = ndp0;
            dp1 = ndp1;
        }
        return min(dp0, dp1);
    }
};

// ====================================================================================================
// 17. VALID BRACKET SEQUENCE WITH AT MOST ONE SWAP
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given a string containing only `'('` and `')'`.
  You may perform at most one swap of two positions.
  Return `1` if the string is already valid or can become a valid bracket sequence using one swap.
  Otherwise return `0`.

  PATTERN:
  Prefix Balance & Minimum Deficit Check

  INTUITION & KEY OBSERVATIONS:
  - For any valid bracket sequence, total balance must be 0, and all prefix balances >= 0.
  - A single swap of a ')' at index i with a '(' at index j (i < j) increases prefix balances
    in range [i, j - 1] by exactly +2.
  - Therefore, at most one swap can fix a prefix balance deficit of at most 2.
  - Condition: balance == 0 && min(prefix_balance) >= -2.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution17 {
public:
    int canBeValid(const string& s) {
        int balance = 0, mn = 0;
        for (char c : s) {
            balance += (c == '(' ? 1 : -1);
            mn = min(mn, balance);
        }
        return balance == 0 && mn >= -2;
    }
};

// ====================================================================================================
// 18. MAXIMUM MINIMUM MAGNETIC FORCE
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given coordinates of `n` baskets and `m` identical balls.
  Place all `m` balls into different baskets.
  The magnetic force between balls at positions `x` and `y` is `|x - y|`.
  Maximize the minimum magnetic force between any two placed balls.

  PATTERN:
  Binary Search on Answer + Greedy Placement

  INTUITION & KEY OBSERVATIONS:
  - Sort basket positions in ascending order.
  - Binary search minimum distance `dist` in range [1, position.back() - position.front()].
  - Greedy check: place first ball at position[0]. For each subsequent basket, place a ball
    if position[i] - last >= dist. If >= m balls can be placed, dist is feasible.

  COMPLEXITY:
  - Time: O(N log(max - min))
  - Space: O(1) extra
*/
class Solution18 {
public:
    long long maxDistance(vector<long long>& position, int m) {
        sort(position.begin(), position.end());
        auto can = [&](long long dist) {
            int balls = 1;
            long long last = position[0];
            for (size_t i = 1; i < position.size(); ++i) {
                if (position[i] - last >= dist) {
                    balls++;
                    last = position[i];
                }
            }
            return balls >= m;
        };
        long long l = 0, r = position.back() - position.front();
        while (l < r) {
            long long mid = l + (r - l + 1) / 2;
            if (can(mid)) l = mid;
            else r = mid - 1;
        }
        return l;
    }
};

// ====================================================================================================
// 19. MINIMUM ARRAY COST WITH UNLIMITED INSERTIONS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  The cost of an array is the sum of squared differences between adjacent elements:
  (b[1]-b[0])² + (b[2]-b[1])² + ...
  You are given an integer array `a`.
  You may insert any number of integers anywhere while keeping all original elements in the same relative order.
  Return the minimum possible cost.

  PATTERN:
  Mathematical Observation (Unit Differences)

  INTUITION & KEY OBSERVATIONS:
  - Between two integers x and y with difference d = |x - y|, if we insert d - 1 intermediate
    integers with step 1, each step contributes 1² = 1.
  - The sum of squared differences becomes 1 * d = |x - y|.
  - By convexity of f(t) = t², unit steps achieve the absolute minimum sum of squares.
  - Total minimum cost = Σ |a[i] - a[i - 1]|.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution19 {
public:
    long long minInsertionCost(const vector<long long>& a) {
        long long ans = 0;
        for (size_t i = 1; i < a.size(); ++i) {
            ans += abs(a[i] - a[i - 1]);
        }
        return ans;
    }
};

// ====================================================================================================
// 20. WEIGHTED MANHATTAN DISTANCE CITY
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  There are `N` cities. City `i` has coordinates `(xi, yi)` and population `pi`.
  Choose integer coordinates `(X, Y)` minimizing:
  Σ pi × (|X - xi| + |Y - yi|).
  If multiple optimal answers exist, return the lexicographically smallest `(X, Y)`.

  PATTERN:
  1D Weighted Median (Independent Dimensions)

  INTUITION & KEY OBSERVATIONS:
  - Manhattan distance splits independently into Σ pi |X - xi| and Σ pi |Y - yi|.
  - The function f(X) = Σ pi |X - xi| is minimized at the weighted median.
  - For lexicographical tie-breaking, select the smallest coordinate where cumulative
    weight reaches at least half of the total population (2 * sum >= total).

  COMPLEXITY:
  - Time: O(N log N)
  - Space: O(N)
*/
class Solution20 {
public:
    pair<ll, ll> solve(const vector<array<ll, 3>>& cities) {
        vector<pair<ll, ll>> xs, ys;
        ll total = 0;
        for (auto [x, y, p] : cities) {
            xs.push_back({x, p});
            ys.push_back({y, p});
            total += p;
        }
        sort(xs.begin(), xs.end());
        sort(ys.begin(), ys.end());
        auto weightedMedian = [&](vector<pair<ll, ll>>& v) {
            ll sum = 0;
            for (auto [x, w] : v) {
                sum += w;
                if (2 * sum >= total) return x;
            }
            return v.back().first;
        };
        return {weightedMedian(xs), weightedMedian(ys)};
    }
};

// ====================================================================================================
// 21. MAXIMUM XOR WITH THE SAME BIT LENGTH
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a number `n`, choose an integer `x` such that `x` has the same bit length as `n`, and `x >= n`.
  Maximize n XOR (n+1) ... XOR x.
  If there is a tie, choose the smallest `x`.

  PATTERN:
  Prefix XOR Periodicity (Modulo 4)

  INTUITION & KEY OBSERVATIONS:
  - Prefix XOR P(x) = 0 ^ 1 ^ ... ^ x repeats with period 4:
    * x % 4 == 0 -> x
    * x % 4 == 1 -> 1
    * x % 4 == 2 -> x + 1
    * x % 4 == 3 -> 0
  - If n % 4 == 0, XOR(n ... x) = P(x).
  - Test the last 4 valid values <= max_with_same_bits.

  COMPLEXITY:
  - Time: O(1)
  - Space: O(1)
*/
class Solution21 {
    ull pref(ull x) {
        if (x % 4 == 0) return x;
        if (x % 4 == 1) return 1;
        if (x % 4 == 2) return x + 1;
        return 0;
    }
public:
    ull solve(ull n) {
        int bits = 64 - __builtin_clzll(n);
        ull hi = (bits == 64 ? ULLONG_MAX : (1ULL << bits) - 1);
        ull ans = n, best = pref(n);
        for (int i = 0; i < 4 && hi >= n + i; ++i) {
            ull x = hi - i, v = pref(x);
            if (v > best || (v == best && x < ans)) {
                best = v;
                ans = x;
            }
        }
        return ans;
    }
};

// ====================================================================================================
// 22. MINIMUM ARRAY COST AFTER ONE INSERTION
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array `a`, the cost is Σ (a[i] - a[i - 1])².
  You are allowed to insert at most ONE integer `x` between any adjacent pair `a[i-1]` and `a[i]`.
  Find the minimum possible array cost.

  PATTERN:
  Maximum Variance Reduction via Midpoint Insertion

  INTUITION & KEY OBSERVATIONS:
  - Inserting x between u and v replaces (u - v)² with (u - x)² + (x - v)².
  - Choosing x as the integer closest to (u + v) / 2 reduces the difference by floor(d² / 2).
  - Find the pair (a[i-1], a[i]) that gives the maximum reduction floor(d² / 2) and subtract it.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution22 {
public:
    long long solve(vector<long long> a) {
        long long sum = 0, save = 0;
        for (size_t i = 1; i < a.size(); ++i) {
            long long d = a[i] - a[i - 1];
            sum += d * d;
            save = max(save, (d * d) / 2);
        }
        return sum - save;
    }
};

// ====================================================================================================
// 23. COUNT ARRAYS REACHABLE BY THRESHOLD DECREMENTS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array `a` of non-negative integers. In each operation, you can choose a
  threshold `T` and decrement all elements that are `>= T` by 1.
  Count how many distinct arrays can be obtained through any sequence of such operations.
  Return the count modulo 10^9 + 7.

  PATTERN:
  Independent Gap Coordinates

  INTUITION & KEY OBSERVATIONS:
  - Sort and remove duplicates from a.
  - Any valid sequence of decrements transforms each gap (a[i] - a[i - 1]) independently into
    any integer in [0, a[i] - a[i - 1]].
  - The smallest element a[0] can become any value in [0, a[0]].
  - Total reachable configurations = (a[0] + 1) * Π (a[i] - a[i - 1] + 1) % MOD.

  COMPLEXITY:
  - Time: O(N log N)
  - Space: O(N)
*/
class Solution23 {
    static const long long MOD = 1000000007;
public:
    long long solve(vector<long long> a) {
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        long long ans = a[0] + 1;
        for (size_t i = 1; i < a.size(); ++i) {
            ans = ans * ((a[i] - a[i - 1] + 1) % MOD) % MOD;
        }
        return ans % MOD;
    }
};

// ====================================================================================================
// 24. MAXIMIZE ONES IN A BINARY STRING
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given a binary string `s` and integer `k`.
  In one operation, you can set `s[i] = max(s[i], s[i + 1])`.
  A `1` can propagate only to the left.
  Find the maximum number of `'1'`s possible after at most `k` operations.

  PATTERN:
  Greedy Left Propagation Reach

  INTUITION & KEY OBSERVATIONS:
  - A '1' at index j can propagate leftward to index i (i < j).
  - Every zero that appears before the last '1' in the string can be converted to '1'.
  - If there are no '1's, answer is 0.
  - Count existing ones and find the last index containing '1'.
  - Available convertible zeros before last '1' = (last + 1) - ones.
  - With at most k operations, we can convert min(k, zeros) zeros into ones.
  - Total ones = ones + min(k, zeros).

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution24 {
public:
    long long maxOnes(const string& s, long long k) {
        int n = s.size(), last = -1;
        long long ones = 0;
        for (int i = 0; i < n; ++i) {
            if (s[i] == '1') {
                ones++;
                last = i;
            }
        }
        if (last == -1) return 0;
        long long zeros = last + 1 - ones;
        return ones + min(k, zeros);
    }
};

// ====================================================================================================
// 25. MINIMIZE ARRAY SUM AFTER K OPERATIONS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array `nums` and an integer `k`.
  In one operation, choose any element `x` and replace it with `ceil(x / 2)`.
  Find the minimum possible array sum after at most `k` operations.

  PATTERN:
  Greedy Max Heap Reduction

  INTUITION & KEY OBSERVATIONS:
  - Replacing x with ceil(x / 2) reduces the sum by floor(x / 2).
  - To maximize the reduction at each step, greedily choose the currently largest element.
  - Use a max priority queue. Pop the largest element, calculate new value ceil(x / 2) = (x + 1) / 2,
    decrease running sum, and push the new value back into heap.

  COMPLEXITY:
  - Time: O((N + K) log N)
  - Space: O(N)
*/
class Solution25 {
public:
    long long minimumSum(vector<long long> nums, long long k) {
        priority_queue<long long> pq;
        long long sum = 0;
        for (long long x : nums) pq.push(x), sum += x;
        while (k-- && !pq.empty() && pq.top() > 1) {
            long long x = pq.top(); pq.pop();
            long long y = (x + 1) / 2;
            sum -= (x - y);
            pq.push(y);
        }
        return sum;
    }
};

// ====================================================================================================
// 26. LONGEST SQUARE CHAIN SUBSET
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array `B` of positive integers.
  A subset is a square chain if, when sorted, every element (except the first) is the square of
  the previous element: B[i] = B[i - 1]².
  Find the maximum size of a square chain subset.

  PATTERN:
  Hash Set / Square Step Chain Exploration

  INTUITION & KEY OBSERVATIONS:
  - All 1's in the array can form a chain of 1's since 1² = 1. Count of 1's is a candidate answer.
  - Insert all unique elements into an unordered_set.
  - For each starting element x > 1:
    * While set contains x: increment length, and advance x = x * x (guarded against overflow).
    * Track maximum chain length.

  COMPLEXITY:
  - Time: O(N) average
  - Space: O(N)
*/
class Solution26 {
public:
    int solution(int n, vector<int>& B) {
        unordered_set<long long> st;
        int ones = 0, ans = 0;
        for (int x : B) {
            st.insert(x);
            if (x == 1) ones++;
        }
        ans = ones;
        for (long long start : st) {
            if (start == 1) continue;
            long long x = start;
            int len = 0;
            while (st.count(x)) {
                len++;
                if (x > 1000000000LL / x) break;
                x *= x;
            }
            ans = max(ans, len);
        }
        return ans;
    }
};

// ====================================================================================================
// 27. AUTHENTICATION TOKEN SYSTEM (UNEXPIRED TOKENS)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Design a time-based authentication token system that manages tokens with a fixed `time_to_live`.
  You are given an integer `time_to_live` and a list of query strings `queries`.
  Queries are of three types:
  1. "generate token time": Generates a new token at given `time`. The token expires at `time + time_to_live`.
  2. "renew token time": Renews the token at given `time` only if the token exists and is currently valid
     (i.e. its expiry time is strictly greater than `time`). If valid, its expiry is updated to `time + time_to_live`.
  3. "count time": Returns the number of currently active, unexpired tokens at given `time` (expiry > time).
  Return a list of integers containing the results for every "count" query.

  PATTERN:
  Hash Map Expiry Tracking / Simulation

  INTUITION & KEY OBSERVATIONS:
  - Maintain a hash map `expiry[token]` storing the expiry timestamp of each active token.
  - For "generate token time": set `expiry[token] = time + time_to_live`.
  - For "renew token time": check if `expiry.count(token) && expiry[token] > time`. If true, update.
    (If `expiry[token] <= time`, the token is already expired and cannot be renewed).
  - For "count time": iterate over the map and count how many tokens have `expiry > time`.

  COMPLEXITY:
  - Time: O(Q) for generate/renew, O(Tokens) per count query.
  - Space: O(Tokens)
*/
class Solution27 {
public:
    vector<int> getUnexpiredTokens(int time_to_live, vector<string> queries) {
        unordered_map<string, int> expiry;
        vector<int> ans;
        for (const string& q : queries) {
            stringstream ss(q);
            string type, token;
            int time;
            ss >> type;
            if (type == "generate") {
                ss >> token >> time;
                expiry[token] = time + time_to_live;
            } else if (type == "renew") {
                ss >> token >> time;
                if (expiry.count(token) && expiry[token] > time) {
                    expiry[token] = time + time_to_live;
                }
            } else { // count
                ss >> time;
                int cnt = 0;
                for (const auto& it : expiry) {
                    if (it.second > time) cnt++;
                }
                ans.push_back(cnt);
            }
        }
        return ans;
    }
};

// ====================================================================================================
// 28. MINIMUM COST TO REACH POINT N
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an integer `n`, an array `cost` of size `n` representing costs of points 0 to n - 1,
  and a maximum jump capacity `k`.
  Starting at point 0, you can jump from any point i to any point j where `i < j <= i + k`.
  The cost to land on point i is `cost[i]`.
  Find the minimum total cost to reach point `n - 1` starting from point 0.

  PATTERN:
  Dynamic Programming + Monotonic Deque (Sliding Window Minimum)

  INTUITION & KEY OBSERVATIONS:
  - Transition: dp[i] = cost[i] + min_{j = max(0, i - k)}^{i - 1} dp[j].
  - Naive evaluation takes O(n * k) time.
  - Maintain a monotonic increasing deque storing indices of previous points within distance k.
  - Deque front always holds the index with the minimum dp value in the current window [i - k, i - 1].
  - Pop expired indices (front < i - k) in O(1) amortized time.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution28 {
public:
    long long getMinimumCost(int n, const vector<int>& cost, int k) {
        vector<long long> dp(n);
        deque<int> dq;
        dp[0] = cost[0];
        dq.push_back(0);
        for (int i = 1; i < n; ++i) {
            while (!dq.empty() && dq.front() < i - k) dq.pop_front();
            dp[i] = dp[dq.front()] + cost[i];
            while (!dq.empty() && dp[dq.back()] >= dp[i]) dq.pop_back();
            dq.push_back(i);
        }
        return dp[n - 1];
    }
};

// ====================================================================================================
// 29. IDEAL NUMBERS (3^x * 5^y)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  An ideal number is defined as a positive integer of the form:
      3^x * 5^y, where x >= 0 and y >= 0.
  Given two 64-bit positive integers `low` and `high`, find the count of ideal numbers that lie
  within the inclusive range [low, high].

  PATTERN:
  Double Geometric Power Traversal / Bounded Enumeration

  INTUITION & KEY OBSERVATIONS:
  - Powers of 3 grow exponentially (up to 3^39 for 64-bit integers), and powers of 5 grow up to 5^27.
  - The total number of pairs (x, y) such that 3^x * 5^y <= high is extremely small (fewer than 1,000 pairs).
  - Iterate power `a` through powers of 3: 1, 3, 9, 27, ... while a <= high.
  - Inside, iterate power `b` through powers of 5: 1, 5, 25, ... while a * b <= high.
  - If a * b >= low, increment count.
  - Guard against integer overflow by checking `b > high / 5` and `a > high / 3`.

  COMPLEXITY:
  - Time: O(log3(high) * log5(high)) ≈ O(1) operations
  - Space: O(1)
*/
class Solution29 {
public:
    long long getIdealNums(long long low, long long high) {
        long long ans = 0;
        for (long long a = 1; a <= high; ) {
            for (long long b = 1; a * b <= high; ) {
                long long num = a * b;
                if (num >= low) ans++;
                if (b > high / 5) break;
                b *= 5;
            }
            if (a > high / 3) break;
            a *= 3;
        }
        return ans;
    }
};

// ====================================================================================================
// 30. MAXIMUM BITWISE OR-SUM
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an integer array `arr` and an integer `k`.
  In an operation, you can multiply any element by 2 (which shifts its bits left by 1).
  You can perform at most `k` such operations in total.
  Find the maximum possible bitwise OR sum of the entire array:
      arr[0] | arr[1] | ... | arr[n - 1]
  after applying at most `k` operations.

  PATTERN:
  Prefix & Suffix Bitwise OR Sweep

  INTUITION & KEY OBSERVATIONS:
  - Doubling a number shifts its binary bits to the left, creating higher-order bits.
  - To maximize the bitwise OR, all k doubling operations should be concentrated on a SINGLE element,
    multiplying that one element by 2^k (arr[i] << k).
  - We can try each index i from 0 to n - 1 as the chosen element.
  - For a chosen index i, the total OR is:
      (pre[i] | ((long long)arr[i] << k) | suf[i + 1])
  - Precomputing prefix OR array `pre` and suffix OR array `suf` allows O(1) evaluation per candidate.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution30 {
public:
    long long getMaxOrSum(vector<int>& arr, int k) {
        int n = arr.size();
        vector<long long> pre(n + 1, 0), suf(n + 1, 0);
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] | arr[i];
        for (int i = n - 1; i >= 0; --i) suf[i] = suf[i + 1] | arr[i];
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            ans = max(ans, pre[i] | ((long long)arr[i] << k) | suf[i + 1]);
        }
        return ans;
    }
};

// ====================================================================================================
// 31. SHOPKEEPER SALE – FINAL PRICE OF ITEMS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  A shopkeeper gives a discount on item `i` equal to the price of the first subsequent item `j` (j > i)
  such that `prices[j] <= prices[i]`.
  If no such item exists, item `i` receives no discount (discount = 0).
  Find:
  1. The total discounted price paid for all items: sum(final_prices).
  2. The list of indices (in strictly increasing order) of items that received NO discount.

  PATTERN:
  Next Smaller or Equal Element via Monotonic Stack

  INTUITION & KEY OBSERVATIONS:
  - For each item, we need the first element to its right with price <= prices[i].
  - Use a monotonic stack storing indices of items waiting for their discount.
  - As we iterate with index i:
    * While stack top j has prices[i] <= prices[j], item j gets discount prices[i].
      Update final[j] -= prices[i] and pop j.
    * Push i onto stack.
  - All indices remaining in the stack received no discount.
  - Collect remaining indices, reverse them to get increasing index order.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(N)
*/
class Solution31 {
public:
    pair<long long, vector<int>> finalPrice(const vector<int>& prices) {
        int n = prices.size();
        vector<int> final = prices;
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && prices[i] <= prices[st.top()]) {
                final[st.top()] -= prices[i];
                st.pop();
            }
            st.push(i);
        }
        long long sum = 0;
        for (int x : final) sum += x;
        vector<int> noDiscount;
        while (!st.empty()) {
            noDiscount.push_back(st.top());
            st.pop();
        }
        reverse(noDiscount.begin(), noDiscount.end());
        return {sum, noDiscount};
    }
};

// ====================================================================================================
// 32. COUNT PALINDROMIC SUBSEQUENCES OF LENGTH 5
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a binary string `s` (consisting only of `'0'` and `'1'`), count the number of palindromic
  subsequences of length exactly 5.
  A length-5 palindrome has the form: `a b c b a` where a, b, c in {'0', '1'}.
  Return the count modulo 10^9 + 7.

  PATTERN:
  Left/Right Pair Frequency Counting Around Center

  INTUITION & KEY OBSERVATIONS:
  - In `a b c b a`, character c is the center at index i.
  - The prefix before i must contribute subsequence `a b`.
  - The suffix after i must contribute subsequence `b a`.
  - Since the alphabet is binary ({'0', '1'}), there are only 2 * 2 = 4 choices for the pair (a, b):
    (0,0), (0,1), (1,0), (1,1).
  - For each of the 4 pairs (a, b):
    * Precompute right-side counts of `b` and pairs `b a`.
    * Scan from left to right, maintaining left-side counts of `a` and pairs `a b`.
    * At each center index i, add `(leftAB * rightBA) % MOD` to the answer.

  COMPLEXITY:
  - Time: O(4 * N) = O(N)
  - Space: O(1)
*/
class Solution32 {
public:
    int getPalindromesCount(string s) {
        const long long MOD = 1e9 + 7;
        int n = s.size();
        long long ans = 0;
        for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
                long long leftA = 0, leftAB = 0;
                long long rightB = 0, rightBA = 0;
                for (int i = n - 1; i >= 0; --i) {
                    int x = s[i] - '0';
                    if (x == a) rightBA += rightB;
                    if (x == b) rightB++;
                }
                for (int i = 0; i < n; ++i) {
                    int x = s[i] - '0';
                    if (x == b) rightB--;
                    if (x == a) rightBA -= rightB;
                    ans = (ans + leftAB * rightBA) % MOD;
                    if (x == a) leftA++;
                    if (x == b) leftAB += leftA;
                }
            }
        }
        return ans;
    }
};

// ====================================================================================================
// 33. BINARY CARDINALITY SORT
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array of integers `nums`.
  Sort the array in ascending order according to the following rules:
  1. Primary key: The binary cardinality (number of set bits `1` in the binary representation).
  2. Secondary key (tie-breaker): The numerical value of the integer in ascending order.
  Return the sorted array.

  PATTERN:
  Multi-Key Custom Comparator Sorting

  INTUITION & KEY OBSERVATIONS:
  - Use `__builtin_popcount(x)` to compute binary cardinality in O(1).
  - Comparator: if popcount(a) != popcount(b), return popcount(a) < popcount(b).
    Else return a < b.

  COMPLEXITY:
  - Time: O(N log N)
  - Space: O(log N) auxiliary space for sorting
*/
class Solution33 {
public:
    vector<int> cardinalitySort(vector<int> nums) {
        sort(nums.begin(), nums.end(), [](int a, int b) {
            int ca = __builtin_popcount(a);
            int cb = __builtin_popcount(b);
            if (ca != cb) return ca < cb;
            return a < b;
        });
        return nums;
    }
};

// ====================================================================================================
// 34. BALL COLLISION AROUND PIVOT POINT
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array `a` of ball speeds on a track, and a 1-based index `x` of a specific ball.
  All balls move in the positive direction.
  * A ball behind x (index i < x - 1) will collide with ball x if it moves faster (a[i] > a[x - 1]).
  * A ball in front of x (index i > x - 1) will collide with ball x if it moves slower (a[i] < a[x - 1]).
  * Balls with speed equal to ball x will never collide.
  Find the total number of collisions involving ball `x`.

  PATTERN:
  Single-Pass Bidirectional Threshold Comparison

  INTUITION & KEY OBSERVATIONS:
  - Let p = x - 1 (0-based index of pivot ball).
  - Scan indices i from 0 to p - 1: increment count if a[i] > a[p].
  - Scan indices i from p + 1 to n - 1: increment count if a[i] < a[p].

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution34 {
public:
    long long countCollisions(const vector<long long>& a, int x) {
        int n = a.size(), p = x - 1;
        long long ans = 0;
        for (int i = 0; i < p; ++i) if (a[i] > a[p]) ans++;
        for (int i = p + 1; i < n; ++i) if (a[i] < a[p]) ans++;
        return ans;
    }
};

// ====================================================================================================
// 35. MAXIMUM WEIGHTED DIFFERENCE (1-BASED INDEX)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given two arrays `arr1` and `arr2` of length `n`.
  You can rearrange both arrays independently in any order.
  Find the maximum value of:
      Σ_{i=1}^{n} i * (arr2[i] - arr1[i])

  PATTERN:
  Sorting + Rearrangement Inequality

  INTUITION & KEY OBSERVATIONS:
  - Weight (i) strictly increases: 1, 2, 3, ..., n.
  - To maximize Σ i * arr2[i], assign largest arr2 values to largest weights: sort arr2 ASCENDING.
  - To minimize Σ i * arr1[i] (which is subtracted), assign largest arr1 values to smallest weights:
    sort arr1 DESCENDING.

  COMPLEXITY:
  - Time: O(N log N)
  - Space: O(1) auxiliary space
*/
class Solution35 {
public:
    long long maxWeightedDifference(vector<long long> arr1, vector<long long> arr2) {
        int n = arr1.size();
        sort(arr1.rbegin(), arr1.rend());
        sort(arr2.begin(), arr2.end());
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += 1LL * (i + 1) * (arr2[i] - arr1[i]);
        }
        return ans;
    }
};

// ====================================================================================================
// 36. DELETE ODD-VALUED NODES FROM A LINKED LIST
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given the `head` of a singly linked list, delete every node whose value is odd (`val % 2 != 0`).
  Return the head of the modified linked list.

  PATTERN:
  Linked List Two-Pointer Traversal / Sentinel Processing

  INTUITION & KEY OBSERVATIONS:
  - Advance head while head is non-null and head->val % 2 != 0 to remove leading odd nodes.
  - Iterate through list with pointer cur:
    * If cur->next->val % 2 != 0: bypass node by cur->next = cur->next->next.
    * Else: advance cur = cur->next.

  COMPLEXITY:
  - Time: O(N)
  - Space: O(1)
*/
class Solution36 {
public:
    ListNode* deleteOddNodes(ListNode* head) {
        while (head && head->val % 2 != 0) head = head->next;
        ListNode* cur = head;
        while (cur && cur->next) {
            if (cur->next->val % 2 != 0) cur->next = cur->next->next;
            else cur = cur->next;
        }
        return head;
    }
};

// ====================================================================================================
// 37. SHORTEST SUBSTRING DELETION FOR DISTINCT CHARACTERS
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s`, find the minimum length of a contiguous substring that must be deleted
  so that all remaining characters in the string are unique/distinct.
  (Deleting 0 characters is allowed if the string already has all distinct characters).

  PATTERN:
  Prefix & Suffix Distinct Sets / Two Pointers

  INTUITION & KEY OBSERVATIONS:
  - The characters kept must form a prefix s[0...l] and a suffix s[r...n-1] (possibly empty) such that:
    1. Prefix s[0...l] has all distinct characters.
    2. Suffix s[r...n-1] has all distinct characters.
    3. Prefix and suffix share NO common characters.
  - The length of the deleted substring between them is r - l - 1.
  - Find the longest suffix with all distinct characters starting at index r.
  - Scan prefix from l = 0 onward. While extending prefix, remove conflicting suffix characters.
  - Track minimum deletion length min(ans, r - l - 1).

  COMPLEXITY:
  - Time: O(N)
  - Space: O(Σ) where Σ is alphabet size
*/
class Solution37 {
public:
    int findShortestSubstring(const string& s) {
        int n = s.size(), r = n, ans = n;
        unordered_set<char> suf, pre;
        while (r > 0 && !suf.count(s[r - 1])) {
            suf.insert(s[--r]);
        }
        ans = r; // delete prefix s[0...r-1]
        for (int l = 0; l < n; ++l) {
            if (pre.count(s[l])) break;
            pre.insert(s[l]);
            while (r < n && suf.count(s[l])) {
                suf.erase(s[r]);
                r++;
            }
            ans = min(ans, r - l - 1);
        }
        return ans;
    }
};

// ====================================================================================================
// 38. MESSAGE DUPLICATE DETECTION RATE LIMITER
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array `timestamps` and an array of `messages` of length `n`, along with an integer `k`.
  A message is delivered successfully (returns `true`) unless the exact SAME message was already
  delivered within the last `k` seconds (`currentTime - lastDeliveredTime <= k`).
  If a message is dropped (returns `false`), it does NOT update the delivery time.
  Return a boolean array of delivery outcomes for each message.

  PATTERN:
  Hash Map Message Timestamp Cache

  INTUITION & KEY OBSERVATIONS:
  - Map each message string to its last successful delivery timestamp.
  - For message msg at timestamps[i]:
    * If last.count(msg) && timestamps[i] - last[msg] <= k:
      Duplicate! Record false, do NOT update last[msg].
    * Else:
      Success! Record true, update last[msg] = timestamps[i].

  COMPLEXITY:
  - Time: O(N) average
  - Space: O(Unique Messages)
*/
class Solution38 {
public:
    vector<bool> messageDelivery(const vector<int>& timestamps, const vector<string>& messages, int k) {
        unordered_map<string, int> last;
        vector<bool> ans;
        for (size_t i = 0; i < messages.size(); ++i) {
            const string& msg = messages[i];
            if (last.count(msg) && timestamps[i] - last[msg] <= k) {
                ans.push_back(false);
            } else {
                ans.push_back(true);
                last[msg] = timestamps[i];
            }
        }
        return ans;
    }
};
