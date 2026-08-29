# C++ STL — Interview Master Guide

**Target:** Google, Microsoft, Meta, Amazon, Apple, NVIDIA, Uber, Bloomberg, Atlassian, Adobe, Salesforce, Goldman Sachs, Rubrik, Databricks, etc.

**Priority:** Interview patterns > complexity > behavior > internals > implementation-specific details.

---

# 1. STL Container Mental Model

## The 4 Families

| Family                | Containers                                                   | Core idea                                   |
| --------------------- | ------------------------------------------------------------ | ------------------------------------------- |
| Sequence              | `vector`, `deque`, `list`, `forward_list`, `array`, `string` | Store elements in sequence                  |
| Ordered associative   | `set`, `map`, `multiset`, `multimap`                         | Sorted keys, `O(log n)`                     |
| Unordered associative | `unordered_set`, `unordered_map`                             | Hash table, average `O(1)`                  |
| Adapters              | `stack`, `queue`, `priority_queue`                           | Restricted interface over another container |

### Must Know

```text
vector       → dynamic contiguous array
deque        → segmented array
list         → doubly linked list
array        → fixed-size contiguous array
string       → dynamic contiguous character sequence

set          → ordered unique keys
map          → ordered unique key-value pairs
multiset     → ordered duplicate keys
multimap     → ordered duplicate key-value pairs

unordered_set → hash table of keys
unordered_map → hash table of key-value pairs

stack        → LIFO
queue        → FIFO
priority_queue → heap
```

---

# 2. `vector` — HIGHEST PRIORITY

## Internal Model

```text
vector object
+---------+---------+---------+
| begin   | end     | capacity|
+---------+---------+---------+
              |
              v
      contiguous heap array
```

### Complexity

| Operation           |       Complexity |
| ------------------- | ---------------: |
| `v[i]`              |           `O(1)` |
| `push_back`         | Amortized `O(1)` |
| `pop_back`          |           `O(1)` |
| insert front/middle |           `O(n)` |
| erase front/middle  |           `O(n)` |
| `size`              |           `O(1)` |
| `reserve`           |     Up to `O(n)` |
| `resize`            |     Up to `O(n)` |

### Interview Must-Know

* `size()` vs `capacity()`
* `reserve()` vs `resize()`
* Why `push_back()` is amortized `O(1)`
* What happens during reallocation
* Iterator/reference/pointer invalidation
* Why vector is usually faster than `list` despite `list` having `O(1)` insertion
* Contiguous memory and cache locality
* `emplace_back()` vs `push_back()`
* `shrink_to_fit()` is a non-binding request
* `data()` gives pointer to contiguous storage

### Classic Trap

```cpp
vector<int> v = {1,2,3};
int* p = &v[0];

v.push_back(4);
```

`p` may become invalid if reallocation occurs.

---

# 3. `deque`

## Internal Model

Not one contiguous array.

```text
        map
         |
   +-----+-----+-----+
   |     |     |     |
   v     v     v     v
 [block][block][block]
```

### Must Know

* Random access: `O(1)`
* `push_front`: `O(1)`
* `push_back`: `O(1)` amortized
* Middle insertion/deletion: `O(n)`
* Elements are not guaranteed to be contiguous
* Iterator invalidation rules differ significantly from `vector`

### Interview Use

Use `deque` when you need efficient insertion/removal at **both ends**.

---

# 4. `list` / `forward_list`

## `list`

```text
node <-> node <-> node
```

* Doubly linked list
* `insert/erase` at known iterator: `O(1)`
* Search: `O(n)`
* No random access
* Iterators to unaffected elements remain valid
* `splice()` is an important interview feature

## `forward_list`

```text
node -> node -> node
```

* Singly linked list
* Lower memory overhead than `list`
* No random access
* No `size()`
* Uses `insert_after`, `erase_after`

### Classic Interview Question

**Why is `vector` often faster than `list` even though list insertion is `O(1)`?**

Answer:

```text
vector → contiguous → cache friendly → fewer allocations
list   → scattered nodes → pointer chasing → poor cache locality
```

---

# 5. `array`

```cpp
array<int, 5> a;
```

Think:

```text
int a[5]
```

but with STL interface.

### Must Know

* Fixed size
* Contiguous
* `O(1)` random access
* No dynamic allocation caused by the container itself
* `size()` known at compile time
* Prefer `std::array` over raw arrays when an STL-compatible fixed-size container is needed

---

# 6. `string`

