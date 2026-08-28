/**
 * ====================================================================================================
 *                 ULTIMATE HEAP & INTERVAL MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Priority Queues (Top-K, Dynamic Median, K-Way Merge, Greedy Heap),
 * IPO Maximized Capital, Minimum Interval Querying, Interval Overlaps, Meeting Rooms, Sweep-Line,
 * Room Allocation, and Interval Coverage.
 */

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


const ll MOD = 1e9 + 7;

// Priority Queue Alias Helpers
using MaxHeap     = priority_queue<int>;
using MinHeap     = priority_queue<int, vector<int>, greater<int>>;
using MaxPairHeap = priority_queue<pii>;
using MinPairHeap = priority_queue<pii, vector<pii>, greater<pii>>;

// ====================================================================================================
// SECTION 1: HEAP FUNDAMENTALS & TOP-K / STREAM PATTERNS
// ====================================================================================================

// 1. K-th Largest & Smallest Elements in Array
int kthLargest(const vi &nums, int k) {
    MinHeap pq;
    for (int x : nums) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();
    }
    return pq.top();
}

int kthSmallest(const vi &nums, int k) {
    MaxHeap pq;
    for (int x : nums) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();
    }
    return pq.top();
}

// 2. Top K Frequent Elements (LeetCode 347)
vi topKFrequent(const vi &nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    MinPairHeap pq;
    for (auto &[num, f] : freq) {
        pq.push({f, num});
        if ((int)pq.size() > k) pq.pop();
    }

    vi ans;
    while (!pq.empty()) {
        ans.push_back(pq.top().second);
        pq.pop();
    }
    return ans;
}

// 3. K Closest Points to Origin (LeetCode 973)
vvi kClosestPoints(const vvi &points, int k) {
    priority_queue<pair<int, int>> pq;
    for (int i = 0; i < (int)points.size(); i++) {
        int d = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        pq.push({d, i});
        if ((int)pq.size() > k) pq.pop();
    }
    vvi ans;
    while (!pq.empty()) {
        ans.push_back(points[pq.top().second]);
        pq.pop();
    }
    return ans;
}

// 4. Find Median from Data Stream (LeetCode 295 - Two Heaps)
class MedianFinder {
    MaxHeap left;
    MinHeap right;
public:
    void addNum(int num) {
        left.push(num);
        right.push(left.top());
        left.pop();
        if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }
    }

    double findMedian() {
        if (left.size() > right.size()) return left.top();
        return (left.top() + right.top()) / 2.0;
    }
};

// 5. IPO - Maximize Capital (LeetCode 502 - Dual Heap Greedy)
int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
    int n = profits.size();
    vector<pii> projects(n);
    for (int i = 0; i < n; i++) projects[i] = {capital[i], profits[i]};
    sort(projects.begin(), projects.end());

    priority_queue<int> maxProfitHeap;
    int idx = 0;

    for (int i = 0; i < k; i++) {
        while (idx < n && projects[idx].first <= w) {
            maxProfitHeap.push(projects[idx].second);
            idx++;
        }
        if (maxProfitHeap.empty()) break;
        w += maxProfitHeap.top();
        maxProfitHeap.pop();
    }
    return w;
}
// Interview Explanation:
// - Problem Statement: Find maximum capital achievable after completing at most k distinct projects (LeetCode 502).
// - Approach: Sort projects by capital + Max-Heap of affordable profits.
// - Intuition: Greedily pick the maximum profit project affordable with current capital `w`; add newly affordable projects into heap at each step.
// - Complexity: Time: O(N \log N + K \log N), Space: O(N).


