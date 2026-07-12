# C++ STL Containers
## Master Interview & OA Guide

---

## How to Decide What to Use

```
Do you need fast lookup by key?
  Key is an integer or string, order doesn't matter  -> unordered_map / unordered_set
  Key is any type, need sorted order                 -> map / set

Do you need a sequence of elements?
  Random access by index, frequent push_back         -> vector
  Frequent push/pop from BOTH ends                   -> deque
  Frequent push/pop from ONE end only                -> stack (LIFO) or queue (FIFO)

Do you need the smallest or largest element always at front?
  -> priority_queue (heap)

Do you need fast insert/delete in the MIDDLE?
  -> list (doubly linked list)

Do you need elements always sorted with fast insert/delete?
  -> set / multiset

Do you need key-value pairs always sorted by key?
  -> map / multimap

Do you need O(1) average lookup AND sorted order?
  -> Not possible with one container. Use map (O(log n)) or unordered_map (O(1) avg).
```

---

# 1. vector

The default container. Use it unless you have a specific reason not to.

## Complexity

| Operation | TC | Notes |
|---|---|---|
| Access by index `v[i]` | O(1) | No bounds check |
| `at(i)` | O(1) | Bounds-checked, throws exception |
| `push_back` | O(1) amortized | Doubles capacity on resize |
| `pop_back` | O(1) | |
| `insert` at position | O(n) | Shifts elements right |
| `erase` at position | O(n) | Shifts elements left |
| `front` / `back` | O(1) | |
| `size` / `empty` | O(1) | |
| `find` (linear scan) | O(n) | Not built-in; use `find()` from `<algorithm>` |
| Sort | O(n log n) | `sort(v.begin(), v.end())` |
| Binary search (sorted) | O(log n) | `lower_bound`, `upper_bound` |

**Space:** O(n). Capacity may be up to 2x size after resizes.

## Initialization

```cpp
#include <vector>
using namespace std;

vector<int> v;                          // empty
vector<int> v(5);                       // [0, 0, 0, 0, 0]
vector<int> v(5, 7);                    // [7, 7, 7, 7, 7]
vector<int> v = {1, 2, 3, 4};          // initializer list
vector<int> v(other.begin(), other.end()); // copy from another container

vector<vector<int>> grid(rows, vector<int>(cols, 0)); // 2D grid
```

## Common Operations

```cpp
v.push_back(10);           // add to end
v.pop_back();              // remove from end
v.insert(v.begin() + i, x); // insert at index i  O(n)
v.erase(v.begin() + i);   // erase at index i     O(n)
v.erase(v.begin()+i, v.begin()+j); // erase range [i, j)
v.clear();                 // remove all elements
v.size();                  // number of elements
v.empty();                 // true if size == 0
v.resize(n);               // resize to n (new elements = 0)
v.reserve(n);              // pre-allocate capacity (avoids reallocations)
v.back();                  // last element
v.front();                 // first element

// Iteration
for (int x : v) { }
for (int i = 0; i < v.size(); i++) { v[i]; }

// Sort
sort(v.begin(), v.end());                         // ascending
sort(v.begin(), v.end(), greater<int>());         // descending
sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // custom

// Useful algorithms
reverse(v.begin(), v.end());
auto it = find(v.begin(), v.end(), target);       // O(n) linear find
auto it = lower_bound(v.begin(), v.end(), x);     // O(log n) — requires sorted
int idx = lower_bound(v.begin(), v.end(), x) - v.begin(); // index
bool has = binary_search(v.begin(), v.end(), x);  // O(log n) — requires sorted
int mx = *max_element(v.begin(), v.end());
int mn = *min_element(v.begin(), v.end());
int total = accumulate(v.begin(), v.end(), 0);
```

## Notes to Remember
- `v[i]` does not check bounds. Use `v.at(i)` in debug if you suspect out-of-bounds.
- Erasing or inserting in the middle invalidates all iterators. Rebuild if needed.
- `reserve(n)` before mass `push_back` prevents repeated reallocations — can be 2x faster.
- `size()` returns `size_t` (unsigned). `v.size() - 1` when size is 0 wraps to a huge number. Guard with `v.empty()` first.
- To remove a value (not by index), use `v.erase(remove(v.begin(), v.end(), val), v.end())`.

---

# 2. string

Behaves like `vector<char>` with extra string-specific methods.

## Complexity

| Operation | TC |
|---|---|
| Access `s[i]` | O(1) |
| `push_back` / `pop_back` | O(1) amortized |
| `substr(pos, len)` | O(len) |
| `find(pattern)` | O(n*m) naive |
| `+` concatenation | O(n) — creates new string |
| `+=` append | O(len) amortized |
| `size` / `length` | O(1) |
| Comparison `==`, `<` | O(n) |

