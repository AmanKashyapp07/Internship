# C++ High-Yield OA Master Cheat-Sheet — Microsoft & Top-Tier Prep

A battle-tested, high-density syntax and pattern reference designed specifically for Microsoft Online Assessments (OA) and technical loops. Focuses strictly on high-ROI patterns, standard algorithmic skeletons, common edge cases, container syntax, and pitfalls.

---

## 1. Fast I/O & Template Boilerplate

Paste at the top of your solution if testing standalone or running competitive I/O:

```cpp
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
#include <utility>
#include <bitset>
#include <tuple>
#include <array>
#include <functional>
#include <sstream>
#include <cctype>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
const ll MOD = 1e9 + 7;

// Fast I/O for standalone main
auto init = []() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}();
```

---

## 2. `vector` — Core Dynamic Array

```cpp
vector<int> v;                                         // Empty
vector<int> v(n);                                      // Size n, 0-initialized
vector<int> v(n, -1);                                  // Size n, filled with -1
vector<vector<int>> grid(m, vector<int>(n, 0));        // M x N matrix

v.push_back(x);
v.pop_back();
v.back(); v.front();
v.empty();

// CRITICAL: size() returns unsigned size_t!
// (int)v.size() - 1 avoids catastrophic underflow bug when empty (0 - 1 = 18446744073709551615ULL)
int n = (int)v.size();

// Range-based for loops
for (int x : v) { ... }                               // Copy (read-only primitives)
for (int &x : v) { x *= 2; }                          // By reference (modify in-place)
for (const auto &row : grid) { ... }                  // Const reference (avoid expensive copy)

// Sorting
sort(v.begin(), v.end());                             // Ascending
sort(v.rbegin(), v.rend());                           // Descending

// Custom comparator (Intervals / 2D vectors)
// STRICT WEAK ORDERING: Return true only if a strictly precedes b! NEVER use <= or >=!
sort(v.begin(), v.end(), [](const auto &a, const auto &b) {
    if (a[0] != b[0]) return a[0] < b[0];             // Sort start time asc
    return a[1] > b[1];                               // Sort end time desc on tie
});

// Binary Search functions on sorted vectors:
// lower_bound: first element >= target
auto it1 = lower_bound(v.begin(), v.end(), target);
int idx1 = it1 - v.begin();

// upper_bound: first element > target
auto it2 = upper_bound(v.begin(), v.end(), target);
int idx2 = it2 - v.begin();

// Prefix fill & generation
iota(v.begin(), v.end(), 0);                          // Fills with 0, 1, 2, ..., n-1
fill(v.begin(), v.end(), val);
```

---

## 3. `unordered_map` & `unordered_set` — $O(1)$ Hash Containers

```cpp
unordered_map<int, int> freq;

freq[x]++;                                             // Inserts 0 if missing, increments — safe
if (freq.count(x)) { ... }                             // Existence check WITHOUT inserting
if (freq.find(x) != freq.end()) { ... }                // Iterator lookup

// C++17 Structured Bindings
for (auto &[key, val] : freq) { ... }

// CAUTION: map[key] inside a condition silently inserts key with default 0!
// ALWAYS use freq.count(key) or freq.find(key) for read-only existence checks.

unordered_set<int> seen;
seen.insert(x);
if (seen.count(x)) { ... }
seen.erase(x);
```

### The Universal Prefix Sum Frequency Pattern (Subarray Sum = K)
Works seamlessly with negative values where two pointers fail:
```cpp
unordered_map<long long, int> prefixCounts;
prefixCounts[0] = 1; // Base case: prefix sum before element 0 is 0
long long runningSum = 0, count = 0;

for (int x : nums) {
    runningSum += x;
    if (prefixCounts.count(runningSum - k)) {
        count += prefixCounts[runningSum - k];
    }
    prefixCounts[runningSum]++;
}
```

### Fixed Alphabet Frequency Optimization
Whenever characters are strictly lowercase `'a'`–`'z'`, use a fixed array instead of `unordered_map` for a 10× speedup:
```cpp
int freq[26] = {0};
for (char c : s) freq[c - 'a']++;
```

### Custom Hash for `pair`/`tuple` Keys (unordered_map doesn't support them natively)
```cpp
struct pair_hash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<long long>()(((long long)p.first << 32) ^ p.second);
    }
};
unordered_map<pair<int, int>, int, pair_hash> memo;

// Simpler alternative: encode pair as a single long long key
unordered_map<long long, int> memo2;
long long key = (long long)r * 100000 + c;             // Safe if c < 100000
```

---

