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
 | 1  | Moving Average from Stream (LeetCode 346)   | Sliding Circular Buffer / Queue   | O(1)     | O(W)     |
 | 2  | First Unique Number in Stream (LC 1429)     | Doubly Linked List + Hash Map     | O(1)     | O(N)     |
 | 3  | First Non-Repeating Character in Stream     | Frequency Table + FIFO Queue      | O(1) avg | O(1)     |
 | 4  | Design Hit Counter (LeetCode 362)           | Circular Buckets (300s) / Queue   | O(1)     | O(1)     |
 | 5  | Logger Rate Limiter (LeetCode 359)          | Hash Map Expiration Timestamps    | O(1)     | O(M)     |
 | 6  | Front Middle Back Queue (LeetCode 1670)     | Dual Balanced Deques (Left/Right) | O(1) all | O(N)     |
 | 7  | Finding MK Average (LeetCode 1825)          | 3 Multisets (Left, Mid, Right)    | O(log M) | O(M)     |
 | 8  | Snapshot Array (LeetCode 1146)              | History Vectors + Binary Search   | O(log S) | O(N + U) |
 | 9  | Stock Price Fluctuation (LeetCode 2034)     | Hash Map + Balanced Multiset      | O(log N) | O(N)     |
 | 10 | Seat Reservation Manager (LeetCode 1845)    | Min-Heap of Free Seat IDs         | O(log N) | O(N)     |
 | 11 | Design Underground System (LeetCode 1396)   | Dual Hash Maps (Transit & Stats)  | O(1) all | O(P + S2)|
 | 12 | Number of Recent Calls (LeetCode 933)       | Sliding Window Queue [t-3000, t]  | O(1) avg | O(W)     |
 | 13 | Design Leaderboard (LeetCode 1244)          | Hash Map + Min-Heap Top-K         | O(N logK)| O(P)     |
 | 14 | Bounded Blocking Queue (LeetCode 1188)      | Mutex + Dual Condition Variables  | O(1) all | O(Cap)   |
 | 15 | Majority Checker in Subarray (LC 1154)      | Boyer-Moore Random Sampling + UB  | O(K logN)| O(N)     |
 | 16 | Design Authentication Manager (LC 1797)     | Hash Map Expiry with Lazy Purge   | O(1) gen | O(T)     |
 | 17 | Multi-Policy Online Cache Engine (LRU/TTL)  | DLL + Hash Map + Expiry Min-Heap  | O(1) avg | O(Cap)   |
 ====================================================================================================
*/

// ============================================================
// 1. MOVING AVERAGE FROM DATA STREAM — LeetCode 346
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
// - Problem Statement: Calculate the moving average of all integers in the sliding window of size w from data stream (LeetCode 346).
// - Approach: Sliding FIFO Queue with Running Window Sum.
// - Intuition: Push incoming values and add to running sum; when size exceeds w, pop oldest from queue and subtract from sum in O(1).
// - Complexity: Time: O(1) per `next`, Space: O(W) where W is window size.


// ============================================================
// 2. FIRST UNIQUE NUMBER IN DATA STREAM — LeetCode 1429
// ============================================================

class FirstUnique {
    list<int> dll; // Doubly linked list of unique elements
    unordered_map<int, list<int>::iterator> nodeMap; // Val -> DLL iterator
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
            // Second occurrence: become duplicate, remove from DLL
            dll.erase(nodeMap[value]);
            nodeMap.erase(value);
            duplicates.insert(value);
        } else {
            // First occurrence: append to DLL
            dll.push_back(value);
            nodeMap[value] = prev(dll.end());
        }
    }
};
// Interview Explanation:
// - Problem Statement: Maintain data stream to return the first unique integer added in O(1) time (LeetCode 1429).
// - Approach: Doubly Linked List + Hash Map (Iterator Locator) + Duplicate Set.
// - Intuition: Store unique items in DLL order. On second occurrence, erase node from DLL in O(1) via stored iterator and mark as permanently duplicate.
// - Complexity: Time: O(1) for both `showFirstUnique` and `add`, Space: O(N).


// ============================================================
// 3. FIRST NON-REPEATING CHARACTER IN A STREAM — GFG / LeetCode
// ============================================================

class StreamFirstNonRepeatingChar {
    int freq[26] = {0};
    queue<char> q;
public:
    StreamFirstNonRepeatingChar() {}

    char add(char c) {
        freq[c - 'a']++;
        q.push(c);

        // Lazily discard characters that have repeated
        while (!q.empty() && freq[q.front() - 'a'] > 1) {
            q.pop();
        }

        return q.empty() ? '#' : q.front();
    }
};
// Interview Explanation:
// - Problem Statement: Given a stream of characters, find the first non-repeating character after reading each character. Return '#' if none exists.
// - Approach: Frequency Array + Queue with Lazy Pop.
// - Intuition: Increment frequency and push to queue. Discard front elements whose frequency > 1. Top of queue is the first non-repeating character.
// - Complexity: Time: O(1) amortized per character, Space: O(1) (26 characters).