## Initialization & Operations

```cpp
string s;                          // empty
string s = "hello";
string s(5, 'a');                  // "aaaaa"
string s = to_string(42);         // int to string
int n = stoi("42");               // string to int
long long n = stoll("42");
double d = stod("3.14");

s.size();   s.length();           // same thing
s.empty();
s.push_back('c');
s.pop_back();
s += "world";                     // append
s.append("world");
s.substr(2, 3);                   // substring starting at 2, length 3
s.find("lo");                     // returns index or string::npos
s.find('o', 3);                   // search starting from index 3
s.replace(1, 3, "xyz");           // replace 3 chars at index 1 with "xyz"
s.erase(2, 4);                    // erase 4 chars starting at index 2
s.insert(2, "xyz");               // insert at index 2

// Char checks (from <cctype>)
isalpha(c);  isdigit(c);  isalnum(c);
isupper(c);  islower(c);
toupper(c);  tolower(c);

// Split string (no built-in — use stringstream)
#include <sstream>
stringstream ss("a b c");
string token;
vector<string> tokens;
while (ss >> token) tokens.push_back(token);
```

## Notes to Remember
- `string::npos` is returned by `find()` when not found. Check `if (s.find(x) != string::npos)`.
- Prefer `+=` over `+` in loops. `+` creates a new string each time — O(n) per call in a loop = O(n^2) total.
- `s[i]` is a `char`. To get a single-char string use `string(1, s[i])`.
- Strings are mutable in C++ unlike Java/Python. You can do `s[i] = 'x'` directly.

---

# 3. unordered_map

Hash map. Best for fast key-value lookup when order does not matter.

## Complexity

| Operation | TC Average | TC Worst |
|---|---|---|
| `insert` / `[]` | O(1) | O(n) — hash collision |
| `find` / `count` | O(1) | O(n) |
| `erase` | O(1) | O(n) |
| Iteration | O(n) | O(n) |

**Space:** O(n).

## Initialization & Operations

```cpp
#include <unordered_map>

unordered_map<string, int> mp;
unordered_map<int, int> mp = {{1, 10}, {2, 20}};

// Insert / update
mp["key"] = 5;           // insert or overwrite
mp.insert({key, val});   // insert only if key not present

// Access
mp["key"];               // inserts default (0) if key missing — use carefully
mp.at("key");            // throws if key missing — safer

// Check existence
mp.count("key");         // 0 or 1 (not in map or in map)
mp.find("key") != mp.end(); // same, but gives iterator

// Erase
mp.erase("key");

// Frequency counting (classic pattern)
unordered_map<int, int> freq;
for (int x : nums) freq[x]++;

// Iteration
for (auto& [key, val] : mp) {
    // use key and val
}

// Size
mp.size();
mp.empty();
mp.clear();
```

## Notes to Remember
- `mp[key]` **inserts** the key with default value 0 if it does not exist. Never use `mp[key]` in a condition check — use `mp.count(key)` or `mp.find(key)`.
- Worst case O(n) happens on hash collision (rare in practice, common in adversarial tests). Switch to `map` if you get TLE on hash map problems.
- Custom hash for pairs (needed sometimes):
  ```cpp
  struct PairHash {
      size_t operator()(const pair<int,int>& p) const {
          return hash<long long>()(((long long)p.first << 32) | p.second);
      }
  };
  unordered_map<pair<int,int>, int, PairHash> mp;
  ```
- Iteration order is arbitrary — do not rely on it.

---

# 4. map

Ordered map. Keys always sorted. Use when you need sorted order or range queries on keys.

## Complexity

| Operation | TC |
|---|---|
| `insert` / `[]` | O(log n) |
| `find` / `count` | O(log n) |
| `erase` | O(log n) |
| Iteration | O(n) |
| `lower_bound(key)` | O(log n) |
| `upper_bound(key)` | O(log n) |

**Space:** O(n). Red-black tree internally — higher constant than unordered_map.

## Initialization & Operations

```cpp
#include <map>

map<string, int> mp;
map<int, int> mp = {{1, 10}, {3, 30}};

// Same insert/access/erase as unordered_map

// Range queries — only possible with map, not unordered_map
mp.lower_bound(k);    // iterator to first key >= k
mp.upper_bound(k);    // iterator to first key >  k
mp.begin();           // smallest key
mp.rbegin();          // largest key (reverse iterator)

// Example: sum of values for keys in range [lo, hi]
auto it = mp.lower_bound(lo);
while (it != mp.upper_bound(hi)) {
    sum += it->second;
    ++it;
}
```

