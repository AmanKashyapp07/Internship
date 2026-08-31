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
 | 1  | Interval List Intersections (LeetCode 986)  | Two-Pointer Boundary Intersection | O(N + M) | O(N + M) |
 | 2  | Data Stream Disjoint Intervals (LC 352)     | Balanced BST (std::set) Neighbor  | O(log N) | O(N)     |
 | 3  | My Calendar I (LeetCode 729)                | std::set lower_bound Boundary BST | O(log N) | O(N)     |
 | 4  | My Calendar II (LeetCode 731)               | Dual Overlap List Verification    | O(N)     | O(N)     |
 | 5  | My Calendar III (LeetCode 732)              | Sweep-Line Difference Map         | O(N)     | O(N)     |
 | 6  | Corporate Flight Bookings (LeetCode 1109)   | 1D Range Difference Array + Prefix| O(N + F) | O(N)     |
 | 7  | Range Module (LeetCode 715)                 | std::map Disjoint Interval Slices | O(K logN)| O(N)     |
 | 8  | Count Integers in Intervals (LC 2276)       | std::map Dynamic Overlap Merging  | O(log N) | O(N)     |
 | 9  | Remove Covered Intervals (LeetCode 1288)    | Custom Sort (Start Asc, End Desc) | O(N logN)| O(1)     |
 | 10 | Maximum Length of Pair Chain (LC 646)       | Greedy End-Time Selection (LIS DP)| O(N logN)| O(1)     |
 | 11 | Area Painted Each Day (LeetCode 2158)       | Jump-Table / DSU Path Compression | O(N+Range| O(Range) |
 | 12 | Teemo Attacking (LeetCode 495)              | Timeline Overlap Accumulation     | O(N)     | O(1)     |
 ====================================================================================================
*/

// ============================================================
// 1. INTERVAL LIST INTERSECTIONS — LeetCode 986
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

        // Advance interval that finishes earlier
        if (firstList[i][1] < secondList[j][1]) i++;
        else j++;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Find intersections of two lists of closed intervals, where each list is pairwise disjoint and sorted (LeetCode 986).
// - Approach: Two-Pointer Sweep Intersection.
// - Intuition: Intersection interval is `[max(startA, startB), min(endA, endB)]`. Valid if `lo <= hi`. Always increment the pointer of the interval with smaller end time since it cannot intersect any future interval.
// - Complexity: Time: O(N + M), Space: O(N + M) output.


// ============================================================
// 2. DATA STREAM AS DISJOINT INTERVALS — LeetCode 352
// ============================================================

class SummaryRangesStream {
    set<pair<int, int>> intervals; // {start, end}
public:
    SummaryRangesStream() {}

    void addNum(int val) {
        auto it = intervals.lower_bound({val, val});
        int start = val, end = val;

        // Check merge with previous interval
        if (it != intervals.begin() && prev(it)->second >= val - 1) {
            it = prev(it);
        }

        // Merge all overlapping or contiguous intervals
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
// - Problem Statement: Maintain a summary of non-overlapping intervals as integers are dynamically added from a data stream (LeetCode 352).
// - Approach: Balanced BST (`std::set<pair<int, int>>`) Disjoint Interval Merging.
// - Intuition: When inserting `val`, check predecessor for `prev.end >= val - 1` and successor for `succ.start <= val + 1`. Erase all merged fragments and insert unified range `[start, end]`.
// - Complexity: Time: O(log N) amortized per `addNum`, O(N) for `getIntervals`, Space: O(N).


// ============================================================
// 3. MY CALENDAR I — LeetCode 729
// ============================================================

class MyCalendar {
    set<pair<int, int>> bookings; // {start, end}
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
// - Problem Statement: Implement calendar booking such that no two events have non-empty intersection (LeetCode 729).
// - Approach: BST `std::set` Binary Search (`lower_bound`).
// - Intuition: Find the first event starting >= `start`. Conflict exists if `next.start < end` or `prev.end > start`. If neither conflicts, insert in O(log N).
// - Complexity: Time: O(log N) per `book`, Space: O(N).


// ============================================================
// 4. MY CALENDAR II — LeetCode 731
// ============================================================

class MyCalendarTwo {
    vector<pair<int, int>> bookings;
    vector<pair<int, int>> overlaps; // Double booked intervals
public:
    MyCalendarTwo() {}

    bool book(int start, int end) {
        // Check triple booking against existing double bookings
        for (auto& [s, e] : overlaps) {
            if (max(start, s) < min(end, e)) return false;
        }

        // Add new overlaps generated by this booking
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
// - Intuition: A triple booking occurs if the new interval intersects any interval in `overlaps`. If safe, compute intersections with all existing `bookings` and store in `overlaps`.
// - Complexity: Time: O(N) per `book`, Space: O(N).


// ============================================================
// 5. MY CALENDAR III — LeetCode 732
// ============================================================

class MyCalendarThree {
    map<int, int> timeline; // Difference map
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
// - Problem Statement: Return the maximum K-booking (maximum concurrent overlapping events) after each new event (LeetCode 732).
// - Approach: Sweep-Line Coordinate Difference Array with `std::map`.
// - Intuition: Increment counter at `start` and decrement at `end`. Running prefix sum over ordered map gives concurrent active events at every boundary point; return maximum observed.
// - Complexity: Time: O(N) per `book`, Space: O(N).


// ============================================================
// 6. CORPORATE FLIGHT BOOKINGS — LeetCode 1109
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
// - Problem Statement: Given flight ranges [first, last] and booked seats, return total seats booked on each flight 1 to n (LeetCode 1109).
// - Approach: 1D Range Difference Array + Prefix Sum Reconstruction.
// - Intuition: Apply range updates in O(1) by marking `+seats` at `first` and `-seats` at `last + 1`. A single prefix sum pass computes the final totals in O(N).
// - Complexity: Time: O(N + Bookings), Space: O(N).


// ============================================================
// 7. RANGE MODULE — LeetCode 715
// ============================================================

class RangeModule {
    map<int, int> ranges; // {left, right} disjoint intervals
public:
    RangeModule() {}

    void addRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second >= left) {
            it = prev(it);
        }

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
        if (it != ranges.begin() && prev(it)->second > left) {
            it = prev(it);
        }

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
// - Problem Statement: Design data structure to track ranges of numbers with `addRange`, `queryRange`, and `removeRange` (LeetCode 715).
// - Approach: `std::map<int, int>` Disjoint Interval Slicing.
// - Intuition: Store disjoint ranges in ordered map. In `addRange`, merge overlapping and contiguous intervals. In `queryRange`, verify enclosing interval. In `removeRange`, delete overlapping parts and re-insert boundary remainders.
// - Complexity: Time: O(K log N) per op where K is overlapping ranges, Space: O(N).


// ============================================================
// 8. COUNT INTEGERS IN INTERVALS — LeetCode 2276
// ============================================================

class CountIntervals {
    map<int, int> mp; // {left, right}
    int totalCount;
public:
    CountIntervals() : totalCount(0) {}

    void add(int left, int right) {
        auto it = mp.upper_bound(left);
        if (it != mp.begin() && prev(it)->second >= left) {
            it = prev(it);
        }

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
// - Problem Statement: Dynamically add intervals and query total number of distinct integers present across all intervals (LeetCode 2276).
// - Approach: `std::map` Disjoint Intervals with Running Integer Count.
// - Intuition: When inserting `[left, right]`, merge overlapping intervals in `std::map`, subtract removed lengths from `totalCount`, insert unified interval, and add its length.
// - Complexity: Time: Amortized O(log N) per `add`, O(1) for `count`, Space: O(N).


// ============================================================
// 9. REMOVE COVERED INTERVALS — LeetCode 1288
// ============================================================

int removeCoveredIntervals(vector<vector<int>>& intervals) {
    // Sort by start ascending, end descending
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
// - Problem Statement: Remove all intervals that are covered by another interval in the list (LeetCode 1288).
// - Approach: Custom Sorting (Start Ascending, End Descending) + Max End Frontier.
// - Intuition: Sorting guarantees `iv[0] >= prev[0]`. Thus `iv` is covered if and only if `iv[1] <= maxEnd`. If `iv[1] > maxEnd`, it is NOT covered, so increment count and update `maxEnd`.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 10. MAXIMUM LENGTH OF PAIR CHAIN — LeetCode 646
// ============================================================

int findLongestChain(vector<vector<int>>& pairs) {
    // Greedy sort by second element (end time)
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
// - Problem Statement: Find the longest chain of pairs (p1, p2) where p1[1] < p2[0] (LeetCode 646).
// - Approach: Greedy Interval Scheduling (Sort by Second Element).
// - Intuition: Equivalent to Interval Scheduling problem. Picking the pair that finishes earliest leaves the maximum room for remaining chain links.
// - Complexity: Time: O(N log N), Space: O(1).


// ============================================================
// 11. AMOUNT OF NEW AREA PAINTED EACH DAY — LeetCode 2158
// ============================================================

vector<int> amountPainted(vector<vector<int>>& paint) {
    int maxPos = 0;
    for (auto& p : paint) maxPos = max(maxPos, p[1]);

    // jump[x] stores the farthest painted point reachable from x
    vector<int> jump(maxPos + 1, 0);
    vector<int> ans(paint.size(), 0);

    for (int i = 0; i < (int)paint.size(); i++) {
        int start = paint[i][0], end = paint[i][1];
        int paintedToday = 0;

        int curr = start;
        while (curr < end) {
            if (jump[curr] == 0) {
                // Point unpainted
                jump[curr] = max(jump[curr], end);
                paintedToday++;
                curr++;
            } else {
                // Already painted; jump to end of previously painted segment
                int nxt = jump[curr];
                jump[curr] = max(jump[curr], end); // Path compression
                curr = nxt;
            }
        }
        ans[i] = paintedToday;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Daily painting of intervals [start, end]; return the amount of new area painted on each day (LeetCode 2158).
// - Approach: Jump Table / DSU Path Compression.
// - Intuition: Maintain `jump[x]` pointing to the next unpainted position after `x`. When re-encountering painted coordinates, jump directly over them in O(1) amortized time.
// - Complexity: Time: O(N + MaxPosition), Space: O(MaxPosition).


// ============================================================
// 12. TEEMO ATTACKING — LeetCode 495
// ============================================================

int findPoisonedDuration(vector<int>& timeSeries, int duration) {
    if (timeSeries.empty()) return 0;
    int total = 0;

    for (int i = 1; i < (int)timeSeries.size(); i++) {
        total += min(duration, timeSeries[i] - timeSeries[i - 1]);
    }
    total += duration; // Add duration for last attack
    return total;
}
// Interview Explanation:
// - Problem Statement: Attacks poison a target for duration seconds; consecutive attacks reset poisoning timer. Return total poisoned time (LeetCode 495).
// - Approach: Linear Interval Overlap Accumulation.
// - Intuition: If gap `timeSeries[i] - timeSeries[i-1] >= duration`, full `duration` is credited. Otherwise, only elapsed time `timeSeries[i] - timeSeries[i-1]` is added.
// - Complexity: Time: O(N), Space: O(1).
