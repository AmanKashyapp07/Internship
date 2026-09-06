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

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                     PROBLEM SUMMARY & COMPLEXITY TABLE: G1.CPP (Problems 1 - 22)                    
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Best Time to Buy and Sell Stock II          | Greedy Valley-to-Peak Accumulation | O(N)     | O(1)     |
 | 2  | Jump Game                                   | Greedy Farthest Reachable Index   | O(N)     | O(1)     |
 | 3  | Candy                                       | Two-Pass Left & Right Sweeps      | O(N)     | O(N)     |
 | 4  | Insert Interval                             | 3-Stage Linear Sweep              | O(N)     | O(1)     |
 | 5  | Non-overlapping Intervals                   | Interval Scheduling by End Time   | O(N logN) | O(1)     |
 | 6  | Meeting Rooms                               | Interval Start-End Overlap Check  | O(N logN) | O(1)     |
 | 7  | Meeting Rooms II                            | Chronological Event Line-Sweep    | O(N logN) | O(N)     |
 | 8  | Min Number Arrows to Burst Balloons         | Interval Scheduling by End Point  | O(N logN) | O(1)     |
 | 9  | Interval List Intersections                 | Two-Pointer Boundary Overlap      | O(N + M) | O(1)     |
 | 10 | Car Pooling                                 | Difference Array Sweep-Line       | O(N + S) | O(S)     |
 | 11 | Corporate Flight Bookings                   | Difference Array Prefix Sum       | O(N + B) | O(N)     |
 | 12 | My Calendar I                               | Ordered Map Adjacent Intersection | O(N logN) | O(N)     |
 | 13 | My Calendar II                              | Double-Booking Conflict Tracking  | O(N^2)   | O(N)     |
 | 14 | My Calendar III                             | Line Sweep Point Concurrency      | O(N^2)   | O(N)     |
 | 15 | Minimum Number of Platforms                 | Chronological Arrival/Departure   | O(N logN) | O(1)     |
 | 16 | Job Sequencing Problem                      | Profit-Descending Slot Allocation | O(N*Dead) | O(Dead)  |
 | 17 | Course Schedule III                         | Greedy Duration Swap Max-Heap     | O(N logN) | O(N)     |
 | 18 | Two City Scheduling                         | Cost Delta Sorting Partition      | O(N logN) | O(1)     |
 | 19 | Maximum Performance of a Team               | Efficiency Sweep + Min-Heap Speed | O(N logN) | O(N + K) |
 | 20 | IPO                                         | Capital-Sorted Scan + Max-Heap W  | O(N logN) | O(N)     |
 | 21 | Single-Threaded CPU                         | Enqueue Sweep + Min-Heap Shortest | O(N logN) | O(N)     |
 | 22 | Process Tasks Using Servers                 | Dual Priority Queues (Free & Busy) | O(M logN) | O(N)     |
 ====================================================================================================
*/

// =========================================================
// 1. BEST TIME TO BUY AND SELL STOCK II [G-1]
// =========================================================

