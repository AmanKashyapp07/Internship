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
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 21 | Min Number Arrows to Burst Balloons         | Interval Scheduling by End Point  | O(N logN)| O(1)     |
 | 22 | Erase Overlapping Intervals                 | Greedy Interval End Filtering     | O(N logN)| O(1)     |
 | 23 | Interval List Intersections                 | Two-Pointer Boundary Overlap      | O(N + M) | O(1)     |
 | 24 | Car Pooling                                 | Difference Array Sweep-Line       | O(N + S) | O(S)     |
 | 25 | Corporate Flight Bookings                   | Difference Array Prefix Sum       | O(N + B) | O(N)     |
 | 26 | My Calendar I                               | Ordered Map Adjacent Intersection | O(N logN)| O(N)     |
 | 27 | My Calendar II                              | Double-Booking Conflict Tracking  | O(N^2)   | O(N)     |
 | 28 | My Calendar III                             | Line Sweep Point Concurrency      | O(N^2)   | O(N)     |
 | 29 | Remove Covered Intervals                    | Start Asc / End Desc Sort Sweep   | O(N logN)| O(1)     |
 | 30 | Minimum Number of Platforms                 | Chronological Arrival/Departure   | O(N logN)| O(1)     |
 | 31 | Activity Selection                          | Earliest Finish Time Selection    | O(N logN)| O(N)     |
 | 32 | Job Sequencing Problem                      | Profit-Descending Slot Allocation | O(N*Dead)| O(Dead)  |
 | 33 | Maximum Meetings in One Room                | Greedy End-Time Room Allocation   | O(N logN)| O(N)     |
 | 34 | Course Schedule III                         | Greedy Duration Swap Max-Heap     | O(N logN)| O(N)     |
 | 35 | Task Scheduler                              | Max-Frequency Idle Slot Formula   | O(N)     | O(1)     |
 | 36 | Two City Scheduling                         | Cost Delta Sorting Partition      | O(N logN)| O(1)     |
 | 37 | Maximum Performance of a Team               | Efficiency Sweep + Min-Heap Speed | O(N logN)| O(N + K) |
 | 38 | IPO                                         | Capital-Sorted Scan + Max-Heap W  | O(N logN)| O(N)     |
 | 39 | Single-Threaded CPU                         | Enqueue Sweep + Min-Heap Shortest | O(N logN)| O(N)     |
 | 40 | Process Tasks Using Servers                 | Dual Priority Queues (Free & Busy)| O(M logN)| O(N)     |
 ====================================================================================================
*/

// =========================================================
// 21. MINIMUM NUMBER OF ARROWS TO BURST BALLOONS
// =========================================================

bool compareBalloonEnds(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1];
}

int findMinArrowShots(vector<vector<int>>& a) {
    if (a.empty()) return 0;
    sort(a.begin(), a.end(), compareBalloonEnds);

    int arrows = 1;
    long long curEnd = a[0][1];

    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] > curEnd) {
            arrows++;
            curEnd = a[i][1];
        }
    }
    return arrows;
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
// 22. ERASE OVERLAPPING INTERVALS
// =========================================================

bool compareIntervalEndsG2(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1];
}

int eraseOverlapIntervalsCount(vector<vector<int>>& a) {
    if (a.empty()) return 0;
    sort(a.begin(), a.end(), compareIntervalEndsG2);

    int kept = 1;
    int end = a[0][1];

    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i][0] >= end) {
            kept++;
            end = a[i][1];
        }
    }
    return (int)a.size() - kept;
}
// Interview Explanation:
// - Problem Statement: Return minimum number of intervals to remove so that the rest are non-overlapping.
// - Approach: Classic Activity Selection by Earliest End Time.
// - Intuition:
//   * Minimizing removals is equivalent to maximizing the cardinality of a mutually disjoint subset.
//   * Sort intervals ascending by end time.
//   * Greedily retain the interval that finishes earliest, leaving maximal space for remaining candidates.
//   * Count intervals kept; the answer is a.size() - kept.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 23. INTERVAL LIST INTERSECTIONS
// =========================================================

