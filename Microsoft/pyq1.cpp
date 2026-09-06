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
                             MICROSOFT OA DSA PROBLEM SHEET (PYQ 1)
                                 SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern to Remember               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 01 | Equal Substrings Within Budget (LC 1208)    | Dynamic Sliding Window with Cost  | O(N)     | O(1)     |
 | 02 | Count String Pairs Forming a Palindrome     | 26-Bit Parity Mask + Hash Map     | O(L+26N) | O(N)     |
 | 03 | Circular Farm With Opposite Sectors         | Opposite Pair Sum + Sliding Window| O(N)     | O(N)     |
 | 04 | Max Distinct Elements Using -1, 0, or +1    | Sort + Greedy Smallest Available  | O(N logN)| O(1)     |
 | 05 | Maximize Weighted Difference by Reordering  | Rearrangement Inequality          | O(N logN)| O(1)     |
 | 06 | Sliding Window Rate Limiter                 | Per-Key Timestamp Deque           | O(N)     | O(U * K) |
 | 07 | Component Strength After Every Edge Addition| DSU with Component Max Tracking   | O((N+Q)α)| O(N)     |
 | 08 | Count Triplets With Sum Divisible by D      | Remainder Pair Tracking DP        | O(N × D) | O(D)     |
 | 09 | Mountain Sequence Sum                       | AP Closed-Form Peak Formula       | O(1)     | O(1)     |
 | 10 | Smallest String After Replacing '?'         | Substring Trial + Greedy 'a' Fill | O(N²)    | O(N)     |
 | 11 | Number of Strictly Increasing Subarrays     | Monotonic Streak Counter          | O(N)     | O(1)     |
 | 12 | Count Palindromic Substrings (LC 647)       | Expand Around Center              | O(N²)    | O(1)     |
 | 13 | Earliest K-Minute Free Meeting Slot         | Interval Merge + Gap Search       | O(M logM)| O(M)     |
 | 14 | Price Query Conversion Cost (LC 2602)       | Prefix Sums + Binary Search       | O((N+Q)lg| O(N)     |
 | 15 | Minimum Stress Path in a Graph (LC 1631)    | Minimax Dijkstra (Priority Queue)  | O((V+E)lg| O(V)     |
 | 16 | Good Array Using Powers of Two (LC 2438)    | Bit Extraction + Prefix Query     | O(log N) | O(log N) |
 | 17 | Minimum Cost to Merge All Numbers (LC 1167) | Huffman Min-Heap Pair Merge       | O(N logN)| O(N)     |
 | 18 | Minimum Starting Resources for Manufacturing| Difference Comparator Greedy      | O(N logN)| O(N)     |
 | 19 | Smallest Concatenation of 3 Strings         | Exhaustive Permutation Search     | O(L)     | O(L)     |
 | 20 | Rotate Rows to Create an All-1 Column       | Circular Distance Optimization    | O(N × M²)| O(1)     |
 | 21 | Sentence Validation                         | Tokenizer & Character Constraints | O(N)     | O(N)     |
 | 22 | 0/1 Knapsack                                | 1D Space-Optimized DP (Reverse)   | O(N × W) | O(W)     |
 | 23 | Prime Factor Multiples Flip                 | Sieve Toggling on Multiples       | O(M√X+Nlg| O(N)     |
 | 24 | Binary String Push + Reverse Construction   | Alternating Deque Simulation      | O(N)     | O(N)     |
 | 25 | Greedy Resource Allocation (LC 455)         | Sort + Dual Pointer Matching      | O(N logN)| O(1)     |
 | 26 | Product Constraint — Count Triplets         | Disjoint Adjacency + Hash Map     | O(N)     | O(N)     |
 | 27 | Roll String                                 | Difference Array (Prefix Shifts)  | O(N + R) | O(N)     |
 | 28 | Longest Subsequence (Subseq X, Substr Y)    | 1D Dynamic Programming            | O(|x|·|y|)| O(|y|)   |
 | 29 | Closest Numbers                             | Sorting + Adjacent Scan           | O(N logN)| O(1)     |
 | 30 | Get Minimum Cost (k-Capable ML Models)      | Capability Units + 2-Way Merge    | O(N logN)| O(N)     |
 | 31 | Get Visible Profiles Count                  | DSU with Component Size Tracking  | O((E+Q)α)| O(N)     |
 ====================================================================================================
*/

// ====================================================================================================
// 1. EQUAL SUBSTRINGS WITHIN BUDGET [LC 1208]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given equal-length strings `s` and `t` and an integer `maxCost`, find the maximum length
  of a substring of `s` that can be transformed into the corresponding substring of `t`
  such that total transformation cost sum(abs(s[i] - t[i])) <= maxCost.

  PATTERN TO REMEMBER:
  Dynamic Sliding Window with Cost Budget

  CORE INTUITION & STEPS:
  - Expand window right pointer `r`, adding conversion cost `abs(s[r] - t[r])`.
  - If cumulative cost exceeds `maxCost`, shrink window from left `l` until within budget.
  - Track max window length `r - l + 1`.

  COMPLEXITY:
  - Time:  O(N) single pass
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Maintain a running transformation cost; whenever budget is breached, increment left pointer.
  The maximum valid window size is the answer.
*/
class Solution1 {
public:
    int equalSubstring(const string& s, const string& t, int maxCost) {
        int l = 0, cost = 0, maxLen = 0;
        for (int r = 0; r < (int)s.size(); ++r) {
            cost += abs(s[r] - t[r]);
            while (cost > maxCost) {
                cost -= abs(s[l] - t[l]);
                l++;
            }
            maxLen = max(maxLen, r - l + 1);
        }
        return maxLen;
    }
};

// ====================================================================================================
// 2. COUNT STRING PAIRS THAT CAN FORM A PALINDROME [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array of lowercase strings `words`, count unordered pairs (i, j) with i < j
  such that characters in `words[i] + words[j]` can be rearranged into a valid palindrome.

  PATTERN TO REMEMBER:
  26-Bit Parity Mask + Frequency HashMap

  CORE INTUITION & STEPS:
  - A rearranged string is a palindrome iff at most one character has an odd frequency.
  - Encode each word into a 26-bit bitmask where bit `k` is toggled for character (`a` + k).
  - Two words form a palindrome iff their XOR sum is 0 (identical parities) or a power of 2 (differs by 1 bit).
  - Use a hash map to accumulate matches in O(26) per word.

  COMPLEXITY:
  - Time:  O(Total Characters + 26 * N)
  - Space: O(N) auxiliary space

  CONCLUSION / TAKEAWAY:
  Palindrome anagram check always reduces to XOR parity bitmasks. Count pairs matching `mask`
  or `mask ^ (1 << bit)`.
*/
class Solution2 {
public:
    long long countPalindromePairs(const vector<string>& words) {
        unordered_map<int, int> maskFreq;
        long long pairs = 0;
        for (const string& s : words) {
            int mask = 0;
            for (char c : s) mask ^= (1 << (c - 'a'));
            pairs += maskFreq[mask];
            for (int bit = 0; bit < 26; ++bit) {
                pairs += maskFreq[mask ^ (1 << bit)];
            }
            maskFreq[mask]++;
        }
        return pairs;
    }
};

// ====================================================================================================
// 3. CIRCULAR FARM WITH OPPOSITE SECTORS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  A circular farm has `N` sectors (even `N`), each yielding `profit[i]`. Choosing sector `i`
  automatically harvests opposite sector `(i + N/2) % N`. Find the maximum profit from
  harvesting `K` consecutive sectors along the circumference.

  PATTERN TO REMEMBER:
  Opposite Pair Sum + Circular Sliding Window

  CORE INTUITION & STEPS:
  - Each selected sector i automatically pairs with opposite sector (i + N/2) % N.
  - Define effective array: val[i] = profit[i] + profit[(i + N/2) % N].
  - Find the maximum sum of K consecutive elements in circular array `val`.

  COMPLEXITY:
  - Time:  O(N) single pass
  - Space: O(N) auxiliary array for combined values

  CONCLUSION / TAKEAWAY:
  Precompute paired opposite values into a single array; problem reduces to standard circular
  sliding window of fixed size K.
*/
class Solution3 {
public:
    long long maxProfit(const vector<int>& profit, int K) {
        int n = profit.size(), half = n / 2;
        vector<long long> val(n);
        for (int i = 0; i < n; ++i) {
            val[i] = (long long)profit[i] + profit[(i + half) % n];
        }
        long long curSum = 0;
        for (int i = 0; i < K; ++i) curSum += val[i]; // Initial window sum
        long long maxSum = curSum;
        for (int i = 1; i < n; ++i) {
            curSum += val[(i + K - 1) % n] - val[i - 1]; // Slide window forward
            maxSum = max(maxSum, curSum);
        }
        return maxSum;
    }

    int maxCircularSubarraySum(vector<int> &arr, int k) {
        int n = arr.size();

        // Calculate sum of first window
        int windowSum = 0;
        for (int i = 0; i < k; i++) {
            windowSum += arr[i % n];
        }

        int maxSum = windowSum;

        // Slide through remaining circular windows
        for (int i = 1; i < n; i++) {
            windowSum -= arr[(i - 1) % n]; // (i-1)%n is the element leaving the window
            windowSum += arr[(i + k - 1) % n]; // (i+k-1)%n is the new element entering the window

            maxSum = max(maxSum, windowSum);
        }

        return maxSum;
    }
};

// ====================================================================================================
// 4. MAXIMUM DISTINCT ELEMENTS USING -1, 0, OR +1 [LC 3397]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an integer array `nums`, you can adjust each element by adding an offset in {-1, 0, +1}.
  Find the maximum possible count of distinct values achievable in the modified array.

  PATTERN TO REMEMBER:
  Sorting + Greedy Smallest Available Assignment

  CORE INTUITION & STEPS:
  - Sort array ascending.
  - To leave maximal room for subsequent numbers, always assign each `x` to the smallest
    available value >= `prev + 1`.
  - Candidate target is `max(x - 1, prev + 1)`. If candidate <= `x + 1`, accept candidate and update `prev`.

  COMPLEXITY:
  - Time:  O(N log N) dominated by sorting
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Sort ascending and assign each element to the earliest unpicked integer in its valid interval
  `[x - 1, x + 1]` to greedily maximize distinct counts.
*/
class Solution4 {
public:
    int maxDistinctElements(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        long long prev = -2e18; // Initialize to a value smaller than any possible candidate
        int distinctCount = 0;
        for (int x : nums) {
            long long cand = max((long long)x - 1, prev + 1); // can be max of x-1 or prev+1
            if (cand <= (long long)x + 1) { // check if candidate is within the valid range
                distinctCount++;
                prev = cand;
            }
        }
        return distinctCount;
    }
};

// ====================================================================================================
// 5. MAXIMIZE WEIGHTED DIFFERENCE BY REORDERING [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given two integer arrays `A1` and `A2` of length `N`, rearrange both arrays independently
  to maximize the weighted difference sum: Σ (i * (A2[i] - A1[i])) for 0 <= i < N.

  PATTERN TO REMEMBER:
  Rearrangement Inequality (Sorted Pair Matching)

  CORE INTUITION & STEPS:
  - By Rearrangement Inequality:
    * Σ i * A2[i] is maximized when A2 is sorted ascending (matching larger weights `i`).
    * Σ i * A1[i] is minimized when A1 is sorted descending (pairing larger weights with smaller values).
  - Sort A2 ascending, A1 descending, and compute weighted sum.

  COMPLEXITY:
  - Time:  O(N log N) for sorting
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  To maximize Σ i*(A2[i] - A1[i]), align A2 ascending with weight indices and reverse-align
  A1 descending to minimize deductions.
*/
class Solution5 {
public:
    long long maxWeightedDifference(vector<int>& A1, vector<int>& A2) {
        sort(A2.begin(), A2.end());
        sort(A1.rbegin(), A1.rend());
        long long total = 0;
        for (int i = 0; i < (int)A1.size(); ++i) {
            total += 1LL * i * (A2[i] - A1[i]);
        }
        return total;
    }
};

// ====================================================================================================
// 6. SLIDING WINDOW RATE LIMITER [MS OA / LC 359]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Design a rate limiter for incoming requests (userId, timestamp). A request is accepted (1)
  if strictly fewer than `K` successful requests were accepted within time window [timestamp - T, timestamp];
  otherwise rejected (0). Only accepted requests are recorded.

  PATTERN TO REMEMBER:
  Per-Key Timestamp Deque / Sliding Window Queue

  CORE INTUITION & STEPS:
  - Map each `userId` to a `queue<int>` containing accepted timestamps.
  - Evict timestamps <= timestamp - T.
  - If remaining queue size < K: accept request, push timestamp, return 1. Else return 0.

  COMPLEXITY:
  - Time:  O(1) amortized per request
  - Space: O(U * K) where U is unique users

  CONCLUSION / TAKEAWAY:
  Maintain a per-user queue of valid timestamps; evict expired entries outside `[t - T, t]`
  before capacity checking.
*/
class Solution6 {
public:
    vector<int> rateLimiter(const vector<string>& users, const vector<int>& times, int T, int K) {
        unordered_map<string, queue<int>> userHistory; // userId -> queue of accepted timestamps
        vector<int> results;
        results.reserve(users.size());
        for (size_t i = 0; i < users.size(); ++i) {
            queue<int>& q = userHistory[users[i]];
            while (!q.empty() && q.front() < times[i] - T) {
                q.pop();
            } // Evict timestamps outside the sliding window
            if (q.size() < K) { // If fewer than K accepted requests in window, accept this request
                q.push(times[i]);
                results.push_back(1);
            } else { // Otherwise, reject the request
                results.push_back(0);
            }
        }
        return results;
    }
};

// ====================================================================================================
// 7. COMPONENT STRENGTH AFTER EVERY EDGE ADDITION [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  An undirected graph on `N` vertices (labeled 1..N) starts with no edges. Component strength
  is the maximum vertex label in that component. As `Q` edges are added one by one, return
  the sum of strengths across all components after each addition.

  PATTERN TO REMEMBER:
  Disjoint Set Union (DSU) with Component Maximum Tracking

  CORE INTUITION & STEPS:
  - Initial sum = 1 + 2 + ... + N = N * (N + 1) / 2.
  - DSU tracks root parent `p` and maximum label `mx[root]`.
  - When merging distinct roots `a` and `b`:
    * subtract old strengths: total -= (mx[a] + mx[b])
    * update parent p[b] = a, mx[a] = max(mx[a], mx[b])
    * add new combined strength: total += mx[a]

  COMPLEXITY:
  - Time:  O((N + Q) * α(N))
  - Space: O(N) for DSU structures

  CONCLUSION / TAKEAWAY:
  Store component maximums at DSU roots; incrementally maintain global strength sum in O(1)
  during each successful union.
*/
class DSU {
public:
    vector<int> p, mx;
    DSU(int n) : p(n + 1), mx(n + 1) {
        iota(p.begin(), p.end(), 0);
        iota(mx.begin(), mx.end(), 0);
    }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        a = find(a), b = find(b);
        if (a != b) p[b] = a, mx[a] = max(mx[a], mx[b]);
    }
};

class Solution7 {
public:
    vector<long long> getStrength(int n, const vector<int>& from, const vector<int>& to) {
        DSU d(n); long long sum = 1LL*n*(n+1)/2; vector<long long> ans;
        for (int i = 0; i < from.size(); i++) {
            int a = d.find(from[i]), b = d.find(to[i]);
            if (a != b) sum -= d.mx[a] + d.mx[b], d.unite(a,b), sum += d.mx[d.find(a)];
            ans.push_back(sum);
        }
        return ans;
    }
};

// ====================================================================================================
// 8. COUNT TRIPLETS WITH SUM DIVISIBLE BY D [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given integer array `nums` and divisor `D`, count index triplets (i, j, k) with i < j < k
  such that (nums[i] + nums[j] + nums[k]) % D == 0.

  PATTERN TO REMEMBER:
  Modulo Remainder Counting + Dynamic Pair Tracking

  CORE INTUITION & STEPS:
  - Decompose triplet condition: (num[i] + num[j] + num[k]) % D == 0.
  - Maintain `cnt[r]` (single remainder frequency) and `pairCnt[r]` (two-element remainder sum frequency).
  - For each element with remainder `r`:
    * Add `pairCnt[(D - r) % D]` to answer.
    * For each remainder `rem`, update `pairCnt[(rem + r) % D] += cnt[rem]`.
    * Increment `cnt[r]`.

  COMPLEXITY:
  - Time:  O(N * D)
  - Space: O(D) auxiliary space

  CONCLUSION / TAKEAWAY:
  Progressively track counts of single remainders and pair remainders; match the third element
  against complementary pair remainders in O(D) per step.
*/
class Solution8 {
public:
    long long countTriplets(const vector<int>& nums, int d) {
        vector<long long> cnt(d, 0), pairCnt(d, 0);
        long long triplets = 0;
        for (int x : nums) {
            int r = ((x % d) + d) % d; // we wanted (A+B+C)%D==0, so we need (A+B)%D == -C%D == (D-C%D)%D
            // x%d + d is due to negative numbers, we want to ensure r is in [0, d-1]
            triplets += pairCnt[(d - r) % d];
            for (int rem = 0; rem < d; ++rem) {
                pairCnt[(rem + r) % d] += cnt[rem]; // Update pair counts for new remainder sums
            }
            cnt[r]++; // Update single remainder count
        }
        return triplets;
    }
};

// ====================================================================================================
// 9. MOUNTAIN SEQUENCE SUM [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  A sequence increases by 1 from `i` up to peak `j`, then decreases by 1 down to `k`.
  Sequence: [i, i+1, ..., j-1, j, j-1, ..., k+1, k]. Calculate the total sum of all elements.

  PATTERN TO REMEMBER:
  Arithmetic Progression Closed-Form Formula

  CORE INTUITION & STEPS:
  - S1 = sum of AP from i to j: (i + j) * (j - i + 1) / 2.
  - S2 = sum of AP from k to j - 1: (k + j - 1) * (j - k) / 2.
  - Total sum = S1 + S2 in O(1) using 64-bit arithmetic.

  COMPLEXITY:
  - Time:  O(1)
  - Space: O(1)

  CONCLUSION / TAKEAWAY:
  Split the mountain at its peak into two distinct arithmetic progressions; evaluate both in O(1)
  with standard AP summation.
*/
class Solution9 {
    long long apSum(long long l, long long r) {
        return l > r ? 0LL : (l + r) * (r - l + 1) / 2;
    }
public:
    long long sequenceSum(int i, int j, int k) {
        return apSum(i, j) + apSum(k, j - 1);
    }
};

// ====================================================================================================
// 10. LEXICOGRAPHICALLY SMALLEST STRING AFTER REPLACING ? [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given string `word` containing letters and '?' and target `sub`, replace every '?' with a lowercase
  letter such that `sub` appears as a contiguous substring in `word` and the result is lexicographically
  minimal. Return "-1" if impossible.

  PATTERN TO REMEMBER:
  Brute-Force Substring Placement + Greedy 'a' Replacement

  CORE INTUITION & STEPS:
  - Iterate every valid start position `i` for `sub` in `word`.
  - Check if `sub` can be embedded (characters match or equal '?').
  - If viable, fill all other remaining '?' with 'a' to form the smallest candidate string.
  - Take the minimum string across all valid placements.

  COMPLEXITY:
  - Time:  O((N - M + 1) * N)
  - Space: O(N) for candidate string copies

  CONCLUSION / TAKEAWAY:
  Try all valid window positions for the mandatory target substring; greedily fill all remaining
  wildcards with 'a' and return the lexicographical minimum.
*/
class Solution10 {
public:
    string smallestString(const string& word, const string& sub) {
        int n = word.size(), m = sub.size();
        string best = "";
        for (int i = 0; i + m <= n; ++i) {
            string cur = word;
            bool match = true;
            int j=0;
            while(j < m) {
                if (cur[i + j] != '?' && cur[i + j] != sub[j]) {
                    match = false;
                    break;
                }
                cur[i + j] = sub[j];
                j++;
            }
            if (!match) continue;
            for (char& c : cur) if(c=='?') c = 'a';
            cur = min(cur, best.empty() ? cur : best);
        }
        return best.empty() ? "-1" : best;
    }
};

// ====================================================================================================
// 11. NUMBER OF STRICTLY INCREASING SUBARRAYS OF LENGTH K [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an integer array `nums` and integer `K`, count contiguous subarrays of size exactly `K`
  that are strictly increasing (nums[i] < nums[i+1] < ...).

  PATTERN TO REMEMBER:
  Running Monotonic Streak Counter / Linear Scan

  CORE INTUITION & STEPS:
  - Maintain running streak of strictly increasing consecutive elements.
  - If nums[i] > nums[i - 1], streak++; else streak = 1.
  - Whenever streak >= K, increment answer count.

  COMPLEXITY:
  - Time:  O(N) single pass
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Maintain a streak of strictly increasing numbers; each position with streak >= K contributes
  exactly one valid subarray of length K.
*/
class Solution11 {
public:
    int countIncreasing(const vector<int>& nums, int k) {
        if (nums.empty() || k <= 0 || k > (int)nums.size()) return 0;
        int streak = 1, ans = (k == 1 ? 1 : 0);
        for (int i = 1; i < nums.size(); ++i) {
            streak = (nums[i] > nums[i - 1]) ? streak + 1 : 1;
            if (streak >= k) ans++;
        }
        return ans;
    }
};

// ====================================================================================================
// 12. COUNT PALINDROMIC SUBSTRINGS [LC 647]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s`, return the total number of palindromic substrings in it.

  PATTERN TO REMEMBER:
  Expand Around Center (Odd and Even Centers)

  CORE INTUITION & STEPS:
  - A string of length N has 2N - 1 centers (N single characters, N - 1 pairs).
  - Expand outward from each center while left and right characters match.
  - Increment count on each matched expansion.

  COMPLEXITY:
  - Time:  O(N²)
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  Every palindrome expands symmetrically around its center; iterate all 2N - 1 center configurations
  in O(1) space.
*/
class Solution12 {
public:
    int expandAroundCenter(const string& s, int left, int right) {
        int count = 0;
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
        return count;
    }
    int countSubstrings(const string& s) {
        int n = s.size(), total = 0;
        for(int i=0;i<n;i++){
            total += expandAroundCenter(s, i, i);     // Odd length palindromes
            total += expandAroundCenter(s, i, i + 1); // Even length palindromes
        }
        return total;
    }
};

// ====================================================================================================
// 13. FIND EARLIEST K-MINUTE SLOT WHEN EVERYONE IS FREE [LC 1229 / MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given busy intervals `[start, end]` of multiple participants and meeting length `K`, find
  the earliest start time `T` such that everyone is free during `[T, T + K]`.

  PATTERN TO REMEMBER:
  Interval Merging + Complementary Free Gap Search

  CORE INTUITION & STEPS:
  - Sort and merge all busy intervals.
  - Inspect gaps between consecutive merged busy intervals starting from time 0.
  - The first gap with length >= K yields the earliest free start time.

  COMPLEXITY:
  - Time:  O(M log M) where M is total interval count
  - Space: O(M) for merged intervals

  CONCLUSION / TAKEAWAY:
  Union all busy schedules via standard interval merge; scan the gaps between busy blocks
  to locate the first interval with span >= K.
*/
class Solution13 {
public:
    int earliestFreeSlot(vector<pair<int,int>>& intervals, int k) {
        sort(intervals.begin(), intervals.end());
        
        int freeStart = 0;
        for (const auto& [s, e] : intervals) {
            if (s - freeStart >= k) return freeStart;
            freeStart = max(freeStart, e);
        }
        return freeStart;
    }
};

// ====================================================================================================
// 14. PRICE QUERY CONVERSION COST [LC 2602]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given array `price` and query array `queries`, calculate for each query `q` the minimum
  total cost to convert every price to `q`, where cost = sum(abs(price[i] - q)).

  PATTERN TO REMEMBER:
  Sorting + Prefix Sums + Binary Search (std::lower_bound)

  CORE INTUITION & STEPS:
  - Sort `price` and precompute prefix sums.
  - For each query target `q`, find split index `idx` using lower_bound:
    * Left elements (< q): cost = idx * q - prefix[idx].
    * Right elements (>= q): cost = (prefix[n] - prefix[idx]) - (n - idx) * q.
  - Total cost = Left cost + Right cost in O(log N) per query.

  COMPLEXITY:
  - Time:  O(N log N + Q log N)
  - Space: O(N) for prefix sums

  CONCLUSION / TAKEAWAY:
  Partition the absolute deviation around target `q` using binary search; answer queries in O(log N)
  via prefix sum range formulas.
*/
class Solution14 {
public:
    vector<long long> solve(vector<int>& price, const vector<int>& queries) {
        sort(price.begin(), price.end());
        int n = price.size();
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + price[i];

        vector<long long> results;
        results.reserve(queries.size());
        for (int q : queries) {
            int idx = lower_bound(price.begin(), price.end(), q) - price.begin();
            long long leftCost = 1LL * q * idx - pref[idx];
            long long rightCost = (pref[n] - pref[idx]) - 1LL * q * (n - idx);
            results.push_back(leftCost + rightCost);
        }
        return results;
    }
};

// ====================================================================================================
// 15. MINIMUM STRESS PATH IN A GRAPH [LC 1631 / MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an undirected graph where edge weights represent stress, the stress of a path is
  the MAXIMUM edge weight along it. Find the path from `src` to `dst` with minimal bottleneck stress.

  PATTERN TO REMEMBER:
  Minimax Dijkstra (Modified Shortest Path via Priority Queue)

  CORE INTUITION & STEPS:
  - Let dist[u] be the minimum possible bottleneck stress to reach vertex u from src.
  - Relax edges with: nextStress = max(curStress, edgeWeight).
  - Use min-heap priority queue to greedily extract the lowest stress node.

  COMPLEXITY:
  - Time:  O((V + E) log V)
  - Space: O(V + E) for adjacency graph and distance array

  CONCLUSION / TAKEAWAY:
  Bottleneck path problems are isomorphic to Dijkstra; replace additive edge relaxation with
  `max(dist[u], weight)`.
*/
class Solution15 {
public:
    int minimumStress(int n, const vector<vector<pair<int, int>>>& graph, int src, int dst) {
        vector<int> dist(n, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [stress, u] = pq.top();
            pq.pop();
            if (u == dst) return stress;
            if (stress > dist[u]) continue;

            for (const auto& [v, w] : graph[u]) {
                int nextStress = max(stress, w); // in normal dijkstra, we do newWeight = dist[u] + w, here we do max
                if (nextStress < dist[v]) {
                    dist[v] = nextStress;
                    pq.push({nextStress, v});
                }
            }
        }
        return -1;
    }
};

// ====================================================================================================
// 16. GOOD ARRAY USING POWERS OF TWO [LC 2438]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  The "good array" of integer `n` consists of ascending powers of 2 matching set bits of `n`.
  For each query `[left, right]`, return (goodArray[left] * ... * goodArray[right]) % (10^9 + 7).

  PATTERN TO REMEMBER:
  Bit Extraction + Range Modular Multiplication

  CORE INTUITION & STEPS:
  - Extract powers 2^b for every bit `b` where (n & (1 << b)) != 0.
  - Evaluate product of powers[l..r] modulo (10^9 + 7).

  COMPLEXITY:
  - Time:  O(log N + Q * log N)
  - Space: O(log N) auxiliary space

  CONCLUSION / TAKEAWAY:
  Decompose `n` into unique set-bit binary powers; compute range product queries directly with modular arithmetic.
*/
class Solution16 {
public:
    vector<int> productQueries(int n, const vector<vector<int>>& queries) {
        const int MOD = 1e9 + 7;
        vector<int> powers;
        for (int b = 0; b < 31; ++b) {
            if (n & (1 << b)) powers.push_back(1 << b);
        }
        vector<int> ans;
        ans.reserve(queries.size());
        for (const auto& q : queries) {
            long long prod = 1;
            for (int i = q[0]; i <= q[1]; ++i) {
                prod = (prod * powers[i]) % MOD;
            }
            ans.push_back(prod);
        }
        return ans;
    }
};

// ====================================================================================================
// 17. MINIMUM COST TO MERGE ALL NUMBERS [LC 1167 / Standard Huffman]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Repeatedly pick two numbers `x` and `y`, merge them into `x + y` with cost `x + y`, until one
  number remains. Find the minimum total merge cost.

  PATTERN TO REMEMBER:
  Huffman Min-Heap Greedy Pair Merge

  CORE INTUITION & STEPS:
  - Merged numbers recur in future operations. To minimize overall sum, always merge the two
    smallest available elements first.
  - Insert all elements into a min-heap. Pop two smallest, add sum to total cost, push sum back.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(N) for priority queue

  CONCLUSION / TAKEAWAY:
  Huffman coding theorem guarantees that greedily pairing the two lowest elements at each step
  yields the minimal aggregate merge tree cost.
*/
class Solution17 {
public:
    long long minimumCost(const vector<int>& nums) {
        priority_queue<long long, vector<long long>, greater<>> pq(nums.begin(), nums.end());
        long long totalCost = 0;
        while (pq.size() > 1) {
            long long a = pq.top(); pq.pop();
            long long b = pq.top(); pq.pop();
            totalCost += a + b;
            pq.push(a + b);
        }
        return totalCost;
    }
};

// ====================================================================================================
// 18. MINIMUM STARTING RESOURCES FOR MANUFACTURING [LC 1665]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given tasks where task `i` requires `worstCase[i]` minimum resources to start and consumes
  `actual[i]` resources upon completion, find the minimum initial resources needed to finish all tasks.

  PATTERN TO REMEMBER:
  Difference Comparator Greedy (worstCase - actual)

  CORE INTUITION & STEPS:
  - Tasks with greater cushion `(worstCase[i] - actual[i])` leave more leftover resources for subsequent tasks.
  - Sort tasks descending by `(worstCase[i] - actual[i])`.
  - Simulate resource balance and top up required starting energy whenever balance drops below `worstCase`.

  COMPLEXITY:
  - Time:  O(N log N) dominated by sorting
  - Space: O(N) for index order array

  CONCLUSION / TAKEAWAY:
  Prioritize tasks offering the largest post-execution refund `(worstCase - actual)` first to minimize
  external capital injection.
*/
class Solution18 {
public:
    long long minimumStartingResources(vector<int>& worstCase, vector<int>& actual) {
        int n = worstCase.size();
        vector<pair<int,int>> tasks(n);
        for (int i = 0; i < n; ++i)
            tasks[i] = {worstCase[i], actual[i]};

        sort(tasks.begin(), tasks.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
            return (a.first - a.second) > (b.first - b.second);
        });

        long long cur = 0, req = 0;
        for (auto& [w, a] : tasks) {
            if (cur < w) {
                req += w - cur;
                cur = w;
            }
            cur -= a;
        }
        return req;
    }
};

// ====================================================================================================
// 19. LEXICOGRAPHICALLY SMALLEST CONCATENATION OF 3 STRINGS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given three strings `a`, `b`, and `c`, concatenate all three together using each exactly once
  to form the lexicographically smallest resulting string.

  PATTERN TO REMEMBER:
  Exhaustive Permutation Search (Fixed N = 3)

  CORE INTUITION & STEPS:
  - For 3 strings, exactly 3! = 6 permutations exist.
  - Iterate all 6 arrangements using `std::next_permutation` and record the minimum.

  COMPLEXITY:
  - Time:  O(L) where L is sum of string lengths
  - Space: O(L) for string concatenation

  CONCLUSION / TAKEAWAY:
  Constant small permutation spaces (N=3) should be evaluated exhaustively rather than through
  complex pairwise comparator heuristics.
*/
class Solution19 {
public:
    string smallestConcat(string a, string b, string c) {
        vector<string> v = {a, b, c};
        sort(v.begin(), v.end());
        string best = v[0] + v[1] + v[2];
        while (next_permutation(v.begin(), v.end())) {
            best = min(best, v[0] + v[1] + v[2]);
        }
        return best;
    }
};

// ====================================================================================================
// 20. ROTATE ROWS TO CREATE AN ALL-1 COLUMN [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given N x M binary matrix `grid`, each cyclical shift of any row left or right costs 1 operation.
  Find the minimum shifts needed to create at least one column consisting entirely of 1s, or -1 if impossible.

  PATTERN TO REMEMBER:
  Circular Distance Optimization per Column

  CORE INTUITION & STEPS:
  - If any row contains zero 1s, it is impossible to form an all-1 column (return -1).
  - For each target column `c`: in row `r`, circular distance to 1 at index `j` is min(|j - c|, M - |j - c|).
  - Accumulate minimal shifts across rows for each candidate column; take the global minimum.

  COMPLEXITY:
  - Time:  O(N * M²)
  - Space: O(1) auxiliary space

  CONCLUSION / TAKEAWAY:
  For circular row rotations, compute shortest wrap-around distance `min(d, M - d)` independently
  for each row toward candidate column `c`.
*/
class Solution20 {
public:
    int minOperations(const vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size(), minShifts = INT_MAX;
        for (int c = 0; c < m; ++c) {
            int totalShifts = 0;
            bool possible = true;
            for (int r = 0; r < n && possible; ++r) {
                int bestDist = INT_MAX;
                for (int j = 0; j < m; ++j) {
                    if (grid[r][j]) {
                        int diff = abs(j - c);
                        bestDist = min(bestDist, min(diff, m - diff));
                    }
                }
                if (bestDist == INT_MAX) possible = false;
                else totalShifts += bestDist;
            }
            if (possible) minShifts = min(minShifts, totalShifts);
        }
        return minShifts == INT_MAX ? -1 : minShifts;
    }
};

// ====================================================================================================
// 21. SENTENCE VALIDATION [LC 3136 Variant]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Validate whether a sentence contains only valid words separated by spaces. A word is valid iff:
  1. Length >= 3.
  2. Contains only alphanumeric characters.
  3. Contains at least one vowel.
  4. Contains at least one consonant.

  PATTERN TO REMEMBER:
  Tokenization & Character Classification

  CORE INTUITION & STEPS:
  - Tokenize words using `stringstream`.
  - Check each word: length >= 3, `isalnum` on all characters, track presence of vowel and consonant.
  - Return true iff every word satisfies all conditions and at least one word exists.

  COMPLEXITY:
  - Time:  O(N) where N is sentence length
  - Space: O(N) for stringstream tokens

  CONCLUSION / TAKEAWAY:
  Tokenize by whitespace and validate word invariant predicates (length, alphanumeric, vowel/consonant)
  in a clean single pass.
*/
class Solution21 {
    bool isValidWord(const string& w) {
        if (w.size() < 3) return false;
        bool hasVowel = false, hasConsonant = false;
        for (char ch : w) {
            if (!isalnum(ch)) return false;
            if (isalpha(ch)) {
                char lower = tolower(ch);
                if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
                    hasVowel = true;
                } else {
                    hasConsonant = true;
                }
            }
        }
        return hasVowel && hasConsonant;
    }
public:
    bool validate(const string& sentence) {
        stringstream ss(sentence);
        string word;
        bool hasWords = false;
        while (ss >> word) {
            hasWords = true;
            if (!isValidWord(word)) return false;
        }
        return hasWords;
    }
};

