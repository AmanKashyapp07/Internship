# Data Structure Operations, Graph Algorithms & Asymptotic Complexity Reference

> **Scope:** Unified Computational Complexity Matrices across Graph Algorithms, Comparison & Non-Comparison Sorting, Linear & Tree Data Structures, Formal Asymptotic Derivations ($O(N)$ Bottom-Up Heapify, Inverse Ackermann Bounds in DSU, Negative-Cycle Detection in Bellman-Ford), and Algorithmic Paradigm Selection Trees.

---

# Table of Contents
1. [Graph Algorithms Complexity Matrix](#1-graph-algorithms-complexity-matrix)
2. [Sorting Algorithms Complexity Matrix](#2-sorting-algorithms-complexity-matrix)
3. [Data Structure Operations & Space Complexity Matrix](#3-data-structure-operations--space-complexity-matrix)
4. [Theoretical Proofs & Foundational Derivations](#4-theoretical-proofs--foundational-derivations)
5. [Universal Algorithmic Paradigm Selector Tree](#5-universal-algorithmic-paradigm-selector-tree)
6. [Core Theoretical Summary Principles](#6-core-theoretical-summary-principles)

---

# 1. Graph Algorithms Complexity Matrix

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM          | GRAPH TOPOLOGY        | TIME COMPLEXITY | SPACE COMPLEXITY | OPERATIONAL CONSTRAINTS |
+----------------------------------------------------------------------------------------------------+
| Breadth-First (BFS)| Unweighted Graph      | O(V + E)        | O(V)             | Shortest path on unweighted|
| 0-1 BFS (Deque)    | Edge weights in {0, 1}| O(V + E)        | O(V)             | Push 0 front, 1 back    |
| Depth-First (DFS)  | General Graph         | O(V + E)        | O(V)             | Recursion stack depth V |
| Dijkstra's (Heap)  | Non-negative weights  | O(E log V)      | O(V)             | Fails on negative edges |
| Bellman-Ford       | Directed with neg wts | O(V * E)        | O(V)             | Detects negative cycles |
| Floyd-Warshall     | All-Pairs Dense Graph | Theta(V^3)      | Theta(V^2)       | Dynamic programming matrix|
| Kruskal's MST      | Undirected Weighted   | O(E log E)      | O(V)             | Edge sort + DSU cycles  |
| Prim's MST (Heap)  | Undirected Weighted   | O(E log V)      | O(V)             | Priority queue expansion|
| Kahn's TopoSort    | Directed Acyclic (DAG)| O(V + E)        | O(V)             | In-degree zero BFS      |
| Tarjan's Bridges   | Undirected Connected  | O(V + E)        | O(V)             | DFS tree low-link values|
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Sorting Algorithms Complexity Matrix

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM          | BEST TIME     | AVERAGE TIME  | WORST TIME    | AUX SPACE | STABILITY | PARADIGM      |
+----------------------------------------------------------------------------------------------------+
| Merge Sort         | O(N log N)    | O(N log N)    | O(N log N)    | O(N)      | Stable    | Divide/Conquer|
| Quick Sort         | O(N log N)    | O(N log N)    | O(N^2)        | O(log N)  | Unstable  | Partitioning  |
| Heap Sort          | O(N log N)    | O(N log N)    | O(N log N)    | O(1)      | Unstable  | Binary Heap   |
| Insertion Sort     | O(N)          | O(N^2)        | O(N^2)        | O(1)      | Stable    | Incremental   |
| Counting Sort      | O(N + K)      | O(N + K)      | O(N + K)      | O(K)      | Stable    | Non-Comparison|
| Radix Sort         | O(d * (N + b))| O(d * (N + b))| O(d * (N + b))| O(N + b)  | Stable    | Digit-by-Digit|
| Quickselect        | O(N)          | O(N)          | O(N^2)        | O(1)      | Unstable  | Selection     |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. Data Structure Operations & Space Complexity Matrix

```
+----------------------------------------------------------------------------------------------------+
| DATA STRUCTURE     | ACCESS        | SEARCH        | INSERTION     | DELETION      | SPACE FOOTPRINT  |
+----------------------------------------------------------------------------------------------------+
| Dynamic Array      | O(1)          | O(N)          | Amortized O(1)| O(N) mid / O(1)| Contiguous RAM   |
| Doubly Linked List | O(N)          | O(N)          | O(1) (at node)| O(1) (at node)| 24 B ptrs/node   |
| Stack / Queue      | O(1) top/front| O(N)          | O(1)          | O(1)          | O(N) capacity    |
| Binary Heap (PQ)   | O(1) peak     | O(N)          | O(log N)      | O(log N)      | Flat contiguous  |
| Red-Black Tree     | N/A           | O(log N)      | O(log N)      | O(log N)      | 32 B node struct |
| Hash Table         | N/A           | Avg O(1)/O(N) | Avg O(1)/O(N) | Avg O(1)/O(N) | Bucket + pointers|
| Prefix Trie        | N/A           | O(L) (length) | O(L)          | O(L)          | Alphabet * N * L |
| Disjoint Set (DSU) | N/A           | O(alpha(N))   | O(alpha(N))   | N/A           | 2N array space   |
| Segment Tree       | N/A           | O(log N) range| O(log N) point| N/A           | 4N tree nodes    |
| Fenwick Tree (BIT) | N/A           | O(log N) pref | O(log N) point| N/A           | N integer array  |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Theoretical Proofs & Foundational Derivations

### 1. $O(N)$ Bottom-Up Heap Construction Derivation
Constructing a binary heap bottom-up via `build_heap` evaluates sift-down operations starting from level $h = \lfloor \log_2 N \rfloor - 1$ down to root level $0$.
- At height $h$, there are at most $\lceil N / 2^{h+1} \rceil$ nodes.
- Total comparison work $S$:
  $$S = \sum_{h=0}^{\lfloor \log N \rfloor} \frac{N}{2^{h+1}} \cdot O(h) = \frac{N}{2} \sum_{h=0}^{\infty} \frac{h}{2^h}$$
- Using the standard geometric progression identity $\sum_{h=0}^{\infty} h x^h = \frac{x}{(1-x)^2}$ for $x = 1/2$:
  $$\sum_{h=0}^{\infty} \frac{h}{2^h} = \frac{1/2}{(1 - 1/2)^2} = 2 \implies S = \frac{N}{2} \cdot 2 = \mathbf{O(N)}$$

---

### 2. Disjoint Set Union (DSU) Inverse Ackermann Bound
Combining **Path Compression** with **Union by Rank** guarantees that any sequence of $M$ operations on $N$ elements executes in:
$$T(M, N) = O(M \cdot \alpha(N))$$
where $\alpha(N)$ is the **Inverse Ackermann Function**, which satisfies $\alpha(N) < 5$ for all values of $N \le 10^{80}$ (the estimated number of atoms in the observable universe), yielding effectively constant amortized runtime ($O(1)$).

---

### 3. Dijkstra's Algorithm Invariant Failure on Negative Edges
Dijkstra's algorithm relies on the monotonic property that whenever a vertex $u$ is extracted from the priority queue, its path distance $d[u]$ represents the definitive shortest path. A negative edge $(u, v)$ with weight $w < 0$ encountered subsequently violates this greedy invariant, as a previously relaxed node cannot be revisited without unbounded re-relaxations (degenerating into Bellman-Ford).

---

### 4. The Master Theorem Formulation
For divide-and-conquer recurrences of the form $T(N) = a T(N / b) + f(N)$, let $c_{\text{crit}} = \log_b a$:
1. **Case 1:** $f(N) = O(N^c)$ where $c < c_{\text{crit}} \implies T(N) = \mathbf{\Theta(N^{\log_b a})}$.
2. **Case 2:** $f(N) = \Theta(N^{c_{\text{crit}}} \log^k N) \implies T(N) = \mathbf{\Theta(N^{\log_b a} \log^{k+1} N)}$.
3. **Case 3:** $f(N) = \Omega(N^c)$ where $c > c_{\text{crit}}$ and regularity condition holds $\implies T(N) = \mathbf{\Theta(f(N))}$.

---

# 5. Universal Algorithmic Paradigm Selector Tree

```
Computational Objective:
|-- Pathfinding / Shortest Distance:
|   |-- Unweighted graph -----------------------------------> Breadth-First Search (Queue)
|   |-- Edge weights in {0, 1} -----------------------------> 0-1 BFS (Double-Ended Queue)
|   |-- Non-negative arbitrary weights ---------------------> Dijkstra's Algorithm (Min-Heap)
|   |-- Negative edge weights / cycle detection ------------> Bellman-Ford Algorithm
|   `-- All-pairs dense shortest path ----------------------> Floyd-Warshall Algorithm
|-- Structural Connectivity / Spanning Subgraphs:
|   |-- Dependency / Prerequisite DAG ordering -------------> Kahn's Topological Sort (In-Degree BFS)
|   |-- Dynamic connected components -----------------------> Disjoint Set Union (Path Compression + Rank)
|   `-- Minimum Spanning Tree ------------------------------> Kruskal's (DSU) or Prim's (Heap)
|-- Sequential & Sequence Optimization:
|   |-- Monotonic extrema search space ---------------------> Binary Search on Solution Space
|   |-- Subarray aggregation / Bound maintenance -----------> Sliding Window / Two Pointers
|   |-- Next greater / Boundary span evaluation ------------> Monotonic Stack
|   |-- Sliding window minimum / maximum -------------------> Monotonic Deque
|   `-- Overlapping subproblems with optimal substructure --> Dynamic Programming
`-- Combinatorial Search:
    `-- Permutation / Combination state-space --------------> Backtracking + Branch Pruning
```

---

# 6. Core Theoretical Summary Principles

1. **Comparison Lower Bound:** $\Omega(N \log N)$ lower bound is mathematically mandated by decision tree height $h \ge \log_2(N!)$.
2. **Bottom-Up Heap Construction:** `build_heap` runs in $O(N)$ because the majority of nodes reside at low tree heights.
3. **Inverse Ackermann Bound:** DSU with path compression and union by rank achieves $O(\alpha(N))$ amortized operation time.
4. **Greedy Invariant Violation:** Dijkstra requires non-negative edge weights to guarantee that extracted vertex distances are globally optimal.
