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
 | 13 | Area Painted Each Day (LeetCode 2158)       | Jump-Table / DSU Path Compression | O(N+Range| O(Range) |
 | 14 | Teemo Attacking (LeetCode 495)              | Timeline Overlap Accumulation     | O(N)     | O(1)     |
 | 15 | In-Place Merge Sort (Gap Method)            | Shell-like Gap Shelling           | O(N log^2N)| O(1)   |
 | 16 | Radix Sort MSD (Recursive Bucket Sort)      | Trie-like MSD Prefix Partitioning | O(D(N+B))| O(N + B) |
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
        int curr = 1; k--;
        while (k > 0) {
            long long steps = 0, first = curr, last = curr + 1;
            while (first <= n) {
                steps += min((long long)n + 1, last) - first;
                first *= 10;
                last *= 10;
            }
            if (steps <= k) { curr++; k -= steps; }
            else { curr *= 10; k--; }
        }
        return curr;
    }
};
// Interview Explanation:
// - Problem Statement: Find the k-th smallest integer in lexicographical order from 1 to n (LC 440).
// - Approach: Trie / Prefix Tree Level-by-Level Skipping.
// - Intuition: Count numbers in prefix subtree [curr, curr+1); skip whole subtree if steps <= k, else go deeper.
// - Complexity: Time: O(log^2 N), Space: O(1).


// ============================================================
// 2. LONGEST NICE SUBARRAY (PAIRWISE AND = 0) — LeetCode 2401
// ============================================================