// ====================================================================================================
// 22. 0/1 KNAPSACK [Standard DP]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given `weights`, `values`, and knapsack capacity `W`, choose a subset of items each used at most
  once to maximize value without exceeding total weight `W`.

  PATTERN TO REMEMBER:
  1D Space-Optimized DP (Reverse Capacity Sweep)

  CORE INTUITION & STEPS:
  - dp[w] = max value achievable with capacity w.
  - For each item (weight, value), iterate capacity `w` backwards from W down to weight.
  - Reverse traversal guarantees each item is counted at most once: dp[w] = max(dp[w], dp[w - weight] + value).

  COMPLEXITY:
  - Time:  O(N * W)
  - Space: O(W) 1D space

  CONCLUSION / TAKEAWAY:
  Loop capacity backwards from W down to item weight to compress the 2D DP state into a single 1D array.
*/
class Solution22 {
public:
    int knapsack(const vector<int>& weight, const vector<int>& value, int capacity) {
        vector<int> dp(capacity + 1, 0);
        for (size_t i = 0; i < weight.size(); ++i) {
            for (int w = capacity; w >= weight[i]; --w) {
                dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
            }
        }
        return dp[capacity];
    }
};

// ====================================================================================================
// 23. PRIME FACTOR MULTIPLES FLIP [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given binary array `state` (1-based index) and numbers `nums`, for every number `x`, find all
  its distinct prime factors `p` and flip all indices in `state` that are multiples of `p`.
  Return the final array state.

  PATTERN TO REMEMBER:
  Prime Factorization + Sieve Toggling on Multiples

  CORE INTUITION & STEPS:
  - For each number `x`, extract distinct prime factors in O(sqrt(x)).
  - Accumulate net flips in `flip` array by toggling `p, 2p, 3p, ... <= N`.
  - Apply net parity flips to `state`.

  COMPLEXITY:
  - Time:  O(M * sqrt(X) + N log N)
  - Space: O(N) auxiliary flip array

  CONCLUSION / TAKEAWAY:
  Decompose queries into distinct prime factors, then sieve-flip harmonic multiples across the array
  to avoid redundant duplicate processing.
