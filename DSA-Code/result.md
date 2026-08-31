# Master Problem Summary & Complexity Reference (Notebooks 1–10)

A unified reference catalog of **109 unique Data Structures & Algorithms problems** implemented across `notebook-1.cpp` through `notebook-10.cpp` (all problems existing in `Modules/` have been deduplicated and removed). Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities.

## 📊 High-Level Problem Distribution

| Notebook | Focus Topic / Core Theme | Unique Problems Count |
| :--- | :--- | :---: |
| **Notebook 1** | Advanced Trees, DAG Paths, Rerooting & Graph Decompositions | **7** |
| **Notebook 2** | Number Theory, CSES Combinatorics, Power Towers & Matrices | **18** |
| **Notebook 3** | Cycle Reconstruction, String DP, Supersequences & Trees | **19** |
| **Notebook 4** | Coordinate Gathering, MEX Grids, Divisor Cuts & Slope Trick | **9** |
| **Notebook 5** | Permutation Operations, Window Extremes & Bidirectional BFS | **8** |
| **Notebook 6** | System Design, Lexicographical Trees & Bitwise Windows | **5** |
| **Notebook 7** | Jump Game Variants, Capability Binary Search & Bounded Knapsacks | **7** |
| **Notebook 8** | Dynamic Intervals, BST Schedules, Difference Arrays & DSU Paints | **12** |
| **Notebook 9** | Stream Median/Averages, Concurrency, Hit Counters & Online Caches | **17** |
| **Notebook 10** | Specialized Sorting (Gap Method, MSD Radix, Pigeonhole, Pancake) | **7** |
| **TOTAL** | | **109** |

---

## 📘 Notebook-by-Notebook Analysis

### Notebook 1: Advanced Trees, DAG Paths, Rerooting & Graph Decompositions

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Tree Centroid | Subtree Size DFS + Heavy Descent | `O(N)` | `O(N)` |
| **2** | Longest Paths & Path Counts on a DAG | Topological Sort + DP Relaxation | `O(V + E)` | `O(V + E)` |
| **3** | Tree Rerooting DP (All-Nodes Distance Sum) | 2-Pass Tree Rerooting DP | `O(N)` | `O(N)` |
| **4** | Tree Diameter & Node Eccentricities | 2-Pass Tree BFS/DFS | `O(N)` | `O(N)` |
| **5** | Functional Graph Decomposition & Queries | Floyd's Cycle + Binary Lifting | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **6** | Tree Isomorphism (Double-Hashing) | Canonical Subtree Hashing (AHU) | `O(N log N)` | `O(N)` |
| **7** | Tree Centers by Leaf Trimming | Topological Leaf Peeling (Deg 1) | `O(N)` | `O(N)` |

---

### Notebook 2: Number Theory, CSES Combinatorics, Power Towers & Matrices

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

---