## Notes to Remember
- Use `map` over `unordered_map` when: you need sorted iteration, range queries (`lower_bound`/`upper_bound`), or ordered processing.
- `mp.begin()->first` is the minimum key. `mp.rbegin()->first` is the maximum key.
- Structured bindings work the same: `for (auto& [k, v] : mp)`.
- `map` with `lower_bound` / `upper_bound` is the go-to for sweep line problems and interval tracking.

---

# 5. unordered_set

Hash set. Fast membership checking. No duplicates, no order.

## Complexity

| Operation | TC Average | TC Worst |
|---|---|---|
| `insert` | O(1) | O(n) |
| `count` / `find` | O(1) | O(n) |
| `erase` | O(1) | O(n) |
| Iteration | O(n) | O(n) |

**Space:** O(n).

## Initialization & Operations

```cpp
#include <unordered_set>

unordered_set<int> st;
unordered_set<int> st = {1, 2, 3};
unordered_set<int> st(v.begin(), v.end()); // build from vector

st.insert(x);
st.erase(x);
st.count(x);          // 0 or 1
st.find(x) != st.end();
st.size();
st.empty();
st.clear();

for (int x : st) { }  // arbitrary order
```

## Notes to Remember
- The most common use: fast O(1) visited check in BFS/DFS instead of a `vector<bool>`.
- Unlike `set`, has no ordering and no `lower_bound` / `upper_bound`.
- Converting a vector to a set removes duplicates: `unordered_set<int> st(v.begin(), v.end())`.
- For small fixed-size bool arrays (like visited in grids), `vector<bool>` or a raw `bool[]` is faster in practice.

---

# 6. set

Ordered set. Elements always sorted. No duplicates.

## Complexity

| Operation | TC |
|---|---|
| `insert` | O(log n) |
| `find` / `count` | O(log n) |
| `erase` | O(log n) |
| `lower_bound` / `upper_bound` | O(log n) |
| Iteration | O(n) |
| Min element (`*begin()`) | O(1) |
| Max element (`*rbegin()`) | O(1) |

**Space:** O(n).

## Initialization & Operations

```cpp
#include <set>

set<int> st;
set<int> st = {3, 1, 2};    // stored as {1, 2, 3}
set<int, greater<int>> st;  // descending order

st.insert(x);
st.erase(x);                // erase by value
st.erase(st.find(x));       // erase by iterator
st.count(x);                // 0 or 1
*st.begin();                // minimum element
*st.rbegin();               // maximum element
st.lower_bound(x);          // iterator to first element >= x
st.upper_bound(x);          // iterator to first element >  x

// Erase a range
st.erase(st.lower_bound(lo), st.upper_bound(hi));
```

## Notes to Remember
- `set` does not support index access. Cannot do `st[0]`.
- `set::lower_bound(x)` is O(log n). `std::lower_bound(st.begin(), st.end(), x)` is O(n) for sets — always use the member function.
- Use `set` when you need: "is this value already seen?" + "what is the nearest value?" (sliding window problems with ordered tracking).
- `multiset` allows duplicates. Erase all copies with `ms.erase(x)`, erase one copy with `ms.erase(ms.find(x))`.

---

# 7. stack

LIFO. Only access the top.

## Complexity

| Operation | TC |
|---|---|
| `push` | O(1) |
| `pop` | O(1) |
| `top` | O(1) |
| `size` / `empty` | O(1) |

**Space:** O(n). Backed by `deque` by default.

## Initialization & Operations

```cpp
#include <stack>

stack<int> st;
stack<int, vector<int>> st; // backed by vector (slightly faster)

st.push(x);
st.pop();         // removes top — does NOT return value
st.top();         // view top without removing
st.empty();
st.size();

// Common pattern: process while stack not empty
while (!st.empty()) {
    int x = st.top();
    st.pop();
    // process x
}
```

## Notes to Remember
- `pop()` does not return the value. Always `top()` then `pop()`.
- No iteration. Cannot peek below the top without popping.
- In interview problems, `string` or `vector<int>` used as a stack is often cleaner: `s.push_back(c)` / `s.pop_back()` / `s.back()`.
- Stack is just a restricted interface on top of deque/vector. If you need to iterate over the elements, use `vector` directly.

---

# 8. queue

FIFO. Elements added at back, removed from front. Used for BFS.

## Complexity