// 6. Minimum Interval to Include Each Query (LeetCode 1851 - Min-Heap + Offline Sorting)
vector<int> minInterval(vector<vector<int>>& intervals, vector<int>& queries) {
    int n = intervals.size(), q = queries.size();
    sort(intervals.begin(), intervals.end());

    vector<pii> sortedQueries(q);
    for (int i = 0; i < q; i++) sortedQueries[i] = {queries[i], i};
    sort(sortedQueries.begin(), sortedQueries.end());

    // Min-Heap: {size, right_end}
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    vector<int> ans(q, -1);
    int idx = 0;

    for (const auto& [queryVal, origIdx] : sortedQueries) {
        while (idx < n && intervals[idx][0] <= queryVal) {
            int len = intervals[idx][1] - intervals[idx][0] + 1;
            pq.push({len, intervals[idx][1]});
            idx++;
        }
        while (!pq.empty() && pq.top().second < queryVal) {
            pq.pop(); // Remove intervals that end before queryVal
        }
        if (!pq.empty()) {
            ans[origIdx] = pq.top().first;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: For each query, find the size of the smallest interval containing the query point (LeetCode 1851).
// - Approach: Offline sorting of queries + Min-Heap tracking active interval sizes.
// - Intuition: Sort queries and intervals; push eligible intervals (`start <= query`) to heap; pop expired intervals (`end < query`). Heap top gives min size.
// - Complexity: Time: O((N + Q) \log N), Space: O(N + Q).


// ====================================================================================================
// SECTION 2: K-WAY MERGE & COMBINATION SUM HEAPS
// ====================================================================================================

// 1. Merge K Sorted Arrays
vi mergeKsortedArrays(const vvi &arrays) {
    using Element = tuple<int, int, int>;
    priority_queue<Element, vector<Element>, greater<Element>> pq;

    for (int i = 0; i < (int)arrays.size(); i++) {
        if (!arrays[i].empty()) pq.push({arrays[i][0], i, 0});
    }

    vi ans;
    while (!pq.empty()) {
        auto [val, arrIdx, elemIdx] = pq.top();
        pq.pop();
        ans.push_back(val);
        if (elemIdx + 1 < (int)arrays[arrIdx].size()) {
            pq.push({arrays[arrIdx][elemIdx + 1], arrIdx, elemIdx + 1});
        }
    }
    return ans;
}

// 2. Find K Pairs with Smallest Sums (LeetCode 373)
vector<pii> kSmallestPairs(const vi &nums1, const vi &nums2, int k) {
    vector<pii> ans;
    if (nums1.empty() || nums2.empty() || k <= 0) return ans;

    using Entry = pair<int, pii>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    for (int i = 0; i < min((int)nums1.size(), k); i++) {
        pq.push({nums1[i] + nums2[0], {i, 0}});
    }

    while (k-- > 0 && !pq.empty()) {
        auto [sum, indices] = pq.top();
        pq.pop();
        auto [i, j] = indices;
        ans.push_back({nums1[i], nums2[j]});
        if (j + 1 < (int)nums2.size()) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
        }
    }
    return ans;
}

// 3. N-th Ugly Number (LeetCode 264)
int nthUglyNumber(int n) {
    priority_queue<ll, vector<ll>, greater<ll>> pq;
    unordered_set<ll> seen;
    pq.push(1);
    seen.insert(1);
    ll ugly = 1;
    while (n-- > 0) {
        ugly = pq.top();
        pq.pop();
        for (int factor : {2, 3, 5}) {
            ll nextUgly = ugly * factor;
            if (!seen.count(nextUgly)) {
                seen.insert(nextUgly);
                pq.push(nextUgly);
            }
        }
    }
    return ugly;
}


// ====================================================================================================
// SECTION 3: GREEDY HEAP & INTERVAL ALGORITHMS
// ====================================================================================================

// 1. Minimum Cost to Connect Ropes / Sticks (LeetCode 1168)
int minCostToConnectSticks(const vi &sticks) {
    MinHeap pq(sticks.begin(), sticks.end());
    int totalCost = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        int merged = a + b;
        totalCost += merged;
        pq.push(merged);
    }
    return totalCost;
}

// 2. Reorganize String (LeetCode 767)
string reorganizeString(const string &s) {
    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    priority_queue<pair<int, char>> pq;
    for (auto &[c, cnt] : freq) {
        if (cnt > ((int)s.length() + 1) / 2) return "";
        pq.push({cnt, c});
    }

    string ans = "";
    pair<int, char> prev = {-1, '#'};
    while (!pq.empty()) {
        auto [cnt, c] = pq.top();
        pq.pop();
        ans += c;
        if (prev.first > 0) pq.push(prev);
        prev = {cnt - 1, c};
    }
    return ans.length() == s.length() ? ans : "";
}

// 3. Course Schedule III (LeetCode 630)
int scheduleCourse(vvi courses) {
    sort(courses.begin(), courses.end(), [](const vi &a, const vi &b) {
        return a[1] < b[1];
    });
    MaxHeap maxDurationHeap;
    int currentTime = 0;
    for (const auto &c : courses) {
        int duration = c[0], deadline = c[1];
        if (currentTime + duration <= deadline) {
            currentTime += duration;
            maxDurationHeap.push(duration);
        } else if (!maxDurationHeap.empty() && maxDurationHeap.top() > duration) {
            currentTime += duration - maxDurationHeap.top();
            maxDurationHeap.pop();
            maxDurationHeap.push(duration);
        }
    }
    return maxDurationHeap.size();
}

// 4. Merge Overlapping Intervals (LeetCode 56)
vvi mergeIntervals(vvi &intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vvi merged;
    for (const auto &interval : intervals) {
        if (merged.empty() || merged.back()[1] < interval[0]) {
            merged.push_back(interval);
        } else {
            merged.back()[1] = max(merged.back()[1], interval[1]);
        }
    }
    return merged;
}

// 5. Insert Interval (LeetCode 57)
vvi insertInterval(const vvi &intervals, vi newInterval) {
    vvi ans;
    int i = 0, n = intervals.size();
    while (i < n && intervals[i][1] < newInterval[0]) ans.push_back(intervals[i++]);
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    ans.push_back(newInterval);
    while (i < n) ans.push_back(intervals[i++]);
    return ans;
}

// 6. Minimum Meeting Rooms (LeetCode 253 / Sweep-Line)
int minMeetingRooms(const vector<pii> &intervals) {
    if (intervals.empty()) return 0;
    vi starts, ends;
    for (const auto &[s, e] : intervals) {
        starts.push_back(s);
        ends.push_back(e);
    }
    sort(starts.begin(), starts.end());
    sort(ends.begin(), ends.end());

    int rooms = 0, endIdx = 0;
    for (size_t i = 0; i < starts.size(); i++) {
        if (starts[i] < ends[endIdx]) {
            rooms++;
        } else {
            endIdx++;
        }
    }
    return rooms;
}

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: HEAPS & INTERVALS
 ====================================================================================================
 1. HEAP STRATEGY MATRIX:
    - IPO Maximize Capital: Sort capital + Max-Heap profit window.
    - Min Interval to Include Query: Sort queries & intervals + Min-Heap active range length.
 ====================================================================================================
*/


// ====================================================================================================
// SECTION: ADVANCED HEAP & INTERVAL INTERVIEW EXTENSIONS
// ====================================================================================================

// 1. Top K Frequent Words (LeetCode 692)
vector<string> topKFrequentWords(vector<string>& words, int k) {
    unordered_map<string, int> freq;
    for (const string& w : words) freq[w]++;

    auto comp = [](const pair<int, string>& a, const pair<int, string>& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second;
    };
    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);

    for (auto& [w, f] : freq) {
        pq.push({f, w});
        if ((int)pq.size() > k) pq.pop();
    }
    vector<string> res(k);
    for (int i = k - 1; i >= 0; --i) {
        res[i] = pq.top().second;
        pq.pop();
    }
    return res;
}

