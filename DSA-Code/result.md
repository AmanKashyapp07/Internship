# Master Problem Summary & Complexity Reference (Notebooks 1–10)

A unified reference catalog of **278 unique Data Structures & Algorithms problems** implemented across `notebook-1.cpp` through `notebook-10.cpp` (all duplicates and prior sorting implementations consolidated exclusively into Notebook 10). Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities.

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
| **8** | Longest Increasing Subsequence (LIS) | Patience Sorting (lower_bound) | `O(N log N)` | `O(N)` |
| **9** | Longest Common Subsequence (Reconstruction) | 2D Dynamic Programming Table | `O(N * M)` | `O(N * M)` |
| **10** | Interval DP Template | DP over Subproblem Lengths | `O(N^3)` | `O(N^2)` |
| **11** | Subset Enumeration & Sum of All Subsets | Bitmask Enumeration [0...2^N - 1] | `O(N * 2^N)` | `O(N * 2^N)` |
| **12** | Kadane's Max Subarray Sum with Indices | Running Prefix Sum Reset | `O(N)` | `O(1)` |
| **13** | Permutation Generation | Bitmask Backtracking | `O(N! * N)` | `O(N! * N)` |
| **14** | Longest Palindromic Subsequence (LPS) | Interval DP / LCS with Reverse | `O(N^2)` | `O(N^2)` |
| **15** | Check Subsequence | Greedy Two-Pointer Linear Scan | `O(len(S))` | `O(1)` |
| **16** | Precompute Palindromic Subarrays Table | 2D Interval DP over Lengths | `O(N^2)` | `O(N^2)` |
| **17** | Booth's Algorithm (Min String Rotation) | Two-Pointer Mismatch on (S + S) | `O(N)` | `O(N)` |
| **18** | Minimum Excluded Value (MEX) | Hash Set Presence Lookup | `O(N)` | `O(N)` |
| **19** | Matrix Multiplication & Exponentiation | Binary Exponentiation on Matrices | `O(N^3 log B)` | `O(N^2)` |
| **20** | N-th Fibonacci (Matrix Exponentiation) | [[1, 1], [1, 0]] Matrix Power | `O(log N)` | `O(1)` |
| **21** | Total Area Covered by Two 2D Rectangles | Inclusion-Exclusion Geometry | `O(1)` | `O(1)` |
| **22** | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n | `O(N)` | `O(N)` |

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
| **7** | Subarray Bitwise ORs | Set DP / Monotonic Frontier Values | `O(N * 30)` | `O(N * 30)` |
| **8** | Count Total Set Bits (1 to N) | Periodic Bit Position Math | `O(log N)` | `O(1)` |
| **9** | Minimum Subset Sum Difference | Subset Sum DP <= Total / 2 | `O(N * Total)` | `O(Total)` |
| **10** | Count of Longest Increasing Subsequences | 1D DP (Length & Count Arrays) | `O(N^2)` | `O(N)` |
| **11** | Dijkstra's Shortest Path | Min-Heap Priority Queue | `O((V + E) log V)` | `O(V + E)` |
| **12** | Bellman-Ford Algorithm | DP Edge Relaxation (V - 1 Passes) | `O(V * E)` | `O(V)` |
| **13** | Shortest Path on Unweighted Graph (BFS) | BFS Queue Level-by-Level Scan | `O(V + E)` | `O(V)` |
| **14** | Floyd-Warshall All-Pairs Shortest Path | Intermediate Vertex DP (k-loop) | `O(V^3)` | `O(V^2)` |
| **15** | Negative Cycle Detection (Floyd-Warshall) | Diagonal Self-Distance Inspection | `O(V^3)` | `O(V^2)` |
| **16** | Lexicographical Topological Sort | Min-Heap Kahn's BFS | `O(V log V + E)` | `O(V)` |
| **17** | Shortest Path on DAG via Topo Order | TopoSort Sequential Relaxation | `O(V + E)` | `O(V)` |
| **18** | Graph M-Coloring (Backtracking) | Backtracking DFS with Safety Check | `O(M^V)` | `O(V)` |
| **19** | Cheapest Flights within K Stops | State-Extended Dijkstra (stops) | `O(E * K)` | `O(V * K)` |
| **20** | Shortest Path Visiting All Nodes | Multi-Source Bitmask BFS | `O(V * 2^V)` | `O(V * 2^V)` |
| **21** | Shortest Common Supersequence (SCS) Length | Reduction: len(S1) + len(S2) - LCS | `O(len(S1) * len(S2))` | `O(len(S1) * len(S2))` |
| **22** | Reconstruct Shortest Common Supersequence | 2D LCS Table Backtracking | `O(len(S1) * len(S2))` | `O(len(S1) * len(S2))` |
| **23** | Minimum Window Subsequence | Forward Match + Backward Shrink | `O(len(S) * len(T))` | `O(1)` |
| **24** | Minimum Window Substring | Sliding Window with Match Counter | `O(len(S) + len(T))` | `O(len(S) + len(T))` |
| **25** | Matrix Chain Multiplication | Interval DP over Chain Lengths | `O(N^3)` | `O(N^2)` |
| **26** | Distinct Subsequences | 1D Space-Optimized DP (Backwards) | `O(len(S) * len(T))` | `O(len(T))` |
| **27** | Maximum Sum BST in Binary Tree | Post-Order Bottom-Up DFS | `O(N)` | `O(H)` |
| **28** | Remove Leaf Nodes with Target Value | Post-Order Recursive Tree Pruning | `O(N)` | `O(H)` |
| **29** | Minimum Extra Characters in String | 1D Memoized DP + Hash Set | `O(N^2)` | `O(N + D)` |
| **30** | Bounded Knapsack (Binary Power Split) | Binary Split + 1D 0/1 Knapsack | `O(W * log K)` | `O(W)` |
| **31** | LCS Length of 2 Permutations | Index Map + Patience Sorting LIS | `O(N log N)` | `O(N)` |
| **32** | Longest Common Increasing Subseq (LCIS) | 1D DP with Optimal Prefix Tracker | `O(N * M)` | `O(M)` |
| **33** | Possible Path Lengths in DAG | TopoSort + 2D Reachability DP | `O(V * N + E * N)` | `O(V * N)` |