| Operation | TC |
|---|---|
| `push` | O(1) |
| `pop` | O(1) |
| `front` / `back` | O(1) |
| `size` / `empty` | O(1) |

**Space:** O(n).

## Initialization & Operations

```cpp
#include <queue>

queue<int> q;

q.push(x);      // add to back
q.pop();        // remove from front — does NOT return value
q.front();      // view front
q.back();       // view back
q.empty();
q.size();

// Standard BFS pattern
queue<int> q;
q.push(start);
visited[start] = true;
while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push(neighbor);
        }
    }
}
```

## Notes to Remember
- `pop()` does not return the value. Always `front()` then `pop()`.
- For multi-source BFS, push all sources before the loop starts.
- `queue` cannot be iterated. If you need both FIFO and iteration, use `deque`.

---

# 9. deque

Double-ended queue. O(1) push/pop at both ends, O(1) random access.

## Complexity

| Operation | TC |
|---|---|
| `push_front` / `push_back` | O(1) amortized |
| `pop_front` / `pop_back` | O(1) |
| Access by index `dq[i]` | O(1) |
| `insert` in middle | O(n) |
| `size` / `empty` | O(1) |

**Space:** O(n). Internally a sequence of fixed-size chunks.

## Initialization & Operations

```cpp
#include <deque>

deque<int> dq;
deque<int> dq = {1, 2, 3};

dq.push_back(x);
dq.push_front(x);
dq.pop_back();
dq.pop_front();
dq.front();
dq.back();
dq[i];           // random access
dq.size();
dq.empty();

// Classic use: sliding window maximum (monotonic deque)
// Store indices, maintain decreasing order of values
deque<int> monoDeque; // stores indices
for (int i = 0; i < n; i++) {
    // Remove index out of window
    if (!monoDeque.empty() && monoDeque.front() < i - k + 1)
        monoDeque.pop_front();
    // Remove indices with smaller values (useless for max)
    while (!monoDeque.empty() && nums[monoDeque.back()] < nums[i])
        monoDeque.pop_back();
    monoDeque.push_back(i);
    if (i >= k - 1)
        result.push_back(nums[monoDeque.front()]);
}
```

## Notes to Remember
- Deque is the right choice for 0-1 BFS (push to front for cost-0 edges, push to back for cost-1 edges).
- Unlike vector, deque does not guarantee contiguous memory — cannot pass `dq.data()` to C APIs.
- Monotonic deque is the critical pattern: the deque stores indices in a strictly increasing order, and values at those indices form a decreasing (or increasing) sequence.

---

# 10. priority_queue

Max heap by default. Always gives the largest element.

## Complexity

| Operation | TC |
|---|---|
| `push` | O(log n) |
| `pop` | O(log n) |
| `top` | O(1) |
| Build from n elements | O(n) |
| `size` / `empty` | O(1) |

**Space:** O(n).

## Initialization & Operations

```cpp
#include <queue>

// Max heap (default)
priority_queue<int> maxPQ;

// Min heap
priority_queue<int, vector<int>, greater<int>> minPQ;

// Max heap of pairs (compares first element first)
priority_queue<pair<int,int>> pairPQ;

// Min heap of pairs
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> minPairPQ;

// Custom comparator (min heap by second element)
auto cmp = [](pair<int,int>& a, pair<int,int>& b) {
    return a.second > b.second; // greater = lower priority = min heap
};
priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);

// Build from existing vector — O(n)
vector<int> v = {3, 1, 4, 1, 5};
priority_queue<int> pq(v.begin(), v.end());

pq.push(x);
pq.pop();        // removes top — does NOT return value
pq.top();        // view top without removing
pq.empty();
pq.size();
```

## Notes to Remember
- `pop()` does not return the value. Always `top()` then `pop()`.
- No iteration, no random access, cannot update a key in-place — this is a limitation of the heap. For Dijkstra, push a new entry with updated cost and skip stale entries with `if (cost > dist[node]) continue`.
- Custom comparator: the comparator returns true if `a` has **lower priority** than `b` (i.e., `a` should come out **later**). This is opposite to `sort` comparators.
  - Want smallest first (min heap): `return a > b` (a has lower priority if a is larger).
- Heapify in O(n): `priority_queue<int> pq(v.begin(), v.end())`.

---

# 11. multiset / multimap

Like `set`/`map` but allow duplicate keys.

## Complexity
Same as `set` and `map`: O(log n) for all operations.

## Key Use Case: Ordered multiset as a sorted sliding window

```cpp
#include <set>

multiset<int> window;
window.insert(x);
window.erase(window.find(x));  // erase ONE copy (not all)
*window.begin();               // minimum
*window.rbegin();              // maximum

// LC 480. Sliding Window Median — use two multisets
multiset<int> lower, upper; // lower = max half, upper = min half
```

