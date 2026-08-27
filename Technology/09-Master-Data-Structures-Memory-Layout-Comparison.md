# Types of Data Structures — Interview Notes

A guide to speak about memory layout physics, CPU cache locality, pointer traversal,
and algorithmic trade-offs with the confidence of an engineer who understands how
data structures interact with physical computer hardware, not just memorized Big-O cheat sheets.

---

## What They Are & Why They Are Needed

### 1. What is a Data Structure?
A data structure is a specialized format for organizing, storing, and manipulating
data in memory. From a physical hardware perspective, a data structure dictates **how
bytes are laid out across CPU cache lines, stack frames, and dynamic heap pages**.

```
+---------------------------------------------------------------------------------------------------+
| PHYSICAL MEMORY MODEL           | DATA STRUCTURE MAPPING                                          |
+---------------------------------------------------------------------------------------------------+
| Contiguous RAM Blocks           | Arrays, Vectors, Ring Buffers, Matrix Grids                     |
| (High L1/L2 Cache Hits)         | ──► [ Item 0 ][ Item 1 ][ Item 2 ][ Item 3 ]                    |
+---------------------------------------------------------------------------------------------------+
| Dispersed Heap Nodes + Pointers | Linked Lists, Binary Search Trees, Tries, Graphs                |
| (High Cache Miss / Pointer Hop) | ──► [ Data | Ptr ] ────► [ Data | Ptr ] ────► [ Data | Ptr ]    |
+---------------------------------------------------------------------------------------------------+
```

### 2. Why Are They Needed?
1. **The CPU Memory Wall & Cache Locality:** Fetching data from L1 CPU cache takes ~1ns (4 cycles), while fetching from main RAM takes ~100ns (200 cycles). The choice of data structure directly dictates whether CPU hardware prefetchers run at maximum speed or stall on cache misses.
2. **Algorithmic Time-Space Trade-offs:** No single structure optimizes all operations. If you need $O(1)$ random index access, you choose an Array; if you need $O(1)$ prefix auto-complete, you choose a Trie; if you need $O(\log N)$ min/max extraction, you choose a Heap.
3. **Systems & Database Foundations:** Every database index (B+ Tree, SkipList, LSM-Tree), OS task scheduler (Priority Heap), and routing table (Trie/Radix Tree) is an applied composite data structure.

---

## 1. Arrays & Dynamic Arrays (Contiguous Memory & Cache Locality)

**What it is:** A collection of homogeneous elements stored in a single, contiguous
block of memory, where element addresses are computed via base pointer arithmetic:
$$\text{Address}(i) = \text{Base} + i \times \text{sizeof}(\text{Element})$$

**Strengths**
- Instant $O(1)$ random access by index via direct hardware address calculation.
- **Maximum CPU Cache Locality:** Because elements sit in adjacent memory addresses,
  the CPU hardware prefetcher loads entire 64-byte **Cache Lines** (L1/L2/L3) in advance,
  making linear scans orders of magnitude faster than pointer-based collections.
- Compact memory layout: zero pointer overhead per element.

**Weaknesses**
- Expensive middle insertion/deletion ($O(N)$) because subsequent elements must be
  shifted in memory via `memmove`.
- Dynamic arrays (like `std::vector` or `ArrayList`) resize geometrically ($1.5\times$ or $2\times$),
  which requires allocating a new memory block and copying all elements ($O(N)$ spike).

**When to use:** The universal default for sequential data, linear scans, binary searches,
and high-performance numerical computing.

**Interview-ready line:** "An array is always my default choice because contiguous memory
maximizes CPU L1/L2 cache line prefetching, outperforming theoretically faster pointer-based structures in real hardware benchmarks."

---

## 2. Linked Lists (Singly, Doubly & Circular)

**What it is:** A sequence of independent nodes allocated dispersed across the heap,
where each node contains data and one or more pointers to neighboring nodes.

**Strengths**
- Instant $O(1)$ insertion and deletion at any known position (given a pointer to that node)
  by simply rewriting pointer addresses without shifting elements.