### Notebook 3: Cycle Reconstruction, String DP, Supersequences & Trees

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Reconstruct Cycle | Predecessor Backtracking Traversal | `O(V)` | `O(V)` |
| **2** | Nodes in Cycles (Topological Peeling) | In-Degree 0 Queue Cascade (Kahn) | `O(V + E)` | `O(V)` |
| **3** | Range Bitwise AND (LeetCode 201) | Binary Prefix Bit-Shifts | `O(log R)` | `O(1)` |
| **4** | Subarray Bitwise ORs (LeetCode 898) | Set DP / Monotonic Frontier Values | `O(N * 30)` | `O(N * 30)` |
| **5** | Count Total Set Bits (1 to N) | Periodic Bit Position Math | `O(log N)` | `O(1)` |
| **6** | Minimum Subset Sum Difference | Subset Sum DP <= Total / 2 | `O(N * Total)` | `O(Total)` |
| **7** | Count of Longest Increasing Subseq (LC 673) | 1D DP (Length & Count Arrays) | `O(N^2)` | `O(N)` |
| **8** | Lexicographical Topological Sort | Min-Heap Kahn's BFS | `O(V log V + E)` | `O(V)` |
| **9** | Shortest Path Visiting All Nodes (LC 847) | Multi-Source Bitmask BFS | `O(V * 2^V)` | `O(V * 2^V)` |
| **10** | Shortest Common Supersequence (SCS) Length | Reduction: len(S1) + len(S2) - LCS | `O(N * M)` | `O(N * M)` |
| **11** | Reconstruct Shortest Common Supersequence | 2D LCS Table Backtracking | `O(N * M)` | `O(N * M)` |
| **12** | Minimum Window Subsequence (LeetCode 727) | Forward Match + Backward Shrink | `O(N * M)` | `O(1)` |
| **13** | Matrix Chain Multiplication | Interval DP over Chain Lengths | `O(N^3)` | `O(N^2)` |
| **14** | Maximum Sum BST in Binary Tree (LC 1373) | Post-Order Bottom-Up DFS | `O(N)` | `O(H)` |
| **15** | Remove Leaf Nodes with Target Value (LC 1325)| Post-Order Recursive Tree Pruning | `O(N)` | `O(H)` |
| **16** | Bounded Knapsack (Binary Power Split) | Binary Split + 1D 0/1 Knapsack | `O(W * log K)` | `O(W)` |
| **17** | LCS Length of 2 Permutations | Index Map + Patience Sorting LIS | `O(N log N)` | `O(N)` |
| **18** | Longest Common Increasing Subseq (LCIS) | 1D DP with Optimal Prefix Tracker | `O(N * M)` | `O(M)` |
| **19** | Possible Path Lengths in DAG | TopoSort + 2D Reachability DP | `O(V * N + E * N)` | `O(V * N)` |

---

### Notebook 4: Coordinate Gathering, MEX Grids, Divisor Cuts & Slope Trick

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Min Moves to Gather K Consecutive Ones (1703)| Shifted Index + Median Prefix Sum | `O(N)` | `O(N)` |
| **2** | 2D Prefix Sum Matrix | 2D Inclusion-Exclusion Prefix DP | `O(M * N)` | `O(M * N)` |
| **3** | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values | `O(N * 30)` | `O(N)` |
| **4** | Multiply Two 2D Matrices | 3-Nested Loop Dot Products | `O(M * N * P)` | `O(M * P)` |
| **5** | Max Components Tree Split with Equal Sum | Divisors + Subtree DFS Cuts | `O(N * divisors(Total))` | `O(N)` |
| **6** | N x N MEX Grid Construction | Bitwise XOR Matrix (i ^ j) | `O(N^2)` | `O(N^2)` |
| **7** | Binary Lifting (LCA, K-th Ancestor) | Ancestor Doubling Table up[u][j] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **8** | Cycle Detection & Reconstruction | DFS Parent Trace / Bellman-Ford | `O(V + E) / O(V * E)` | `O(V)` |
| **9** | Make Array Non-Decreasing (Slope Trick) | Greedy Max-Heap Slope Inflection | `O(N log N)` | `O(N)` |

---

### Notebook 5: Permutation Operations, Window Extremes & Bidirectional BFS

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Max Subarray Sum of Length at Most K | Monotonic Deque on Prefix Sums | `O(N)` | `O(N)` |
| **2** | Lexicographical Rank of a Permutation | Factorial Positional Weighting | `O(N^2)` | `O(N)` |
| **3** | LCM of Array Elements Modulo MOD | Prime Factorization + Max Powers | `O(N * sqrt(max_A))` | `O(U)` |
| **4** | Permutation Rounds (LCM of Cycles — CSES 3398)| Disjoint Cycle Decomposition + LCM | `O(N + sum(sqrt(len)))` | `O(N)` |
| **5** | Min Move-to-Anywhere Operations (N - LIS) | Longest Increasing Subseq (N - LIS) | `O(N log N)` | `O(N)` |
| **6** | Min Move-to-Front Operations (N...1) | Greedy Backwards Suffix Matching | `O(N)` | `O(1)` |
| **7** | Min Subarrays with Sum <= K | Greedy Running Sum Partitioning | `O(N)` | `O(1)` |
| **8** | Bidirectional BFS Template | Two-Ended BFS (Smaller Frontier) | `O(B^(D/2))` | `O(B^(D/2))` |