class LongestNiceSubarray {
public:
    int longestNiceSubarray(vector<int>& nums) {
        int n = nums.size(), left = 0, mask = 0, ans = 0;
        for (int right = 0; right < n; right++) {
            while ((mask & nums[right]) != 0) {
                mask ^= nums[left];
                left++;
            }
            mask |= nums[right];
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest subarray where bitwise AND of every pair is 0 (LC 2401).
// - Approach: Sliding Window with cumulative bitmask.
// - Intuition: Maintain window bitmask with XOR/OR; shrink left whenever adding nums[right] introduces a bit collision.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 3. INTERVAL LIST INTERSECTIONS — LeetCode 986
// ============================================================

vector<vector<int>> intervalIntersection(vector<vector<int>>& firstList, vector<vector<int>>& secondList) {
    vector<vector<int>> result;
    int i = 0, j = 0;
    int n = firstList.size(), m = secondList.size();

    while (i < n && j < m) {
        int lo = max(firstList[i][0], secondList[j][0]);
        int hi = min(firstList[i][1], secondList[j][1]);

        if (lo <= hi) {
            result.push_back({lo, hi});
        }

        if (firstList[i][1] < secondList[j][1]) i++;
        else j++;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Find intersections of two lists of closed intervals (LeetCode 986).
// - Approach: Two-Pointer Sweep Intersection.
// - Intuition: Intersection interval is `[max(startA, startB), min(endA, endB)]`. Valid if `lo <= hi`.
// - Complexity: Time: O(N + M), Space: O(N + M).


// ============================================================
// 4. DATA STREAM AS DISJOINT INTERVALS — LeetCode 352
// ============================================================

class SummaryRangesStream {
    set<pair<int, int>> intervals;
public:
    SummaryRangesStream() {}

    void addNum(int val) {
        auto it = intervals.lower_bound({val, val});
        int start = val, end = val;

        if (it != intervals.begin() && prev(it)->second >= val - 1) {
            it = prev(it);
        }

        while (it != intervals.end() && it->first <= val + 1) {
            start = min(start, it->first);
            end = max(end, it->second);
            it = intervals.erase(it);
        }

        intervals.insert({start, end});
    }

    vector<vector<int>> getIntervals() {
        vector<vector<int>> res;
        for (auto& [s, e] : intervals) res.push_back({s, e});
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain non-overlapping intervals as integers are added dynamically (LeetCode 352).
// - Approach: Balanced BST (`std::set<pair<int, int>>`) Disjoint Interval Merging.
// - Intuition: Check predecessor and successor; erase overlapping fragments and insert unified range.
// - Complexity: Time: O(log N) amortized per `addNum`, O(N) for `getIntervals`, Space: O(N).


// ============================================================
// 5. MY CALENDAR I — LeetCode 729
// ============================================================

class MyCalendar {
    set<pair<int, int>> bookings;
public:
    MyCalendar() {}

    bool book(int start, int end) {
        auto next = bookings.lower_bound({start, end});
        if (next != bookings.end() && next->first < end) return false;
        if (next != bookings.begin() && prev(next)->second > start) return false;

        bookings.insert({start, end});
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Implement calendar booking such that no two events intersect (LeetCode 729).
// - Approach: BST `std::set` Binary Search (`lower_bound`).
// - Intuition: Find first event starting >= `start`. Conflict exists if `next.start < end` or `prev.end > start`.
// - Complexity: Time: O(log N) per `book`, Space: O(N).


// ============================================================
// 6. MY CALENDAR II — LeetCode 731
// ============================================================

class MyCalendarTwo {
    vector<pair<int, int>> bookings;
    vector<pair<int, int>> overlaps;
public:
    MyCalendarTwo() {}

    bool book(int start, int end) {
        for (auto& [s, e] : overlaps) {
            if (max(start, s) < min(end, e)) return false;
        }

        for (auto& [s, e] : bookings) {
            int os = max(start, s), oe = min(end, e);
            if (os < oe) overlaps.push_back({os, oe});
        }

        bookings.push_back({start, end});
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Allow double bookings but reject any booking causing a triple booking (LeetCode 731).
// - Approach: Dual Interval Lists (`bookings` & `overlaps`).
// - Intuition: A triple booking occurs if new interval intersects any interval in `overlaps`.
// - Complexity: Time: O(N) per `book`, Space: O(N).


// ============================================================
// 7. MY CALENDAR III — LeetCode 732
// ============================================================

class MyCalendarThree {
    map<int, int> timeline;
public:
    MyCalendarThree() {}

    int book(int start, int end) {
        timeline[start]++;
        timeline[end]--;

        int ongoing = 0, maxK = 0;
        for (auto& [time, count] : timeline) {
            ongoing += count;
            maxK = max(maxK, ongoing);
        }
        return maxK;
    }
};
// Interview Explanation:
// - Problem Statement: Return maximum K-booking (concurrent overlapping events) after each booking (LeetCode 732).
// - Approach: Sweep-Line Coordinate Difference Array with `std::map`.
// - Intuition: Running prefix sum over ordered map gives concurrent active events at every point.
// - Complexity: Time: O(N) per `book`, Space: O(N).


// ============================================================
// 8. CORPORATE FLIGHT BOOKINGS — LeetCode 1109
// ============================================================

vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> diff(n + 1, 0);
    for (auto& b : bookings) {
        int first = b[0] - 1, last = b[1] - 1, seats = b[2];
        diff[first] += seats;
        diff[last + 1] -= seats;
    }

    vector<int> answer(n);
    int current = 0;
    for (int i = 0; i < n; i++) {
        current += diff[i];
        answer[i] = current;
    }
    return answer;
}
// Interview Explanation:
// - Problem Statement: Return total seats booked on each flight 1 to n (LeetCode 1109).
// - Approach: 1D Range Difference Array + Prefix Sum Reconstruction.
// - Intuition: Apply range updates in O(1) with `+seats` at start and `-seats` at end+1; prefix sum reconstructs array.
// - Complexity: Time: O(N + Bookings), Space: O(N).


// ============================================================
// 9. RANGE MODULE — LeetCode 715
// ============================================================

class RangeModule {
    map<int, int> ranges;
public:
    RangeModule() {}

    void addRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second >= left) it = prev(it);

        while (it != ranges.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            it = ranges.erase(it);
        }
        ranges[left] = right;
    }

    bool queryRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it == ranges.begin()) return false;
        it = prev(it);
        return it->second >= right;
    }

    void removeRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second > left) it = prev(it);

        vector<pair<int, int>> toInsert;
        while (it != ranges.end() && it->first < right) {
            if (it->first < left) toInsert.push_back({it->first, left});
            if (it->second > right) toInsert.push_back({right, it->second});
            it = ranges.erase(it);
        }

        for (auto& [l, r] : toInsert) ranges[l] = r;
    }
};
// Interview Explanation:
// - Problem Statement: Design data structure to track numeric ranges with add, query, remove (LeetCode 715).
// - Approach: `std::map<int, int>` Disjoint Interval Slicing.
// - Intuition: Merge overlapping ranges on add; check enclosing interval on query; carve intervals on remove.
// - Complexity: Time: O(K log N) per op, Space: O(N).


// ============================================================
// 10. COUNT INTEGERS IN INTERVALS — LeetCode 2276
// ============================================================

class CountIntervals {
    map<int, int> mp;
    int totalCount;
public:
    CountIntervals() : totalCount(0) {}

    void add(int left, int right) {
        auto it = mp.upper_bound(left);
        if (it != mp.begin() && prev(it)->second >= left) it = prev(it);

        while (it != mp.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            totalCount -= (it->second - it->first + 1);
            it = mp.erase(it);
        }

        mp[left] = right;
        totalCount += (right - left + 1);
    }

    int count() {
        return totalCount;
    }
};
// Interview Explanation:
// - Problem Statement: Add intervals and query total count of distinct integers covered (LeetCode 2276).
// - Approach: `std::map` Disjoint Intervals with Running Integer Count.
// - Intuition: Deduct erased interval lengths and add newly merged unified length in O(log N) amortized.
// - Complexity: Time: Amortized O(log N) per `add`, O(1) for `count`, Space: O(N).


// ============================================================
// 11. REMOVE COVERED INTERVALS — LeetCode 1288
// ============================================================

int removeCoveredIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] != b[0]) return a[0] < b[0];
        return a[1] > b[1];
    });

    int count = 0;
    int maxEnd = 0;

    for (const auto& iv : intervals) {
        if (iv[1] > maxEnd) {
            count++;
            maxEnd = iv[1];
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Remove all intervals covered by another interval in the list (LeetCode 1288).
// - Approach: Custom Sorting (Start Ascending, End Descending) + Max End Frontier.
// - Intuition: Sorting guarantees `iv[0] >= prev[0]`. Thus `iv` is covered iff `iv[1] <= maxEnd`.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 12. MAXIMUM LENGTH OF PAIR CHAIN — LeetCode 646
// ============================================================

int findLongestChain(vector<vector<int>>& pairs) {
    sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });

    int chainLen = 1;
    int currEnd = pairs[0][1];

    for (int i = 1; i < (int)pairs.size(); i++) {
        if (pairs[i][0] > currEnd) {
            chainLen++;
            currEnd = pairs[i][1];
        }
    }
    return chainLen;
}
// Interview Explanation:
// - Problem Statement: Find longest chain of pairs (p1, p2) where p1[1] < p2[0] (LeetCode 646).
// - Approach: Greedy Interval Scheduling (Sort by Second Element).
// - Intuition: Picking pair finishing earliest leaves maximum room for remaining links.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 13. AMOUNT OF NEW AREA PAINTED EACH DAY — LeetCode 2158
// ============================================================

