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
#include <random>
#include <mutex>
#include <condition_variable>
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
 | 1  | Design HashMap (MyHashMap - LC 706)         | Separate Chaining (Bucket Lists)  | Avg O(1) | O(N)     |
 | 2  | Summary Ranges (LC 228 / LC 352)            | Sorted Set Range Traversal        | O(log N) | O(N)     |
 | 3  | Water and Jug Problem (BFS - LC 365)        | BFS 6-State Graph Traversal       | O(X * Y) | O(X * Y) |
 | 4  | Moving Average from Stream (LeetCode 346)   | Sliding Circular Buffer / Queue   | O(1)     | O(W)     |
 | 5  | First Unique Number in Stream (LC 1429)     | Doubly Linked List + Hash Map     | O(1)     | O(N)     |
 | 6  | First Non-Repeating Character in Stream     | Frequency Table + FIFO Queue      | O(1) avg | O(1)     |
 | 7  | Design Hit Counter (LeetCode 362)           | Circular Buckets (300s) / Queue   | O(1)     | O(1)     |
 | 8  | Logger Rate Limiter (LeetCode 359)          | Hash Map Expiration Timestamps    | O(1)     | O(M)     |
 | 9  | Front Middle Back Queue (LeetCode 1670)     | Dual Balanced Deques (Left/Right) | O(1) all | O(N)     |
 | 10 | Finding MK Average (LeetCode 1825)          | 3 Multisets (Left, Mid, Right)    | O(log M) | O(M)     |
 | 11 | Snapshot Array (LeetCode 1146)              | History Vectors + Binary Search   | O(log S) | O(N + U) |
 | 12 | Stock Price Fluctuation (LeetCode 2034)     | Hash Map + Balanced Multiset      | O(log N) | O(N)     |
 | 13 | Seat Reservation Manager (LeetCode 1845)    | Min-Heap of Free Seat IDs         | O(log N) | O(N)     |
 | 14 | Design Underground System (LeetCode 1396)   | Dual Hash Maps (Transit & Stats)  | O(1) all | O(P + S^2)|
 | 15 | Number of Recent Calls (LeetCode 933)       | Sliding Window Queue [t-3000, t]  | O(1) avg | O(W)     |
 | 16 | Design Leaderboard (LeetCode 1244)          | Hash Map + Min-Heap Top-K         | O(N logK)| O(P)     |
 | 17 | Bounded Blocking Queue (LeetCode 1188)      | Mutex + Dual Condition Variables  | O(1) all | O(Cap)   |
 | 18 | Majority Checker in Subarray (LC 1154)      | Boyer-Moore Random Sampling + UB  | O(K logN)| O(N)     |
 | 19 | Design Authentication Manager (LC 1797)     | Hash Map Expiry with Lazy Purge   | O(1) gen | O(T)     |
 | 20 | Multi-Policy Online Cache Engine (LRU/TTL)  | DLL + Hash Map + Expiry Min-Heap  | O(1) avg | O(Cap)   |
 ====================================================================================================
*/

// ============================================================
// 1. DESIGN HASHMAP (MYHASHMAP) — LeetCode 706
// ============================================================

class MyHashMap {
    static const int SZ = 1000;
    vector<list<pair<int, int>>> b;
    int h(int k) { return k % SZ; }
public:
    MyHashMap() : b(SZ) {}
    void put(int k, int v) {
        int idx = h(k);
        for (auto& [key, val] : b[idx]) if (key == k) { val = v; return; }
        b[idx].push_back({k, v});
    }
    int get(int k) {
        for (auto& [key, val] : b[h(k)]) if (key == k) return val;
        return -1;
    }
    void remove(int k) {
        int idx = h(k);
        for (auto it = b[idx].begin(); it != b[idx].end(); ++it)
            if (it->first == k) { b[idx].erase(it); return; }
    }
};
// Interview Explanation:
// - Problem Statement: Implement a basic HashMap with put, get, and remove operations (LC 706).
// - Approach: Separate Chaining via array of linked lists (buckets).
// - Intuition:
//   * A hash map maps arbitrary integer keys to bounded array slots using modulo hashing `k % SZ`.
//   * Hash collisions occur when different keys hash to the same bucket index.
//   * Separate chaining resolves collisions by storing key-value pairs in a linked list per bucket.
//   * Operations scan the target bucket: update value if key exists, append if new, erase if removed.
// - Complexity: Time: O(1) average per op, Space: O(N).