---

### Notebook 4: Binary Lifting, Monotonic Stacks, Digit DP & Slope Trick

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | K-th Element & Median of 2 Sorted Arrays | Binary Search on Partition Cut | `O(log(min(M, N)))` | `O(1)` |
| **2** | Longest Substring Without Repeating Chars | Sliding Window + Last Seen Map | `O(N)` | `O(min(N, Alphabet))` |
| **3** | Min Moves to Gather K Consecutive Ones | Shifted Index + Median Prefix Sum | `O(N)` | `O(N)` |
| **4** | 2D Prefix Sum Matrix | 2D Inclusion-Exclusion Prefix DP | `O(M * N)` | `O(M * N)` |
| **5** | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values | `O(N * 30)` | `O(N)` |
| **6** | Multiply Two 2D Matrices | 3-Nested Loop Dot Products | `O(M * N * P)` | `O(M * P)` |
| **7** | Count Subsequences with Sum Equal to K | 1D 0/1 Knapsack DP (Backwards) | `O(N * K)` | `O(K)` |
| **8** | Max Components Tree Split with Equal Sum | Divisors + Subtree DFS Cuts | `O(N * divisors(Total))` | `O(N)` |
| **9** | Wildcard Pattern Matching | 2D DP Memoization ('?' and '*') | `O(len(S) * len(P))` | `O(len(S) * len(P))` |
| **10** | Next Greater Element | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **11** | Next Smaller Element | Monotonic Increasing Stack | `O(N)` | `O(N)` |
| **12** | Previous Greater Element | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **13** | Previous Smaller Element | Monotonic Increasing Stack | `O(N)` | `O(N)` |
| **14** | Stock Span Problem | Monotonic Stack of Indices | `O(N)` | `O(N)` |
| **15** | Next Greater Element in Circular Array | Monotonic Stack on Doubled (2N) | `O(N)` | `O(N)` |
| **16** | Sum of Subarray Minimums | Monotonic Stack Contribution | `O(N)` | `O(N)` |
| **17** | Maximum Score of Good Subarray | Monotonic Stack Range Bounds (K) | `O(N)` | `O(N)` |
| **18** | Sum of Subarray Maximums | Monotonic Stack Contribution | `O(N)` | `O(N)` |
| **19** | Max of Minimums for Every Window Size | Monotonic Stack + Suffix Max | `O(N)` | `O(N)` |
| **20** | Array Median using Quickselect | Randomized QuickSelect (Lomuto) | `O(N)` avg | `O(1)` |
| **21** | N x N MEX Grid Construction | Bitwise XOR Matrix (i ^ j) | `O(N^2)` | `O(N^2)` |
| **22** | Binary Lifting (LCA, K-th Ancestor) | Ancestor Doubling Table up[u][j] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **23** | Digit DP Template | Digit Memo (pos,start,tight,state) | `O(digits * states)` | `O(digits * states)` |
| **24** | Cycle Detection & Reconstruction | DFS Parent Trace / Bellman-Ford | `O(V + E) / O(V * E)` | `O(V)` |
| **25** | Make Array Non-Decreasing (Slope Trick) | Greedy Max-Heap Slope Inflection | `O(N log N)` | `O(N)` |

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
| **19** | Count Reverse Pairs (a[i] > x * a[j]) | Modified Merge Sort Counting | `O(N log N)` | `O(N)` |
| **20** | Find Median via std::nth_element | QuickSelect Median via STL | `O(N)` avg | `O(1)` |
| **21** | Longest Substring with At Least K Frequency | Sliding Window per Unique Target | `O(26 * N)` | `O(1)` |
| **22** | Cyclic Right and Left Rotation | 3-Step Range Reversal Algorithm | `O(N)` | `O(1)` |
| **23** | LRU Cache | Hash Map + Doubly Linked List | `O(1)` | `O(Cap)` |
| **24** | LFU Cache | Hash Map + Freq-to-List + minFreq | `O(1)` get/put | `O(Cap)` |
| **25** | Gas Station Circuit & String Utils | Greedy Net Surplus & Tank Reset | `O(N)` | `O(1)` |

