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
                             MICROSOFT OA DSA PROBLEM SHEET (PYQ)
                                SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 01 | Equal Substrings Within Budget              | Sliding Window with Cost Budget   | O(N)     | O(1)     |
 | 02 | Count String Pairs That Can Form a Palindro | 26-Bit Parity Mask + HashMap      | O(totalC | O(N)     |
 | 03 | Circular Farm With Opposite Sectors         | Opposite Pair Sum + Circular Slid | O(N)     | O(N)     |
 | 04 | Maximum Distinct Elements Using -1, 0, or + | Sort + Greedy Smallest Available  | O(N logN)| O(1)     |
 | 05 | Maximize Weighted Difference by Reordering  | Rearrangement Inequality (A2 asc) | O(N logN)| O(1)     |
 | 06 | Sliding Window Rate Limiter                 | Per-User Timestamp Deque / Queue  | O(N)     | O(N)     |
 | 07 | Component Strength After Every Edge Additio | DSU with Max Node Tracking per Co | O((N+Q)α)| O(N)     |
 | 08 | Count Triplets With Sum Divisible by D      | Remainder Frequency Counting & Dy | O(N × D) | O(D)     |
 | 09 | Mountain Sequence Sum                       | Arithmetic Progression Formula i. | O(1)     | O(1)     |
 | 10 | Lexicographically Smallest String After Rep | Brute-force Substring Placement + | O(N²)    | O(N)     |
 | 11 | Number of Strictly Increasing Subarrays of  | Running Monotonic Streak Counter  | O(N)     | O(1)     |
 | 12 | Count Palindromic Substrings                | Expand Around Center (Odd and Eve | O(N²)    | O(1)     |
 | 13 | Find Earliest K-Minute Slot When Everyone I | Sort + Merge Busy Intervals + Gap | O(M logM)| O(M)     |
 | 14 | Price Query Conversion Cost                 | Sort + Prefix Sums + Binary Searc | O((N+Q)lg| O(N)     |
 | 15 | Minimum Stress Path in a Graph              | Minimax Dijkstra with Priority Qu | O((V+E)lg| O(V)     |
 | 16 | Good Array Using Powers of Two              | Set Bit Extraction + Query Range  | O(log N) | O(log N) |
 | 17 | Minimum Cost to Merge All Numbers           | Huffman Min-Heap Pair Summation   | O(N logN)| O(N)     |
 | 18 | Minimum Starting Resources for Manufacturin | Greedy Ordering Comparator on (wo | O(N logN)| O(N)     |
 | 19 | Lexicographically Smallest Concatenation of | Permutations of 3! = 6 Arrangemen | O(L)     | O(L)     |
 | 20 | Rotate Rows to Create an All-1 Column       | Circular Distance per Target Colu | O(N × M²)| O(1)     |
 | 21 | Sentence Validation                         | Word Tokenization + Alphanumeric  | O(N)     | O(N)     |
 | 22 | 0/1 Knapsack                                | 1D Space-Optimized DP (Reverse Sc | O(N × W) | O(W)     |
 | 23 | Prime Factor Multiples Flip                 | Trial Division Factors + Sieve Fl | O(M√X+Nlg| O(N)     |
 | 24 | Binary String Push + Reverse Construction   | Deque Reverse Simulation to Find  | O(N)     | O(N)     |
 | 25 | Greedy Resource Allocation                  | Sort Requirements & Resources + G | O(N logN)| O(1)     |
 ====================================================================================================
*/

// ====================================================================================================
// 1. EQUAL SUBSTRINGS WITHIN BUDGET (LC 1208)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given two strings `s` and `t` of equal length, and an integer `maxCost`.
  Changing `s[i]` into `t[i]` costs `abs(s[i] - t[i])` (the absolute ASCII difference).
  Find the maximum length of a substring of `s` that can be changed into the
  corresponding substring of `t` with total conversion cost at most `maxCost`.
  If there is no substring from `s` that can be changed within budget, return 0.

  PATTERN:
  Sliding Window with Dynamic Cost Budget

  INTUITION & KEY OBSERVATIONS:
  - We can define an effective cost array: `diff[i] = abs(s[i] - t[i])`.
  - Maintain a sliding window [l...r] expanding to the right.
  - While cost > maxCost, shrink window from left: cost -= abs(s[l] - t[l]), l++.
  - Update ans = max(ans, r - l + 1) at each step.

  COMPLEXITY:
  - Time: O(N) where N is length of strings.
  - Space: O(1) auxiliary space.
*/
class Solution1 {
public:
    int equalSubstring(string s, string t, int maxCost) {
        int l = 0, cost = 0, ans = 0;
        for (int r = 0; r < (int)s.size(); ++r) {
            cost += abs(s[r] - t[r]);
            while (cost > maxCost) cost -= abs(s[l] - t[l]), l++;
            ans = max(ans, r - l + 1);
        }
        return ans;
    }
};

// ====================================================================================================
// 2. COUNT STRING PAIRS THAT CAN FORM A PALINDROME (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array of lowercase English strings `words`, count the number of unordered
  pairs `(i, j)` with `i < j` such that after concatenating `words[i] + words[j]` and
  rearranging all characters in the combined string, the result can form a palindrome.

  PATTERN:
  26-Bit Parity Mask + Hash Map

  INTUITION & KEY OBSERVATIONS:
  - A string can be rearranged into a palindrome iff at most one character has an odd frequency.
  - Character parity mask: k-th bit is 1 if ('a' + k) frequency is odd, 0 if even.
  - Combined mask is m1 ^ m2. Valid if m1 ^ m2 == 0 or m1 ^ m2 == (1 << k).
  - Use hash map to match seen masks with current mask or 1-bit toggled masks.

  COMPLEXITY:
  - Time: O(totalChars + 26 * N).
  - Space: O(N) auxiliary space.
*/
class Solution2 {
public:
    long long countPalindromePairs(vector<string>& words) {
        unordered_map<int, int> freq;
        long long ans = 0;
        for (const string& s : words) {
            int mask = 0;
            for (char c : s) mask ^= (1 << (c - 'a'));
            ans += freq[mask];
            for (int i = 0; i < 26; ++i) ans += freq[mask ^ (1 << i)];
            freq[mask]++;
        }
        return ans;
    }
};

// ====================================================================================================
// 3. CIRCULAR FARM WITH OPPOSITE SECTORS (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  A circular farm is partitioned into `N` equal sectors numbered `0` to `N - 1`, where
  `N` is an EVEN integer. Each sector `i` yields a profit given by `profit[i]`.
  A farmer selects `K` consecutive sectors along the circumference (`1 <= K <= N / 2`).
  Whenever sector `i` is selected, its diametrically opposite sector `(i + N / 2) % N`
  is also harvested automatically.
  Find the maximum total profit the farmer can harvest.

  PATTERN:
  Opposite Pair Sum + Circular Sliding Window

  INTUITION & KEY OBSERVATIONS:
  - Effective yield of sector i: val[i] = profit[i] + profit[(i + N / 2) % N].
  - Reduces to finding maximum sum of K consecutive elements in circular array val.
  - Compute initial window of size K, slide across all N positions in O(N).

  COMPLEXITY:
  - Time: O(N) single pass sliding window.
  - Space: O(N) auxiliary space.
*/
class Solution3 {
public:
    long long maxProfit(vector<int>& profit, int K) {
        int n = profit.size(), half = n / 2;
        vector<long long> val(n);
        for (int i = 0; i < n; ++i) val[i] = (long long)profit[i] + profit[(i + half) % n];
        long long sum = accumulate(val.begin(), val.begin() + K, 0LL), ans = sum;
        for (int i = 1; i < n; ++i) {
            sum += val[(i + K - 1) % n] - val[i - 1];
            ans = max(ans, sum);
        }
        return ans;
    }
};

int maxCircularSum(vector<int>& val, int k) {
    int n = val.size();

    // Sum of first window
    int sum = 0;
    for (int i = 0; i < k; i++)
        sum += val[i % n];

    int ans = sum;

    for(int j = 1; j < n; j++) {
        sum += val[(j + k - 1) % n] - val[j - 1]; // val[(j + k - 1) % n] is the new element entering the window, val[j - 1] is the element leaving the window
        ans = max(ans, sum);
    }

    return ans;
} // this is for finding the maximum sum of k consecutive elements in a circular array

// ====================================================================================================
// 4. MAXIMUM DISTINCT ELEMENTS USING -1, 0, OR +1 (LC 3397)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an integer array `nums`.
  For each element `nums[i]`, you can perform at most one operation:
  replace `nums[i]` with `nums[i] - 1`, `nums[i]`, or `nums[i] + 1` (add an offset in {-1, 0, +1}).
  Find the maximum possible number of distinct elements in the array after modifying the elements.

  PATTERN:
  Sorting + Greedy Smallest Available Assignment

  INTUITION & KEY OBSERVATIONS:
  - Sort array ascending. Always assign each x to the smallest available value >= prev + 1.
  - Candidate is max((long long)x - 1, prev + 1). If candidate <= x + 1, accept it and update prev.

  COMPLEXITY:
  - Time: O(N log N) dominated by sorting.
  - Space: O(1) auxiliary space.
*/
class Solution4 {
public:
    int maxDistinctElements(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        long long prev = -2e18;
        int ans = 0;
        for (int x : nums) {
            long long cand = max((long long)x - 1, prev + 1);
            if (cand <= x + 1) ans++, prev = cand;
        }
        return ans;
    }
};

// ====================================================================================================
// 5. MAXIMIZE WEIGHTED DIFFERENCE BY REORDERING (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given two integer arrays `A1` and `A2`, each of length `N`.
  You are permitted to independently rearrange (permute) both arrays in any order you choose.
  Maximize the weighted sum:
      Σ (i * (A2[i] - A1[i])) for 0 <= i < N.

  PATTERN:
  Sorting + Rearrangement Inequality

  INTUITION & KEY OBSERVATIONS:
  - Σ i * A2[i] is maximized by sorting A2 ascending (same order as i).
  - Σ i * A1[i] is minimized by sorting A1 descending (opposite order to i).
  - Total weighted sum Σ i * (A2[i] - A1[i]) is maximized.

  COMPLEXITY:
  - Time: O(N log N) for sorting both arrays.
  - Space: O(1) auxiliary space.
*/
class Solution5 {
public:
    long long maxWeightedDifference(vector<int>& A1, vector<int>& A2) {
        sort(A2.begin(), A2.end());
        sort(A1.rbegin(), A1.rend());
        long long ans = 0;
        for (int i = 0; i < (int)A1.size(); ++i) ans += 1LL * i * (A2[i] - A1[i]);
        return ans;
    }
};

// ====================================================================================================
// 6. SLIDING WINDOW RATE LIMITER (MS OA / LC 359)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Design a rate limiter for incoming requests. Each request arrives with a `userId`
  and a `timestamp`.
  You are given sliding time window duration `T` and threshold `K` (maximum successful
  requests allowed in any window of duration `T`).
  A request is ACCEPTED (returns 1) if strictly fewer than `K` successful requests were
  made in [timestamp - T, timestamp]. Otherwise, REJECTED (returns 0).
  Only ACCEPTED requests are recorded into history.

  PATTERN:
  HashMap of Queues / Sliding Window Time Queues

  INTUITION & KEY OBSERVATIONS:
  - Map each userId to a queue of accepted timestamps.
  - Pop expired timestamps < timestamp - T.
  - If queue size < K: push current time, record 1. Else record 0.

  COMPLEXITY:
  - Time: O(1) amortized per request.
  - Space: O(U * K) where U is number of unique users.
*/
class Solution {
public:
    vector<int> rateLimiter(vector<string>& users, vector<int>& times, int T, int K) {
        // userId -> queue of accepted request timestamps
        unordered_map<string, queue<int>> mp;

        vector<int> ans;

        for (int i = 0; i < users.size(); i++) {
            string user = users[i];
            int time = times[i];

            queue<int>& q = mp[user];

            // Remove requests outside the sliding window
            while (!q.empty() && q.front() < time - T) {
                q.pop();
            }

            // Check if request can be accepted
            if (q.size() < K) {
                q.push(time);       // Record only accepted requests
                ans.push_back(1);   // Accepted
            } else {
                ans.push_back(0);   // Rejected
            }
        }

        return ans;
    }
};

// ====================================================================================================
// 7. COMPONENT STRENGTH AFTER EVERY EDGE ADDITION (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an undirected graph with `N` vertices labeled `1` to `N`.
  Initially, the graph contains no edges (each vertex forms its own isolated component).
  The "strength" of a connected component is defined as the MAXIMUM vertex label in that component.
  You are given a sequence of `Q` edge additions: `edges[i] = (u, v)`.
  After each edge is added, calculate the sum of strengths across all connected components
  present in the graph.

  PATTERN:
  Disjoint Set Union (DSU) with Component Maximum Tracking

  INTUITION & KEY OBSERVATIONS:
  - Initial sum = 1 + 2 + ... + N = N * (N + 1) / 2.
  - Track mx[root] in DSU. When edge (u, v) merges distinct roots a and b:
      total -= (mx[a] + mx[b])
      mx[a] = max(mx[a], mx[b]), p[b] = a
      total += mx[a]

  COMPLEXITY:
  - Time: O((N + Q) * α(N)).
  - Space: O(N) for DSU structures.
*/
class Solution7 {
    vector<int> p, mx;
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
public:
    vector<long long> getStrength(int n, vector<int>& from, vector<int>& to) {
        p.resize(n + 1); mx.resize(n + 1);
        iota(p.begin(), p.end(), 0);
        iota(mx.begin(), mx.end(), 0);
        long long total = 1LL * n * (n + 1) / 2;
        vector<long long> ans;
        for (int i = 0; i < (int)from.size(); ++i) {
            int a = find(from[i]), b = find(to[i]);
            if (a != b) {
                total -= (mx[a] + mx[b]);
                p[b] = a;
                mx[a] = max(mx[a], mx[b]);
                total += mx[a];
            }
            ans.push_back(total);
        }
        return ans;
    }
};

// ====================================================================================================
// 8. COUNT TRIPLETS WITH SUM DIVISIBLE BY D (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array of integers `nums` of length `N` and a positive integer `D`, count the
  number of index triplets `(i, j, k)` with `0 <= i < j < k < N` such that:
      (nums[i] + nums[j] + nums[k]) % D == 0.

  PATTERN:
  Modulo Remainder Counting + Dynamic Pair Tracking

  INTUITION & KEY OBSERVATIONS:
  - In modular arithmetic: (a + b + c) % D == 0 iff (a + b) % D == (D - c % D) % D.
  - Maintain single remainder counts cnt[r] and pair remainder counts pairCnt[r].
  - For each element x: add pairCnt[(D - r) % D] to answer, update pairCnt with all cnt,
    and increment cnt[r].

  COMPLEXITY:
  - Time: O(N * D).
  - Space: O(D) auxiliary space.
*/
class Solution8 {
public:
    long long countTriplets(vector<int>& nums, int d) {
        vector<long long> cnt(d, 0), pairCnt(d, 0);
        long long ans = 0;
        for (int x : nums) {
            int r = ((x % d) + d) % d; // handle negative numbers
            ans += pairCnt[(d - r) % d]; // count pairs (i, j) such that (nums[i] + nums[j] + x) % d == 0
            for (int rem = 0; rem < d; ++rem) pairCnt[(rem + r) % d] += cnt[rem]; // update pair counts with current remainder
            cnt[r]++; // increment single remainder count
        }
        return ans;
    }
};

// ====================================================================================================
// 9. MOUNTAIN SEQUENCE SUM (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given three integers `i`, `j`, and `k` satisfying `j >= i` and `j >= k`.
  A mountain sequence begins at `i`, strictly increases by 1 each step until reaching
  peak `j`, and then strictly decreases by 1 each step down to `k`.
  The sequence is:
      [i, i + 1, ..., j - 1, j, j - 1, ..., k + 1, k]
  Calculate the total sum of all elements in this sequence.

  PATTERN:
  Arithmetic Progression Closed-Form Formula

  INTUITION & KEY OBSERVATIONS:
  - S1: sum of AP from i to j = (i + j) * (j - i + 1) / 2.
  - S2: sum of AP from k to j - 1 = (k + j - 1) * (j - k) / 2.
  - Total = S1 + S2 using 64-bit integers.

  COMPLEXITY:
  - Time: O(1).
  - Space: O(1).
*/
class Solution9 {
    long long sum(long long l, long long r) { return l > r ? 0 : (l + r) * (r - l + 1) / 2; }
public:
    long long sequenceSum(int i, int j, int k) {
        return sum(i, j) + sum(k, j - 1);
    }
};

// ====================================================================================================
// 10. LEXICOGRAPHICALLY SMALLEST STRING AFTER REPLACING ? (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given a string `word` containing lowercase English letters and question marks `'?'`,
  and a target string `substr` containing only lowercase English letters.
  You must replace EVERY `'?'` in `word` with a lowercase English letter such that:
  1. `substr` appears as a contiguous substring in `word` at least once.
  2. The resulting string is lexicographically as small as possible.
  If it is impossible for `substr` to appear in `word`, return `"-1"`.

  PATTERN:
  Brute-Force Substring Placement + Greedy 'a' Replacement

  INTUITION & KEY OBSERVATIONS:
  - Try placing substr at every valid position i in [0, N - M].
  - Check match with word[i ... i + M - 1] (char equal or '?').
  - Fill all other '?' with 'a' to get candidate; track lexicographical minimum.

  COMPLEXITY:
  - Time: O((N - M + 1) * N).
  - Space: O(N) auxiliary space.
*/
class Solution10 {
public:
    string smallestString(string word, string sub) {
        int n = word.size(), m = sub.size();
        string ans = "";
        for (int i = 0; i + m <= n; ++i) {
            string cur = word;
            bool ok = true;
            for (int j = 0; j < m && ok; ++j) {
                if (cur[i + j] != '?' && cur[i + j] != sub[j]) ok = false;
                cur[i + j] = sub[j];
            }
            if (!ok) continue;
            for (char& c : cur) if (c == '?') c = 'a';
            if (ans.empty() || cur < ans) ans = cur;
        }
        return ans.empty() ? "-1" : ans;
    }
};

// ====================================================================================================
// 11. NUMBER OF STRICTLY INCREASING SUBARRAYS OF LENGTH K (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given an array of integers `nums` of length `N` and an integer `K` (`1 <= K <= N`),
  count the total number of contiguous subarrays of size exactly `K` that are
  strictly increasing (i.e. `arr[0] < arr[1] < ... < arr[K - 1]`).

  PATTERN:
  Running Monotonic Streak Counter / Linear Scan

  INTUITION & KEY OBSERVATIONS:
  - Maintain streak of strictly increasing elements ending at index i.
  - If nums[i] > nums[i - 1], streak++; else streak = 1.
  - When streak >= K, increment answer count.

  COMPLEXITY:
  - Time: O(N) single linear pass.
  - Space: O(1) auxiliary space.
*/
class Solution11 {
public:
    int countIncreasing(vector<int>& nums, int k) {
        int streak = 1, ans = (k == 1 ? 1 : 0);
        for (int i = 1; i < (int)nums.size(); ++i) {
            streak = (nums[i] > nums[i - 1]) ? streak + 1 : 1;
            if (streak >= k) ans++;
        }
        return ans;
    }
};

// ====================================================================================================
// 12. COUNT PALINDROMIC SUBSTRINGS (LC 647)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `s`, return the total number of palindromic substrings in it.
  A substring is a contiguous sequence of characters within the string.
  A string is a palindrome if it reads the same backward as forward.

  PATTERN:
  Expand Around Center (Odd and Even Centers)

  INTUITION & KEY OBSERVATIONS:
  - A string of length N has 2N - 1 possible centers.
  - Expand outward while characters match, incrementing palindrome count.

  COMPLEXITY:
  - Time: O(N^2) worst case.
  - Space: O(1) auxiliary space.
*/
class Solution12 {
public:
    int countSubstrings(string s) {
        int n = s.size(), ans = 0;
        for (int c = 0; c < 2 * n - 1; ++c) {
            int l = c / 2, r = l + c % 2;
            while (l >= 0 && r < n && s[l] == s[r]) ans++, l--, r++;
        }
        return ans;
    }
};

// ====================================================================================================
// 13. FIND EARLIEST K-MINUTE SLOT WHEN EVERYONE IS FREE (LC 1229 / MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given the busy schedules of multiple people represented as lists of time
  intervals `[start, end]`, and a meeting duration `K`.
  Find the earliest continuous time slot `[start, start + K]` of length `K` during which
  EVERY person is free (no person has any conflicting busy interval), within the overall
  operating timeframe `[0, dayEnd]`.
  If no common free slot of length at least `K` exists, return `{-1, -1}`.

  PATTERN:
  Interval Merging + Complementary Free Gap Search

  INTUITION & KEY OBSERVATIONS:
  - Sort and merge all busy intervals.
  - The free time intervals are the gaps between consecutive merged busy blocks.
  - The first gap of length >= K gives the earliest valid meeting start.

  COMPLEXITY:
  - Time: O(M log M) where M is total number of intervals.
  - Space: O(M) to store merged intervals.
*/
class Solution13 {
public:
    int earliestFreeSlot(vector<pair<int, int>>& intervals, int k) {
        sort(intervals.begin(), intervals.end());
        vector<pair<int, int>> merged;
        for (auto [s, e] : intervals) {
            if (merged.empty() || s > merged.back().second) merged.push_back({s, e});
            else merged.back().second = max(merged.back().second, e);
        }
        int freeStart = 0;
        for (auto [s, e] : merged) {
            if (s - freeStart >= k) return freeStart;
            freeStart = max(freeStart, e);
        }
        return freeStart;
    }
};

// ====================================================================================================
// 14. PRICE QUERY CONVERSION COST (LC 2602)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an integer array `prices` of length `N` and an array of integer queries `queries`
  of length `Q`.
  For each query `queries[j] = q`, calculate the minimum total cost to make all elements in
  `prices` equal to `q`.
  The cost to convert a price `prices[i]` into `q` is `abs(prices[i] - q)`.
  Return an array of answers for each query.

  PATTERN:
  Sorting + Prefix Sums + Binary Search (std::lower_bound)

  INTUITION & KEY OBSERVATIONS:
  - Sort prices, build prefix sums array.
  - For query q, binary search index idx with lower_bound:
      leftCost = idx * q - pref[idx]
      rightCost = (pref[n] - pref[idx]) - (n - idx) * q
  - Total cost = leftCost + rightCost in O(log N) per query.

  COMPLEXITY:
  - Time: O(N log N + Q log N).
  - Space: O(N) for prefix sums.
*/
class Solution14 {
public:
    vector<long long> solve(vector<int>& price, vector<int>& queries) {
        sort(price.begin(), price.end());
        int n = price.size();
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + price[i];
        vector<long long> ans;
        for (int q : queries) {
            int idx = lower_bound(price.begin(), price.end(), q) - price.begin();
            long long left = 1LL * q * idx - pref[idx];
            long long right = (pref[n] - pref[idx]) - 1LL * q * (n - idx);
            ans.push_back(left + right);
        }
        return ans;
    }
};

// ====================================================================================================
// 15. MINIMUM STRESS PATH IN A GRAPH (LC 1631 / MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an undirected connected graph with `V` vertices (labeled `0` to `V - 1`)
  and weighted edges `(u, v, weight)`.
  The "stress" of a path is defined as the MAXIMUM edge weight encountered along that path.
  Given a `source` vertex and a `destination` vertex, find the minimum possible stress
  of any path connecting `source` to `destination`.

  PATTERN:
  Minimax Dijkstra (Modified Shortest Path via Priority Queue)

  INTUITION & KEY OBSERVATIONS:
  - Let dist[u] be the minimum possible max-edge weight on path from source to u.
  - Relax edges with nextStress = max(stress, weight).
  - Min-heap extracts vertices in order of increasing bottleneck stress.

  COMPLEXITY:
  - Time: O((V + E) log V).
  - Space: O(V + E) for adjacency list and distance array.
*/
class Solution15 {
public:
    int minimumStress(int n, vector<vector<pair<int, int>>>& graph, int src, int dst) {
        vector<int> dist(n, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [stress, u] = pq.top(); pq.pop();
            if (u == dst) return stress;
            if (stress > dist[u]) continue;
            for (auto [v, w] : graph[u]) {
                int nextStress = max(stress, w);
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
// 16. GOOD ARRAY USING POWERS OF TWO (LC 2438)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a positive integer `n`, define its "good array" as the unique non-decreasing array
  of powers of 2 whose sum equals `n` (corresponding to the set bits in `n`'s binary representation).
  You are also given a 2D array `queries` where `queries[i] = [left, right]`.
  For each query, calculate the product of elements from index `left` to `right` (inclusive)
  in the good array, modulo `10^9 + 7`:
      (goodArray[left] * ... * goodArray[right]) % (10^9 + 7).
  Return an array containing the results for each query.

  PATTERN:
  Binary Bit Decomposition + Exponent Modular Multiplication

  INTUITION & KEY OBSERVATIONS:
  - Extract powers 2^b corresponding to set bits of n into powers array.
  - For each query [l, r], compute product of powers[l ... r] modulo q[2].

  COMPLEXITY:
  - Time: O(log n + Q).
  - Space: O(log n) auxiliary space.
*/
class Solution16 {
public:
    vector<int> productQueries(int n, vector<vector<int>>& queries) {
        vector<int> powers;
        for (int b = 0; b < 31; ++b) if (n & (1 << b)) powers.push_back(1 << b);
        vector<int> ans;
        for (auto& q : queries) {
            long long prod = 1;
            for (int i = q[0]; i <= q[1]; ++i) prod = (prod * powers[i]) % q[2];
            ans.push_back(prod);
        }
        return ans;
    }
};

// ====================================================================================================
// 17. MINIMUM COST TO MERGE ALL NUMBERS (LC 1167 / Standard Huffman)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array of integers `nums`. You can perform the following merge operation
  repeatedly until exactly one number remains:
  1. Pick any two numbers `x` and `y` from the array.
  2. Remove `x` and `y`.
  3. Insert `x + y` back into the array.
  The cost of this operation is `x + y`.
  Find the minimum total cost required to reduce the array to a single element.

  PATTERN:
  Min-Heap / Huffman Greedy Optimal Merge Pattern

  INTUITION & KEY OBSERVATIONS:
  - Merged numbers contribute repeatedly to subsequent operations.
  - Greedily merge the two smallest available numbers at each step via a min-heap.

  COMPLEXITY:
  - Time: O(N log N) using a min-heap.
  - Space: O(N) to store elements in priority queue.
*/
class Solution17 {
public:
    long long minimumCost(vector<int>& nums) {
        priority_queue<long long, vector<long long>, greater<>> pq(nums.begin(), nums.end());
        long long cost = 0;
        while (pq.size() > 1) {
            long long a = pq.top(); pq.pop();
            long long b = pq.top(); pq.pop();
            cost += a + b;
            pq.push(a + b);
        }
        return cost;
    }
};

// ====================================================================================================
// 18. MINIMUM STARTING RESOURCES FOR MANUFACTURING (LC 1665)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an array of tasks/products where `tasks[i] = [actual[i], worstCase[i]]`:
  - `actual[i]`: the amount of resources consumed to complete task `i`.
  - `worstCase[i]`: the minimum resources you must possess before starting task `i`.
  You can execute the tasks in any order.
  Find the minimum initial resources required to finish all tasks.

  PATTERN:
  Greedy Task Scheduling by Difference Comparator (worstCase - actual)

  INTUITION & KEY OBSERVATIONS:
  - Tasks with larger cushion (worstCase - actual) must be completed first.
  - Sort indices in descending order of (worstCase[i] - actual[i]).
  - Simulate resource depletion and accumulate required initial resources.

  COMPLEXITY:
  - Time: O(N log N) dominated by sorting.
  - Space: O(1) auxiliary space beyond index sorting.
*/
class Solution18 {
public:
    long long minimumStartingResources(vector<int>& worstCase, vector<int>& actual) {
        int n = worstCase.size();
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return (worstCase[a] - actual[a]) > (worstCase[b] - actual[b]);
        });
        long long cur = 0, req = 0;
        for (int i : order) {
            if (cur < worstCase[i]) req += (worstCase[i] - cur), cur = worstCase[i];
            cur -= actual[i];
        }
        return req;
    }
};

// ====================================================================================================
// 19. LEXICOGRAPHICALLY SMALLEST CONCATENATION OF 3 STRINGS (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given three strings `s1`, `s2`, and `s3`.
  You must concatenate all three strings together using each string exactly once.
  Find the ordering of concatenation that yields the lexicographically smallest resulting string.

  PATTERN:
  Permutation Exhaustion (Fixed N = 3, 3! = 6 Permutations)

  INTUITION & KEY OBSERVATIONS:
  - With 3 strings, there are only 3! = 6 permutations.
  - Sort strings initially, cycle permutations with next_permutation, and find minimum.

  COMPLEXITY:
  - Time: O(L) where L is total length of strings.
  - Space: O(L) auxiliary space.
*/
class Solution19 {
public:
    string smallestConcat(string a, string b, string c) {
        vector<string> v = {a, b, c};
        sort(v.begin(), v.end());
        string ans = v[0] + v[1] + v[2];
        while (next_permutation(v.begin(), v.end())) ans = min(ans, v[0] + v[1] + v[2]);
        return ans;
    }
};

// ====================================================================================================
// 20. ROTATE ROWS TO CREATE AN ALL-1 COLUMN (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given an `N x M` binary matrix `grid` (containing only 0s and 1s).
  In one operation, you can cyclically shift any row to the left by 1 or to the right by 1.
  Each shift costs 1 operation.
  Find the minimum total operations needed to create at least one column consisting entirely of 1s.
  If it is impossible (i.e. some row contains no 1s), return -1.

  PATTERN:
  Candidate Column Trial + Circular Distance Minimization

  INTUITION & KEY OBSERVATIONS:
  - If any row has no 1s, return -1.
  - For each target column c: in each row r, find minimum circular distance
    min(|j - c|, m - |j - c|) to bring any 1 to column c.
  - Total operations = sum of row minima. Return overall minimum across all columns.

  COMPLEXITY:
  - Time: O(N * M^2).
  - Space: O(1) auxiliary space.
*/
class Solution20 {
public:
    int minOperations(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size(), ans = INT_MAX;
        for (int c = 0; c < m; ++c) {
            int total = 0;
            bool ok = true;
            for (int r = 0; r < n && ok; ++r) {
                int best = INT_MAX;
                for (int j = 0; j < m; ++j) {
                    if (grid[r][j]) best = min(best, min(abs(j - c), m - abs(j - c)));
                }
                if (best == INT_MAX) ok = false;
                else total += best;
            }
            if (ok) ans = min(ans, total);
        }
        return ans == INT_MAX ? -1 : ans;
    }
};

// ====================================================================================================
// 21. SENTENCE VALIDATION (LC 3136 variant)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  Given a string `sentence` containing words separated by single or multiple whitespace characters,
  determine if the sentence is valid.
  A sentence is valid if and only if EVERY word satisfies ALL of the following conditions:
  1. Length is at least 3 characters.
  2. Contains only alphanumeric characters (digits '0'-'9', letters 'a'-'z', 'A'-'Z').
  3. Contains at least one vowel ('a', 'e', 'i', 'o', 'u', case-insensitive).
  4. Contains at least one consonant (any alphabetic letter that is not a vowel, case-insensitive).
  An empty sentence (no words) is invalid and returns `false`.

  PATTERN:
  String Tokenization + Character State Classification

  INTUITION & KEY OBSERVATIONS:
  - Tokenize sentence with stringstream.
  - Each word must have length >= 3, isalnum characters only, and at least 1 vowel + 1 consonant.

  COMPLEXITY:
  - Time: O(N) where N is length of sentence.
  - Space: O(N) for token storage.
*/
class Solution21 {
    bool validWord(const string& w) {
        if (w.size() < 3) return false;
        bool v = false, c = false;
        for (char ch : w) {
            if (!isalnum(ch)) return false;
            if (isalpha(ch)) {
                if (string("aeiouAEIOU").find(ch) != string::npos) v = true;
                else c = true;
            }
        }
        return v && c;
    }
public:
    bool validate(string sentence) {
        stringstream ss(sentence);
        string w;
        bool hasWord = false;
        while (ss >> w) {
            hasWord = true;
            if (!validWord(w)) return false;
        }
        return hasWord;
    }
};

// ====================================================================================================
// 22. 0/1 KNAPSACK (Standard DP)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given `N` items, each with an associated weight `weights[i]` and profit `values[i]`.
  You are also given a knapsack with maximum weight capacity `W`.
  Each item can be included at most once (0 or 1).
  Find the maximum total profit that can be achieved without exceeding the weight capacity `W`.

  PATTERN:
  0/1 Knapsack DP with 1D Space Optimization

  INTUITION & KEY OBSERVATIONS:
  - dp[w]: maximum profit with capacity w.
  - For each item, iterate w backwards from capacity down to weight[i] to ensure each item is
    used at most once: dp[w] = max(dp[w], dp[w - weight[i]] + value[i]).

  COMPLEXITY:
  - Time: O(N * W).
  - Space: O(W) 1D space-optimized array.
*/
class Solution22 {
public:
    int knapsack(vector<int>& weight, vector<int>& value, int capacity) {
        vector<int> dp(capacity + 1, 0);
        for (int i = 0; i < (int)weight.size(); ++i) {
            for (int w = capacity; w >= weight[i]; --w) {
                dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
            }
        }
        return dp[capacity];
    }
};

// ====================================================================================================
// 23. PRIME FACTOR MULTIPLES FLIP (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given:
  1. A binary array `state` of size `N` (1-based indexed, elements are 0 or 1).
  2. An array `nums` of positive integers.
  For every number `x` in `nums`:
  1. Find all distinct prime factors of `x`.
  2. For every distinct prime factor `p`, flip all positions in `state` whose 1-based index
     is divisible by `p` (i.e. indices `p, 2p, 3p, ... <= N`).
  Return the final `state` array after processing all numbers.

  PATTERN:
  Trial Division Prime Factorization + Sieve Toggling

  INTUITION & KEY OBSERVATIONS:
  - For each number x, factorize in O(√x).
  - For each distinct prime factor p, toggle all multiples p, 2p, 3p, ... in flip array.
  - Apply net flips to state array in O(N).

  COMPLEXITY:
  - Time: O(M * √X + N log N).
  - Space: O(N) auxiliary space.
*/
class Solution23 {
public:
    vector<int> finalState(vector<int>& state, vector<int>& nums) {
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
        for (int i = 1; i <= n; ++i) if (flip[i]) state[i - 1] ^= 1;
        return state;
    }
};

// ====================================================================================================
// 24. BINARY STRING PUSH + REVERSE CONSTRUCTION (MS OA)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given the count of zeros (`zeros`) and count of ones (`ones`) available to construct
  a binary string `s` of length `N = zeros + ones`.
  Starting with an empty string `b = ""`, characters of `s` are processed from left to right:
  For each character `s[i]`:
  1. Append `s[i]` to `b`.
  2. Reverse `b`.
  You want to arrange the available `zeros` 0's and `ones` 1's in `s` such that the final
  string `b` is lexicographically as large as possible.
  Return the optimal final string `b` (and the input sequence `s` that generates it).

  PATTERN:
  Permutation Index Simulation + Greedy MSB Placement

  INTUITION & KEY OBSERVATIONS:
  - Simulating the append-and-reverse operations on indices tracks where each s[i] lands in b.
  - Greedily place '1's at the most significant positions of b (earliest in deque simulation order).

  COMPLEXITY:
  - Time: O(N) where N = zeros + ones.
  - Space: O(N) auxiliary space.
*/
#include <bits/stdc++.h>
using namespace std;

pair<string, string> getOptimalStrings(int zeros, int ones) {
    int n = zeros + ones;

    string b(ones, '1');
    b += string(zeros, '0');

    vector<int> order(n);
    int l = 0, r = n - 1;
    bool placeLeft = true;

    for (int i = n - 1; i >= 0; --i) {
        if (placeLeft)
            order[l++] = i;
        else
            order[r--] = i;

        placeLeft = !placeLeft;
    }

    string s(n, '0');

    for (int i = 0; i < n; ++i)
        s[order[i]] = b[i];

    return {b, s};
}

// ====================================================================================================
// 25. GREEDY RESOURCE ALLOCATION (LC 455 variant)
// ====================================================================================================
/*
  PROBLEM STATEMENT:
  You are given two integer arrays:
  - `requirements`: where `requirements[i]` is the minimum resource capacity needed by the i-th task.
  - `resources`: where `resources[j]` is the capacity of the j-th available resource.
  Each resource can be assigned to at most one task, and each task can receive at most one resource.
  A resource `j` satisfies task `i` if `resources[j] >= requirements[i]`.
  Find the maximum number of tasks/requirements that can be satisfied.

  PATTERN:
  Sorting + Two Pointers (Greedy Matching)

  INTUITION & KEY OBSERVATIONS:
  - Sort requirements and resources ascending.
  - Greedily match smallest capable resource to smallest requirement with two pointers.

  COMPLEXITY:
  - Time: O(N log N + M log M).
  - Space: O(1) auxiliary space beyond sorting.
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