## 4. `set`, `multiset` & `map` — $O(\log N)$ Ordered Red-Black Trees

Use when you need **sorted order maintained dynamically**, **range queries**, or **predecessor/successor lookups**.

```cpp
set<int> s;
s.insert(10); s.insert(20); s.insert(30);

// CRITICAL TLE TRAP: ALWAYS use member function s.lower_bound(x), NEVER std::lower_bound(s.begin(), s.end(), x)!
// s.lower_bound(x) is O(log N). std::lower_bound(s.begin(), s.end(), x) is O(N) because set iterators are NOT random access!
auto it = s.lower_bound(15);                           // First element >= 15 (points to 20)
auto it2 = s.upper_bound(20);                          // First element > 20 (points to 30)

// Predecessor lookup (largest element < x)
auto it = s.lower_bound(x);
if (it != s.begin()) {
    int pred = *prev(it);                              // Largest element strictly smaller than x
}

// Largest / Smallest elements in set in O(1)
int smallest = *s.begin();
int largest = *s.rbegin();
```

### `std::multiset` (Duplicates Allowed) & The Deletion Trap
```cpp
multiset<int> ms = {5, 5, 5, 10};

// CATASTROPHIC BUG: ms.erase(5) deletes ALL instances of 5!
// ms.erase(5) -> ms becomes {10}

// CORRECT: To erase ONLY ONE instance of 5:
auto it = ms.find(5);
if (it != ms.end()) ms.erase(it);                      // Erases only the single node pointed to!
```

### `std::map` (Ordered Key-Value)
```cpp
map<int, int> orderedMap;
orderedMap[10] = 100;
orderedMap[20] = 200;

// Keys are iterated in strictly ascending order:
for (auto &[k, v] : orderedMap) { ... }

// Reverse iteration (highest key first)
for (auto it = orderedMap.rbegin(); it != orderedMap.rend(); ++it) {
    int maxKey = it->first;
}
```

---

## 5. `list` (Doubly Linked List) — $O(1)$ Splicing for LRU / LFU Cache

`std::list` is the standard tool for **LRU Cache (LC 146)** and **LFU Cache (LC 460)** because it supports $O(1)$ node splicing without node reallocation.

```cpp
list<pair<int, int>> dll;                              // Stores {key, value}

dll.emplace_front(k, v);                               // Push to front (most recently used)
dll.pop_back();                                        // Evict least recently used (tail)
int oldestKey = dll.back().first;

// dll.splice(dest_pos, source_list, node_iterator)
// Moves the node pointed to by `it` to the front of dll in O(1) time without copying/deleting!
dll.splice(dll.begin(), dll, it);

// Removal by iterator in O(1)
dll.erase(it);
```

---

## 6. `bitset<N>` — Word-Level 64-bit Parallelism

Fixed-size bit array that executes bitwise operations $64\times$ faster than standard loops by utilizing 64-bit CPU words. Essential for **0/1 Subset Sum / Knapsack (LC 416)** and bitmask states:

```cpp
bitset<10001> bs;                                      // Fixed capacity of 10,001 bits, initialized to 0
bs[0] = 1;

// Ultra-fast 0/1 Subset Sum transition:
// bs << x shifts all reachable subset sums by x. Bitwise OR transitions in parallel!
for (int x : nums) {
    bs |= (bs << x);
    if (bs[target]) return true;                       // Target subset sum reachable
}

// Common operations:
bs.count();                                            // Count of 1-bits
bs.test(i);                                            // Safe check if bit i is 1 (like bs[i])
bs.set(i);                                             // Sets bit i = 1
bs.reset(i);                                           // Sets bit i = 0
bs.reset();                                            // Clears all bits to 0
bs.all(); bs.any(); bs.none();                         // Boolean state checks
```

---

## 7. `pair`, `tuple`, and `array` — Multi-Dimensional States

```cpp
// 1. std::pair
pair<int, int> p = {row, col};
auto [r, c] = p;                                       // C++17 structured bindings

// 2. std::tuple (for 3+ fields, e.g. Dijkstra states {distance, row, col, keys})
tuple<int, int, int> state = {dist, r, c};
auto [d, r_pos, c_pos] = state;                        // C++17 unpack
int d_val = get<0>(state);                             // C++11 access

// 3. std::array (Stack-allocated, zero-overhead fixed array)
array<int, 4> dirs = {0, 1, 0, -1};
array<array<int, 2>, 4> directions = {{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
for (auto &[dr, dc] : directions) { ... }
```

---

## 8. `string` — Fast String Operations