// 2. Sliding Window Median (LeetCode 480 - Two Multisets)
vector<double> medianSlidingWindow(vi& nums, int k) {
    multiset<int> low, high;
    vector<double> medians;

    auto balance = [&]() {
        while (low.size() > high.size() + 1) {
            auto it = prev(low.end());
            high.insert(*it);
            low.erase(it);
        }
        while (high.size() > low.size()) {
            auto it = high.begin();
            low.insert(*it);
            high.erase(it);
        }
    };

    auto add = [&](int num) {
        if (low.empty() || num <= *prev(low.end())) low.insert(num);
        else high.insert(num);
        balance();
    };

    auto remove = [&](int num) {
        auto it = low.find(num);
        if (it != low.end()) low.erase(it);
        else high.erase(high.find(num));
        balance();
    };

    for (int i = 0; i < (int)nums.size(); ++i) {
        add(nums[i]);
        if (i >= k - 1) {
            if (k % 2 == 1) medians.push_back((double)*prev(low.end()));
            else medians.push_back(((double)*prev(low.end()) + *high.begin()) / 2.0);
            remove(nums[i - k + 1]);
        }
    }
    return medians;
}

// 3. Meeting Rooms I (LeetCode 252 - Can Attend All Meetings)
bool canAttendMeetings(vvi& intervals) {
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < (int)intervals.size(); ++i) {
        if (intervals[i][0] < intervals[i - 1][1]) return false;
    }
    return true;
}

// 4. Employee Free Time (LeetCode 759)
vvi employeeFreeTime(vector<vvi>& schedule) {
    vvi intervals;
    for (auto& emp : schedule) {
        for (auto& iv : emp) intervals.push_back(iv);
    }
    sort(intervals.begin(), intervals.end());

    vvi freeTime;
    int end = intervals[0][1];
    for (int i = 1; i < (int)intervals.size(); ++i) {
        if (intervals[i][0] > end) {
            freeTime.push_back({end, intervals[i][0]});
        }
        end = max(end, intervals[i][1]);
    }
    return freeTime;
}

// 5. Minimum Number of Arrows to Burst Balloons (LeetCode 452)
int findMinArrowShots(vvi& points) {
    if (points.empty()) return 0;
    sort(points.begin(), points.end(), [](const vi& a, const vi& b) {
        return a[1] < b[1];
    });
    int arrows = 1;
    int firstEnd = points[0][1];
    for (int i = 1; i < (int)points.size(); ++i) {
        if (points[i][0] > firstEnd) {
            arrows++;
            firstEnd = points[i][1];
        }
    }
    return arrows;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