*/
class Solution23 {
public:
    vector<int> finalState(vector<int>& state, const vector<int>& nums) {
        int n = state.size();
        vector<int> flip(n + 1, 0);
        for (int x : nums) {
            for (int p = 2; 1LL * p * p <= x; ++p) {
                if (x % p == 0) {
                    for (int i = p; i <= n; i += p) flip[i] ^= 1;
                    while (x % p == 0) x /= p;
                }
            }
            if (x > 1) {
                for (int i = x; i <= n; i += x) flip[i] ^= 1;
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (flip[i]) state[i - 1] ^= 1;
        }
        return state;
    }
};

// ====================================================================================================
// 24. BINARY STRING PUSH + REVERSE CONSTRUCTION [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given count of `zeros` and `ones` for string `s` of length N = zeros + ones. Starting with empty `b`,
  for each character s[i], append s[i] then reverse `b`. Arrange `s` to make final `b` lexicographically
  maximal. Return optimal final string `b` and input sequence `s`.

  PATTERN TO REMEMBER:
  Alternating Deque Simulation + Greedy MSB Placement

  CORE INTUITION & STEPS:
  - Simulating the append-and-reverse operations tracks index landing order.
  - Characters placed at alternating deque boundaries: left, right, left, ...
  - To maximize lexicographical result, set `b` to have all '1's first, followed by '0's.
  - Reconstruct `s` by inverting the simulation mapping onto `b`.