// ============================================================
// 2. SUMMARY RANGES — LeetCode 228 / 352
// ============================================================

class SummaryRanges {
    set<int> st;
public:
    SummaryRanges() {}
    void addNum(int x) { st.insert(x); }
    vvi getIntervals() {
        if (st.empty()) return {};
        vvi ans;
        int s = *st.begin(), e = s;
        for (int x : st) {
            if (x == e + 1) e = x;
            else if (x != s) { ans.push_back({s, e}); s = e = x; }
        }
        ans.push_back({s, e});
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain disjoint intervals as numbers are added dynamically (LC 352).
// - Approach: Sorted Set + Linear Interval grouping.
// - Intuition:
//   * An ordered set automatically stores elements in ascending unique order with logarithmic insertion.
//   * Traversing the sorted elements from left to right allows merging contiguous integers into closed intervals [s, e].
//   * If current element x equals e + 1, it extends the current interval to e = x.
//   * Otherwise, save the completed interval [s, e] and start a new range with s = e = x.
// - Complexity: Time: O(log N) for addNum, O(N) for getIntervals, Space: O(N).


// ============================================================
// 3. WATER AND JUG PROBLEM (BFS) — LeetCode 365
// ============================================================

bool canMeasureWater(int x, int y, int t) {
    if (t > x + y) return false;
    queue<pair<int, int>> q; set<pair<int, int>> vis;
    q.push({0, 0}); vis.insert({0, 0});
    while (!q.empty()) {
        auto [a, b] = q.front(); q.pop();
        if (a + b == t) return true;
        vector<pair<int, int>> nxt = {
            {x, b}, {a, y}, {0, b}, {a, 0},
            {max(0, a - (y - b)), min(y, b + a)}, {min(x, a + b), max(0, b - (x - a))}
        };
        for (auto state : nxt) if (!vis.count(state)) { vis.insert(state); q.push(state); }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if exactly target liters can be measured using two jugs (LC 365).
// - Approach: BFS state-space search over 6 deterministic transitions.
// - Intuition:
//   * State of the system at any moment is uniquely identified by the pair (a, b) of current water volumes in both jugs.
//   * From state (a, b), exactly 6 operations are possible: fill jug 1, fill jug 2, empty jug 1, empty jug 2, pour jug 1 -> 2, pour jug 2 -> 1.
//   * Breadth-First Search (BFS) explores the state graph level-by-level with a visited set preventing infinite cycles.
//   * Target is achievable as soon as any reachable state has total volume a + b == target.
// - Complexity: Time: O(X * Y), Space: O(X * Y).


// ============================================================
// 4. MOVING AVERAGE FROM DATA STREAM — LeetCode 346
// ============================================================

class MovingAverage {
    queue<int> q;
    int cap;
    double sum;
public:
    MovingAverage(int sz) : cap(sz), sum(0.0) {}

    double next(int x) {
        q.push(x);
        sum += x;
        if ((int)q.size() > cap) {
            sum -= q.front();
            q.pop();
        }
        return sum / q.size();
    }
};
// Interview Explanation:
// - Problem Statement: Calculate the moving average of integers in sliding window of size w (LeetCode 346).
// - Approach: Sliding FIFO Queue with Running Window Sum.
// - Intuition:
//   * Recomputing the sum over the window naively takes O(W) per call.
//   * Maintain a running sum and a FIFO queue of window elements.
//   * Push incoming value x and add it to sum.
//   * If queue size exceeds capacity, subtract the oldest element `q.front()` from sum and pop it from queue, achieving strict O(1) time.
// - Complexity: Time: O(1) per `next`, Space: O(W).


// ============================================================
// 5. FIRST UNIQUE NUMBER IN DATA STREAM — LeetCode 1429
// ============================================================

class FirstUnique {
    list<int> dll;
    unordered_map<int, list<int>::iterator> mp;
    unordered_set<int> dup;
public:
    FirstUnique(vi& a) {
        for (int x : a) add(x);
    }

    int showFirstUnique() {
        return dll.empty() ? -1 : dll.front();
    }

    void add(int x) {
        if (dup.count(x)) return;
        if (mp.count(x)) {
            dll.erase(mp[x]);
            mp.erase(x);
            dup.insert(x);
        } else {
            dll.push_back(x);
            mp[x] = prev(dll.end());
        }
    }
};
// Interview Explanation:
// - Problem Statement: Maintain data stream to return first unique integer added in O(1) (LeetCode 1429).
// - Approach: Doubly Linked List + Hash Map (Node Iterator Locator) + Duplicate Set.
// - Intuition:
//   * Maintain ordering of currently unique elements in a doubly linked list (DLL) so the first unique is always at `dll.front()`.
//   * A hash map `mp` maps each unique value to its iterator in the DLL for O(1) splice/erasure.
//   * A set `dup` stores numbers that have already appeared more than once.
//   * When adding x: if in `dup`, ignore; if already in `mp`, erase from DLL, remove from map, and add to `dup`; else push to DLL.
// - Complexity: Time: O(1) for both `showFirstUnique` and `add`, Space: O(N).


// ============================================================
// 6. FIRST NON-REPEATING CHARACTER IN A STREAM — GFG / LeetCode
// ============================================================

class StreamFirstNonRepeatingChar {
    int cnt[26] = {0};
    queue<char> q;
public:
    StreamFirstNonRepeatingChar() {}

    char add(char c) {
        cnt[c - 'a']++;
        q.push(c);
        while (!q.empty() && cnt[q.front() - 'a'] > 1) {
            q.pop();
        }
        return q.empty() ? '#' : q.front();
    }
};
// Interview Explanation:
// - Problem Statement: Find first non-repeating character after reading each character in stream.
// - Approach: Frequency Array + Queue with Lazy Pop.
// - Intuition:
//   * Append every incoming character to a FIFO queue and increment its frequency counter.
//   * Characters at the front of the queue that have frequency > 1 are permanently invalid and will never become unique again.
//   * Pop stale characters lazily from the front until the front character has frequency == 1 or queue becomes empty.
//   * Each character enters and leaves the queue at most once, yielding O(1) amortized cost.
// - Complexity: Time: O(1) amortized per character, Space: O(1).


// ============================================================
// 7. DESIGN HIT COUNTER — LeetCode 362
// ============================================================

class HitCounter {
    int t[300];
    int h[300];
public:
    HitCounter() {
        fill(t, t + 300, 0);
        fill(h, h + 300, 0);
    }

    void hit(int ts) {
        int idx = ts % 300;
        if (t[idx] != ts) {
            t[idx] = ts;
            h[idx] = 1;
        } else {
            h[idx]++;
        }
    }

    int getHits(int ts) {
        int tot = 0;
        for (int i = 0; i < 300; i++) {
            if (ts - t[i] < 300) {
                tot += h[i];
            }
        }
        return tot;
    }
};
// Interview Explanation:
// - Problem Statement: Count number of hits received in past 5 minutes (300 seconds) (LeetCode 362).
// - Approach: Circular Fixed-Size Buffer (300 Buckets).
// - Intuition:
//   * Storing individual hit timestamps in a queue can consume unbounded memory under heavy traffic bursts.
//   * A circular buffer of size 300 aggregates hits occurring within the same second via index `ts % 300`.
//   * When recording a hit, if `t[idx]` does not match `ts`, the slot holds stale data from 300+ seconds ago; overwrite `t[idx] = ts` and reset `h[idx] = 1`.
//   * Summing active hits with `ts - t[i] < 300` across all 300 slots takes constant O(1) time and memory.
// - Complexity: Time: O(1) for `hit`, O(1) for `getHits`, Space: O(1).


// ============================================================
// 8. LOGGER RATE LIMITER — LeetCode 359
// ============================================================

class Logger {
    unordered_map<string, int> mp;
public:
    Logger() {}

    bool shouldPrintMessage(int ts, string s) {
        if (!mp.count(s) || ts >= mp[s]) {
            mp[s] = ts + 10;
            return true;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Print message only if not printed in last 10 seconds (LeetCode 359).
// - Approach: Hash Map with Next Available Timestamp.
// - Intuition:
//   * Map each unique string message to the earliest timestamp at which it is eligible to be printed again.
//   * If message s is absent or `ts >= mp[s]`, allow printing and update threshold to `ts + 10`.
//   * Otherwise, the message is throttled; return false without modifying the threshold.
// - Complexity: Time: O(1) per message, Space: O(M).


// ============================================================
// 9. DESIGN FRONT MIDDLE BACK QUEUE — LeetCode 1670
// ============================================================

class FrontMiddleBackQueue {
    deque<int> l, r;

    void balance() {
        if (l.size() > r.size() + 1) {
            r.push_front(l.back());
            l.pop_back();
        } else if (l.size() < r.size()) {
            l.push_back(r.front());
            r.pop_front();
        }
    }
public:
    FrontMiddleBackQueue() {}

    void pushFront(int val) {
        l.push_front(val);
        balance();
    }

    void pushMiddle(int val) {
        if (l.size() > r.size()) {
            r.push_front(l.back());
            l.pop_back();
        }
        l.push_back(val);
        balance();
    }

    void pushBack(int val) {
        r.push_back(val);
        balance();
    }

    int popFront() {
        if (l.empty()) return -1;
        int val = l.front();
        l.pop_front();
        balance();
        return val;
    }

    int popMiddle() {
        if (l.empty()) return -1;
        int val = l.back();
        l.pop_back();
        balance();
        return val;
    }

    int popBack() {
        if (l.empty()) return -1;
        int val;
        if (r.empty()) {
            val = l.back();
            l.pop_back();
        } else {
            val = r.back();
            r.pop_back();
        }
        balance();
        return val;
    }
};
// Interview Explanation:
// - Problem Statement: Design a queue supporting push/pop at front, middle, and back in O(1) (LeetCode 1670).
// - Approach: Two Balanced Deques (`l` and `r`).
// - Intuition:
//   * A single array or linked list cannot support O(1) middle insertions and deletions.
//   * Splitting elements into two halves `l` and `r` keeps the middle boundary accessible at `l.back()` or `r.front()`.
//   * Maintain invariant: `l.size() == r.size()` or `l.size() == r.size() + 1`.
//   * Rebalancing transfers a single element across the boundary in O(1) whenever size disparity exceeds 1.
// - Complexity: Time: O(1) for all operations, Space: O(N).


// ============================================================
// 10. FINDING MK AVERAGE — LeetCode 1825
// ============================================================

class MKAverage {
    int m, k;
    queue<int> q;
    multiset<int> l, m_st, r_st;
    ll sum;

    void add(int x) {
        l.insert(x);
        if ((int)l.size() > k) {
            auto it = prev(l.end());
            m_st.insert(*it);
            sum += *it;
            l.erase(it);
        }
        if ((int)m_st.size() > m - 2 * k) {
            auto it = prev(m_st.end());
            sum -= *it;
            r_st.insert(*it);
            m_st.erase(it);
        }
    }

    void remove(int x) {
        if (x <= *prev(l.end())) {
            l.erase(l.find(x));
            auto it = m_st.begin();
            l.insert(*it);
            sum -= *it;
            m_st.erase(it);
            auto it2 = r_st.begin();
            m_st.insert(*it2);
            sum += *it2;
            r_st.erase(it2);
        } else if (x <= *prev(m_st.end())) {
            m_st.erase(m_st.find(x));
            sum -= x;
            auto it = r_st.begin();
            m_st.insert(*it);
            sum += *it;
            r_st.erase(it);
        } else {
            r_st.erase(r_st.find(x));
        }
    }
public:
    MKAverage(int m, int k) : m(m), k(k), sum(0) {}

    void addElement(int x) {
        q.push(x);
        if ((int)q.size() <= m) {
            add(x);
        } else {
            add(x);
            remove(q.front());
            q.pop();
        }
    }

    int calculateMKAverage() {
        if ((int)q.size() < m) return -1;
        return sum / (m - 2 * k);
    }
};
// Interview Explanation:
// - Problem Statement: Calculate average of smallest m elements after removing k smallest and k largest (LeetCode 1825).
// - Approach: Three Balanced Multisets (`l`, `m_st`, `r_st`) + Sliding FIFO Queue.
// - Intuition:
//   * The last m elements are partitioned into three contiguous sorted partitions: `l` (size k), `m_st` (size m - 2k), and `r_st` (size k).
//   * A running scalar `sum` maintains the exact sum of elements in `m_st`.
//   * Adding/removing an element shifts boundary elements between adjacent multisets to maintain fixed target sizes.
//   * Calculating MK average runs in O(1) via integer division `sum / (m - 2 * k)`.
// - Complexity: Time: O(log M) per `addElement`, O(1) for `calculateMKAverage`, Space: O(M).


// ============================================================
// 11. SNAPSHOT ARRAY — LeetCode 1146
// ============================================================

class SnapshotArray {
    vector<vector<pair<int, int>>> hist;
    int snapId;
public:
    SnapshotArray(int len) : hist(len), snapId(0) {
        for (int i = 0; i < len; i++) {
            hist[i].push_back({0, 0});
        }
    }

    void set(int idx, int val) {
        if (hist[idx].back().first == snapId) {
            hist[idx].back().second = val;
        } else {
            hist[idx].push_back({snapId, val});
        }
    }

    int snap() {
        return snapId++;
    }

    int get(int idx, int id) {
        auto& h = hist[idx];
        auto it = upper_bound(h.begin(), h.end(), make_pair(id, INT_MAX));
        return prev(it)->second;
    }
};
// Interview Explanation:
// - Problem Statement: Array supporting set, snap, and historical snapshot queries (LeetCode 1146).
// - Approach: History Vectors + Binary Search (`upper_bound`).
// - Intuition:
//   * Storing a complete copy of the array on each snap consumes O(N * S) memory and causes TLE.
//   * Instead, store change history per index: a list of `{snap_id, val}` pairs.
//   * Mutating a value appends `{snap_id, val}` or updates the trailing entry if snap_id matches current epoch.
//   * Querying snapshot id uses binary search (`upper_bound`) to find the latest version recorded at or before id.
// - Complexity: Time: O(1) set/snap, O(log S) get, Space: O(N + Total Mutates).


// ============================================================
// 12. STOCK PRICE FLUCTUATION — LeetCode 2034
// ============================================================

class StockPrice {
    unordered_map<int, int> mp;
    multiset<int> st;
    int maxT;
public:
    StockPrice() : maxT(0) {}

    void update(int ts, int price) {
        maxT = max(maxT, ts);
        if (mp.count(ts)) {
            int old = mp[ts];
            st.erase(st.find(old));
        }
        mp[ts] = price;
        st.insert(price);
    }

    int current() {
        return mp[maxT];
    }

    int maximum() {
        return *st.rbegin();
    }

    int minimum() {
        return *st.begin();
    }
};
// Interview Explanation:
// - Problem Statement: Process stock prices with corrections; query current, max, and min prices (LeetCode 2034).
// - Approach: Hash Map + Balanced Multiset.
// - Intuition:
//   * A hash map tracks current mappings `{timestamp -> price}`.
//   * Updates can modify previously entered timestamps (out of order price corrections).
//   * When a timestamp is revised, erase one instance of its old price from multiset and insert the new price.
//   * Track `maxT` for O(1) current price lookup; multiset endpoints yield O(1) maximum (`*st.rbegin()`) and minimum (`*st.begin()`).
// - Complexity: Time: O(log N) for `update`, O(1) for `current`, `max`, `min`, Space: O(N).


// ============================================================
// 13. SEAT RESERVATION MANAGER — LeetCode 1845
// ============================================================

class SeatManager {
    priority_queue<int, vector<int>, greater<int>> pq;
public:
    SeatManager(int n) {
        for (int i = 1; i <= n; i++) pq.push(i);
    }

    int reserve() {
        int s = pq.top();
        pq.pop();
        return s;
    }

    void unreserve(int s) {
        pq.push(s);
    }
};
// Interview Explanation:
// - Problem Statement: Manage reservations for n seats, always assigning lowest-numbered free seat (LeetCode 1845).
// - Approach: Min-Heap of Available Seat IDs.
// - Intuition:
//   * Seats must always be assigned in strictly ascending numerical order.
//   * A min-heap (`std::greater<int>`) maintains free seat numbers.
//   * `reserve()` pops the minimum seat number in O(log N).
//   * `unreserve(s)` re-inserts seat s into the min-heap in O(log N).
// - Complexity: Time: O(log N) per operation, Space: O(N).


// ============================================================
// 14. DESIGN UNDERGROUND SYSTEM — LeetCode 1396
// ============================================================

class UndergroundSystem {
    unordered_map<int, pair<string, int>> inMp;
    unordered_map<string, pair<ll, int>> statMp;
public:
    UndergroundSystem() {}

    void checkIn(int id, string sName, int t) {
        inMp[id] = {sName, t};
    }

    void checkOut(int id, string sName, int t) {
        auto [sStation, sTime] = inMp[id];
        inMp.erase(id);

        string route = sStation + ">" + sName;
        statMp[route].first += (t - sTime);
        statMp[route].second++;
    }

    double getAverageTime(string sStation, string eStation) {
        string route = sStation + ">" + eStation;
        auto [totTime, cnt] = statMp[route];
        return (double)totTime / cnt;
    }
};
// Interview Explanation:
// - Problem Statement: Track customer travel times between underground stations and compute averages (LeetCode 1396).
// - Approach: Dual Hash Maps (`inMp` and `statMp`).
// - Intuition:
//   * Passengers travel concurrently: store active check-ins in `inMp` mapping `id -> {station, time}`.
//   * On checkout, retrieve check-in details, erase passenger record, and form composite route key `startStation>endStation`.
//   * Accumulate total transit duration and passenger trip count in `statMp[route]`.
//   * Query average time runs in strict O(1) by dividing total accumulated time by trip count.
// - Complexity: Time: O(1) for all ops, Space: O(Passengers + Stations^2).


// ============================================================
// 15. NUMBER OF RECENT CALLS — LeetCode 933
// ============================================================

class RecentCounter {
    queue<int> q;
public:
    RecentCounter() {}

    int ping(int t) {
        q.push(t);
        while (!q.empty() && q.front() < t - 3000) {
            q.pop();
        }
        return q.size();
    }
};
// Interview Explanation:
// - Problem Statement: Count number of recent requests within time frame [t - 3000, t] (LeetCode 933).
// - Approach: Sliding Window FIFO Queue.
// - Intuition:
//   * Input timestamps arrive in strictly increasing chronological order.
//   * Push timestamp t to the back of the queue.
//   * Expired calls with timestamp `< t - 3000` will never fall into future sliding windows and can be safely discarded.
//   * Pop front elements until `q.front() >= t - 3000`. Size of queue represents calls within the past 3000ms.
// - Complexity: Time: O(1) amortized per `ping`, Space: O(W).


// ============================================================
// 16. DESIGN LEADERBOARD — LeetCode 1244
// ============================================================

class Leaderboard {
    unordered_map<int, int> mp;
public:
    Leaderboard() {}

    void addScore(int id, int s) {
        mp[id] += s;
    }

    int top(int K) {
        priority_queue<int, vi, greater<int>> pq;
        for (auto& [id, s] : mp) {
            pq.push(s);
            if ((int)pq.size() > K) pq.pop();
        }
        int tot = 0;
        while (!pq.empty()) {
            tot += pq.top();
            pq.pop();
        }
        return tot;
    }

    void reset(int id) {
        mp.erase(id);
    }
};
// Interview Explanation:
// - Problem Statement: Leaderboard supporting score additions, top K scores sum, and score resets (LeetCode 1244).
// - Approach: Hash Map + Min-Heap Top-K Extraction.
// - Intuition:
//   * A hash map stores total aggregated scores per player id.
//   * `addScore` and `reset` execute in O(1) average time.
//   * For `top(K)`, maintain a min-heap bounded by size K while iterating over all player scores.
//   * Popping the smallest when size exceeds K leaves the K largest scores in heap, taking O(N log K) time and O(K) extra space.
// - Complexity: Time: O(1) add/reset, O(N log K) top, Space: O(Players).


// ============================================================
// 17. BOUNDED BLOCKING QUEUE — LeetCode 1188
// ============================================================

class BoundedBlockingQueue {
    queue<int> q;
    int cap;
    mutex mtx;
    condition_variable cvFull;
    condition_variable cvEmpty;
public:
    BoundedBlockingQueue(int cap) : cap(cap) {}

    void enqueue(int x) {
        unique_lock<mutex> lock(mtx);
        while ((int)q.size() >= cap) {
            cvFull.wait(lock);
        }
        q.push(x);
        cvEmpty.notify_one();
    }

    int dequeue() {
        unique_lock<mutex> lock(mtx);
        while (q.empty()) {
            cvEmpty.wait(lock);
        }
        int val = q.front();
        q.pop();
        cvFull.notify_one();
        return val;
    }

    int size() {
        lock_guard<mutex> lock(mtx);
        return q.size();
    }
};
// Interview Explanation:
// - Problem Statement: Design thread-safe bounded blocking queue (LeetCode 1188).
// - Approach: Mutex + Dual Condition Variables (`cvFull`, `cvEmpty`).
// - Intuition:
//   * Producer threads calling `enqueue` must block when queue reaches maximum capacity.
//   * Consumer threads calling `dequeue` must block when queue is empty.
//   * Condition variables suspend calling threads without CPU busy-waiting (spinning).
//   * `enqueue` signals `cvEmpty` to wake up blocked consumers; `dequeue` signals `cvFull` to wake up waiting producers.
// - Complexity: Time: O(1) per operation, Space: O(Capacity).


// ============================================================
// 18. MAJORITY CHECKER IN SUBARRAY — LeetCode 1154
// ============================================================

class MajorityChecker {
    vi a;
    unordered_map<int, vi> pos;
public:
    MajorityChecker(vi& a) : a(a) {
        for (int i = 0; i < (int)a.size(); i++) {
            pos[a[i]].push_back(i);
        }
    }

    int query(int l, int r, int th) {
        int len = r - l + 1;
        for (int it = 0; it < 20; it++) {
            int randIdx = l + rand() % len;
            int cand = a[randIdx];
            auto& idxs = pos[cand];

            int cnt = upper_bound(idxs.begin(), idxs.end(), r) -
                      lower_bound(idxs.begin(), idxs.end(), l);

            if (cnt >= th) return cand;
        }
        return -1;
    }
};
// Interview Explanation:
// - Problem Statement: Query if any element in subarray occurs >= threshold times (LeetCode 1154).
// - Approach: Randomized Monte Carlo Sampling + Binary Search Frequency Verification.
// - Intuition:
//   * If a majority element exists, it constitutes at least half the subarray elements.
//   * Picking a random element from subarray has >= 50% probability of selecting the majority candidate.
//   * Repeating 20 random trials reduces error probability of missing an existing majority element to (1/2)^20 < 10^-6.
//   * Verify each candidate's exact count in [l, r] in O(log N) using `upper_bound - lower_bound` on precomputed occurrence indices.
// - Complexity: Time: O(20 * log N) per query, Space: O(N).


// ============================================================
// 19. DESIGN AUTHENTICATION MANAGER — LeetCode 1797
// ============================================================

class AuthenticationManager {
    int ttl;
    unordered_map<string, int> mp;
public:
    AuthenticationManager(int ttl) : ttl(ttl) {}

    void generate(string id, int t) {
        mp[id] = t + ttl;
    }

    void renew(string id, int t) {
        if (mp.count(id) && mp[id] > t) {
            mp[id] = t + ttl;
        }
    }

    int countUnexpiredTokens(int t) {
        int ans = 0;
        for (auto& [id, exp] : mp) {
            if (exp > t) ans++;
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Authentication manager supporting token generation, renewal, and active counting (LeetCode 1797).
// - Approach: Hash Map of Token Expiry Timestamps.
// - Intuition:
//   * Store each token's absolute expiration timestamp `currentTime + timeToLive`.
//   * `renew` checks if token exists and is strictly unexpired (`expiry > currentTime`); if valid, push expiration forward.
//   * `countUnexpiredTokens` iterates over map and tallies tokens whose expiration exceeds current query timestamp.
// - Complexity: Time: O(1) generate/renew, O(Tokens) count, Space: O(Tokens).


// ============================================================
// 20. MULTI-POLICY ONLINE CACHE ENGINE (LRU + TTL)
// ============================================================

class OnlineCacheEngine {
    int cap;
    list<pair<string, string>> dll;
    unordered_map<string, pair<list<pair<string, string>>::iterator, int>> mp;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    void purge(int t) {
        while (!pq.empty() && pq.top().first <= t) {
            string k = pq.top().second;
            pq.pop();
            if (mp.count(k) && mp[k].second <= t) {
                dll.erase(mp[k].first);
                mp.erase(k);
            }
        }
    }
public:
    OnlineCacheEngine(int cap) : cap(cap) {}

    string get(string k, int t) {
        purge(t);
        if (!mp.count(k)) return "";
        auto it = mp[k].first;
        string val = it->second;
        dll.splice(dll.begin(), dll, it);
        return val;
    }

    void put(string k, string v, int ttl, int t) {
        purge(t);
        int exp = t + ttl;

        if (mp.count(k)) {
            auto it = mp[k].first;
            it->second = v;
            dll.splice(dll.begin(), dll, it);
            mp[k].second = exp;
        } else {
            if ((int)mp.size() == cap) {
                string evict = dll.back().first;
                dll.pop_back();
                mp.erase(evict);
            }
            dll.push_front({k, v});
            mp[k] = {dll.begin(), exp};
        }
        pq.push({exp, k});
    }
};
// Interview Explanation:
// - Problem Statement: Design online cache supporting get, put, LRU eviction, and TTL expiration.
// - Approach: Doubly Linked List (LRU Recency) + Hash Map + Expiration Min-Heap.
// - Intuition:
//   * Dual eviction policies: capacity overflow evicts Least Recently Used (LRU), while time expiration evicts stale TTL keys.
//   * A doubly linked list keeps recency order; `splice` moves accessed items to head in O(1).
//   * A min-heap indexed by `{expiration_timestamp, key}` detects expired keys lazily at start of every operation.
//   * If capacity limit is reached upon inserting a new key, remove tail node from DLL and erase from map.
// - Complexity: Time: O(1) amortized for `get` and `put`, Space: O(Capacity).
