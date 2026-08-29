# Foundations, Linear Data Structures & Hashing — Interview Master Guide

**Target:** Google, Microsoft, Meta, Amazon, Apple, NVIDIA, Uber, Bloomberg, Atlassian, Adobe, Salesforce, Goldman Sachs, Rubrik, Databricks, etc.

**Priority:** Interview patterns > complexity > behavior > internals > implementation-specific details.

---

# 1. Asymptotic Complexity & Interview Mental Model

## Big-O, Big-Omega, Big-Theta

| Notation | Meaning | Interview Intuition |
|---|---|---|
| **Big-O (O)** | Asymptotic Upper Bound | "Worst-case growth ceiling: runtime will not exceed this." |
| **Big-Omega (Omega)** | Asymptotic Lower Bound | "Best-case floor: algorithm takes at least this many operations." |
| **Big-Theta (Theta)** | Asymptotic Tight Bound | "Exact bound: algorithm is both O(g(n)) and Omega(g(n))." |

```text
Operations
    ^
    |                      f(n) <= c * g(n)  [Big-O: Upper Ceiling]
    |                     /
    |                  .-'
    |                .'   f(n) (Actual Runtime)
    |              .'
    |            .'       c' * g(n)  [Big-Omega: Lower Floor]
    +-----------+-------------------------> Input Size (n)
               n_0
```

### The Asymptotic Hierarchy

```text
O(1) < O(log log n) < O(log n) < O(sqrt(n)) < O(n) < O(n log n) < O(n^2) < O(2^n) < O(n!)
```

### Amortized Analysis (Vector Doubling)
* **What is it?** An occasional expensive operation (`O(n)` reallocation) averaged over a long sequence of cheap operations (`O(1)` insertions).
* **The Aggregate Proof:** 
  - Inserting `N` elements into a dynamic array that doubles:
  - Reallocations happen at sizes `1, 2, 4, 8, ..., N`.
  - Total copy operations: `1 + 2 + 4 + ... + N = 2N - 1 < 2N`.
  - Amortized cost per `push_back`: `2N / N = O(1)`.

### Master Theorem (The 3 Interview Cases)
For divide-and-conquer recurrences: `T(n) = a * T(n/b) + O(n^c)`

```text
Compare c with log_b(a) (Work at root vs Work at leaves):

1. log_b(a) > c  --> Leaves dominate: T(n) = Theta(n^(log_b a))   [e.g. Karatsuba]
2. log_b(a) == c --> Work balanced:   T(n) = Theta(n^c * log n)    [e.g. Merge Sort: a=2, b=2, c=1 => Theta(n log n)]
3. log_b(a) < c  --> Root dominates:   T(n) = Theta(n^c)           [e.g. Binary Search: a=1, b=2, c=0 => Theta(log n)]
```

---

# 2. Arrays & Memory Layout

## Contiguous Memory & Cache Lines

```text
Array in Memory (Contiguous Addresses):
[ 0x1000 | 0x1004 | 0x1008 | 0x100c | 0x1010 | ... ]
  arr[0]   arr[1]   arr[2]   arr[3]   arr[4]
|<----------- Single 64-Byte CPU Cache Line ----------->|
```

### Must Know
* **Spatial Locality:** Accessing `arr[i]` loads the entire 64-byte cache line containing `arr[i+1], arr[i+2]...` into L1 cache for free.
* **Row-Major vs Column-Major Scanning:**
  - Row-Major (`for i: for j: A[i][j]`): Sequential memory access -> 100% cache hits.
  - Column-Major (`for j: for i: A[i][j]`): Strided memory access -> cache misses per access. Can be 10x slower on large matrices!
* **Random Access:** `O(1)` via direct pointer arithmetic: `Address(arr[i]) = Base + i * sizeof(T)`.

---

# 3. Strings & Memory Models

### Must Know
* **Immutability vs Mutability:**
  - C++ `std::string`: Mutable in-place.
  - Java / Python / JavaScript `String`: Immutable. Concatenating inside a loop (`s += c`) creates a new string each iteration -> `O(N^2)` time! Use `StringBuilder` / `list.append()`.
* **Two Pointers on Strings:** Palindrome verification, string reversal, anagram matching.
* **String Parsing:** Tokenization, integer conversion (`stoi`), parenthesis validation.

---

# 4. Linked Lists

## Singly vs Doubly vs Circular

```text
Singly:   [val | next] -> [val | next] -> NULL
Doubly:   NULL <- [prev | val | next] <-> [prev | val | next] -> NULL
Circular: [val | next] -> [val | next] -> (back to head)
```

### Complexity

| Operation | Array / Vector | Singly Linked List | Doubly Linked List |
|---|---|---|---|
| Access by Index | `O(1)` | `O(N)` | `O(N)` |
| Insert / Erase at Head | `O(N)` (shift) | `O(1)` | `O(1)` |
| Insert / Erase at Tail | Amortized `O(1)` | `O(N)` (without tail) / `O(1)` (with tail) | `O(1)` (with tail) |
| Insert / Erase at known node | `O(N)` (shift) | `O(1)` (if prev known) | `O(1)` |

