# C++ STL Container Internals: 1-Day-Before Interview Master Sheet

> **Target Audience:** High-yield reference for C++ technical interviews (Systems, FinTech, FAANG) testing low-level STL memory layouts, underlying data structures, hardware cache locality, growth factors, and iterator invalidation rules.

---

# Table of Contents
1. [Sequence Containers (`vector`, `deque`, `list`, `array`, `string`)](#1-sequence-containers)
2. [Ordered Associative Containers (`map`, `set`, `multimap`, `multiset`)](#2-ordered-associative-containers)
3. [Unordered Associative Containers (`unordered_map`, `unordered_set`)](#3-unordered-associative-containers)
4. [Container Adapters (`stack`, `queue`, `priority_queue`)](#4-container-adapters)
5. [Master Iterator Invalidation Matrix](#5-master-iterator-invalidation-matrix)
6. [The "Specialization Traps" (`vector<bool>`, `std::string` SSO)](#6-the-specialization-traps)
7. [Top 10 C++ STL Interview Trap Questions](#7-top-10-cpp-stl-interview-trap-questions)

---

# 1. Sequence Containers

## A. `std::vector<T>`

```
std::vector Memory Layout (24 bytes in 64-bit systems):
+-------------------------+-------------------------+-------------------------+
|  T* _M_start            |  T* _M_finish           |  T* _M_end_of_storage   | (3 Pointers = 24 bytes)
+-------------------------+-------------------------+-------------------------+
         |                         |                         |
         v                         v                         v
Heap:  [ Elem 0 | Elem 1 | Elem 2 | Elem 3 |  ...  | Elem K | Empty | Empty ]
       |<-------------- size() = 4 --------------->|
       |<--------------------------- capacity() = 8 ------------------------->|
```

- **Underlying Engine:** Single contiguous dynamic heap array.
- **Memory Overhead:** 24 bytes on the stack (3 pointers: `begin`, `end`, `end_of_storage`).
- **Growth Factor:**
  - **GCC `libstdc++` & Clang `libc++`:** **$2.0\times$** capacity growth.
  - **MSVC (Microsoft C++):** **$1.5\times$** capacity growth (allows memory allocator to reuse previously deallocated contiguous blocks).
- **Time Complexities:**
  - Indexing `v[i]`: **$O(1)$** (exact pointer arithmetic: `_M_start + i`).
  - `push_back()` / `pop_back()`: **Amortized $O(1)$** (Worst-case $O(N)$ during memory reallocation and copy).
  - Insertion / Deletion in middle: **$O(N)$** (requires shifting elements with `memmove` / move constructors).
- **`reserve(N)` vs. `resize(N)`:**
  - `reserve(N)`: Allocates raw uninitialized heap capacity $\ge N$ without constructing objects (`capacity` changes, `size` remains unchanged).
  - `resize(N)`: Changes `size` to $N$. If $N > \text{size}$, default-constructs new elements; if $N < \text{size}$, destroys excess elements.
- **`shrink_to_fit()`:** Non-binding request to release unused memory (`capacity = size`), allocating a new exact-sized buffer and moving elements.
- **`emplace_back()` vs `push_back()`:**
  - `push_back()` constructs a temporary object on the stack and moves/copies it into the vector buffer.
  - `emplace_back()` passes arguments directly (`std::forward<Args>`) to construct the object **in-place directly inside the vector's heap memory**, eliminating constructor/destructor overhead.

---

## B. `std::deque<T>` (Double-Ended Queue)

```
std::deque Memory Layout (Map of Pointers to Fixed-Size Buffers):
+-------------------------------------------------------------+
|  Map Array of Pointers (T** _M_map)                         |
|  [ *Buf0 ] ---> [ 512-byte Chunks: Elem 0, Elem 1, Elem 2 ] |
|  [ *Buf1 ] ---> [ 512-byte Chunks: Elem 3, Elem 4, Elem 5 ] |
|  [ *Buf2 ] ---> [ 512-byte Chunks: Elem 6, Elem 7, Elem 8 ] |
+-------------------------------------------------------------+
```

- **Underlying Engine:** A centralized dynamic **Map Array of Pointers**, where each pointer references a **fixed-size contiguous chunk buffer** (typically 512 bytes or $N$ elements).
- **Indexing Formula ($O(1)$ via Double Indirection):**
  $$\text{Chunk Index} = \frac{\text{index}}{\text{CHUNK\_SIZE}}, \quad \text{Offset Inside Chunk} = \text{index} \pmod{\text{CHUNK\_SIZE}}$$
  $$\text{Element Address} = \text{\_M\_map}[\text{Chunk Index}][\text{Offset Inside Chunk}]$$
- **Why use `deque` over `vector`?**
  - Supports **strict $O(1)$ `push_front()` and `push_back()`** without copying all existing elements.
  - Growing a deque only allocates a new 512-byte block and adds a pointer to `_M_map`, avoiding the massive full-array reallocation copies of `vector`.
- **Trade-off vs. `vector`:** Deque elements are **NOT stored in a single contiguous memory block**; passing `&dq[0]` to C-style APIs expecting contiguous memory is **Undefined Behavior**.

---

## C. `std::list<T>` (Doubly Linked List)

```
std::list Node Layout:
+-------------------------------------------------------------+
|  Node<T>* next  (8 bytes)                                   |
|  Node<T>* prev  (8 bytes)                                   |
|  T value        (sizeof(T) + alignment padding)             |
+-------------------------------------------------------------+
```

- **Underlying Engine:** Non-contiguous circular doubly linked list with a sentinel head node.
- **Memory Overhead:** **16 bytes of pointer metadata per element** (64-bit pointers) + malloc chunk allocation header (8-16 bytes per node).
- **Key Feature:** **Strict $O(1)$ insertions and deletions anywhere** given an iterator, with **ZERO iterator invalidation** (iterators pointing to other nodes remain valid forever).
- **`list::splice()`:** Transfers elements/sublists from one list to another in **strict $O(1)$ pointer swaps** without copying or allocating memory.
- **Hardware Flaw:** Terrible CPU cache performance; node pointer hops cause L1/L2 cache misses on every single iteration.

---

## D. `std::forward_list<T>` (Singly Linked List)
- **Underlying Engine:** Singly linked list.
- **Memory Overhead:** Exactly **8 bytes per node** (`next` pointer).
- **Key Constraints:** No `size()` member function (tracking size would add 8 bytes overhead, violating the zero-overhead principle); supports only forward iteration; insertions/deletions use `insert_after()` and `erase_after()`.

---

## E. `std::array<T, N>`
- **Underlying Engine:** Thin wrapper around a fixed C-style stack array (`T _M_elems[N]`).
- **Memory Overhead:** Exactly `N * sizeof(T)` (zero pointer overhead, zero dynamic heap allocations).
- **Compile-Time Safety:** Size $N$ is part of the type signature (`std::array<int, 5>` is a completely different type from `std::array<int, 10>`). Provides `.at(i)` bounds-checked access.

---

# 2. Ordered Associative Containers

### `std::set`, `std::map`, `std::multiset`, `std::multimap`

```
Red-Black Tree Node Memory Layout:
+-------------------------------------------------------------+
|  Node* _M_parent    (8 bytes)                               |
|  Node* _M_left      (8 bytes)                               |
|  Node* _M_right     (8 bytes)                               |
|  _Rb_tree_color     (1 byte enum + 7 bytes struct padding)  |
|  std::pair<const Key, Value> _M_value_field                 |
+-------------------------------------------------------------+
Total Node Overhead: ~32 to 40 bytes per key-value entry!
```

- **Underlying Engine:** **Self-Balancing Red-Black Tree** (strictly ordered BST).
- **Guaranteed Bounds:** Search, Insertion, and Deletion are **strictly $\Theta(\log N)$ in Best, Average, and Worst cases**.
- **Comparator Requirement:** Keys must implement a **Strict Weak Ordering** predicate (`operator<`):
  1. Irreflexive: `!(a < a)`
  2. Asymmetric: `if (a < b) then !(b < a)`
  3. Transitive: `if (a < b && b < c) then (a < c)`
  4. Transitivity of Equivalence: if `!(a < b) && !(b < a)`, then $a$ and $b$ are treated as equal keys.
- **Iterator Invalidation:** Insertions and deletions **NEVER invalidate iterators, pointers, or references** to other nodes in the tree (only the exact deleted iterator is invalidated).
- **In-Order Traversal:** Iterating from `begin()` to `end()` yields keys in strictly sorted ascending order.

---

# 3. Unordered Associative Containers

### `std::unordered_map`, `std::unordered_set`, `std::unordered_multimap`, `std::unordered_multiset`

```
std::unordered_map Memory Architecture (Separate Chaining):
Bucket Array (Vector of Pointers: _M_buckets):
Index:   0         1         2         3         4
       [ * ]     [ * ]     [ * ]     [ * ]     [ * ]
         |                   |
         v                   v
       [Node 1]            [Node 2] ---> [Node 3] (Collision Singly-Linked List)
       - hash_code         - hash_code   - hash_code
       - next ptr          - next ptr    - next ptr
       - pair<K, V>        - pair<K, V>  - pair<K, V>
```

- **Underlying Engine:** **Hash Table using Separate Chaining** with singly linked bucket lists.
- **Complexity:**
  - **Average Case:** **$O(1)$** for Search, Insert, and Delete.
  - **Worst Case:** **$O(N)$** (occurs during severe hash collisions or HashDoS attacks).
- **Key Requirements:**
  1. `std::hash<Key>` functor to compute integer hash code.
  2. `operator==` equality predicate to resolve bucket collisions.
- **Load Factor & Rehashing:**
  - $\text{Load Factor } \alpha = \frac{\text{size()}}{\text{bucket\_count()}}$.
  - Default `max_load_factor()` is **1.0**.
  - When $\alpha > 1.0$, insertion triggers **Rehashing**: allocates a larger prime bucket array, recomputes bucket indices, and relinks nodes in **$O(N)$ time (Amortized $O(1)$)**.
- **Iterator Invalidation on Rehashing:**
  - **Iterators are INVALIDATED** after rehashing.
  - **Pointers and References to elements REMAIN VALID** because node memory on the heap is not moved, only bucket pointer arrays are rewired.

---

# 4. Container Adapters

Container Adapters do not implement their own data structures; they provide a restricted interface wrapping an underlying sequence container.

```
+---------------------------------------------------------------------------------------------------+
| ADAPTER               | DEFAULT UNDERLYING CONTAINER | ALTERNATIVE BACKINGS ALLOWED               |
+---------------------------------------------------------------------------------------------------+
| std::stack<T>         | std::deque<T>                | std::vector<T>, std::list<T>               |
| std::queue<T>         | std::deque<T>                | std::list<T> (vector FAILS: no pop_front!) |
| std::priority_queue<T>| std::vector<T>               | std::deque<T>                              |
+---------------------------------------------------------------------------------------------------+
```

### Deep Dive: `std::priority_queue<T, Container, Compare>`
- **Underlying Algorithms:** Wraps a contiguous container (`std::vector`) using STL heap algorithms:
  - `push()` $\implies$ `v.push_back()` + `std::push_heap()` ($O(\log N)$ Heapify-Up).
  - `pop()` $\implies$ `std::pop_heap()` (swaps root to end) + `v.pop_back()` ($O(\log N)$ Heapify-Down).
  - `top()` $\implies$ `v.front()` ($O(1)$).
- **Default Comparator Trap:**
  - `std::priority_queue<int>` uses `std::less<int>` $\implies$ **MAX-HEAP** (largest on top).
  - To create a **MIN-HEAP**, you must explicitly provide all 3 template arguments:
    `std::priority_queue<int, vector<int>, std::greater<int>> minHeap;`

---

# 5. Master Iterator Invalidation Matrix

This is the **#1 most tested topic** in systems and C++ interviews:

```
+---------------------------------------------------------------------------------------------------+
| CONTAINER          | INSERTION / PUSH                             | DELETION / ERASE              |
+---------------------------------------------------------------------------------------------------+
| std::vector        | If capacity grows: ALL iterators/refs DIE.   | Iterators/refs at or AFTER    |
|                    | If no reallocation: Iterators AFTER insert   | erase point DIE.              |
|                    | point DIE; before remain VALID.              | Elements before remain VALID. |
+---------------------------------------------------------------------------------------------------+
| std::deque         | Insert at ends (push_front/back): Iterators  | Erase at ends: Only erased    |
|                    | DIE, but REFERENCES/POINTERS REMAIN VALID!   | iterator dies.                |
|                    | Insert in middle: ALL iterators/refs DIE.    | Erase in middle: ALL DIE.     |
+---------------------------------------------------------------------------------------------------+
| std::list /        | ZERO Invalidation! ALL iterators/refs        | Only the exact erased iterator|
| std::forward_list  | remain 100% VALID.                           | DIES. All others stay VALID.  |
+---------------------------------------------------------------------------------------------------+
| std::map / set /   | ZERO Invalidation! ALL iterators/refs        | Only the exact erased iterator|
| multiset / multimap| remain 100% VALID.                           | DIES. All others stay VALID.  |
+---------------------------------------------------------------------------------------------------+
| std::unordered_map | If rehash occurs: ALL iterators DIE, but     | Only the exact erased iterator|
| / unordered_set    | REFERENCES/POINTERS REMAIN VALID!            | DIES. All others stay VALID.  |
|                    | If no rehash: Iterators in same bucket DIE.  |                               |
+---------------------------------------------------------------------------------------------------+
```

---

# 6. The "Specialization Traps"

### A. The `std::vector<bool>` Nightmare
- `std::vector<bool>` is **NOT a standard STL container**; it is a space-optimized template specialization.
- **Internal Optimization:** To save RAM, it allocates **1 single bit per boolean** (packing 8 booleans into a single byte).
- **The Deadly C++ Interview Trap:**
  - In C++, the smallest addressable unit of memory is **1 byte**. You cannot have a raw pointer or reference to a single bit (`bool* ptr = &v[0]` **will not compile!**).
  - Therefore, `operator[]` returns a temporary **Proxy Object (`std::vector<bool>::reference`)**, not a `bool&`.
  - **The Bug:** Writing `auto& bit = v[0];` fails compilation. Writing `auto bit = v[0];` captures a proxy object whose internal bit-mask pointer dangles if the vector reallocates!
  - **Production Alternative:** Use `std::vector<uint8_t>` or `std::bitset<N>`.

---

### B. `std::string` Small String Optimization (SSO)

```
std::string Struct (24-32 bytes layout in 64-bit systems):
union {
    struct { char* _M_p; size_t _M_size; size_t _M_capacity; } _M_heap; // When size > 15
    char _M_local_buf[16];                                               // When size <= 15 (SSO)
};
```

- If `string.length() <= 15` characters, characters are stored **directly on the stack inside the string object's internal buffer** (`_M_local_buf`). Zero dynamic heap allocation (`malloc`) is invoked.
- When length exceeds 15 characters, it switches to heap allocation (`_M_heap`).

---

# 7. Top 10 C++ STL Interview Trap Questions

### Q1: What happens if you look up a non-existent key using `map[key]` vs `map.find(key)`?
> **Answer:** `map[key]` performs a lookup; if the key does not exist, it **silently inserts a new key with a default-constructed value** (`Value()`), mutating the map. To perform a non-mutating read, use `map.find(key)` (returns `map.end()`) or `map.at(key)` (throws `std::out_of_range`).

### Q2: Why is modifying an element's key through a `std::set` iterator forbidden?
> **Answer:** Dereferencing a `std::set<T>::iterator` returns `const T&`. Mutating the value in-place would alter its sorted position without notifying the underlying Red-Black tree, violating the BST invariant and permanently corrupting tree search lookups.

### Q3: How do you safely erase elements while iterating through a `std::vector` in C++11 vs C++20?
> **Answer:**
> - In C++11: Must reassign iterator to the return value of `erase()`:
>   `for (auto it = v.begin(); it != v.end(); ) { if (*it == val) it = v.erase(it); else ++it; }`
> - In C++20: Use the non-member standard function `std::erase(v, val)` (Erase-Remove idiom).

### Q4: Why does `std::vector` use a geometric growth factor ($2.0\times$ or $1.5\times$) instead of arithmetic growth ($+100$)?
> **Answer:** Geometric doubling bounds the total copying cost over $N$ insertions to $\sum_{i=0}^{\log N} 2^i < 2N$, achieving **Amortized $O(1)$ insertion**. Arithmetic expansion ($+K$) requires copying $\sum_{i=1}^{N/K} i \cdot K \approx O(N^2)$ elements, degrading insertions to disastrous **$O(N)$ linear time**.

### Q5: Why is `std::string_view` dangerous if not used carefully?
> **Answer:** `std::string_view` is a non-owning 16-byte view (pointer + length). If the underlying `std::string` is modified (triggering heap reallocation) or goes out of scope (temporary destruction), the `string_view` is left holding a **dangling pointer**, causing undefined behavior on read.

### Q6: Can a custom class be used as a key in `std::unordered_map` without defining `operator<`?
> **Answer:** Yes! `std::unordered_map` does not use `operator<`. It requires two things: a specialization of `std::hash<MyClass>` (or custom hash functor) and `operator==` equality comparison.

### Q7: What is the difference between `std::map::lower_bound` and `std::map::upper_bound`?
> **Answer:**
> - `lower_bound(k)` returns an iterator to the **first element with key $\ge k$**.
> - `upper_bound(k)` returns an iterator to the **first element with key $> k$**.

### Q8: Why does `std::sort` fail on `std::list`?
> **Answer:** `std::sort` requires **Random Access Iterators** (`it + n`, `it[n]`) to perform QuickSort/Introsort pivot swaps. `std::list` provides only **Bidirectional Iterators** (`++it`, `--it`); you must use the member function `list.sort()` which implements a pointer-based MergeSort in $O(N \log N)$ time.

### Q9: What is the difference between `size()` and `capacity()` in `std::vector`?
> **Answer:** `size()` is the number of active, initialized elements currently in the vector; `capacity()` is the total allocated heap memory slots available before the next reallocation occurs.

### Q10: How does `std::unordered_map` prevent HashDoS attacks in competitive programming?
> **Answer:** Default `std::hash<int>` in GCC is the identity function (`hash(x) = x`), making it trivial for adversaries to generate numbers with identical modulo values (`M, 2M, 3M...`) to force $O(N^2)$ TLE. The fix is passing a custom hash functor incorporating a randomized high-entropy splitmix64 timestamp salt.