---

### Notebook 6: Sliding Windows, 2D Matrix Binary Search & Custom Classes

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Container With Most Water | Inward Converging Two Pointers | `O(N)` | `O(1)` |
| **2** | Spiral Matrix Traversal | Layer Boundary Simulation | `O(M * N)` | `O(1)` |
| **3** | Merge Two Sorted Arrays | Two-Pointer Linear Merge Step | `O(N + M)` | `O(N + M)` |
| **4** | Longest Consecutive Sequence | Hash Set Boundary (count(x - 1)) | `O(N)` | `O(N)` |
| **6** | First Missing Positive | Cyclic Sort In-Place Hashing | `O(N)` | `O(1)` |
| **7** | Word Break | 1D Dynamic Programming + Set | `O(N^2 * L)` | `O(N + D)` |
| **8** | Array Stack Implementation | Array with Top Index | `O(1)` all ops | `O(Cap)` |
| **9** | Kth Largest Element in Array | Min-Heap of Size K | `O(N log K)` | `O(K)` |
| **10** | Move Zeroes | Two-Pointer In-Place Swap | `O(N)` | `O(1)` |
| **11** | Majority Element (Boyer-Moore Voting) | Candidate & Count Cancellation | `O(N)` | `O(1)` |
| **12** | Design HashMap (MyHashMap) | Separate Chaining (Bucket Lists) | `O(1)` avg | `O(N)` |
| **13** | Median of Row-Wise Sorted Matrix | Binary Search on Range + UB | `O(R * log C * log(max - min))` | `O(1)` |
| **14** | Kth Missing Positive Number | Binary Search on Missing Count | `O(log N)` | `O(1)` |
| **15** | Summary Ranges | Sorted Set Range Traversal | `O(log N)` add, `O(N)` get | `O(N)` |
| **16** | Design Twitter | Multi-Way Heap Merge + Hash Map | `O(K log F)` feed | `O(U + T)` |
| **17** | Water and Jug Problem (BFS) | BFS 6-State Graph Traversal | `O(X * Y)` | `O(X * Y)` |
| **18** | Wiggle Subsequence | Greedy / DP Peak-Valley Tracking | `O(N)` | `O(1)` |
| **19** | Run-Length Encoding (Compress) | Two-Pointer Suffix Counter Scan | `O(N)` | `O(N)` |
| **20** | Decode String | Nested Number & String Stacks | `O(N)` | `O(N)` |
| **21** | 132 Pattern | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **22** | Find All Numbers Disappeared in Array | Cyclic Sort In-Place Hashing | `O(N)` | `O(1)` |
| **23** | Kth Smallest in Lexicographical Order | Trie Prefix Tree Level Skipping | `O(log^2 N)` | `O(1)` |
| **24** | Kth Smallest Element in Sorted Matrix | Binary Search on Value Range + UB | `O(N log M * log(max - min))` | `O(1)` |
| **25** | Longest Nice Subarray (Pairwise AND = 0) | Sliding Window + Cumulative OR | `O(N)` | `O(1)` |

