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
 | 1  | Merge Intervals (LeetCode 56)               | Start-Time Sorting + Running Max  | O(N logN)| O(N)     |
 | 2  | Insert Interval (LeetCode 57)               | 3-Stage Linear Range Merge Scan   | O(N)     | O(N)     |
 | 3  | Non-overlapping Intervals (LeetCode 435)    | Greedy End-Time Interval Schedule | O(N logN)| O(1)     |
 | 4  | Meeting Rooms I (LeetCode 252)              | Start-Time Sorting + Overlap Check| O(N logN)| O(1)     |
 | 5  | Meeting Rooms II (LeetCode 253)             | Min-Heap Active Rooms / Sweep Line| O(N logN)| O(N)     |
 | 6  | Meeting Rooms III (LeetCode 2402)           | Dual Min-Heaps (Free & Busy Rooms)| O(M logM)| O(N)     |
 | 7  | Interval List Intersections (LeetCode 986)  | Two-Pointer Boundary Intersection | O(N + M) | O(N + M) |
 | 8  | Min Arrows to Burst Balloons (LC 452)       | Greedy End-Coordinate Pinning     | O(N logN)| O(1)     |
 | 9  | Employee Free Time (LeetCode 759)           | Multi-Way Heap / Gap Extraction   | O(N logN)| O(N)     |
 | 10 | Data Stream Disjoint Intervals (LC 352)     | Balanced BST (std::set) Neighbor  | O(log N) | O(N)     |
 | 11 | My Calendar I (LeetCode 729)                | std::set lower_bound Boundary BST | O(log N) | O(N)     |
 | 12 | My Calendar II (LeetCode 731)               | Dual Overlap List Verification    | O(N)     | O(N)     |
 | 13 | My Calendar III (LeetCode 732)              | Sweep-Line Difference Map         | O(N)     | O(N)     |
 | 14 | Car Pooling (LeetCode 1094)                 | Timestamp Difference Array / Sweep| O(N+Range| O(Range) |
 | 15 | Corporate Flight Bookings (LeetCode 1109)   | 1D Range Difference Array + Prefix| O(N + F) | O(N)     |
 | 16 | Range Module (LeetCode 715)                 | std::map Disjoint Interval Slices | O(K logN)| O(N)     |
 | 17 | Count Integers in Intervals (LC 2276)       | std::map Dynamic Overlap Merging  | O(log N) | O(N)     |
 | 18 | Remove Covered Intervals (LeetCode 1288)    | Custom Sort (Start Asc, End Desc) | O(N logN)| O(1)     |
 | 19 | Video Stitching (LeetCode 1024)             | Greedy Jump Interval Max Reach    | O(N + T) | O(T)     |
 | 20 | Maximum Length of Pair Chain (LC 646)       | Greedy End-Time Selection (LIS DP)| O(N logN)| O(1)     |
 | 21 | Min Interval to Include Each Query (LC 1851)| Offline Sorted Queries + Min-Heap | O(NlogN+Q)| O(N + Q) |
 | 22 | Area Painted Each Day (LeetCode 2158)       | Jump-Table / DSU Path Compression | O(N+Range| O(Range) |
 | 23 | Partition Labels (LeetCode 763)             | Character Last Occurrence Interval| O(N)     | O(1)     |
 | 24 | Teemo Attacking (LeetCode 495)              | Timeline Overlap Accumulation     | O(N)     | O(1)     |
 | 25 | Task Scheduler (LeetCode 621)               | Idle Interval Slot Math           | O(N)     | O(1)     |
 ====================================================================================================
*/

// ============================================================
// 1. MERGE INTERVALS — LeetCode 56
// ============================================================

vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());

    vector<vector<int>> merged;
    merged.push_back(intervals[0]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (merged.back()[1] >= intervals[i][0]) {
            merged.back()[1] = max(merged.back()[1], intervals[i][1]);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}
// Interview Explanation:
// - Problem Statement: Given an array of intervals where intervals[i] = [start_i, end_i], merge all overlapping intervals and return non-overlapping intervals covering all input (LeetCode 56).
// - Approach: Sort by start time + Greedy running linear scan.
// - Intuition: Sorting by start times ensures overlapping intervals are contiguous. Maintain `merged` array; if `current.start <= last_merged.end`, merge by extending `last_merged.end = max(last_merged.end, current.end)`.
// - Complexity: Time: O(N log N), Space: O(N) for output.

// ============================================================
// 2. INSERT INTERVAL — LeetCode 57
// ============================================================

vector<vector<int>> insertInterval(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> result;
    int i = 0, n = intervals.size();

    // Step 1: Add all intervals ending before newInterval starts
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }

    // Step 2: Merge all overlapping intervals with newInterval
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);

    // Step 3: Add all remaining intervals starting after newInterval ends
    while (i < n) {
        result.push_back(intervals[i++]);
    }

    return result;
}
// Interview Explanation:
// - Problem Statement: Insert a new interval into a sorted list of non-overlapping intervals and merge if necessary (LeetCode 57).
// - Approach: 3-Stage Linear Range Scan.
// - Intuition: Pass through in three natural phases without re-sorting: strictly left intervals, overlapping intervals (accumulate min start & max end), and strictly right intervals.
// - Complexity: Time: O(N), Space: O(N) output array.

