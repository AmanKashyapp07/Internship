# Master Guide 04: C++ STL Containers & Complexity Cheat Sheet

> **Focus:** C++ Standard Template Library (STL) Container Internals, Memory Layouts, Iterator Invalidation Rules, Hardware Cache Locality, Custom Comparators, Complexities, and C++23 Flat Containers.
> 
> *Designed to crush top-tier FAANG/HFT technical interviews without missing a single bit of detail.*

---

# Table of Contents
1. [Sequence Containers (`vector`, `deque`, `list`, `forward_list`, `array`, `string`)](#1-sequence-containers)
2. [Ordered Associative Containers (`map`, `set`, `multimap`, `multiset`)](#2-ordered-associative-containers)
3. [Unordered Associative Containers (`unordered_map`, `unordered_set`)](#3-unordered-associative-containers)
4. [Container Adapters (`stack`, `queue`, `priority_queue`)](#4-container-adapters)
5. [C++23 Flat Containers (`flat_map`, `flat_set`)](#5-c23-flat-containers)
6. [Master Complexity & Operations Reference Table](#6-master-complexity--operations-reference-table)
7. [Master Iterator Invalidation Matrix](#7-master-iterator-invalidation-matrix)
8. [The "Specialization Traps" (`vector<bool>`, `std::string` SSO)](#8-the-specialization-traps)
9. [Top 10 C++ STL Interview Trap Questions](#9-top-10-cpp-stl-interview-trap-questions)

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
- **Memory Footprint:** Exactly 24 bytes on stack (3 pointers: `_M_start`, `_M_finish`, `_M_end_of_storage`).
- **Capacity Growth Factor:**
  - **GCC `libstdc++` & Clang `libc++`:** **$2.0\times$** capacity growth.
  - **MSVC (Microsoft C++):** **$1.5\times$** capacity growth (allows allocator to reuse deallocated memory blocks).
- **Time Complexities:**
  - Random Access (`v[i]`, `v.at(i)`): **$O(1)$** (exact pointer arithmetic: `_M_start + i`).
  - `push_back()` / `pop_back()`: **Amortized $O(1)$** (Worst-case $O(N)$ during reallocation).
  - Insert / Delete in middle: **$O(N)$** (shifts subsequent elements with `memmove`/move assignment).
- **`reserve(N)` vs `resize(N)`:**
  - `reserve(N)`: Allocates uninitialized heap capacity $\ge N$. `capacity()` changes, `size()` stays unchanged.
  - `resize(N)`: Changes `size()` to $N$. If $N > \text{size()}$, constructs default elements; if $N < \text{size()}$, destroys excess elements.
- **`emplace_back()` vs `push_back()`:**
  - `push_back()` takes an existing object or constructs a temporary on stack and moves/copies it into vector memory.
  - `emplace_back()` forwards arguments (`std::forward<Args>`) to construct object **in-place directly inside vector heap memory**, bypassing copy/move constructors.

---

## B. `std::deque<T>` (Double-Ended Queue)

```
std::deque Memory Layout (Map Array of Pointers to Fixed-Size Buffers):
+-------------------------------------------------------------+
|  Map Array of Pointers (T** _M_map)                         |
|  [ *Buf0 ] ---> [ 512-byte Chunk: Elem 0, Elem 1, Elem 2 ]  |
|  [ *Buf1 ] ---> [ 512-byte Chunk: Elem 3, Elem 4, Elem 5 ]  |
|  [ *Buf2 ] ---> [ 512-byte Chunk: Elem 6, Elem 7, Elem 8 ]  |
+-------------------------------------------------------------+
```

- **Underlying Engine:** Centralized dynamic **Map Array of Pointers** to fixed-size chunk buffers (typically 512 bytes).
- **Double Indirection Indexing ($O(1)$):**
  $$\text{Chunk Index} = \frac{\text{index}}{\text{CHUNK\_SIZE}}, \quad \text{Offset} = \text{index} \pmod{\text{CHUNK\_SIZE}}$$
  $$\text{Address} = \text{\_M\_map}[\text{Chunk Index}][\text{Offset}]$$
- **Key Advantage:** Supports **strict $O(1)$ `push_front()` and `push_back()`** without copying existing elements.
- **Caveat:** Deque elements are **NOT contiguous overall**; passing `&dq[0]` to C-style array APIs is **Undefined Behavior**.

---

## C. `std::list<T>` (Doubly Linked List)
- **Underlying Engine:** Non-contiguous circular doubly linked list with a sentinel node.
- **Memory Overhead:** **16 bytes pointer overhead per node** (`prev`, `next`) + malloc chunk header per node.
- **Key Feature:** **Strict $O(1)$ insert/delete at known iterator**, with **ZERO iterator invalidation** of other nodes.
- **`list::splice()`:** Transfers nodes/sublists between lists in **strict $O(1)$ pointer swaps** without allocation.

---

## D. `std::forward_list<T>` (Singly Linked List)
- **Underlying Engine:** Singly linked list.
- **Memory Overhead:** Exactly **8 bytes per node** (`next` pointer).
- **Zero-Overhead Principle:** Has NO `size()` member method (tracking size would add 8 bytes stack overhead).

---

## E. `std::array<T, N>`
- **Underlying Engine:** Thin wrapper over fixed stack array (`T _M_elems[N]`).
- **Memory Overhead:** **0 bytes overhead.** Occupies strictly `N * sizeof(T)` on Stack. Zero heap allocations.

---

## F. `std::string`
- **Underlying Engine:** Contiguous char array with Small String Optimization (SSO).
- **Small String Optimization (SSO):** Strings $\le 15$ chars (22 in libc++) allocate **0 heap memory**, storing characters inside stack object buffer union.

---

# 2. Ordered Associative Containers

### `std::set`, `std::map`, `std::multiset`, `std::multimap`

```
Red-Black Tree Node Memory Overhead:
- Node* _M_parent  (8 bytes)
- Node* _M_left    (8 bytes)
- Node* _M_right   (8 bytes)
- _Rb_tree_color   (1 byte enum + 7 bytes alignment padding)
- Value Payload    (Key / Value pair)
Total Overhead: ~32-40 bytes PER ENTRY!
```

- **Engine:** **Self-Balancing Red-Black Tree** (strictly ordered BST).
- **Time Complexity:** Search, Insert, Delete are **strictly $\Theta(\log N)$ in Best, Average, and Worst cases**.
- **Comparator Requirement:** Keys must implement **Strict Weak Ordering** (`operator<`):
  1. Irreflexive: `!(a < a)`
  2. Asymmetric: `if (a < b) then !(b < a)`
  3. Transitive: `if (a < b && b < c) then (a < c)`
  4. Equivalence: `!(a < b) && !(b < a)` treats $a$ and $b$ as equal keys.
- **Iterators:** `lower_bound(k)` (first element $\ge k$), `upper_bound(k)` (first element $> k$).

---

# 3. Unordered Associative Containers

### `std::unordered_map`, `std::unordered_set`

```
Unordered Container Bucket Architecture (Separate Chaining):
Buckets Vector (_M_buckets):
[0] -> NULL
[1] -> [Node: "apple", 5] -> [Node: "cat", 2] -> NULL  (Collision List)
[2] -> [Node: "banana", 8] -> NULL
```

- **Engine:** Hash Table with Separate Chaining bucket linked lists.
- **Time Complexity:** Insert, Search, Delete are **Average $O(1)$**, **Worst-Case $O(N)$** (when all keys collide into 1 bucket).
- **Rehash Trigger:** Load factor $\alpha = N / M > 1.0$ (Default `max_load_factor` is 1.0).
- **Custom Key Requirements:** Must provide custom **Hash Functor** and **Equality Operator (`operator==`)**.

```cpp
// Custom Struct Key in std::unordered_map
struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
    }
};

unordered_map<Point, string, PointHash> pointMap;
```

---

# 4. Container Adapters

Adapters wrap underlying sequence containers to enforce restricted interfaces:

1. **`std::stack<T>`:** LIFO wrapper over `std::deque<T>` (or `std::vector`). Supports `push`, `pop`, `top`.
2. **`std::queue<T>`:** FIFO wrapper over `std::deque<T>`. Supports `push`, `pop`, `front`, `back`.
3. **`std::priority_queue<T>`:** Max-Heap wrapper over `std::vector<T>`.
   - Operations: `push()` $O(\log N)$, `pop()` $O(\log N)$, `top()` $O(1)$.
   - Min-Heap declaration: `priority_queue<int, vector<int>, greater<int>> minPq;`

---

# 5. C++23 Flat Containers

C++23 introduces `std::flat_map` and `std::flat_set` inside `<flat_map>` / `<flat_set>`:
- **Underlying Engine:** Two sorted contiguous vectors (`vector<Key>` and `vector<Value>`).
- **Lookup:** **$O(\log N)$ via Binary Search** with **100% CPU Cache Line locality**!
- **Insertion / Deletion:** **$O(N)$** due to contiguous vector shifting.
- **Use Case:** Read-heavy workloads where keys are populated once and searched millions of times (vastly outperforms `std::map` due to cache locality).

---

# 6. Master Complexity & Operations Reference Table

| Container | Random Access | Front Insert/Delete | Back Insert/Delete | Mid Insert/Delete | Search by Key | Range Search (`lower_bound`) | Cache Locality |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **`vector`** | $O(1)$ | $O(N)$ | Amortized $O(1)$ | $O(N)$ | $O(N)$ | $O(\log N)$ (if sorted) | **Maximum** (100% Contiguous) |
| **`deque`** | $O(1)$ | Strict $O(1)$ | Strict $O(1)$ | $O(N)$ | $O(N)$ | $O(\log N)$ (if sorted) | High (within 512B chunks) |
| **`list`** | N/A | Strict $O(1)$ | Strict $O(1)$ | Strict $O(1)^*$ | $O(N)$ | $O(N)$ | **Terrible** (Heap pointer hops) |
| **`forward_list`**| N/A | Strict $O(1)$ | N/A | Strict $O(1)^*$ | $O(N)$ | $O(N)$ | **Terrible** |
| **`array`** | $O(1)$ | N/A | N/A | N/A | $O(N)$ | $O(\log N)$ (if sorted) | **Maximum** (Stack contiguous) |
| **`set` / `map`** | N/A | N/A | N/A | $O(\log N)$ | $\Theta(\log N)$ | $\Theta(\log N)$ | Low (Tree node pointer hops) |
| **`unordered_map`**| N/A | N/A | N/A | Avg $O(1)$ | Avg $O(1)$ / Worst $O(N)$ | N/A | Medium (Vector buckets + linked nodes) |
| **`flat_map` (C++23)**| $O(1)$ | $O(N)$ | $O(N)$ | $O(N)$ | $O(\log N)$ | $O(\log N)$ | **Maximum** (Contiguous vectors) |

*$O(1)$ mid insert/delete in list assumes iterator to target position is already known.*

---

# 7. Master Iterator Invalidation Matrix

| Container | Operation | Invalidated Iterators | Invalidated References / Pointers |
| :--- | :--- | :--- | :--- |
| **`vector`** | `push_back` (No Realloc) | `end()` only | **None** |
| **`vector`** | `push_back` (Realloc Triggered) | **ALL iterators** | **ALL references/pointers** |
| **`vector`** | `insert(pos)` / `erase(pos)` | At and after `pos` | At and after `pos` |
| **`deque`** | `push_front` / `push_back` | `begin()` and `end()` | **None!** (Pointers to elements remain valid!) |
| **`deque`** | `insert(pos)` / `erase(pos)` | **ALL iterators** | **ALL references/pointers** |
| **`list`** | `insert` / `erase(pos)` | Only erased `pos` iterator | Only erased element reference |
| **`set` / `map`**| `insert` / `erase(pos)` | Only erased `pos` iterator | Only erased element reference |
| **`unordered_map`**| `insert` (No Rehash) | `end()` only | **None** |
| **`unordered_map`**| `insert` (Rehash Triggered)| **ALL iterators** | **None!** (Element node addresses do NOT change!) |

---

# 8. The "Specialization Traps"

### A. `std::vector<bool>`: The Proxy Object Nightmare
- `vector<bool>` is **NOT a standard C++ container**! It is a space-optimized template specialization that packs booleans as **individual bits** (8 booleans per byte).
- **Why it breaks code:** Individual bits cannot be directly addressed in C++ (smallest addressable memory unit is 1 byte).
- Calling `vector<bool>::operator[]` returns a **temporary proxy object (`std::vector<bool>::reference`)**, NOT a true `bool&`.
- `auto& b = v[0];` **FAILS to compile**!
- **Fix:** Use `std::vector<char>` or `std::deque<bool>` if true `bool&` references are required.

---

# 9. Top 10 C++ STL Interview Trap Questions

1. **"Why does `std::vector::reserve()` invalidate `end()` iterators?"**
   - *Answer:* Because `reserve()` allocates a new memory block; even before elements are moved, `end()` points to the old buffer boundary.
2. **"What is the difference between `map::operator[]` and `map::at()`?"**
   - *Answer:* `operator[]` inserts a default-constructed Value if Key is missing. `at()` throws `std::out_of_range` exception without mutating the map (safe for `const map`).
3. **"Why does `unordered_map` rehash invalidate iterators but NOT pointers to elements?"**
   - *Answer:* Rehash reallocates the bucket pointer vector array (`_M_buckets`), invalidating iterator traversal links. However, actual key-value nodes remain at their exact heap memory addresses.
4. **"Why is `list::size()` $O(1)$ in C++11 onwards?"**
   - *Answer:* C++11 mandated $O(1)$ `list::size()`, requiring `list` to store an internal `size` counter updated on insertions/deletions.
5. **"Can `std::set` elements be modified via iterators?"**
   - *Answer:* No! `set::iterator` is always `const_iterator`. Modifying a set key in-place would violate the Red-Black Tree invariant. To update, erase old key and insert new key.
