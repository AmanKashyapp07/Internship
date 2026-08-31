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
 | 1  | Design HashMap (MyHashMap - LC 706)         | Separate Chaining (Bucket Lists)  | Avg O(1) | O(N)     |
 | 2  | Summary Ranges (LC 228 / LC 352)            | Sorted Set Range Traversal        | O(log N) | O(N)     |
 | 3  | Water and Jug Problem (BFS - LC 365)        | BFS 6-State Graph Traversal       | O(X * Y) | O(X * Y) |
 | 4  | Kth Smallest in Lexicographical Order (440) | Trie Prefix Tree Level Skipping   | O(log^2N)| O(1)     |
 | 5  | Longest Nice Subarray (LC 2401)             | Sliding Window + Cumulative OR    | O(N)     | O(1)     |
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
// 4. KTH SMALLEST IN LEXICOGRAPHICAL ORDER — LeetCode 440
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
// 5. LONGEST NICE SUBARRAY (PAIRWISE AND = 0) — LeetCode 2401
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
