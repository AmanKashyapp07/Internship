# Master Problem Summary & Complexity Reference (Notebooks 1–5)

A unified reference catalog of **109 unique Data Structures & Algorithms problems** consolidated across `notebook-1.cpp` through `notebook-5.cpp` (all problems existing in `Modules/` have been deduplicated and removed). Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities.

## High-Level Problem Distribution

| Notebook | Focus Topic / Core Theme | Unique Problems Count |
| :--- | :--- | :---: |
| **Notebook 1** | Advanced Trees, Graph Algorithms, Binary Lifting & Cycle Theory | **22** |
| **Notebook 2** | Number Theory, CSES Combinatorics, Power Towers & Bit Manipulation | **24** |
| **Notebook 3** | Dynamic Programming, Knapsacks, Supersequences & Slope Trick | **22** |
| **Notebook 4** | Dynamic Intervals, Sweep-Line, Difference Arrays & Specialized Sorting | **21** |
| **Notebook 5** | System Design, Online Data Streams, Caches (LRU/TTL) & Concurrency | **20** |
| **TOTAL** | | **109** |

---

## Notebook-by-Notebook Analysis

### Notebook 1: Advanced Trees, Graph Algorithms, Binary Lifting & Cycle Theory

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Tree Centroid | Subtree Size DFS + Heavy Descent | `O(N)` | `O(N)` |
| **2** | Longest Paths & Path Counts on a DAG | Topological Sort + DP Relaxation | `O(V + E)` | `O(V + E)` |
| **3** | Tree Rerooting DP (All-Nodes Distance Sum) | 2-Pass Tree Rerooting DP | `O(N)` | `O(N)` |
| **4** | Tree Diameter & Node Eccentricities | 2-Pass Tree BFS/DFS | `O(N)` | `O(N)` |
| **5** | Functional Graph Decomposition & Queries | Floyd's Cycle + Binary Lifting | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **6** | Tree Isomorphism (Double-Hashing) | Canonical Subtree Hashing (AHU) | `O(N log N)` | `O(N)` |
| **7** | Tree Centers by Leaf Trimming | Topological Leaf Peeling (Deg 1) | `O(N)` | `O(N)` |
| **8** | Reconstruct Cycle | Predecessor Backtracking DFS/BFS | `O(V)` | `O(V)` |
| **9** | Nodes in Cycles (Topological Peeling) | In-Degree 0 Queue Cascade (Kahn) | `O(V + E)` | `O(V)` |
| **10** | Lexicographical Topological Sort | Min-Heap Kahn's BFS | `O(V log V + E)` | `O(V)` |
| **11** | Shortest Path Visiting All Nodes (LC 847) | Multi-Source Bitmask BFS | `O(V * 2^V)` | `O(V * 2^V)` |
| **12** | Maximum Sum BST in Binary Tree (LC 1373) | Post-Order Bottom-Up DFS | `O(N)` | `O(H)` |
| **13** | Remove Leaf Nodes with Target Value (LC 1325)| Post-Order Recursive Tree Pruning | `O(N)` | `O(H)` |
| **14** | Possible Path Lengths in DAG | TopoSort + 2D Reachability DP | `O((V + E) * N / 64)` | `O(V * N / 64)` |
| **15** | Max Components Tree Split with Equal Sum | Divisors + Subtree DFS Cuts | `O(N * divisors(Total))` | `O(N)` |
| **16** | Binary Lifting (LCA, K-th Ancestor) | Ancestor Doubling Table up[u][j] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **17** | Cycle Detection & Reconstruction | DFS Parent Trace / Back-Edge Check | `O(V + E)` | `O(V)` |
| **18** | Bidirectional BFS Template | Two-Ended BFS (Smaller Frontier) | `O(B^(D/2))` | `O(B^(D/2))` |
| **19** | Jump Game III (LeetCode 1306) | Graph BFS Reachability | `O(N)` | `O(N)` |
| **20** | Jump Game IV (LeetCode 1345) | BFS + Same-Value Pruning | `O(N)` | `O(N)` |
| **21** | Jump Game V (LeetCode 1340) | Memoized DFS / DAG Longest Path | `O(N * D)` | `O(N)` |
| **22** | Jump Game VII (LeetCode 1871) | Sliding Window Reachability DP | `O(N)` | `O(N)` |

---