vector<int> amountPainted(vector<vector<int>>& paint) {
    int maxPos = 0;
    for (auto& p : paint) maxPos = max(maxPos, p[1]);

    vector<int> jump(maxPos + 1, 0);
    vector<int> ans(paint.size(), 0);

    for (int i = 0; i < (int)paint.size(); i++) {
        int start = paint[i][0], end = paint[i][1];
        int paintedToday = 0;

        int curr = start;
        while (curr < end) {
            if (jump[curr] == 0) {
                jump[curr] = max(jump[curr], end);
                paintedToday++;
                curr++;
            } else {
                int nxt = jump[curr];
                jump[curr] = max(jump[curr], end);
                curr = nxt;
            }
        }
        ans[i] = paintedToday;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Daily painting of intervals [start, end]; return new area painted each day (LeetCode 2158).
// - Approach: Jump Table / DSU Path Compression.
// - Intuition: `jump[x]` stores next unpainted point; compressed paths jump directly over painted segments in O(1) amortized.
// - Complexity: Time: O(N + MaxPosition), Space: O(MaxPosition).


// ============================================================
// 14. TEEMO ATTACKING — LeetCode 495
// ============================================================

int findPoisonedDuration(vector<int>& timeSeries, int duration) {
    if (timeSeries.empty()) return 0;
    int total = 0;

    for (int i = 1; i < (int)timeSeries.size(); i++) {
        total += min(duration, timeSeries[i] - timeSeries[i - 1]);
    }
    total += duration;
    return total;
}
// Interview Explanation:
// - Problem Statement: Return total poisoned time given attack timestamps and duration (LeetCode 495).
// - Approach: Linear Interval Overlap Accumulation.
// - Intuition: If gap >= duration, full duration is credited; otherwise elapsed delta is added.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 15. IN-PLACE MERGE SORT (GAP METHOD)
// ============================================================

void inPlaceMerge(vector<int>& arr, int l, int m, int r) {
    int len = r - l + 1;
    int gap = (len + 1) / 2;
    while (gap > 0) {
        int i = l;
        while (i + gap <= r) {
            int j = i + gap;
            if (arr[i] > arr[j]) {
                swap(arr[i], arr[j]);
            }
            i++;
        }
        gap = (gap > 1) ? (gap + 1) / 2 : 0;
    }
}

void inPlaceMergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    inPlaceMergeSort(arr, l, mid);
    inPlaceMergeSort(arr, mid + 1, r);
    inPlaceMerge(arr, l, mid, r);
}
// Interview Explanation:
// - Problem Statement: Merge two sorted adjacent subarrays in-place without extra memory.
// - Approach: Shell's Gap Reduction Method across boundary.
// - Intuition: Compare elements separated by `gap = ceil(len / 2)`. Halving gap sorts range in O(N log N) merge.
// - Complexity: Time: O(N log^2 N), Space: O(1).


// ============================================================
// 16. RADIX SORT MSD (RECURSIVE BUCKET SORT)
// ============================================================

void msdRadixSortHelper(vector<string>& arr, int lo, int hi, int d, vector<string>& aux) {
    if (hi <= lo || d >= 100) return;
    const int R = 256;
    vector<int> count(R + 2, 0);

    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        count[c + 1]++;
    }
    for (int r = 0; r < R + 1; r++) count[r + 1] += count[r];
    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        aux[lo + count[c]++] = arr[i];
    }
    for (int i = lo; i <= hi; i++) arr[i] = aux[i];

    for (int r = 0; r < R; r++) {
        msdRadixSortHelper(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
    }
}
// Interview Explanation:
// - Problem Statement: Sort variable-length strings lexicographically using Most Significant Digit (MSD) partitioning.
// - Approach: Recursive Trie-like MSD Bucket Partitioning.
// - Intuition: Partition into 256 character buckets based on d-th character; sort buckets recursively.
// - Complexity: Time: O(N * L), Space: O(N + R * Depth).