---

### Notebook 7: Graph Sequences, Peaks, DP State Machines, Coin Change & Special Sorts

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Course Schedule I (Cycle Detection) | Kahn's Algorithm / In-degree BFS | `O(V + E)` | `O(V + E)` |
| **2** | Course Schedule II (Topological Sort Order) | Kahn's Topological Sort BFS | `O(V + E)` | `O(V + E)` |
| **3** | Parallel Courses (Min Semesters / Graph Depth)| Level-Order Kahn's BFS (Layer Steps)| `O(V + E)` | `O(V + E)` |
| **4** | Find Peak Element (1D Peak Detection) | Binary Search on Slope Inflection | `O(log N)` | `O(1)` |
| **5** | Find a Peak Element II (2D Peak Grid) | Binary Search on Columns + Row Max | `O(M log N)` | `O(1)` |
| **6** | Jump Game I (Reachability Check) | Greedy Farthest Reachable Index | `O(N)` | `O(1)` |
| **7** | Jump Game II (Min Jumps) | Greedy BFS Horizon Interval | `O(N)` | `O(1)` |
| **8** | Jump Game III (Reach Zero Value) | Graph BFS Reachability | `O(N)` | `O(N)` |
| **9** | Jump Game IV (Min Jumps with Same Values) | BFS + Same-Value Pruning | `O(N)` | `O(N)` |
| **10** | Jump Game V (Max Visited Indices) | Memoized DFS / DAG Longest Path | `O(N * D)` | `O(N)` |
| **11** | Jump Game VI (Max Score Sliding Window) | Monotonic Deque Sliding Window DP | `O(N)` | `O(N)` |
| **12** | Jump Game VII (String Jump Reachability) | Sliding Window Reachability DP | `O(N)` | `O(N)` |
| **13** | Stock I (At Most 1 Transaction) | Running Minimum Price Tracking | `O(N)` | `O(1)` |
| **14** | Stock II (Unlimited Transactions) | Greedy Positive Slope Harvest | `O(N)` | `O(1)` |
| **15** | Stock III (At Most 2 Transactions) | 4-State Finite State Machine | `O(N)` | `O(1)` |
| **16** | Stock IV (At Most K Transactions) | 1D DP K-Transaction Compression | `O(N * K)` | `O(K)` |
| **17** | Stock with Cooldown (1-Day Rest) | 3-State Machine (Held, Sold, Rest) | `O(N)` | `O(1)` |
| **18** | Stock with Transaction Fee | 2-State Machine (Cash, Hold) | `O(N)` | `O(1)` |
| **19** | House Robber I (Linear Street) | 1D DP Space-Optimized (prev1, prev2) | `O(N)` | `O(1)` |
| **20** | House Robber II (Circular Street) | Circular Array 2-Pass Range DP | `O(N)` | `O(1)` |
| **21** | House Robber III (Binary Tree) | Post-Order Tree DP (Rob / Skip) | `O(N)` | `O(H)` |
| **22** | House Robber IV (Min Capability) | Binary Search on Min Capability | `O(N log(max - min))` | `O(1)` |
| **23** | Delete and Earn / Robber V (Value Domain) | Frequency Array Reduction to DP | `O(N + max(nums))` | `O(max(nums))` |
| **24** | Coin Change I — Fewest (LeetCode 322) | 1D Unbounded Knapsack DP (Min) | `O(N * amount)` | `O(amount)` |
| **25** | Coin Change II — Combinations (LC 518) | 1D Unbounded Combination DP | `O(N * amount)` | `O(amount)` |
| **26** | Coin Change Permutations (LeetCode 377) | 1D Unbounded Permutation DP | `O(N * amount)` | `O(amount)` |
| **27** | Bounded Coin Change (Limited Supply) | Binary Splitting + 0/1 Knapsack | `O(amount * sum(log(limit)))` | `O(amount)` |
| **28** | Coin Change Path Reconstruction | DP Predecessor Pointer Traceback | `O(N * amount)` | `O(amount)` |
| **29** | Reachable Money Sums (CSES Money Sums) | 0/1 Knapsack Boolean Reachability | `O(N * sum(coins))` | `O(sum(coins))` |
| **30** | Redundant Connection (Undirected Graph) | DSU / Union-Find Cycle Detection | `O(N * α(N))` | `O(N)` |
| **31** | Redundant Connection II (Directed Graph) | 2-Parent Edge + DSU Directed Cycle | `O(N * α(N))` | `O(N)` |
| **32** | Valid Parentheses (LeetCode 20) | Stack Character Matching (3 Pairs) | `O(N)` | `O(N)` |
| **33** | Generate Parentheses (LeetCode 22) | Backtracking Open/Close Invariant | `O(4^N / sqrt(N))` | `O(N)` |
| **34** | Longest Valid Parentheses (LeetCode 32) | Stack with -1 Base Index Sentinel | `O(N)` | `O(N)` |
| **35** | Valid Parentheses with Wildcard '*' (LC 678) | Greedy Min-Max Bounds [cmin, cmax] | `O(N)` | `O(1)` |
| **36** | Min Add to Make Valid (LeetCode 921) | Open & Close Counter Balance | `O(N)` | `O(1)` |
| **37** | Min Remove to Make Valid (LeetCode 1249) | Stack Index Filter & String Rebuild | `O(N)` | `O(N)` |
| **38** | Remove Invalid Parentheses (LeetCode 301) | BFS Level-Order Minimal Deletion | `O(2^N * N)` | `O(2^N)` |
| **39** | Score of Parentheses (LeetCode 856) | Depth Bit-Shift Math (1 << depth) | `O(N)` | `O(1)` |
| **40** | Min Swaps to Make Balanced (LeetCode 1963) | Imbalance Math: (maxImb + 1) / 2 | `O(N)` | `O(1)` |
| **41** | Different Ways to Add Parentheses (LC 241) | Divide-and-Conquer + DP Memo | `O(Catalan(N))` | `O(Catalan(N))` |
| **42** | Remove Outermost Parentheses (LC 1021) | Open Counter Primitive Decomp | `O(N)` | `O(N)` |
| **43** | Can Be Valid with Locked Bits (LC 2116) | 2-Pass Forward & Backward Balance | `O(N)` | `O(1)` |