// ============================================================
// 3. NON-OVERLAPPING INTERVALS — LeetCode 435
// ============================================================

int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    // Sort by end time
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });

    int count = 1; // Maximum non-overlapping intervals we can keep
    int lastEnd = intervals[0][1];

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] >= lastEnd) {
            count++;
            lastEnd = intervals[i][1];
        }
    }
    return (int)intervals.size() - count;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of intervals you need to remove to make the rest non-overlapping (LeetCode 435).
// - Approach: Classic Greedy Interval Scheduling (Earliest Finish Time First).
// - Intuition: Maximizing kept non-overlapping intervals minimizes removed intervals. Always pick interval finishing earliest to leave maximal space for subsequent intervals.
// - Complexity: Time: O(N log N), Space: O(1).

// ============================================================
// 4. MEETING ROOMS I — LeetCode 252
// ============================================================

bool canAttendMeetings(vector<vector<int>>& intervals) {
    if (intervals.empty()) return true;
    sort(intervals.begin(), intervals.end());

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < intervals[i - 1][1]) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if a person could attend all meetings without overlap (LeetCode 252).
// - Approach: Start-Time Sorting + Pairwise Overlap Verification.
// - Intuition: Sort meetings by start time; if any meeting begins before the previous meeting finishes (`intervals[i][0] < intervals[i-1][1]`), a conflict exists.
// - Complexity: Time: O(N log N), Space: O(1).

// ============================================================
// 5. MEETING ROOMS II — LeetCode 253
// ============================================================

int minMeetingRooms(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end());

    // Min-heap tracking earliest ending meeting room
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(intervals[0][1]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] >= minHeap.top()) {
            minHeap.pop(); // Reuse room: meeting finished
        }
        minHeap.push(intervals[i][1]); // Allocate/extend room
    }

    return (int)minHeap.size();
}
// Interview Explanation:
// - Problem Statement: Find minimum number of conference rooms required to hold all meetings (LeetCode 253).
// - Approach: Min-Heap of Active Room End Times.
// - Intuition: Sort meetings by start time. When a meeting starts, check if earliest finishing room is free (`start >= minHeap.top()`). If so, reuse it (`pop()`); otherwise, allocate a new room. Size of heap is total rooms needed.
// - Complexity: Time: O(N log N), Space: O(N).

// ============================================================
// 6. MEETING ROOMS III — LeetCode 2402
// ============================================================

int mostBooked(int n, vector<vector<int>>& meetings) {
    sort(meetings.begin(), meetings.end());

    // available: min-heap of free room IDs [0...n-1]
    priority_queue<int, vector<int>, greater<int>> available;
    for (int i = 0; i < n; i++) available.push(i);

    // busy: min-heap of pairs {endTime, roomID}
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> busy;

    vector<int> count(n, 0);

    for (auto& m : meetings) {
        long long start = m[0], end = m[1], duration = end - start;

        // Release rooms that have finished before current meeting start
        while (!busy.empty() && busy.top().first <= start) {
            available.push(busy.top().second);
            busy.pop();
        }

        if (!available.empty()) {
            int room = available.top();
            available.pop();
            count[room]++;
            busy.push({end, room});
        } else {
            // Delay meeting until earliest room is free
            auto [earliestEnd, room] = busy.top();
            busy.pop();
            count[room]++;
            busy.push({earliestEnd + duration, room});
        }
    }

    int maxCount = 0, bestRoom = 0;
    for (int i = 0; i < n; i++) {
        if (count[i] > maxCount) {
            maxCount = count[i];
            bestRoom = i;
        }
    }
    return bestRoom;
}
// Interview Explanation:
// - Problem Statement: Given n rooms and meetings, allocate rooms with lowest ID first; if all busy, delay meeting until earliest free room. Return room that hosted most meetings (LeetCode 2402).
// - Approach: Dual Min-Heaps (Available Room IDs & Busy `{endTime, roomID}`).
// - Intuition: Free up finished rooms into `available`. If a room is free, assign lowest ID. If all busy, pick top of `busy`, delay finish time to `earliestEnd + duration`, and reinsert.
// - Complexity: Time: O(M log M + M log N), Space: O(N).

