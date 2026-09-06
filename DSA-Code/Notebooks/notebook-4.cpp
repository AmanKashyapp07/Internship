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
 | 1  | Kth Smallest in Lexicographical Order (440) | Trie Prefix Tree Level Skipping   | O(log^2N)| O(1)     |
 | 2  | Longest Nice Subarray (LC 2401)             | Sliding Window + Cumulative OR    | O(N)     | O(1)     |
 | 3  | Interval List Intersections (LeetCode 986)  | Two-Pointer Boundary Intersection | O(N + M) | O(N + M) |
 | 4  | Data Stream Disjoint Intervals (LC 352)     | Balanced BST (std::set) Neighbor  | O(log N) | O(N)     |
 | 5  | My Calendar I (LeetCode 729)                | std::set lower_bound Boundary BST | O(log N) | O(N)     |
 | 6  | My Calendar II (LeetCode 731)               | Dual Overlap List Verification    | O(N)     | O(N)     |
 | 7  | My Calendar III (LeetCode 732)              | Sweep-Line Difference Map         | O(N)     | O(N)     |
 | 8  | Corporate Flight Bookings (LeetCode 1109)   | 1D Range Difference Array + Prefix| O(N + F) | O(N)     |
 | 9  | Range Module (LeetCode 715)                 | std::map Disjoint Interval Slices | O(K logN)| O(N)     |
 | 10 | Count Integers in Intervals (LC 2276)       | std::map Dynamic Overlap Merging  | O(log N) | O(N)     |
 | 11 | Remove Covered Intervals (LeetCode 1288)    | Custom Sort (Start Asc, End Desc) | O(N logN)| O(1)     |
 | 12 | Maximum Length of Pair Chain (LC 646)       | Greedy End-Time Selection (LIS DP)| O(N logN)| O(1)     |
 | 13 | Area Painted Each Day (LeetCode 2158)       | Jump-Table / DSU Path Compression | O(N+MaxP)| O(MaxPos)|
 | 14 | Teemo Attacking (LeetCode 495)              | Timeline Overlap Accumulation     | O(N)     | O(1)     |
 | 15 | In-Place Merge Sort (Gap Method)            | Shell-like Gap Shelling           | O(N log^2N)| O(1)   |
 | 16 | Radix Sort MSD (Recursive Bucket Sort)      | Trie-like MSD Prefix Partitioning | O(N * L) | O(N+R*D) |
 | 17 | Pigeonhole Sort (Direct Range Slots)        | Range-Sized Bucket Placement      | O(N + R) | O(N + R) |
 | 18 | Pancake Sorting (LeetCode 969)              | Greedy Suffix Placement (2-Flips) | O(N^2)   | O(N)     |
 | 19 | Patience Sorting & LIS (Greene's Theorem)   | Pile Insertion via lower_bound    | O(N logN)| O(N)     |
 | 20 | Sort Matrix Diagonally (LeetCode 1329)      | Hash Map Diagonal Bucketing       | O(MN logM)| O(M * N)|
 | 21 | Sort Transformed Array (LeetCode 360)       | Parabola Concavity Two-Pointers   | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. KTH SMALLEST IN LEXICOGRAPHICAL ORDER — LeetCode 440
// ============================================================

class LexicographicalKth {
public:
    int findKthNumber(int n, int k) {
        int cur = 1; k--;
        while (k > 0) {
            ll steps = 0, first = cur, last = cur + 1;
            while (first <= n) {
                steps += min((ll)n + 1, last) - first;
                first *= 10;
                last *= 10;
            }
            if (steps <= k) { cur++; k -= steps; }
            else { cur *= 10; k--; }
        }
        return cur;
    }
};
// Interview Explanation:
// - Problem Statement: Find the k-th smallest integer in lexicographical order from 1 to n (LC 440).
// - Approach: Denary Trie / Prefix Tree Level Skipping.
// - Intuition:
//   * Lexicographical ordering corresponds to pre-order traversal of a 10-ary prefix tree rooted at numbers 1..9.
//   * For current prefix `cur`, the range of numbers under its subtree at depth d is [cur * 10^d, (cur + 1) * 10^d).
//   * Count how many valid integers <= n fall within the subtree of `cur`.
//   * If total subtree count `steps <= k`, the k-th number is not in this subtree; skip horizontally (`cur++`, `k -= steps`).
//   * Otherwise, the target lies within this subtree; descend one level deeper (`cur *= 10`, `k--`).
// - Complexity: Time: O(log^2 N), Space: O(1).


// ============================================================
// 2. LONGEST NICE SUBARRAY (PAIRWISE AND = 0) — LeetCode 2401
// ============================================================

class LongestNiceSubarray {
public:
    int longestNiceSubarray(vi& a) {
        int n = a.size(), l = 0, mask = 0, ans = 0;
        // mask represents the bitwise OR of all elements in the current window [l..r], we are storing OR because if any two numbers have a common set bit, their AND will be non-zero.
        for (int r = 0; r < n; r++) {
            while ((mask & a[r]) != 0) { // remove left until no common set bits with a[r]
                mask ^= a[l];
                l++;
            }
            mask |= a[r]; // add a[r] to current window
            ans = max(ans, r - l + 1);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest subarray where bitwise AND of every pair is 0 (LC 2401).
// - Approach: Sliding Window with cumulative bitmask.
// - Intuition:
//   * Pairwise AND = 0 across all elements in a subarray means no two numbers share a set bit at any position.
//   * The bitwise OR of all numbers in the window equals their sum, and each bit position is set at most once.
//   * Since standard integers have at most 30 bits, any valid window can contain at most 30 elements.
//   * If adding a[r] introduces a collision (`mask & a[r] != 0`), shrink from the left by XORing out a[l] until collision clears.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 3. INTERVAL LIST INTERSECTIONS — LeetCode 986
// ============================================================

vvi intervalIntersection(vvi& a, vvi& b) {
    vvi ans;
    int i = 0, j = 0;
    int n = a.size(), m = b.size();

    while (i < n && j < m) {
        int lo = max(a[i][0], b[j][0]);
        int hi = min(a[i][1], b[j][1]);

        if (lo <= hi) { // if they intersect, add the intersection to the answer
            ans.push_back({lo, hi});
        }

        if (a[i][1] < b[j][1]) i++;
        else j++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find intersections of two lists of closed intervals (LeetCode 986).
// - Approach: Two-Pointer Sweep Intersection.
// - Intuition:
//   * Both lists are pairwise disjoint and individually sorted.
//   * The overlapping segment between interval a[i] and b[j] is always [max(a[i][0], b[j][0]), min(a[i][1], b[j][1])].
//   * A valid non-empty overlap exists if and only if lo <= hi.
//   * Advance the pointer of the interval that finishes earlier (smaller endpoint), as it cannot possibly intersect any future interval.
// - Complexity: Time: O(N + M), Space: O(N + M).


// ============================================================
// 4. DATA STREAM AS DISJOINT INTERVALS — LeetCode 352
// ============================================================

class SummaryRangesStream {
    set<pair<int, int>> st;
public:
    SummaryRangesStream() {}

    void addNum(int x) {
        auto it = st.lower_bound({x, x});
        int l = x, r = x;

        if (it != st.begin() && prev(it)->second >= x - 1) {
            it = prev(it);
        }

        while (it != st.end() && it->first <= x + 1) {
            l = min(l, it->first);
            r = max(r, it->second);
            it = st.erase(it);
        }

        st.insert({l, r});
    }

    vvi getIntervals() {
        vvi ans;
        for (auto& [s, e] : st) ans.push_back({s, e});
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain non-overlapping intervals as integers are added dynamically (LeetCode 352).
// - Approach: Balanced BST (`std::set<pair<int, int>>`) Disjoint Interval Merging.
// - Intuition:
//   * Maintain disjoint intervals sorted by start coordinate in a balanced binary search tree.
//   * Adding number x can either bridge two existing intervals, extend an existing interval, or form a new single-element interval.
//   * Check predecessor to see if it can absorb or touch x (prev->second >= x - 1).
//   * Erase all consecutive overlapping or adjacent intervals (it->first <= x + 1) while expanding unified bounds [l, r], then re-insert.
// - Complexity: Time: Amortized O(log N) per `addNum`, O(N) for `getIntervals`, Space: O(N).


// ============================================================
// 9. RANGE MODULE — LeetCode 715
// ============================================================

class RangeModule {
    map<int, int> mp;
public:
    RangeModule() {}

    void addRange(int l, int r) {
        auto it = mp.upper_bound(l);
        if (it != mp.begin() && prev(it)->second >= l) it = prev(it);

        while (it != mp.end() && it->first <= r) {
            l = min(l, it->first);
            r = max(r, it->second);
            it = mp.erase(it);
        }
        mp[l] = r;
    }

    bool queryRange(int l, int r) {
        auto it = mp.upper_bound(l);
        if (it == mp.begin()) return false;
        it = prev(it);
        return it->second >= r;
    }

    void removeRange(int l, int r) {
        auto it = mp.upper_bound(l);
        if (it != mp.begin() && prev(it)->second > l) it = prev(it);

        vector<pair<int, int>> ins;
        while (it != mp.end() && it->first < r) {
            if (it->first < l) ins.push_back({it->first, l});
            if (it->second > r) ins.push_back({r, it->second});
            it = mp.erase(it);
        }

        for (auto& [a, b] : ins) mp[a] = b;
    }
};
// Interview Explanation:
// - Problem Statement: Design data structure to track numeric ranges with add, query, remove (LeetCode 715).
// - Approach: `std::map<int, int>` Disjoint Interval Slicing.
// - Intuition:
//   * Maintain disjoint intervals in `mp` mapping interval start -> end.
//   * `addRange`: Find overlapping ranges, merge endpoints [min_l, max_r], erase swallowed intervals, insert merged range.
//   * `queryRange`: Find candidate covering range via upper_bound; it fully covers [l, r) iff `prev->first <= l` and `prev->second >= r`.
//   * `removeRange`: Intersecting intervals are trimmed or split: preserve remaining left slice (start < l) and right slice (end > r).
// - Complexity: Time: Amortized O(log N) per operation, Space: O(N).


// ============================================================
// 10. COUNT INTEGERS IN INTERVALS — LeetCode 2276
// ============================================================

class CountIntervals {
    map<int, int> mp;
    int tot;
public:
    CountIntervals() : tot(0) {}

    void add(int l, int r) {
        auto it = mp.upper_bound(l);
        if (it != mp.begin() && prev(it)->second >= l) it = prev(it);

        while (it != mp.end() && it->first <= r) {
            l = min(l, it->first);
            r = max(r, it->second);
            tot -= (it->second - it->first + 1);
            it = mp.erase(it);
        }

        mp[l] = r;
        tot += (r - l + 1);
    }

    int count() {
        return tot;
    }
};
// Interview Explanation:
// - Problem Statement: Add intervals and query total count of distinct integers covered (LeetCode 2276).
// - Approach: `std::map` Disjoint Intervals with Running Integer Count.
// - Intuition:
//   * Maintain disjoint intervals and a running total `tot` of distinct covered integers.
//   * Locate predecessor if it overlaps or abuts l (prev->second >= l).
//   * While next interval overlaps r (it->first <= r), deduct its length from `tot` and erase it from map.
//   * Insert unified interval [l, r] and add its length (r - l + 1) to `tot`.
//   * Query executes in O(1) by returning `tot`.
// - Complexity: Time: Amortized O(log N) per `add`, O(1) for `count`, Space: O(N).


// ============================================================
// 11. REMOVE COVERED INTERVALS — LeetCode 1288
// ============================================================

bool compareCoveredIntervals(const vi& a, const vi& b) {
    if (a[0] != b[0]) return a[0] < b[0];
    return a[1] > b[1];
}

int removeCoveredIntervals(vvi& a) {
    sort(a.begin(), a.end(), compareCoveredIntervals);

    int ans = 0;
    int mxR = 0;

    for (const auto& iv : a) {
        if (iv[1] > mxR) {
            ans++;
            mxR = iv[1];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Remove all intervals covered by another interval in the list (LeetCode 1288).
// - Approach: Custom Sorting (Start Ascending, End Descending) + Max End Frontier.
// - Intuition:
//   * Interval B is covered by A iff A[0] <= B[0] and B[1] <= A[1].
//   * Sort by start time ascending. If start times match, sort by end time descending so wider intervals come first.
//   * Under this ordering, every subsequent interval has start >= previous start.
//   * Therefore, an interval is covered if and only if its end is <= the maximum end seen so far (`mxR`).
//   * If `iv[1] > mxR`, it cannot be covered; increment count and update `mxR`.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 12. MAXIMUM LENGTH OF PAIR CHAIN — LeetCode 646
// ============================================================

int findLongestChain(vector<vector<int>>& pairs) {
        // Sort pairs by their SECOND element (right endpoint)
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });
        
        int chainCount = 0;
        long long curEnd = -1e18; // Use a tiny number as initial boundary
        
        for (const auto& pair : pairs) {
            // If the current pair starts after the previous one ends
            if (pair[0] > curEnd) {
                chainCount++;
                curEnd = pair[1]; // Move the boundary to this pair's end
            }
        }
        
        return chainCount;
    }
// Interview Explanation:
// - Problem Statement: Find longest chain of pairs (p1, p2) where p1[1] < p2[0] (LeetCode 646).
// - Approach: Greedy Interval Scheduling (Sort by Second Element).
// - Intuition:
//   * This is isomorphic to the classic Activity Selection / Interval Scheduling problem.
//   * Sorting pairs by their finish time (second element) ascending guarantees greedy optimality.
//   * Selecting the pair that ends earliest leaves maximum possible remaining timeline room for subsequent pairs.
//   * Iterate through sorted pairs; whenever `a[i][0] > curR`, append the pair to the chain and update `curR`.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 13. AMOUNT OF NEW AREA PAINTED EACH DAY — LeetCode 2158
// ============================================================

vi amountPainted(vvi& a) {
    int mx = 0;
    for (auto& p : a) mx = max(mx, p[1]);

    vi jmp(mx + 1, 0);
    vi ans(a.size(), 0);

    for (int i = 0; i < (int)a.size(); i++) {
        int l = a[i][0], r = a[i][1];
        int painted = 0;

        int cur = l;
        while (cur < r) {
            if (jmp[cur] == 0) {
                jmp[cur] = max(jmp[cur], r);
                painted++;
                cur++;
            } else {
                int nxt = jmp[cur];
                jmp[cur] = max(jmp[cur], r);
                cur = nxt;
            }
        }
        ans[i] = painted;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Daily painting of intervals [start, end]; return new area painted each day (LeetCode 2158).
// - Approach: Jump Table / DSU Path Compression.
// - Intuition:
//   * Naively iterating painted cells would take O(N * length), which TLEs on repeated paints.
//   * Maintain `jmp[x]`, storing the farthest painted endpoint reachable from x.
//   * If `jmp[cur] == 0`, point cur has never been painted; increment day count, mark `jmp[cur] = max(jmp[cur], r)`, and advance to cur + 1.
//   * If `jmp[cur] > 0`, position cur is already painted; jump directly to `jmp[cur]` and update `jmp[cur] = max(jmp[cur], r)`.
//   * Path compression ensures each timeline unit is processed once.
// - Complexity: Time: O(N + MaxPosition), Space: O(MaxPosition).


// ============================================================
// 14. TEEMO ATTACKING — LeetCode 495
// ============================================================

int findPoisonedDuration(vi& a, int d) {
    if (a.empty()) return 0;
    int ans = 0;

    for (int i = 1; i < (int)a.size(); i++) {
        ans += min(d, a[i] - a[i - 1]);
    }
    ans += d;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return total poisoned time given attack timestamps and duration (LeetCode 495).
// - Approach: Linear Interval Overlap Accumulation.
// - Intuition:
//   * Each attack at time t resets the poison timer for d seconds, covering [t, t + d).
//   * If the gap to the next attack `a[i] - a[i-1]` is >= d, the poison expires completely, contributing full duration d.
//   * If the gap is < d, the poison resets early, contributing only the elapsed time `a[i] - a[i-1]`.
//   * The final attack always contributes full duration d.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 15. IN-PLACE MERGE SORT (GAP METHOD)
// ============================================================

void inPlaceMerge(vi& a, int l, int m, int r) {
    int len = r - l + 1;
    int gap = (len + 1) / 2;
    while (gap > 0) {
        int i = l;
        while (i + gap <= r) {
            int j = i + gap;
            if (a[i] > a[j]) {
                swap(a[i], a[j]);
            }
            i++;
        }
        gap = (gap > 1) ? (gap + 1) / 2 : 0;
    }
}

void inPlaceMergeSort(vi& a, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    inPlaceMergeSort(a, l, mid);
    inPlaceMergeSort(a, mid + 1, r);
    inPlaceMerge(a, l, mid, r);
}
// Interview Explanation:
// - Problem Statement: Merge two sorted adjacent subarrays in-place without extra memory.
// - Approach: Shell's Gap Reduction Method across boundary.
// - Intuition:
//   * Traditional merge sort requires O(N) auxiliary space for buffer arrays.
//   * The Gap Method (from Shell sort) initializes `gap = ceil(len / 2)`.
//   * Compare elements separated by distance `gap` (`a[i]` and `a[i + gap]`) and swap if out of order.
//   * Halving gap repeatedly until 0 guarantees that all elements settle into their fully sorted positions with O(1) extra space.
// - Complexity: Time: O(N log^2 N), Space: O(1).


// ============================================================
// 18. PANCAKE SORTING — LeetCode 969
// ============================================================

vi pancakeSort(vi& a) {
    vi ans;
    int n = a.size();
    for (int j = n; j > 1; j--) {
        int mxIdx = max_element(a.begin(), a.begin() + j) - a.begin(); // find max element in subarray [0..j-1]
        if (mxIdx != j - 1) { // if max element is not already at its final position (j-1), we need to flip
            if (mxIdx != 0) { // if max element is not at the front, flip it to the front first
                ans.push_back(mxIdx + 1);
                reverse(a.begin(), a.begin() + mxIdx + 1);
            } // after this flip, the max element is at index 0
            ans.push_back(j); // flip the first j elements to move max element to its final position at index j-1
            reverse(a.begin(), a.begin() + j); // reverse the first j elements to place the max element at its final position
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort array using prefix reversals (pancake flips) with <= 2*N flips (LeetCode 969).
// - Approach: Greedy Suffix Placement (At most 2 flips per element).
// - Intuition:
//   * Sort elements from largest to smallest, locking elements into place from the end (n down to 2).
//   * For current target value j, find its index `mxIdx` in prefix [0..j-1].
//   * If not already at index 0, flip prefix of length `mxIdx + 1` to bring target value to index 0.
//   * Flip prefix of length j to move target value to its correct final sorted position at index j - 1.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 19. PATIENCE SORTING & LIS (GREENE'S THEOREM)
// ============================================================

int patienceSortLISLength(const vi& a) {
    vi p;
    for (int x : a) {
        auto it = lower_bound(p.begin(), p.end(), x); // find leftmost pile top >= x
        if (it == p.end()) p.push_back(x); // if no such pile exists, create new pile
        else *it = x; // replace the top of the found pile with x to maintain minimum tail value
    }
    return p.size();
}
// Interview Explanation:
// - Problem Statement: Determine LIS length using Patience Sorting card pile game.
// - Approach: Greedy Card Pile Placement + Binary Search (`lower_bound`).
// - Intuition:
//   * In patience sorting, each card is placed on the leftmost pile whose top card is >= current card.
//   * If no such pile exists, create a new pile to the right.
//   * Piles maintain the minimum tail value of increasing subsequences of length i + 1.
//   * By Greene's Theorem, the minimum number of decreasing piles formed equals the length of the Longest Increasing Subsequence.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 20. SORT MATRIX DIAGONALLY — LeetCode 1329
// ============================================================

vvi diagonalSort(vvi& a) {
    int m = a.size(), n = a[0].size();
    unordered_map<int, vi> mp;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mp[i - j].push_back(a[i][j]);
        }
    }
    for (auto& [k, v] : mp) {
        sort(v.rbegin(), v.rend());
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = mp[i - j].back();
            mp[i - j].pop_back();
        }
    }
    return a;
}
// Interview Explanation:
// - Problem Statement: Sort each diagonal of an m x n matrix ascending (LeetCode 1329).
// - Approach: Hash Map Diagonal Grouping with Key `i - j`.
// - Intuition:
//   * Any diagonal running from top-left to bottom-right satisfies the algebraic invariant that `row - col` is constant.
//   * Group all elements of matrix by key `i - j` into a hash map of vectors.
//   * Sort each diagonal vector in descending order so popping from the back gives the smallest elements first.
//   * Re-populate matrix cells by popping back from `mp[i - j]`.
// - Complexity: Time: O(M * N * log(min(M, N))), Space: O(M * N).