---

### Notebook 8: Popular Interval Problems (Greedy, Sweep-Line, BST, Heaps & DP)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Merge Intervals (LeetCode 56) | Start-Time Sorting + Running Max | `O(N log N)` | `O(N)` |
| **2** | Insert Interval (LeetCode 57) | 3-Stage Linear Range Merge Scan | `O(N)` | `O(N)` |
| **3** | Non-overlapping Intervals (LeetCode 435) | Greedy End-Time Interval Schedule | `O(N log N)` | `O(1)` |
| **4** | Meeting Rooms I (LeetCode 252) | Start-Time Sorting + Overlap Check | `O(N log N)` | `O(1)` |
| **5** | Meeting Rooms II (LeetCode 253) | Min-Heap Active Rooms / Sweep Line | `O(N log N)` | `O(N)` |
| **6** | Meeting Rooms III (LeetCode 2402) | Dual Min-Heaps (Free & Busy Rooms) | `O(M log M + M log N)` | `O(N)` |
| **7** | Interval List Intersections (LeetCode 986) | Two-Pointer Boundary Intersection | `O(N + M)` | `O(N + M)` |
| **8** | Min Arrows to Burst Balloons (LC 452) | Greedy End-Coordinate Pinning | `O(N log N)` | `O(1)` |
| **9** | Employee Free Time (LeetCode 759) | Multi-Way Heap / Gap Extraction | `O(N log N)` | `O(N)` |
| **10** | Data Stream Disjoint Intervals (LC 352) | Balanced BST (`std::set`) Neighbor Merge | `O(log N)` add, `O(N)` get | `O(N)` |
| **11** | My Calendar I (LeetCode 729) | `std::set` `lower_bound` Boundary BST | `O(log N)` per book | `O(N)` |
| **12** | My Calendar II (LeetCode 731) | Dual Overlap List Verification | `O(N)` per book | `O(N)` |
| **13** | My Calendar III (LeetCode 732) | Sweep-Line Difference Map (`std::map`) | `O(N)` per book | `O(N)` |
| **14** | Car Pooling (LeetCode 1094) | Timestamp Difference Array / Sweep | `O(N + MaxLocation)` | `O(MaxLocation)` |
| **15** | Corporate Flight Bookings (LeetCode 1109) | 1D Range Difference Array + Prefix Sum | `O(N + Bookings)` | `O(N)` |
| **16** | Range Module (LeetCode 715) | `std::map` Disjoint Interval Slices | `O(K log N)` per op | `O(N)` |
| **17** | Count Integers in Intervals (LC 2276) | `std::map` Dynamic Overlap Merging | Amortized `O(log N)` | `O(N)` |
| **18** | Remove Covered Intervals (LeetCode 1288) | Custom Sort (Start Asc, End Desc) | `O(N log N)` | `O(1)` |
| **19** | Video Stitching (LeetCode 1024) | Greedy Jump Interval Max Reach | `O(N + Time)` | `O(Time)` |
| **20** | Maximum Length of Pair Chain (LC 646) | Greedy End-Time Selection (LIS DP) | `O(N log N)` | `O(1)` |
| **21** | Min Interval to Include Each Query (LC 1851)| Offline Sorted Queries + Min-Heap | `O(N log N + Q log Q)` | `O(N + Q)` |
| **22** | Area Painted Each Day (LeetCode 2158) | Jump-Table / DSU Path Compression | `O(N + MaxPosition)` | `O(MaxPosition)` |
| **23** | Partition Labels (LeetCode 763) | Character Last Occurrence Interval | `O(N)` | `O(1)` |
| **24** | Teemo Attacking (LeetCode 495) | Timeline Overlap Accumulation | `O(N)` | `O(1)` |
| **25** | Task Scheduler (LeetCode 621) | Idle Interval Slot Math | `O(N)` | `O(1)` |

