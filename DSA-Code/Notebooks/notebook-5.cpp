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
 | 14 | Design Underground System (LeetCode 1396)   | Dual Hash Maps (Transit & Stats)  | O(1) all | O(P + S2)|
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
    static const int SIZE = 1000;
    vector<list<pair<int, int>>> buckets;
    int hash(int key) { return key % SIZE; }
public:
    MyHashMap() : buckets(SIZE) {}
    void put(int key, int value) {
        int idx = hash(key);
        for (auto& [k, v] : buckets[idx]) if (k == key) { v = value; return; }
        buckets[idx].push_back({key, value});
    }
    int get(int key) {
        for (auto& [k, v] : buckets[hash(key)]) if (k == key) return v;
        return -1;
    }
    void remove(int key) {
        int idx = hash(key);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it)
            if (it->first == key) { buckets[idx].erase(it); return; }
    }
};
// Interview Explanation:
// - Problem Statement: Implement a basic HashMap with put, get, and remove operations (LC 706).
// - Approach: Chaining via array of linked lists (buckets).
// - Intuition: Hash function maps keys to bucket indices; separate chaining handles collisions.
// - Complexity: Time: O(1) average, Space: O(N).


// ============================================================
// 2. SUMMARY RANGES — LeetCode 228 / 352
// ============================================================