### Notebook 2: Number Theory, CSES Combinatorics, Power Towers & Bit Manipulation

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Check Subsequence | Greedy Two-Pointer Linear Scan | `O(len(S))` | `O(1)` |
| **2** | Precompute Palindromic Subarrays Table | 2D Interval DP over Lengths | `O(N^2)` | `O(N^2)` |
| **3** | Minimum Excluded Value (MEX) | Hash Set Presence Lookup | `O(N)` | `O(N)` |
| **4** | Matrix Multiplication & Exponentiation | Binary Exponentiation on Matrices | `O(N^3 log B)` | `O(N^2)` |
| **5** | Total Area Covered by Two 2D Rectangles | Inclusion-Exclusion Geometry | `O(1)` | `O(1)` |
| **6** | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n | `O(N)` | `O(N)` |
| **7** | Exponentiation II (Power Tower — CSES 1712) | Euler's Totient + Fermat MOD-1 | `O(log B + log C)` | `O(1)` |
| **8** | Divisor Analysis (CSES 1713 / Number Theory) | Prime Factorization Powers Modulo | `O(N log MOD)` | `O(N)` |
| **9** | Maximum GCD Pair in Range (CSES 1081) | Multiples Sieve Frequency Scan | `O(M log M + N)` | `O(M)` |
| **10** | Next Prime Search (CSES 3396) | Deterministic Miller-Rabin Test | `O(k log^3 N)` | `O(1)` |
| **11** | Derangements / Christmas Party (CSES 1717) | Derangement Recurrence DP | `O(N)` | `O(N)` |
| **12** | Distributing Apples / Multinomial (CSES 1715)| Stars and Bars Formula nCr | `O(N)` build, `O(1)` query | `O(N)` |
| **13** | Creating Strings II / Stars & Bars (CSES 1716)| Multinomial Permutations Formula | `O(N)` build, `O(1)` query | `O(N)` |
| **14** | Graph Paths II / Length K Walks (CSES 1723) | Min-Plus Matrix Exponentiation | `O(V^3 log K)` | `O(V^2)` |
| **15** | Permutation Inversions / Lehmer (CSES 3397) | Inversion Table + Fenwick Tree | `O(N log N)` | `O(N)` |
| **16** | Permutation Rounds / Orbit Period (CSES 3398)| Permutation Orbit LCM Modulo | `O(N log MOD)` | `O(N)` |
| **17** | Prime Multiples (CSES 2185 — PIE) | Inclusion-Exclusion on Bitmasks | `O(K * 2^K)` | `O(K)` |
| **18** | Throwing Dice (CSES 1096 — Order-6 Exponent)| Order-6 Companion Exponentiation | `O(6^3 log N)` | `O(1)` |
| **19** | Range Bitwise AND (LeetCode 201) | Binary Prefix Bit-Shifts | `O(log R)` | `O(1)` |
| **20** | Subarray Bitwise ORs (LeetCode 898) | Set DP / Monotonic Frontier Values | `O(N * 30)` | `O(N * 30)` |
| **21** | Count Total Set Bits (1 to N) | Periodic Bit Position Math | `O(log N)` | `O(1)` |
| **22** | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values | `O(N * 30)` | `O(30)` |
| **23** | Multiply Two 2D Matrices | 3-Nested Loop Dot Products | `O(M * K * N)` | `O(M * N)` |
| **24** | N x N MEX Grid Construction | Bitwise XOR Matrix (i ^ j) | `O(N^2)` | `O(N^2)` |

---

