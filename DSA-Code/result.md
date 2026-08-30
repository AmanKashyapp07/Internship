# Master Problem Summary & Complexity Reference (Notebooks 1–7)

A unified reference catalog of **187 classic Data Structures & Algorithms problems** implemented across `notebook-1.cpp` through `notebook-7.cpp`. Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities.

## 📘 Notebook-by-Notebook Analysis

### Notebook 1: Trees, Graphs, DSU & String Hashing

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Tree Centroid | Subtree Size DFS + Heavy Descent | `O(N)` | `O(N)` |
| **2** | Polynomial Rolling Hash of a String | Horner's Polynomial (Base P = 31) | `O(len(S))` | `O(1)` |
| **3** | Substring Polynomial Rolling Hash | Prefix Hash Array + Power Table | `O(N)` build, `O(1)` query | `O(N)` |
| **4** | Disjoint Set Union (DSU / Union-Find) | Path Compression + Union by Size | `O(α(N))` | `O(N)` |
| **5** | Kruskal's Minimum Spanning Tree (MST) | Greedy Edge Sorting + DSU | `O(E log E)` | `O(V + E)` |
| **6** | Prim's Minimum Spanning Tree (MST) | Min-Heap Vertex Expansion | `O(E log V)` | `O(V + E)` |
| **7** | Kahn's Topological Sort | In-Degree 0 FIFO Queue (BFS) | `O(V + E)` | `O(V + E)` |
| **8** | Longest Paths & Path Counts on a DAG | Topological Sort + DP Relaxation | `O(V + E)` | `O(V + E)` |
| **9** | Tree Rerooting DP (All-Nodes Distance Sum) | 2-Pass Tree Rerooting DP | `O(N)` | `O(N)` |
| **10** | Tree Diameter & Node Eccentricities | 2-Pass Tree BFS/DFS | `O(N)` | `O(N)` |
| **11** | Strongly Connected Components (Kosaraju's) | Forward DFS + Reverse Graph DFS | `O(V + E)` | `O(V + E)` |
| **12** | Functional Graph Decomposition & Queries | Floyd's Cycle + Binary Lifting | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **13** | Tree Isomorphism (Double-Hashing) | Canonical Subtree Hashing (AHU) | `O(N log N)` | `O(N)` |
| **14** | Tree Centers by Leaf Trimming | Topological Leaf Peeling (Deg 1) | `O(N)` | `O(N)` |

---

### Notebook 2: Number Theory, Combinatorics, Sieve & Dynamic Programming

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Binary Exponentiation (Modular Power) | Bitwise Repeated Squaring | `O(log B)` | `O(1)` |
| **2** | Modular Multiplicative Inverse | Fermat's Little Theorem (MOD - 2) | `O(log MOD)` | `O(1)` |
| **3** | Factorials & Combinatorics (nCr, nPr) | Factorial & Inverse Fac Arrays | `O(N)` build, `O(1)` query | `O(N)` |
| **4** | Sieve of Eratosthenes | Multiples Cross-Off | `O(N log log N)` | `O(N)` |
| **5** | Prime Factorization | Trial Division up to sqrt(N) | `O(sqrt(N))` | `O(log N)` |
| **6** | Prefix XOR (1 to N) | Periodicity of 4 in Binary XOR | `O(1)` | `O(1)` |
| **7** | Sliding Window Maximum & Minimum | Monotonic Double-Ended Queue (Deque) | `O(N)` | `O(K)` |
| **8** | Max Subarray Sum of Length at Most K | Prefix Sums + Monotonic Deque | `O(N)` | `O(N)` |
| **9** | Longest Increasing Subsequence (LIS) | Patience Sorting (lower_bound) | `O(N log N)` | `O(N)` |
| **10** | Longest Common Subsequence (Reconstruction) | 2D Dynamic Programming Table | `O(N * M)` | `O(N * M)` |
| **11** | Interval DP Template | DP over Subproblem Lengths | `O(N^3)` | `O(N^2)` |
| **12** | Subset Enumeration & Sum of All Subsets | Bitmask Enumeration [0...2^N - 1] | `O(N * 2^N)` | `O(N * 2^N)` |
| **13** | Kadane's Max Subarray Sum with Indices | Running Prefix Sum Reset | `O(N)` | `O(1)` |
| **14** | Permutation Generation | Bitmask Backtracking | `O(N! * N)` | `O(N! * N)` |
| **15** | Longest Palindromic Subsequence (LPS) | Interval DP / LCS with Reverse | `O(N^2)` | `O(N^2)` |
| **16** | Check Subsequence | Greedy Two-Pointer Linear Scan | `O(len(S))` | `O(1)` |
| **17** | Precompute Palindromic Subarrays Table | 2D Interval DP over Lengths | `O(N^2)` | `O(N^2)` |
| **18** | Booth's Algorithm (Min String Rotation) | Two-Pointer Mismatch on (S + S) | `O(N)` | `O(N)` |
| **19** | Minimum Excluded Value (MEX) | Hash Set Presence Lookup | `O(N)` | `O(N)` |
| **20** | Inversion Count (a[i] > x * a[j]) | Divide & Conquer Merge Sort Count | `O(N log N)` | `O(N)` |
| **21** | Matrix Multiplication & Exponentiation | Binary Exponentiation on Matrices | `O(N^3 log B)` | `O(N^2)` |
| **22** | N-th Fibonacci (Matrix Exponentiation) | [[1, 1], [1, 0]] Matrix Power | `O(log N)` | `O(1)` |
| **23** | Total Area Covered by Two 2D Rectangles | Inclusion-Exclusion Geometry | `O(1)` | `O(1)` |
| **24** | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n | `O(N)` | `O(N)` |

---

### Notebook 3: Shortest Paths, Knapsack, Advanced Trees & String DP

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Reconstruct Cycle | Predecessor Backtracking Traversal | `O(V)` | `O(V)` |
| **2** | Shortest Cycle Length (Girth) | Multi-Source BFS on All Vertices | `O(V * (V + E))` | `O(V)` |
| **3** | Nodes in Cycles (Topological Peeling) | In-Degree 0 Queue Cascade (Kahn) | `O(V + E)` | `O(V)` |
| **4** | Bipartite Graph Check (DFS 2-Coloring) | Alternating DFS 2-Coloring | `O(V + E)` | `O(V)` |
| **5** | Shortest Path on Weighted DAG | TopoSort + Linear Edge Relaxation | `O(V + E)` | `O(V)` |
| **6** | Range Bitwise AND | Binary Prefix Bit-Shifts | `O(log R)` | `O(1)` |
| **7** | Longest Nice Subarray (Pairwise AND = 0) | Sliding Window + Cumulative OR | `O(N)` | `O(1)` |
| **8** | Subarray Bitwise ORs | Set DP / Monotonic Frontier Values | `O(N * 30)` | `O(N * 30)` |
| **9** | Count Total Set Bits (1 to N) | Periodic Bit Position Math | `O(log N)` | `O(1)` |
| **10** | Count Subsets with Sum K | 1D 0/1 Knapsack DP (Backwards) | `O(N * K)` | `O(K)` |
| **11** | Minimum Subset Sum Difference | Subset Sum DP <= Total / 2 | `O(N * Total)` | `O(Total)` |
| **12** | Get Money Sums | Reachable Subset Sums Boolean DP | `O(N * Total)` | `O(Total)` |
| **13** | Count of Longest Increasing Subsequences | 1D DP (Length & Count Arrays) | `O(N^2)` | `O(N)` |
| **14** | Dijkstra's Shortest Path | Min-Heap Priority Queue | `O((V + E) log V)` | `O(V + E)` |
| **15** | Bellman-Ford Algorithm | DP Edge Relaxation (V - 1 Passes) | `O(V * E)` | `O(V)` |
| **16** | Shortest Path on Unweighted Graph (BFS) | BFS Queue Level-by-Level Scan | `O(V + E)` | `O(V)` |
| **17** | Floyd-Warshall All-Pairs Shortest Path | Intermediate Vertex DP (k-loop) | `O(V^3)` | `O(V^2)` |
| **18** | Negative Cycle Detection (Floyd-Warshall) | Diagonal Self-Distance Inspection | `O(V^3)` | `O(V^2)` |
| **19** | Lexicographical Topological Sort | Min-Heap Kahn's BFS | `O(V log V + E)` | `O(V)` |
| **20** | Shortest Path on DAG via Topo Order | TopoSort Sequential Relaxation | `O(V + E)` | `O(V)` |
| **21** | Graph M-Coloring (Backtracking) | Backtracking DFS with Safety Check | `O(M^V)` | `O(V)` |
| **22** | Cheapest Flights within K Stops | State-Extended Dijkstra (stops) | `O(E * K)` | `O(V * K)` |
| **23** | Shortest Path Visiting All Nodes | Multi-Source Bitmask BFS | `O(V * 2^V)` | `O(V * 2^V)` |
| **24** | Shortest Common Supersequence (SCS) Length | Reduction: len(S1) + len(S2) - LCS | `O(len(S1) * len(S2))` | `O(len(S1) * len(S2))` |
| **25** | Reconstruct Shortest Common Supersequence | 2D LCS Table Backtracking | `O(len(S1) * len(S2))` | `O(len(S1) * len(S2))` |
| **26** | Minimum Window Subsequence | Forward Match + Backward Shrink | `O(len(S) * len(T))` | `O(1)` |
| **27** | Minimum Window Substring | Sliding Window with Match Counter | `O(len(S) + len(T))` | `O(len(S) + len(T))` |
| **28** | Matrix Chain Multiplication | Interval DP over Chain Lengths | `O(N^3)` | `O(N^2)` |
| **29** | Distinct Subsequences | 1D Space-Optimized DP (Backwards) | `O(len(S) * len(T))` | `O(len(T))` |
| **30** | Maximum Sum BST in Binary Tree | Post-Order Bottom-Up DFS | `O(N)` | `O(H)` |
| **31** | Remove Leaf Nodes with Target Value | Post-Order Recursive Tree Pruning | `O(N)` | `O(H)` |
| **32** | Minimum Extra Characters in String | 1D Memoized DP + Hash Set | `O(N^2)` | `O(N + D)` |
| **33** | Bounded Knapsack (Binary Power Split) | Binary Split + 1D 0/1 Knapsack | `O(W * log K)` | `O(W)` |
| **34** | LCS Length of 2 Permutations | Index Map + Patience Sorting LIS | `O(N log N)` | `O(N)` |
| **35** | Longest Common Increasing Subseq (LCIS) | 1D DP with Optimal Prefix Tracker | `O(N * M)` | `O(M)` |
| **36** | Possible Path Lengths in DAG | TopoSort + 2D Reachability DP | `O(V * N + E * N)` | `O(V * N)` |

---

### Notebook 4: Binary Lifting, Monotonic Stacks, Digit DP & Slope Trick

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | K-th Element & Median of 2 Sorted Arrays | Binary Search on Partition Cut | `O(log(min(M, N)))` | `O(1)` |
| **2** | Next Greater Permutation | 3-Step Suffix Inversion Reversal | `O(N)` | `O(1)` |
| **3** | Longest Substring Without Repeating Chars | Sliding Window + Last Seen Map | `O(N)` | `O(min(N, Alphabet))` |
| **4** | Min Moves to Gather K Consecutive Ones | Shifted Index + Median Prefix Sum | `O(N)` | `O(N)` |
| **5** | 2D Prefix Sum Matrix | 2D Inclusion-Exclusion Prefix DP | `O(M * N)` | `O(M * N)` |
| **6** | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values | `O(N * 30)` | `O(N)` |
| **7** | Multiply Two 2D Matrices | 3-Nested Loop Dot Products | `O(M * N * P)` | `O(M * P)` |
| **8** | Count Subsequences with Sum Equal to K | 1D 0/1 Knapsack DP (Backwards) | `O(N * K)` | `O(K)` |
| **9** | Row with Maximum 1s in Binary Matrix | Top-Right Corner Staircase Scan | `O(N + M)` | `O(1)` |
| **10** | Max Components Tree Split with Equal Sum | Divisors + Subtree DFS Cuts | `O(N * divisors(Total))` | `O(N)` |
| **11** | Wildcard Pattern Matching | 2D DP Memoization ('?' and '*') | `O(len(S) * len(P))` | `O(len(S) * len(P))` |
| **12** | Next Greater Element | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **13** | Next Smaller Element | Monotonic Increasing Stack | `O(N)` | `O(N)` |
| **14** | Previous Greater Element | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **15** | Previous Smaller Element | Monotonic Increasing Stack | `O(N)` | `O(N)` |
| **16** | Stock Span Problem | Monotonic Stack of Indices | `O(N)` | `O(N)` |
| **17** | Next Greater Element in Circular Array | Monotonic Stack on Doubled (2N) | `O(N)` | `O(N)` |
| **18** | Sum of Subarray Minimums | Monotonic Stack Contribution | `O(N)` | `O(N)` |
| **19** | Maximum Score of Good Subarray | Monotonic Stack Range Bounds (K) | `O(N)` | `O(N)` |
| **20** | Sum of Subarray Maximums | Monotonic Stack Contribution | `O(N)` | `O(N)` |
| **21** | Max of Minimums for Every Window Size | Monotonic Stack + Suffix Max | `O(N)` | `O(N)` |
| **22** | Array Median using Quickselect | Randomized QuickSelect (Lomuto) | `O(N)` avg | `O(1)` |
| **23** | N x N MEX Grid Construction | Bitwise XOR Matrix (i ^ j) | `O(N^2)` | `O(N^2)` |
| **24** | Binary Lifting (LCA, K-th Ancestor) | Ancestor Doubling Table up[u][j] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **25** | Digit DP Template | Digit Memo (pos,start,tight,state) | `O(digits * states)` | `O(digits * states)` |
| **26** | Cycle Detection & Reconstruction | DFS Parent Trace / Bellman-Ford | `O(V + E) / O(V * E)` | `O(V)` |
| **27** | Make Array Non-Decreasing (Slope Trick) | Greedy Max-Heap Slope Inflection | `O(N log N)` | `O(N)` |

---

### Notebook 5: QuickSort/Select, Caches (LRU/LFU), Wiggle Sort & Graph Walks

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Modular Exponentiation | Binary Exponentiation (Squaring) | `O(log B)` | `O(1)` |
| **2** | Matrix Multiplication Modulo MOD | 3-Nested Loop Dot Product (K x K) | `O(K^3)` | `O(K^2)` |
| **3** | Square Matrix Exponentiation | Binary Exponentiation on Matrices | `O(K^3 log P)` | `O(K^2)` |
| **4** | Number of Paths of Length K on Graph | Adjacency Matrix Exponentiation | `O(V^3 log K)` | `O(V^2)` |
| **5** | N-th Fibonacci via Matrix Exponentiation | [[1, 1], [1, 0]] Matrix Power | `O(log N)` | `O(1)` |
| **6** | Max Subarray Sum of Length at Most K | Monotonic Deque on Prefix Sums | `O(N)` | `O(N)` |
| **7** | K-th Lexicographical Permutation | Factorial Number System (Lehmer) | `O(N^2)` | `O(N)` |
| **8** | Lexicographical Rank of a Permutation | Factorial Positional Weighting | `O(N^2)` | `O(N)` |
| **9** | Next Greater Permutation | 3-Step Suffix Inversion Reversal | `O(N)` | `O(1)` |
| **10** | LCM of Array Elements Modulo MOD | Prime Factorization + Max Powers | `O(N * sqrt(max_A))` | `O(U)` |
| **11** | Permutation Rounds (LCM of Cycles) | Disjoint Cycle Decomposition + LCM | `O(N + sum(sqrt(len)))` | `O(N)` |
| **12** | Min Arbitrary Swaps to Sort Permutation | Permutation Cycles (N - cycles) | `O(N)` | `O(N)` |
| **13** | Min Move-to-Anywhere Operations | Longest Increasing Subseq (N - LIS) | `O(N log N)` | `O(N)` |
| **14** | Min Move-to-Front Operations | Greedy Backwards Suffix Matching | `O(N)` | `O(1)` |
| **15** | Repeated Substring Pattern | String Doubling: (S+S)[1...2N-2] | `O(N)` | `O(N)` |
| **16** | Min Subarrays with Sum <= K | Greedy Running Sum Partitioning | `O(N)` | `O(1)` |
| **17** | K-th Smallest via QuickSelect | Lomuto Partition QuickSelect | `O(N)` avg | `O(1)` |
| **18** | Bidirectional BFS | Two-Ended BFS (Smaller Frontier) | `O(B^(D/2))` | `O(B^(D/2))` |
| **19** | Valid Parentheses with Wildcard '*' | Greedy Min-Max Bounds (cmin, cmax) | `O(N)` | `O(1)` |
| **20** | Count Reverse Pairs (a[i] > x * a[j]) | Modified Merge Sort Counting | `O(N log N)` | `O(N)` |
| **21** | Find Median via std::nth_element | QuickSelect Median via STL | `O(N)` avg | `O(1)` |
| **22** | Sort 0s, 1s, and 2s (Dutch National Flag) | 3-Way In-Place Partitioning | `O(N)` | `O(1)` |
| **23** | Merge Two Sorted Arrays | Two-Pointer Merge Step | `O(N + M)` | `O(N + M)` |
| **24** | Merge Sort | Divide-and-Conquer Merge Sort | `O(N log N)` | `O(N)` |
| **25** | Longest Substring with At Least K Frequency | Sliding Window per Unique Target | `O(26 * N)` | `O(1)` |
| **26** | 3-Way QuickSort (DNF Partition) | Dutch National Flag QuickSort | `O(N log N)` avg | `O(log N)` |
| **27** | Cyclic Right and Left Rotation | 3-Step Range Reversal Algorithm | `O(N)` | `O(1)` |
| **28** | Cyclic Sort [1 to N] | In-Place Index Mapping Swap | `O(N)` | `O(1)` |
| **29** | Largest Number from Concatenation | Custom String Comparator Sorting | `O(N log N)` | `O(N)` |
| **30** | Running K-th Element Stream | Two Heaps (Max-Heap + Min-Heap) | `O(log K)` add, `O(1)` get | `O(N)` |
| **31** | Wiggle Sort II | Median + DNF 3-Way + Interleaving | `O(N)` | `O(N)` |
| **32** | LRU Cache & Merge Intervals | Hash Map + Doubly Linked List / Greedy | `O(1)` get/put, `O(N log N)` merge | `O(Cap) / O(N)` |
| **33** | LFU Cache | Hash Map + Freq-to-List + minFreq | `O(1)` get/put | `O(Cap)` |
| **34** | Gas Station Circuit & String Utils | Greedy Net Surplus & Tank Reset | `O(N)` | `O(1)` |

---

### Notebook 6: Sliding Windows, 2D Matrix Binary Search & Custom Classes

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Container With Most Water | Inward Converging Two Pointers | `O(N)` | `O(1)` |
| **2** | Spiral Matrix Traversal | Layer Boundary Simulation | `O(M * N)` | `O(1)` |
| **3** | Merge Two Sorted Arrays | Two-Pointer Linear Merge Step | `O(N + M)` | `O(N + M)` |
| **4** | Longest Consecutive Sequence | Hash Set Boundary (count(x - 1)) | `O(N)` | `O(N)` |
| **5** | Row with Maximum 1s in Binary Matrix | Top-Right Corner Staircase Scan | `O(N + M)` | `O(1)` |
| **6** | Find Median from Data Stream (MedianFinder) | Two Heaps (Max-Heap + Min-Heap) | `O(log N)` add, `O(1)` get | `O(N)` |
| **7** | First Missing Positive | Cyclic Sort In-Place Hashing | `O(N)` | `O(1)` |
| **8** | Word Break | 1D Dynamic Programming + Set | `O(N^2 * L)` | `O(N + D)` |
| **9** | Array Stack Implementation | Array with Top Index | `O(1)` all ops | `O(Cap)` |
| **10** | Kth Largest Element in Array | Min-Heap of Size K | `O(N log K)` | `O(K)` |
| **11** | Longest Valid Parentheses | Stack of Indices (Base -1) | `O(N)` | `O(N)` |
| **12** | Move Zeroes | Two-Pointer In-Place Swap | `O(N)` | `O(1)` |
| **13** | Majority Element (Boyer-Moore Voting) | Candidate & Count Cancellation | `O(N)` | `O(1)` |
| **14** | Design HashMap (MyHashMap) | Separate Chaining (Bucket Lists) | `O(1)` avg | `O(N)` |
| **15** | Median of Row-Wise Sorted Matrix | Binary Search on Range + UB | `O(R * log C * log(max - min))` | `O(1)` |
| **16** | Find Peak Element in 2D Grid | Column Binary Search + Column Max | `O(M log N)` | `O(1)` |
| **17** | Kth Missing Positive Number | Binary Search on Missing Count | `O(log N)` | `O(1)` |
| **18** | Summary Ranges | Sorted Set Range Traversal | `O(log N)` add, `O(N)` get | `O(N)` |
| **19** | Design Twitter | Multi-Way Heap Merge + Hash Map | `O(K log F)` feed | `O(U + T)` |
| **20** | Water and Jug Problem (BFS) | BFS 6-State Graph Traversal | `O(X * Y)` | `O(X * Y)` |
| **21** | Wiggle Subsequence | Greedy / DP Peak-Valley Tracking | `O(N)` | `O(1)` |
| **22** | Run-Length Encoding (Compress) | Two-Pointer Suffix Counter Scan | `O(N)` | `O(N)` |
| **23** | Decode String | Nested Number & String Stacks | `O(N)` | `O(N)` |
| **24** | 132 Pattern | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **25** | Find All Numbers Disappeared in Array | Cyclic Sort In-Place Hashing | `O(N)` | `O(1)` |
| **26** | Kth Smallest in Lexicographical Order | Trie Prefix Tree Level Skipping | `O(log^2 N)` | `O(1)` |
| **27** | Kth Smallest Element in Sorted Matrix | Binary Search on Value Range + UB | `O(N log M * log(max - min))` | `O(1)` |
| **28** | Longest Nice Subarray (Pairwise AND = 0) | Sliding Window + Cumulative OR | `O(N)` | `O(1)` |

---

### Notebook 7: Course Scheduling, Peaks, Pancake Sort, Jump Games, Stock Series & House Robber

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Course Schedule I (Cycle Detection) | Kahn's Algorithm / In-degree BFS | `O(V + E)` | `O(V + E)` |
| **2** | Course Schedule II (Topological Sort Order) | Kahn's Topological Sort BFS | `O(V + E)` | `O(V + E)` |
| **3** | Parallel Courses (Min Semesters / Graph Depth)| Level-Order Kahn's BFS (Layer Steps)| `O(V + E)` | `O(V + E)` |
| **4** | Find Peak Element (1D Peak Detection) | Binary Search on Slope Inflection | `O(log N)` | `O(1)` |
| **5** | Find a Peak Element II (2D Peak Grid) | Binary Search on Columns + Row Max | `O(M log N)` | `O(1)` |
| **6** | Pancake Sorting (Prefix Reversals) | Greedy Suffix Placement + 2 Reversals | `O(N^2)` | `O(N)` |
| **7** | Jump Game I (Reachability Check) | Greedy Farthest Reachable Index | `O(N)` | `O(1)` |
| **8** | Jump Game II (Min Jumps) | Greedy BFS Horizon Interval | `O(N)` | `O(1)` |
| **9** | Jump Game III (Reach Zero Value) | Graph BFS Reachability | `O(N)` | `O(N)` |
| **10** | Jump Game IV (Min Jumps with Same Values) | BFS + Same-Value Pruning | `O(N)` | `O(N)` |
| **11** | Jump Game V (Max Visited Indices) | Memoized DFS / DAG Longest Path | `O(N * D)` | `O(N)` |
| **12** | Jump Game VI (Max Score Sliding Window) | Monotonic Deque Sliding Window DP | `O(N)` | `O(N)` |
| **13** | Jump Game VII (String Jump Reachability) | Sliding Window Reachability DP | `O(N)` | `O(N)` |
| **14** | Stock I (At Most 1 Transaction) | Running Minimum Price Tracking | `O(N)` | `O(1)` |
| **15** | Stock II (Unlimited Transactions) | Greedy Positive Slope Harvest | `O(N)` | `O(1)` |
| **16** | Stock III (At Most 2 Transactions) | 4-State Finite State Machine | `O(N)` | `O(1)` |
| **17** | Stock IV (At Most K Transactions) | 1D DP K-Transaction Compression | `O(N * K)` | `O(K)` |
| **18** | Stock with Cooldown (1-Day Rest) | 3-State Machine (Held, Sold, Rest) | `O(N)` | `O(1)` |
| **19** | Stock with Transaction Fee | 2-State Machine (Cash, Hold) | `O(N)` | `O(1)` |
| **20** | House Robber I (Linear Street) | 1D DP Space-Optimized (prev1, prev2) | `O(N)` | `O(1)` |
| **21** | House Robber II (Circular Street) | Circular Array 2-Pass Range DP | `O(N)` | `O(1)` |
| **22** | House Robber III (Binary Tree) | Post-Order Tree DP (Rob / Skip) | `O(N)` | `O(H)` |
| **23** | House Robber IV (Min Capability) | Binary Search on Min Capability | `O(N log(max - min))` | `O(1)` |
| **24** | Delete and Earn / Robber V (Value Domain) | Frequency Array Reduction to DP | `O(N + max(nums))` | `O(max(nums))` |
