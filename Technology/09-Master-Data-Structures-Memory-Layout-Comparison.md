# Data Structure Taxonomies, Memory Layout Physics & Hardware Cache Locality

> **Scope:** Deep Systems Architecture of Fundamental Data Structures: Physical Memory Layouts, CPU L1/L2/L3 Cache Line Mechanics, Contiguous Dynamic Arrays, Heap-Allocated Node Pointer Traversal, Hash Table Collision Physics (Chaining vs. Open Addressing), Self-Balancing Trees (AVL, Red-Black, B+ Trees), Complete Binary Heaps, Radix Tries, Disjoint Set Union (DSU), and Graph Adjacency Structures.

---

# Table of Contents
1. [Physical Memory Hierarchy & Hardware Cache Line Physics](#1-physical-memory-hierarchy--hardware-cache-line-physics)
2. [Contiguous Arrays & Dynamic Vectors](#2-contiguous-arrays--dynamic-vectors)
3. [Linked Node Topologies (Singly, Doubly, Circular)](#3-linked-node-topologies-singly-doubly-circular)
4. [Hash Tables: Collision Resolution & Load Factor Mathematics](#4-hash-tables-collision-resolution--load-factor-mathematics)
5. [Tree Structures: Balanced Binary Search & On-Disk B+ Trees](#5-tree-structures-balanced-binary-search--on-disk-b-trees)
6. [Complete Binary Heaps & Flat Array Indexing](#6-complete-binary-heaps--flat-array-indexing)
7. [Prefix Tries & Compressed Radix Trees](#7-prefix-tries--compressed-radix-trees)
8. [Disjoint Set Union (DSU): Path Compression & Rank Mechanics](#8-disjoint-set-union-dsu-path-compression--rank-mechanics)
9. [Graph Adjacency Memory Representations](#9-graph-adjacency-memory-representations)
10. [Master Data Structures & Memory Layout Comparison Matrix](#10-master-data-structures--memory-layout-comparison-matrix)

---

# 1. Physical Memory Hierarchy & Hardware Cache Line Physics

Modern computer hardware retrieves memory across a hierarchical caching architecture, moving data in fixed-size **64-Byte Cache Lines**:

```
Hardware Latency & Memory Access Costs:
+-------------------------------------------------------------------+
| L1 CPU Cache:         0.5 ns - 1.0 ns (4 clock cycles)            |
| L2 CPU Cache:         3.0 ns - 7.0 ns (14 clock cycles)           |
| L3 Shared Cache:      10.0 ns - 20.0 ns (50 clock cycles)         |
| Main Memory (DRAM):   50.0 ns - 100.0 ns (200 clock cycles)       |
+-------------------------------------------------------------------+
```

```
Contiguous vs. Dispersed Heap Allocation in CPU Cache Lines:
Contiguous Array (High Spatial Locality):
[ Byte 0..3: A[0] | Byte 4..7: A[1] | Byte 8..11: A[2] ... Byte 60..63: A[15] ]
-> Fetching A[0] pre-loads A[0..15] into L1 cache (15 subsequent cache hits).

Dispersed Heap Nodes (Pointer Chasing / Pointer Stalls):
[ Node A (0x1000) ] ---pointer---> [ Node B (0x8F40) ] ---pointer---> [ Node C (0x3A20) ]
-> Every pointer dereference triggers an independent DRAM fetch cycle (~100ns stall).
```

---

# 2. Contiguous Arrays & Dynamic Vectors

Elements occupy a single, contiguous physical address space calculated via direct base pointer arithmetic:

$$\text{Address}(i) = \text{Base} + i \times \text{sizeof}(\text{Element})$$

```
+----------------------------------------------------------------------------------------------------+
| OPERATION            | COMPLEXITY         | HARDWARE EXECUTION MECHANICS                           |
+----------------------------------------------------------------------------------------------------+
| Random Index Access  | O(1)               | Single CPU calculation + L1 cache line lookup          |
| Append (`push_back`) | Amortized O(1)     | Direct write; geometric expansion ($2\times$) on full  |
| Arbitrary Insert/Del | O(N)               | Memory shift via `memmove` CPU vector instructions     |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. Linked Node Topologies (Singly, Doubly, Circular)

Linked structures disperse node allocations across virtual memory, establishing topological order via explicit pointer references:

```
Doubly Linked Node Heap Memory Layout (64-bit Architecture):
+--------------------+--------------------+--------------------+
| Prev* (8 Bytes)    | Payload Data (8B)  | Next* (8 Bytes)    |
+--------------------+--------------------+--------------------+
-> Incurs 16 bytes of metadata overhead per node; eliminates contiguous reallocation spikes.
```

- **LRU Eviction Application:** Combining a Doubly Linked List with a Hash Table enables $O(1)$ node splicing to the list head upon access and $O(1)$ eviction from the list tail.

---

# 4. Hash Tables: Collision Resolution & Load Factor Mathematics

An associative map projecting keys into bucket indices via a universal hash function:

$$\text{Index} = \text{hash}(\text{key}) \pmod M$$

```
Collision Resolution Schemes:
1. Separate Chaining:
[ Bucket 0 ] -> nullptr
[ Bucket 1 ] -> [ Key A | Val A ] ---> [ Key B | Val B ] ---> nullptr

2. Open Addressing (Linear Probing):
[ Index 0: Key A ] [ Index 1: Key B (Collision slot) ] [ Index 2: Key C ]
```

```
+----------------------------------------------------------------------------------------------------+
| RESOLUTION SCHEME    | CACHE LOCALITY      | DEGRADATION MODE      | REHASH THRESHOLD              |
+----------------------------------------------------------------------------------------------------+
| Separate Chaining    | Poor (Pointer hop)  | Graceful list growth  | Load factor $\alpha \ge 1.0$  |
| Open Addressing      | High (Contiguous)   | Primary clustering    | Load factor $\alpha \ge 0.70$ |
+----------------------------------------------------------------------------------------------------+
```

---

# 5. Tree Structures: Balanced Binary Search & On-Disk B+ Trees

```
+----------------------------------------------------------------------------------------------------+
| TREE ARCHITECTURE    | BALANCING MECHANISM                  | STORAGE OPTIMIZATION TARGET          |
+----------------------------------------------------------------------------------------------------+
| AVL Tree             | Strict height invariant:             | Read-intensive in-memory search      |
|                      | $|h_{\text{left}} - h_{\text{right}}| \le 1$ | lookups                              |
| Red-Black Tree       | Relaxed 5-color invariants:          | General-purpose in-memory map        |
|                      | $h_{\max} \le 2 \log_2(N+1)$         | (`std::map`, Linux `rbtree`)         |
| B+ Tree              | Multi-way high branching factor:     | Block storage systems & databases    |
|                      | $M \in [100, 1000]$; linked leaves   | (Nodes sized to 8KB/16KB disk pages) |
+----------------------------------------------------------------------------------------------------+
```

---

# 6. Complete Binary Heaps & Flat Array Indexing

A **Complete Binary Tree** mapped directly into a flat contiguous array without pointers:

$$\text{Parent}(i) = \left\lfloor \frac{i - 1}{2} \right\rfloor, \quad \text{LeftChild}(i) = 2i + 1, \quad \text{RightChild}(i) = 2i + 2$$

```
Heap Sift-Down & Construction Invariant:
Building an N-element heap bottom-up via heapify:
S = sum_{h=0}^{log N} (N / 2^{h+1}) * O(h) = (N / 2) * sum (h / 2^h) = (N / 2) * 2 = O(N)
```

---

# 7. Prefix Tries & Compressed Radix Trees

```
Standard Character Trie vs. Compressed Radix Tree:
Standard Trie:      [ Root ] ---> ('t') ---> ('e') ---> ('s') ---> ('t') -> Terminal
Compressed Radix:   [ Root ] --------------------> ("test") --------------> Terminal
```

- **Radix Tree Optimization:** Merges non-branching intermediate node sequences into single edge strings, reducing memory footprint and pointer traversal depth during routing table lookups.

---

# 8. Disjoint Set Union (DSU): Path Compression & Rank Mechanics

Tracks partitioned non-overlapping subsets with near-constant amortized time complexity:

```
Path Compression + Union by Rank Optimization:
parent[x] = find(parent[x]) // Flattens tree traversal directly to root

Amortized Operational Bound:
T(M, N) = O(M \cdot \alpha(N))
where \alpha(N) is the Inverse Ackermann Function (\alpha(N) < 5 for all physical inputs).
```

---

# 9. Graph Adjacency Memory Representations

```
+----------------------------------------------------------------------------------------------------+
| REPRESENTATION       | MEMORY CONSUMPTION                   | EDGE CHECK ($u \to v$)| NEIGHBOR ITERATION    |
+----------------------------------------------------------------------------------------------------+
| Adjacency Matrix     | $\Theta(V^2)$ contiguous RAM         | $O(1)$ direct index   | $\Theta(V)$ row scan  |
| Adjacency List       | $\Theta(V + E)$ dispersed vectors    | $O(\text{deg}(u))$    | $O(\text{deg}(u))$    |
+----------------------------------------------------------------------------------------------------+
```

---

# 10. Master Data Structures & Memory Layout Comparison Matrix

```
+----------------------------------------------------------------------------------------------------+
| DATA STRUCTURE| MEMORY LAYOUT      | ACCESS TIME | SEARCH TIME | INSERT TIME | CACHE LOCALITY      |
+----------------------------------------------------------------------------------------------------+
| Dynamic Array | Contiguous Block   | O(1)        | O(N)        | O(N) mid    | Optimal (L1 Hits)   |
| Doubly Linked | Dispersed Nodes    | O(N)        | O(N)        | O(1) node   | Poor (DRAM Stalls)  |
| Hash Table    | Bucket Array + List| N/A         | Avg O(1)    | Avg O(1)    | Moderate / Low      |
| Red-Black Tree| Dispersed Nodes    | N/A         | O(log N)    | O(log N)    | Moderate            |
| B+ Tree       | Block Disk Pages   | N/A         | O(log_B N)  | O(log_B N)  | Block Aligned (Disk)|
| Binary Heap   | Contiguous Array   | O(1) root   | O(N)        | O(log N)    | High                |
| Radix Tree    | Compressed Nodes   | N/A         | O(KeyLength)| O(KeyLength)| Moderate            |
| DSU           | Contiguous Arrays  | N/A         | O(alpha(N)) | O(alpha(N)) | High                |
+----------------------------------------------------------------------------------------------------+
```