```cpp
string s = "microsoft";
s.size(); (int)s.length();
s.substr(start_index, length);                         // Note: 2nd argument is LENGTH, not end index!
s.substr(start_index);                                 // Omit length to extract to end

s.find("cro");                                         // Returns starting index or string::npos
if (s.find("xyz") == string::npos) { /* not found */ }

to_string(12345);                                      // Number to string
stoi("12345");                                         // String to int
stoll("123456789012");                                 // String to long long

reverse(s.begin(), s.end());                           // In-place string reversal

// Character classification & case conversion (ALL take/return int, watch signedness with signed char!)
isalpha(c); isdigit(c); isalnum(c); isspace(c); isupper(c); islower(c);
char lower = tolower(c); char upper = toupper(c);

// Splitting a string by delimiter using stringstream (very common OA parsing need)
string csv = "10,20,,30";
vector<string> tokens;
stringstream ss(csv);
string token;
while (getline(ss, token, ',')) {
    tokens.push_back(token);                           // NOTE: keeps empty tokens ("" for "20,,30")
}

// Splitting by whitespace (skips consecutive spaces automatically)
stringstream ss2("the quick  brown fox");
vector<string> words;
string w;
while (ss2 >> w) words.push_back(w);

// Building a string efficiently
ostringstream oss;
oss << "value=" << 42 << ",";
string result = oss.str();

// Palindrome / anagram check helpers
bool isAnagram(string a, string b) {
    if (a.size() != b.size()) return false;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    return a == b;
}
```

---

## 9. `priority_queue` — Heaps & Custom Comparators

```cpp
// 1. Max-Heap (default)
priority_queue<int> maxHeap;
maxHeap.push(x);
maxHeap.top();
maxHeap.pop();

// 2. Min-Heap
priority_queue<int, vector<int>, greater<int>> minHeap;

// 3. Min-Heap of pairs (sorted by .first, then .second)
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

// 4. Custom Comparator Struct (Clean, zero lambda, reusable)
struct NodeCmp {
    bool operator()(const ListNode* a, const ListNode* b) const {
        return a->val > b->val;                        // '>' creates MIN-heap for pointers/nodes
    }
};
priority_queue<ListNode*, vector<ListNode*>, NodeCmp> minNodeHeap;

// 5. Tuple Min-Heap (e.g. for K Smallest Pairs / Dijkstra)
struct TupleCmp {
    bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const {
        return get<0>(a) > get<0>(b);                  // Min-heap by first element
    }
};
priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, TupleCmp> pq;
```

---

## 10. Monotonic Stack & Monotonic Deque

### Monotonic Stack (Next Greater Element / Histogram)
Stores indices or values in strictly increasing/decreasing order:
```cpp
// Next Greater Element (Circular Array / Standard)
vector<int> nextGreaterElements(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, -1);
    stack<int> st; // stores indices

    for (int i = 0; i < 2 * n; ++i) { // 2*n simulates circular traversal
        int idx = i % n;
        while (!st.empty() && nums[st.top()] < nums[idx]) {
            res[st.top()] = nums[idx];
            st.pop();
        }
        if (i < n) st.push(idx);
    }
    return res;
}
```

### Monotonic Deque (Sliding Window Maximum in $O(N)$)
```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // Stores INDICES in strictly decreasing order of values
    vector<int> res;

    for (int i = 0; i < (int)nums.size(); ++i) {
        // 1. Evict elements out of current window
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();

        // 2. Maintain monotonic decreasing invariant
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();

        dq.push_back(i);

        // 3. Record window maximum
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}
```

---

## 11. Two Pointers & Sliding Window Archetypes

### Dynamic Variable-Size Window (Longest / Shortest Substring)
```cpp
int left = 0, ans = 0;
unordered_map<char, int> counts;

for (int right = 0; right < n; ++right) {
    counts[s[right]]++;
    
    // Shrink condition while window invariant is violated
    while (/* condition invalid, e.g. counts[s[right]] > 1 */) {
        counts[s[left]]--;
        left++;
    }
    ans = max(ans, right - left + 1);
}
```

### Fixed-Size Window (Exactly K elements)
```cpp
int windowSum = 0;
for (int i = 0; i < (int)nums.size(); ++i) {
    windowSum += nums[i];
    if (i >= k) windowSum -= nums[i - k];               // Slide: remove element leaving the window
    if (i >= k - 1) ans = max(ans, windowSum);           // Window is full, record result
}
```