  COMPLEXITY:
  - Time:  O(N) where N = zeros + ones
  - Space: O(N) auxiliary space

  CONCLUSION / TAKEAWAY:
  Append-and-reverse generates an alternating double-ended queue trajectory; place all '1's at the
  most significant positions of the output string.
*/
class Solution24 {
public:
    pair<string, string> getOptimalStrings(int zeros, int ones) {
        int n = zeros + ones;
        string b(ones, '1');
        b += string(zeros, '0');

        vector<int> order(n);
        int l = 0, r = n - 1;
        bool placeLeft = true;

        for (int i = n - 1; i >= 0; --i) {
            if (placeLeft) order[l++] = i;
            else order[r--] = i;
            placeLeft = !placeLeft;
        }

        string s(n, '0');
        for (int i = 0; i < n; ++i) {
            s[order[i]] = b[i];
        }
        return {b, s};
    }
};

// ====================================================================================================
// 25. GREEDY RESOURCE ALLOCATION [LC 455 Variant]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given task requirements and available resource capacities, each resource can fulfill at most
  one task with `resource >= requirement`. Maximize total tasks satisfied.

  PATTERN TO REMEMBER:
  Sorting + Dual Pointer Greedy Matching

  CORE INTUITION & STEPS:
  - Sort requirements and resources ascending.
  - With two pointers (i for requirement, j for resource):
    * If resource[j] >= requirement[i]: task satisfied, advance both i and j.
    * Else: resource too small, advance j to find larger resource.