- Dynamic size: grows and shrinks node by node without requiring large contiguous memory allocations.
- Ideal for implementing fixed-capacity queues, ring buffers, and **LRU Eviction Caches** (paired with a Hash Map).

**Weaknesses**
- No random access: accessing the $k$-th element requires an $O(N)$ sequential pointer traversal.
- **Catastrophic CPU Cache Misses:** Dispersed heap nodes cause pointer chasing that stalls
  the CPU instruction pipeline on main RAM access.
- Memory overhead: 8 bytes (Singly) or 16 bytes (Doubly) of pointer metadata per node.

**When to use:** Implementing queues/stacks, LRU cache eviction lists, and lock-free concurrency primitives.

**Interview-ready line:** "Linked Lists trade CPU cache locality for $O(1)$ node insertion and deletion —
they shine in LRU caches and lock-free queues where elements are frequently spliced between pointers."

---

## 3. Hash Maps & Hash Tables (Buckets & Collision Physics)

**What it is:** An associative key-value store that maps arbitrary keys to bucket
array indices using a **Hash Function**:
$$\text{Index} = \text{hash}(\text{key}) \pmod{\text{Capacity}}$$

**Strengths**
- Average-case $O(1)$ time complexity for insertions, deletions, and key lookups.
- Universal versatility for caching, frequency counting, and indexing.
- **Collision Resolution Strategies:**
  - *Separate Chaining:* Each bucket holds a linked list (or Red-Black tree in Java 8+) of collided entries.
  - *Open Addressing (Linear/Quadratic Probing & Robin Hood Hashing):* Collisions probe sequential slots in the array, maximizing cache locality.

**Weaknesses**
- Worst-case $O(N)$ lookup time if a poor hash function maps all keys to a single bucket
  (can be exploited in **Hash Collision Denial-of-Service attacks**).
- No inherent ordering: keys are unordered (cannot perform range scans like `BETWEEN 10 AND 50`).
- Resizing overhead: when the **Load Factor** ($\alpha = N / \text{Capacity}$) exceeds a threshold
  (typically 0.75), the backing array doubles, forcing an $O(N)$ rehash of all keys.

**When to use:** Sub-millisecond key lookups, deduplication, caches, and associative mapping.

**Interview-ready line:** "Hash maps deliver amortized $O(1)$ operations, but maintaining that performance
requires keeping the load factor below 0.75 and using collision-resistant hash functions like SipHash to prevent HashDoS vulnerabilities."

---

## 4. Trees & Balanced Binary Search Trees (BST, AVL, Red-Black & B+ Trees)

**What it is:** Hierarchical structures of connected nodes. Binary Search Trees enforce
the ordering property: $\text{Left} < \text{Root} \le \text{Right}$.

**Strengths**
- **Self-Balancing BSTs (Red-Black Trees, AVL Trees):** Enforce height balance via tree rotations,
  guaranteeing strict $O(\log N)$ worst-case search, insertion, and deletion.
- In-order traversal yields elements in sorted order; supports range queries and min/max queries.
- **B-Trees & B+ Trees:** Multi-way balanced search trees with high branching factors ($M \approx 100\text{--}1000$).
  Designed specifically for block storage (PostgreSQL B-Tree indexes, filesystems), fitting an entire tree node into an 8KB/16KB disk page.

**Weaknesses**
- Standard un-balanced BSTs degrade to an $O(N)$ linked list if elements are inserted in sorted order.
- In-memory BSTs suffer from pointer chasing and cache misses compared to flat arrays.

**When to use:** Red-Black trees for in-memory sorted maps (`std::map`, Java `TreeMap`);
B+ Trees for on-disk database indexing and filesystem directories.

**Interview-ready line:** "Red-Black trees optimize for in-memory rotations, whereas B+ Trees maximize
branching factors to align nodes with 8KB disk pages, minimizing expensive disk seeks during database index scans."

---

## 5. Heaps & Priority Queues (Binary Min/Max Heaps)