### Exact $K$ Condition via AtMost Reduction
To find subarrays with **exactly $K$** distinct elements:
$$\text{Exact}(K) = \text{AtMost}(K) - \text{AtMost}(K - 1)$$
```cpp
int atMost(vector<int>& nums, int k) {
    int left = 0, count = 0, distinct = 0;
    unordered_map<int, int> freq;
    for (int right = 0; right < (int)nums.size(); ++right) {
        if (freq[nums[right]]++ == 0) distinct++;
        while (distinct > k) {
            if (--freq[nums[left]] == 0) distinct--;
            left++;
        }
        count += right - left + 1;
    }
    return count;
}
int exactK = atMost(nums, k) - atMost(nums, k - 1);
```

### Kadane's Algorithm (Maximum Subarray Sum)
```cpp
int maxSubArray(vector<int>& nums) {
    int currMax = nums[0], globalMax = nums[0];
    for (int i = 1; i < (int)nums.size(); ++i) {
        currMax = max(nums[i], currMax + nums[i]);
        globalMax = max(globalMax, currMax);
    }
    return globalMax;
}
```

---

## 12. Binary Search Archetypes

### Direct Search (Partitioned / Rotated Arrays)
```cpp
int low = 0, high = n - 1;
while (low <= high) {
    int mid = low + (high - low) / 2;
    if (nums[mid] == target) return mid;
    
    if (nums[low] <= nums[mid]) { // Left half sorted
        if (nums[low] <= target && target < nums[mid]) high = mid - 1;
        else low = mid + 1;
    } else {                      // Right half sorted
        if (nums[mid] < target && target <= nums[high]) low = mid + 1;
        else high = mid - 1;
    }
}
```

### Binary Search on the Answer (Predicate / Capacity / Speed)
Use when predicate `isPossible(x)` is monotonic:
```cpp
bool isPossible(int val) {
    // Greedy feasibility check in O(N)
    return true;
}

int low = min_possible, high = max_possible;
int ans = high;

while (low <= high) {
    int mid = low + (high - low) / 2;
    if (isPossible(mid)) {
        ans = mid;         // Record best feasible answer
        high = mid - 1;    // Try smaller value (if minimizing)
    } else {
        low = mid + 1;     // Increase search space
    }
}
return ans;
```

---

## 13. Dynamic Programming Skeletons

### The Knapsack Master Rules

| Type | Outer Loop | Inner Loop Direction | Element Reuse? |
| :--- | :--- | :--- | :--- |
| **0/1 Knapsack** | Items | **BACKWARD** (`w = capacity; w >= wt; w--`) | No (Used at most once) |
| **Unbounded Knapsack** | Items | **FORWARD** (`w = wt; w <= capacity; w++`) | Yes (Unlimited reuse) |
| **Combinations (Coin Change 2)** | Coins (Outer) | **FORWARD** (`a = c; a <= amount; a++`) | `dp[a] += dp[a - c]` |
| **Permutations** | Amount (Outer) | Coins (Inner) | `dp[a] += dp[a - c]` |

```cpp
// 0/1 Knapsack (or Partition Equal Subset Sum)
vector<int> dp(capacity + 1, 0);
for (int i = 0; i < n; ++i) {
    for (int w = capacity; w >= weight[i]; --w) {
        dp[w] = max(dp[w], value[i] + dp[w - weight[i]]);
    }
}

// Unbounded Knapsack
vector<int> dp(capacity + 1, 0);
for (int i = 0; i < n; ++i) {
    for (int w = weight[i]; w <= capacity; ++w) {
        dp[w] = max(dp[w], value[i] + dp[w - weight[i]]);
    }
}

// Coin Change 1 (Minimum Coins)
vector<int> dp(amount + 1, amount + 1);
dp[0] = 0;
for (int x = 1; x <= amount; ++x) {
    for (int c : coins) {
        if (c <= x) dp[x] = min(dp[x], dp[x - c] + 1);
    }
}
return dp[amount] > amount ? -1 : dp[amount];
```

### Longest Increasing Subsequence (LIS) in $O(N \log N)$
```cpp
vector<int> tails;
for (int x : nums) {
    auto it = lower_bound(tails.begin(), tails.end(), x);
    if (it == tails.end()) tails.push_back(x);
    else *it = x;
}
return (int)tails.size();
```

### Top-Down Memoization Template (Recursion + Cache)
The fastest way to convert a brute-force recursive solution into DP under time pressure:
```cpp
unordered_map<int, long long> memo;                    // Key by encoded state

long long solve(int state /*, other params */) {
    if (/* base case */) return /* base value */;
    if (memo.count(state)) return memo[state];          // Cache hit

    long long result = /* combine solve(subproblem1), solve(subproblem2), ... */ 0;

    return memo[state] = result;                         // Cache & return
}
```