// ============================================================
// 7. INTERVAL LIST INTERSECTIONS — LeetCode 986
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
// 8. MINIMUM NUMBER OF ARROWS TO BURST BALLOONS — LeetCode 452
// ============================================================

int findMinArrowShots(vector<vector<int>>& points) {
    if (points.empty()) return 0;
    // Sort by end coordinate
    sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });

    int arrows = 1;
    int lastArrow = points[0][1];

    for (int i = 1; i < (int)points.size(); i++) {
        if (points[i][0] > lastArrow) {
            arrows++;
            lastArrow = points[i][1];
        }
    }
    return arrows;
}
// Interview Explanation:
// - Problem Statement: Minimum number of vertical arrows to burst all spherical balloons on X-axis (LeetCode 452).
// - Approach: Greedy Sort by End Coordinate.
// - Intuition: Shooting an arrow at the earliest balloon's end coordinate maximizes overlaps with other balloons starting before or at that point. If a balloon starts strictly after `lastArrow`, a new arrow is required.
// - Complexity: Time: O(N log N), Space: O(1).

// ============================================================
// 9. EMPLOYEE FREE TIME — LeetCode 759
// ============================================================

vector<vector<int>> employeeFreeTime(vector<vector<vector<int>>>& schedule) {
    vector<vector<int>> allIntervals;
    for (auto& emp : schedule) {
        for (auto& iv : emp) allIntervals.push_back(iv);
    }
    sort(allIntervals.begin(), allIntervals.end());

    vector<vector<int>> freeTime;
    int lastEnd = allIntervals[0][1];

    for (int i = 1; i < (int)allIntervals.size(); i++) {
        if (allIntervals[i][0] > lastEnd) {
            freeTime.push_back({lastEnd, allIntervals[i][0]});
        }
        lastEnd = max(lastEnd, allIntervals[i][1]);
    }
    return freeTime;
}
// Interview Explanation:
// - Problem Statement: Find common positive-length free time intervals for all employees (LeetCode 759).
// - Approach: Flatten & Sort + Active Gap Extraction.
// - Intuition: Collect all working intervals and sort by start time. Merge overlapping working intervals; whenever a gap occurs (`current.start > lastEnd`), that gap represents common free time.
// - Complexity: Time: O(N log N), Space: O(N).

// ============================================================
// 10. DATA STREAM AS DISJOINT INTERVALS — LeetCode 352
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
// 11. MY CALENDAR I — LeetCode 729
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
// 12. MY CALENDAR II — LeetCode 731
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
// 13. MY CALENDAR III — LeetCode 732
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
// 14. CAR POOLING — LeetCode 1094
// ============================================================