---

### Notebook 9: Popular Data Stream & Online Algorithm Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Find Median from Data Stream (LC 295) | Dual Balanced Heaps (Max/Min) | `O(log N)` add, `O(1)` get | `O(N)` |
| **2** | Kth Largest in a Stream (LeetCode 703) | Fixed-Size K Min-Heap Stream | `O(log K)` per add | `O(K)` |
| **3** | Moving Average from Stream (LeetCode 346) | Sliding Circular Buffer / Queue | `O(1)` per next | `O(W)` |
| **4** | First Unique Number in Stream (LC 1429) | Doubly Linked List + Hash Map | `O(1)` all ops | `O(N)` |
| **5** | First Non-Repeating Character in Stream | Frequency Table + FIFO Queue | `O(1)` avg | `O(1)` |
| **6** | Online Stock Span (LeetCode 901) | Monotonic Decreasing Stack | `O(1)` avg per query | `O(N)` |
| **7** | Design Hit Counter (LeetCode 362) | Circular Buckets (300s) / Queue | `O(1)` hit / `O(1)` get | `O(1)` |
| **8** | Logger Rate Limiter (LeetCode 359) | Hash Map Expiration Timestamps | `O(1)` per message | `O(M)` |
| **9** | Front Middle Back Queue (LeetCode 1670) | Dual Balanced Deques (Left/Right) | `O(1)` all operations | `O(N)` |
| **10** | Finding MK Average (LeetCode 1825) | 3 Multisets (Left, Mid, Right) | `O(log M)` add, `O(1)` avg | `O(M)` |
| **11** | Stream of Characters (LeetCode 1032) | Reversed Suffix Trie + Stream Log | `O(L)` per query | `O(Trie)` |
| **12** | Snapshot Array (LeetCode 1146) | History Vectors + Binary Search | `O(1)` set, `O(log S)` get | `O(N + U)` |
| **13** | Stock Price Fluctuation (LeetCode 2034) | Hash Map + Balanced Multiset | `O(log N)` update, `O(1)` get| `O(N)` |
| **14** | Time Based Key-Value Store (LeetCode 981) | Hash Map + Sorted Vector UB | `O(1)` set, `O(log T)` get | `O(K * T)` |
| **15** | Sliding Window Median (LeetCode 480) | Dual Multisets / Lazy Heap Deletes| `O(N log K)` | `O(K)` |
| **16** | Maximum Frequency Stack (LeetCode 895) | Frequency Map + Stacks per Level | `O(1)` push & pop | `O(N)` |
| **17** | Seat Reservation Manager (LeetCode 1845) | Min-Heap of Free Seat IDs | `O(log N)` reserve/unreserve| `O(N)` |
| **18** | Design Underground System (LeetCode 1396) | Dual Hash Maps (Transit & Stats) | `O(1)` all operations | `O(P + S^2)`|
| **19** | Number of Recent Calls (LeetCode 933) | Sliding Window Queue [t-3000, t] | `O(1)` avg per ping | `O(W)` |
| **20** | Design Leaderboard (LeetCode 1244) | Hash Map + Min-Heap Top-K | `O(1)` add, `O(N log K)` top | `O(P)` |
| **21** | Reservoir Sampling from Stream (LC 382/398)| Algorithm R Random Replacement | `O(1)` per item | `O(1)` |
| **22** | Bounded Blocking Queue (LeetCode 1188) | Mutex + Dual Condition Variables | `O(1)` all operations | `O(Cap)` |
| **23** | Majority Checker in Subarray (LC 1154) | Boyer-Moore Random Sampling + UB | `O(K log N)` per query | `O(N)` |
| **24** | Design Authentication Manager (LC 1797) | Hash Map Expiry with Lazy Purge | `O(1)` gen, `O(N)` count | `O(T)` |
| **25** | Multi-Policy Online Cache Engine (LRU/TTL) | DLL + Hash Map + Expiry Min-Heap | `O(1)` avg get/put | `O(Cap)` |