// ============================================================
// 4. DESIGN HIT COUNTER — LeetCode 362
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
// - Problem Statement: Count the number of hits received in the past 5 minutes (300 seconds) (LeetCode 362).
// - Approach: Circular Fixed-Size Buffer (300 Buckets).
// - Intuition: Use array of size 300 indexed by `timestamp % 300`. If bucket timestamp is outdated, reset count to 1; otherwise increment. `getHits` sums buckets where `timestamp - times[i] < 300`.
// - Complexity: Time: O(1) for `hit`, O(300) = O(1) for `getHits`, Space: O(300) = O(1).


// ============================================================
// 5. LOGGER RATE LIMITER — LeetCode 359
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
// - Problem Statement: Decide if a message should be printed: only if it has not been printed in the last 10 seconds (LeetCode 359).
// - Approach: Hash Map with Next Available Timestamp.
// - Intuition: Store the earliest timestamp at which the message is eligible to print again (`timestamp + 10`). Check and update in O(1).
// - Complexity: Time: O(1) per message, Space: O(M) where M is number of unique messages.


// ============================================================
// 6. DESIGN FRONT MIDDLE BACK QUEUE — LeetCode 1670
// ============================================================

class FrontMiddleBackQueue {
    deque<int> left, right;

    void balance() {
        // Invariant: left.size() == right.size() OR left.size() == right.size() + 1
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
// - Problem Statement: Design a queue supporting push/pop at front, middle, and back in O(1) time (LeetCode 1670).
// - Approach: Two Balanced Deques (`left` and `right`).
// - Intuition: Middle element is always `left.back()`. Maintain balance invariant so `left.size()` is equal to or 1 greater than `right.size()`.
// - Complexity: Time: O(1) for all operations, Space: O(N).


// ============================================================
// 7. FINDING MK AVERAGE — LeetCode 1825
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
// - Problem Statement: Calculate average of smallest m elements in stream after removing k smallest and k largest elements (LeetCode 1825).
// - Approach: Three Multisets (`left` of size k, `mid` of size m - 2k, `right` of size k) + Sliding Queue of size m.
// - Intuition: Maintain running sum of the middle multiset `midSum`. On insertions and deletions, cascade elements across set boundaries to preserve exact partition sizes.
// - Complexity: Time: O(log M) per `addElement`, O(1) for `calculateMKAverage`, Space: O(M).


// ============================================================
// 8. SNAPSHOT ARRAY — LeetCode 1146
// ============================================================

class SnapshotArray {
    vector<vector<pair<int, int>>> history; // index -> vector of {snap_id, val}
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
        // Upper bound on snap_id to find largest snap_id <= target
        auto it = upper_bound(hist.begin(), hist.end(), make_pair(snap_id, INT_MAX));
        return prev(it)->second;
    }
};
// Interview Explanation:
// - Problem Statement: Array supporting `set`, taking snapshots `snap`, and retrieving values at historical snapshots `get(index, snap_id)` (LeetCode 1146).
// - Approach: History Vectors + Binary Search (`upper_bound`).
// - Intuition: Record `{snap_id, val}` only when mutated. To get value at `snap_id`, binary search for the most recent mutation recorded <= `snap_id`.
// - Complexity: Time: O(1) for `set` and `snap`, O(log S) for `get`, Space: O(Length + Set Operations).


// ============================================================
// 9. STOCK PRICE FLUCTUATION — LeetCode 2034
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
// - Problem Statement: Process stream of stock prices with timestamp corrections; query current, maximum, and minimum prices (LeetCode 2034).
// - Approach: Hash Map + Balanced Multiset.
// - Intuition: Hash map maintains `{timestamp -> price}`. Multiset maintains sorted prices; remove old price on correction and insert new price in O(log N).
// - Complexity: Time: O(log N) for `update`, O(1) for `current`, `maximum`, `minimum`, Space: O(N).


// ============================================================
// 10. SEAT RESERVATION MANAGER — LeetCode 1845
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
// - Problem Statement: Manage reservations for n seats numbered 1 to n, always assigning the lowest-numbered unreserved seat (LeetCode 1845).
// - Approach: Min-Heap of Available Seat IDs.
// - Intuition: Min-heap guarantees O(log N) retrieval of the minimum available seat number and O(log N) unreservation.
// - Complexity: Time: O(N log N) initialization, O(log N) per `reserve` and `unreserve`, Space: O(N).


// ============================================================
// 11. DESIGN UNDERGROUND SYSTEM — LeetCode 1396
// ============================================================

class UndergroundSystem {
    unordered_map<int, pair<string, int>> checkInMap; // id -> {station, time}
    unordered_map<string, pair<long long, int>> routeStats; // "start>end" -> {totalTime, tripCount}
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
// - Problem Statement: Track customer travel times between underground stations and compute average travel times (LeetCode 1396).
// - Approach: Dual Hash Maps (`checkInMap` for active trips, `routeStats` for historical aggregates).
// - Intuition: When customer checks out, calculate trip duration and update route sum & count in O(1). `getAverageTime` divides total time by count in O(1).
// - Complexity: Time: O(1) for `checkIn`, `checkOut`, `getAverageTime`, Space: O(Active Passengers + Station Pairs).


// ============================================================
// 12. NUMBER OF RECENT CALLS — LeetCode 933
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
// - Intuition: Push incoming timestamp `t`. Pop expired timestamps `< t - 3000`. Number of elements remaining in queue is the active count.
// - Complexity: Time: O(1) amortized per `ping`, Space: O(W) where W <= 3000.


// ============================================================
// 13. DESIGN LEADERBOARD — LeetCode 1244
// ============================================================

class Leaderboard {
    unordered_map<int, int> scores; // playerId -> score
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
// - Problem Statement: Design leaderboard supporting adding scores to players, computing top K scores sum, and resetting player scores (LeetCode 1244).
// - Approach: Hash Map + Min-Heap Top-K Extraction.
// - Intuition: `scores` tracks player totals in O(1). `top(K)` maintains a min-heap of size K over all players in O(N log K).
// - Complexity: Time: O(1) for `addScore` and `reset`, O(N log K) for `top`, Space: O(Players).


// ============================================================
// 14. BOUNDED BLOCKING QUEUE — LeetCode 1188
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
// - Problem Statement: Design thread-safe bounded blocking queue where enqueue blocks if full and dequeue blocks if empty (LeetCode 1188).
// - Approach: Mutex + Two Condition Variables (`cv_not_full`, `cv_not_empty`).
// - Intuition: Synchronization primitives protect critical section. Threads wait on condition variables until space/item is available, then signal waiting consumer/producer.
// - Complexity: Time: O(1) per operation, Space: O(Capacity).


// ============================================================
// 15. MAJORITY CHECKER IN SUBARRAY — LeetCode 1154
// ============================================================

class MajorityChecker {
    vector<int> arr;
    unordered_map<int, vector<int>> pos; // val -> list of indices
public:
    MajorityChecker(vector<int>& arr) : arr(arr) {
        for (int i = 0; i < (int)arr.size(); i++) {
            pos[arr[i]].push_back(i);
        }
    }