**What it is:** A complete binary tree satisfying the **Heap Property**: in a Min-Heap,
every parent node is $\le$ its children ($\text{Root} = \text{Minimum}$). Stored compactly
in a flat array without pointers:
$$\text{Left Child}(i) = 2i + 1, \quad \text{Right Child}(i) = 2i + 2, \quad \text{Parent}(i) = \lfloor (i-1)/2 \rfloor$$

**Strengths**
- Instant $O(1)$ access to the minimum (or maximum) element.
- $O(\log N)$ insertion (`siftUp`) and $O(\log N)$ root extraction (`siftDown`).
- **$O(N)$ Heap Construction (`heapify`):** Building a heap from an unsorted array takes linear $O(N)$
  time using bottom-up sift-down, not $O(N \log N)$.
- Zero pointer overhead: mapped entirely onto a contiguous dynamic array.

**Weaknesses**
- Inefficient search: finding an arbitrary non-root element requires an $O(N)$ linear scan.
- Not a sorted structure: only the root element's relative rank is guaranteed.

**When to use:** Operating system process schedulers, Dijkstra's Shortest Path algorithm,
Top-K elements streaming, Huffman coding, and event-driven timers.

**Interview-ready line:** "A binary heap gives you $O(1)$ min/max extraction with zero pointer overhead
because its complete binary tree structure maps directly onto a flat array with $O(N)$ `heapify` construction."

---

## 6. Graphs (Adjacency Matrix vs. Adjacency List vs. Index-Free Adjacency)

**What it is:** A network of **Vertices (Nodes)** connected by **Edges (Relationships)**,
which can be directed, undirected, weighted, or cyclic.

**Strengths**
- **Representations:**
  - *Adjacency Matrix ($V \times V$ boolean array):* $O(1)$ edge existence check (`hasEdge(u, v)`), ideal for dense graphs ($E \approx V^2$).
  - *Adjacency List (Array of Lists):* Space-efficient $O(V + E)$ memory footprint, ideal for sparse graphs ($E \ll V^2$).
  - *Index-Free Adjacency (Neo4j / Graph DBs):* Nodes store direct physical memory pointers to neighbor edge records, executing $O(k)$ pointer hops without table joins.
- Core Algorithms: BFS (shortest path in unweighted graphs), DFS (cycle detection, topological sort), Dijkstra (weighted shortest path).

**Weaknesses**
- Adjacency Matrix wastes $O(V^2)$ memory for sparse graphs.
- Adjacency List requires traversing a linked list to verify edge existence ($O(\text{degree})$).

**When to use:** Social networks, recommendation systems, road routing networks, dependency
schedulers (DAGs), and fraud detection rings.

**Interview-ready line:** "For sparse real-world networks, Adjacency Lists save $O(V^2)$ memory,
while Graph Databases use Index-Free Adjacency so traversals become raw $O(k)$ memory pointer hops instead of multi-table joins."

---

## 7. Tries / Prefix Trees (Radix Trees & Compressed Tries)

**What it is:** A tree-based search structure where each node represents a character
along a string path, allowing words with shared prefixes to share identical root paths.

**Strengths**
- **$O(L)$ Search & Insert Time:** Bounded purely by the length of the string $L$,
  completely independent of the total number of words in the dataset $N$.
- Instant Prefix Filtering: ideal for search autocomplete, spell checkers, and IP routing tables.
- **Radix Tree (Compressed Trie):** Merges single-child descendant nodes into single edges
  (e.g., `t-e-s-t` compressed into `test`), drastically reducing memory overhead.

**Weaknesses**
- Memory intensive: standard uncompressed Tries allocate 26 (or 256 for ASCII) child pointers
  per node, leading to high heap consumption and sparse pointers.

**When to use:** Autocomplete search bars, typeahead prediction, dictionary lookups,
network IP CIDR prefix matching (Longest Prefix Match), and inverted index tokenizers.

**Interview-ready line:** "A Trie decouples search speed from dataset size — query time is
bounded strictly by string length $O(L)$, making it the standard for autocomplete engines and IP routing tables."

