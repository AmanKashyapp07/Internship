# Algorithm Design Paradigms, Computational Complexity & Systems Theory

> **Scope:** Comparison Sorting Lower Bound Proofs, Linear-Time Sorting, Monotonic Binary Search Predicates, Two-Pointer & Sliding Window Mathematical Reductions, Greedy Choice Invariants, Dynamic Programming Substructure & Formulations, Backtracking State-Space Pruning, Hardware Memory Hierarchy Latency & Cache Locality, Storage Engine Topologies (B+ Trees vs. LSM-Trees), and Probabilistic Set Membership (Bloom Filters).

---

# Table of Contents
1. [Comparison Sorting Lower Bound & Sorting Taxonomy](#1-comparison-sorting-lower-bound--sorting-taxonomy)
2. [Monotonic Predicates & Binary Search on Solution Spaces](#2-monotonic-predicates--binary-search-on-solution-spaces)
3. [Two-Pointer & Sliding Window Formalisms](#3-two-pointer--sliding-window-formalisms)
4. [Greedy Choice Property & Interval Scheduling Proofs](#4-greedy-choice-property--interval-scheduling-proofs)
5. [Dynamic Programming Mathematical Formulations](#5-dynamic-programming-mathematical-formulations)
6. [Backtracking State-Space Traversal & Branch Pruning](#6-backtracking-state-space-traversal--branch-pruning)
7. [Hardware Memory Hierarchy & Cache Locality Physics](#7-hardware-memory-hierarchy--cache-locality-physics)
8. [Storage Engine Topologies: B+ Trees vs. LSM-Trees](#8-storage-engine-topologies-b-trees-vs-lsm-trees)
9. [Probabilistic Data Structures: Bloom Filter Theory](#9-probabilistic-data-structures-bloom-filter-theory)
10. [Core Theoretical Summary Principles](#10-core-theoretical-summary-principles)

---

# 1. Comparison Sorting Lower Bound & Sorting Taxonomy

### The $\Omega(N \log N)$ Decision Tree Lower Bound Proof
1. A comparison-based sorting algorithm determines the relative order of elements strictly via pairwise comparisons ($a_i \le a_j$).
2. For an array of size $N$, there are $N!$ distinct possible permutations.
3. Every execution path corresponds to a root-to-leaf path in a binary decision tree of height $h$.
4. A binary tree of height $h$ contains at most $2^h$ leaves:
   $$2^h \ge N! \implies h \ge \log_2(N!)$$
5. Applying Stirling's Approximation ($\ln(N!) \approx N \ln N - N$):
   $$h \ge \Omega(N \log N)$$
- **Theorem:** No comparison-based sorting algorithm can achieve an asymptotic worst-case time complexity strictly better than $\Omega(N \log N)$.

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM      | BEST TIME    | AVERAGE TIME | WORST TIME   | AUX SPACE | STABILITY | CACHE LOCALITY   |
+----------------------------------------------------------------------------------------------------+
| Merge Sort     | O(N log N)   | O(N log N)   | O(N log N)   | O(N)      | Stable    | Moderate (Copies)|
| Quick Sort     | O(N log N)   | O(N log N)   | O(N^2)       | O(log N)  | Unstable  | Optimal (In-Place|
| Heap Sort      | O(N log N)   | O(N log N)   | O(N log N)   | O(1)      | Unstable  | Poor (Random hops|
| Insertion Sort | O(N)         | O(N^2)       | O(N^2)       | O(1)      | Stable    | Optimal (Local)  |
| Counting Sort  | O(N + K)     | O(N + K)     | O(N + K)     | O(K)      | Stable    | High             |
| Radix Sort     | O(d * (N+b)) | O(d * (N+b)) | O(d * (N+b)) | O(N + b)  | Stable    | High             |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Monotonic Predicates & Binary Search on Solution Spaces

Binary search applies to any discrete or continuous domain mapped through a monotonic boolean predicate function $f: S \to \{0, 1\}$.

```
Monotonicity Property:
Search Domain: [  x0,   x1,   x2,   x3,   x4,   x5,   x6,   x7  ]
Predicate f(x): [   0,    0,    0,    0,    1,    1,    1,    1  ]
                                            ^
                                     First True (Infimum)
```

### Algorithmic Template:
```cpp
int findMinimumFeasible(int low, int high) {
    int result = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2; // Prevents 32-bit signed integer overflow
        if (isValidPredicate(mid)) {
            result = mid;       // Record feasible candidate
            high = mid - 1;     // Contract search space to find smaller valid candidates
        } else {
            low = mid + 1;      // Infeasible; advance lower bound
        }
    }
    return result;
}
```

---

# 3. Two-Pointer & Sliding Window Formalisms

### The 4 Canonical Pointer Archetypes:
1. **Inward Converging:** Pointers initialize at boundaries ($L=0, R=N-1$) and converge monotonically based on sorted ordering ($O(N)$ sum checks, container bounding).
2. **Fast & Slow (Floyd's Cycle Finding):** Two pointers advancing at rates $v$ and $2v$ ($O(N)$ cycle detection in linked lists and sequence state spaces).
3. **Fixed Window:** Window of fixed width $K$ sliding across a sequence with $O(1)$ state updates upon element entry and exit.
4. **Variable Window:** Monotonically expanding right boundary ($R++$) paired with conditionally contracting left boundary ($L++$) to maintain a state invariant.

### Exact-$K$ Subarray Reduction Theorem:
For any monotonic counting condition, the number of contiguous subarrays satisfying **exactly $K$** instances of a property equals the difference between two monotonic prefix windows:
$$\text{Count}(\text{Exactly } K) = \text{Count}(\text{At Most } K) - \text{Count}(\text{At Most } K - 1)$$

---

# 4. Greedy Choice Property & Interval Scheduling Proofs

A problem exhibits the **Greedy Choice Property** if a globally optimal solution can be assembled by iteratively making locally optimal choices without backtracking:

### Interval Scheduling Optimization Rules:
1. **Maximum Non-Overlapping Intervals:** Sort intervals by **End Time** ($f_i$). Always select the compatible interval with the earliest finish time, leaving maximal remaining time capacity for subsequent selections.
2. **Interval Union Merging:** Sort intervals by **Start Time** ($s_i$). Maintain active interval $[S, E]$; expand $E = \max(E, e_{\text{curr}})$ if $s_{\text{curr}} \le E$, else finalize active segment and reset.
3. **Minimum Concurrent Resource Capacity:** Transform intervals into discrete point events $(t_{\text{start}}, +1)$ and $(t_{\text{end}}, -1)$; evaluate a prefix sum sweep over sorted timestamps to find peak concurrent concurrency.

---

# 5. Dynamic Programming Mathematical Formulations

Dynamic Programming applies to problems satisfying **Optimal Substructure** and **Overlapping Subproblems**:

```
+---------------------------------------------------------------------------------------------------+
| DP ARCHETYPE         | RECURRENCE FORMULATION EXAMPLE                                             |
+---------------------------------------------------------------------------------------------------+
| 1. 1D Linear State   | dp[i] = max(dp[i-1], dp[i-2] + value[i])                                   |
| 2. 2D Grid / Matrix  | dp[i][j] = cost[i][j] + min(dp[i-1][j], dp[i][j-1])                       |
| 3. Sequence / LCS    | dp[i][j] = (s1[i]==s2[j]) ? 1 + dp[i-1][j-1] : max(dp[i-1][j], dp[i][j-1])|
| 4. Interval DP       | dp[i][j] = min_{i <= k < j} (dp[i][k] + dp[k+1][j] + cost(i, k, j))        |
| 5. 0/1 Knapsack      | dp[w] = max(dp[w], dp[w - weight[i]] + value[i]) (Reverse inner loop)      |
| 6. Unbounded Knapsack| dp[w] = max(dp[w], dp[w - weight[i]] + value[i]) (Forward inner loop)      |
+---------------------------------------------------------------------------------------------------+
```

---

# 6. Backtracking State-Space Traversal & Branch Pruning

Backtracking traverses a combinatorial state-space tree using Depth-First Search (DFS) with programmatic branch pruning:

```
                          [ Root State ]
                         /      |       \
                   [ Choice A ] [ B ]  [ C (Pruned: Infeasible) ]
                     /    \
               [ Goal ]  [ Dead End ]
```

```cpp
void exploreStateSpace(State& state, int depth) {
    if (isGoalState(state)) {
        recordSolution(state);
        return;
    }
    for (const auto& candidate : generateCandidates(state, depth)) {
        if (!isFeasibleConstraint(state, candidate)) {
            continue; // Subtree pruning: Discards exponential search branches
        }
        applyMutation(state, candidate);     // Transition state
        exploreStateSpace(state, depth + 1); // Recurse
        revertMutation(state, candidate);    // Backtrack (Restore invariant)
    }
}
```

---

# 7. Hardware Memory Hierarchy & Cache Locality Physics

```
+---------------------------------------------------------------------------------------------------+
| HARDWARE COMPONENT                   | ACCESS LATENCY           | RELATIVE SCALE FACTOR           |
+---------------------------------------------------------------------------------------------------+
| L1 CPU Cache Reference               | 0.5 ns - 1.0 ns          | 1.0x (Baseline)                 |
| Branch Misprediction Penalty         | 3.0 ns - 5.0 ns          | 5.0x                            |
| L2 CPU Cache Reference               | 3.0 ns - 7.0 ns          | 7.0x                            |
| L3 CPU Shared Cache Reference        | 10.0 ns - 20.0 ns        | 20.0x                           |
| Main Memory DRAM Reference           | 50.0 ns - 100.0 ns       | 100.0x                          |
| Solid State Disk (SSD) I/O           | 50.0 us - 150.0 us       | 150,000.0x                      |
| Rotational Hard Drive (HDD) Seek     | 5.0 ms - 10.0 ms         | 10,000,000.0x                   |
+---------------------------------------------------------------------------------------------------+
```

### Locality Principles:
1. **Temporal Locality:** Memory addresses accessed recently are likely to be accessed again in the immediate future (retained in high-speed hardware caches).
2. **Spatial Locality:** Accessing memory address $A$ triggers hardware prefetching of contiguous cache lines (typically 64 bytes). Sequential array traversals maximize cache line utilization, whereas pointer-linked node traversals incur memory stall bubbles.

---

# 8. Storage Engine Topologies: B+ Trees vs. LSM-Trees

```
B+ Tree (In-Place Mutation / Read-Optimized):
[ Internal Routing Nodes ] ---> [ Contiguous Doubly-Linked Leaf Pages on Disk ]

Log-Structured Merge-Tree (Append-Only / Write-Optimized):
[ In-Memory MemTable (SkipList) ] --Flush--> [ L0 SSTable ] --Compaction--> [ L1 SSTable ]
```

```
+---------------------------------------------------------------------------------------------------+
| ARCHITECTURAL METRIC | B+ TREE (PostgreSQL, MySQL InnoDB)    | LSM-TREE (RocksDB, Cassandra)      |
+---------------------------------------------------------------------------------------------------+
| Storage Engine Model | Balanced multi-way tree               | Log-structured append runs         |
| Write Complexity     | High: In-place random disk writes     | Low: Sequential append to WAL/MemT |
| Read Complexity      | Optimal: O(log N) point lookup        | Variable: Checks MemTable + SSTable|
| Range Scan Mechanism | Sequential leaf node scan             | Multi-way merge across SSTables    |
| Compaction Overhead  | Page split rebalancing                | Background merge compaction runs   |
+---------------------------------------------------------------------------------------------------+
```

---

# 9. Probabilistic Data Structures: Bloom Filter Theory

A **Bloom Filter** is a space-efficient probabilistic data structure used to test set membership with zero false negatives:

```
Bit Array of Size M (Initialized to 0):
Index:  [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ]
          0   1   0   1   0   0   1   0   1   0
              ^       ^           ^       ^
              |       |           |       |
            h1(x)   h2(x)       h3(x)   h1(y)...
```

### Mathematical Invariants:
1. **Zero False Negatives:** If an element $x$ was inserted, all $k$ hash positions $h_1(x), \dots, h_k(x)$ are guaranteed to be 1. If any $h_i(x) == 0$, $x$ is **definitively not in the set**.
2. **Bounded False Positives:** If all $h_i(x) == 1$, $x$ is **probably in the set**. The false positive probability $p$ is parameterized by bit array size $M$, element count $N$, and hash count $k$:
   $$p \approx \left(1 - e^{-kN/M}\right)^k$$
   Optimal hash functions: $k = \frac{M}{N} \ln 2$.

---

# 10. Core Theoretical Summary Principles

1. **Sorting Lower Bound:** Any comparison-based sorting algorithm requires at least $\Omega(N \log N)$ worst-case operations based on decision tree height.
2. **Monotonic Binary Partitioning:** Monotonicity enables logarithmic space partitioning to locate extrema in $O(\log N)$ evaluations.
3. **Exact-$K$ Difference Invariant:** Calculating intervals matching exact criteria is solved by subtracting two monotonic bounded prefix queries.
4. **Greedy vs. DP Boundary:** Greedy algorithms require the optimal choice at step $k$ to never require retrospective revision; otherwise, dynamic programming is mandatory.
5. **Memory Hierarchy Dominance:** Hardware cache line locality (64 bytes) dictates that contiguous arrays outperform linked node structures by orders of magnitude due to cache miss stall latency.