    int query(int left, int right, int threshold) {
        // Monte Carlo Random Sampling: 20 iterations
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
// - Problem Statement: Query if any element in subarray arr[left...right] occurs >= threshold times (where threshold > (right - left + 1) / 2) (LeetCode 1154).
// - Approach: Randomized Sampling + Binary Search Frequency Counting.
// - Intuition: If a majority element exists, picking a random index hits it with probability >= 0.5. With 20 iterations, failure probability is <= (0.5)^20 < 10^-6. Verify candidate count via `upper_bound - lower_bound` on index list.
// - Complexity: Time: O(20 * log N) per query, Space: O(N).


// ============================================================
// 16. DESIGN AUTHENTICATION MANAGER — LeetCode 1797
// ============================================================

class AuthenticationManager {
    int timeToLive;
    unordered_map<string, int> tokens; // tokenId -> expiryTime
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
// - Problem Statement: Authentication manager supporting token generation, renewal, and counting unexpired tokens (LeetCode 1797).
// - Approach: Hash Map of Token Expiry Timestamps.
// - Intuition: Store expiry time `currentTime + timeToLive`. On renewal, check if `expiry > currentTime`. Count active tokens in linear scan over active map.
// - Complexity: Time: O(1) for `generate` and `renew`, O(N) for `countUnexpiredTokens`, Space: O(Tokens).


// ============================================================
// 17. MULTI-POLICY ONLINE CACHE ENGINE (LRU + TTL)
// ============================================================

class OnlineCacheEngine {
    int cap;
    list<pair<string, string>> lruList; // {key, value}
    unordered_map<string, pair<list<pair<string, string>>::iterator, int>> cache; // key -> {list_it, expiryTime}
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> expiryMinHeap; // {expiry, key}

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
// - Problem Statement: Design a production-grade online cache supporting get, put, LRU eviction upon capacity saturation, and TTL expiration.
// - Approach: Doubly Linked List (LRU Recency) + Hash Map + Expiration Min-Heap.
// - Intuition: DLL maintains access recency for O(1) LRU eviction. Min-heap prioritizes earliest expiring keys for lazy TTL purging.
// - Complexity: Time: O(1) amortized for `get` and `put`, Space: O(Capacity).