### Must Know

* Contiguous character storage
* `size()` / `length()` → `O(1)`
* `operator[]` → `O(1)`
* `substr()` → linear in substring length
* `find()` → generally linear-ish, but exact complexity depends on implementation/algorithm
* `c_str()` gives null-terminated C-style string
* SSO may avoid heap allocation for short strings

### Do NOT Memorize

Exact SSO capacity such as:

```text
15 chars
22 chars
```

This is implementation-dependent and is not a useful universal interview fact.

---

# 7. `set` / `map` / `multiset` / `multimap`

## Internal Model

Typically a balanced BST, commonly a **Red-Black Tree** in major implementations.

```text
             root
            /    \
         left    right
         /          \
       ...          ...
```

### Complexity

| Operation     | Complexity |
| ------------- | ---------: |
| `find`        | `O(log n)` |
| `insert`      | `O(log n)` |
| `erase(key)`  | `O(log n)` |
| `lower_bound` | `O(log n)` |
| `upper_bound` | `O(log n)` |

### Differences

```text
set       → unique keys
multiset  → duplicate keys

map       → unique keys + values
multimap  → duplicate keys + values
```

### HIGH-ROI Topics

* Why `set` cannot modify its key through an iterator
* `lower_bound`
* `upper_bound`
* `equal_range`
* Custom comparator
* Strict weak ordering
* `map::operator[]`
* `map::at()`
* `find()` vs `count()`
* Iterator invalidation
* Why ordered containers are useful when you need sorted/range queries

---

# 8. `unordered_set` / `unordered_map`

## Internal Model

Hash table.

Conceptually:

```text
hash(key)
    |
    v
bucket index
    |
    v
bucket → node → node → node
```

### Complexity

| Operation | Average |  Worst |
| --------- | ------: | -----: |
| `find`    |  `O(1)` | `O(n)` |
| `insert`  |  `O(1)` | `O(n)` |
| `erase`   |  `O(1)` | `O(n)` |

### Must Know

* Hash function
* Bucket
* Collision
* Load factor
* `max_load_factor`
* Rehash
* Custom hash
* `operator==`
* Iterator invalidation during rehash
* Why pointers/references to elements have different invalidation behavior from iterators

### Critical Rule

```text
unordered_map → key → value
unordered_set → key only
```

### Custom Hash Pattern

```cpp
struct Hash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.x) ^
               (hash<int>()(p.y) << 1);
    }
};
```

---

# 9. `stack`

Default underlying container is typically `deque`.

```cpp
stack<int> st;

st.push(10);
st.top();
st.pop();
```

### Complexity

```text
push → O(1)
pop  → O(1)
top  → O(1)
```

### Interview Applications

* Parentheses
* Monotonic stack
* Next greater element
* DFS
* Expression evaluation
* Histogram problems

---

# 10. `queue`

Typically backed by `deque`.

```text
front → [1][2][3][4] ← back
```

### Complexity

```text
push  → O(1)
pop   → O(1)
front → O(1)
back  → O(1)
```

### Interview Applications

* BFS
* Level-order traversal
* Sliding window variants
* Producer/consumer concepts

---

# 11. `priority_queue`

## Internal Model

Binary heap stored in a contiguous array, typically `vector`.

```text
             50
           /    \
         30      40
        /  \
      10    20
```

### Complexity

```text
top  → O(1)
push → O(log n)
pop  → O(log n)
```

### Max Heap

```cpp
priority_queue<int> pq;
```

### Min Heap

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

### Interview Applications

* Top K
* Kth largest/smallest
* Dijkstra
* Merge K sorted lists
* Scheduling
* Median / two heaps

---

# 12. `pair` / `tuple`

### `pair`

```cpp
pair<int,string> p;
```

Used heavily with:

```cpp
vector<pair<int,int>>
map<int,pair<int,int>>
priority_queue<pair<int,int>>
```

### `tuple`

Useful when carrying multiple values:

```cpp
tuple<int,int,int> t;
```

Know:

```cpp
get<0>(t)
get<1>(t)
get<2>(t)
```

---

# 13. Iterators — HIGH PRIORITY

Understand:

```text
iterator
const_iterator
reverse_iterator
```

And:

```cpp
begin()
end()
cbegin()
cend()
rbegin()
rend()
```

### Iterator Categories

```text
Input
Output
Forward
Bidirectional
Random Access
Contiguous
```

### Important Mapping