  COMPLEXITY:
  - Time:  O(N log N + M log M)
  - Space: O(1) auxiliary space beyond sorting

  CONCLUSION / TAKEAWAY:
  Greedily assign the smallest capable resource to the smallest pending requirement to preserve
  larger resources for heavier tasks.
*/
class Solution25 {
public:
    int maximumAssignments(vector<int>& req, vector<int>& res) {
        sort(req.begin(), req.end());
        sort(res.begin(), res.end());
        int i = 0, j = 0;
        while (i < (int)req.size() && j < (int)res.size()) {
            if (res[j] >= req[i]) i++;
            j++;
        }
        return i;
    }
};

// ====================================================================================================
// 26. PRODUCT CONSTRAINT — COUNT VALID TRIPLETS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an integer array `A` and `target`, count the number of unordered triplets of distinct
  indices {i, j, k} such that A[i] * A[j] * A[k] == target and at least two indices are adjacent.

  PATTERN TO REMEMBER:
  Disjoint Adjacency Partitioning + Frequency Map Lookup

  CORE INTUITION & STEPS:
  - For sorted indices i < j < k, "at least two adjacent" partitions into two disjoint sets:
    1. First two are adjacent: j = i + 1, with k in [i + 2, n - 1].
    2. Last two are adjacent but first two are NOT: k = j + 1, with i in [0, j - 2].
  - Maintain frequency maps for candidate values:
    * In Case 1: Suffix frequency map over A[i+2..n-1].
    * In Case 2: Prefix frequency map over A[0..j-2].
  - Required third value is `target / (A[x] * A[y])` when product divides target.