### 2D Grid DP (Unique Paths / Min Path Sum) — Space-Optimized to 1D
```cpp
vector<int> dp(n, 0);
dp[0] = grid[0][0];
for (int c = 1; c < n; ++c) dp[c] = dp[c - 1] + grid[0][c];

for (int r = 1; r < m; ++r) {
    dp[0] += grid[r][0];
    for (int c = 1; c < n; ++c) {
        dp[c] = min(dp[c], dp[c - 1]) + grid[r][c];      // dp[c] currently holds row r-1's value
    }
}
return dp[n - 1];
```

---

## 14. Graph & Grid Traversal Skeletons

### Multi-Source BFS on 2D Matrix (Rotting Oranges / Distances)
```cpp
queue<pair<int, int>> q;
// Enqueue all sources initially (Level 0)
for (int r = 0; r < m; ++r)
    for (int c = 0; c < n; ++c)
        if (grid[r][c] == SOURCE) q.push({r, c});

int steps = 0;
const int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

while (!q.empty()) {
    int sz = q.size();
    for (int i = 0; i < sz; ++i) {
        auto [r, c] = q.front(); q.pop();
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
            if (grid[nr][nc] != TARGET) continue;
            grid[nr][nc] = VISITED;                    // In-place mark
            q.push({nr, nc});
        }
    }
    steps++;
}
```

### Recursive DFS on Grid (Number of Islands / Flood Fill)
```cpp
void dfs(vector<vector<int>>& grid, int r, int c, int m, int n) {
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 1) return;
    grid[r][c] = 0;                                     // Mark visited in-place
    dfs(grid, r + 1, c, m, n);
    dfs(grid, r - 1, c, m, n);
    dfs(grid, r, c + 1, m, n);
    dfs(grid, r, c - 1, m, n);
}
```

### General Graph BFS / DFS (Adjacency List, Not Grid)
```cpp
vector<vector<int>> adj(n);                             // adj[u] = list of neighbors of u
vector<bool> visited(n, false);

// BFS
void bfs(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

// DFS (recursive)
void dfs(int u) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfs(v);
    }
}

// DFS (iterative, avoids stack overflow on deep graphs)
void dfsIterative(int start) {
    stack<int> st;
    st.push(start);
    while (!st.empty()) {
        int u = st.top(); st.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (int v : adj[u]) if (!visited[v]) st.push(v);
    }
}
```

### Dijkstra's Algorithm (Weighted Shortest Path, Non-Negative Weights)
```cpp
vector<long long> dijkstra(int n, int src, vector<vector<pair<int,int>>>& adj) {
    // adj[u] = list of {neighbor, weight}
    vector<long long> dist(n, LLONG_MAX);
    dist[src] = 0;
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0, src});                                  // {distance, node} — MIN-heap by distance

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;                       // Stale entry, skip (lazy deletion)
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

### 8-Line Disjoint Set Union (DSU / Union-Find) with Path Compression
```cpp
struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n) { iota(parent.begin(), parent.end(), 0); }
    int find(int i) {
        return (parent[i] == i) ? i : (parent[i] = find(parent[i]));
    }
    bool unite(int u, int v) {
        int rootU = find(u), rootV = find(v);
        if (rootU == rootV) return false;              // Cycle detected
        parent[rootU] = rootV;
        return true;
    }
};
```

### Kahn's Algorithm (Topological Sort / Eventual Safe States)
```cpp
vector<int> inDegree(n, 0);
for (int u = 0; u < n; ++u)
    for (int v : adj[u]) inDegree[v]++;

queue<int> q;
for (int i = 0; i < n; ++i)
    if (inDegree[i] == 0) q.push(i);

vector<int> topoOrder;
while (!q.empty()) {
    int u = q.front(); q.pop();
    topoOrder.push_back(u);
    for (int v : adj[u]) {
        if (--inDegree[v] == 0) q.push(v);
    }
}
bool hasCycle = (topoOrder.size() != n);
```

---

## 15. Linked List Micro-Templates

```cpp
// 1. Dummy Head Sentinel Pattern
ListNode dummy(0);
dummy.next = head;
ListNode* prev = &dummy;
// ... manipulate pointers using prev ...
return dummy.next;

// 2. In-Place List Reversal
ListNode *prev = nullptr, *curr = head;
while (curr != nullptr) {
    ListNode* nxt = curr->next;
    curr->next = prev;
    prev = curr;
    curr = nxt;
}
return prev; // New head

// 3. Find Middle Node (Tortoise & Hare)
ListNode *slow = head, *fast = head;
while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
}
// slow is at exact middle (or second middle if even)

