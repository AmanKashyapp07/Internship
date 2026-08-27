# Master Guide 05: Core DSA Algorithms Complexity & Interview Flashcards

> **Focus:** Comprehensive master reference breakdown of Time Complexity (Best, Average, Worst), Auxiliary Space Complexity, underlying engines, mechanics, decision trees, and FAANG interview flashcards for all core Data Structures & Algorithms.
> 
> *Designed to crush top-tier FAANG/HFT technical interviews without missing a single bit of detail.*

---

# Table of Contents
1. [Graph Algorithms Master Reference](#1-graph-algorithms-master-reference)
2. [Sorting & Selection Algorithms Master Reference](#2-sorting--selection-algorithms-master-reference)
3. [Searching & String Algorithms Master Reference](#3-searching--string-algorithms-master-reference)
4. [Tree & Range Query Algorithms Master Reference](#4-tree--range-query-algorithms-master-reference)
5. [Dynamic Programming & Number Theory Master Reference](#5-dynamic-programming--number-theory-master-reference)
6. [Top FAANG Interview Flashcard Q&A](#6-top-faang-interview-flashcard-qa)

---

# 1. Graph Algorithms Master Reference

## Breadth-First Search (BFS)
- **Time Complexity:** **$\Theta(V + E)$** (Every vertex is enqueued/dequeued once; every edge in adjacency list inspected once).
- **Auxiliary Space:** **$O(V)$** (Queue holds max width of graph; `visited[]` array of size $V$).
- **Underlying Engine:** FIFO Queue + Adjacency List.
- **Key Guarantee:** Shortest path in **unweighted graphs** (minimum edge count).

## 0-1 BFS
- **Time Complexity:** **$O(V + E)$** (Avoids Dijkstra's $O((V + E) \log V)$ log factor).
- **Auxiliary Space:** **$O(V)$** (Deque + distance array).
- **Underlying Engine:** Deque (`std::deque`).
- **Mechanics:** Weight 0 edge $\implies$ `push_front()`; Weight 1 edge $\implies$ `push_back()`. Maintains monotonic distances in $O(1)$ time.

## Depth-First Search (DFS)
- **Time Complexity:** **$\Theta(V + E)$**
- **Auxiliary Space:** **$O(V)$** (Call stack depth $O(V)$ in skewed graph).
- **Underlying Engine:** System Call Stack + Adjacency List.
- **Applications:** Connected components, cycle detection, topological sort, maze solving, SCCs, bridges.

## Dijkstra’s Algorithm
- **Time Complexity:**
  - With Binary Heap (`priority_queue`): **$O((V + E) \log V)$**
  - With Fibonacci Heap: **$O(E + V \log V)$**
  - With Unindexed Array: **$O(V^2)$** (Optimal for dense graphs $E \approx V^2$).
- **Auxiliary Space:** **$O(V)$**
- **Underlying Engine:** Min-Priority Queue + Adjacency List.
- **Critical Failure Mode:** **Fails on graphs with Negative Edge Weights** (greedily finalizes distance, missing shorter negative edge paths).

## Bellman-Ford Algorithm
- **Time Complexity:** **$\Theta(V \cdot E)$** (Relaxes all $E$ edges $V-1$ times).
- **Auxiliary Space:** **$O(V)$** (1D `dist[]` array).
- **Underlying Engine:** Edge List array.
- **Key Advantage:** Handles negative weights and **detects Negative Weight Cycles** on a $V$-th pass.

## Floyd-Warshall Algorithm
- **Time Complexity:** **$\Theta(V^3)$** (3 nested loops: intermediate node $k$, source $i$, destination $j$).
- **Auxiliary Space:** **$\Theta(V^2)$** (2D matrix `dist[V][V]`).
- **Underlying Engine:** 2D Dynamic Programming Grid.
- **Critical Rule:** Intermediate node $k$ **MUST be in the outermost loop**. Detects negative cycle if `dist[i][i] < 0`.

## Kruskal’s MST Algorithm
- **Time Complexity:** **$O(E \log E) = O(E \log V)$** (Dominated by sorting all $E$ edges globally).
- **Auxiliary Space:** **$O(V + E)$** (Edge list + DSU arrays).
- **Underlying Engine:** Disjoint Set Union (DSU) with Path Compression + Edge Array.
- **Ideal For:** Sparse Graphs ($E \approx V$).

## Prim’s MST Algorithm
- **Time Complexity:** **$O((V + E) \log V)$** with Binary Heap; **$O(V^2)$** with Matrix (Dense).
- **Auxiliary Space:** **$O(V)$**
- **Underlying Engine:** Min-Priority Queue + Adjacency List.
- **Ideal For:** Dense Graphs ($E \approx V^2$).

## Kahn’s Algorithm (Topological Sort)
- **Time Complexity:** **$\Theta(V + E)$**
- **Auxiliary Space:** **$O(V)$** (In-degree array + Queue).
- **Underlying Engine:** FIFO Queue + In-Degree Array.
- **Cycle Detection:** If topological order output has $< V$ nodes, graph has a **Cycle**.

## Disjoint Set Union (DSU)
- **Time Complexity:** **Amortized $O(\alpha(N)) \approx O(1)$** per `find`/`union` operation ($\alpha(N) \le 4$).
- **Auxiliary Space:** **$O(N)$** (`parent` and `rank`/`size` arrays).
- **Engine:** Path Compression + Union by Rank/Size.

## Tarjan’s Bridge & Articulation Points
- **Time Complexity:** **$\Theta(V + E)$** (Single-pass DFS).
- **Auxiliary Space:** **$O(V)$** (`disc[]`, `low[]` arrays + stack).
- **Bridge Condition:** Edge $(u, v)$ is a bridge iff **`low[v] > disc[u]`**.

## Kosaraju’s SCC Algorithm
- **Time Complexity:** **$\Theta(V + E)$** (Two DFS passes: Pass 1 on $G$, Pass 2 on Transpose $G^T$).
- **Auxiliary Space:** **$O(V + E)$** (Transposed graph + finish stack).

---

# 2. Sorting & Selection Algorithms Master Reference

## Merge Sort
- **Time Complexity:** Best: $O(N \log N)$, Average: $O(N \log N)$, Worst: $O(N \log N)$.
- **Auxiliary Space:** **$O(N)$** (Merge buffer).
- **Stability:** **Stable**.

## Quick Sort
- **Time Complexity:** Best: $O(N \log N)$, Average: $O(N \log N)$, Worst: $O(N^2)$.
- **Auxiliary Space:** **$O(\log N)$** (Call stack).
- **Stability:** **Unstable**.

## Heap Sort
- **Time Complexity:** Best: $O(N \log N)$, Average: $O(N \log N)$, Worst: $O(N \log N)$.
- **Auxiliary Space:** **$O(1)$ Strictly In-Place**.
- **Stability:** **Unstable**.

## QuickSelect (Hoare's Selection for $K$-th Smallest Element)
- **Time Complexity:** **Average: $O(N)$ Linear Time**, Worst-Case: $O(N^2)$.
- **Auxiliary Space:** **$O(1)$ Auxiliary Space** ($O(\log N)$ stack recursion).
- **Mechanics:** Partition array around pivot. Recurse into ONLY ONE partition containing index $K$.

## Counting Sort
- **Time Complexity:** **$O(N + K)$** ($K$ is integer range).
- **Auxiliary Space:** **$O(N + K)$**. Stable non-comparison sort.

## Radix Sort
- **Time Complexity:** **$O(d \cdot (N + b))$** ($d$ digits, base $b$).
- **Auxiliary Space:** **$O(N + b)$**.

---

# 3. Searching & String Algorithms Master Reference

## Binary Search
- **Time Complexity:** **$O(\log N)$**
- **Auxiliary Space:** **$O(1)$**

## KMP (Knuth-Morris-Pratt Substring Search)
- **Time Complexity:** **$O(N + M)$** ($N = |T|, M = |P|$).
- **Auxiliary Space:** **$O(M)$** (LPS Prefix Table array).
- **Mechanics:** Computes `lps[i]` storing longest proper prefix which is also a suffix. Bypasses redundant character re-comparisons.

## Rabin-Karp Algorithm
- **Time Complexity:** Average: **$O(N + M)$**, Worst-Case: $O(N \cdot M)$ (Hash collisions).
- **Auxiliary Space:** **$O(1)$**
- **Mechanics:** Rolling Polynomial Hash: $\text{Hash}_{\text{new}} = (\text{Hash}_{\text{old}} - c_{\text{out}} B^{M-1}) B + c_{\text{in}} \pmod P$.

## Z-Algorithm
- **Time Complexity:** **$O(N)$ Linear Time**
- **Auxiliary Space:** **$O(N)$** ($Z$-array storing longest common prefix between $S$ and suffix $S[i \dots N-1]$).

## Manacher’s Algorithm (Longest Palindromic Substring)
- **Time Complexity:** **$O(N)$ Linear Time**
- **Auxiliary Space:** **$O(N)$** (Palindrome radius array $P[i]$).

---

# 4. Tree & Range Query Algorithms Master Reference

## Segment Tree
- **Time Complexity:** Build: $O(N)$, Point Update: $O(\log N)$, Range Query: $O(\log N)$, Range Update (Lazy Prop): $O(\log N)$.
- **Auxiliary Space:** **$O(N)$** (Tree array size $4N$).

## Fenwick Tree (Binary Indexed Tree - BIT)
- **Time Complexity:** Build: $O(N)$, Point Update: $O(\log N)$, Prefix Sum Query: $O(\log N)$.
- **Auxiliary Space:** **$O(N)$** (1D Array size $N+1$). Bitwise index math: `idx += idx & -idx`.

## Sparse Table (Static Range Minimum Queries - RMQ)
- **Time Complexity:** Precomputation: **$O(N \log N)$**, Range Query: **Strict $O(1)$**!
- **Auxiliary Space:** **$O(N \log N)$** (2D array `st[N][logN]`).
- **Mechanics:** Query range $[L, R]$ uses $k = \lfloor \log_2(R - L + 1) \rfloor$: $\min(\text{st}[L][k], \text{st}[R - 2^k + 1][k])$. Works ONLY for idempotent operations ($\min, \max, \gcd$).

---

# 5. Dynamic Programming & Number Theory Master Reference

## Sieve of Eratosthenes (Prime Generation)
- **Time Complexity:** **$O(N \log \log N) \approx O(N)$**
- **Auxiliary Space:** **$O(N)$** (Boolean prime array).

## Euclidean Algorithm (GCD)
- **Time Complexity:** **$O(\log(\min(A, B)))$** (By Lame's Theorem).
- **Auxiliary Space:** $O(1)$ iterative (`while(b) { a %= b; swap(a, b); }`).

## 0/1 Knapsack Problem
- **Time Complexity:** **$O(N \cdot W)$** (Pseudo-polynomial time where $W$ is capacity).
- **Auxiliary Space:** **$O(W)$** (1D DP array updated backwards).

## Longest Common Subsequence (LCS)
- **Time Complexity:** **$O(M \cdot N)$**
- **Auxiliary Space:** **$O(M \cdot N)$** (Compressible to $O(\min(M, N))$ space).

---

# 6. Top FAANG Interview Flashcard Q&A

> 🎴 **Card 1: "Why does QuickSelect run in $O(N)$ average time while QuickSort runs in $O(N \log N)$?"**  
> **Answer:** QuickSort recurses into **BOTH** left and right partitions ($N + N/2 + N/4 + \dots = 2N \log N$). QuickSelect recurses into **ONLY ONE** partition ($N + N/2 + N/4 + \dots = N \sum (1/2)^i = 2N = O(N)$).

> 🎴 **Card 2: "Why is Dijkstra's algorithm greedy, and why does it fail on negative edges?"**  
> **Answer:** Dijkstra greedily pops the smallest unvisited distance and marks it as permanently finalized. If negative edges exist, a longer path popped earlier could be rendered shorter later by a negative edge, breaking the finalization assumption.

> 🎴 **Card 3: "How does Sparse Table achieve $O(1)$ range queries, and why doesn't it work for Range Sum?"**  
> **Answer:** It overlaps two precomputed power-of-2 ranges covering $[L, R]$. For idempotent functions ($\min(x, x) = x$), overlapping elements do not alter the result ($\min$ is safe). For sum, overlapping elements would be double-counted, ruining the sum.

> 🎴 **Card 4: "What is the difference between Prim's and Kruskal's MST algorithms?"**  
> **Answer:** Kruskal's builds MST by processing global edges in ascending order using DSU ($O(E \log E)$, best for sparse graphs). Prim's builds MST by growing a single connected component using a min-heap ($O((V+E) \log V)$, best for dense graphs).

> 🎴 **Card 5: "Why is DSU time complexity $O(\alpha(N))$ and not $O(1)$?"**  
> **Answer:** Because even with path compression and union by rank, tree depth is bounded by the Inverse Ackermann function $\alpha(N)$. Since $\alpha(N) \le 4$ for all $N < 10^{80}$ (atoms in universe), it is practically constant $O(1)$.
