/**
 * ====================================================================================================
 *                 ULTIMATE SYSTEM DESIGN & LIVE INTERVIEW PLAYBOOK
 * ====================================================================================================
 * Dedicated Master Reference for System Design Data Structures, In-Memory Caches,
 * Multi-Level HashMaps, Concurrency Follow-ups, and Live Interview Communication Strategy.
 *
 * All domain-specific algorithmic implementations have been consolidated into dedicated modules:
 *  - Arrays, Two Pointers & Binary Search -> DSA-Code/sorting_template.cpp
 *  - Sliding Window & Prefix Sum          -> DSA-Code/sliding_template.cpp
 *  - Monotonic Stack & Deque Evaluators   -> DSA-Code/stack_template.cpp
 *  - Heaps, Priority Queues & Intervals   -> DSA-Code/heap_interval_template.cpp
 *  - Recursion & Advanced Backtracking    -> DSA-Code/recursion_template.cpp
 *  - Binary Trees & BST Master Suite      -> DSA-Code/notebook-7.cpp (Problems 1 to 43)
 *  - Graphs, DSU & Shortest Paths Suite   -> DSA-Code/notebook-8.cpp (Problems 1 to 35)
 *  - Dynamic Programming Master Suite     -> DSA-Code/notebook-10.cpp (Sections 1 to 12)
 *  - SDE Sheet Consolidated Master Suite  -> DSA-Code/notebook-11.cpp (29 Master Modules)
 * ====================================================================================================
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
#include <list>
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
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

// ============================================================================
// COMMON INTERVIEW DATA STRUCTURE DEFINITIONS
// ============================================================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x = 0, ListNode* n = nullptr) : val(x), next(n) {}
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(x), left(l), right(r) {}
};

struct NodeWithRandom {
    int val;
    NodeWithRandom* next;
    NodeWithRandom* random;
    NodeWithRandom(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

struct DSU {
    vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i == root_j) return false;
        if (rank[root_i] < rank[root_j]) swap(root_i, root_j);
        parent[root_j] = root_i;
        if (rank[root_i] == rank[root_j]) rank[root_i]++;
        return true;
    }
};

// ============================================================================
// 21. DESIGN-ORIENTED DATA STRUCTURES
// ============================================================================

// [CORE] Implement Queue using Stacks
// Idea: Two stacks: inStack for push, outStack for pop/peek. Shift elements on demand for amortized O(1).
// Time: Push O(1), Pop/Peek O(1) amortized
// Space: O(n)
class MyQueue {
    stack<int> inStack, outStack;
    void shift() {
        if (outStack.empty()) {
            while (!inStack.empty()) {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
    }
public:
    MyQueue() {}
    void push(int x) { inStack.push(x); }
    int pop() { shift(); int v = outStack.top(); outStack.pop(); return v; }
    int peek() { shift(); return outStack.top(); }
    bool empty() { return inStack.empty() && outStack.empty(); }
};

// [CORE] Design HashMap
// Idea: Separate chaining using an array of buckets (lists of {key, value} pairs) with a hash function.
// Time: O(1) average for put, get, remove
// Space: O(Capacity + Elements)
class MyHashMap {
    static const int BUCKET_COUNT = 10007;
    vector<list<pair<int, int>>> buckets;

    int hash(int key) const { return key % BUCKET_COUNT; }

public:
    MyHashMap() : buckets(BUCKET_COUNT) {}

    void put(int key, int value) {
        auto& chain = buckets[hash(key)];
        for (auto& entry : chain) {
            if (entry.first == key) {
                entry.second = value;
                return;
            }
        }
        chain.push_back({key, value});
    }

    int get(int key) const {
        const auto& chain = buckets[hash(key)];
        for (const auto& entry : chain) {
            if (entry.first == key) return entry.second;
        }
        return -1;
    }

    void remove(int key) {
        auto& chain = buckets[hash(key)];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                return;
            }
        }
    }
};

// [HIGH] Design HashSet
// Idea: Separate chaining with vector of lists of keys.
// Time: O(1) average for add, remove, contains
// Space: O(Capacity + Elements)
class MyHashSet {
    static const int BUCKET_COUNT = 10007;
    vector<list<int>> buckets;
    int hash(int key) const { return key % BUCKET_COUNT; }
public:
    MyHashSet() : buckets(BUCKET_COUNT) {}
    void add(int key) {
        auto& chain = buckets[hash(key)];
        for (int k : chain) if (k == key) return;
        chain.push_back(key);
    }
    void remove(int key) {
        auto& chain = buckets[hash(key)];
        chain.remove(key);
    }
    bool contains(int key) const {
        const auto& chain = buckets[hash(key)];
        for (int k : chain) if (k == key) return true;
        return false;
    }
};



// [CORE] Insert Delete GetRandom O(1)
// Idea: Dynamic array storing values + hash map storing value to array index. Swap with back element to delete in O(1).
// Time: O(1) average for insert, remove, getRandom
// Space: O(n)
class RandomizedSet {
    vector<int> nums;
    unordered_map<int, int> valToIdx;

public:
    RandomizedSet() {}

    bool insert(int val) {
        if (valToIdx.count(val)) return false;
        valToIdx[val] = nums.size();
        nums.push_back(val);
        return true;
    }

    bool remove(int val) {
        if (!valToIdx.count(val)) return false;
        int idx = valToIdx[val];
        int lastVal = nums.back();

        nums[idx] = lastVal;
        valToIdx[lastVal] = idx;

        nums.pop_back();
        valToIdx.erase(val);
        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};


// [ADVANCED] All O(1) Data Structure
// Idea: Doubly linked list of frequency buckets, where each bucket holds a set of keys. Maps key to bucket iterator.
// Time: O(1) for inc, dec, getMaxKey, getMinKey
// Space: O(number of keys)
class AllOne {
    struct Bucket {
        int count;
        unordered_set<string> keys;
    };
    list<Bucket> buckets; // Sorted frequency list
    unordered_map<string, list<Bucket>::iterator> keyMap;

public:
    AllOne() {}

    void inc(string key) {
        if (!keyMap.count(key)) {
            if (buckets.empty() || buckets.front().count != 1) {
                buckets.push_front({1, {key}});
            } else {
                buckets.front().keys.insert(key);
            }
            keyMap[key] = buckets.begin();
        } else {
            auto cur = keyMap[key];
            auto nxt = next(cur);
            if (nxt == buckets.end() || nxt->count != cur->count + 1) {
                nxt = buckets.insert(nxt, {cur->count + 1, {}});
            }
            nxt->keys.insert(key);
            keyMap[key] = nxt;
            cur->keys.erase(key);
            if (cur->keys.empty()) buckets.erase(cur);
        }
    }

    void dec(string key) {
        auto cur = keyMap[key];
        if (cur->count == 1) {
            keyMap.erase(key);
        } else {
            auto prv = prev(cur);
            if (cur == buckets.begin() || prv->count != cur->count - 1) {
                prv = buckets.insert(cur, {cur->count - 1, {}});
            }
            prv->keys.insert(key);
            keyMap[key] = prv;
        }
        cur->keys.erase(key);
        if (cur->keys.empty()) buckets.erase(cur);
    }

    string getMaxKey() { return buckets.empty() ? "" : *buckets.back().keys.begin(); }
    string getMinKey() { return buckets.empty() ? "" : *buckets.front().keys.begin(); }
};



// ============================================================================
// 22. INTERVIEW FOLLOW-UP PATTERNS & PLAYBOOK
// ============================================================================
/*
--------------------------------------------------------------------------------
  1. TWO SUM VARIANTS & SCALE FOLLOW-UPS
--------------------------------------------------------------------------------
  - Sorted Input:
    Use Two Pointers (O(n) time, O(1) space) instead of Hash Map (O(n) space).
  - Return All Unique Pairs:
    Sort first or use hash map with frequency counters; skip duplicate elements upon match.
  - 3Sum / 4Sum Generalization:
    Sort array, reduce K-Sum to (K-1)-Sum recursively or iteratively until 2Sum with Two Pointers.
  - Streaming Input / Data Stream:
    Use balanced BST or Two Pointers if window bounded; hash map if unbounded.
  - Limited Memory / Massive Dataset (Does not fit in RAM):
    External sort chunks of data on disk, then perform two-pointer linear pass across sorted external files,
    or use bucketed hashing (partition data into hash buckets on disk where x and (target - x) go to paired partitions).

--------------------------------------------------------------------------------
  2. NUMBER OF ISLANDS VARIANTS
--------------------------------------------------------------------------------
  - 8-Directional Movement:
    Expand direction arrays `dr[] = {-1,-1,-1,0,0,1,1,1}` and `dc[] = {-1,0,1,-1,1,-1,0,1}`.
  - Largest Island Area:
    DFS returns cumulative cell count `1 + sum(dfs(neighbor))` instead of void.
  - Dynamic Land Additions / Online Queries (Number of Islands II):
    Use Disjoint Set Union (DSU). For each added land cell, initialize component with parent=self,
    check 4 neighbors, and execute `dsu.unite(land, neighbor)` for every adjacent land cell.
  - Hexagonal Grids:
    Adjust adjacency list based on odd/even row parity coordinates.

--------------------------------------------------------------------------------
  3. LRU CACHE EXTENSIONS & CONCURRENCY
--------------------------------------------------------------------------------
  - Thread Safety & Concurrency:
    Use std::shared_mutex (Read-Write lock) for fine-grained concurrent access, or Sharded LRU
    (partition hash keys across N independent LRU segments, e.g. 16 shards, to minimize lock contention).
  - TTL (Time-To-Live / Key Expiration):
    Store expiration timestamp in Node. Evict expired nodes lazily during `get()` / `put()`,
    or maintain an auxiliary Min-Heap ordered by expiration timestamp for background cleanup.
  - Memory-Bounded Eviction (Variable Payload Sizes):
    Track current total memory in bytes instead of element count; evict tail until `currentBytes + newItemBytes <= maxBytes`.

--------------------------------------------------------------------------------
  4. BFS & SHORTEST PATH TRADE-OFFS
--------------------------------------------------------------------------------
  - Unweighted Graph -> Standard BFS (O(V + E) using queue).
  - Binary Weights (0 and 1) -> 0-1 BFS (O(V + E) using std::deque: 0 to front, 1 to back).
  - Non-negative Weights -> Dijkstra's Algorithm (O((V + E) log V) using min-priority queue).
  - General Weights with Negative Edges -> Bellman-Ford (O(V * E)).
  - Large Search Space (e.g. Word Ladder, Rubik's Cube) -> Bidirectional BFS (meets in middle, reduces branching factor from b^d to 2 * b^(d/2)).
  - Multi-Source Propagation (Rotting Oranges, Walls and Gates) -> Push all sources into queue initially at level 0.

--------------------------------------------------------------------------------
  5. BINARY SEARCH ON REAL NUMBERS & PREDICATES
--------------------------------------------------------------------------------
  - Monotonic Feasibility Predicate (BS on Answer):
    If condition `canAchieve(mid)` is monotonic (`TTTTFFFF` or `FFFFTTTT`), binary search over candidate value domain [low..high].
  - Floating Point / Real Number Precision:
    Instead of `while (low <= high)`, iterate for a fixed count (e.g. `for (int iter = 0; iter < 100; ++iter)`)
    or check `while (high - low > 1e-7)` to prevent infinite floating-point loops.
  - Boundary Disambiguation:
    `lower_bound`: first element >= target.
    `upper_bound`: first element > target.

--------------------------------------------------------------------------------
  6. TOP-K & STREAMING QUANTILES
--------------------------------------------------------------------------------
  - In-Memory Static Array -> QuickSelect (O(n) average, O(1) space).
  - Streaming / Continuous Updates -> Min-Heap of size K (O(n log k) time, O(k) space).
  - Bounded Integer Range -> Bucket Sort (O(n) time, O(range) space).
  - Approximate Quantiles in Distributed Stream -> Count-Min Sketch / T-Digest / HyperLogLog.
================================================================================
*/