  COMPLEXITY:
  - Time:  O(N) average
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Partition "at least two adjacent" into mutually exclusive cases (first pair adjacent vs last pair
  adjacent with gap) to prevent double counting while querying frequency maps in O(1).
*/

long long countTriplets(vector<int>& A, long long target) {
    int n = A.size();
    long long count = 0;

    // Case 1: (i, i+1) are adjacent
    unordered_map<long long, int> suffix;

    for (int k = 2; k < n; k++)
        suffix[A[k]]++;

    for (int i = 0; i < n - 2; i++) {
        long long product = 1LL * A[i] * A[i + 1];

        if (target % product == 0)
            count += suffix[target / product];

        suffix[A[i + 2]]--; // Remove A[i+2] from suffix as we move the window forward
    }

    // Case 2: (j, j+1) are adjacent
    // and i is NOT adjacent to j
    unordered_map<long long, int> prefix;

    for (int j = 1; j < n - 1; j++) {
        long long product = 1LL * A[j] * A[j + 1];

        if (target % product == 0)
            count += prefix[target / product];

        prefix[A[j - 1]]++; // Add A[j-1] to prefix as we move the window forward
    }

    return count;
}

// ====================================================================================================
// 27. ROLL STRING [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given lowercase string `s` and array `roll`, each operation advances the first `roll[i]` characters
  by one position cyclically ('a' -> 'b', ..., 'z' -> 'a'). Return the final string.