---

### Notebook 6: System Design, Lexicographical Trees & Bitwise Windows

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Design HashMap (MyHashMap - LC 706) | Separate Chaining (Bucket Lists) | Avg `O(1)` | `O(N)` |
| **2** | Summary Ranges (LC 228 / LC 352) | Sorted Set Range Traversal | `O(log N)` add, `O(N)` get | `O(N)` |
| **3** | Water and Jug Problem (BFS - LC 365) | BFS 6-State Graph Traversal | `O(X * Y)` | `O(X * Y)` |
| **4** | Kth Smallest in Lexicographical Order (440) | Trie Prefix Tree Level Skipping | `O(log^2 N)` | `O(1)` |
| **5** | Longest Nice Subarray (LC 2401) | Sliding Window + Cumulative OR | `O(N)` | `O(1)` |

---

### Notebook 7: Jump Game Variants, Capability Binary Search & Bounded Knapsacks

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Jump Game III (LeetCode 1306) | Graph BFS Reachability | `O(N)` | `O(N)` |
| **2** | Jump Game IV (LeetCode 1345) | BFS + Same-Value Pruning | `O(N)` | `O(N)` |
| **3** | Jump Game V (LeetCode 1340) | Memoized DFS / DAG Longest Path | `O(N * D)` | `O(N)` |
| **4** | Jump Game VII (LeetCode 1871) | Sliding Window Reachability DP | `O(N)` | `O(N)` |
| **5** | House Robber IV (LeetCode 2560) | Binary Search on Min Capability | `O(N log(max - min))` | `O(1)` |
| **6** | Bounded Coin Change (Limited Supply) | Binary Splitting + 0/1 Knapsack | `O(amount * sum(log(limit)))` | `O(amount)` |
| **7** | Coin Change Path Reconstruction | DP Predecessor Pointer Traceback | `O(N * amount)` | `O(amount)` |

---

### Notebook 8: Dynamic Intervals, BST Schedules, Difference Arrays & DSU Paints

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Interval List Intersections (LeetCode 986) | Two-Pointer Boundary Intersection | `O(N + M)` | `O(N + M)` |
| **2** | Data Stream Disjoint Intervals (LC 352) | Balanced BST (`std::set`) Neighbor Merge | `O(log N)` add, `O(N)` get | `O(N)` |
| **3** | My Calendar I (LeetCode 729) | `std::set` `lower_bound` Boundary BST | `O(log N)` per book | `O(N)` |
| **4** | My Calendar II (LeetCode 731) | Dual Overlap List Verification | `O(N)` per book | `O(N)` |
| **5** | My Calendar III (LeetCode 732) | Sweep-Line Difference Map (`std::map`) | `O(N)` per book | `O(N)` |
| **6** | Corporate Flight Bookings (LeetCode 1109) | 1D Range Difference Array + Prefix Sum | `O(N + Bookings)` | `O(N)` |
| **7** | Range Module (LeetCode 715) | `std::map` Disjoint Interval Slices | `O(K log N)` per op | `O(N)` |
| **8** | Count Integers in Intervals (LC 2276) | `std::map` Dynamic Overlap Merging | Amortized `O(log N)` | `O(N)` |
| **9** | Remove Covered Intervals (LeetCode 1288) | Custom Sort (Start Asc, End Desc) | `O(N log N)` | `O(1)` |
| **10** | Maximum Length of Pair Chain (LC 646) | Greedy End-Time Selection (LIS DP) | `O(N log N)` | `O(1)` |
| **11** | Area Painted Each Day (LeetCode 2158) | Jump-Table / DSU Path Compression | `O(N + MaxPosition)` | `O(MaxPosition)` |
| **12** | Teemo Attacking (LeetCode 495) | Timeline Overlap Accumulation | `O(N)` | `O(1)` |