### Notebook 3: Dynamic Programming, Knapsacks, Sequences & Optimization

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Minimum Subset Sum Difference | Subset Sum DP <= Total / 2 | `O(N * Total)` | `O(Total)` |
| **2** | Count of Longest Increasing Subseq (LC 673) | 1D DP (Length & Count Arrays) | `O(N^2)` | `O(N)` |
| **3** | Shortest Common Supersequence (SCS) Length | Reduction: len(S1) + len(S2) - LCS | `O(N * M)` | `O(N * M)` |
| **4** | Reconstruct Shortest Common Supersequence | 2D LCS Table Backtracking | `O(N * M)` | `O(N * M)` |
| **5** | Minimum Window Subsequence (LeetCode 727) | Forward Match + Backward Shrink | `O(N * M)` | `O(1)` |
| **6** | Matrix Chain Multiplication | Interval DP over Chain Lengths | `O(N^3)` | `O(N^2)` |
| **7** | Bounded Knapsack (Binary Power Split) | Binary Split + 1D 0/1 Knapsack | `O(W * sum(log K))` | `O(W)` |
| **8** | LCS Length of 2 Permutations | Index Map + Patience Sorting LIS | `O(N log N)` | `O(N)` |
| **9** | Longest Common Increasing Subseq (LCIS) | 1D DP with Optimal Prefix Tracker | `O(N * M)` | `O(M)` |
| **10** | Min Moves to Gather K Consecutive Ones (1703)| Shifted Index + Median Prefix Sum | `O(N)` | `O(N)` |
| **11** | 2D Prefix Sum Matrix | 2D Inclusion-Exclusion Prefix DP | `O(M * N)` build, `O(1)` | `O(M * N)` |
| **12** | Make Array Non-Decreasing (Slope Trick) | Greedy Max-Heap Slope Inflection | `O(N log N)` | `O(N)` |
| **13** | Max Subarray Sum of Length <= K (CSES II) | Monotonic Deque on Prefix Sums | `O(N)` | `O(N)` |
| **14** | Lexicographical Rank of a Permutation | Factorial Positional Weighting | `O(N^2)` | `O(N)` |
| **15** | LCM of Array Elements Modulo MOD | Prime Factorization + Max Powers | `O(N * sqrt(max_A))` | `O(U)` |
| **16** | Permutation Rounds (LCM of Cycles — CSES 3398)| Disjoint Cycle Decomposition + LCM | `O(N)` | `O(N)` |
| **17** | Min Move-to-Anywhere Operations (N - LIS) | Longest Increasing Subseq (N - LIS) | `O(N log N)` | `O(N)` |
| **18** | Min Move-to-Front Operations (N...1) | Greedy Backwards Suffix Matching | `O(N)` | `O(1)` |
| **19** | Min Subarrays with Sum <= K | Greedy Running Sum Partitioning | `O(N)` | `O(1)` |
| **20** | House Robber IV (LeetCode 2560) | Binary Search on Min Capability | `O(N log(max - min))` | `O(1)` |
| **21** | Bounded Coin Change (Limited Supply) | Binary Splitting + 0/1 Knapsack | `O(amount * sum(log limit))` | `O(amount)` |
| **22** | Coin Change Path Reconstruction | DP Predecessor Pointer Traceback | `O(N * amount)` | `O(amount)` |

---