class SummaryRanges {
    set<int> nums;
public:
    SummaryRanges() {}
    void addNum(int val) { nums.insert(val); }
    vector<vector<int>> getIntervals() {
        if (nums.empty()) return {};
        vector<vector<int>> intervals;
        int start = *nums.begin(), end = start;
        for (int x : nums) {
            if (x == end + 1) end = x;
            else if (x != start) { intervals.push_back({start, end}); start = end = x; }
        }
        intervals.push_back({start, end});
        return intervals;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain disjoint intervals as numbers are added dynamically (LC 352).
// - Approach: Sorted Set + Linear Interval grouping.
// - Intuition: Iterate through sorted unique values and group consecutive numbers into [start, end].
// - Complexity: Time: O(log N) for addNum, O(N) for getIntervals, Space: O(N).


// ============================================================
// 3. WATER AND JUG PROBLEM (BFS) — LeetCode 365
// ============================================================

bool canMeasureWater(int x, int y, int target) {
    if (target > x + y) return false;
    queue<pair<int, int>> q; set<pair<int, int>> vis;
    q.push({0, 0}); vis.insert({0, 0});
    while (!q.empty()) {
        auto [a, b] = q.front(); q.pop();
        if (a + b == target) return true;
        vector<pair<int, int>> next = {
            {x, b}, {a, y}, {0, b}, {a, 0},
            {max(0, a - (y - b)), min(y, b + a)}, {min(x, a + b), max(0, b - (x - a))}
        };
        for (auto state : next) if (!vis.count(state)) { vis.insert(state); q.push(state); }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if exactly target liters can be measured using two jugs (LC 365).
// - Approach: BFS state-space search over 6 transitions (fill, empty, pour).
// - Intuition: Model each jug state as (a, b) and traverse reachable states using BFS with visited set.
// - Complexity: Time: O(x * y), Space: O(x * y).


// ============================================================
// 4. MOVING AVERAGE FROM DATA STREAM — LeetCode 346
// ============================================================

class MovingAverage {
    queue<int> q;
    int maxSize;
    double sum;
public:
    MovingAverage(int size) : maxSize(size), sum(0.0) {}

    double next(int val) {
        q.push(val);
        sum += val;
        if ((int)q.size() > maxSize) {
            sum -= q.front();
            q.pop();
        }
        return sum / q.size();
    }
};
// Interview Explanation:
// - Problem Statement: Calculate the moving average of integers in sliding window of size w (LeetCode 346).
// - Approach: Sliding FIFO Queue with Running Window Sum.
// - Intuition: Push incoming values and add to running sum; when size exceeds w, pop oldest from queue.
// - Complexity: Time: O(1) per `next`, Space: O(W).


// ============================================================
// 5. FIRST UNIQUE NUMBER IN DATA STREAM — LeetCode 1429
// ============================================================

class FirstUnique {
    list<int> dll;
    unordered_map<int, list<int>::iterator> nodeMap;
    unordered_set<int> duplicates;
public:
    FirstUnique(vector<int>& nums) {
        for (int x : nums) add(x);
    }

    int showFirstUnique() {
        return dll.empty() ? -1 : dll.front();
    }

    void add(int value) {
        if (duplicates.count(value)) return;
        if (nodeMap.count(value)) {
            dll.erase(nodeMap[value]);
            nodeMap.erase(value);
            duplicates.insert(value);
        } else {
            dll.push_back(value);
            nodeMap[value] = prev(dll.end());
        }
    }
};
// Interview Explanation:
// - Problem Statement: Maintain data stream to return first unique integer added in O(1) (LeetCode 1429).
// - Approach: Doubly Linked List + Hash Map (Iterator Locator) + Duplicate Set.
// - Intuition: Store unique items in DLL order; on repeat, erase node from DLL via stored iterator.
// - Complexity: Time: O(1) for both `showFirstUnique` and `add`, Space: O(N).


// ============================================================
// 6. FIRST NON-REPEATING CHARACTER IN A STREAM — GFG / LeetCode
// ============================================================

class StreamFirstNonRepeatingChar {
    int freq[26] = {0};
    queue<char> q;
public:
    StreamFirstNonRepeatingChar() {}

    char add(char c) {
        freq[c - 'a']++;
        q.push(c);
        while (!q.empty() && freq[q.front() - 'a'] > 1) {
            q.pop();
        }
        return q.empty() ? '#' : q.front();
    }
};
// Interview Explanation:
// - Problem Statement: Find first non-repeating character after reading each character in stream.
// - Approach: Frequency Array + Queue with Lazy Pop.
// - Intuition: Increment frequency and push to queue; discard front elements whose frequency > 1.
// - Complexity: Time: O(1) amortized per character, Space: O(1).


// ============================================================
// 7. DESIGN HIT COUNTER — LeetCode 362
// ============================================================

class HitCounter {
    int times[300];
    int hits[300];
public:
    HitCounter() {
        fill(times, times + 300, 0);
        fill(hits, hits + 300, 0);
    }

    void hit(int timestamp) {
        int idx = timestamp % 300;
        if (times[idx] != timestamp) {
            times[idx] = timestamp;
            hits[idx] = 1;
        } else {
            hits[idx]++;
        }
    }

    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) {
                total += hits[i];
            }
        }
        return total;
    }
};
// Interview Explanation:
// - Problem Statement: Count number of hits received in past 5 minutes (300 seconds) (LeetCode 362).
// - Approach: Circular Fixed-Size Buffer (300 Buckets).
// - Intuition: Array of size 300 indexed by `timestamp % 300`. Reset count if timestamp is outdated.
// - Complexity: Time: O(1) for `hit`, O(1) for `getHits`, Space: O(1).


// ============================================================
// 8. LOGGER RATE LIMITER — LeetCode 359
// ============================================================

class Logger {
    unordered_map<string, int> msgTimestamps;
public:
    Logger() {}