```text
vector       → Random Access + Contiguous
deque        → Random Access
list         → Bidirectional
forward_list → Forward
set/map      → Bidirectional
unordered_*  → Forward
```

This is much more interview-relevant than memorizing implementation fields.

---

# 14. Iterator Invalidation — MUST KNOW

## Vector

```text
reallocation → all iterators/references/pointers invalidated
erase        → iterators/references at and after erase point affected
```

## List

```text
insert → existing iterators remain valid
erase  → only erased element invalidated
```

## Set / Map

```text
insert → existing iterators remain valid
erase  → only erased element invalidated
```

## Unordered Containers

```text
rehash → iterators invalidated
        references/pointers to elements remain valid
```

Memorize the **behavior**, not implementation variable names.

---

# 15. Custom Comparators

## Sorting

```cpp
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;
});
```

## Set

```cpp
set<int, greater<int>> s;
```

## Priority Queue

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

### Critical Concept

Comparator answers:

```text
"Should a come before b?"
```

It must satisfy **strict weak ordering**.

---

# 16. Custom Hashing

Know how to hash:

```cpp
pair<int,int>
tuple
string
custom struct
```

### Interview Trap

For unordered containers:

```text
hash equality must be compatible with ==
```

If:

```text
a == b
```

then:

```text
hash(a) == hash(b)
```

must hold.

The reverse is NOT required:

```text
hash(a) == hash(b)
```

does not mean:

```text
a == b
```

---

# 17. C++20/23 Containers — LOW/MEDIUM PRIORITY

Know conceptually:

```text
std::span
std::ranges
std::flat_map
std::flat_set
```

## `flat_map` / `flat_set`

Think:

```text
sorted contiguous storage
        +
binary search
```

Advantages:

* Excellent cache locality
* `O(log n)` lookup
* Good for read-heavy workloads

Disadvantage:

* Insertion/deletion can be `O(n)`

Do NOT prioritize these over:

```text
vector
unordered_map
map
set
priority_queue
deque
```

---

# 18. Container Selection — VERY HIGH ROI

When interviewer asks:

**"Which container would you use?"**

Think in this order:

### Need random access?

```text
vector / array
```

### Need insertion/removal at both ends?

```text
deque
```

### Need sorted unique elements?

```text
set
```

### Need sorted key-value pairs?

```text
map
```

### Need duplicates + sorted order?

```text
multiset / multimap
```

### Need fast average lookup?

```text
unordered_set / unordered_map
```

### Need LIFO?

```text
stack
```

### Need FIFO?

```text
queue
```

### Need repeatedly extract min/max?

```text
priority_queue
```

### Need `O(1)` insertion/erase at known position and don't care about locality?

```text
list
```

---

# 19. Master Complexity Table

| Container        |     Access |     Search |                Insert |            Erase | Main Advantage     |
| ---------------- | ---------: | ---------: | --------------------: | ---------------: | ------------------ |
| `array`          |     `O(1)` |     `O(n)` |                   N/A |              N/A | Fixed + contiguous |
| `vector`         |     `O(1)` |     `O(n)` | Amortized `O(1)` back |           `O(n)` | General purpose    |
| `deque`          |     `O(1)` |     `O(n)` |           `O(1)` ends |      `O(1)` ends | Both ends          |
| `list`           |     `O(n)` |     `O(n)` |      `O(1)` known pos | `O(1)` known pos | Stable nodes       |
| `set`            |        N/A | `O(log n)` |            `O(log n)` |       `O(log n)` | Sorted unique      |
| `map`            |        N/A | `O(log n)` |            `O(log n)` |       `O(log n)` | Sorted key-value   |
| `unordered_set`  |        N/A | Avg `O(1)` |            Avg `O(1)` |       Avg `O(1)` | Fast membership    |
| `unordered_map`  |        N/A | Avg `O(1)` |            Avg `O(1)` |       Avg `O(1)` | Fast key-value     |
| `priority_queue` | Top `O(1)` |        N/A |            `O(log n)` |       `O(log n)` | Repeated min/max   |

---

# 20. Top STL Interview Questions

## Tier 1 — MUST KNOW