### Notebook 4: Dynamic Intervals, Sweep-Line, Difference Arrays & Specialized Sorting

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Kth Smallest in Lexicographical Order (440) | Trie Prefix Tree Level Skipping | `O(log^2 N)` | `O(1)` |
| **2** | Longest Nice Subarray (LC 2401) | Sliding Window + Cumulative OR | `O(N)` | `O(1)` |
| **3** | Interval List Intersections (LeetCode 986) | Two-Pointer Boundary Intersection | `O(N + M)` | `O(N + M)` |
| **4** | Data Stream Disjoint Intervals (LC 352) | Balanced BST (`std::set`) Neighbor Merge | `O(log N)` add, `O(N)` get | `O(N)` |
| **5** | My Calendar I (LeetCode 729) | `std::set` `lower_bound` Boundary BST | `O(log N)` per book | `O(N)` |
| **6** | My Calendar II (LeetCode 731) | Dual Overlap List Verification | `O(N)` per book | `O(N)` |
| **7** | My Calendar III (LeetCode 732) | Sweep-Line Difference Map (`std::map`) | `O(N)` per book | `O(N)` |
| **8** | Corporate Flight Bookings (LeetCode 1109) | 1D Range Difference Array + Prefix Sum | `O(N + Bookings)` | `O(N)` |
| **9** | Range Module (LeetCode 715) | `std::map` Disjoint Interval Slices | `O(K log N)` per op | `O(N)` |
| **10** | Count Integers in Intervals (LC 2276) | `std::map` Dynamic Overlap Merging | Amortized `O(log N)` | `O(N)` |
| **11** | Remove Covered Intervals (LeetCode 1288) | Custom Sort (Start Asc, End Desc) | `O(N log N)` | `O(1)` |
| **12** | Maximum Length of Pair Chain (LC 646) | Greedy End-Time Selection (LIS DP) | `O(N log N)` | `O(1)` |
| **13** | Area Painted Each Day (LeetCode 2158) | Jump-Table / DSU Path Compression | `O(N + MaxPosition)` | `O(MaxPosition)` |
| **14** | Teemo Attacking (LeetCode 495) | Timeline Overlap Accumulation | `O(N)` | `O(1)` |
| **15** | In-Place Merge Sort (Gap Method) | Shell-like Gap Reduction | `O(N log^2 N)` | `O(1)` |
| **16** | Radix Sort MSD (Recursive Bucket Sort) | Trie-like MSD Prefix Partitioning | `O(N * L)` | `O(N + R * Depth)` |
| **17** | Pigeonhole Sort (Direct Range Slots) | Range-Sized Bucket Placement | `O(N + Range)` | `O(N + Range)` |
| **18** | Pancake Sorting (LeetCode 969) | Greedy Suffix Placement (2-Flips) | `O(N^2)` | `O(N)` |
| **19** | Patience Sorting & LIS (Greene's Theorem) | Pile Insertion via lower_bound | `O(N log N)` | `O(N)` |
| **20** | Sort Matrix Diagonally (LeetCode 1329) | Hash Map Diagonal Bucketing | `O(M * N * log(min(M,N)))` | `O(M * N)` |
| **21** | Sort Transformed Array (LeetCode 360) | Parabola Concavity Two-Pointers | `O(N)` | `O(N)` |

---

### Notebook 5: System Design, Online Data Streams, Caches (LRU/TTL) & Concurrency

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Design HashMap (MyHashMap - LC 706) | Separate Chaining (Bucket Lists) | Avg `O(1)` | `O(N)` |
| **2** | Summary Ranges (LC 228 / LC 352) | Sorted Set Range Traversal | `O(log N)` add, `O(N)` get | `O(N)` |
| **3** | Water and Jug Problem (BFS - LC 365) | BFS 6-State Graph Traversal | `O(X * Y)` | `O(X * Y)` |
| **4** | Moving Average from Stream (LeetCode 346) | Sliding Circular Buffer / Queue | `O(1)` per next | `O(W)` |
| **5** | First Unique Number in Stream (LC 1429) | Doubly Linked List + Hash Map | `O(1)` all ops | `O(N)` |
| **6** | First Non-Repeating Character in Stream | Frequency Table + FIFO Queue | `O(1)` avg | `O(1)` |
| **7** | Design Hit Counter (LeetCode 362) | Circular Buckets (300s) / Queue | `O(1)` hit / `O(1)` get | `O(1)` |
| **8** | Logger Rate Limiter (LeetCode 359) | Hash Map Expiration Timestamps | `O(1)` per message | `O(M)` |
| **9** | Front Middle Back Queue (LeetCode 1670) | Dual Balanced Deques (Left/Right) | `O(1)` all operations | `O(N)` |
| **10** | Finding MK Average (LeetCode 1825) | 3 Multisets (Left, Mid, Right) | `O(log M)` add, `O(1)` avg | `O(M)` |
| **11** | Snapshot Array (LeetCode 1146) | History Vectors + Binary Search | `O(1)` set, `O(log S)` get | `O(N + U)` |
| **12** | Stock Price Fluctuation (LeetCode 2034) | Hash Map + Balanced Multiset | `O(log N)` update, `O(1)` get| `O(N)` |
| **13** | Seat Reservation Manager (LeetCode 1845) | Min-Heap of Free Seat IDs | `O(log N)` reserve/unreserve| `O(N)` |
| **14** | Design Underground System (LeetCode 1396) | Dual Hash Maps (Transit & Stats) | `O(1)` all operations | `O(P + S^2)`|
| **15** | Number of Recent Calls (LeetCode 933) | Sliding Window Queue [t-3000, t] | `O(1)` avg per ping | `O(W)` |
| **16** | Design Leaderboard (LeetCode 1244) | Hash Map + Min-Heap Top-K | `O(1)` add, `O(N log K)` top | `O(P)` |
| **17** | Bounded Blocking Queue (LeetCode 1188) | Mutex + Dual Condition Variables | `O(1)` all operations | `O(Cap)` |
| **18** | Majority Checker in Subarray (LC 1154) | Boyer-Moore Random Sampling + UB | `O(K log N)` per query | `O(N)` |
| **19** | Design Authentication Manager (LC 1797) | Hash Map Expiry with Lazy Purge | `O(1)` gen, `O(N)` count | `O(T)` |
| **20** | Multi-Policy Online Cache Engine (LRU/TTL) | DLL + Hash Map + Expiry Min-Heap | `O(1)` avg get/put | `O(Cap)` |
