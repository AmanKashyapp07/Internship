# C++ STL Containers: Time & Space Complexity Master Reference

> **Quick Reference Guide:** Complete point-by-point breakdown of time complexity, memory overhead, and mechanical reasons for every common operation across all standard C++ containers.

---

# Table of Contents
1. [Sequence Containers](#1-sequence-containers)
   - [`std::vector<T>`](#stdvectort)
   - [`std::deque<T>`](#stddequet)
   - [`std::list<T>` (Doubly Linked List)](#stdlistt-doubly-linked-list)
   - [`std::forward_list<T>` (Singly Linked List)](#stdforward_listt-singly-linked-list)
   - [`std::array<T, N>`](#stdarrayt-n)
   - [`std::string`](#stdstring)
2. [Ordered Associative Containers](#2-ordered-associative-containers)
   - [`std::set<Key>` & `std::multiset<Key>`](#stdsetkey--stdmultisetkey)
   - [`std::map<Key, Value>` & `std::multimap<Key, Value>`](#stdmapkey-value--stdmultimapkey-value)
3. [Unordered Associative Containers](#3-unordered-associative-containers)
   - [`std::unordered_set<Key>` & `std::unordered_multiset<Key>`](#stdunordered_setkey--stdunordered_multisetkey)
   - [`std::unordered_map<Key, Value>` & `std::unordered_multimap<Key, Value>`](#stdunordered_mapkey-value--stdunordered_multimapkey-value)
4. [Container Adapters](#4-container-adapters)
   - [`std::stack<T>`](#stdstackt)
   - [`std::queue<T>`](#stdqueuet)
   - [`std::priority_queue<T>` (Binary Heap)](#stdpriority_queuet-binary-heap)
5. [C++23 Flat Containers](#5-c23-flat-containers)
   - [`std::flat_map<Key, Value>` & `std::flat_set<Key>`](#stdflat_mapkey-value--stdflat_setkey)
6. [Container Selection Cheat Sheet](#6-container-selection-cheat-sheet)

---

# 1. Sequence Containers

## `std::vector<T>`
- **Underlying Structure:** Single contiguous dynamic array on the Heap.
- **Random Access (`v[i]`, `v.at(i)`):** **$O(1)$**
  - Direct pointer arithmetic: `_M_start + i * sizeof(T)`. Exact 1 memory jump.
- **Back Insertion (`push_back`, `emplace_back`):** **Amortized $O(1)$**
  - Instant $O(1)$ append if `size < capacity`.
  - Worst-case $O(N)$ when capacity is full (allocates $2\times$ memory, copies/moves all elements, destroys old buffer).
- **Back Removal (`pop_back`):** **$O(1)$**
  - Destroys last element and decrements internal pointer (`--_M_finish`).
- **Front Insertion / Removal (`push_front` / `pop_front`):** **$O(N)$**
  - Must shift all existing $N$ elements right or left in contiguous memory (`v.insert(v.begin(), val)` / `v.erase(v.begin())`).
- **Middle Insertion / Removal (`insert(pos)`, `erase(pos)`):** **$O(N)$**
  - Must shift all subsequent $(N - \text{index})$ elements in memory using move assignments.
- **Search by Value (`std::find(v.begin(), v.end(), val)`):** **$O(N)$**
  - Linear scan across contiguous memory.
- **Range Binary Search (`std::lower_bound`, `std::upper_bound`):** **$O(\log N)$**
  - Requires array to be sorted; uses random access iterator jumps.
- **Memory Overhead:**
  - Stack footprint: Exactly **24 bytes** (3 pointers: `begin`, `end`, `end_of_storage`).
  - Heap allocation: 1 contiguous chunk holding capacity elements.
- **CPU Cache Performance:** **100% Contiguous (Maximum possible spatial cache line utilization).**

---

## `std::deque<T>`
- **Underlying Structure:** Dynamic Map Array of pointers referencing fixed-size contiguous chunk buffers (typically 512 bytes).
- **Random Access (`dq[i]`, `dq.at(i)`):** **$O(1)$**
  - Double indirection math: `_M_map[i / CHUNK_SIZE][i % CHUNK_SIZE]`. 2 memory jumps.
- **Front Operations (`push_front`, `pop_front`, `front`):** **Strict $O(1)$**
  - Prepends directly to the first 512-byte buffer. If the chunk is full, allocates 1 new 512-byte chunk and prepends its pointer to the map without copying existing elements.
- **Back Operations (`push_back`, `pop_back`, `back`):** **Strict $O(1)$**
  - Appends directly to the last 512-byte buffer. Allocates 1 new chunk if full; never copies existing elements.
- **Middle Insertion / Removal (`insert(pos)`, `erase(pos)`):** **$O(N)$**
  - Shifts elements toward whichever end (front or back) is closer.
- **Search by Value (`std::find`):** **$O(N)$**
  - Traverses chunk buffers sequentially.
- **Range Binary Search (`std::lower_bound`):** **$O(\log N)$**
  - Requires sorted elements; supports random access iterator indexing.
- **Memory Overhead:**
  - Stack footprint: ~48 bytes (map pointer, start iterator, finish iterator).
  - Heap allocation: Granular 512-byte blocks allocated on demand.
- **CPU Cache Performance:** High within individual chunks, with small pointer hops between chunk boundaries.

---

## `std::list<T>` (Doubly Linked List)
- **Underlying Structure:** Non-contiguous circular doubly linked list with a sentinel head node.
- **Random Access (`operator[]`):** **Not Supported (N/A)**
  - Accessing index $i$ requires sequential pointer traversal via `std::advance(it, i)` in **$O(N)$ time**.
- **Front Operations (`push_front`, `pop_front`, `front`):** **Strict $O(1)$**
  - Links/unlinks node pointers at the sentinel head.
- **Back Operations (`push_back`, `pop_back`, `back`):** **Strict $O(1)$**
  - Links/unlinks node pointers at the sentinel tail.
- **Middle Insertion / Removal (`insert(pos)`, `erase(pos)`):** **Strict $O(1)$**
  - Rewires 4 adjacent pointers if the iterator `pos` is already known. (Finding `pos` takes $O(N)$).
- **Splicing Sublists (`list::splice`):** **Strict $O(1)$**
  - Transfers entire chains of nodes from one list into another in constant pointer swaps without memory allocation or copying.
- **Search by Value (`std::find`):** **$O(N)$**
  - Chases pointers sequentially node by node.
- **Range Search (`std::lower_bound`):** **$O(N)$**
  - Algorithm takes $O(\log N)$ comparisons but must take $O(N)$ sequential pointer steps (`++it`).
- **Sorting (`list.sort()`):** **$O(N \log N)$**
  - Member method implementing pointer-based MergeSort. Generic `std::sort` fails because list iterators lack random access.
- **Memory Overhead:**
  - Per-node overhead: **16 bytes of pointers** (`next`, `prev`) + malloc chunk header overhead per node.
- **CPU Cache Performance:** **Terrible.** Nodes are scattered across heap pages, triggering L1/L2 cache misses on every hop.

---

## `std::forward_list<T>` (Singly Linked List)
- **Underlying Structure:** Singly linked list.
- **Random Access:** **Not Supported (N/A)**
- **Front Operations (`push_front`, `pop_front`, `front`):** **Strict $O(1)$**
- **Back Operations (`push_back`, `pop_back`):** **Not Supported ($O(N)$ if traversed)**
  - Does not maintain a tail pointer to adhere to zero-overhead principles.
- **Middle Operations (`insert_after(pos)`, `erase_after(pos)`):** **Strict $O(1)$**
  - Modifies `next` pointer of the given node.
- **Size Query (`size()`):** **Not Supported**
  - Intentionally omitted; tracking size would add 8 bytes overhead.
- **Memory Overhead:** Exactly **8 bytes per node** (`next` pointer).

---

## `std::array<T, N>`
- **Underlying Structure:** Stack-allocated fixed C-array wrapper (`T _M_elems[N]`).
- **Random Access (`arr[i]`, `arr.at(i)`):** **$O(1)$**
  - Instant compile-time base offset calculation.
- **Insertions / Deletions (`push_back`, `insert`, `erase`):** **Not Supported (N/A)**
  - Size is completely fixed at compile time.
- **Search by Value (`std::find`):** **$O(N)$**
- **Range Binary Search (`std::lower_bound` on sorted array):** **$O(\log N)$**
- **Memory Overhead:** **0 bytes overhead.** Occupies strictly `N * sizeof(T)` on the Stack. Zero heap allocation.
- **CPU Cache Performance:** **100% Contiguous (Maximum cache locality).**

---

## `std::string`
- **Underlying Structure:** Contiguous char array with Small String Optimization (SSO).
- **Random Access (`s[i]`):** **$O(1)$**
- **Back Append (`push_back`, `+=`):** **Amortized $O(1)$**
- **Front / Middle Insertion (`insert`, `erase`):** **$O(N)$**
- **Substring Search (`s.find("sub")`):** **$O(N \cdot M)$ worst-case / $O(N)$ average**
- **Memory Overhead:**
  - Small strings ($\le 15$ characters): **0 Heap bytes** (stored in 16-byte stack buffer union).
  - Large strings ($> 15$ characters): 24-byte stack object + dynamic heap buffer.

---

# 2. Ordered Associative Containers

## `std::set<Key>` & `std::multiset<Key>`
- **Underlying Structure:** Self-Balancing **Red-Black Tree** (strictly sorted by `operator<`).
- **Lookup / Search (`s.find(k)`, `s.count(k)`, `s.contains(k)`):** **Strict $\Theta(\log N)$**
  - Traverses binary search tree height bounded by $2 \log_2(N + 1)$.
- **Insertion (`s.insert(k)`, `s.emplace(k)`):** **Strict $\Theta(\log N)$**
  - Finds insertion leaf in $O(\log N)$ and performs at most 2 tree rotations in $O(1)$ time.
- **Deletion (`s.erase(k)`):** **Strict $\Theta(\log N)$**
  - Locates node in $O(\log N)$ and performs at most 3 tree rotations in $O(1)$ time.
- **Deletion by Iterator (`s.erase(it)`):** **Amortized $O(1)$**
  - Deletes known node directly and rebalances locally.
- **Range Queries (`s.lower_bound(k)`, `s.upper_bound(k)`, `s.equal_range(k)`):** **Strict $O(\log N)$**
  - Fast binary search tree traversal via member methods.
- **In-Order Traversal (`begin()` to `end()`):** **$O(N)$**
  - Visits elements in strictly sorted ascending order.
- **Memory Overhead:**
  - **32 to 40 bytes per node** (`parent*`, `left*`, `right*`, `color` enum, key payload).

---

## `std::map<Key, Value>` & `std::multimap<Key, Value>`
- **Underlying Structure:** Self-Balancing **Red-Black Tree** storing `std::pair<const Key, Value>`.
- **Element Access (`m[k]`):** **Strict $\Theta(\log N)$**
  - Searches for key. **If key is not found, silently default-constructs and inserts `Value()`**.
- **Checked Element Access (`m.at(k)`):** **Strict $\Theta(\log N)$**
  - Searches for key. Throws `std::out_of_range` if key is missing (non-mutating).
- **Lookup (`m.find(k)`):** **Strict $\Theta(\log N)$**
  - Returns `m.end()` if key is missing (safe non-mutating search).
- **Insertion (`m.insert({k, v})`, `m.emplace(k, v)`):** **Strict $\Theta(\log N)$**
- **Deletion (`m.erase(k)`):** **Strict $\Theta(\log N)$**
- **Range Queries (`m.lower_bound(k)`, `m.upper_bound(k)`):** **Strict $O(\log N)$**
- **Memory Overhead:** **32 to 48 bytes per node** (pointers + color + key-value pair).

---

# 3. Unordered Associative Containers

## `std::unordered_set<Key>` & `std::unordered_multiset<Key>`
- **Underlying Structure:** **Hash Table with Separate Chaining** (Array of bucket pointers pointing to singly linked list nodes).
- **Lookup / Search (`set.find(k)`, `set.contains(k)`):**
  - **Average Case:** **$O(1)$** (Computes `hash(k) % buckets` and checks node equality).
  - **Worst Case:** **$O(N)$** (When all keys collide into the exact same bucket).
- **Insertion (`set.insert(k)`):**
  - **Average Case:** **Amortized $O(1)$**
  - **Worst Case:** **$O(N)$** (During dynamic rehashing or severe collisions).
- **Deletion (`set.erase(k)`):**
  - **Average Case:** **$O(1)$**
  - **Worst Case:** **$O(N)$**
- **Range Queries (`lower_bound` / `upper_bound`):** **Not Supported (N/A)**
  - Hash tables do not maintain ordering; range queries require full $O(N)$ table scans.
- **Rehashing (`set.rehash(M)` / `set.reserve(N)`):** **$O(N)$**
  - Allocates larger bucket array, recomputes bucket indices for all elements, and relinks nodes.
- **Memory Overhead:**
  - Bucket array: 8 bytes per bucket pointer.
  - Per-node overhead: **16 bytes** (`next*`, `cached_hash_code`, Key payload).

---

## `std::unordered_map<Key, Value>` & `std::unordered_multimap<Key, Value>`
- **Underlying Structure:** **Hash Table with Separate Chaining** storing `std::pair<const Key, Value>`.
- **Element Access (`map[k]`):**
  - **Average Case:** **$O(1)$** (Inserts default value if key is missing).
  - **Worst Case:** **$O(N)$**
- **Checked Access (`map.at(k)`):**
  - **Average Case:** **$O(1)$** (Throws `std::out_of_range` if missing).
  - **Worst Case:** **$O(N)$**
- **Lookup (`map.find(k)`):**
  - **Average Case:** **$O(1)$** (Returns `map.end()` if missing).
  - **Worst Case:** **$O(N)$**
- **Insertion (`map.insert({k, v})`):**
  - **Average Case:** **Amortized $O(1)$**
  - **Worst Case:** **$O(N)$**
- **Deletion (`map.erase(k)`):**
  - **Average Case:** **$O(1)$**
  - **Worst Case:** **$O(N)$**
- **Range Queries:** **Not Supported (N/A)**
- **Memory Overhead:** Bucket pointer array + **16 to 24 bytes per node**.

---

# 4. Container Adapters

## `std::stack<T>`
- **Default Underlying Container:** `std::deque<T>` (Customizable to `std::vector` or `std::list`).
- **Push (`st.push(val)`, `st.emplace(args)`):** **$O(1)$**
  - Invokes `deque::push_back()`.
- **Pop (`st.pop()`):** **$O(1)$**
  - Invokes `deque::pop_back()`.
- **Top (`st.top()`):** **$O(1)$**
  - Accesses `deque::back()`.
- **Random Access / Search:** **Not Supported (N/A)**

---

## `std::queue<T>`
- **Default Underlying Container:** `std::deque<T>` (Customizable to `std::list`).
  - *Note: `std::vector` CANNOT be used as a backing container because vector lacks $O(1)$ `pop_front()`.*
- **Push (`q.push(val)`):** **$O(1)$**
  - Invokes `deque::push_back()`.
- **Pop (`q.pop()`):** **$O(1)$**
  - Invokes `deque::pop_front()`.
- **Front (`q.front()`) & Back (`q.back()`):** **$O(1)$**
- **Random Access / Search:** **Not Supported (N/A)**

---

## `std::priority_queue<T>` (Binary Heap)
- **Default Underlying Container:** `std::vector<T>` backed by STL heap algorithms.
- **Top / Peek (`pq.top()`):** **$O(1)$**
  - Returns `vector.front()` (Max element by default).
- **Push (`pq.push(val)`):** **$O(\log N)$**
  - Appends to vector (`push_back` in $O(1)$) and executes `std::push_heap` (Heapify-Up across $O(\log N)$ levels).
- **Pop (`pq.pop()`):** **$O(\log N)$**
  - Executes `std::pop_heap` (swaps root to end) and calls `vector::pop_back()` (Heapify-Down across $O(\log N)$ levels).
- **Batch Heap Construction:** **$O(N)$**
  - Calling constructor with range iterators `priority_queue pq(v.begin(), v.end())` runs bottom-up `std::make_heap` in **strictly linear $O(N)$ time**.
- **Search by Arbitrary Key:** **Not Supported ($O(N)$ full scan if accessed directly)**

---

# 5. C++23 Flat Containers

## `std::flat_map<Key, Value>` & `std::flat_set<Key>`
- **Underlying Structure:** Container adapter storing keys and values in **two sorted contiguous `std::vector` arrays** instead of a node-based Red-Black tree.
- **Lookup / Search (`fm.find(k)`, `fm[k]`):** **Strict $O(\log N)$**
  - Executes binary search (`std::lower_bound`) over the contiguous sorted key vector.
- **Insertion / Deletion (`fm.insert({k, v})`, `fm.erase(k)`):** **$O(N)$**
  - Must keep the underlying vectors strictly sorted by shifting elements.
- **Why Use `flat_map` Over `std::map`?**
  - **Blazing Cache Performance:** Traverses contiguous array memory without pointer chasing. For read-heavy datasets ($N \le 10,000$), `flat_map` lookups are **up to 5x faster than `std::map`**.
  - **Zero Node Overhead:** Eliminates 32 bytes of tree pointer metadata per element.

---

# 6. Container Selection Cheat Sheet

- **Default General Choice:** Always use **`std::vector`** (contiguous memory, maximum CPU cache throughput, $O(1)$ random access).
- **Need $O(1)$ insertions/removals at BOTH Front and Back:** Use **`std::deque`**.
- **Need $O(1)$ insertions/deletions in the Middle or $O(1)$ Splicing:** Use **`std::list`**.
- **Need Fast Key-Value Lookups (Order does not matter):** Use **`std::unordered_map`** ($O(1)$ average).
- **Need Keys Strictly Sorted or Need Range Searches (`lower_bound`):** Use **`std::map`** ($O(\log N)$ guaranteed).
- **Need Priority / Top-K elements:** Use **`std::priority_queue`** ($O(\log N)$ push/pop, $O(1)$ top).
- **Small, Fixed-Size Array at Compile-Time:** Use **`std::array`** (Zero heap overhead).