// 4. Floyd's Cycle Detection & Entrance Find
ListNode *slow = head, *fast = head;
while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) break;
}
if (!fast || !fast->next) return nullptr; // No cycle
slow = head;
while (slow != fast) {
    slow = slow->next;
    fast = fast->next;
}
return slow; // Entrance to cycle
```

---

## 16. Binary Tree Traversal Templates

```cpp
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Recursive DFS traversals
void preorder(TreeNode* root, vector<int>& out) {       // Root -> Left -> Right
    if (!root) return;
    out.push_back(root->val);
    preorder(root->left, out);
    preorder(root->right, out);
}
void inorder(TreeNode* root, vector<int>& out) {         // Left -> Root -> Right (sorted for BST)
    if (!root) return;
    inorder(root->left, out);
    out.push_back(root->val);
    inorder(root->right, out);
}
void postorder(TreeNode* root, vector<int>& out) {       // Left -> Right -> Root
    if (!root) return;
    postorder(root->left, out);
    postorder(root->right, out);
    out.push_back(root->val);
}

// Iterative Inorder (using explicit stack — common follow-up ask)
vector<int> inorderIterative(TreeNode* root) {
    vector<int> out;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    while (curr || !st.empty()) {
        while (curr) { st.push(curr); curr = curr->left; }
        curr = st.top(); st.pop();
        out.push_back(curr->val);
        curr = curr->right;
    }
    return out;
}

// Level Order / BFS (returns levels separately — common for "zigzag" / "right side view")
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(level);
    }
    return res;
}

// Max Depth / Height (canonical recursive pattern reused everywhere)
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

---

## 17. Backtracking Templates (Subsets / Permutations / Combinations)

```cpp
// 1. Subsets (Power Set) — include/exclude decision at each index
void subsets(vector<int>& nums, int idx, vector<int>& curr, vector<vector<int>>& res) {
    if (idx == (int)nums.size()) {
        res.push_back(curr);
        return;
    }
    subsets(nums, idx + 1, curr, res);                  // Exclude nums[idx]
    curr.push_back(nums[idx]);
    subsets(nums, idx + 1, curr, res);                  // Include nums[idx]
    curr.pop_back();                                     // Backtrack
}

// 2. Permutations — swap-based in-place generation, avoids extra "used" array
void permute(vector<int>& nums, int start, vector<vector<int>>& res) {
    if (start == (int)nums.size()) {
        res.push_back(nums);
        return;
    }
    for (int i = start; i < (int)nums.size(); ++i) {
        swap(nums[start], nums[i]);
        permute(nums, start + 1, res);
        swap(nums[start], nums[i]);                      // Backtrack (undo swap)
    }
}

// 3. Combinations / Combination Sum — pruning with sorted input + start index
void combinationSum(vector<int>& candidates, int target, int start,
                     vector<int>& curr, vector<vector<int>>& res) {
    if (target == 0) { res.push_back(curr); return; }
    for (int i = start; i < (int)candidates.size(); ++i) {
        if (candidates[i] > target) break;               // Requires candidates sorted ascending — prune
        curr.push_back(candidates[i]);
        combinationSum(candidates, target - candidates[i], i, curr, res); // 'i' allows reuse; use i+1 to forbid reuse
        curr.pop_back();                                  // Backtrack
    }
}

// 4. Skipping duplicates in sorted input (Subsets II / Permutations II)
// Sort first, then at each level: if (i > start && nums[i] == nums[i-1]) continue;
```

Use `std::next_permutation` when you only need permutations in lexicographic order, not the backtracking tree itself:
```cpp
sort(nums.begin(), nums.end());
do {
    // process current permutation of nums
} while (next_permutation(nums.begin(), nums.end()));
```

---

## 18. Trie (Prefix Tree) — Word Search / Autocomplete

```cpp
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};

class Trie {
public:
    TrieNode* root = new TrieNode();

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int i = c - 'a';
            if (!node->children[i]) node->children[i] = new TrieNode();
            node = node->children[i];
        }
        node->isEnd = true;
    }

    bool search(const string& word) {
        TrieNode* node = find(word);
        return node && node->isEnd;
    }

    bool startsWith(const string& prefix) {
        return find(prefix) != nullptr;
    }

private:
    TrieNode* find(const string& s) {
        TrieNode* node = root;
        for (char c : s) {
            int i = c - 'a';
            if (!node->children[i]) return nullptr;
            node = node->children[i];
        }
        return node;
    }
};
```

---

## 19. Fenwick Tree (Binary Indexed Tree) — Prefix Sum with Point Updates