    bool shouldPrintMessage(int timestamp, string message) {
        if (!msgTimestamps.count(message) || timestamp >= msgTimestamps[message]) {
            msgTimestamps[message] = timestamp + 10;
            return true;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Print message only if not printed in last 10 seconds (LeetCode 359).
// - Approach: Hash Map with Next Available Timestamp.
// - Intuition: Store earliest timestamp at which message is eligible to print again.
// - Complexity: Time: O(1) per message, Space: O(M).


// ============================================================
// 9. DESIGN FRONT MIDDLE BACK QUEUE — LeetCode 1670
// ============================================================

class FrontMiddleBackQueue {
    deque<int> left, right;

    void balance() {
        if (left.size() > right.size() + 1) {
            right.push_front(left.back());
            left.pop_back();
        } else if (left.size() < right.size()) {
            left.push_back(right.front());
            right.pop_front();
        }
    }
public:
    FrontMiddleBackQueue() {}

    void pushFront(int val) {
        left.push_front(val);
        balance();
    }

    void pushMiddle(int val) {
        if (left.size() > right.size()) {
            right.push_front(left.back());
            left.pop_back();
        }
        left.push_back(val);
        balance();
    }

    void pushBack(int val) {
        right.push_back(val);
        balance();
    }

    int popFront() {
        if (left.empty()) return -1;
        int val = left.front();
        left.pop_front();
        balance();
        return val;
    }

    int popMiddle() {
        if (left.empty()) return -1;
        int val = left.back();
        left.pop_back();
        balance();
        return val;
    }

    int popBack() {
        if (left.empty()) return -1;
        int val;
        if (right.empty()) {
            val = left.back();
            left.pop_back();
        } else {
            val = right.back();
            right.pop_back();
        }
        balance();
        return val;
    }
};
// Interview Explanation:
// - Problem Statement: Design a queue supporting push/pop at front, middle, and back in O(1) (LeetCode 1670).
// - Approach: Two Balanced Deques (`left` and `right`).
// - Intuition: Middle element is always `left.back()`. Maintain balance invariant across both deques.
// - Complexity: Time: O(1) for all operations, Space: O(N).


// ============================================================
// 10. FINDING MK AVERAGE — LeetCode 1825
// ============================================================

class MKAverage {
    int m, k;
    queue<int> stream;
    multiset<int> left, mid, right;
    long long midSum;

    void add(int num) {
        left.insert(num);
        if ((int)left.size() > k) {
            auto it = prev(left.end());
            mid.insert(*it);
            midSum += *it;
            left.erase(it);
        }
        if ((int)mid.size() > m - 2 * k) {
            auto it = prev(mid.end());
            midSum -= *it;
            right.insert(*it);
            mid.erase(it);
        }
    }

    void remove(int num) {
        if (num <= *prev(left.end())) {
            left.erase(left.find(num));
            auto it = mid.begin();
            left.insert(*it);
            midSum -= *it;
            mid.erase(it);
            auto it2 = right.begin();
            mid.insert(*it2);
            midSum += *it2;
            right.erase(it2);
        } else if (num <= *prev(mid.end())) {
            mid.erase(mid.find(num));
            midSum -= num;
            auto it = right.begin();
            mid.insert(*it);
            midSum += *it;
            right.erase(it);
        } else {
            right.erase(right.find(num));
        }
    }
public:
    MKAverage(int m, int k) : m(m), k(k), midSum(0) {}

    void addElement(int num) {
        stream.push(num);
        if ((int)stream.size() <= m) {
            add(num);
        } else {
            add(num);
            remove(stream.front());
            stream.pop();
        }
    }

    int calculateMKAverage() {
        if ((int)stream.size() < m) return -1;
        return midSum / (m - 2 * k);
    }
};
// Interview Explanation:
// - Problem Statement: Calculate average of smallest m elements after removing k smallest and k largest (LeetCode 1825).
// - Approach: Three Multisets (`left`, `mid`, `right`) + Sliding Queue of size m.
// - Intuition: Maintain running sum of middle multiset `midSum`; cascade elements across set boundaries.
// - Complexity: Time: O(log M) per `addElement`, O(1) for `calculateMKAverage`, Space: O(M).


// ============================================================
// 11. SNAPSHOT ARRAY — LeetCode 1146
// ============================================================

class SnapshotArray {
    vector<vector<pair<int, int>>> history;
    int snapCount;
public:
    SnapshotArray(int length) : history(length), snapCount(0) {
        for (int i = 0; i < length; i++) {
            history[i].push_back({0, 0});
        }
    }

    void set(int index, int val) {
        if (history[index].back().first == snapCount) {
            history[index].back().second = val;
        } else {
            history[index].push_back({snapCount, val});
        }
    }

    int snap() {
        return snapCount++;
    }

    int get(int index, int snap_id) {
        auto& hist = history[index];
        auto it = upper_bound(hist.begin(), hist.end(), make_pair(snap_id, INT_MAX));
        return prev(it)->second;
    }
};
// Interview Explanation:
// - Problem Statement: Array supporting set, snap, and historical snapshot queries (LeetCode 1146).
// - Approach: History Vectors + Binary Search (`upper_bound`).
// - Intuition: Record `{snap_id, val}` only when mutated; binary search returns most recent record <= snap_id.
// - Complexity: Time: O(1) set/snap, O(log S) get, Space: O(N + U).


// ============================================================
// 12. STOCK PRICE FLUCTUATION — LeetCode 2034
// ============================================================

class StockPrice {
    unordered_map<int, int> timestampPriceMap;
    multiset<int> prices;
    int latestTimestamp;
public:
    StockPrice() : latestTimestamp(0) {}

    void update(int timestamp, int price) {
        latestTimestamp = max(latestTimestamp, timestamp);
        if (timestampPriceMap.count(timestamp)) {
            int oldPrice = timestampPriceMap[timestamp];
            prices.erase(prices.find(oldPrice));
        }
        timestampPriceMap[timestamp] = price;
        prices.insert(price);
    }

    int current() {
        return timestampPriceMap[latestTimestamp];
    }

    int maximum() {
        return *prices.rbegin();
    }

    int minimum() {
        return *prices.begin();
    }
};
// Interview Explanation:
// - Problem Statement: Process stock prices with corrections; query current, max, and min prices (LeetCode 2034).
// - Approach: Hash Map + Balanced Multiset.
// - Intuition: Map tracks `{timestamp -> price}`; multiset keeps prices sorted for O(1) min/max queries.
// - Complexity: Time: O(log N) for `update`, O(1) for `current`, `max`, `min`, Space: O(N).


// ============================================================
// 13. SEAT RESERVATION MANAGER — LeetCode 1845
// ============================================================

class SeatManager {
    priority_queue<int, vector<int>, greater<int>> freeSeats;
public:
    SeatManager(int n) {
        for (int i = 1; i <= n; i++) freeSeats.push(i);
    }

    int reserve() {
        int seat = freeSeats.top();
        freeSeats.pop();
        return seat;
    }

    void unreserve(int seatNumber) {
        freeSeats.push(seatNumber);
    }
};
// Interview Explanation:
// - Problem Statement: Manage reservations for n seats, always assigning lowest-numbered free seat (LeetCode 1845).
// - Approach: Min-Heap of Available Seat IDs.
// - Intuition: Min-heap guarantees O(log N) retrieval of lowest available seat number.
// - Complexity: Time: O(log N) per op, Space: O(N).


// ============================================================
// 14. DESIGN UNDERGROUND SYSTEM — LeetCode 1396
// ============================================================

class UndergroundSystem {
    unordered_map<int, pair<string, int>> checkInMap;
    unordered_map<string, pair<long long, int>> routeStats;
public:
    UndergroundSystem() {}

    void checkIn(int id, string stationName, int t) {
        checkInMap[id] = {stationName, t};
    }

    void checkOut(int id, string stationName, int t) {
        auto [startStation, startTime] = checkInMap[id];
        checkInMap.erase(id);

        string route = startStation + ">" + stationName;
        routeStats[route].first += (t - startTime);
        routeStats[route].second++;
    }

    double getAverageTime(string startStation, string endStation) {
        string route = startStation + ">" + endStation;
        auto [totalTime, count] = routeStats[route];
        return (double)totalTime / count;
    }
};
// Interview Explanation:
// - Problem Statement: Track customer travel times between underground stations and compute averages (LeetCode 1396).
// - Approach: Dual Hash Maps (`checkInMap` and `routeStats`).
// - Intuition: Record check-in times; upon check-out, accumulate duration and trip counts per route in O(1).
// - Complexity: Time: O(1) for all ops, Space: O(Passengers + Routes).


// ============================================================
// 15. NUMBER OF RECENT CALLS — LeetCode 933
// ============================================================

class RecentCounter {
    queue<int> calls;
public:
    RecentCounter() {}

    int ping(int t) {
        calls.push(t);
        while (!calls.empty() && calls.front() < t - 3000) {
            calls.pop();
        }
        return calls.size();
    }
};
// Interview Explanation:
// - Problem Statement: Count number of recent requests within time frame [t - 3000, t] (LeetCode 933).
// - Approach: Sliding Window FIFO Queue.
// - Intuition: Push incoming timestamp `t`; pop expired timestamps `< t - 3000`.
// - Complexity: Time: O(1) amortized per `ping`, Space: O(W).


// ============================================================
// 16. DESIGN LEADERBOARD — LeetCode 1244
// ============================================================

class Leaderboard {
    unordered_map<int, int> scores;
public:
    Leaderboard() {}

    void addScore(int playerId, int score) {
        scores[playerId] += score;
    }

    int top(int K) {
        priority_queue<int, vector<int>, greater<int>> minHeap;
        for (auto& [id, score] : scores) {
            minHeap.push(score);
            if ((int)minHeap.size() > K) minHeap.pop();
        }
        int total = 0;
        while (!minHeap.empty()) {
            total += minHeap.top();
            minHeap.pop();
        }
        return total;
    }

    void reset(int playerId) {
        scores.erase(playerId);
    }
};
// Interview Explanation:
// - Problem Statement: Leaderboard supporting score additions, top K scores sum, and score resets (LeetCode 1244).
// - Approach: Hash Map + Min-Heap Top-K Extraction.
// - Intuition: `scores` tracks player totals; `top(K)` maintains min-heap of size K in O(N log K).
// - Complexity: Time: O(1) add/reset, O(N log K) top, Space: O(Players).


// ============================================================
// 17. BOUNDED BLOCKING QUEUE — LeetCode 1188
// ============================================================

class BoundedBlockingQueue {
    queue<int> q;
    int capacity;
    mutex mtx;
    condition_variable cv_not_full;
    condition_variable cv_not_empty;
public:
    BoundedBlockingQueue(int capacity) : capacity(capacity) {}

    void enqueue(int element) {
        unique_lock<mutex> lock(mtx);
        cv_not_full.wait(lock, [this]() { return (int)q.size() < capacity; });
        q.push(element);
        cv_not_empty.notify_one();
    }

    int dequeue() {
        unique_lock<mutex> lock(mtx);
        cv_not_empty.wait(lock, [this]() { return !q.empty(); });
        int val = q.front();
        q.pop();
        cv_not_full.notify_one();
        return val;
    }

    int size() {
        lock_guard<mutex> lock(mtx);
        return q.size();
    }
};
// Interview Explanation:
// - Problem Statement: Design thread-safe bounded blocking queue (LeetCode 1188).
// - Approach: Mutex + Two Condition Variables (`cv_not_full`, `cv_not_empty`).
// - Intuition: Threads block on condition variables until space/element is available, then signal consumers/producers.
// - Complexity: Time: O(1) per operation, Space: O(Capacity).


// ============================================================
// 18. MAJORITY CHECKER IN SUBARRAY — LeetCode 1154
// ============================================================

class MajorityChecker {
    vector<int> arr;
    unordered_map<int, vector<int>> pos;
public:
    MajorityChecker(vector<int>& arr) : arr(arr) {
        for (int i = 0; i < (int)arr.size(); i++) {
            pos[arr[i]].push_back(i);
        }
    }

    int query(int left, int right, int threshold) {
        int len = right - left + 1;
        for (int iter = 0; iter < 20; iter++) {
            int randIdx = left + rand() % len;
            int candidate = arr[randIdx];
            auto& indices = pos[candidate];

            int count = upper_bound(indices.begin(), indices.end(), right) -
                        lower_bound(indices.begin(), indices.end(), left);

            if (count >= threshold) return candidate;
        }
        return -1;
    }
};
// Interview Explanation:
// - Problem Statement: Query if any element in subarray occurs >= threshold times (LeetCode 1154).
// - Approach: Randomized Sampling + Binary Search Frequency Counting.
// - Intuition: 20 Monte Carlo samples give < 10^-6 failure rate; verify candidate frequency via binary search.
// - Complexity: Time: O(20 * log N) per query, Space: O(N).


// ============================================================
// 19. DESIGN AUTHENTICATION MANAGER — LeetCode 1797
// ============================================================

class AuthenticationManager {
    int timeToLive;
    unordered_map<string, int> tokens;
public:
    AuthenticationManager(int timeToLive) : timeToLive(timeToLive) {}

    void generate(string tokenId, int currentTime) {
        tokens[tokenId] = currentTime + timeToLive;
    }

    void renew(string tokenId, int currentTime) {
        if (tokens.count(tokenId) && tokens[tokenId] > currentTime) {
            tokens[tokenId] = currentTime + timeToLive;
        }
    }

    int countUnexpiredTokens(int currentTime) {
        int count = 0;
        for (auto& [id, expiry] : tokens) {
            if (expiry > currentTime) count++;
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Authentication manager supporting token generation, renewal, and active counting (LeetCode 1797).
// - Approach: Hash Map of Token Expiry Timestamps.
// - Intuition: Store expiry timestamp `currentTime + timeToLive`; verify `expiry > currentTime` on renewal.
// - Complexity: Time: O(1) generate/renew, O(N) count, Space: O(Tokens).


// ============================================================
// 20. MULTI-POLICY ONLINE CACHE ENGINE (LRU + TTL)
// ============================================================

class OnlineCacheEngine {
    int cap;
    list<pair<string, string>> lruList;
    unordered_map<string, pair<list<pair<string, string>>::iterator, int>> cache;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> expiryMinHeap;

    void purgeExpired(int currentTime) {
        while (!expiryMinHeap.empty() && expiryMinHeap.top().first <= currentTime) {
            string key = expiryMinHeap.top().second;
            expiryMinHeap.pop();
            if (cache.count(key) && cache[key].second <= currentTime) {
                lruList.erase(cache[key].first);
                cache.erase(key);
            }
        }
    }
public:
    OnlineCacheEngine(int capacity) : cap(capacity) {}

    string get(string key, int currentTime) {
        purgeExpired(currentTime);
        if (!cache.count(key)) return "";
        auto it = cache[key].first;
        string val = it->second;
        lruList.splice(lruList.begin(), lruList, it);
        return val;
    }

    void put(string key, string value, int ttl, int currentTime) {
        purgeExpired(currentTime);
        int expiry = currentTime + ttl;

        if (cache.count(key)) {
            auto it = cache[key].first;
            it->second = value;
            lruList.splice(lruList.begin(), lruList, it);
            cache[key].second = expiry;
        } else {
            if ((int)cache.size() == cap) {
                string evictKey = lruList.back().first;
                lruList.pop_back();
                cache.erase(evictKey);
            }
            lruList.push_front({key, value});
            cache[key] = {lruList.begin(), expiry};
        }
        expiryMinHeap.push({expiry, key});
    }
};
// Interview Explanation:
// - Problem Statement: Design online cache supporting get, put, LRU eviction, and TTL expiration.
// - Approach: Doubly Linked List (LRU Recency) + Hash Map + Expiration Min-Heap.
// - Intuition: DLL maintains access recency; min-heap prioritizes earliest expiring keys for lazy TTL purging.
// - Complexity: Time: O(1) amortized for `get` and `put`, Space: O(Capacity).