---

## 8. Advanced Specialized Structures (Bloom Filters & SkipLists)

**What it is:** High-performance probabilistic and probabilistic-ordered structures.

**Strengths**
- **Bloom Filter:** A space-efficient probabilistic bit array using $k$ independent hash functions.
  Guarantees: **Zero False Negatives** (*"Definitely NOT in set"* or *"PROBABLY in set"*).
  Uses ~10 bits per element to protect databases from reading disks for non-existent keys.
- **SkipList:** A probabilistic multi-level linked list that uses coin-flip heights to achieve
  $O(\log N)$ search, insertion, and range scanning without complex tree balancing rotations (powers **Redis Sorted Sets `ZSET`** and RocksDB MemTables).

**Weaknesses**
- Bloom Filters cannot delete elements (unless using Counting Bloom Filters) and have false positive rates.
- SkipLists use probabilistic randomization, meaning bad random seed streaks can temporarily degrade balance.

**When to use:** Bloom Filters in front of databases (LSM SSTables) to eliminate useless disk reads;
SkipLists for concurrent in-memory sorted sets and range queries.

**Interview-ready line:** "Bloom Filters protect disks from useless seeks with zero false negatives,
while SkipLists provide $O(\log N)$ sorted range queries with far simpler lock-free concurrency than Red-Black trees."

---

## Side-by-Side Comparison

| Data Structure | Access Time | Search Time | Insertion Time | Deletion Time | Space Overhead | Cache Locality |
|---|---|---|---|---|---|---|
| **Array** | $O(1)$ | $O(N)$ / $O(\log N)$ | $O(N)$ (shift) | $O(N)$ (shift) | Minimal ($O(N)$ contiguous) | **Best (100% Cache Lines)** |
| **Linked List** | $O(N)$ | $O(N)$ | $O(1)$ (at ptr) | $O(1)$ (at ptr) | 8–16 bytes ptr / node | **Worst (Pointer Chasing)** |
| **Hash Map** | N/A | $O(1)$ avg / $O(N)$ | $O(1)$ avg / $O(N)$ | $O(1)$ avg / $O(N)$ | Load factor slack (~1.3x) | Medium (Open Addressing) |
| **Red-Black Tree**| N/A | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | 3 ptrs + 1 bit / node | Poor (Dispersed Heap) |
| **B+ Tree** | N/A | $O(\log_M N)$ | $O(\log_M N)$ | $O(\log_M N)$ | Page block aligned | **High (Page Chunked)** |
| **Binary Heap** | $O(1)$ (min/max) | $O(N)$ | $O(\log N)$ | $O(\log N)$ | Minimal ($O(N)$ flat array) | **High (Array Indexed)** |
| **Trie** | N/A | $O(L)$ ($L = \text{len}$) | $O(L)$ | $O(L)$ | High (Sparse Child Ptrs) | Poor |
| **SkipList** | N/A | $O(\log N)$ avg | $O(\log N)$ avg | $O(\log N)$ avg | ~1.33 ptrs / node | Medium |

---

## Decision Framework (say this out loud in interviews)

1. **Sequential data, linear scans, and maximum numerical performance** → Contiguous Array / Vector.
2. **Key-value lookups with sub-millisecond average access** → Hash Map (keep load factor $<0.75$).
3. **Sorted data requiring in-memory range queries and order traversal** → Red-Black Tree (`std::map`).
4. **On-disk database index or filesystem storage** → B+ Tree (aligned to 8KB/16KB page blocks).
5. **Continuous min/max priority retrieval or task scheduling** → Binary Min/Max Heap ($O(N)$ `heapify`).
6. **Network relationships, social graphs, or dependency execution** → Graph (Adjacency List for sparse, Matrix for dense).
7. **Search bar autocomplete, typeahead, or IP prefix matching** → Trie / Radix Tree ($O(L)$ time).
8. **Preventing expensive database disk reads for non-existent keys** → Bloom Filter.
9. **Concurrent sorted set with lock-free range queries** → SkipList (Redis `ZSET`).