## Notes to Remember
- `ms.erase(x)` removes ALL copies of x. `ms.erase(ms.find(x))` removes exactly ONE.
- For a sliding window where you need quick min/max with deletions, `multiset` is the right tool.
- `multimap` is rarely used directly; most problems use `unordered_map<key, vector<value>>` instead.

---

# 12. pair and tuple

Not containers but used constantly alongside containers.

## Operations

```cpp
#include <utility>   // pair
#include <tuple>     // tuple

pair<int, int> p = {1, 2};
p.first;   p.second;
auto p = make_pair(1, 2);

tuple<int, int, int> t = {1, 2, 3};
get<0>(t); get<1>(t); get<2>(t);
auto t = make_tuple(1, 2, 3);
auto [a, b, c] = t;  // structured binding (C++17)

// Pairs are compared lexicographically: first by .first, then by .second
// This makes them naturally sortable
vector<pair<int,int>> v = {{3,1},{1,2},{3,0}};
sort(v.begin(), v.end()); // -> {{1,2},{3,0},{3,1}}
```

## Notes to Remember
- Pairs in a priority_queue are compared by `.first` first, then `.second`. To make a min heap of `(distance, node)`, use `greater<pair<int,int>>`.
- Structured bindings (`auto& [k, v]`) require C++17. Always use in competitive programming.
- To sort by second element: `sort(v.begin(), v.end(), [](auto& a, auto& b){ return a.second < b.second; })`.

---

# Summary Comparison Table

| Container | Ordered | Duplicates | Access | Insert | Delete | Find | Best Use Case |
|---|---|---|---|---|---|---|---|
| `vector` | By index | Yes | O(1) | O(1) back | O(n) mid | O(n) | Default sequence |
| `string` | By index | Yes | O(1) | O(1) amort | O(n) mid | O(n) | Character sequences |
| `unordered_map` | No | No (keys) | - | O(1) avg | O(1) avg | O(1) avg | Fast key-value lookup |
| `map` | By key | No (keys) | - | O(log n) | O(log n) | O(log n) | Sorted key-value, range queries |
| `unordered_set` | No | No | - | O(1) avg | O(1) avg | O(1) avg | Fast membership check |
| `set` | Sorted | No | - | O(log n) | O(log n) | O(log n) | Sorted unique values, nearest neighbor |
| `multiset` | Sorted | Yes | - | O(log n) | O(log n) | O(log n) | Sorted with duplicates, sliding window |
| `stack` | LIFO | Yes | Top only | O(1) | O(1) | - | Matching, undo, DFS |
| `queue` | FIFO | Yes | Front/Back | O(1) | O(1) | - | BFS, level order |
| `deque` | By index | Yes | O(1) | O(1) ends | O(1) ends | O(n) | Push/pop both ends, monotonic deque |
| `priority_queue` | Heap | Yes | Top only | O(log n) | O(log n) | - | Always get min/max |

---

# Common Patterns and Which Container to Use

| Pattern | Container |
|---|---|
| BFS traversal | `queue` |
| DFS iterative | `stack` |
| Frequency counting | `unordered_map<T, int>` |
| Visited set in BFS/DFS | `unordered_set<T>` |
| Sliding window max/min | `deque` (monotonic) |
| Sliding window with ordered access | `multiset` |
| Kth largest / smallest stream | `priority_queue` |
| Median of stream | Two `priority_queue`s |
| Merge k sorted lists | `priority_queue` (min heap) |
| Sorted key-value with range queries | `map` |
| Two sum / lookup complement | `unordered_map` or `unordered_set` |
| Remove duplicates from collection | `unordered_set` |
| Events sorted by time | `map<int, int>` (sweep line) |
| Graph adjacency list | `vector<vector<int>>` or `vector<unordered_set<int>>` |
| Trie node children | `unordered_map<char, TrieNode*>` or `TrieNode* children[26]` |

---

# Quick Decision Flowchart

```
Need key-value?
  YES -> Ordered (range queries needed)?
           YES -> map
           NO  -> unordered_map
  NO  -> Need uniqueness only (set)?
           YES -> Ordered (sorted / lower_bound)?
                    YES -> set / multiset
                    NO  -> unordered_set
           NO  -> Need sequence?
                    Push/pop BOTH ends?   -> deque
                    LIFO only?            -> stack
                    FIFO only?            -> queue
                    Always get min/max?   -> priority_queue
                    Random access?        -> vector
```