1. `vector` vs `list`
2. `vector` vs `deque`
3. `map` vs `unordered_map`
4. `set` vs `unordered_set`
5. `map` vs `multimap`
6. `set` vs `multiset`
7. How does `unordered_map` work internally?
8. How are hash collisions handled?
9. What is load factor?
10. What is rehashing?
11. Why is `unordered_map` average `O(1)`?
12. Why can `unordered_map` become `O(n)`?
13. How does `map` work internally?
14. Why is `map` `O(log n)`?
15. What is a Red-Black Tree?
16. `reserve()` vs `resize()`
17. `push_back()` vs `emplace_back()`
18. Iterator invalidation rules
19. `map::operator[]` vs `map::at()`
20. `lower_bound()` vs `upper_bound()`
21. How does `priority_queue` work?
22. How to implement a min heap using `priority_queue`?
23. Custom comparator
24. Custom hash
25. Why must hash and equality agree?

---

# 21. Tier 2 — Strong Candidate

26. Why is vector cache-friendly?
27. Why can `list` be slower despite `O(1)` insertion?
28. Why is `set` iterator effectively read-only?
29. What happens to vector references after reallocation?
30. What happens during unordered-map rehash?
31. `iterator` vs `const_iterator`
32. Iterator categories
33. `begin()` vs `cbegin()`
34. `erase()` return value
35. `emplace()` vs `insert()`
36. `extract()` and node handles
37. `merge()` for associative containers
38. `equal_range()`
39. Transparent comparators / heterogeneous lookup
40. `string::c_str()` vs `data()`
41. SSO
42. `vector<bool>` specialization
43. `std::array` vs C array
44. `std::span`
45. `flat_map` vs `map`

---

# 22. Tier 3 — Advanced / HFT / C++ Heavy Interviews

46. Allocators
47. `pmr::vector`
48. `pmr::unordered_map`
49. Node handles
50. Heterogeneous lookup
51. Transparent hashing
52. Exception guarantees of container operations
53. Move semantics and container reallocation
54. `noexcept` move constructor and vector
55. Allocator propagation
56. Memory fragmentation
57. Cache-line locality
58. False sharing
59. Branch prediction and tree traversal
60. Why contiguous containers often outperform pointer-based structures
61. Flat containers and cache locality
62. Iterator implementation
63. Sentinel nodes
64. Hash-table bucket architecture
65. Amortized analysis of vector growth

---

# 23. STL + DSA Pattern Recognition

This is one of the most important sections for coding interviews.

| Problem Pattern        | Typical STL                     |
| ---------------------- | ------------------------------- |
| Frequency counting     | `unordered_map`                 |
| Membership check       | `unordered_set`                 |
| Deduplication          | `unordered_set`                 |
| Sorted unique data     | `set`                           |
| Range queries          | `set` / `map`                   |
| Top K                  | `priority_queue`                |
| K-way merge            | `priority_queue`                |
| BFS                    | `queue`                         |
| DFS iterative          | `stack`                         |
| Monotonic stack        | `stack` / `vector`              |
| Sliding window         | `deque` / `unordered_map`       |
| Coordinate compression | `vector` + `sort`               |
| Interval processing    | `vector` + `sort`               |
| Custom ordering        | comparator                      |
| Graph adjacency list   | `vector<vector<int>>`           |
| Weighted graph         | `vector<vector<pair<int,int>>>` |
| Dijkstra               | `priority_queue`                |
| DSU                    | `vector`                        |
| Trie                   | custom nodes / arrays           |
| LRU cache              | `list` + `unordered_map`        |

---

# 24. Interview Priority

## P0 — Absolutely Master

```text
vector
unordered_map
unordered_set
map
set
priority_queue
queue
stack

complexities
iterator invalidation
comparators
hashing
reserve/resize
lower_bound/upper_bound
```

## P1 — Strongly Know

```text
deque
string
array
list
multiset
multimap
iterator categories
custom hash
emplace vs insert
```

## P2 — Know Conceptually

```text
forward_list
flat_map
flat_set
span
ranges
node handles
pmr
allocator internals
```

## P3 — Don't Waste Interview Prep Time Memorizing

```text
_GCC private member names
exact node byte sizes
exact deque chunk sizes
exact SSO capacity
exact vector growth factor
implementation-specific memory layouts
```

These are implementation details, not portable guarantees.

---

# Final Mental Model

```text
                    STL
                     |
       +-------------+-------------+
       |             |             |
    Sequence      Associative    Adapters
       |             |
   vector          Ordered
   deque           /     \
   list           map    set
   array           |
   string       Unordered
                   /    \
                map     set

Adapters:
stack
queue
priority_queue
```

The key interview skill is not memorizing every STL container.

It is being able to answer:

> **"Given these operations and constraints, which container should I choose and why?"**