---

## What separates a senior answer from a junior one

### 1. Hardware Cache Lines vs. Pure Big-O Theory
- **Junior:** "Linked Lists are faster than Arrays for insertions because inserting is $O(1)$ while Array is $O(N)$."
- **Senior:** *"In real hardware benchmarks, an Array iteration is often **10 to 50 times faster** than traversing a Linked List, even with extra element copies. A senior explains the physics: modern CPUs fetch memory in **64-byte Cache Lines**. An Array's contiguous memory maximizes L1/L2 cache prefetching with ~1ns latency. A Linked List allocates nodes randomly across heap memory, forcing the CPU instruction pipeline to stall for ~100ns on every pointer hop. Unless you are constantly splicing nodes in the middle without shifting, flat contiguous vectors beat linked lists."*

### 2. Hash Collision Resolution Mechanics
- **Junior:** "A Hash Map handles collisions with a linked list."
- **Senior:** *"A senior compares collision resolution paradigms:
  - **Separate Chaining (Java 8+):** Buckets hold linked lists, but upgrade to **Red-Black trees** when a bucket exceeds 8 entries to defend against $O(N)$ denial-of-service degradation.
  - **Open Addressing with Robin Hood Hashing:** Collided items probe subsequent array slots, taking slots from 'richer' items (items with fewer probe hops) to minimize the maximum probe distance. This keeps data inside a single contiguous array, maximizing cache line utilization."*

### 3. Tree Physics: Why Databases Use B+ Trees Over Red-Black Trees
- **Junior:** "Databases use binary search trees to index rows."
- **Senior:** *"A binary search tree (like a Red-Black tree) has a branching factor of 2, resulting in tree heights of $\log_2(1,000,000) \approx 20$. In a disk-based database, traversing 20 levels requires 20 random disk I/O seeks ($20 \times 10\text{ms} = 200\text{ms}$). A senior explains why databases use **B+ Trees**:
  1. **High Branching Factor ($M \approx 500$):** A tree height of only 3 can index 125 million rows ($3\text{ disk seeks}$).
  2. **Page Alignment:** Each B+ tree node fits precisely into a single 8KB/16KB disk page block.
  3. **Leaf Linked Lists:** All leaf nodes are doubly linked, allowing range scans (`WHERE age BETWEEN 20 AND 30`) to scan contiguous pages without re-traversing the tree root."*

### 4. Dynamic Array Geometric Growth Factor ($1.5\times$ vs. $2\times$)
- **Junior:** "When an array runs out of space, it doubles in size."
- **Senior:** *"A senior explains why modern standard libraries choose different growth factors:
  - Doubling ($2\times$) guarantees amortized $O(1)$ insertions, but because the new size ($2^k$) is strictly greater than the sum of all previously allocated blocks ($\sum_{i=0}^{k-1} 2^i = 2^k - 1$), the memory allocator can **never reuse previously freed memory chunks**.
  - Modern engines (like MSVC and Facebook folly `fbvector`) use a **$1.5\times$ growth factor**. At $1.5\times$, by the 4th reallocation, the newly requested size is smaller than the sum of previously freed contiguous blocks, allowing the OS memory allocator to reuse cached heap chunks."*

### 5. Graph Traversals: Matrix vs. List vs. Index-Free Adjacency
- **Junior:** "Graphs are just matrices with 1s and 0s."
- **Senior:** *"A senior matches representation to the problem:
  - **Adjacency Matrix ($O(V^2)$):** Ideal for dense graphs or algorithms computing transitive closure with matrix multiplication (Floyd-Warshall).
  - **Adjacency List ($O(V + E)$):** Standard for sparse real-world graphs (social networks, road maps) to prevent gigabytes of zero-filled memory.
  - **Index-Free Adjacency (Neo4j):** In relational databases, traversing many-to-many joins requires $O(N \log N)$ index seeks; in graph engines, vertices store **direct physical memory pointers** to adjacent edge records, allowing $k$-hop traversals in constant $O(k)$ pointer time regardless of total database size."*