  PATTERN TO REMEMBER:
  Difference Array / Prefix Shift Accumulation

  CORE INTUITION & STEPS:
  - Each `roll[k]` adds +1 to all indices in [0, roll[k] - 1].
  - Use a difference array: `diff[0]++`, `diff[roll[k]]--`.
  - Prefix sum accumulation calculates net shift for each index i.
  - Apply net shift: `s[i] = 'a' + (s[i] - 'a' + totalShifts % 26) % 26`.

  COMPLEXITY:
  - Time:  O(N + roll.size())
  - Space: O(N) for difference array

  CONCLUSION / TAKEAWAY:
  Multiple prefix range increments are accumulated efficiently via a difference array in a single pass.
*/
class Solution27 {
public:
    string rollString(string s, const vector<int>& roll) {
        int n = s.size();
        vector<int> diff(n + 1, 0);
        for (int k : roll) {
            int len = min(k, n);
            diff[0]++;
            diff[len]--;
        }
        int add = 0;
        for (int i = 0; i < n; i++) {
            add = (add + diff[i]) % 26;
            s[i] = 'a' + (s[i] - 'a' + add) % 26;
        }
        return s;
    }
};

// ====================================================================================================
// 28. LONGEST SUBSEQUENCE (SUBSEQUENCE OF X, SUBSTRING OF Y) [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given strings `x` and `y`, find the maximum length of a string that is simultaneously a subsequence
  of `x` and a contiguous substring of `y`.

  PATTERN TO REMEMBER:
  Dynamic Programming (Subsequence vs Substring Matching)

  CORE INTUITION & STEPS:
  - Let `prev[j]` = max length of a substring ending at y[j-1] that is a subsequence of processed prefix of x.
  - When scanning char `a` from x:
    * Skipping `a` preserves `cur[j] = prev[j]`.
    * Matching `a == y[j-1]` allows extending the contiguous match: `cur[j] = max(cur[j], prev[j-1] + 1)`.
  - Track global maximum over all positions.

  COMPLEXITY:
  - Time:  O(|x| * |y|)
  - Space: O(|y|) 1D space optimized

  CONCLUSION / TAKEAWAY:
  Subsequence allows character skips in x; substring enforces contiguous increment in y:
  `dp[j] = prev[j-1] + 1` when characters match.
*/

class Solution28 {
  public:
    string X, Y;
    vector<vector<int>> memo;
    int ans = 0;