---

### Notebook 9: Stream Median/Averages, Concurrency, Hit Counters & Online Caches

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Moving Average from Stream (LeetCode 346) | Sliding Circular Buffer / Queue | `O(1)` per next | `O(W)` |
| **2** | First Unique Number in Stream (LC 1429) | Doubly Linked List + Hash Map | `O(1)` all ops | `O(N)` |
| **3** | First Non-Repeating Character in Stream | Frequency Table + FIFO Queue | `O(1)` avg | `O(1)` |
| **4** | Design Hit Counter (LeetCode 362) | Circular Buckets (300s) / Queue | `O(1)` hit / `O(1)` get | `O(1)` |
| **5** | Logger Rate Limiter (LeetCode 359) | Hash Map Expiration Timestamps | `O(1)` per message | `O(M)` |
| **6** | Front Middle Back Queue (LeetCode 1670) | Dual Balanced Deques (Left/Right) | `O(1)` all operations | `O(N)` |
| **7** | Finding MK Average (LeetCode 1825) | 3 Multisets (Left, Mid, Right) | `O(log M)` add, `O(1)` avg | `O(M)` |
| **8** | Snapshot Array (LeetCode 1146) | History Vectors + Binary Search | `O(1)` set, `O(log S)` get | `O(N + U)` |
| **9** | Stock Price Fluctuation (LeetCode 2034) | Hash Map + Balanced Multiset | `O(log N)` update, `O(1)` get| `O(N)` |
| **10** | Seat Reservation Manager (LeetCode 1845) | Min-Heap of Free Seat IDs | `O(log N)` reserve/unreserve| `O(N)` |
| **11** | Design Underground System (LeetCode 1396) | Dual Hash Maps (Transit & Stats) | `O(1)` all operations | `O(P + S^2)`|
| **12** | Number of Recent Calls (LeetCode 933) | Sliding Window Queue [t-3000, t] | `O(1)` avg per ping | `O(W)` |
| **13** | Design Leaderboard (LeetCode 1244) | Hash Map + Min-Heap Top-K | `O(1)` add, `O(N log K)` top | `O(P)` |
| **14** | Bounded Blocking Queue (LeetCode 1188) | Mutex + Dual Condition Variables | `O(1)` all operations | `O(Cap)` |
| **15** | Majority Checker in Subarray (LC 1154) | Boyer-Moore Random Sampling + UB | `O(K log N)` per query | `O(N)` |
| **16** | Design Authentication Manager (LC 1797) | Hash Map Expiry with Lazy Purge | `O(1)` gen, `O(N)` count | `O(T)` |
| **17** | Multi-Policy Online Cache Engine (LRU/TTL) | DLL + Hash Map + Expiry Min-Heap | `O(1)` avg get/put | `O(Cap)` |

---

### Notebook 10: Specialized Sorting Concepts & Interview Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | In-Place Merge Sort (Gap Method) | Shell-like Gap Reduction | `O(N log^2 N)` | `O(1)` |
| **2** | Radix Sort MSD (Recursive Bucket Sort) | Trie-like MSD Prefix Partitioning | `O(N * L)` | `O(N + B * D)` |
| **3** | Pigeonhole Sort (Direct Range Slots) | Range-Sized Bucket Placement | `O(N + R)` | `O(N + R)` |
| **4** | Pancake Sorting (LeetCode 969) | Greedy Suffix Placement (2-Flips) | `O(N^2)` | `O(N)` |
| **5** | Patience Sorting & LIS (Greene's Theorem) | Pile Insertion via lower_bound | `O(N log N)` | `O(N)` |
| **6** | Sort Matrix Diagonally (LeetCode 1329) | Hash Map Diagonal Bucketing | `O(M * N * log(min(M,N)))` | `O(M * N)` |
| **7** | Sort Transformed Array (LeetCode 360) | Parabola Concavity Two-Pointers | `O(N)` | `O(N)` |