int maxProfit(vector<int>& p) {
    int ans = 0;
    for (int i = 1; i < (int)p.size(); ++i)
        ans += max(0, p[i] - p[i - 1]);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum total profit from buying and selling stock multiple times with at most 1 share held at once.
// - Approach: Greedy Valley-to-Peak Accumulation.
// - Intuition:
//   * Any multi-day price increase (p[k] - p[i]) decomposes into consecutive single-day increases: (p[i+1]-p[i]) + ... + (p[k]-p[k-1]).
//   * Capturing every positive single-day slope guarantees collecting the maximum profit across all upward trends.
//   * Skip negative slopes entirely by holding no position across declining days.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 2. JUMP GAME [G-2]
// =========================================================

bool canJump(vector<int>& a) {
    int mx = 0, n = a.size();
    for (int i = 0; i < n; ++i) {
        if (i > mx) return false;
        mx = max(mx, i + a[i]);
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if index n - 1 is reachable from index 0 where a[i] is max jump length.
// - Approach: Greedy Farthest Reachable Index Tracking.
// - Intuition:
//   * At any index i, index i is reachable if and only if i <= mx (the farthest reach discovered so far).
//   * If i > mx, we hit an impassable barrier; return false immediately.
//   * Update farthest reachable boundary: mx = max(mx, i + a[i]).
//   * If mx extends to or beyond n - 1, destination is reachable; return true early.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 3. CANDY [G-3]
// =========================================================

int candy(vector<int>& r) {
    int n = r.size();
    vector<int> c(n, 1);
    for (int i = 1; i < n; ++i)
        if (r[i] > r[i - 1]) c[i] = c[i - 1] + 1;
    for (int i = n - 2; i >= 0; --i)
        if (r[i] > r[i + 1]) c[i] = max(c[i], c[i + 1] + 1);
    return accumulate(c.begin(), c.end(), 0);
}
// Interview Explanation:
// - Problem Statement: Distribute minimum candies such that each child has at least 1, and higher-rated children get more than neighbors.
// - Approach: Two-Pass Greedy Left & Right Sweeps.
// - Intuition:
//   * Decompose two-sided neighbor constraints into two independent directional passes.
//   * Pass 1 (Left to Right): If rating r[i] > r[i-1], child i gets c[i-1] + 1 candies; ensures left neighbor constraint.
//   * Pass 2 (Right to Left): If rating r[i] > r[i+1], child i needs at least c[i+1] + 1 candies; take max(c[i], c[i+1] + 1).
//   * Taking the pointwise maximum satisfies both left and right constraints simultaneously with minimal total candies.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 4. INSERT INTERVAL [G-4]
// =========================================================

vector<vector<int>> insert(vector<vector<int>>& a, vector<int>& nw) {
    vector<vector<int>> ans;
    int i = 0, n = a.size();
    while (i < n && a[i][1] < nw[0]) ans.push_back(a[i++]);
    while (i < n && a[i][0] <= nw[1]) {
        nw[0] = min(nw[0], a[i][0]);
        nw[1] = max(nw[1], a[i][1]);
        i++;
    }
    ans.push_back(nw);
    while (i < n) ans.push_back(a[i++]);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into sorted non-overlapping intervals, merging if necessary.
// - Approach: 3-Stage Linear Sweep without re-sorting.
// - Intuition:
//   * Stage 1: Collect all intervals strictly preceding nw (a[i][1] < nw[0]).
//   * Stage 2: Merge overlapping intervals (a[i][0] <= nw[1]) by updating nw[0] = min and nw[1] = max.
//   * Push the merged interval nw.
//   * Stage 3: Collect all intervals strictly following nw.
//   * Achieves O(N) time by exploiting existing sorted order.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 5. NON-OVERLAPPING INTERVALS [G-5]
// =========================================================

int eraseOverlapIntervals(vector<vector<int>>& a) {
    sort(a.begin(), a.end(), [](auto& x, auto& y) { return x[1] < y[1]; });
    int ans = 0, prev = INT_MIN;
    for (auto& v : a) {
        if (v[0] >= prev) prev = v[1];
        else ans++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum intervals to remove to make remaining intervals non-overlapping.
// - Approach: Greedy Interval Scheduling by Earliest End Time.
// - Intuition:
//   * Minimizing removals is equivalent to maximizing the count of mutually compatible intervals.
//   * Classic activity selection: sort intervals by end time in ascending order.
//   * Greedily retain the interval ending earliest to leave maximal space for subsequent intervals.
//   * If next interval starts after or at prevEnd, keep it and advance prevEnd.
//   * Result is total_intervals - kept.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 6. MEETING ROOMS [G-6]
// =========================================================

bool canAttendMeetings(vector<vector<int>>& a) {
    sort(a.begin(), a.end());
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] < a[i - 1][1]) {
            return false;
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if person can attend all meetings without overlap.
// - Approach: Sort by Start Time and Check Adjacencies.
// - Intuition:
//   * Sort meetings in chronological order by start time.
//   * If any meeting starts strictly before the previous meeting finishes (a[i][0] < a[i-1][1]), a conflict exists.
//   * If no adjacent overlaps occur throughout the sorted sequence, all meetings can be attended.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 7. MEETING ROOMS II [G-7]
// =========================================================
int minMeetingRooms(vector<vector<int>>& a) {
    sort(a.begin(), a.end());
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto& m : a) {
        if (!pq.empty() && pq.top() <= m[0]) pq.pop();
        pq.push(m[1]);
    }
    return pq.size();
}
// Interview Explanation:
// - Problem Statement: Find minimum meeting rooms required to accommodate all intervals.
// - Approach: Chronological Two-Pointer Sweep on Separate Starts and Ends.
// - Intuition:
//   * A room is needed whenever a meeting starts before the earliest finishing meeting concludes.
//   * Decouple start and end timestamps into two sorted arrays.
//   * When starts[i] < ends[j], a new meeting begins before any existing room empties; increment rooms and advance i.
//   * When starts[i] >= ends[j], an active meeting ends; decrement rooms and advance j.
//   * The maximum concurrency across time gives the minimum rooms required.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 8. MINIMUM NUMBER OF ARROWS TO BURST BALLOONS [G-8]
// =========================================================

int findMinArrowShots(vector<vector<int>>& a) {
    if (a.empty()) return 0;
    sort(a.begin(), a.end(), [](auto& x, auto& y) { return x[1] < y[1]; });
    int ans = 1, end = a[0][1];
    for (auto& b : a) {
        if (b[0] > end) {
            ans++;
            end = b[1];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum arrows fired perpendicularly to burst all balloon intervals.
// - Approach: Interval Scheduling by Earliest End Coordinate.
// - Intuition:
//   * Sort balloons by their end coordinate in ascending order.
//   * Greedily place the arrow at the end coordinate of the earliest-finishing unburst balloon.
//   * Any subsequent balloon starting before or at this arrow coordinate is simultaneously burst for free.
//   * When a balloon starts strictly after curEnd, a new arrow is unavoidable; fire at its end coordinate.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 9. INTERVAL LIST INTERSECTIONS [G-9]
// =========================================================

vector<vector<int>> intervalIntersection(vector<vector<int>>& a, vector<vector<int>>& b) {
    vector<vector<int>> ans;
    int i = 0, j = 0;
    while (i < (int)a.size() && j < (int)b.size()) {
        int lo = max(a[i][0], b[j][0]), hi = min(a[i][1], b[j][1]);
        if (lo <= hi) ans.push_back({lo, hi});
        if (a[i][1] < b[j][1]) i++;
        else j++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the intersection of two closed sorted disjoint interval lists.
// - Approach: Two Pointers with Endpoint Comparisons.
// - Intuition:
//   * An intersection between a[i] and b[j] exists iff max(start_a, start_b) <= min(end_a, end_b).
//   * If valid, append the intersecting segment [lo, hi] to ans.
//   * Advance the pointer of the interval that ends first, because it cannot overlap with any subsequent intervals.
//   * Disjointness guarantees each interval is advanced at most once.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.


// =========================================================
// 10. CAR POOLING [G-10]
// =========================================================

bool carPooling(vector<vector<int>>& trips, int capacity) {
    int diff[1001] = {0};
    for (auto& t : trips) {
        diff[t[1]] += t[0];
        diff[t[2]] -= t[0];
    }
    for (int x : diff) {
        capacity -= x;
        if (capacity < 0) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if car can complete all trips without exceeding passenger capacity.
// - Approach: Difference Array / Chronological Line Sweep.
// - Intuition:
//   * For each trip, passenger count changes at discrete locations: +passengers at 'from', -passengers at 'to'.
//   * Apply difference array updates: diff[from] += num, diff[to] -= num.
//   * Accumulate running sum across all stops 0 to 1000.
//   * If the passenger count exceeds capacity at any stop, return false immediately.
// - Complexity: Time: O(N + S) where S = 1001 stops, Space: O(S) = O(1).


// =========================================================
// 11. CORPORATE FLIGHT BOOKINGS [G-11]
// =========================================================

vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> ans(n);
    for (auto& b : bookings) {
        ans[b[0] - 1] += b[2];
        if (b[1] < n) ans[b[1]] -= b[2];
    }
    for (int i = 1; i < n; ++i) ans[i] += ans[i - 1];
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return total seats reserved for each of n flights given interval bookings.
// - Approach: 1D Difference Array with Prefix Sum Accumulation.
// - Intuition:
//   * Directly updating intervals takes O(N * B) time, leading to TLE.
//   * A reservation of k seats on flights [first, last] adds +k at (first - 1) and -k at last.
//   * Compute running prefix sum across the difference array in a single O(N) pass.
//   * Directly yields the net seats booked on each flight in O(N + B) time.
// - Complexity: Time: O(N + B), Space: O(N).


// =========================================================
// 12. MY CALENDAR I [G-12]
// =========================================================

class MyCalendar {
    map<int, int> mp; // start -> end

public:
    bool book(int start, int end) {
        auto it = mp.upper_bound(start);
        // Overlap with next interval
        if (it != mp.end() && it->first < end) return false;
        // Overlap with previous interval
        if (it != mp.begin() && prev(it)->second > start) return false;
        mp[start] = end;
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Book events without double booking using dynamic interval insertions.
// - Approach: Balanced Binary Search Tree (std::map) Neighbor Verification.
// - Intuition:
//   * Existing booked events are kept sorted by start time in std::map.
//   * On book(start, end), binary search for the first event starting strictly after start using upper_bound.
//   * Check conflict with successor: if it->start < end, overlap exists.
//   * Check conflict with predecessor: if prev(it)->end > start, overlap exists.
//   * If both adjacent checks pass, insert {start, end} in O(log N) time.
// - Complexity: Time: O(log N) per booking, Space: O(N).


// =========================================================
// 13. MY CALENDAR II [G-13]
// =========================================================

class MyCalendarTwo {
    vector<pair<int,int>> booked, overlap;

public:
    bool book(int start, int end) {
        // Triple booking check
        for (auto [s, e] : overlap)
            if (max(start, s) < min(end, e))
                return false;

        // Create new double-booked intervals
        for (auto [s, e] : booked)
            if (max(start, s) < min(end, e))
                overlap.push_back({max(start, s), min(end, e)});

        booked.push_back({start, end});
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Book events without triple bookings (at most 2 overlapping events allowed).
// - Approach: Explicit Overlap Tracking List.
// - Intuition:
//   * A triple booking occurs if the new event intersects any interval that is already doubly booked.
//   * Maintain a list of double bookings overlaps.
//   * If [start, end) intersects any interval in overlaps (max(start, s) < min(end, e)), reject booking.
//   * Otherwise, intersect [start, end) with all single bookings to generate new double bookings.
//   * Append the new event to single bookings list.
// - Complexity: Time: O(N) per booking, Space: O(N).


// =========================================================
// 14. MY CALENDAR III [G-14]
// =========================================================

class MyCalendarThree {
    map<int, int> mp;

public:
    int book(int start, int end) {
        mp[start]++;
        mp[end]--;

        int cur = 0, ans = 0;

        // Prefix sum = active events
        for (auto [time, delta] : mp) {
            cur += delta;
            ans = max(ans, cur);
        }

        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum k-booking (maximum overlapping intervals) after each booking.
// - Approach: Boundary Line Sweep on Ordered Map.
// - Intuition:
//   * Each event [start, end) increases concurrency by +1 at start and decreases it by -1 at end.
//   * Insert boundary delta points into an ordered map: mp[start]++, mp[end]--.
//   * Traverse map chronologically: running prefix sum calculates concurrent active events at every timestamp.
//   * Return maximum concurrency observed.
// - Complexity: Time: O(N) per booking, Space: O(N).


// =========================================================
// 15. MINIMUM NUMBER OF PLATFORMS [G-15]
// =========================================================

int findPlatform(vector<int>& arr, vector<int>& dep) {
    sort(arr.begin(), arr.end());
    sort(dep.begin(), dep.end());

    int n = arr.size();
    int plat = 0, maxPlat = 0;
    int i = 0, j = 0;

    while (i < n) {
        if (arr[i] <= dep[j]) {
            plat++;
            i++;
        } else {
            plat--;
            j++;
        }
        maxPlat = max(maxPlat, plat);
    }
    return maxPlat;
}
// Interview Explanation:
// - Problem Statement: Find minimum train platforms needed so no train waits.
// - Approach: Chronological Two-Pointer Sweep on Arrival and Departure.
// - Intuition:
//   * A platform is required whenever a train arrives before an earlier arrived train departs.
//   * Sort arrival and departure arrays independently.
//   * When arr[i] <= dep[j], a train arrives before a platform is freed; increment plat and advance i.
//   * When arr[i] > dep[j], a platform is vacated; decrement plat and advance j.
//   * The maximum simultaneous platform count represents the answer.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 16. JOB SEQUENCING PROBLEM [G-16]
// =========================================================

struct Job {
    int id, dead, profit;
};

pair<int, int> JobScheduling(vector<Job>& a) {
    sort(a.begin(), a.end(), [](auto& x, auto& y) { return x.profit > y.profit; });
    int maxDead = 0;
    for (auto& j : a) maxDead = max(maxDead, j.dead);
    vector<int> slot(maxDead + 1, -1);
    int count = 0, totalProfit = 0;
    for (auto& j : a) {
        for (int d = j.dead; d > 0; --d) {
            if (slot[d] == -1) {
                slot[d] = j.id;
                count++;
                totalProfit += j.profit;
                break;
            }
        }
    }
    return {count, totalProfit};
}
// Interview Explanation:
// - Problem Statement: Schedule jobs to maximize total profit where each job takes 1 unit of time and has a deadline.
// - Approach: Greedy Profit-Descending Slot Allocation.
// - Intuition:
//   * High-profit jobs should be scheduled first to guarantee their inclusion.
//   * Sort jobs in descending order of profit.
//   * For each job, greedily assign it to the latest possible vacant time slot <= its deadline.
//   * Delaying execution as close to the deadline as possible preserves earlier time slots for jobs with tighter deadlines.
// - Complexity: Time: O(N log N + N * MaxDeadline), Space: O(MaxDeadline).


// =========================================================
// 17. COURSE SCHEDULE III [G-17]
// =========================================================

int scheduleCourse(vector<vector<int>>& courses) {
    sort(courses.begin(), courses.end(), [](auto& a, auto& b) { return a[1] < b[1]; });
    priority_queue<int> pq;
    int cur = 0;
    for (auto& c : courses) {
        cur += c[0];
        pq.push(c[0]);
        if (cur > c[1]) {
            cur -= pq.top();
            pq.pop();
        }
    }
    return pq.size();
}
// Interview Explanation:
// - Problem Statement: Find maximum courses you can take given [duration, lastDay].
// - Approach: Greedy Deadline Sort + Max-Heap Duration Replacement.
// - Intuition:
//   * Sort courses by deadline lastDay. Courses with tighter deadlines must be resolved earlier.
//   * Maintain a max-heap of durations for currently scheduled courses.
//   * If adding current course fits within lastDay, take it.
//   * If it exceeds lastDay, compare its duration with the longest scheduled course (pq.top()).
//   * If current duration is shorter, swap it in: total course count stays same, but curTime decreases, leaving more headroom for future courses!
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 18. TWO CITY SCHEDULING [G-18]
// =========================================================

int twoCitySchedCost(vector<vector<int>>& costs) {
    sort(costs.begin(), costs.end(), [](auto& a, auto& b) {
        return a[0] - a[1] < b[0] - b[1];
    });
    int total = 0, n = costs.size() / 2;
    for (int i = 0; i < n; ++i)
        total += costs[i][0] + costs[i + n][1];
    return total;
}
// Interview Explanation:
// - Problem Statement: Send exactly N people to city A and N people to city B minimizing total cost.
// - Approach: Cost Delta Sorting Partition.
// - Intuition:
//   * Sending a person to A instead of B provides a relative savings of (costA - costB).
//   * Sort people in ascending order of (costA - costB).
//   * The first N people have the smallest (most negative) delta; sending them to city A saves the most money.
//   * The remaining N people are assigned to city B.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 19. MAXIMUM PERFORMANCE OF A TEAM [G-19]
// =========================================================

int maxPerformance(int n, vector<int>& speed, vector<int>& efficiency, int k) {
    vector<pair<int, int>> eng(n);
    for (int i = 0; i < n; ++i) eng[i] = {efficiency[i], speed[i]};
    sort(eng.rbegin(), eng.rend());

    priority_queue<int, vector<int>, greater<int>> pq;
    long long sum = 0, ans = 0;
    for (auto [eff, spd] : eng) {
        pq.push(spd);
        sum += spd;
        if ((int)pq.size() > k) {
            sum -= pq.top();
            pq.pop();
        }
        ans = max(ans, sum * eff);
    }
    return ans % (int)(1e9 + 7);
}
// Interview Explanation:
// - Problem Statement: Choose at most k engineers maximizing (sum(speed) * min(efficiency)).
// - Approach: Efficiency-Descending Sweep with Min-Heap Speed Window.
// - Intuition:
//   * The bottleneck multiplier is min(efficiency).
//   * Sort engineers in descending order of efficiency.
//   * When considering engineer i, their efficiency is guaranteed to be the minimum of any engineer seen so far.
//   * To maximize the product, maintain the k largest speeds among engineers processed so far using a min-heap.
//   * If heap size exceeds k, evict the smallest speed.
// - Complexity: Time: O(N log N + N log K), Space: O(N + K).


// =========================================================
// 20. IPO [G-20]
// =========================================================

int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
    int n = profits.size();
    vector<pair<int, int>> p(n);
    for (int i = 0; i < n; ++i) p[i] = {capital[i], profits[i]};
    sort(p.begin(), p.end());

    priority_queue<int> pq;
    int i = 0;
    while (k--) {
        while (i < n && p[i].first <= w) pq.push(p[i++].second);
        if (pq.empty()) break;
        w += pq.top();
        pq.pop();
    }
    return w;
}
// Interview Explanation:
// - Problem Statement: Start with capital w, choose at most k distinct projects maximizing final capital.
// - Approach: Capital-Sorted Scan + Max-Heap of Available Profits.
// - Intuition:
//   * Sort projects in ascending order of required initial capital.
//   * At any point, all projects with required capital <= current capital w can be undertaken.
//   * Push profits of all newly affordable projects into a max-heap.
//   * Greedily undertake the project yielding the maximum profit (pq.top()), adding its profit to w.
//   * Repeat for up to k rounds or until no affordable projects remain.
// - Complexity: Time: O(N log N + K log N), Space: O(N).


// =========================================================
// 21. SINGLE-THREADED CPU [G-21]
// =========================================================

vector<int> getOrder(vector<vector<int>>& tasks) {
    int n = tasks.size();
    vector<array<int, 3>> a(n); // {enqueueTime, processingTime, index}
    for (int i = 0; i < n; ++i) a[i] = {tasks[i][0], tasks[i][1], i};
    sort(a.begin(), a.end());

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> ans;
    long long time = 0;
    int i = 0;

    while (i < n || !pq.empty()) {
        if (pq.empty() && time < a[i][0]) time = a[i][0];
        while (i < n && a[i][0] <= time) {
            pq.push({a[i][1], a[i][2]});
            i++;
        }
        auto [proc, idx] = pq.top();
        pq.pop();
        time += proc;
        ans.push_back(idx);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Simulate single-threaded CPU processing tasks by shortest processing time, breaking ties by smaller index.
// - Approach: Event-Time Sweep with Priority Queue.
// - Intuition:
//   * Tag each task with its original index and sort tasks ascending by enqueueTime.
//   * Maintain a min-heap prioritized by {processingTime, originalIndex}.
//   * Advance curTime to next available task's enqueueTime if the CPU is idle.
//   * Enqueue all tasks whose enqueueTime <= curTime into the heap.
//   * Pop and execute the top task: curTime += procTime, record original index.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 22. PROCESS TASKS USING SERVERS [G-22]
// =========================================================

vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
    int n = servers.size(), m = tasks.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> free;
    using T = tuple<long long, int, int>;
    priority_queue<T, vector<T>, greater<T>> busy;

    for (int i = 0; i < n; ++i) free.push({servers[i], i});

    vector<int> ans(m);
    long long time = 0;

    for (int i = 0; i < m; ++i) {
        time = max(time, (long long)i);
        while (!busy.empty() && get<0>(busy.top()) <= time) {
            auto [_, w, idx] = busy.top();
            busy.pop();
            free.push({w, idx});
        }
        if (free.empty()) {
            time = get<0>(busy.top());
            while (!busy.empty() && get<0>(busy.top()) <= time) {
                auto [_, w, idx] = busy.top();
                busy.pop();
                free.push({w, idx});
            }
        }
        auto [w, idx] = free.top();
        free.pop();
        ans[i] = idx;
        busy.push({time + tasks[i], w, idx});
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Assign incoming tasks to free server with smallest weight (tie broken by smallest index).
// - Approach: Dual Priority Queues (Free Servers & Busy Servers).
// - Intuition:
//   * Free servers are ordered by {weight, index}.
//   * Busy servers are ordered by {freeTime, weight, index}.
//   * For task i arriving at time i, first free all servers whose freeTime <= curTime.
//   * If no servers are free, fast-forward curTime to the earliest freeTime in busyServers and release them.
//   * Pop the optimal server from freeServers, assign task i, and push to busyServers with its completion time.
// - Complexity: Time: O((M + N) log N), Space: O(N).