### High-ROI Interview Techniques
1. **Sentinel Dummy Nodes:** Eliminates edge cases for head deletion and insertion (`ListNode dummy(0); dummy.next = head;`).
2. **Fast & Slow Pointers (Floyd's Tortoise and Hare):**
   - Find middle of linked list (`slow` moves 1, `fast` moves 2).
   - Detect cycle in `O(N)` time and `O(1)` space.
   - Cycle Start Node: Once `slow` meets `fast`, reset `slow` to `head`. Move both 1 step at a time; their meeting point is the cycle start.
3. **In-Place Reversal:** Standard 3-pointer pattern (`prev`, `curr`, `next`).
4. **LRU Cache Building Block:** Doubly linked list + Hash Map provides `O(1)` get and `O(1)` put.

---

# 5. Stacks & Monotonic Stacks

## Mental Model
* **LIFO (Last-In, First-Out):** Elements pushed and popped from the same end (`top`).
* **Underlying Container:** `std::deque` or `std::vector`.

```text
Stack Operations:
push(x) -> [ x ]
top()   -> inspect x
pop()   -> remove x
All operations are strictly O(1).
```

### Monotonic Stack Archetype (HIGHEST ROI)
* **Monotonic Increasing Stack:** Stores elements in strictly increasing order. Finds **Next Smaller Element (NSE)** and **Previous Smaller Element (PSE)**.
* **Monotonic Decreasing Stack:** Stores elements in strictly decreasing order. Finds **Next Greater Element (NGE)** and **Previous Greater Element (PGE)**.
* **Invariant:** When an incoming element violates monotonicity, pop elements from the stack. The popped element's boundary is the incoming element!
* **Interview Applications:**
  - Daily Temperatures (LC 739)
  - Next Greater Element I & II (LC 496, 503)
  - Largest Rectangle in Histogram (LC 84)
  - Sum of Subarray Minimums (LC 907 - Contribution Technique)
  - Trapping Rain Water (LC 42)

---

# 6. Queues & Monotonic Deques

## Mental Model
* **FIFO (First-In, First-Out):** Insert at `back`, remove from `front`.
* **Circular Queue (Array implementation):**
  - `front = (front + 1) % capacity`
  - `back = (back + 1) % capacity`
  - Distinguishes full vs empty via `count` variable.

### Monotonic Deque Archetype
* **Double-Ended Queue (`std::deque`):** Supports `O(1)` push/pop at both `front` and `back`.
* **Sliding Window Maximum (LC 239):**
  - Maintain indices in deque such that values are monotonically decreasing.
  - Front of deque always holds the maximum of current window in `O(1)`.
  - Pop from back if `nums[back] <= nums[i]`; pop from front if index falls out of window.
  - Overall time: `O(N)` for entire array of size `N`.

---

# 7. Hash Tables & Hashing Architecture

## Hash Table Engine

```text
Key ---> [ Hash Function hash(key) ] ---> Integer Hash Code
                                                |
                                                v
                                         Index = HashCode % BucketCount
                                                |
                                                v
Buckets Array:
[0] -> NULL
[1] -> [Key1, Val1] -> [Key2, Val2] -> NULL  (Separate Chaining)
[2] -> [Key3, Val3] -> NULL
```

### Collision Resolution Strategies

| Strategy | Mechanism | Pros | Cons |
|---|---|---|---|
| **Separate Chaining** | Each bucket is a linked list (or BST if >= 8 elements in Java 8) | Simple deletion, degrades gracefully under high load | Pointer overhead, poor cache locality |
| **Linear Probing** | If slot occupied, try `(index + 1) % M, (index + 2) % M...` | Excellent cache locality (contiguous) | **Primary Clustering** (long contiguous occupied runs) |
| **Quadratic Probing** | Try `(index + 1^2) % M, (index + 2^2) % M...` | Reduces primary clustering | **Secondary Clustering** |
| **Double Hashing** | Try `(index + i * hash2(key)) % M` | Eliminates clustering | Extra hash computation |

### Load Factor & Dynamic Rehashing
* **Load Factor (alpha):** `alpha = N / M` (where `N` = elements, `M` = bucket count).
* **Rehashing Trigger:** When `alpha > max_load_factor` (default 1.0 in C++, 0.75 in Java):
  1. Allocate new bucket array of size `2M` (or next prime).
  2. Recompute bucket indices for all `N` elements (`new_index = hash(key) % (2M)`).
  3. Rehash cost: `O(N)` occasionally, maintaining **Amortized O(1)** operations.

### Why Hash Maps Degrade to O(N) Worst Case
* If hash function generates identical hash codes for all keys (Hash Collision Attack), all elements land in a single bucket.
* Lookup / Insert degrades to linear search `O(N)`.
* C++ `std::unordered_map` is susceptible to anti-hash tests in competitive programming / adversarial inputs; fix by using custom splitmix64 hash.

---

# 8. HashMap vs TreeMap

| Feature | `std::unordered_map` / `HashMap` | `std::map` / `TreeMap` |
|---|---|---|
| **Underlying Structure** | Hash Table (Buckets + Chaining) | Self-Balancing Red-Black Tree |
| **Search / Insert / Delete** | Average `O(1)`, Worst `O(N)` | Strictly `Theta(log N)` always |
| **Ordering** | No ordering | Strictly sorted by Key |
| **Range Queries (`lower_bound`)** | No (`O(N)` scan) | Yes (`Theta(log N)`) |
| **Key Requirements** | `hash<Key>` and `operator==` | `operator<` (Strict Weak Ordering) |
| **Memory Overhead** | Bucket vector + node pointers | 32-40 bytes per node (parent, left, right, color) |

---

# 9. Classic Traps & Gotchas

1. **`map::operator[]` vs `map::at()` / `map::find()`:**
   - `map[key]` **inserts** a default value if `key` is missing! Never use `operator[]` on `const map` or just to check existence.
   - Use `map.count(key)` or `map.find(key) != map.end()`.
2. **String Concatenation in Loops:**
   - `string s = ""; for (...) s += c;` in C++ is `O(1)` amortized per append (mutable).
   - In Java / Python, `s += c` creates a new string object each iteration -> `O(N^2)` total!
3. **Modifying Keys in Ordered Set / Map:**
   - Modifying a key in-place through a pointer corrupts the Red-Black tree invariant. Always erase old key, then insert updated key.
4. **Signed vs Unsigned Index Arithmetic:**
   - `v.size() - 1` when `v.empty()` underflows to `18446744073709551615` (unsigned `size_t`). Always use `(int)v.size()`.

---

# 10. Top Interview Questions

## Tier 1 — Must Know
1. Explain how a dynamic array (vector) achieves `O(1)` amortized insertion.
2. What is the difference between `O(N)`, `Omega(N)`, and `Theta(N)`?
3. How does a Hash Table resolve collisions? Compare Separate Chaining vs Open Addressing.
4. What happens during a Hash Table rehash?
5. How does Floyd's Cycle Detection algorithm work and why does it find the cycle start?
6. When should you use a Monotonic Stack vs a Two-Pointer approach?
7. Explain the Sliding Window Maximum problem and its `O(N)` Monotonic Deque solution.
8. Compare `std::map` vs `std::unordered_map` across time, space, and ordering guarantees.

## Tier 2 — Strong Candidate
9. Why is iterating over a 2D matrix row-by-row faster than column-by-column?
10. How would you design an `O(1)` `getMin()` Stack? (Using 2 stacks or delta math).
11. How to implement a queue using two stacks? What is the amortized cost per operation?
12. Why can Hash Map lookups degrade to `O(N)`? How does Java 8 mitigate this with Red-Black trees?
13. How does Small String Optimization (SSO) work?

## Tier 3 — Advanced / Systems
14. Explain Robin Hood Hashing and Cuckoo Hashing.
15. What is cache line bouncing and false sharing in array-based multithreaded queues?
16. How does memory alignment and struct padding affect array storage in hardware?

---

# 11. Pattern Recognition Guide

| Clue in Problem Statement | Target Data Structure / Technique |
|---|---|
| Next greater / smaller element, contiguous subsegment bounds | **Monotonic Stack** |
| Maximum / minimum in a moving sliding window of size K | **Monotonic Deque** |
| Fast `O(1)` lookup, frequency counts, pair sum matching | **Hash Map / Hash Set** |
| Sorted keys, predecessor/successor lookup, dynamic range queries | **Ordered Map / Red-Black Tree** |
| Cycle detection, list midpoint, meeting point | **Fast & Slow Pointers** |
| `O(1)` Get and `O(1)` Put with capacity eviction | **Hash Map + Doubly Linked List (LRU)** |
| Balanced parentheses, expression evaluation, recursive parsing | **Stack** |

---

# 12. Interview Priority

## P0 — Absolutely Master
```text
Dynamic array growth & amortized O(1)
Hash table mechanics (chaining, probing, load factor, rehashing)
Monotonic stack (Next Greater Element, Histogram)
Fast & Slow Pointers (Floyd's cycle detection, list middle)
HashMap vs TreeMap trade-offs
Stack & Queue standard interview patterns
```

## P1 — Strongly Know
```text
Monotonic Deque (Sliding Window Maximum)
LRU Cache implementation mechanics
Two-pointer string techniques & immutability traps
Matrix row-major vs col-major cache locality
Circular buffer queue implementation
```

## P2 — Know Conceptually
```text
Master Theorem cases
Robin Hood / Cuckoo hashing
Java 8 treeification of hash buckets
```

## P3 — Don't Waste Time Memorizing
```text
Formal epsilon-delta asymptotic definitions
Complex mathematical derivations of Stirling's approximation
Hardware-specific L1/L2 cache line fetch micro-op codes
```