Use when you need repeated range-sum queries **and** point updates in $O(\log N)$ (plain prefix-sum arrays only handle the static case in $O(1)$ query but $O(N)$ update):
```cpp
struct Fenwick {
    vector<long long> tree;
    int n;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}

    void update(int i, long long delta) {                // 1-indexed! i in [1, n]
        for (; i <= n; i += i & (-i)) tree[i] += delta;
    }

    long long query(int i) {                              // Prefix sum [1, i]
        long long sum = 0;
        for (; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }

    long long rangeQuery(int l, int r) {                   // Sum [l, r], 1-indexed inclusive
        return query(r) - query(l - 1);
    }
};
```

---

## 20. 2D Prefix Sum (Range Sum Queries on a Matrix)

```cpp
// prefix[i][j] = sum of all elements in the rectangle (0,0) to (i-1,j-1)
vector<vector<long long>> prefix(m + 1, vector<long long>(n + 1, 0));
for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
        prefix[i + 1][j + 1] = prefix[i][j + 1] + prefix[i + 1][j] - prefix[i][j] + grid[i][j];

// Sum of rectangle [r1, c1] to [r2, c2] inclusive, 0-indexed:
auto rangeSum = [&](int r1, int c1, int r2, int c2) {
    return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
};
```

---

## 21. Interval Merging Pattern

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());            // Sort by start time
    vector<vector<int>> res = {intervals[0]};

    for (int i = 1; i < (int)intervals.size(); ++i) {
        if (intervals[i][0] <= res.back()[1]) {            // Overlaps with last merged interval
            res.back()[1] = max(res.back()[1], intervals[i][1]);
        } else {
            res.push_back(intervals[i]);
        }
    }
    return res;
}
```

---

## 22. Matrix Manipulation

```cpp
// Rotate N x N matrix 90 degrees clockwise, in-place
void rotate(vector<vector<int>>& m) {
    int n = m.size();
    // Transpose
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            swap(m[i][j], m[j][i]);
    // Reverse each row
    for (auto& row : m) reverse(row.begin(), row.end());
}

// Spiral traversal of an M x N matrix
vector<int> spiralOrder(vector<vector<int>>& m) {
    vector<int> res;
    int top = 0, bottom = m.size() - 1, left = 0, right = m[0].size() - 1;
    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; ++c) res.push_back(m[top][c]);
        top++;
        for (int r = top; r <= bottom; ++r) res.push_back(m[r][right]);
        right--;
        if (top <= bottom) { for (int c = right; c >= left; --c) res.push_back(m[bottom][c]); bottom--; }
        if (left <= right) { for (int r = bottom; r >= top; --r) res.push_back(m[r][left]); left++; }
    }
    return res;
}
```

---

## 23. Useful STL Algorithms (Frequently Forgotten Under Pressure)

```cpp
accumulate(v.begin(), v.end(), 0LL);                    // Sum (0LL seed avoids overflow on large sums)
count(v.begin(), v.end(), x);                            // Count occurrences of x
count_if(v.begin(), v.end(), [](int x){ return x > 0; }); // Count matching predicate
*max_element(v.begin(), v.end());                        // Max value (dereference the iterator!)
*min_element(v.begin(), v.end());                        // Min value
unique(v.begin(), v.end());                              // Removes consecutive dupes — sort first, then:
v.erase(unique(v.begin(), v.end()), v.end());             // ...then erase the leftover tail (idiom)
swap(a, b);                                               // O(1) swap, works on containers too
__gcd(a, b);                                              // GCD (also: std::gcd(a,b) in <numeric>, C++17)
lcm(a, b);                                                // C++17 <numeric>: LCM
all_of(v.begin(), v.end(), pred); any_of(...); none_of(...); // Predicate-based checks over a range
```

---

## 24. Bit Manipulation & Builtin Hacks

```cpp
// Builtin Functions (Extremely fast, compiler intrinsics)
__builtin_popcount(x);                                 // Count 1-bits (32-bit int)
__builtin_popcountll(x);                               // Count 1-bits (64-bit unsigned long long)
__builtin_clz(x);                                      // Count leading zeros
__builtin_ctz(x);                                      // Count trailing zeros

// Fundamental Bit Tricks
x & (x - 1);                                           // Clears lowest set bit (Power of 2 check: (x > 0 && (x & (x - 1)) == 0))
x & -x;                                                // Extracts lowest set bit (Fenwick / BIT index step)
x ^ y;                                                 // Bitwise difference (Identical values cancel to 0)

