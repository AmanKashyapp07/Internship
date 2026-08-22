# Core DSA Algorithms: Master Time & Space Complexity Reference

> **Quick Reference Guide:** Complete point-by-point breakdown of Time Complexity, Auxiliary Space Complexity, underlying data structures, algorithmic mechanics, and edge cases for all core algorithms across Data Structures & Algorithms (formatted strictly in structured points with zero tables).

---

# Table of Contents
1. [Graph Algorithms](#1-graph-algorithms)
2. [Sorting & Selection Algorithms](#2-sorting--selection-algorithms)
3. [Searching & String Matching Algorithms](#3-searching--string-matching-algorithms)
4. [Tree & Range Query Algorithms](#4-tree--range-query-algorithms)
5. [Dynamic Programming Algorithms](#5-dynamic-programming-algorithms)
6. [Mathematical & Number Theory Algorithms](#6-mathematical--number-theory-algorithms)

---

# 1. Graph Algorithms

## Breadth-First Search (BFS)
- **Time Complexity:** **$\Theta(V + E)$**
  - Every vertex $V$ is enqueued and dequeued exactly once: $O(V)$.
  - Every edge $E$ in the adjacency list is inspected: $O(E)$ (directed) or $O(2E)$ (undirected).
- **Auxiliary Space Complexity:** **$O(V)$**
  - Queue storage holds at most the maximum width of the graph ($O(V)$).
  - Boolean `visited` array of size $V$: $O(V)$.
- **Underlying Engine:** FIFO Queue + Adjacency List.
- **Key Guarantee:** Finds the **Shortest Path in Unweighted Graphs** with minimum edge count.

---

## 0-1 BFS (Shortest Path on 0/1 Weighted Graphs)
- **Time Complexity:** **$O(V + E)$**
  - Bypasses Dijkstra's $O((V + E) \log V)$ priority queue overhead by using a Double-Ended Queue (Deque).
- **Auxiliary Space Complexity:** **$O(V)$**
  - Deque storage + `dist[]` array of size $V$.
- **Underlying Engine:** Double-Ended Queue (`std::deque`).
- **Mechanics:** If traversing an edge of weight $0$, push to front (`push_front`); if weight $1$, push to back (`push_back`), keeping distances in the queue monotonically sorted in $O(1)$ time.

---

## Depth-First Search (DFS)
- **Time Complexity:** **$\Theta(V + E)$**
  - Visits every vertex once and explores all incident edges.
- **Auxiliary Space Complexity:** **$O(V)$**
  - Recursive call stack depth is $O(V)$ in worst-case skewed/line graphs ($O(\log V)$ on balanced trees).
  - `visited` array of size $V$: $O(V)$.
- **Underlying Engine:** System Call Stack (or explicit `std::stack`) + Adjacency List.
- **Key Uses:** Connected components, cycle detection, topological sorting, backtracking, maze solving.

---

## Dijkstra’s Algorithm (Single-Source Shortest Path)
- **Time Complexity:**
  - **With Binary Heap (`std::priority_queue`):** **$O((V + E) \log V)$**
    - Each vertex is extracted from the heap once: $O(V \log V)$.
    - Each edge relaxation potentially pushes a new distance to the heap: $O(E \log V)$.
  - **With Fibonacci Heap:** **$O(E + V \log V)$** (via $O(1)$ amortized `decrease-key`).
  - **With Unindexed Array:** **$O(V^2)$** (optimal for dense graphs where $E \approx V^2$).
- **Auxiliary Space Complexity:** **$O(V)$**
  - Priority Queue holds at most $O(V)$ (or $O(E)$ with duplicates) pairs + `dist[]` array of size $V$.
- **Underlying Engine:** Min-Priority Queue (Binary Min-Heap) + Adjacency List.
- **Critical Failure Mode:** **Fails completely on graphs with Negative Edge Weights** (greedily marks nodes as finalized, missing shorter paths via negative edges).

---

## Bellman-Ford Algorithm (Shortest Path with Negative Edges)
- **Time Complexity:**
  - **Standard Run:** **$\Theta(V \cdot E)$**
    - Relaxes all $E$ edges in the graph sequentially for $V - 1$ outer iterations.
  - **Complete Graph (Dense):** $O(V^3)$ (since $E = V(V-1)/2$).
- **Auxiliary Space Complexity:** **$O(V)$**
  - Stores a 1D `dist[]` array of size $V$.
- **Underlying Engine:** Edge List array.
- **Key Advantage:** Handles negative edge weights and **detects Negative Weight Cycles** on a $V$-th relaxation pass.

---

## Floyd-Warshall Algorithm (All-Pairs Shortest Path)
- **Time Complexity:** **$\Theta(V^3)$**
  - 3 deeply nested loops: intermediate vertex $k$ from $0 \to V-1$, source $i$ from $0 \to V-1$, destination $j$ from $0 \to V-1$.
- **Auxiliary Space Complexity:** **$\Theta(V^2)$**
  - Requires a 2D distance matrix `dist[V][V]`.
- **Underlying Engine:** 2D Dynamic Programming Grid.
- **Key Rule:** Intermediate vertex $k$ **MUST be in the outermost loop** to satisfy dynamic programming subproblem dependencies. Detects negative cycles if `dist[i][i] < 0`.

---

## Kruskal’s Algorithm (Minimum Spanning Tree)
- **Time Complexity:** **$O(E \log E) = O(E \log V)$**
  - Sorting all $E$ edges globally takes $O(E \log E)$.
  - Performing $2E$ `find()` and $V - 1$ `union()` operations via DSU takes $O(E \cdot \alpha(V)) \approx O(E)$.
  - Total time is completely dominated by edge sorting: $O(E \log E)$.
- **Auxiliary Space Complexity:** **$O(V + E)$**
  - Edge List storage: $O(E)$.
  - Disjoint Set Union (DSU) `parent` and `rank` arrays of size $V$: $O(V)$.
- **Underlying Engine:** Disjoint Set Union (DSU / Union-Find) with Path Compression + Edge Array.
- **Ideal For:** Sparse Graphs ($E \approx V$).

---

## Prim’s Algorithm (Minimum Spanning Tree)
- **Time Complexity:**
  - **With Binary Min-Heap (`priority_queue`):** **$O((V + E) \log V)$**
  - **With Fibonacci Heap:** **$O(E + V \log V)$**
  - **With Adjacency Matrix (Dense):** **$O(V^2)$**
- **Auxiliary Space Complexity:** **$O(V)$**
  - Priority Queue holds active cut boundary edges + `inMST[]` boolean array of size $V$.
- **Underlying Engine:** Min-Priority Queue + Adjacency List.
- **Ideal For:** Dense Graphs ($E \approx V^2$).

---

## Kahn’s Algorithm (Topological Sort via BFS In-Degree)
- **Time Complexity:** **$\Theta(V + E)$**
  - Computes initial in-degree for all vertices: $O(V + E)$.
  - Enqueues and decrements edges: $O(V + E)$.
- **Auxiliary Space Complexity:** **$O(V)$**
  - In-degree array of size $V$ + FIFO queue holding zero in-degree vertices.
- **Underlying Engine:** FIFO Queue + In-Degree Array.
- **Cycle Detection:** If the resulting topological sort order contains fewer than $V$ vertices, the directed graph contains a **Cycle**.

---

## Disjoint Set Union (DSU / Union-Find)
- **Time Complexity per Operation (`find`, `union`):** **$O(\alpha(N)) \approx O(1)$ Amortized**
  - $\alpha(N)$ is the Inverse Ackermann Function. For any practical universe input size ($N \le 10^{80}$), $\alpha(N) \le 4$.
- **Auxiliary Space Complexity:** **$O(N)$**
  - `parent[]` and `rank[]` (or `size[]`) arrays of size $N$.
- **Underlying Engine:** Tree-based array with **Path Compression** (`parent[i] = find(parent[i])`) and **Union by Rank / Size**.

---

## Tarjan’s Bridge & Articulation Point Algorithm
- **Time Complexity:** **$\Theta(V + E)$**
  - Single-pass DFS traversal calculating discovery timestamps and low-link reachable values.
- **Auxiliary Space Complexity:** **$O(V)$**
  - Recursion call stack + `disc[]` and `low[]` arrays of size $V$.
- **Underlying Engine:** DFS Recursion + Low-Link Values.
- **Bridge Formula:** Edge $(u, v)$ is a bridge if and only if **`low[v] > disc[u]`**.

---

## Kosaraju’s Algorithm (Strongly Connected Components - SCC)
- **Time Complexity:** **$\Theta(V + E)$**
  - Pass 1: Standard DFS on original graph pushing nodes to stack on finish: $O(V + E)$.
  - Reversing graph edges (Transpose $G^T$): $O(V + E)$.
  - Pass 2: DFS on $G^T$ popping from stack: $O(V + E)$.
- **Auxiliary Space Complexity:** **$O(V + E)$**
  - Transposed adjacency list $G^T$ ($O(V + E)$) + Finish-time stack ($O(V)$).

---

# 2. Sorting & Selection Algorithms

## Merge Sort
- **Time Complexity:**
  - **Best Case:** **$\Theta(N \log N)$**
  - **Average Case:** **$\Theta(N \log N)$**
  - **Worst Case:** **$\Theta(N \log N)$**
- **Auxiliary Space Complexity:** **$O(N)$**
  - Requires a temporary auxiliary buffer during the merge phase + $O(\log N)$ call stack frames.
- **Properties:** **Stable**, **Not In-Place**.

---

## Quick Sort
- **Time Complexity:**
  - **Best Case:** **$\Theta(N \log N)$** (Pivots split array into equal halves $N/2$).
  - **Average Case:** **$\Theta(N \log N)$**
  - **Worst Case:** **$O(N^2)$** (Pivots split array into $0$ and $N-1$ elements on already-sorted arrays).
- **Auxiliary Space Complexity:**
  - **Average:** **$O(\log N)$** (Call stack depth).
  - **Worst:** **$O(N)$** (Skewed recursion tree).
- **Properties:** **Unstable**, **In-Place** (Lomuto / Hoare partitioning).

---

## Heap Sort
- **Time Complexity:**
  - **Best Case:** **$\Theta(N \log N)$**
  - **Average Case:** **$\Theta(N \log N)$**
  - **Worst Case:** **$\Theta(N \log N)$**
- **Auxiliary Space Complexity:** **$O(1)$ (Strictly In-Place)**
  - Operates directly inside the original array.
- **Properties:** **Unstable**, **In-Place** (Builds max-heap in $O(N)$, extracts in $O(N \log N)$).

---

## Counting Sort
- **Time Complexity:** **$\Theta(N + K)$** (where $K$ is the range of values $\max - \min + 1$).
  - Scans array $N$ items + accumulates prefix sums over frequency array of size $K$.
- **Auxiliary Space Complexity:** **$\Theta(N + K)$**
  - Frequency count array of size $K$ + output array of size $N$.
- **Properties:** **Stable** (when iterating backwards), **Non-Comparison Linear Sort**.

---

## Radix Sort (LSD)
- **Time Complexity:** **$\Theta(d \cdot (N + b))$**
  - $d$ is the number of digits/passes ($\approx \log_b(\max)$), $b$ is the number base (typically 10 or 256).
- **Auxiliary Space Complexity:** **$\Theta(N + b)$**
  - Intermediate count array of size $b$ + output buffer of size $N$.
- **Properties:** **Stable**, **Non-Comparison Linear Sort**.

---

## Quickselect (K-th Order Statistic / K-th Smallest Element)
- **Time Complexity:**
  - **Average Case:** **$O(N)$ Linear Time** (Master theorem: $T(N) = T(N/2) + O(N) = 2N = O(N)$).
  - **Worst Case:** **$O(N^2)$** (Pathological pivot selection).
- **Auxiliary Space Complexity:** **$O(1)$ Iterative** (or $O(\log N)$ recursive stack).
- **Properties:** Modifies array in-place without sorting the entire collection.

---

# 3. Searching & String Matching Algorithms

## Binary Search
- **Time Complexity:**
  - **Best Case:** **$O(1)$** (Target is at the exact middle element).
  - **Average / Worst Case:** **$\Theta(\log N)$** (Halves the search space on every iteration).
- **Auxiliary Space Complexity:** **$O(1)$ Iterative** (or $O(\log N)$ recursive).
- **Requirements:** Array must be monotonically ordered.

---

## KMP Algorithm (Knuth-Morris-Pratt Pattern Matching)
- **Time Complexity:** **$\Theta(N + M)$**
  - Precomputing LPS (Longest Proper Prefix which is Suffix) table for pattern $P$: $\Theta(M)$.
  - Scanning text $T$ of length $N$ without backtracking: $\Theta(N)$.
- **Auxiliary Space Complexity:** **$\Theta(M)$**
  - `lps[]` array of size $M$ (length of pattern).
- **Advantage:** Never rewinds or backtracks text pointer in $T$.

---

## Rabin-Karp Algorithm (Rolling Polynomial Hashing)
- **Time Complexity:**
  - **Average / Best Case:** **$\Theta(N + M)$** (Computes rolling hash in $O(1)$ per character).
  - **Worst Case:** **$O(N \cdot M)$** (When severe hash collisions force full string comparisons at every index).
- **Auxiliary Space Complexity:** **$O(1)$**
  - Only stores integer hash variables.

---

## Manacher’s Algorithm (Longest Palindromic Substring)
- **Time Complexity:** **$\Theta(N)$ Strict Linear Time**
  - Exploits palindromic symmetry to avoid re-scanning previously expanded centers.
- **Auxiliary Space Complexity:** **$\Theta(N)$**
  - Radius array `P[]` of length $2N + 1$ (interleaved with `#` delimiters).

---

# 4. Tree & Range Query Algorithms

## Binary Heap Operations
- **`buildHeap` (Bottom-Up Construction):** **$\Theta(N)$ Strict Linear Time**
  - Taylor series proof: $\sum_{h=0}^{\log N} \frac{h}{2^h} = 2 \implies O(N)$.
- **`push()` / `insert()`:** **$O(\log N)$ Time** (Heapify-Up).
- **`pop()` / `extractMin()`:** **$O(\log N)$ Time** (Heapify-Down).
- **`top()` / `peek()`:** **$O(1)$ Time** (Accesses root `arr[0]`).
- **Auxiliary Space Complexity:** **$O(1)$** beyond backing array.

---

## Segment Tree
- **Tree Construction (`build`):** **$\Theta(N)$ Time** (Recursively builds $2N - 1$ nodes).
- **Point Update:** **$\Theta(\log N)$ Time** (Updates leaf and traverses path to root).
- **Range Query (Sum / Min / Max):** **$\Theta(\log N)$ Time** (Inspects at most 4 nodes per level).
- **Range Update with Lazy Propagation:** **$\Theta(\log N)$ Time** (Defers updates to `lazy[]` array).
- **Auxiliary Space Complexity:** **$\Theta(4N)$** (Fixed array of size $4N$ stores all internal tree nodes).

---

## Fenwick Tree (Binary Indexed Tree - BIT)
- **Tree Construction (`build`):** **$O(N)$ Time** (or $O(N \log N)$ via repeated updates).
- **Point Update:** **$\Theta(\log N)$ Time** (Traverses lowest set bits: `i += (i & -i)`).
- **Prefix Sum Query:** **$\Theta(\log N)$ Time** (Strips lowest set bits: `i -= (i & -i)`).
- **Range Sum Query $[L, R]$:** **$\Theta(\log N)$ Time** (`query(R) - query(L - 1)`).
- **Auxiliary Space Complexity:** **$\Theta(N)$** (Compact $1\times$ array of size $N + 1$, zero pointers).

---

## Lowest Common Ancestor (LCA) via Binary Lifting
- **Preprocessing Time:** **$\Theta(N \log N)$**
  - Builds $2^k$-th ancestor table `up[N][log N]`: $\text{up}[u][k] = \text{up}[\text{up}[u][k-1]][k-1]$.
- **LCA Query Time:** **$\Theta(\log N)$ per query**
  - Equalizes depths and binary lifts both nodes simultaneously.
- **Auxiliary Space Complexity:** **$\Theta(N \log N)$**
  - Table of dimensions $N \times \log_2 N$.

---

# 5. Dynamic Programming Algorithms

## 0/1 Knapsack Problem
- **Time Complexity:** **$\Theta(N \cdot W)$**
  - $N$ items $\times$ capacity $W$. (Pseudo-polynomial time).
- **Auxiliary Space Complexity:**
  - **Standard 2D Table:** $\Theta(N \cdot W)$.
  - **Space-Optimized 1D Array:** **$\Theta(W)$** (by iterating capacity backwards from $W$ down to $w_i$).

---

## Longest Increasing Subsequence (LIS)
- **Classic Dynamic Programming:**
  - **Time Complexity:** **$\Theta(N^2)$** (Nested loops comparing `nums[j] < nums[i]`).
  - **Space Complexity:** **$\Theta(N)$** (`dp[]` array of size $N$).
- **Patience Sorting with Binary Search (`std::lower_bound`):**
  - **Time Complexity:** **$\Theta(N \log N)$** (Binary searches position in `tails[]` array for each element).
  - **Space Complexity:** **$\Theta(N)$** (`tails[]` array of maximum size $N$).

---

## Longest Common Subsequence (LCS) / Edit Distance
- **Time Complexity:** **$\Theta(M \cdot N)$**
  - Compares all characters between String 1 (length $M$) and String 2 (length $N$).
- **Auxiliary Space Complexity:**
  - **Standard 2D Grid:** $\Theta(M \cdot N)$.
  - **Space-Optimized:** **$\Theta(\min(M, N))$** (requires only 2 rolling rows).

---

## Traveling Salesperson Problem (TSP via Bitmask DP)
- **Time Complexity:** **$\Theta(N^2 \cdot 2^N)$**
  - Total states $= 2^N$ masks $\times N$ cities. Each state transitions to $N$ candidate cities: $2^N \times N \times N$.
- **Auxiliary Space Complexity:** **$\Theta(N \cdot 2^N)$**
  - Memoization DP table `memo[1 << N][N]`.
- **Comparison:** Vastly superior to naive brute-force factorial permutation search ($O(N!)$).

---

# 6. Mathematical & Number Theory Algorithms

## Sieve of Eratosthenes (Prime Generation up to $N$)
- **Time Complexity:** **$\Theta(N \log (\log N))$**
  - Sum of prime reciprocals harmonic series: $N \sum_{p \le N} \frac{1}{p} = N \ln(\ln N) = O(N \log \log N)$.
- **Auxiliary Space Complexity:** **$\Theta(N)$**
  - Boolean `isPrime[]` bitset array of size $N + 1$.

---

## Euclidean Algorithm (Greatest Common Divisor - GCD)
- **Time Complexity:** **$O(\log(\min(a, b)))$**
  - Lamé's Theorem: In the worst case (consecutive Fibonacci numbers), the number of modulo division steps is at most $5 \times \text{number of digits of } \min(a, b)$.
- **Auxiliary Space Complexity:** **$O(1)$ Iterative** (or $O(\log(\min(a, b)))$ recursive stack).

---

## Fast Binary Exponentiation ($x^N \pmod M$)
- **Time Complexity:** **$\Theta(\log N)$**
  - Divides exponent $N$ by 2 on every step ($N \to N/2$).
- **Auxiliary Space Complexity:** **$O(1)$**
  - Executes directly inside CPU register variables.
