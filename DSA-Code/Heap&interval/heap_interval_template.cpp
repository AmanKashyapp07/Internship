/**
 * ====================================================================================================
 *                 ULTIMATE HEAP & INTERVAL MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Priority Queues (Top-K, Dynamic Median, K-Way Merge, Greedy Heap),
 * Interval Overlaps, Meeting Rooms, Sweep-Line, Room Allocation, and Interval Coverage.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;

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
// Interview Explanation:
// - Problem Statement: Find the k-th largest (or smallest) element in an unsorted array.
// - Approach: Min-Heap of size k for K-th largest; Max-Heap of size k for K-th smallest.
// - Intuition: Min-Heap keeps the k largest elements seen so far; top element is the smallest among the k largest, which is the k-th largest overall.
// - Complexity: Time: O(N \log K), Space: O(K).


// 2. Top K Frequent Elements (LeetCode 347)
vi topKFrequent(const vi &nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    MinPairHeap pq; // {frequency, number}
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
// Interview Explanation:
// - Problem Statement: Return the k most frequent elements in an array.
// - Approach: Frequency map + Min-Heap of size k ordered by frequency.
// - Intuition: Evicting low-frequency elements whenever heap size exceeds k leaves the top k frequent elements.
// - Complexity: Time: O(N + U \log K) where U is unique elements, Space: O(U + K).


// 3. K Closest Points to Origin (LeetCode 973)
vvi kClosestPoints(const vvi &points, int k) {
    priority_queue<pair<int, int>> pq; // {squared_dist, index}
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
// Interview Explanation:
// - Problem Statement: Find the k closest 2D points to origin (0, 0) based on Euclidean distance.
// - Approach: Max-Heap of size k storing squared distances.
// - Intuition: Max-heap evicts the farthest point whenever size exceeds k; squared distance avoids floating point inaccuracies.
// - Complexity: Time: O(N \log K), Space: O(K).


// 4. Find Median from Data Stream (LeetCode 295 - Two Heaps)
class MedianFinder {
    MaxHeap left; // Stores smaller half of numbers (max element on top)
    MinHeap right; // Stores larger half of numbers (min element on top)
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
// Interview Explanation:
// - Problem Statement: Maintain a data stream supporting O(1) median queries and O(log N) number insertions.
// - Approach: Dual-heap balancing (Max-Heap left half, Min-Heap right half).
// - Intuition: Max-heap holds smaller half, Min-heap holds larger half; balance condition guarantees left.size() == right.size() or left.size() == right.size() + 1.
// - Complexity: Time: O(\log N) per insertion, O(1) for median query, Space: O(N).


// 5. K-th Largest Element in a Stream (LeetCode 703)
class KthLargestStream {
    MinHeap pq;
    int k;
public:
    KthLargestStream(int k, const vi &nums) : k(k) {
        for (int x : nums) add(x);
    }
    int add(int val) {
        pq.push(val);
        if ((int)pq.size() > k) pq.pop();
        return pq.top();
    }
};
// Interview Explanation:
// - Problem Statement: Design a class to find the k-th largest element in a dynamic stream of values.
// - Approach: Fixed Min-Heap of size k.
// - Intuition: Maintain exactly k elements in a min-heap; the smallest among them (top) is the k-th largest stream value.
// - Complexity: Time: O(\log K) per add operation, Space: O(K).


// 6. Rank Transform of an Array (LeetCode 1331)
vi replaceElementsByRank(const vi &arr) {
    vi sorted = arr;
    sort(sorted.begin(), sorted.end());
    unordered_map<int, int> rankMap;
    int rank = 1;
    for (int x : sorted) {
        if (!rankMap.count(x)) rankMap[x] = rank++;
    }
    vi ans;
    for (int x : arr) ans.push_back(rankMap[x]);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Replace each array element with its 1-based rank (smallest element gets rank 1).
// - Approach: Sorting + Hash Map deduplicated rank assignment.
// - Intuition: Sort a copy of the array and assign incremental ranks to distinct values; map original array elements to their ranks.
// - Complexity: Time: O(N \log N), Space: O(N).


// ====================================================================================================
// SECTION 2: K-WAY MERGE & COMBINATION SUM HEAPS
// ====================================================================================================

// 1. Merge K Sorted Arrays (LeetCode 23 variant)
vi mergeKsortedArrays(const vvi &arrays) {
    using Element = tuple<int, int, int>; // {val, arrayIdx, elemIdx}
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
// Interview Explanation:
// - Problem Statement: Merge K sorted arrays into a single sorted array.
// - Approach: K-Way Merge using a Min-Heap of size K storing `{value, array_index, element_index}`.
// - Intuition: Heap always yields the global minimum among all current array heads; advancing the pointer in the popped array maintains valid candidates.
// - Complexity: Time: O(N \log K) where N is total elements, Space: O(K) heap size.


// 2. Find K Pairs with Smallest Sums (LeetCode 373)
vector<pii> kSmallestPairs(const vi &nums1, const vi &nums2, int k) {
    vector<pii> ans;
    if (nums1.empty() || nums2.empty() || k <= 0) return ans;

    using Entry = pair<int, pii>; // {sum, {idx1, idx2}}
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
// Interview Explanation:
// - Problem Statement: Find K pairs (u, v) with the smallest sum taking u from nums1 and v from nums2.
// - Approach: Min-Heap pair sum traversal initializing with `(i, 0)` for all $i \in [0, \min(N_1, K)]$.
// - Intuition: Since arrays are sorted, `(i, 0)` is the smallest pair for `nums1[i]`; popping `(i, j)` pushes successor `(i, j + 1)`.
// - Complexity: Time: O(K \log K), Space: O(K).


// 3. Maximum K Sum Combinations (Top K Sums of Two Arrays)
vi maxKsumCombination(vi nums1, vi nums2, int k) {
    sort(nums1.rbegin(), nums1.rend());
    sort(nums2.rbegin(), nums2.rend());

    using Entry = pair<int, pii>; // {sum, {idx1, idx2}}
    priority_queue<Entry> pq;
    set<pii> visited;

    pq.push({nums1[0] + nums2[0], {0, 0}});
    visited.insert({0, 0});

    vi ans;
    int n1 = nums1.size(), n2 = nums2.size();

    while (k-- > 0 && !pq.empty()) {
        auto [sum, pos] = pq.top();
        pq.pop();
        auto [i, j] = pos;
        ans.push_back(sum);

        if (i + 1 < n1 && !visited.count({i + 1, j})) {
            pq.push({nums1[i + 1] + nums2[j], {i + 1, j}});
            visited.insert({i + 1, j});
        }
        if (j + 1 < n2 && !visited.count({i, j + 1})) {
            pq.push({nums1[i] + nums2[j + 1], {i, j + 1}});
            visited.insert({i, j + 1});
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the K maximum sum combinations of two arrays.
// - Approach: Descending sort + Max-Heap priority queue with 2D visited state tracking.
// - Intuition: Sort arrays descending; `nums1[0] + nums2[0]` is max sum. Popping `(i, j)` pushes adjacent candidates `(i+1, j)` and `(i, j+1)`.
// - Complexity: Time: O(N \log N + K \log K), Space: O(K).


// 4. K-th Smallest Subarray/Matrix Row Sum (LeetCode 1439)
vi mergeTwoRowsKSmallest(const vi &a, const vi &b, int k) {
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<>> pq;
    for (int i = 0; i < min((int)a.size(), k); i++) {
        pq.push({a[i] + b[0], {i, 0}});
    }
    vi ans;
    while (!pq.empty() && (int)ans.size() < k) {
        auto [sum, pos] = pq.top();
        pq.pop();
        auto [i, j] = pos;
        ans.push_back(sum);
        if (j + 1 < (int)b.size()) {
            pq.push({a[i] + b[j + 1], {i, j + 1}});
        }
    }
    return ans;
}

int kthSmallestMatrixRowSum(const vvi &mat, int k) {
    vi rowSums = mat[0];
    for (size_t i = 1; i < mat.size(); i++) {
        rowSums = mergeTwoRowsKSmallest(rowSums, mat[i], k);
    }
    return rowSums[min((int)rowSums.size() - 1, k - 1)];
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest sum formed by picking one element from each row of an M x N matrix.
// - Approach: Iterative row-by-row K-Way Merge maintaining at most k smallest candidate sums.
// - Intuition: Merging row 1 with row 2 using heap yields top k sums; repeat process with subsequent rows to keep total sums bounded to size k.
// - Complexity: Time: O(M \cdot K \log K), Space: O(K).


// 5. N-th Ugly Number (LeetCode 264)
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
// Interview Explanation:
// - Problem Statement: Find the n-th ugly number (positive integer whose prime factors are limited to 2, 3, 5).
// - Approach: Min-Heap with hash set deduplication.
// - Intuition: Pop current smallest ugly number and generate candidate multiples `ugly * 2`, `ugly * 3`, `ugly * 5`, deduplicating via set.
// - Complexity: Time: O(N \log N), Space: O(N).


// ====================================================================================================
// SECTION 3: GREEDY HEAP ALGORITHMS
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
// Interview Explanation:
// - Problem Statement: Connect all sticks into one stick with minimum total cost (cost to connect two sticks is sum of their lengths).
// - Approach: Huffman Coding Greedy Strategy via Min-Heap.
// - Intuition: Always greedily pick and merge the two shortest sticks to minimize cumulative sum additions.
// - Complexity: Time: O(N \log N), Space: O(N).


// 2. Reorganize String (LeetCode 767 - No Adjacent Duplicates)
string reorganizeString(const string &s) {
    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    priority_queue<pair<int, char>> pq;
    for (auto &[c, cnt] : freq) {
        if (cnt > ((int)s.length() + 1) / 2) return ""; // Pigeonhole principle check
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
// Interview Explanation:
// - Problem Statement: Rearrange characters of s so that no two adjacent characters are the same.
// - Approach: Max-Heap character frequency placement with previous character hold-back.
// - Intuition: Always pick the most frequent available character; hold back the previously placed character for one iteration to prevent adjacency.
// - Complexity: Time: O(N \log |\Sigma|), Space: O(|\Sigma|).


// 3. Course Schedule III (LeetCode 630)
int scheduleCourse(vvi courses) {
    // Sort courses strictly by deadline
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
            // Swap out longest duration course taken so far to free up time
            currentTime += duration - maxDurationHeap.top();
            maxDurationHeap.pop();
            maxDurationHeap.push(duration);
        }
    }
    return maxDurationHeap.size();
}
// Interview Explanation:
// - Problem Statement: Given courses with [duration, deadline], return the maximum number of courses you can take.
// - Approach: Sort by deadline + Max-Heap duration replacement strategy.
// - Intuition: If a course exceeds deadline, swap out the longest duration course taken previously if it reduces total time, maximizing future capacity.
// - Complexity: Time: O(N \log N), Space: O(N).


// ====================================================================================================
// SECTION 4: INTERVALS & SWEEP-LINE BLUEPRINT
// ====================================================================================================

// 1. Merge Overlapping Intervals (LeetCode 56)
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
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals and return an array of non-overlapping intervals.
// - Approach: Sort by start time + linear scan merging overlaps.
// - Intuition: Sorting guarantees that if interval i overlaps with current merged range, `interval[0] <= merged.back()[1]`.
// - Complexity: Time: O(N \log N), Space: O(N).


// 2. Insert Interval (LeetCode 57)
vvi insertInterval(const vvi &intervals, vi newInterval) {
    vvi ans;
    int i = 0, n = intervals.size();
    // Phase 1: Add all intervals ending before newInterval starts
    while (i < n && intervals[i][1] < newInterval[0]) {
        ans.push_back(intervals[i++]);
    }
    // Phase 2: Merge all overlapping intervals with newInterval
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    ans.push_back(newInterval);
    // Phase 3: Add remaining intervals
    while (i < n) ans.push_back(intervals[i++]);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into a sorted non-overlapping interval list and merge if necessary.
// - Approach: 3-Phase Linear Scan (before, overlapping merge, after).
// - Intuition: Process intervals strictly before newInterval, merge all overlapping ranges into newInterval, then append trailing intervals.
// - Complexity: Time: O(N) single pass, Space: O(N).


// 3. Minimum Meeting Rooms / Max Overlap Count (LeetCode 253 / Sweep-Line)
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
            rooms++; // Room occupied, allocate new room
        } else {
            endIdx++; // Reuse existing room
        }
    }
    return rooms;
}
// Interview Explanation:
// - Problem Statement: Find the minimum number of conference rooms required to schedule all meetings.
// - Approach: Two-Pointer Sweep-Line on sorted start and end times.
// - Intuition: If a meeting starts before the earliest ending meeting (`starts[i] < ends[endIdx]`), a new room must be allocated.
// - Complexity: Time: O(N \log N), Space: O(N).


// 4. CSES Room Allocation (Assign Customer Room Numbers)
vi assignRooms(vector<tuple<int, int, int>> customers) {
    // customers: {arrival, departure, original_index}
    int n = customers.size();
    vi assigned(n);
    sort(customers.begin(), customers.end());

    // Min-Heap: {departure_time, room_id}
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    int roomCount = 0;

    for (const auto &cust : customers) {
        auto [arrival, departure, idx] = cust;
        int roomId;
        if (!pq.empty() && pq.top().first < arrival) {
            roomId = pq.top().second; // Reuse free room
            pq.pop();
        } else {
            roomId = ++roomCount; // Open new room
        }
        assigned[idx] = roomId;
        pq.push({departure, roomId});
    }
    return assigned;
}
// Interview Explanation:
// - Problem Statement: Assign customer room numbers maximizing reuse and minimizing total rooms used.
// - Approach: Sort arrival times + Min-Heap tracking `{departure_time, room_id}`.
// - Intuition: If `pq.top().departure < arrival`, reuse room_id; else increment room count and assign new room.
// - Complexity: Time: O(N \log N), Space: O(N).


// 5. Non-Overlapping Intervals (LeetCode 435 - Removals to Eliminate Overlaps)
int eraseOverlapIntervals(vvi intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const vi &a, const vi &b) {
        return a[1] < b[1]; // Greedy selection by earliest finish time
    });
    int keepCount = 1, lastEnd = intervals[0][1];
    for (size_t i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] >= lastEnd) {
            keepCount++;
            lastEnd = intervals[i][1];
        }
    }
    return intervals.size() - keepCount;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of intervals to remove to make the rest non-overlapping.
// - Approach: Activity selection sorting by finish time; answer is `N - maxKeepCount`.
// - Intuition: Maximizing non-overlapping intervals kept is mathematically equivalent to minimizing interval removals.
// - Complexity: Time: O(N \log N), Space: O(1).


// 6. Minimum Intervals to Cover Range [0, Target]
int intervalCover(vector<pii> intervals, int target) {
    sort(intervals.begin(), intervals.end());
    int count = 0, covered = 0, i = 0, n = intervals.size();
    while (covered < target) {
        int farthest = covered;
        while (i < n && intervals[i].first <= covered) {
            farthest = max(farthest, intervals[i].second);
            i++;
        }
        if (farthest == covered) return -1; // Gap detected, cannot cover
        covered = farthest;
        count++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of intervals required to completely cover range [0, target].
// - Approach: Greedy extension scanning available intervals covering the current boundary.
// - Intuition: At each step, among all intervals starting <= covered, greedily choose the one extending farthest to the right.
// - Complexity: Time: O(N \log N), Space: O(1).


// 7. Find K-th Smallest Pair Distance (LeetCode 719)
int countPairsWithDistanceAtMost(const vi &nums, int targetDist) {
    int count = 0, n = nums.size();
    for (int right = 0; right < n; right++) {
        int left = lower_bound(nums.begin(), nums.begin() + right, nums[right] - targetDist) - nums.begin();
        count += right - left;
    }
    return count;
}

int smallestDistancePair(vi nums, int k) {
    sort(nums.begin(), nums.end());
    int low = 0, high = nums.back() - nums.front(), ans = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (countPairsWithDistanceAtMost(nums, mid) >= k) {
            ans = mid;
            high = mid - 1; // Try smaller valid distance
        } else {
            low = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest distance among all pairs in an integer array.
// - Approach: Binary Search on Answer distance range + `lower_bound` pair count.
// - Intuition: Monotonic property: as distance mid increases, count of pairs with distance <= mid increases; binary search finds exact k-th threshold.
// - Complexity: Time: O(N \log N + N \log N \cdot \log(\text{range})), Space: O(1).

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: HEAPS & INTERVALS
 ====================================================================================================

 1. HEAP STRATEGY MATRIX:
    -------------------------------------------------------------------------------------------------
    Goal                                    | Heap Type           | Heap Condition / Size
    -------------------------------------------------------------------------------------------------
    Find K-th Largest Element               | Min-Heap            | Keep size == K; top is K-th largest
    Find K-th Smallest Element              | Max-Heap            | Keep size == K; top is K-th smallest
    Top K Frequent Elements                 | Min-Pair-Heap       | Key on frequency; keep size == K
    K Closest Points to Origin              | Max-Heap            | Key on distance^2; keep size == K
    Dynamic Stream Median                   | Max-Heap + Min-Heap | Left half Max-Heap, Right half Min-Heap
    -------------------------------------------------------------------------------------------------

 2. CANONICAL INTERVAL PATTERNS & SORTING RULES:
    • Merge Overlapping Intervals (LeetCode 56):
      - SORT BY START TIME (`intervals[i][0]`). Merge if `next.start <= current.end`.
    • Non-Overlapping Removals / Activity Selection (LeetCode 435 / 600):
      - SORT BY FINISH TIME (`intervals[i][1]`). Keep next if `next.start >= lastEnd`.
    • Meeting Rooms II / Max Overlap Count (LeetCode 253):
      - Two Pointers on sorted `starts` and `ends`: If `starts[i] < ends[endIdx]`, allocate new room.
    • Insert Interval (LeetCode 57):
      - 3 Phase Scan: Add left non-overlapping, merge overlapping, add right non-overlapping.
    • Minimum Interval Cover [0, Target]:
      - SORT BY START TIME. Greedily pick max `end` among all intervals with `start <= covered`.

 3. SWEEP LINE TIE-BREAKING RULE:
    • Touch vs. Overlap:
      - If interval bounds [1, 2] and [2, 3] are OVERLAPPING (closed intervals), process Start (+1) before End (-1).
      - If interval bounds [1, 2) and [2, 3) are SEPARATE (open intervals), process End (-1) before Start (+1).
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