vector<vector<int>> intervalIntersection(vector<vector<int>>& a, vector<vector<int>>& b) {
    vector<vector<int>> ans;
    int i = 0, j = 0;
    int n = a.size(), m = b.size();

    while (i < n && j < m) {
        int lo = max(a[i][0], b[j][0]);
        int hi = min(a[i][1], b[j][1]);

        if (lo <= hi) {
            ans.push_back({lo, hi});
        }

        if (a[i][1] < b[j][1]) {
            i++;
        } else {
            j++;
        }
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
// 24. CAR POOLING
// =========================================================

bool carPooling(vector<vector<int>>& trips, int capacity) {
    int diff[1001] = {0};
    for (const auto& t : trips) {
        diff[t[1]] += t[0];
        diff[t[2]] -= t[0];
    }

    int cur = 0;
    for (int i = 0; i <= 1000; ++i) {
        cur += diff[i];
        if (cur > capacity) return false;
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
// 25. CORPORATE FLIGHT BOOKINGS
// =========================================================

vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> diff(n + 1, 0);
    for (const auto& b : bookings) {
        diff[b[0] - 1] += b[2];
        diff[b[1]]     -= b[2];
    }

    vector<int> ans(n);
    int cur = 0;
    for (int i = 0; i < n; ++i) {
        cur += diff[i];
        ans[i] = cur;
    }
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
// 26. MY CALENDAR I
// =========================================================

class MyCalendar {
    map<int, int> mp; // start -> end
public:
    MyCalendar() {}

    bool book(int start, int end) {
        auto it = mp.upper_bound(start);
        if (it != mp.end() && it->first < end) {
            return false;
        }
        if (it != mp.begin() && prev(it)->second > start) {
            return false;
        }
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
// 27. MY CALENDAR II
// =========================================================

class MyCalendarTwo {
    vector<pair<int, int>> bookings;
    vector<pair<int, int>> overlaps;
public:
    MyCalendarTwo() {}

    bool book(int start, int end) {
        for (const auto& [s, e] : overlaps) {
            if (max(start, s) < min(end, e)) return false;
        }
        for (const auto& [s, e] : bookings) {
            if (max(start, s) < min(end, e)) {
                overlaps.push_back({max(start, s), min(end, e)});
            }
        }
        bookings.push_back({start, end});
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
// 28. MY CALENDAR III
// =========================================================

class MyCalendarThree {
    map<int, int> mp;
public:
    MyCalendarThree() {}

    int book(int start, int end) {
        mp[start]++;
        mp[end]--;
        int cur = 0, mx = 0;
        for (const auto& [time, count] : mp) {
            cur += count;
            mx = max(mx, cur);
        }
        return mx;
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
// 29. REMOVE COVERED INTERVALS
// =========================================================

bool compareCoveredIntervals(const vector<int>& a, const vector<int>& b) {
    if (a[0] != b[0]) return a[0] < b[0];
    return a[1] > b[1];
}

int removeCoveredIntervals(vector<vector<int>>& a) {
    sort(a.begin(), a.end(), compareCoveredIntervals);

    int ans = 0, maxEnd = 0;
    for (const auto& v : a) {
        if (v[1] > maxEnd) {
            ans++;
            maxEnd = v[1];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return number of remaining intervals after removing all intervals covered by another.
// - Approach: Dual-Key Sorting (Start Ascending, End Descending) + Linear Sweep.
// - Intuition:
//   * If two intervals start at the same point, the one with larger end covers the smaller one.
//   * Sorting by start ascending and end descending guarantees that any candidate covering interval comes first.
//   * Maintain maxEnd of all retained intervals.
//   * If current interval has end <= maxEnd, it is completely covered by a preceding interval; discard it.
//   * If end > maxEnd, it extends beyond previous coverage; increment ans and update maxEnd = end.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 30. MINIMUM NUMBER OF PLATFORMS
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
            maxPlat = max(maxPlat, plat);
            i++;
        } else {
            plat--;
            j++;
        }
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
// 31. ACTIVITY SELECTION
// =========================================================

struct Activity {
    int start, finish;
};

bool compareActivities(const Activity& a, const Activity& b) {
    return a.finish < b.finish;
}

int maxActivities(vector<int>& start, vector<int>& finish) {
    int n = start.size();
    vector<Activity> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = {start[i], finish[i]};
    }
    sort(a.begin(), a.end(), compareActivities);

    int cnt = 1;
    int prevFinish = a[0].finish;

    for (int i = 1; i < n; ++i) {
        if (a[i].start >= prevFinish) {
            cnt++;
            prevFinish = a[i].finish;
        }
    }
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Select maximum number of non-conflicting activities performed by a single person.
// - Approach: Greedy Selection by Earliest Finish Time.
// - Intuition:
//   * Finishing an activity earlier leaves the maximum remaining time available for future activities.
//   * Sort all activities by their finish times in ascending order.
//   * Always pick the first activity.
//   * For subsequent activities, pick activity i if start[i] >= finish time of the last selected activity.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 32. JOB SEQUENCING PROBLEM
// =========================================================

struct Job {
    int id, dead, profit;
};

bool compareJobs(const Job& a, const Job& b) {
    return a.profit > b.profit;
}

pair<int, int> JobScheduling(vector<Job>& a) {
    sort(a.begin(), a.end(), compareJobs);

    int maxDead = 0;
    for (const auto& j : a) maxDead = max(maxDead, j.dead);

    vector<int> slot(maxDead + 1, -1);
    int count = 0, totalProfit = 0;

    for (const auto& j : a) {
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
// 33. MAXIMUM MEETINGS IN ONE ROOM
// =========================================================

struct Meeting {
    int start, end, id;
};

bool compareMeetings(const Meeting& a, const Meeting& b) {
    if (a.end != b.end) return a.end < b.end;
    return a.id < b.id;
}

vector<int> maxMeetings(vector<int>& start, vector<int>& end) {
    int n = start.size();
    vector<Meeting> m(n);
    for (int i = 0; i < n; ++i) {
        m[i] = {start[i], end[i], i + 1};
    }
    sort(m.begin(), m.end(), compareMeetings);

    vector<int> ans;
    ans.push_back(m[0].id);
    int prevEnd = m[0].end;

    for (int i = 1; i < n; ++i) {
        if (m[i].start > prevEnd) {
            ans.push_back(m[i].id);
            prevEnd = m[i].end;
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find list of meeting IDs that can be accommodated in a single conference room.
// - Approach: Greedy End-Time Room Allocation.
// - Intuition:
//   * Bundle meeting start, end, and 1-based index.
//   * Sort meetings ascending by end time (tie-break by smaller ID).
//   * Select the first meeting. Greedily append subsequent meetings whose start time is strictly greater than prevEnd.
//   * Sort result IDs in ascending order as standard OA format.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 34. COURSE SCHEDULE III
// =========================================================

bool compareCourseLastDay(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1];
}

int scheduleCourse(vector<vector<int>>& courses) {
    sort(courses.begin(), courses.end(), compareCourseLastDay);

    priority_queue<int> pq; // max-heap of durations
    int curTime = 0;

    for (const auto& c : courses) {
        int dur = c[0], last = c[1];
        if (curTime + dur <= last) {
            curTime += dur;
            pq.push(dur);
        } else if (!pq.empty() && pq.top() > dur) {
            curTime += dur - pq.top();
            pq.pop();
            pq.push(dur);
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
// 35. TASK SCHEDULER
// =========================================================

int leastInterval(vector<char>& tasks, int n) {
    vector<int> cnt(26, 0);
    int mx = 0;
    for (char t : tasks) {
        cnt[t - 'A']++;
        mx = max(mx, cnt[t - 'A']);
    }

    int k = 0;
    for (int f : cnt) {
        if (f == mx) k++;
    }

    int slots = (mx - 1) * (n + 1) + k;
    return max((int)tasks.size(), slots);
}
// Interview Explanation:
// - Problem Statement: Find minimum CPU intervals to complete tasks with cooling window n between same tasks.
// - Approach: Greedy Max-Frequency Frame Construction Math Formula.
// - Intuition:
//   * Tasks with maximum frequency mx dictate the idle schedule bottleneck.
//   * These tasks establish (mx - 1) chunks of length (n + 1), followed by a final chunk containing the k maximum-frequency tasks.
//   * Required intervals formula: slots = (mx - 1) * (n + 1) + k.
//   * If remaining tasks exceed the idle capacity, no idle periods are needed; answer is tasks.size().
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 36. TWO CITY SCHEDULING
// =========================================================

bool compareCityCosts(const vector<int>& a, const vector<int>& b) {
    return (a[0] - a[1]) < (b[0] - b[1]);
}

int twoCitySchedCost(vector<vector<int>>& costs) {
    sort(costs.begin(), costs.end(), compareCityCosts);

    int total = 0;
    int n = costs.size() / 2;
    for (int i = 0; i < n; ++i) {
        total += costs[i][0];
    }
    for (int i = n; i < 2 * n; ++i) {
        total += costs[i][1];
    }
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
// 37. MAXIMUM PERFORMANCE OF A TEAM
// =========================================================

struct Engineer {
    int speed, eff;
};

bool compareEngineers(const Engineer& a, const Engineer& b) {
    return a.eff > b.eff;
}

int maxPerformance(int n, vector<int>& speed, vector<int>& efficiency, int k) {
    vector<Engineer> eng(n);
    for (int i = 0; i < n; ++i) {
        eng[i] = {speed[i], efficiency[i]};
    }
    sort(eng.begin(), eng.end(), compareEngineers);

    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of speeds
    long long sumSpeed = 0, maxPerf = 0;

    for (int i = 0; i < n; ++i) {
        pq.push(eng[i].speed);
        sumSpeed += eng[i].speed;

        if ((int)pq.size() > k) {
            sumSpeed -= pq.top();
            pq.pop();
        }

        maxPerf = max(maxPerf, sumSpeed * eng[i].eff);
    }
    return maxPerf % MOD;
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
// 38. IPO
// =========================================================

struct Project {
    int cap, prof;
};

bool compareProjects(const Project& a, const Project& b) {
    return a.cap < b.cap;
}

int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
    int n = profits.size();
    vector<Project> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = {capital[i], profits[i]};
    }
    sort(p.begin(), p.end(), compareProjects);

    priority_queue<int> pq; // max-heap of profits
    int idx = 0;

    for (int step = 0; step < k; ++step) {
        while (idx < n && p[idx].cap <= w) {
            pq.push(p[idx].prof);
            idx++;
        }

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
// 39. SINGLE-THREADED CPU
// =========================================================

struct CpuTask {
    int enq, proc, id;
};

bool compareTaskEnqueue(const CpuTask& a, const CpuTask& b) {
    return a.enq < b.enq;
}

struct CpuTaskCompare {
    bool operator()(const CpuTask& a, const CpuTask& b) const {
        if (a.proc != b.proc) return a.proc > b.proc;
        return a.id > b.id;
    }
};

vector<int> getOrder(vector<vector<int>>& tasks) {
    int n = tasks.size();
    vector<CpuTask> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = {tasks[i][0], tasks[i][1], i};
    }
    sort(a.begin(), a.end(), compareTaskEnqueue);

    priority_queue<CpuTask, vector<CpuTask>, CpuTaskCompare> pq;
    vector<int> ans;
    long long curTime = 0;
    int idx = 0;

    while (idx < n || !pq.empty()) {
        if (pq.empty() && curTime < a[idx].enq) {
            curTime = a[idx].enq;
        }

        while (idx < n && a[idx].enq <= curTime) {
            pq.push(a[idx++]);
        }

        CpuTask top = pq.top();
        pq.pop();
        curTime += top.proc;
        ans.push_back(top.id);
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
// 40. PROCESS TASKS USING SERVERS
// =========================================================

struct ServerFreeCompare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        if (a.first != b.first) return a.first > b.first; // weight
        return a.second > b.second;                      // index
    }
};

struct ServerBusyCompare {
    bool operator()(const tuple<long long, int, int>& a, const tuple<long long, int, int>& b) const {
        if (get<0>(a) != get<0>(b)) return get<0>(a) > get<0>(b); // freeTime
        if (get<1>(a) != get<1>(b)) return get<1>(a) > get<1>(b); // weight
        return get<2>(a) > get<2>(b);                             // index
    }
};

vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
    int numServers = servers.size();
    int numTasks = tasks.size();

    priority_queue<pair<int, int>, vector<pair<int, int>>, ServerFreeCompare> freeServers;
    for (int i = 0; i < numServers; ++i) {
        freeServers.push({servers[i], i});
    }

    priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, ServerBusyCompare> busyServers;

    vector<int> ans(numTasks);
    long long curTime = 0;

    for (int i = 0; i < numTasks; ++i) {
        curTime = max(curTime, (long long)i);

        while (!busyServers.empty() && get<0>(busyServers.top()) <= curTime) {
            auto [t, w, idx] = busyServers.top();
            busyServers.pop();
            freeServers.push({w, idx});
        }

        if (freeServers.empty()) {
            curTime = get<0>(busyServers.top());
            while (!busyServers.empty() && get<0>(busyServers.top()) <= curTime) {
                auto [t, w, idx] = busyServers.top();
                busyServers.pop();
                freeServers.push({w, idx});
            }
        }

        auto [w, idx] = freeServers.top();
        freeServers.pop();
        ans[i] = idx;
        busyServers.push({curTime + tasks[i], w, idx});
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