bool carPooling(vector<vector<int>>& trips, int capacity) {
    int timeline[1001] = {0};
    for (auto& t : trips) {
        int numPassengers = t[0], from = t[1], to = t[2];
        timeline[from] += numPassengers;
        timeline[to] -= numPassengers;
    }

    int current = 0;
    for (int i = 0; i <= 1000; i++) {
        current += timeline[i];
        if (current > capacity) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if vehicle with fixed capacity can complete all pick-up and drop-off trips (LeetCode 1094).
// - Approach: Timestamp Difference Array Sweep-Line.
// - Intuition: For each trip, add passengers at `from` and remove at `to`. Compute running sum; if capacity is exceeded at any point, return false.
// - Complexity: Time: O(N + MaxLocation), Space: O(MaxLocation) = O(1).

// ============================================================
// 15. CORPORATE FLIGHT BOOKINGS — LeetCode 1109
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
// 16. RANGE MODULE — LeetCode 715
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
// 17. COUNT INTEGERS IN INTERVALS — LeetCode 2276
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
// 18. REMOVE COVERED INTERVALS — LeetCode 1288
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
// 19. VIDEO STITCHING — LeetCode 1024
// ============================================================

int videoStitching(vector<vector<int>>& clips, int time) {
    vector<int> maxReach(time + 1, 0);
    for (auto& c : clips) {
        if (c[0] <= time) {
            maxReach[c[0]] = max(maxReach[c[0]], c[1]);
        }
    }

    int jumps = 0, currEnd = 0, nextEnd = 0;
    for (int i = 0; i < time; i++) {
        nextEnd = max(nextEnd, maxReach[i]);
        if (i == currEnd) {
            jumps++;
            currEnd = nextEnd;
            if (currEnd <= i) return -1; // Stuck cannot advance
        }
    }
    return currEnd >= time ? jumps : -1;
}
// Interview Explanation:
// - Problem Statement: Cut video clips to cover the interval [0, time] with the minimum number of clips (LeetCode 1024).
// - Approach: Greedy Interval Jump Game (Reach Horizon).
// - Intuition: Reduce to Jump Game II: `maxReach[t]` is max end reachable from start time `t`. At current horizon `currEnd`, pick clip that extends farthest (`nextEnd`).
// - Complexity: Time: O(N + Time), Space: O(Time).

// ============================================================
// 20. MAXIMUM LENGTH OF PAIR CHAIN — LeetCode 646
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
// 21. MINIMUM INTERVAL TO INCLUDE EACH QUERY — LeetCode 1851
// ============================================================

vector<int> minInterval(vector<vector<int>>& intervals, vector<int>& queries) {
    sort(intervals.begin(), intervals.end());

    int k = queries.size();
    vector<pair<int, int>> sortedQueries(k);
    for (int i = 0; i < k; i++) sortedQueries[i] = {queries[i], i};
    sort(sortedQueries.begin(), sortedQueries.end());

    // Min-heap storing pair: {interval_size, right_endpoint}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> ans(k, -1);
    int idx = 0, n = intervals.size();

    for (auto& [q, origIdx] : sortedQueries) {
        // Add all intervals starting <= q
        while (idx < n && intervals[idx][0] <= q) {
            int l = intervals[idx][0], r = intervals[idx][1];
            pq.push({r - l + 1, r});
            idx++;
        }

        // Remove outdated intervals ending < q
        while (!pq.empty() && pq.top().second < q) {
            pq.pop();
        }

        if (!pq.empty()) {
            ans[origIdx] = pq.top().first;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: For each query point q, find the minimum size (right - left + 1) of an interval containing q (LeetCode 1851).
// - Approach: Offline Query Sorting + Min-Heap.
// - Intuition: Sort queries and intervals. Push all intervals with `start <= q` into min-heap sorted by length. Lazily pop intervals whose `end < q`. Top of heap is guaranteed smallest valid containing interval.
// - Complexity: Time: O(N log N + Q log Q), Space: O(N + Q).

// ============================================================
// 22. AMOUNT OF NEW AREA PAINTED EACH DAY — LeetCode 2158
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
// 23. PARTITION LABELS — LeetCode 763
// ============================================================

vector<int> partitionLabels(string s) {
    int last[26] = {0};
    for (int i = 0; i < (int)s.size(); i++) {
        last[s[i] - 'a'] = i;
    }

    vector<int> partitions;
    int start = 0, end = 0;

    for (int i = 0; i < (int)s.size(); i++) {
        end = max(end, last[s[i] - 'a']);
        if (i == end) {
            partitions.push_back(end - start + 1);
            start = i + 1;
        }
    }
    return partitions;
}
// Interview Explanation:
// - Problem Statement: Partition string into maximum number of parts such that each letter appears in at most one part (LeetCode 763).
// - Approach: Interval Greedy with Character Last-Seen Indices.
// - Intuition: Record the last occurrence index of every character. Scan string while expanding partition boundary `end = max(end, last[c])`. When `i == end`, current partition is complete.
// - Complexity: Time: O(N), Space: O(1) auxiliary space (26 letters).

// ============================================================
// 24. TEEMO ATTACKING — LeetCode 495
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

// ============================================================
// 25. TASK SCHEDULER (IDLE INTERVAL SLOTS) — LeetCode 621
// ============================================================

int leastInterval(vector<char>& tasks, int n) {
    int freq[26] = {0};
    for (char c : tasks) freq[c - 'A']++;

    int maxFreq = *max_element(freq, freq + 26);
    int countMaxFreq = 0;
    for (int i = 0; i < 26; i++) {
        if (freq[i] == maxFreq) countMaxFreq++;
    }

    int intervalsNeeded = (maxFreq - 1) * (n + 1) + countMaxFreq;
    return max((int)tasks.size(), intervalsNeeded);
}
// Interview Explanation:
// - Problem Statement: Find minimum CPU time intervals required to execute all tasks with cooldown period n between identical tasks (LeetCode 621).
// - Approach: Frequency Counting + Idle Interval Slot Math.
// - Intuition: The task(s) with max frequency `maxFreq` define the skeletal framework of `(maxFreq - 1)` chunks of size `(n + 1)`. Remaining tasks fill empty slots; total intervals cannot be less than `tasks.size()`.
// - Complexity: Time: O(N), Space: O(1).
