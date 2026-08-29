# Algorithms, Design Paradigms & System-Adjacent Theory — Interview Master Guide

**Target:** Google, Microsoft, Meta, Amazon, Apple, NVIDIA, Uber, Bloomberg, Atlassian, Adobe, Salesforce, Goldman Sachs, Rubrik, Databricks, etc.

**Priority:** Interview patterns > complexity > behavior > internals > implementation-specific details.

---

# 1. Sorting Algorithms & The Decision Tree Lower Bound

## Why Comparison Sort is Omega(N log N)
* `N` elements have `N!` possible permutations.
* A comparison-based sort forms a binary decision tree of height `h`.
* `2^h >= N!` ==> `h >= log2(N!) = Omega(N log N)` (via Stirling's approximation).
* **Takeaway:** No comparison sort can ever beat `O(N log N)` in the worst case.

## Comparison Sorting Matrix

| Algorithm | Best | Average | Worst | Space | Stable? | Key Interview Insight |
|---|---|---|---|---|---|---|
| **Merge Sort** | `O(N log N)` | `O(N log N)` | `O(N log N)` | `O(N)` | **Yes** | Divide & Conquer; preferred for Linked Lists (no random access needed). |
| **Quick Sort** | `O(N log N)` | `O(N log N)` | `O(N^2)` | `O(log N)` | **No** | Cache-friendly in-place partitioning; worst case occurs on already sorted array with naive pivot. |
| **Heap Sort** | `O(N log N)` | `O(N log N)` | `O(N log N)` | `O(1)` | **No** | In-place, guaranteed `O(N log N)`, but poorer cache locality than QuickSort. |
| **Insertion Sort**| `O(N)` | `O(N^2)` | `O(N^2)` | `O(1)` | **Yes** | Blazing fast on small (`N <= 16`) or nearly-sorted arrays. Used in `std::sort` (Introsort). |

## Non-Comparison Sorting (O(N))
* **Counting Sort:** `O(N + K)` where `K` is the range of keys. Optimal when `K ~ N`.
* **Radix Sort:** `O(d * (N + b))` sorting digit-by-digit from LSD to MSD using stable counting sort.

---

# 2. Binary Search & Monotonic Predicates

## The Universal Binary Search on Answer Template
Whenever looking for the **minimum valid** or **maximum valid** value in a monotonic search space:

```cpp
int low = minPossible, high = maxPossible, ans = -1;
while (low <= high) {
    int mid = low + (high - low) / 2;
    if (isValid(mid)) {
        ans = mid;
        high = mid - 1; // Or low = mid + 1 for maximization
    } else {
        low = mid + 1;  // Or high = mid - 1
    }
}
return ans;
```

### Classic Interview Problems
* Capacity To Ship Packages Within D Days (LC 1011)
* Koko Eating Bananas (LC 875)
* Split Array Largest Sum (LC 410)
* Search in Rotated Sorted Array (LC 33 - Identify which half is sorted!).

---

# 3. Two Pointers & Sliding Window

## The 4 Archetypes

```text
1. Inward Converging:   [ Left --->        <--- Right ]  (2Sum, Container With Most Water)
2. Fast & Slow:         [ Slow ->  Fast ------>       ]  (Linked list cycle, Remove duplicates)
3. Fixed Window:        [ <--- Size K ---> ] --------->   (Max sum subarray of size K)
4. Variable Window:     [ Left ...... Right ] -------->  (Longest substring without repeats)
```

### Exact-K Subarrays Reduction (Universal Trick)
`Count of Subarrays with Exactly K = atMost(K) - atMost(K - 1)`

Applies to: Subarrays with `K` Different Integers (LC 992), Binary Subarrays With Sum (LC 930), Nice Subarrays (LC 1248).

---

# 4. Greedy Algorithms Paradigm

## The Greedy Choice Property
* Make the locally optimal choice at each step; prove it leads to a globally optimal solution.
* **If a choice requires regret or backtracking ==> Dynamic Programming, not Greedy.**

### Interval Scheduling Golden Rules
* **Max Non-Overlapping Intervals (LC 435):** Sort by **End Time** (earliest finish leaves most room).
* **Merge Overlapping Intervals (LC 56):** Sort by **Start Time** (extend `end = max(end, curr.end)`).
* **Min Meeting Rooms / Platforms (LC 253):** Chronological Two-Pointer Sweep of start and end times.

---

# 5. Dynamic Programming Master Mental Model

## When is a Problem DP?
1. **Optimal Substructure:** Optimal solution of size `N` is built from optimal solutions of subproblems (`N-1, N-2, ...`).
2. **Overlapping Subproblems:** The same subproblems are solved repeatedly across recursive branches.

## The 5 Core DP Archetypes

```text
1. 1D Linear DP:       dp[i] = max(dp[i-1], dp[i-2] + val)      (House Robber, Climb Stairs, Decode Ways)
2. 2D Grid DP:         dp[i][j] = val + min(dp[i-1][j], dp[i][j-1]) (Unique Paths, Min Path Sum, Maximal Square)
3. String / LCS DP:    dp[i][j] = s1[i]==s2[j] ? 1+dp[i-1][j-1] : max(...) (LCS, Edit Distance, Interleaving)
4. Interval DP:        dp[i][j] = min(dp[i][k] + dp[k+1][j] + cost) (Burst Balloons, Matrix Chain Mult)
5. Knapsack Suite:     0/1 Knapsack (Backwards loop), Unbounded Knapsack (Forwards loop)
```

---

# 6. Backtracking & State-Space Search

```text
The Standard Backtracking Skeleton:
void backtrack(State& state, int startIdx) {
    if (isGoal(state)) {
        result.push_back(state);
        return;
    }
    for (int i = startIdx; i < choices.size(); i++) {
        if (!isValid(choices[i])) continue; // PRUNING
        
        makeChoice(state, choices[i]);      // CHOOSE
        backtrack(state, i + 1);             // EXPLORE
        undoChoice(state, choices[i]);      // UNCHOOSE (Backtrack)
    }
}
```

---

# 7. Real-World Systems & Hardware Theory

## Latency Numbers Every Engineer Must Know

| Operation | Latency | Real-World Scale Analogy |
|---|---|---|
| **L1 Cache Reference** | **0.5 ns** | 1 heart beat |
| **Branch Mispredict** | **5 ns** | 10 heart beats |
| **L2 Cache Reference** | **7 ns** | 14 heart beats |
| **Mutex Lock / Unlock** | **25 ns** | 50 heart beats |
| **Main Memory (RAM) Access** | **100 ns** | 3.3 minutes |
| **SSD Random Read** | **150 us** | 3.5 days |
| **Data Center Roundtrip (LAN)** | **500 us** | 1.6 weeks |
| **HDD Seek** | **10 ms** | 7.7 months |
| **Internet Packet (SF to NYC)** | **40 ms** | 2.5 years |

### Spatial & Temporal Cache Locality
* **Temporal Locality:** If a memory address is accessed, it will likely be accessed again soon (keep in L1/L2 cache).
* **Spatial Locality:** If a memory address is accessed, nearby addresses will likely be accessed soon (fetch full 64-byte cache line).
* **Why Vector Outperforms Linked List:** Sequential array elements are contiguous ==> 100% cache line hits ==> no pointer chasing stalls.

---

# 8. Storage Engines: B+ Trees vs LSM-Trees

```text
B+ Tree (Read-Heavy / In-Place Updates):
[ Internal Node: Routing Keys ] ---> [ Leaf Node: [Key, Val] <-> [Key, Val] ] (Linked Leaf Chain)

LSM-Tree (Write-Heavy / Append-Only):
Write -> [ In-Memory MemTable (SkipList) ] ---> Flush to Disk ---> [ SSTable (Immutable Sorted File) ]
```

| Dimension | B+ Tree (Postgres, MySQL InnoDB) | LSM-Tree (Cassandra, RocksDB, BigTable) |
|---|---|---|
| **Primary Architecture** | Self-balancing tree with linked leaves | In-memory MemTable + On-disk SSTables |
| **Write Cost** | High (Random in-place page writes) | **Low (Sequential append-only writes)** |
| **Read Cost** | **Low (O(log N) point lookups)** | Higher (Check MemTable + Bloom Filters + SSTables) |
| **Range Queries** | Excellent (Scan leaf node linked list) | Merges multiple SSTables |

---

# 9. Probabilistic Data Structures: Bloom Filters

## Architecture
* A bit array of size `M` initialized to all 0s + `k` independent hash functions.
* **Insertion:** Hash key with `k` hash functions; set all `k` bit positions to 1.
* **Membership Query:**
  - If **ANY** of the `k` bits is 0 ==> **Definitively NOT in Set** (Zero False Negatives).
  - If **ALL** `k` bits are 1 ==> **PROBABLY in Set** (Possible False Positive).
* **Use Cases:** Database read optimization (skip checking SSTable if Bloom filter returns false), CDN caching, URL blacklist checking.

---

# 10. Top Interview Questions

## Tier 1 — Must Know
1. Why is comparison-based sorting bounded by `Omega(N log N)`?
2. Compare QuickSort vs MergeSort across stability, in-place behavior, and cache locality.
3. How does the "Binary Search on Answer" pattern work?
4. When does a Greedy approach fail, requiring Dynamic Programming?
5. What is the difference between Memoization (Top-Down) and Tabulation (Bottom-Up)?
6. Explain the Exact-K reduction technique in sliding window problems.
7. Why is row-major matrix traversal faster than column-major in C++?

## Tier 2 — Strong Candidate
8. Explain the Master Theorem and how it applies to Merge Sort and Binary Search.
9. How does Introsort work in standard libraries (C++ `std::sort`)?
10. How does a Bloom Filter guarantee zero false negatives?
11. Compare B+ Trees vs LSM-Trees for read vs write heavy database workloads.
12. What causes branch misprediction in hardware and how does sorting input reduce it?

## Tier 3 — Advanced / Systems
13. How does Quickselect find the `K`-th largest element in `O(N)` average time?
14. Explain memory hierarchy latency differences between L1 cache, RAM, and Disk.
15. What are write amplification and compaction in LSM-tree based databases?

---

# 11. Pattern Recognition Guide

| Clue in Problem Statement | Target Paradigm / Technique |
|---|---|
| Find min/max value satisfying a monotonic condition | **Binary Search on Answer** |
| Subarray sum, substring with at most / exact K characters | **Sliding Window / Two Pointers** |
| Overlapping subproblems, choice sequence, max/min cost | **Dynamic Programming** |
| All permutations, combinations, N-Queens, grid search | **Backtracking** |
| Interval merging, activity selection, non-overlapping tasks | **Greedy (Sort by start/end)** |
| Fast approximate set membership with minimal RAM | **Bloom Filter** |
| High write throughput database storage | **LSM-Tree / SSTables** |

---

# 12. Interview Priority

## P0 — Absolutely Master
```text
Binary Search on Answer predicate template
Dynamic Programming 5 Core Archetypes
Sliding Window (Fixed, Variable, Exact-K Reduction)
Two Pointers (Inward, Fast & Slow)
QuickSort vs MergeSort mechanics & trade-offs
Greedy Interval Scheduling rules
```

## P1 — Strongly Know
```text
Backtracking standard template & pruning
Kadane's algorithm & variants
Hardware cache locality (Row vs Col major)
Quickselect O(N) Kth element finding
Counting sort & Radix sort linear mechanics
```

## P2 — Know Conceptually
```text
B+ Trees vs LSM-Trees (Database storage trade-offs)
Bloom Filters mechanics & False Positive property
Latency numbers hierarchy (L1 vs RAM vs Disk)
```

## P3 — Don't Waste Time Memorizing
```text
Mathematical proofs of Stirling's approximation
Bit-level details of IEEE 754 floating point mantissas
Obscure sorting algorithms (TimSort internals, ShellSort gap sequences)
```