---

### Notebook 10: Famous Sorting Concepts, Algorithms & Interview Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Bubble Sort with Early Stop & Round Count | Adjacent Swaps + Max Displacement | `O(N^2)` worst/avg, `O(N)` best | `O(1)` |
| **2** | Selection Sort (Min-Element Swap) | Suffix Minimum Index Extraction | `O(N^2)` all cases | `O(1)` |
| **3** | Insertion Sort | Shift Larger Prefix In-Place | `O(N + I)` adaptive | `O(1)` |
| **4** | Merge Sort (Classic Divide & Conquer) | Recursive 2-Way Merge Subroutine | `O(N log N)` guaranteed | `O(N)` |
| **5** | In-Place Merge Sort (Gap Method) | Shell-like Gap Reduction | `O(N log^2 N)` | `O(1)` |
| **6** | QuickSort (Lomuto & Hoare Partitioning) | Two-Way Pivot Partitioning | `O(N log N)` avg, `O(N^2)` worst | `O(log N)` |
| **7** | Randomized QuickSort (Introsort Hybrid) | Random Pivot Selection | `O(N log N)` expected | `O(log N)` |
| **8** | 3-Way QuickSort (Bentley-McIlroy DNF) | Duplicates Pivot Segregation | `O(N log N)` avg, `O(N)` best | `O(log N)` |
| **9** | HeapSort (In-Place Max-Heapify) | Sift-Down In-Place Max Extraction | `O(N log N)` guaranteed | `O(1)` |
| **10** | ShellSort (Knuth's Gap Sequence) | Diminishing Increment Insertion | `O(N^(3/2))` | `O(1)` |
| **11** | TimSort Concept (Small Runs Insertion Merge)| Run Detection + Insertion + Merge | `O(N log N)` worst, `O(N)` best | `O(N)` |
| **12** | Counting Sort (Stable Frequency Table) | Cumulative Prefix Sum Table | `O(N + K)` | `O(N + K)` |
| **13** | Radix Sort LSD (Base-10 Digit-by-Digit) | Digit-by-Digit Stable Counting | `O(D * (N + B))` | `O(N + B)` |
| **14** | Radix Sort MSD (Recursive Bucket Sort) | Trie-like MSD Prefix Partitioning | `O(N * L)` | `O(N + B * D)` |
| **15** | Bucket Sort (Uniform Real Range [0, 1)) | Scatter-Gather Bucketing + Sort | `O(N)` avg, `O(N^2)` worst | `O(N)` |
| **16** | Cyclic Sort [1 to N] (In-Place Swaps) | Direct Index Mapping Swap | `O(N)` | `O(1)` |
| **17** | Pigeonhole Sort (Direct Range Slots) | Range-Sized Bucket Placement | `O(N + R)` | `O(N + R)` |
| **18** | Pancake Sorting (LeetCode 969) | Greedy Suffix Placement (2-Flips) | `O(N^2)` | `O(N)` |
| **19** | Sort Colors / DNF 3-Way (LeetCode 75) | In-Place 3-Pointer Partition | `O(N)` | `O(1)` |
| **20** | Largest Number from Concatenation (LC 179) | Custom Transitive String Comp | `O(N log N * L)` | `O(N * L)` |
| **21** | Wiggle Sort I & II (LeetCode 280 / 324) | Greedy Swap / Median + DNF Index | `O(N)` | `O(1)` |
| **22** | Topological Sort vs Comparison Sort | Kahn's In-Degree BFS DAG Sort | `O(V + E)` | `O(V + E)` |
| **23** | Patience Sorting & LIS (Greene's Theorem) | Pile Insertion via lower_bound | `O(N log N)` | `O(N)` |
| **24** | Sort Characters By Frequency (LeetCode 451) | Bucket Sorting on Frequency Count | `O(N)` | `O(N)` |
| **25** | Sort Matrix Diagonally (LeetCode 1329) | Hash Map Diagonal Bucketing | `O(M * N * log(min(M,N)))` | `O(M * N)` |
| **26** | Multi-Criteria Custom Sorting Lambda | Multi-Field Tie-Breaking Predicate| `O(N log N)` | `O(1)` |
| **27** | Min Swaps to Sort Permutation | Disjoint Cycle Decomposition | `O(N log N)` | `O(N)` |
| **28** | Count Inversions & Reverse Pairs (LC 493) | Merge Sort Inversion Accumulator | `O(N log N)` | `O(N)` |
| **29** | Sort Transformed Array (LeetCode 360) | Parabola Concavity Two-Pointers | `O(N)` | `O(N)` |
