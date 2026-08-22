# C++ Syntax Sheet (OA)

A quick-reference guide covering syntax, member functions, time complexities, custom comparators, algorithms, and common traps for Online Assessments (OA) and Competitive Programming.

---

## Table of Contents
1. [Fast I/O & Common Macros](#1-fast-io--common-macros)
2. [Sequence Containers](#2-sequence-containers)
   - [`vector`](#vector)
   - [`string`](#string)
   - [`deque`](#deque)
   - [`list` (Doubly Linked List)](#list)
3. [Container Adapters](#3-container-adapters)
   - [`stack`](#stack)
   - [`queue`](#queue)
   - [`priority_queue` (Heaps)](#priority_queue-heaps)
4. [Associative Containers (Ordered - $O(\log N)$)](#4-associative-containers-ordered---olog-n)
   - [`set`](#set)
   - [`multiset` (Important OA Nuances!)](#multiset)
   - [`map`](#map)
   - [`multimap`](#multimap)
5. [Unordered Containers (Hash Tables - $O(1)$ avg)](#5-unordered-containers-hash-tables---o1-avg)
   - [`unordered_set` & `unordered_map`](#unordered_set--unordered_map)
   - [Custom Hash for Pairs / Anti-Hash-Collisions](#custom-hash)
6. [Utility & Bitwise Structures](#6-utility--bitwise-structures)
   - [`pair` & `tuple`](#pair--tuple)
   - [`bitset`](#bitset)
7. [Essential STL Algorithms (`<algorithm>`, `<numeric>`)](#7-essential-stl-algorithms)
   - [Sorting & Reordering](#sorting--reordering)
   - [Binary Search & Bounds](#binary-search--bounds)
   - [Permutations & Partitioning](#permutations--partitioning)
   - [Numeric Operations & Prefix Sums](#numeric-operations--prefix-sums)
   - [Coordinate Compression Pattern](#coordinate-compression-pattern)
8. [Custom Comparators Cheat Sheet](#8-custom-comparators-cheat-sheet)
9. [Bit Manipulation & Built-in Intrinsics](#9-bit-manipulation--built-in-intrinsics)
10. [Critical OA Pitfalls & Traps to Avoid](#10-critical-oa-pitfalls--traps-to-avoid)

---

## 1. Fast I/O & Common Macros

```cpp
#include <bits/stdc++.h>
using namespace std;

// Fast I/O
void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

// Common typedefs & constants
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

const int INF = 1e9 + 7;
const ll LINF = 1e18;
const int MOD = 1e9 + 7; // or 998244353

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
```

---

## 2. Sequence Containers

### `vector`
Dynamic array that supports contiguous memory and $O(1)$ amortized append.

```cpp
// 1. Initialization
vector<int> a;                            // Empty
vector<int> b(n, 0);                      // Size n, filled with 0
vector<int> c = {1, 2, 3, 4};             // Initializer list
vector<vector<int>> dp(n, vector<int>(m, -1)); // 2D: n rows x m cols with -1
vector<vector<vector<int>>> d3(n, vector<vector<int>>(m, vector<int>(k, 0))); // 3D

// 2. Modifiers
a.push_back(10);                          // Append element: O(1) amortized
a.emplace_back(10);                       // In-place construction (faster for objects/pairs)
a.pop_back();                             // Remove last element: O(1)
a.insert(a.begin() + idx, 5);             // Insert at index: O(N)
a.erase(a.begin() + idx);                 // Erase at index: O(N)
a.erase(a.begin() + l, a.begin() + r);    // Erase range [l, r): O(N)
a.clear();                                // Clear elements (capacity remains)
a.shrink_to_fit();                        // Reduce capacity to fit size
a.resize(new_size, default_val);          // Resize vector
a.assign(n, val);                         // Reset size to n with value val

// 3. Access & Capacity
int first = a.front();                    // First element: a[0]
int last  = a.back();                     // Last element: a[a.size() - 1]
bool empty = a.empty();                   // Check if empty
int size   = a.size();                    // Number of elements

// 4. Iteration
for (int i = 0; i < (int)a.size(); i++) { /* a[i] */ }
for (int &x : a) { /* modifiable reference */ }
for (const auto &x : a) { /* read-only */ }
for (auto it = a.rbegin(); it != a.rend(); ++it) { /* reverse traversal: *it */ }
```

---

### `string`
Dynamic character sequence with rich substring and search APIs.

```cpp
// 1. Initialization & Modifiers
string s = "hello";
s += " world";                            // Append: O(len)
s.push_back('!');                         // Append char: O(1)
s.pop_back();                             // Remove last char: O(1)
s.insert(pos, "abc");                     // Insert at pos: O(N)
s.erase(pos, len);                        // Erase `len` characters from `pos`: O(N)

// 2. Substrings & Search
string sub = s.substr(pos, len);          // Substring starting at pos of length len: O(len)
// Note: If len is omitted, goes till the end of the string.

size_t found = s.find("world");           // First occurrence index or string::npos
size_t rfound = s.rfind('l');             // Last occurrence index or string::npos
if (s.find("abc") != string::npos) { /* found */ }

// 3. Conversions
int num = stoi("12345");                  // String to int
ll bignum = stoll("1234567890123");       // String to long long
double d = stod("3.1415");                // String to double
string str = to_string(12345);            // Number to string

// 4. In-place Case Conversion & Reversal
reverse(s.begin(), s.end());
for (char &c : s) c = tolower(c);         // or toupper(c)
transform(s.begin(), s.end(), s.begin(), ::tolower);

// 5. String Stream (splitting by whitespace)
string sentence = "apple banana cherry";
stringstream ss(sentence);
string word;
while (ss >> word) {
    // processes "apple", then "banana", then "cherry"
}
```

---

### `deque`
Double-ended queue. Supports $O(1)$ push/pop at **both** ends and $O(1)$ random access `[]`. Perfect for sliding window / monotonic queue / 0-1 BFS.

```cpp
deque<int> dq;

dq.push_back(10);                         // O(1)
dq.push_front(20);                        // O(1)
dq.pop_back();                            // O(1)
dq.pop_front();                           // O(1)

int front_elem = dq.front();              // O(1)
int back_elem  = dq.back();               // O(1)
int idx_elem   = dq[2];                   // Random access: O(1)
dq.clear();
```

---

### `list`
Doubly linked list. Use when frequent $O(1)$ insertions/deletions in the middle are needed (e.g., **LRU Cache** alongside `unordered_map`).

```cpp
list<int> l = {1, 2, 3};
l.push_front(0);                          // O(1)
l.push_back(4);                           // O(1)
l.pop_front();
l.pop_back();

auto it = l.begin();
advance(it, 2);                           // Advance iterator by 2 steps: O(K)
auto new_it = l.insert(it, 99);           // Insert before it: O(1) returns iterator
l.erase(new_it);                          // Erase element: O(1)
l.splice(l.begin(), l, it);               // Move node `it` to front: O(1) (Crucial for LRU!)
```

---

## 3. Container Adapters

### `stack`
LIFO (Last-In-First-Out) container.

```cpp
stack<int> st;
st.push(10);                              // Push: O(1)
st.emplace(20);                           // Construct in-place: O(1)
st.pop();                                 // Remove top element (returns void): O(1)
int top_val = st.top();                   // Access top element: O(1)
bool empty  = st.empty();
int size    = st.size();
```

---

### `queue`
FIFO (First-In-First-Out) container (used in Standard BFS).

```cpp
queue<int> q;
q.push(10);                               // Push to back: O(1)
q.emplace(20);                            // Construct in-place: O(1)
q.pop();                                  // Remove from front (returns void): O(1)
int front_val = q.front();                // Access front element: O(1)
int back_val  = q.back();                 // Access last inserted element: O(1)
bool empty    = q.empty();
int size      = q.size();
```

---

### `priority_queue` (Heaps)
Default is a **Max-Heap**. Underlying container is `vector`. Operations are $O(\log N)$ push/pop and $O(1)$ top.

```cpp
// 1. Max-Heap (Default - largest on top)
priority_queue<int> max_pq;
max_pq.push(10);
max_pq.push(30);
max_pq.push(20);
int top_max = max_pq.top();               // 30
max_pq.pop();

// 2. Min-Heap (smallest on top)
priority_queue<int, vector<int>, greater<int>> min_pq;
min_pq.push(10);
min_pq.push(30);
min_pq.push(20);
int top_min = min_pq.top();               // 10
min_pq.pop();

// 3. Priority Queue of Pairs / Tuples (Min-Heap by first element - Dijkstra)
using pii = pair<int, int>;
priority_queue<pii, vector<pii>, greater<pii>> dij_pq;
dij_pq.push({dist, node});

// 4. Custom Comparator Struct
struct CustomComp {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        // Return true if `a` has LOWER priority than `b` (meaning `b` comes to top)
        if (a.first == b.first) return a.second > b.second;
        return a.first < b.first;
    }
};
priority_queue<pair<int, int>, vector<pair<int, int>>, CustomComp> custom_pq;

// 5. Custom Comparator Lambda (C++11/14/17/20)
auto cmp = [](const array<int, 3>& a, const array<int, 3>& b) {
    return a[0] > b[0]; // Min-heap on first element
};
priority_queue<array<int, 3>, vector<array<int, 3>>, decltype(cmp)> lambda_pq(cmp);
```

---

## 4. Associative Containers (Ordered - $O(\log N)$)
Implemented as Red-Black Trees. Elements are stored in sorted order.

### `set`
Stores unique elements in strictly ascending order.

```cpp
set<int> s;                               // Ascending
set<int, greater<int>> desc_s;           // Descending

s.insert(10);                             // Insert: O(log N)
s.emplace(20);                            // In-place: O(log N)
s.erase(10);                              // Erase by value: O(log N)
s.erase(s.begin());                       // Erase by iterator: O(1) amortized

// Lookup
auto it = s.find(20);                     // Returns iterator, or s.end()
int cnt = s.count(20);                    // Returns 1 if present, else 0

// Binary Search in Set (ALWAYS use member functions!)
auto it1 = s.lower_bound(15);             // First element >= 15: O(log N)
auto it2 = s.upper_bound(15);             // First element > 15: O(log N)

// Min and Max elements in set
int min_val = *s.begin();                 // Smallest
int max_val = *s.rbegin();                // Largest (or *prev(s.end()))
```

---

### `multiset`
Stores duplicate elements in sorted order.

> [!WARNING]
> **CRITICAL OA TRAP with `multiset::erase`:**
> - `ms.erase(val)` removes **ALL** copies of `val` in $O(\log N + \text{count})$!
> - `ms.erase(ms.find(val))` removes **ONLY ONE** single copy in $O(\log N)$!

```cpp
multiset<int> ms;
ms.insert(5);
ms.insert(5);
ms.insert(5); // ms = {5, 5, 5}

// Remove only one occurrence of 5:
auto it = ms.find(5);
if (it != ms.end()) {
    ms.erase(it);                         // ms = {5, 5}
}

// CAUTION: ms.count(val) is O(count + log N). Avoid if there are many duplicates!
// To check existence in O(log N), use: ms.find(val) != ms.end()

// lower_bound and upper_bound work identically to set
auto lb = ms.lower_bound(5);              // First 5
auto ub = ms.upper_bound(5);              // First element > 5
```

---

### `map`
Key-Value store sorted by Key.

```cpp
map<string, int> mp;

// 1. Insertion & Access
mp["apple"] = 5;                          // Creates or updates: O(log N)
mp.insert({"banana", 2});                 // Insert pair: O(log N)
mp.emplace("cherry", 8);                  // In-place: O(log N)

// 2. Lookup & Safety
// NOTE: mp[key] automatically inserts default value if key does not exist!
if (mp.find("apple") != mp.end()) { /* exists */ }
if (mp.count("apple")) { /* exists */ }

// 3. Deletion
mp.erase("banana");                       // Erase by key: O(log N)
mp.erase(mp.begin());                     // Erase by iterator

// 4. Binary search on Keys
auto it = mp.lower_bound("b");            // First key >= "b"

// 5. Iteration (C++17 Structured Binding)
for (auto& [key, val] : mp) {
    cout << key << ": " << val << "\n";
}
for (auto it = mp.begin(); it != mp.end(); ++it) {
    cout << it->first << " -> " << it->second << "\n";
}
```

---

### `multimap`
Key-Value store where keys can repeat. Does **not** support `operator[]`.

```cpp
multimap<int, string> mm;
mm.insert({1, "one_a"});
mm.insert({1, "one_b"});

auto range = mm.equal_range(1);           // Returns pair of iterators [first, last)
for (auto it = range.first; it != range.second; ++it) {
    cout << it->second << " ";
}
```

---

## 5. Unordered Containers (Hash Tables - $O(1)$ avg)
Implemented using hash tables with separate chaining.

### `unordered_set` & `unordered_map`

```cpp
unordered_set<int> us;
us.insert(10);                            // O(1) avg, O(N) worst-case
us.erase(10);                             // O(1) avg
if (us.find(10) != us.end()) { /* found */ }

unordered_map<int, int> um;
um[42] = 100;                             // O(1) avg, O(N) worst-case
um.reserve(1024);                         // Performance tip: pre-allocate buckets
um.max_load_factor(0.25);                 // Performance tip: reduce collision rate
```

---

### Custom Hash
By default, `unordered_map` has no hash function for `pair` or `tuple`, and standard hash on integers can be hacked to $O(N)$ worst-case.

#### Hash for `pair<int, int>`:
```cpp
struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        return (size_t)p.first ^ ((size_t)p.second << 32) ^ 0x9e3779b97f4a7c15ULL;
    }
};

unordered_map<pair<int, int>, int, PairHash> pair_map;
unordered_set<pair<int, int>, PairHash> pair_set;
```

#### Anti-Hack Custom Hash (splitmix64) for Codeforces/OAs:
```cpp
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

unordered_map<long long, int, custom_hash> safe_map;
```

---

## 6. Utility & Bitwise Structures

### `pair` & `tuple`

```cpp
// Pair
pair<int, string> p = {1, "hello"};
int id = p.first;
string name = p.second;
auto p2 = make_pair(2, "world");

// Tuple (3 or more elements)
tuple<int, string, double> t = {1, "alpha", 3.14};
int x = get<0>(t);
string y = get<1>(t);
double z = get<2>(t);

// Structured Binding (C++17) - Easiest & cleanest for OA:
auto [a, b, c] = t;

// Tie (unpacking or comparing)
int u; string v; double w;
tie(u, v, w) = t;
tie(ignore, v, ignore) = t;               // Ignore unwanted values
```

---

### `bitset`
Fixed-size sequence of $N$ bits. Operations are bitwise parallelized ($64\times$ speedup), ideal for subset sum, graph reachability, 0-1 knapsack in $O(\frac{N \times W}{64})$.

```cpp
bitset<64> b;                             // All 0s: 0000...0000
bitset<64> b2(42);                        // From integer: binary representation of 42
bitset<64> b3("110101");                  // From binary string

// 1. Bit Access & Modification
b.set(3);                                 // Set bit 3 to 1: O(1)
b.set(3, 0);                              // Set bit 3 to 0: O(1)
b.set();                                  // Set ALL bits to 1
b.reset(3);                               // Set bit 3 to 0
b.reset();                                // Set ALL bits to 0
b.flip(3);                                // Toggle bit 3
b.flip();                                 // Toggle ALL bits
bool val = b.test(3);                     // Check bit 3 (bounds-checked): returns bool
bool val2 = b[3];                         // Access bit 3 (operator[])

// 2. Queries
int ones  = b.count();                    // Count number of set bits (popcount): O(N/64)
int total = b.size();                     // Number of bits (template parameter N)
bool any_set = b.any();                   // True if >= 1 bit is set
bool none_set = b.none();                 // True if 0 bits are set
bool all_set = b.all();                   // True if all bits are set

// 3. Conversions & Shifts
string bit_str = b.to_string();           // Convert to binary string
unsigned long u_long = b.to_ulong();      // Convert to unsigned long
unsigned long long u_ll = b.to_ullong();  // Convert to unsigned long long
b = (b << 5);                             // Shift left by 5 bits: O(N/64)
b = (b >> 2);                             // Shift right by 2 bits: O(N/64)

// 4. Typical OA Pattern: 0/1 Knapsack Reachability
bitset<100005> reachable;
reachable[0] = 1;
for (int weight : items) {
    reachable |= (reachable << weight);   // Reaches all new sums in O(MAX_SUM / 64)
}
if (reachable[target]) { /* Target sum is reachable! */ }
```

---

## 7. Essential STL Algorithms

```cpp
#include <algorithm>
#include <numeric>
```

### Sorting & Reordering

```cpp
vector<int> a = {5, 2, 8, 1, 9};

sort(a.begin(), a.end());                 // Ascending: O(N log N)
sort(a.rbegin(), a.rend());               // Descending: O(N log N)
sort(a.begin(), a.end(), greater<int>()); // Descending using functor

// Stable sort (preserves relative order of equal elements)
stable_sort(a.begin(), a.end(), custom_comparator);

// Reverse range
reverse(a.begin(), a.end());              // Inverts order: O(N)

// Check if sorted
bool ok = is_sorted(a.begin(), a.end());
```

---

### Binary Search & Bounds
> [!IMPORTANT]
> The range **MUST BE SORTED** before applying binary search functions!

```cpp
vector<int> a = {10, 20, 20, 20, 30, 40};

// 1. binary_search: Returns true/false
bool exists = binary_search(a.begin(), a.end(), 20); // true

// 2. lower_bound: First element >= X
auto it1 = lower_bound(a.begin(), a.end(), 20);
int idx1 = it1 - a.begin();               // idx = 1 (points to first 20)

// 3. upper_bound: First element > X
auto it2 = upper_bound(a.begin(), a.end(), 20);
int idx2 = it2 - a.begin();               // idx = 4 (points to 30)

// Count occurrences of X in sorted array in O(log N):
int count_20 = upper_bound(all(a), 20) - lower_bound(all(a), 20); // 3

// 4. equal_range: Pair of [lower_bound, upper_bound]
auto [low, high] = equal_range(a.begin(), a.end(), 20);
```

---

### Permutations & Partitioning

```cpp
// 1. Next & Previous Permutations (Array MUST be sorted for all permutations)
vector<int> p = {1, 2, 3};
do {
    // Process permutation {1,2,3}, {1,3,2}, etc.
} while (next_permutation(p.begin(), p.end()));

prev_permutation(p.begin(), p.end());

// 2. Min/Max Elements & Clamp
auto min_it = min_element(a.begin(), a.end()); // Returns iterator to minimum
auto max_it = max_element(a.begin(), a.end()); // Returns iterator to maximum
int min_val = *min_it;
int min_idx = min_it - a.begin();

int bounded = clamp(val, low_limit, high_limit); // Clamps val in [low, high] (C++17)
```

---

### Numeric Operations & Prefix Sums

```cpp
vector<int> a = {1, 2, 3, 4, 5};

// 1. Accumulate (Sum of elements)
// WARNING: 3rd argument type determines calculation type! Use 0LL for 64-bit int:
ll sum = accumulate(a.begin(), a.end(), 0LL); 

// 2. Iota (Generate sequence: 0, 1, 2, ..., n-1)
vector<int> p(n);
iota(p.begin(), p.end(), 0);              // p becomes {0, 1, 2, ..., n-1}

// 3. Prefix Sum (partial_sum)
vector<int> pref(n);
partial_sum(a.begin(), a.end(), pref.begin()); // pref[i] = sum(a[0]..a[i])

// 4. GCD and LCM (C++17 built-in)
ll g = gcd(a_val, b_val);                 // Greatest common divisor
ll l = lcm(a_val, b_val);                 // Least common multiple
```

---

### Coordinate Compression Pattern
Extremely common in segment trees, sweep-line, and Fenwick trees.

```cpp
vector<int> coords = {1000, 5, 200, 5, 1000, 40};
vector<int> temp = coords;

// 1. Sort and remove duplicates
sort(temp.begin(), temp.end());
temp.erase(unique(temp.begin(), temp.end()), temp.end());
// temp is now: {5, 40, 200, 1000}

// 2. Map original values to compressed rank [0, unique_count - 1]
auto get_compressed = [&](int val) {
    return lower_bound(temp.begin(), temp.end(), val) - temp.begin();
};

for (int &x : coords) {
    x = get_compressed(x);                // Converts to 0-indexed ranks
}
```

---

## 8. Custom Comparators Cheat Sheet

> [!IMPORTANT]
> **Strict Weak Ordering Rule:** Always use `<` (strict less-than) or `>` (strict greater-than). **NEVER use `<=` or `>=`** in custom comparators, as doing so leads to undefined behavior / crashes due to strict weak ordering violations.

### 1. Vector Sorting with Lambda
```cpp
struct Interval { int start, end, id; };
vector<Interval> intervals;

// Sort by start ascending; if equal, by end descending:
sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
    if (a.start != b.start) return a.start < b.start;
    return a.end > b.end;
});
```

### 2. Standalone Function Comparator
```cpp
bool comp(const pair<int, int>& a, const pair<int, int>& b) {
    if (a.first == b.first) return a.second < b.second;
    return a.first > b.first;             // First element descending
}
sort(vec.begin(), vec.end(), comp);
```

### 3. Struct with `operator<` (Overloading `<`)
```cpp
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;     // Ascending by weight
    }
};
vector<Edge> edges;
sort(edges.begin(), edges.end());
```

### 4. `set` / `map` Custom Comparator
```cpp
struct Comp {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    }
};
set<pair<int, int>, Comp> custom_set;
map<pair<int, int>, int, Comp> custom_map;
```

---

## 9. Bit Manipulation & Built-in Intrinsics

GCC / Clang built-in functions operate in $O(1)$ hardware instructions.

| Function (`long long` version) | Meaning / Action | Example (`x = 18` = `0b...010010`) |
| :--- | :--- | :--- |
| `__builtin_popcountll(x)` | Count of set bits ($1$s) | `__builtin_popcountll(18)` $\rightarrow 2$ |
| `__builtin_clzll(x)` | Count of **leading** zeros ($64 - 1 - \lfloor\log_2 x\rfloor$) | `__builtin_clzll(18)` $\rightarrow 59$ |
| `__builtin_ctzll(x)` | Count of **trailing** zeros (index of lowest set bit) | `__builtin_ctzll(18)` $\rightarrow 1$ |
| `__lg(x)` | $\lfloor\log_2(x)\rfloor$ (Index of highest set bit) | `__lg(18)` $\rightarrow 4$ |

### Common Bitmask Hacks

```cpp
// 1. Lowest set bit (LSB)
ll lsb = (x & -x);

// 2. Remove lowest set bit
x = (x & (x - 1));

// 3. Check if number is power of 2
bool is_pow2 = (x > 0) && ((x & (x - 1)) == 0);

// 4. Set / Clear / Toggle k-th bit (0-indexed)
x |= (1LL << k);                          // Set k-th bit
x &= ~(1LL << k);                         // Clear k-th bit
x ^= (1LL << k);                          // Toggle k-th bit
bool is_set = (x >> k) & 1LL;             // Check k-th bit

// 5. Iterate over all submasks of mask 'm'
for (int sub = m; sub > 0; sub = (sub - 1) & m) {
    // processes non-empty submask 'sub'
}

// 6. Iterate all subsets of size N:
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if (mask & (1 << i)) { /* element i included */ }
    }
}
```

---

## 10. Critical OA Pitfalls & Traps to Avoid

### 1. Integer Overflow in Operations & Accumulate
```cpp
// BAD:
long long prod = a * b;                   // If a and b are int, product overflows before assignment!
long long sum = accumulate(v.begin(), v.end(), 0); // Accumulates in 32-bit int!

// GOOD:
long long prod = 1LL * a * b;
long long sum = accumulate(v.begin(), v.end(), 0LL);
```

### 2. Modulo with Negative Numbers
```cpp
// In C++, (-5) % 3 is -2, NOT 1!
// Correct standard formula for (a - b) % MOD:
long long safe_mod(long long a, long long m = MOD) {
    return (a % m + m) % m;
}
```

### 3. Binary Search in `set` / `map`
```cpp
set<int> s;
// BAD: O(N) linear scan! (lower_bound treats set iterators as non-random-access)
auto it = lower_bound(s.begin(), s.end(), val); 

// GOOD: O(log N) tree traversal
auto it = s.lower_bound(val); 
```

### 4. `vector<bool>` Proxy Reference Gotcha
```cpp
// vector<bool> is space-optimized to 1 bit per element and returns a proxy object, not bool&.
// BAD:
bool &ref = vec_bool[0];                  // Compilation error!

// GOOD:
bool val = vec_bool[0];                   // Copy value
auto ref = vec_bool[0];                   // Proxy reference
```

### 5. Infinite Loops in `for (int i = 0; i <= v.size() - 1; ...)`
```cpp
// If v is empty, v.size() is 0.
// Since size() returns unsigned size_t, (v.size() - 1) wraps around to 18446744073709551615!
// ALWAYS cast size():
for (int i = 0; i < (int)v.size(); i++)
```

### 6. Map `operator[]` Side Effects in Read-only Checks
```cpp
map<int, int> mp;
// Calling mp[x] will INSERT key x with value 0 if it doesn't exist, increasing map size!
// Use find or count:
if (mp.count(x)) { /* ... */ }
```

---

## Quick Complexity Summary Table

| Container | Access `[]` | Search | Insert | Erase | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `vector` | $O(1)$ | $O(N)$ / $O(\log N)$* | $O(1)$ amortized back, $O(N)$ middle | $O(1)$ back, $O(N)$ middle | *$\log N$ if sorted |
| `deque` | $O(1)$ | $O(N)$ | $O(1)$ front/back, $O(N)$ middle | $O(1)$ front/back | Monotonic queue, 0-1 BFS |
| `list` | $O(N)$ | $O(N)$ | $O(1)$ at known iterator | $O(1)$ at known iterator | LRU Cache |
| `stack` / `queue` | $N/A$ | $N/A$ | $O(1)$ push | $O(1)$ pop | Adapter |
| `priority_queue`| $O(1)$ top | $N/A$ | $O(\log N)$ push | $O(\log N)$ pop | Max/Min heap |
| `set` / `map` | $O(\log N)$ map | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | Red-Black Tree (Ordered) |
| `multiset` | $N/A$ | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ iterator, $O(\log N + k)$ val | Duplicate keys |
| `unordered_set/map`| $O(1)$ map | $O(1)$ avg, $O(N)$ worst | $O(1)$ avg, $O(N)$ worst | $O(1)$ avg, $O(N)$ worst | Hash table |
| `bitset` | $O(1)$ test | Bitwise | $O(1)$ set/reset | $O(1)$ | $64\times$ speedup |