// ============================================================
// 17. PIGEONHOLE SORT (DIRECT RANGE SLOTS)
// ============================================================

void pigeonholeSort(vector<int>& arr) {
    if (arr.empty()) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    vector<vector<int>> holes(range);
    for (int x : arr) holes[x - minVal].push_back(x);

    int idx = 0;
    for (int i = 0; i < range; i++) {
        for (int x : holes[i]) arr[idx++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort integer array where value range R is close to array size N.
// - Approach: Direct Pigeonhole / Slot Mapping.
// - Intuition: Direct array mapping from key to bucket index `val - minVal`.
// - Complexity: Time: O(N + Range), Space: O(N + Range).


// ============================================================
// 18. PANCAKE SORTING — LeetCode 969
// ============================================================

vector<int> pancakeSort(vector<int>& arr) {
    vector<int> ans;
    int n = arr.size();
    for (int j = n; j > 1; j--) {
        int maxIdx = max_element(arr.begin(), arr.begin() + j) - arr.begin();
        if (maxIdx != j - 1) {
            if (maxIdx != 0) {
                ans.push_back(maxIdx + 1);
                reverse(arr.begin(), arr.begin() + maxIdx + 1);
            }
            ans.push_back(j);
            reverse(arr.begin(), arr.begin() + j);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort array using prefix reversals (pancake flips) with <= 2*N flips (LeetCode 969).
// - Approach: Greedy Suffix Placement (At most 2 flips per element).
// - Intuition: Flip max element in unsorted prefix to index 0, then flip whole range to place at end.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 19. PATIENCE SORTING & LIS (GREENE'S THEOREM)
// ============================================================

int patienceSortLISLength(const vector<int>& arr) {
    vector<int> piles;
    for (int x : arr) {
        auto it = lower_bound(piles.begin(), piles.end(), x);
        if (it == piles.end()) piles.push_back(x);
        else *it = x;
    }
    return piles.size();
}
// Interview Explanation:
// - Problem Statement: Determine LIS length using Patience Sorting card pile game.
// - Approach: Greedy Card Pile Placement + Binary Search (`lower_bound`).
// - Intuition: Number of piles formed in patience sorting equals LIS length (Greene's Theorem).
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 20. SORT MATRIX DIAGONALLY — LeetCode 1329
// ============================================================

vector<vector<int>> diagonalSort(vector<vector<int>>& mat) {
    int m = mat.size(), n = mat[0].size();
    unordered_map<int, vector<int>> diagonals;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            diagonals[i - j].push_back(mat[i][j]);
        }
    }
    for (auto& [key, vec] : diagonals) {
        sort(vec.rbegin(), vec.rend());
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = diagonals[i - j].back();
            diagonals[i - j].pop_back();
        }
    }
    return mat;
}
// Interview Explanation:
// - Problem Statement: Sort each diagonal of an m x n matrix ascending (LeetCode 1329).
// - Approach: Hash Map Diagonal Grouping with Key `i - j`.
// - Intuition: Cells on the same diagonal share invariant `i - j`. Group, sort, and write back.
// - Complexity: Time: O(M * N * log(min(M, N))), Space: O(M * N).


// ============================================================
// 21. SORT TRANSFORMED ARRAY — LeetCode 360
// ============================================================

vector<int> sortTransformedArray(vector<int>& nums, int a, int b, int c) {
    auto f = [&](int x) { return a * x * x + b * x + c; };
    int n = nums.size();
    vector<int> res(n);
    int i = 0, j = n - 1;

    if (a >= 0) {
        int k = n - 1;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 >= v2) { res[k--] = v1; i++; }
            else { res[k--] = v2; j--; }
        }
    } else {
        int k = 0;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 <= v2) { res[k++] = v1; i++; }
            else { res[k++] = v2; j--; }
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Given sorted array, apply f(x) = ax^2 + bx + c and return sorted array in O(N) (LC 360).
// - Approach: Parabola Concavity / Convexity + Two Pointers.
// - Intuition: If a >= 0 (convex), maximums are at outer endpoints; if a < 0 (concave), minimums are at endpoints.
// - Complexity: Time: O(N), Space: O(N).
