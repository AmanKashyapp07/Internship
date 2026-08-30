# Asymptotic Analysis, Linear Data Structures & Hash Table Architecture

> **Scope:** Formal Asymptotic Bounding ($O, \Omega, \Theta$), Master Theorem Recurrences, Dynamic Array Geometric Doubling & Amortization, Memory Spatial Locality & Cache Line Mechanics, Linked List Pointer Manipulations & Floyd's Cycle Proof, Monotonic Stack & Deque Invariants, Hash Table Collision Resolution, Load Factors, and Associative Container Topologies.

---

# Table of Contents
1. [Formal Asymptotic Bounds & Master Theorem](#1-formal-asymptotic-bounds--master-theorem)
2. [Dynamic Arrays & Geometric Amortization Mechanics](#2-dynamic-arrays--geometric-amortization-mechanics)
3. [Memory Layout, Cache Lines & Traversal Spatial Locality](#3-memory-layout-cache-lines--traversal-spatial-locality)
4. [Linked List Structures & Pointer Cycle Mathematics](#4-linked-list-structures--pointer-cycle-mathematics)
5. [Monotonic Stack Formalisms & Boundary Queries](#5-monotonic-stack-formalisms--boundary-queries)
6. [Monotonic Double-Ended Queues (Deques)](#6-monotonic-double-ended-queues-deques)
7. [Hash Table Architecture & Collision Resolution Schemes](#7-hash-table-architecture--collision-resolution-schemes)
8. [Hash-Based vs. Tree-Based Associative Structures](#8-hash-based-vs-tree-based-associative-structures)
9. [Core Theoretical Summary Principles](#9-core-theoretical-summary-principles)

---

# 1. Formal Asymptotic Bounds & Master Theorem

### Formal Asymptotic Definitions:
1. **$O(g(n))$ (Asymptotic Upper Bound):**
   $$f(n) = O(g(n)) \iff \exists\, c > 0, n_0 > 0 \text{ such that } 0 \le f(n) \le c \cdot g(n) \quad \forall n \ge n_0$$
2. **$\Omega(g(n))$ (Asymptotic Lower Bound):**
   $$f(n) = \Omega(g(n)) \iff \exists\, c > 0, n_0 > 0 \text{ such that } 0 \le c \cdot g(n) \le f(n) \quad \forall n \ge n_0$$
3. **$\Theta(g(n))$ (Asymptotically Tight Bound):**
   $$f(n) = \Theta(g(n)) \iff f(n) = O(g(n)) \text{ and } f(n) = \Omega(g(n))$$

```
Operations
    ^
    |                      f(n) <= c * g(n)  [Big-O: Upper Ceiling]
    |                     /
    |                  .-'
    |                .'   f(n) (Actual Runtime Function)
    |              .'
    |            .'       c' * g(n)  [Big-Omega: Lower Floor]
    +-----------+-------------------------> Input Size (n)
               n_0
```

---

# 2. Dynamic Arrays & Geometric Amortization Mechanics

A dynamic array provides random access over a contiguous heap allocation that doubles its capacity upon saturation:

```
Array Allocation Progression:
Capacity 1: [ A ]
Capacity 2: [ A | B ]                          (Copy 1 element)
Capacity 4: [ A | B | C | D ]                  (Copy 2 elements)
Capacity 8: [ A | B | C | D | E | F | G | H ]  (Copy 4 elements)
```

### Amortized $O(1)$ Aggregate Proof:
For an insertion sequence of $N$ elements into an array scaling by factor $\beta = 2$:
1. Reallocations occur at capacities $1, 2, 4, 8, \dots, 2^{\lfloor \log_2 N \rfloor}$.
2. Total data copy operations across all reallocations:
   $$\text{Total Copies} = \sum_{i=0}^{\lfloor \log_2 N \rfloor} 2^i = 2^{\lfloor \log_2 N \rfloor + 1} - 1 < 2N$$
3. Amortized cost per insertion operation:
   $$\text{Amortized Cost} = \frac{N \text{ (inserts)} + 2N \text{ (copies)}}{N} = 3 = \mathbf{O(1)}$$

---

# 3. Memory Layout, Cache Lines & Traversal Spatial Locality

Modern CPU hardware loads data into L1/L2 caches in fixed-size blocks called **Cache Lines** (typically 64 bytes):

```
Contiguous DRAM Frame (Single 64-Byte Cache Line):
+-------------------------------------------------------------------+
| arr[0] (4B) | arr[1] (4B) | arr[2] (4B) | ... | arr[15] (4B)      |
+-------------------------------------------------------------------+
-> Fetching arr[0] loads arr[0..15] into L1 cache, eliminating bus stalls for subsequent 15 reads.
```

### Row-Major vs. Column-Major 2D Matrix Traversal:
- **Row-Major (`A[i][j]`):** Advances linearly through contiguous physical addresses, yielding a **cache miss rate of only $1/16 \approx 6.25\%$** for 4-byte integers.
- **Column-Major (`A[j][i]`):** Strides across row boundaries by offset $N \times \text{sizeof}(T)$, evicting active cache lines and yielding up to **100% cache miss rates** on large matrices.

---

# 4. Linked List Structures & Pointer Cycle Mathematics

```
Singly Linked:   [ Data | Next ] ---> [ Data | Next ] ---> nullptr
Doubly Linked:   nullptr <--- [ Prev | Data | Next ] <---> [ Prev | Data | Next ] ---> nullptr
```

```
+----------------------------------------------------------------------------------------------------+
| OPERATION          | DYNAMIC ARRAY / VECTOR       | SINGLY LINKED LIST        | DOUBLY LINKED LIST |
+----------------------------------------------------------------------------------------------------+
| Random Access      | O(1) direct offset math      | O(N) linear traversal     | O(N) linear traversal|
| Insertion at Head  | O(N) shift operations        | O(1) pointer adjustment   | O(1) pointer adjustment|
| Insertion at Tail  | Amortized O(1)               | O(1) with tail pointer    | O(1) with tail pointer |
| Node Deletion      | O(N) shift operations        | O(1) if predecessor known | O(1) given node ptr|
| Node Memory Header | 0 bytes overhead per element | 8 bytes pointer / node    | 16 bytes ptrs / node|
+----------------------------------------------------------------------------------------------------+
```

### Floyd's Cycle Detection & Cycle Entry Proof:
Let $L$ be the distance from the head to the cycle start, $C$ be the cycle length, and $k$ be the distance from the cycle start to the initial meeting point:

```
[ Head ] ---- L ----> [ Cycle Start ] ---- k ----> [ Meeting Point ]
                           ^                             |
                           |------------ C - k ----------|
```
1. Distance traversed by slow pointer: $D_{\text{slow}} = L + k$.
2. Distance traversed by fast pointer: $D_{\text{fast}} = 2 \cdot D_{\text{slow}} = 2(L + k)$.
3. Fast pointer traversed the loop $m$ extra times: $2(L + k) - (L + k) = m C \implies L + k = m C$.
4. Rewriting for $L$: $L = m C - k = (m - 1) C + (C - k)$.
- **Theorem:** Advancing one pointer from `Head` and another from `Meeting Point` at identical speeds guarantees they collide at `Cycle Start` after exactly $L$ steps.

---

# 5. Monotonic Stack Formalisms & Boundary Queries

A **Monotonic Stack** enforces a strictly ascending or descending order of elements, enabling $O(N)$ evaluation of boundary queries:

```
Monotonic Ascending Stack (Maintains invariant stack[i] < stack[i+1]):
Incoming element x < stack.top():
1. Pop stack elements >= x (The boundary for popped elements is x).
2. Push x onto stack.
```

### Formal Applications:
- **Next Greater Element (NGE):** Monotonically decreasing stack storing indices. An element $x > \text{stack.top()}$ resolves the NGE for all popped items.
- **Largest Rectangle in Histogram:** Tracks continuous height boundaries; popping an index $i$ evaluates maximal area bounded by left index $\text{stack.top()}$ and right index $\text{current}$.

---

# 6. Monotonic Double-Ended Queues (Deques)

A Monotonic Deque maintains sequence extrema across a sliding window of length $K$ in overall $O(N)$ time:

```
Sliding Window Maximum Algorithm:
For each element arr[i] with active window [i - K + 1, i]:
1. Evict elements from back of deque while arr[deque.back()] <= arr[i] (Monotonicity).
2. Insert index i at back of deque.
3. Evict index from front of deque if deque.front() <= i - K (Window expiration).
4. Deque front invariant: arr[deque.front()] is the maximal value in [i - K + 1, i].
```

---

# 7. Hash Table Architecture & Collision Resolution Schemes

```
Key Input ---> [ Universal Hash Function h(k) ] ---> Hash Code
                                                          |
                                                          v
                                                   Bucket Index = HashCode % M
                                                          |
                                                          v
Bucket Array:
[ 0 ] -> nullptr
[ 1 ] -> [ Key1 | Value1 ] ---> [ Key2 | Value2 ] ---> nullptr  (Separate Chaining)
[ 2 ] -> [ Key3 | Value3 ] ---> nullptr
```

```
+----------------------------------------------------------------------------------------------------+
| RESOLUTION SCHEME  | COLLISION MECHANICS                | PROS                  | CONS             |
+----------------------------------------------------------------------------------------------------+
| Separate Chaining  | Buckets hold linked lists/BSTs     | Simple deletion logic;| Pointer memory   |
|                    | of colliding elements              | degrades gracefully   | chasing latency  |
| Linear Probing     | Probe sequence: (h(k) + i) % M     | Optimal spatial cache | Primary clustering|
|                    | for i = 0, 1, 2...                 | line locality         | long block runs  |
| Quadratic Probing  | Probe sequence: (h(k) + i^2) % M   | Mitigates primary     | Secondary        |
|                    | for i = 0, 1, 2...                 | clustering            | clustering       |
| Double Hashing     | Probe sequence: (h1(k) + i*h2(k))%M| Uniform distribution; | Second hash      |
|                    | using secondary hash function      | zero clustering       | compute overhead |
+----------------------------------------------------------------------------------------------------+
```

### Load Factor & Dynamic Expansion:
$$\text{Load Factor } \alpha = \frac{N \text{ (Total Elements)}}{M \text{ (Total Buckets)}}$$
When $\alpha > \alpha_{\text{threshold}}$ (typically $1.0$ for separate chaining, $0.5\text{--}0.7$ for open addressing), the hash table allocates a new bucket buffer of size $2M$ (or next prime integer) and rehashes all elements in $O(N)$ time, maintaining amortized $O(1)$ operations.

---

# 8. Hash-Based vs. Tree-Based Associative Structures

```
+----------------------------------------------------------------------------------------------------+
| ATTRIBUTE          | HASH TABLE (`std::unordered_map`)   | BALANCED BST (`std::map` / Red-Black)   |
+----------------------------------------------------------------------------------------------------+
| Average Time       | Access: O(1), Insert: O(1)          | Access: O(log N), Insert: O(log N)      |
| Worst-Case Time    | O(N) under adversarial collisions   | Strictly bounded O(log N)               |
| Ordering Guarantee | Unordered / Non-deterministic       | Strict Weak Ordering (`operator<`)      |
| Range Scan Support | Infeasible (Requires full O(N) scan)| Optimal via iterators (`lower_bound`)   |
| Key Requirements   | `std::hash<Key>` and `operator==`   | `operator<` (Strict Weak Ordering)      |
| Node Memory Layout | Bucket array + singly linked nodes  | 32-40 bytes per node (3 ptrs + color)   |
+----------------------------------------------------------------------------------------------------+
```

---

# 9. Core Theoretical Summary Principles

1. **Amortized Geometric Array Expansion:** Doubling capacity during reallocation bounds cumulative data copy work to $< 2N$, yielding true $O(1)$ amortized insertion.
2. **Hardware Cache Line Exploitation:** Contiguous row-major storage maximizes the utilization of 64-byte hardware cache lines, avoiding CPU memory stall cycles.
3. **Floyd's Cycle Convergence Invariant:** The algebraic relationship $L = (m-1)C + (C-k)$ guarantees that pointer collision precisely marks the cycle start.
4. **Monotonic Boundary Property:** Maintaining ordered elements in a stack or deque computes boundary and window extrema in $O(N)$ total amortized time.
5. **Hash Collisions & Rehashing:** Load factor thresholds maintain $O(1)$ amortized access by dynamically re-distributing keys across scaled bucket arrays.