// Bitmask Iteration (Subsets of length N)
for (int mask = 0; mask < (1 << n); ++mask) {
    for (int i = 0; i < n; ++i) {
        if (mask & (1 << i)) {
            // Element i is selected in subset
        }
    }
}
```

---

## 25. Math & Number Theory Quick Snippets

### Fast Modular Exponentiation ($O(\log N)$)
```cpp
long long power(long long base, long long exp, long long mod = MOD) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}
```

### Sieve of Eratosthenes ($O(N \log \log N)$)
```cpp
vector<bool> isPrime(n + 1, true);
isPrime[0] = isPrime[1] = false;
for (int p = 2; p * p <= n; ++p) {
    if (isPrime[p]) {
        for (int i = p * p; i <= n; i += p) isPrime[i] = false;
    }
}
```

### Modular Arithmetic Invariants
```cpp
long long addMod(long long a, long long b) { return (a + b) % MOD; }
long long subMod(long long a, long long b) { return (a - b + MOD) % MOD; }
long long mulMod(long long a, long long b) { return (1LL * a * b) % MOD; }
```

### GCD / LCM (Euclidean Algorithm, in case `<numeric>` versions aren't allowed)
```cpp
long long gcdFn(long long a, long long b) { return b == 0 ? a : gcdFn(b, a % b); }
long long lcmFn(long long a, long long b) { return a / gcdFn(a, b) * b; }   // Divide first to reduce overflow risk
```

---

## 26. High-Stakes OA Trap Checklist

1. **Integer Overflow During Multiplication**:
   ```cpp
   // WRONG: a * b overflows 32-bit before widening to long long!
   long long area = a * b;

   // CORRECT: Cast first operand to 64-bit
   long long area = (long long)a * b;
   ```
2. **Unsigned `size()` Underflow**:
   ```cpp
   // WRONG: If v.empty(), v.size() - 1 produces 18,446,744,073,709,551,615!
   for (int i = 0; i < v.size() - 1; ++i)

   // CORRECT:
   int n = (int)v.size();
   for (int i = 0; i < n - 1; ++i)
   ```
3. **Strict Weak Ordering Violation in Custom Comparator**:
   ```cpp
   // CRASH: Returning true on equal values breaks std::sort and causes SIGSEGV / memory corruption!
   bool cmp(int a, int b) { return a <= b; } // NEVER DO THIS

   // CORRECT:
   bool cmp(int a, int b) { return a < b; }
   ```
4. **Member `s.lower_bound()` vs Generic `std::lower_bound()`**:
   ```cpp
   // CRITICAL TLE BUG:
   auto it = lower_bound(s.begin(), s.end(), target);   // O(N) on set/map!
   
   // CORRECT:
   auto it = s.lower_bound(target);                     // O(log N)
   ```
5. **Deleting from `multiset`**:
   ```cpp
   // WRONG: Deletes ALL duplicates with value x!
   ms.erase(x);

   // CORRECT: Deletes only one instance of x:
   auto it = ms.find(x);
   if (it != ms.end()) ms.erase(it);
   ```
6. **Modifying Container While Iterating**:
   - Never call `v.erase()` or `map.erase()` inside a range-based `for (auto &x : c)` loop without proper iterator advancement.
7. **Graph 1-Indexed vs 0-Indexed**:
   - Always verify if graph vertices are labeled $0 \dots n-1$ or $1 \dots n$. Size DSU/Adjacency lists to $n + 1$ when unsure.
8. **`char` Signedness in `isalpha`/`tolower`/etc.**:
   ```cpp
   // UB risk: char can be signed; passing a negative value (from extended ASCII) to isalpha() is undefined
   char c = s[i];
   if (isalpha((unsigned char)c)) { ... }               // CORRECT: cast to unsigned char first
   ```
9. **Recursion Depth on Large Inputs**:
   - Recursive DFS on a linked list/tree/graph with $10^5$+ nodes can stack-overflow. Prefer the iterative BFS/DFS templates (Sections 14 & 16) when input size is large and depth is unbounded.
10. **Off-by-One in `substr`**:
    ```cpp
    // s.substr(start, length) — length is a COUNT, not an end index!
    s.substr(2, 3);                                      // 3 chars starting at index 2, NOT s[2..3]
    ```
11. **Comparing Floating Point Directly**:
    ```cpp
    // WRONG: exact equality on doubles is fragile due to precision
    if (a == b) { ... }

    // CORRECT: compare within an epsilon
    if (fabs(a - b) < 1e-9) { ... }
    ```
12. **Forgetting `const &` on Large Object Parameters**:
    - Passing `vector<vector<int>>` or `string` by value into a recursive/backtracking function copies it on every call — pass by reference (`&`) unless you specifically need a mutable local copy.