    // f(i, j) = length of the longest substring of Y ending exactly at Y[j-1]
    //           that is also a subsequence of X[0..i-1]
    int solve(int i, int j) {
        if (i == 0 || j == 0)
            return 0;
        if (memo[i][j] != -1)
            return memo[i][j];

        int res = solve(i - 1, j); // skip X[i-1], carry forward best so far

        if (X[i - 1] == Y[j - 1]) {
            res = max(res, solve(i - 1, j - 1) + 1); // extend contiguous match in Y
        }

        ans = max(ans, res);
        return memo[i][j] = res;
    }

    int longestSubsequence(const string &x, const string &y) {
        X = x;
        Y = y;
        int n = x.size(), m = y.size();
        memo.assign(n + 1, vector<int>(m + 1, -1));

        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= m; ++j)
                solve(i, j);

        return ans;
    }
};

class Solution299 {
public:
    int longestSubsequence(const string& x, const string& y) {
        int n = x.size(), m = y.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        int ans = 0;

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                dp[i][j] = dp[i - 1][j]; // skip x[i-1]

                if (x[i - 1] == y[j - 1]) {
                    dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] + 1); // extend contiguous match in y
                }

                ans = max(ans, dp[i][j]);
            }
        }

        return ans;
    }
};


// ====================================================================================================
// 29. CLOSEST NUMBERS [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array of distinct integers, find all pairs with the minimum absolute difference.
  Return pairs sorted by the first element, with smaller number first in each pair.

  PATTERN TO REMEMBER:
  Sorting + Adjacent Element Difference Scan

  CORE INTUITION & STEPS:
  - In a sorted array, the minimum difference between any two elements must occur between adjacent neighbors.
  - Sort the array in ascending order.
  - Pass 1: Find the minimum adjacent difference `mn`.
  - Pass 2: Collect all pairs `(numbers[i-1], numbers[i])` where difference equals `mn`.

  COMPLEXITY:
  - Time:  O(N log N) dominated by sorting
  - Space: O(1) auxiliary space beyond output

  CONCLUSION / TAKEAWAY:
  Sorting collapses 2D pairwise minimum difference search into a 1D adjacent neighbor scan.
*/
class Solution29 {
public:
    vector<pair<long long, long long>> closestNumbers(vector<long long> numbers) {
        sort(numbers.begin(), numbers.end());
        long long mn = LLONG_MAX;
        for (size_t i = 1; i < numbers.size(); i++) {
            mn = min(mn, numbers[i] - numbers[i - 1]);
        }
        vector<pair<long long, long long>> ans;
        for (size_t i = 1; i < numbers.size(); i++) {
            if (numbers[i] - numbers[i - 1] == mn) {
                ans.push_back({numbers[i - 1], numbers[i]});
            }
        }
        return ans;
    }
};

// ====================================================================================================
// 30. GET MINIMUM COST (K-CAPABLE ML MODELS) [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given `n` ML models with `cost[i]` and features in {"00", "01", "10", "11"}. A set is k-capable
  if >= k models support feature A and >= k support feature B. For each k from 1 to n, find the
  minimum total cost, or -1 if impossible.

  PATTERN TO REMEMBER:
  Greedy Capability Unit Pairing + Two-Way Merge

  CORE INTUITION & STEPS:
  - Discard "00" models.
  - Capability of both A and B by +1 can be obtained in two ways:
    1. Single "11" model.
    2. Paired combination of one "01" and one "10" model.
  - Sort "11", "01", and "10" models by cost.
  - Form paired costs `pairCost[i] = a[i] + b[i]` for i < min(|a|, |b|).
  - Merge sorted "11" and `pairCost` arrays into a single sorted list of capability units.
  - Compute prefix sums: the answer for k is the sum of the first k capability units.

  COMPLEXITY:
  - Time:  O(N log N)
  - Space: O(N)

  CONCLUSION / TAKEAWAY:
  Reduce multi-requirement coverage to atomic capability units ("11" vs "01"+"10"), then greedily
  pick cheapest units via prefix sum.
*/
class Solution30 {
public:
    vector<long long> getMinCost(int n, const vector<int>& cost, const vector<string>& featureAvailability) {
        vector<long long> both, a, b;
        for (int i = 0; i < n; i++) {
            if (featureAvailability[i] == "11") both.push_back(cost[i]);
            else if (featureAvailability[i] == "01") a.push_back(cost[i]);
            else if (featureAvailability[i] == "10") b.push_back(cost[i]);
        }
        sort(both.begin(), both.end());
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());

        vector<long long> pairCost;
        size_t limit = min(a.size(), b.size());
        for (size_t i = 0; i < limit; i++) {
            pairCost.push_back(a[i] + b[i]);
        }

        vector<long long> units;
        merge(both.begin(), both.end(), pairCost.begin(), pairCost.end(), back_inserter(units));

        vector<long long> ans(n, -1);
        long long sum = 0;
        for (size_t i = 0; i < units.size() && (int)i < n; i++) {
            sum += units[i];
            ans[i] = sum;
        }
        return ans;
    }
};

// ====================================================================================================
// 31. GET VISIBLE PROFILES COUNT [MS OA]
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an undirected graph of users with edges (u[i], v[i]), users connected directly or indirectly
  can view each other's profiles. For each user in `queries`, return the number of accessible profiles
  (including their own).

  PATTERN TO REMEMBER:
  Disjoint Set Union (DSU) with Component Sizes

  CORE INTUITION & STEPS:
  - Transitive access implies all users in the same connected component can view each other.
  - Build DSU over nodes 1..nodes tracking `sz[root]`.
  - For each edge (u, v), union components and sum their sizes: `sz[rootA] += sz[rootB]`.
  - For each query `x`, return `sz[find(x)]`.

  COMPLEXITY:
  - Time:  O((E + Q) * α(N))
  - Space: O(N) for DSU parent and size arrays

  CONCLUSION / TAKEAWAY:
  Social graph reachability is standard connected components; DSU answers component size queries in O(α(N)).
*/
class Solution31 {
public:
    vector<int> getVisibleProfilesCount(int nodes, const vector<int>& u, const vector<int>& v, const vector<int>& queries) {
        vector<int> parent(nodes + 1), sz(nodes + 1, 1);
        iota(parent.begin(), parent.end(), 0);

        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        for (size_t i = 0; i < u.size(); i++) {
            int a = find(u[i]), b = find(v[i]);
            if (a != b) {
                if (sz[a] < sz[b]) swap(a, b);
                parent[b] = a;
                sz[a] += sz[b];
            }
        }

        vector<int> ans;
        ans.reserve(queries.size());
        for (int x : queries) {
            ans.push_back(sz[find(x)]);
        }
        return ans;
    }
};
