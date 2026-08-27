# Master Guide 03: Algorithms, Design Paradigms & System-Adjacent Theory

> **Focus:** Sorting & Searching Mechanics, Theoretical Decision Trees, Algorithm Design Paradigms (Divide & Conquer, Greedy, Dynamic Programming Patterns, Backtracking), and Real-World Systems Theory (CPU Cache Locality, Memory Hierarchy, B+ Trees vs LSM-Trees, Bloom Filters, Latency Constants).
> 
> *Designed to crush top-tier FAANG/HFT technical interviews without missing a single bit of detail.*

---

# Table of Contents
1. [The $\Omega(N \log N)$ Decision Tree Lower Bound](#1-the-decision-tree-lower-bound)
2. [Comparison-Based Sorting Algorithms](#2-comparison-based-sorting-algorithms)
3. [Non-Comparison Linear Sorting Algorithms](#3-non-comparison-linear-sorting-algorithms)
4. [Master Sorting Comparison Matrix](#4-master-sorting-comparison-matrix)
5. [Binary Search & Monotonic Predicates](#5-binary-search--monotonic-predicates)
6. [Divide & Conquer Paradigm](#6-divide--conquer-paradigm)
7. [Greedy Algorithms Paradigm](#7-greedy-algorithms-paradigm)
8. [Dynamic Programming Foundations & Master Patterns](#8-dynamic-programming-foundations--master-patterns)
9. [Backtracking & State-Space Search](#9-backtracking--state-space-search)
10. [Two Pointers & Sliding Window Mechanics](#10-two-pointers--sliding-window-mechanics)
11. [Big-O Realities & Constant Factors](#11-big-o-realities--constant-factors)
12. [Latency Numbers Every Engineer Must Know](#12-latency-numbers-every-engineer-must-know)
13. [CPU Cache Locality & Hardware Architecture](#13-cpu-cache-locality--hardware-architecture)
14. [Database Storage Engines: B+ Trees vs. LSM-Trees](#14-database-storage-engines-b-trees-vs-lsm-trees)
15. [Probabilistic Data Structures: Bloom Filters](#15-probabilistic-data-structures-bloom-filters)
16. [Interview Quick-Fire Recap](#16-interview-quick-fire-recap)

---

# 1. The $\Omega(N \log N)$ Decision Tree Lower Bound

A fundamental theorem states that **no comparison-based sorting algorithm can achieve a worst-case time complexity faster than $\Omega(N \log N)$**.

```
Decision Tree for Sorting 3 Elements [ a, b, c ] (Total Permutations = 3! = 6):

                               [ a < b ? ]
                              /           \
                     (YES)  /               \  (NO)
                     [ b < c ? ]         [ a < c ? ]
                    /          \         /          \
            (YES) /      (NO)    \     (YES) /  (NO)  \
             [a,b,c]    [ a < c ? ]   [b,a,c]   [b,c,a]
                        /         \
                      (YES)       (NO)
                     [a,c,b]     [c,a,b]
```

### Mathematical Proof via Decision Trees
1. An array of $N$ distinct elements has **$N!$ possible permutations**.
2. A comparison sort is a binary decision tree where each internal node is a comparison ($a_i \le a_j$) and each leaf represents a unique sorted permutation.
3. The decision tree must have at least $N!$ leaves: $\text{Leaves } L \ge N!$.
4. A binary tree of height $h$ has at most $2^h$ leaves:
   $$2^h \ge L \ge N! \implies h \ge \log_2(N!)$$
5. By **Stirling's Approximation** ($\ln(N!) \approx N \ln N - N$):
   $$h \ge \log_2(N!) = \sum_{i=1}^N \log_2 i \ge \sum_{i=N/2}^N \log_2(N/2) = \frac{N}{2} \log_2\left(\frac{N}{2}\right) = \mathbf{\Omega(N \log N)}$$

---

# 2. Comparison-Based Sorting Algorithms

### A. Merge Sort
- **Mechanics:** Recursively divides array into two halves, sorts each half, and merges sorted halves.
- **Complexity:** **$\Theta(N \log N)$ across Best, Average, and Worst cases**.
- **Space:** **$O(N)$ Auxiliary Space** for merge buffer.
- **Stability:** **Strictly Stable** (using `<=` preserves original left array order).

### B. Quick Sort
- **Mechanics:** Partitions array around a pivot element into elements $\le \text{pivot}$ and $> \text{pivot}$, recursing on both partitions.
- **Complexity:** **Average $\Theta(N \log N)$**, **Worst-Case $O(N^2)$**.
- **Space:** $O(\log N)$ stack space on average ($O(N)$ worst-case).
- **Stability:** **Unstable** (long-distance swaps disrupt order).

```cpp
// Hoare's Partitioning Scheme (3x fewer swaps than Lomuto)
int hoarePartition(vector<int>& arr, int low, int high) {
    int pivot = arr[low + (high - low) / 2];
    int i = low - 1, j = high + 1;
    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}
```

### C. Heap Sort
- **Mechanics:** Builds Max-Heap in $O(N)$ linear time, repeatedly swaps root to end and calls `heapifyDown` ($O(\log N)$).
- **Complexity:** **$\Theta(N \log N)$ across all cases**.
- **Space:** **$O(1)$ Strictly In-Place**.
- **Stability:** **Unstable**.

### D. Production Hybrids: Introsort & Timsort
- **Introsort (`std::sort` in C++):** QuickSort + switches to HeapSort if recursion depth exceeds $2 \log_2 N$ (preventing $O(N^2)$ worst case) + switches to InsertionSort for small chunks ($N \le 16$).
- **Timsort (Python / Java `Arrays.sort(Object[])`):** Hybrid MergeSort + InsertionSort. Detects natural sorted runs, runs in **$O(N)$ time on pre-sorted data**, and is **Stable**.

---

# 3. Non-Comparison Linear Sorting Algorithms

Bypasses $\Omega(N \log N)$ decision tree bound by assuming key properties.

- **Counting Sort:** Assumes keys are integers in range $[0, K]$. **Time: $O(N + K)$, Space: $O(N + K)$**.
- **Radix Sort:** Sorts $d$-digit numbers base $b$ using stable Counting Sort on each digit. **Time: $O(d \cdot (N + b))$, Space: $O(N + b)$**.
- **Bucket Sort:** Distributes elements into $K$ buckets over $[0.0, 1.0)$. **Average Time: $O(N + K)$**.

---

# 4. Master Sorting Comparison Matrix

| Algorithm | Best Time | Average Time | Worst Time | Space | Stable? | In-Place? |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Selection Sort** | $O(N^2)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | No | Yes |
| **Bubble Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Yes | Yes |
| **Insertion Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Yes | Yes |
| **Merge Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(N)$ | Yes | No |
| **Quick Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N^2)$ | $O(\log N)$| No | Yes |
| **Heap Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(1)$ | No | Yes |
| **Counting Sort** | $O(N + K)$ | $O(N + K)$ | $O(N + K)$ | $O(N + K)$| Yes | No |
| **Radix Sort** | $O(d(N+b))$ | $O(d(N+b))$ | $O(d(N+b))$ | $O(N + b)$ | Yes | No |

---

# 5. Binary Search & Monotonic Predicates

Binary search finds elements or optimal thresholds in **$O(\log N)$ time** on monotonic domains.

```cpp
// Lower Bound (First element >= target)
int lowerBound(const vector<int>& nums, int target) {
    int l = 0, r = nums.size();
    while (l < r) {
        int m = l + (r - l) / 2;
        if (nums[m] >= target) r = m;
        else l = m + 1;
    }
    return l;
}

// Binary Search on Answer (Monotonic Predicate Function P(x))
long long binarySearchOnAnswer(long long low, long long high) {
    long long ans = high;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (isValidPredicate(mid)) {
            ans = mid;
            high = mid - 1; // Try to find smaller valid answer
        } else {
            low = mid + 1;
        }
    }
    return ans;
}
```

---

# 6. Divide & Conquer Paradigm

Divides problem into $a$ independent subproblems of size $N/b$, solves recursively, and combines results.

- **Fast Binary Exponentiation ($O(\log N)$):**
  $$x^n = \begin{cases} (x^{n/2})^2 & \text{if } n \text{ is even} \\ x \cdot (x^{(n-1)/2})^2 & \text{if } n \text{ is odd} \end{cases}$$

```cpp
long long power(long long x, long long n, long long mod) {
    long long res = 1; x %= mod;
    while (n > 0) {
        if (n & 1) res = (res * x) % mod;
        x = (x * x) % mod;
        n >>= 1;
    }
    return res;
}
```

---

# 7. Greedy Algorithms Paradigm

Makes the **locally optimal choice** at each step without backtracking. Requires **Greedy Choice Property** and **Optimal Substructure**.

- **Interval Scheduling (Activity Selection):** Sort intervals by **End Time ascending**. Always pick the activity that finishes earliest!
- **Greedy vs DP Rule:** Fractional Knapsack $\implies$ Greedy works; 0/1 Knapsack $\implies$ Greedy FAILS (must use DP).

---

# 8. Dynamic Programming Foundations & Master Patterns

DP is applicable when a problem exhibits **Optimal Substructure** and **Overlapping Subproblems**.

- **Memoization (Top-Down):** Recursion with subproblem result caching.
- **Tabulation (Bottom-Up):** Iteration filling DP table from base cases up to $N$.

### Core DP Patterns Cheat Sheet
1. **0/1 Knapsack:** `dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i])` (Iterate budget backwards!).
2. **Unbounded Knapsack / Combinations:** Iterate budget forwards.
3. **Longest Common Subsequence (LCS):** `dp[i][j] = 1 + dp[i-1][j-1]` if match, else `max(dp[i-1][j], dp[i][j-1])`.
4. **Interval DP (Matrix Chain / Game Theory):** Outer loop over subproblem range length `len`.

---

# 9. Backtracking & State-Space Search

Explores a state-space tree recursively, **pruning** invalid branches early.

```cpp
// Backtracking Template
void backtrack(State& state, int choices) {
    if (isGoal(state)) { recordSolution(state); return; }
    for (auto& choice : getValidChoices(state)) {
        makeChoice(state, choice);
        backtrack(state, choices);
        undoChoice(state, choice); // Backtrack!
    }
}
```

---

# 10. Two Pointers & Sliding Window Mechanics

- **Two Pointers:** Opposite squeezes on sorted arrays ($O(N)$ time, $O(1)$ space).
- **Sliding Window:** Expand `right`, contract `left` when window constraint is violated.

---

# 11. Big-O Realities & Constant Factors

- For small arrays ($N \le 16$), **Insertion Sort beats QuickSort/MergeSort** due to low constant factors and tight cache loops.
- **Space-Time Trade-offs:** Memoization trades RAM for CPU time; Bloom filters trade probabilistic accuracy for tiny space.

---

# 12. Latency Numbers Every Engineer Must Know

```
+-----------------------------------------------------------------------------------------+
| OPERATION                                | HUMAN SCALED TIME (If 1 CPU Cycle = 1 sec)   | ACTUAL TIME |
+-----------------------------------------------------------------------------------------+
| 1 CPU L1 Cache Reference                 | 0.5 seconds                                  | 0.5 ns      |
| Branch Misprediction Penalty             | 5 seconds                                    | 5 ns        |
| 1 CPU L2 Cache Reference                 | 7 seconds                                    | 7 ns        |
| Mutex Lock / Unlock                      | 25 seconds                                   | 25 ns       |
| Main Memory (RAM) Access                 | 1.5 minutes                                  | 100 ns      |
| Read 1 MB sequentially from RAM          | 4 minutes                                    | 3,000 ns    |
| Read 1 MB sequentially from NVMe SSD     | 5.5 hours                                    | 50,000 ns   |
| Random Seek on NVMe SSD                  | 1.7 days                                     | 150 µs      |
| Random Seek on Rotational HDD            | 4 months (10,000,000x slower than L1 cache!) | 10,000 µs   |
| Network Roundtrip (CA to Netherlands)    | 5 years                                      | 150,000 µs  |
+-----------------------------------------------------------------------------------------+
```

---

# 13. CPU Cache Locality & Hardware Architecture

- **64-Byte Cache Lines:** CPUs fetch 64 bytes at a time into L1 cache. Sequential array reads achieve near 100% cache hits.
- **False Sharing:** Two threads writing to different variables on the same 64-byte cache line cause severe cache line ping-ponging. Fix: Align variables via `alignas(64)`.

---

# 14. Database Storage Engines: B+ Trees vs. LSM-Trees

```
+-----------------------------------------------------------------------------------------+
| B+ TREE (Read-Heavy Engines: PostgreSQL, MySQL InnoDB)                                 |
| - High Fan-Out (Order M = 500+). Tree height is 3 to 4. Reads take 3 disk page seeks.  |
| - Leaf nodes are doubly linked for blazingly fast range queries.                       |
+-----------------------------------------------------------------------------------------+
| LSM-TREE (Log-Structured Merge Tree - Write-Heavy Engines: RocksDB, Cassandra)          |
| - Writes go to in-memory MemTable (SSTable) and append-only WAL on disk (O(1) sequential)|
| - Background compaction merges SSTable files on disk. Range reads use Bloom Filters.    |
+-----------------------------------------------------------------------------------------+
```

---

# 15. Probabilistic Data Structures: Bloom Filters

A space-efficient probabilistic data structure that tests set membership:
- **Guarantees:** **NO False Negatives** ("Definitely Not in Set" is 100% true).
- **Caveat:** **Possible False Positives** ("Possibly in Set").
- **Structure:** Bit array of size $M$ + $K$ independent hash functions.
- **Operations:** Insert sets $K$ bits to 1; Query checks if all $K$ bits are 1.

---

# 16. Interview Quick-Fire Recap

1. **Comparison Sort Bound:** Decision tree height $h \ge \log_2(N!) = \Omega(N \log N)$.
2. **QuickSort vs MergeSort:** QuickSort has superior spatial cache locality and operates in-place without $O(N)$ allocations.
3. **Timsort Secret:** Detects pre-sorted runs and uses insertion sort on small chunks for $O(N)$ best-case runtime.
4. **Binary Search Predicate:** Always define monotonic property $P(x)$ returning `bool`.
5. **False Sharing:** Two cores updating adjacent variables in one 64-byte cache line invalidate each other's L1 cache. Fix with 64-byte alignment.
6. **Bloom Filter Guarantee:** Zero false negatives; eliminates 99% of unnecessary database disk reads.
