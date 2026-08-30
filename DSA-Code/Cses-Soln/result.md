# CSES Problem Solutions — Problem Summary & Complexity Reference

A structured reference catalog of all **232 solved CSES problems** in `DSA-Code/Cses-Soln`. Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities categorized by domain.

## 📘 Category-by-Category Analysis

### 1. Introductory Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Weird Algorithm | Collatz Conjecture Simulation | `O(steps)` | `O(1)` |
| **2** | Missing Number | Sum Formula / Bitwise XOR Cumulative | `O(N)` | `O(1)` |
| **3** | Repetitions | Two-Pointer / Sliding Window Max Run | `O(N)` | `O(1)` |
| **4** | Increasing Array | Greedy Prefix Max Monotonicity | `O(N)` | `O(1)` |
| **5** | Permutations | Interleaved Evens and Odds Construction | `O(N)` | `O(N)` |
| **6** | Number Spiral | Math Closed Form / Layer Boundary Calculation | `O(1)` | `O(1)` |
| **7** | Two Knights | Combinatorics — Non-Attacking Knight Pairs | `O(1)` | `O(1)` |
| **8** | Two Sets | Modulo 4 Equi-Partition Construction | `O(N)` | `O(N)` |
| **9** | Bit Strings | Modular Binary Exponentiation (2^N mod M) | `O(log N)` | `O(1)` |
| **10** | Trailing Zeros | Legendre's Formula (Powers of 5 Factorization) | `O(log_5 N)` | `O(1)` |
| **11** | Coin Piles | System of Linear Equations Invariant Check | `O(1)` | `O(1)` |
| **12** | Palindrome Reorder | Character Frequency Counting + Symmetric Mirror | `O(N)` | `O(N)` |
| **13** | Gray Code | Bitwise Gray Conversion (i ^ (i >> 1)) | `O(2^N)` | `O(2^N)` |
| **14** | Mex Grid Construction | Bitwise XOR Matrix (i ^ j) | `O(N^2)` | `O(N^2)` |
| **15** | Creating Strings | Backtracking Permutations / STL next_permutation | `O(N! * N)` | `O(N)` |
| **16** | Apple Division | Bitmask Subset Difference Enumeration | `O(2^N)` | `O(N)` |
| **17** | Digit Queries | Digit Length Interval Math + Binary Search | `O(log K)` | `O(1)` |

---

### 2. Sorting and Searching

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Distinct Numbers | Sorting / Hash Set Presence Lookup | `O(N log N)` | `O(N)` |
| **2** | Apartments | Two-Pointer Inward Greedy Match | `O(N log N + M log M)` | `O(1)` |
| **3** | Ferris Wheel | Two-Pointer Greedy Pairing (Lightest + Heaviest) | `O(N log N)` | `O(1)` |
| **4** | Concert Tickets | Sorted Multiset Upper Bound Query and Erase | `O(M log N)` | `O(N)` |
| **5** | Restaurant Customers | Coordinate Event Sweepline (Arrival / Departure) | `O(N log N)` | `O(N)` |
| **6** | Movie Festival | Greedy Interval Scheduling (Earliest End Time) | `O(N log N)` | `O(N)` |
| **7** | Sum of Two Values | Two-Pointer Inward Scan on Sorted Indices | `O(N log N)` | `O(N)` |
| **8** | Nearest Smaller Values | Monotonic Increasing Stack of Indices | `O(N)` | `O(N)` |
| **9** | Maximum Subarray Sum | Kadane's Algorithm | `O(N)` | `O(1)` |
| **10** | Stick Lengths | Median Minimization via Sorting | `O(N log N)` | `O(1)` |
| **11** | Missing Coin Sum | Greedy Prefix Reachability (sum + 1 < x) | `O(N log N)` | `O(1)` |
| **12** | Collecting Numbers | Value-to-Index Forward Inversion Count | `O(N)` | `O(N)` |
| **13** | Collecting Numbers II | Dynamic Index Swaps + Local Neighbor Invariants | `O(N + M)` | `O(N)` |
| **14** | Playlist | Sliding Window + Last Seen Index Map | `O(N log N)` | `O(N)` |
| **15** | Towers | Binary Search upper_bound Greedy Placement | `O(N log N)` | `O(N)` |
| **16** | Traffic Lights | Segment Tree / Set Splits + Multiset Lengths | `O(N log N)` | `O(N)` |
| **17** | Josephus Problem I | Queue Simulation / Modular Math (Step 2) | `O(N)` | `O(N)` |
| **18** | Josephus Problem II | Fenwick Tree / PBDS Order Statistics | `O(N log N)` | `O(N)` |
| **19** | Nested Ranges Check | Sweepline + Running Suffix Min / Prefix Max | `O(N log N)` | `O(N)` |
| **20** | Nested Ranges Count | Sweepline + Fenwick Tree Rank Frequency | `O(N log N)` | `O(N)` |
| **21** | Room Allocation | Min-Heap Priority Queue on Active Room Departures | `O(N log N)` | `O(N)` |
| **22** | Factory Machines | Binary Search on Total Production Time | `O(N log(max_T))` | `O(1)` |
| **23** | Tasks and Deadlines | Greedy Shortest Processing Time (SPT) First | `O(N log N)` | `O(1)` |
| **24** | Reading Books | Greedy Split vs Largest Book Boundary (2 * max) | `O(N)` | `O(1)` |
| **25** | Sum of Three Values | Fix Pivot + Inward Two Pointers | `O(N^2)` | `O(N)` |
| **26** | Sum of Four Values | Pair Sums Hash Map / Meet-in-the-Middle | `O(N^2 log N)` | `O(N^2)` |
| **27** | Subarray Sums I | Sliding Window / Two Pointers (Positive Values) | `O(N)` | `O(1)` |
| **28** | Subarray Sums II | Prefix Sums + Hash Map Frequency Lookup | `O(N log N)` | `O(N)` |
| **29** | Subarray Divisibility | Prefix Sums Modulo N Hash Map Frequency | `O(N log N)` | `O(N)` |
| **30** | Distinct Values Subarrays | Sliding Window + Frequency Hash Map | `O(N)` | `O(N)` |
| **31** | Distinct Values Subarrays II | At Most K Distinct Sliding Window | `O(N)` | `O(K)` |
| **32** | Distinct Values Subsequences | Coordinate Compression + DP Frequency Counter | `O(N log N)` | `O(N)` |
| **33** | Array Division | Binary Search on Maximum Subarray Sum Bound | `O(N log(sum))` | `O(1)` |
| **34** | Movie Festival II | Greedy Multi-Screen Scheduling via Multiset | `O(N log K)` | `O(K)` |
| **35** | Maximum Subarray Sum II | Prefix Sums + Monotonic Deque / Multiset Window | `O(N)` | `O(N)` |

---

### 3. Dynamic Programming

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Dice Combinations | 1D Rolling DP Sum of Previous 6 States | `O(N)` | `O(1)` |
| **2** | Minimizing Coins | Unbounded Knapsack 1D DP Table | `O(N * X)` | `O(X)` |
| **3** | Coin Combinations I | Permutation Subset Sum 1D DP | `O(N * X)` | `O(X)` |
| **4** | Coin Combinations II | Combination Subset Sum 1D DP (Outer Coin Loop) | `O(N * X)` | `O(X)` |
| **5** | Removing Digits | Greedy / 1D DP Digit Subtraction | `O(N log_10 N)` | `O(N)` |
| **6** | Grid Paths I | 2D Grid DP with Obstacle Handling | `O(N^2)` | `O(N)` |
| **7** | Book Shop | 0/1 Knapsack 1D DP (Backwards Transition) | `O(N * X)` | `O(X)` |
| **8** | Array Description | 2D DP Adjacent Element Difference Constraint | `O(N * M)` | `O(M)` |
| **9** | Minimal Grid Path | 2D Grid DP Diagonal Greedy Path Minimization | `O(N^2)` | `O(N^2)` |
| **10** | Edit Distance | Levenshtein 2D Dynamic Programming Table | `O(N * M)` | `O(N * M)` |
| **11** | Rectangle Cutting | 2D Interval Grid DP (H x W) | `O(A * B * (A + B))` | `O(A * B)` |
| **12** | Money Sums | Reachable Subset Sums Boolean DP | `O(N * sum)` | `O(sum)` |
| **13** | Removal Game | 2D Interval Minimax Game DP | `O(N^2)` | `O(N^2)` |
| **14** | Two Sets II | Subset Sum DP (Target = N*(N+1)/4) | `O(N^3)` | `O(N^2)` |
| **15** | Increasing Subsequence | Patience Sorting / Binary Search (lower_bound) | `O(N log N)` | `O(N)` |
| **16** | Projects | Coordinate Compression + DP + Binary Search | `O(N log N)` | `O(N)` |
| **17** | Elevator Rides | Bitmask DP (rides, last_weight) | `O(N * 2^N)` | `O(2^N)` |
| **18** | Counting Numbers | Digit DP with Adjacent Digit Inequality | `O(digits * 10)` | `O(digits)` |
| **19** | Increasing Subsequence II | Fenwick Tree / Coordinate-Compressed DP | `O(N log N)` | `O(N)` |
| **20** | Longest Common Subsequence | 2D Dynamic Programming Table | `O(N * M)` | `O(N * M)` |

---

### 4. Graph Algorithms

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Counting Rooms | Connected Components via Multi-Source 2D BFS/DFS | `O(N * M)` | `O(N * M)` |
| **2** | Labyrinth | Grid BFS Shortest Path + Direction Traceback | `O(N * M)` | `O(N * M)` |
| **3** | Building Roads | Connected Components via DSU / DFS | `O(V + E)` | `O(V)` |
| **4** | Message Route | BFS Shortest Path on Unweighted Graph | `O(V + E)` | `O(V)` |
| **5** | Building Teams | 2-Coloring Bipartite Graph BFS/DFS | `O(V + E)` | `O(V)` |
| **6** | Round Trip | Undirected Cycle Detection via DFS Parent Trace | `O(V + E)` | `O(V)` |
| **7** | Monsters | Multi-Source BFS on Monsters + 1-Source Human BFS | `O(N * M)` | `O(N * M)` |
| **8** | Shortest Routes I | Dijkstra's Single-Source Shortest Path | `O((V + E) log V)` | `O(V + E)` |
| **9** | Shortest Routes II | Floyd-Warshall All-Pairs Shortest Path | `O(V^3)` | `O(V^2)` |
| **10** | High Score | Bellman-Ford Negative Cycle Detection | `O(V * E)` | `O(V)` |
| **11** | Flight Discount | State-Extended Dijkstra (Coupon Used / Not Used) | `O((V + E) log V)` | `O(V)` |
| **12** | Cycle Finding | Bellman-Ford Negative Cycle Recovery (V Passes) | `O(V * E)` | `O(V)` |
| **13** | Flight Routes | K-Shortest Paths via State-Tracked Dijkstra | `O(K * E log(K * V))` | `O(K * V)` |
| **14** | Round Trip II | Directed Cycle Detection via 3-Color DFS | `O(V + E)` | `O(V)` |
| **15** | Course Schedule | Kahn's Topological Sort BFS | `O(V + E)` | `O(V + E)` |
| **16** | Longest Flight Route | Topological Sort + DAG DP Distance Relaxation | `O(V + E)` | `O(V + E)` |
| **17** | Game Routes | Topological Sort + DAG Path Count DP | `O(V + E)` | `O(V + E)` |
| **18** | Investigation | Multi-Property Dijkstra (Cost, Counts, Min/Max Hops) | `O((V + E) log V)` | `O(V)` |
| **19** | Planets Queries I | Binary Lifting / Successor Graphs (2^k Jumps) | `O((N + Q) log K)` | `O(N log K)` |
| **20** | Planets Queries II | Functional Graph Cycle Decomposition + Tree Lifting | `O((N + Q) log N)` | `O(N log N)` |
| **21** | Planets Cycles | Functional Graph Component Cycles & Depth Offsets | `O(N)` | `O(N)` |
| **22** | Road Reparation | Kruskal's / Prim's Minimum Spanning Tree | `O(E log E)` | `O(V + E)` |
| **23** | Road Construction | DSU Component Count & Max Component Tracker | `O(E * α(V))` | `O(V)` |
| **24** | Flight Routes Check | Kosaraju's Strongly Connected Components (SCC) | `O(V + E)` | `O(V + E)` |
| **25** | Planets and Kingdoms | Strongly Connected Components (Kosaraju / Tarjan) | `O(V + E)` | `O(V + E)` |
| **26** | Coin Collector | SCC Condensation DAG + DP Longest Path | `O(V + E)` | `O(V + E)` |
| **27** | Mail Delivery | Hierholzer's Algorithm for Eulerian Circuit | `O(V + E)` | `O(V + E)` |
| **28** | De Bruijn Sequence | Eulerian Path on 2^(N-1) De Bruijn Graph | `O(2^N)` | `O(2^N)` |
| **29** | Teleporters Path | Directed Eulerian Path (In/Out Degree Checks) | `O(V + E)` | `O(V + E)` |
| **30** | Hamiltonian Flights | Bitmask DP over Subsets of Visited Nodes | `O(2^N * N^2)` | `O(2^N * N)` |
| **31** | Download Speed | Edmonds-Karp / Dinic's Maximum Flow | `O(V^2 * E)` | `O(V + E)` |
| **32** | Police Chase | Dinic's Min-Cut Edge Decomposition | `O(V^2 * E)` | `O(V + E)` |

---

### 5. Range Queries

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Static Range Sum Queries | 1D Prefix Sums Array | `O(N)` build, `O(1)` query | `O(N)` |
| **2** | Static Range Minimum Queries | Sparse Table / RMQ | `O(N log N)` build, `O(1)` query | `O(N log N)` |
| **3** | Dynamic Range Sum Queries | Fenwick Tree / Binary Indexed Tree | `O(log N)` per op | `O(N)` |
| **4** | Dynamic Range Minimum Queries | Point Update Segment Tree | `O(log N)` per op | `O(N)` |
| **5** | Range Xor Queries | Prefix XOR Array | `O(N)` build, `O(1)` query | `O(N)` |
| **6** | Range Update Queries | Difference Array / Range Update Point Query Fenwick | `O(log N)` per op | `O(N)` |
| **7** | Forest Queries | 2D Prefix Sum Matrix | `O(N^2)` build, `O(1)` query | `O(N^2)` |
| **8** | Hotel Queries | Segment Tree Binary Search (First >= X) | `O(log N)` per op | `O(N)` |
| **9** | List Removals | Segment Tree / PBDS Rank Selection | `O(log N)` per op | `O(N)` |
| **10** | Salary Queries | PBDS order_of_key / Coordinate Compression Fenwick | `O(log N)` per op | `O(N)` |
| **11** | Prefix Sum Queries | Segment Tree storing (sum, max_prefix) | `O(log N)` per op | `O(N)` |
| **12** | Subarray Sum Queries | Segment Tree Maximum Subarray Sum Node | `O(log N)` per op | `O(N)` |
| **13** | Subarray Sum Queries II | Dynamic Segment Tree Subarray Sum Tracker | `O(log N)` per op | `O(N)` |
| **14** | Distinct Values Queries | Mo's Algorithm / Offline Fenwick Tree Next Pointer | `O((N + Q) log N)` | `O(N + Q)` |

---

### 6. Tree Algorithms

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Subordinates | Subtree Size DFS | `O(N)` | `O(N)` |
| **2** | Tree Matching | Greedy / Tree DP Maximum Independent Set | `O(N)` | `O(N)` |
| **3** | Tree Diameter | 2-Pass BFS / DFS Tree Traversal | `O(N)` | `O(N)` |
| **4** | Tree Distances I | 2-Pass Tree Rerooting / Diameter Extremities | `O(N)` | `O(N)` |
| **5** | Tree Distances II | 2-Pass Tree Rerooting DP (Sum of Distances) | `O(N)` | `O(N)` |
| **6** | Company Queries I | Binary Lifting Ancestor Doubling up[u][k] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **7** | Company Queries II | Lowest Common Ancestor (LCA) Binary Lifting | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **8** | Distance Queries | Tree Distance via LCA: depth[u]+depth[v]-2*depth[lca] | `O(N log N)` build, `O(log N)` query | `O(N log N)` |
| **9** | Counting Paths | Tree Prefix Difference Array on LCA Nodes | `O(N log N + Q)` | `O(N log N)` |
| **10** | Subtree Queries | Euler Tour Flattening + Fenwick Tree | `O(log N)` per op | `O(N)` |
| **11** | Path Queries | Euler Tour In/Out Range Updates on Fenwick Tree | `O(log N)` per op | `O(N)` |
| **12** | Distinct Colors | DSU on Tree (Sack) / Offline Subtree Queries | `O(N log N)` | `O(N)` |
| **13** | Finding a Centroid | Subtree Size DFS + Heavy Child Descent | `O(N)` | `O(N)` |

---

### 7. Mathematics

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Exponentiation | Binary Exponentiation (A^B mod M) | `O(log B)` | `O(1)` |
| **2** | Exponentiation II | Fermat's Little Theorem (A^(B^C mod (M-1)) mod M) | `O(log B + log C)` | `O(1)` |
| **3** | Counting Divisors | Sieve-Based Prime Factorization | `O(N log log N)` build, `O(log X)` query | `O(N)` |
| **4** | Common Divisors | Multiples Frequency Bucket Counting | `O(N + max_A log max_A)` | `O(max_A)` |
| **5** | Sum of Divisors | Hyperbola / Square-Root Interval Harmonic Sum | `O(sqrt(N))` | `O(1)` |
| **6** | Divisor Analysis | Prime Factorization Multiplicative Number Theory | `O(N log MOD)` | `O(N)` |
| **7** | Prime Multiples | Inclusion-Exclusion Principle over Bitmasks | `O(K * 2^K)` | `O(K)` |
| **8** | Counting Coprime Pairs | Mobius Inversion / Multiples Inclusion-Exclusion | `O(N sqrt(max_A))` | `O(max_A)` |
| **9** | Binomial Coefficients | Factorial & Inverse Factorials Precomputed Table | `O(N)` build, `O(1)` query | `O(N)` |
| **10** | Creating Strings II | Multinomial Coefficient (N! / product(c_i!)) | `O(N)` | `O(N)` |
| **11** | Distributing Apples | Stars and Bars Combinatorics (N+M-1 choose N-1) | `O(N + M)` | `O(N + M)` |
| **12** | Christmas Party | Derangements Formula D_n = (n-1)*(D_{n-1} + D_{n-2}) | `O(N)` | `O(N)` |
| **13** | Bracket Sequences I | Catalan Number Formula C_{N/2} mod M | `O(N)` | `O(N)` |
| **14** | Fibonacci Numbers | 2x2 Matrix Exponentiation | `O(log N)` | `O(1)` |
| **15** | Throwing Dice | 6x6 Matrix Exponentiation | `O(6^3 log N)` | `O(1)` |
| **16** | Graph Paths I | Adjacency Matrix Exponentiation | `O(V^3 log K)` | `O(V^2)` |
| **17** | Next Prime | Miller-Rabin / Deterministic Prime Check | `O(sqrt(P))` | `O(1)` |
| **18** | Permutation Order | Factorial Positional Weighting (Lehmer Code) | `O(N^2)` | `O(N)` |
| **19** | Permutation Rounds | Disjoint Cycle Decomposition + LCM | `O(N + sum(sqrt(L)))` | `O(N)` |
| **20** | Sum of Four Squares | Lagrange's Four-Square Theorem / Sieve Decomposition | `O(sqrt(N))` | `O(1)` |
| **21** | Triangle Number Sums | Math Inversion on Triangular Numbers | `O(sqrt(N))` | `O(1)` |

---

### 8. String Algorithms

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Word Combinations | Trie / Aho-Corasick + 1D DP | `O(N * L + sum(len(w)))` | `O(sum(len(w)))` |
| **2** | String Matching | KMP Algorithm / Knuth-Morris-Pratt Prefix Function | `O(N + M)` | `O(M)` |
| **3** | Finding Borders | KMP pi-Table Suffix Recovery | `O(N)` | `O(N)` |
| **4** | Finding Periods | KMP pi-Table Period Boundary Check | `O(N)` | `O(N)` |
| **5** | Minimal Rotation | Booth's Algorithm for Lexicographical Minimum | `O(N)` | `O(N)` |
| **6** | Longest Palindrome | Manacher's Algorithm for Palindromic Radii | `O(N)` | `O(N)` |
| **7** | All Palindromes | Manacher's Radii Expansion Table | `O(N)` | `O(N)` |
| **8** | Palindrome Queries | Segment Tree on Dual Polynomial Rolling Hashes | `O(log N)` per op | `O(N)` |
| **9** | Distinct Subsequences | 1D Space-Optimized DP (Backwards Transition) | `O(len(S) * len(T))` | `O(len(T))` |
| **10** | Repeating Substring | Suffix Automaton / Rolling Hash Binary Search | `O(N log N)` | `O(N)` |
| **11** | String Functions | Z-Algorithm + KMP pi-Array Computation | `O(N)` | `O(N)` |

---

### 9. Geometry

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Point Location Test | 2D Cross Product (Orientation Determinant) | `O(1)` | `O(1)` |

---

### 10. Advanced Graph Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Graph Girth | Multi-Source BFS on Unweighted Graph | `O(V * (V + E))` | `O(V)` |
| **2** | Strongly Connected Edges | Bridge-Finding DFS + Back-Edge Orientation | `O(V + E)` | `O(V + E)` |
| **3** | Network Renovation | Bridge-Block Tree / Leaf Pairing DFS | `O(V + E)` | `O(V + E)` |
| **4** | Network Breakdown | Offline Reverse-Time DSU Component Merges | `O(E + Q * α(V))` | `O(V + E + Q)` |
| **5** | Visiting Cities | Articulation Points on DAG / Dominator Paths | `O(V + E)` | `O(V + E)` |
| **6** | MST Edge Check | Kruskal's DSU + Binary Lifting Max Edge on Tree | `O((E + Q) log V)` | `O(V log V + E)` |
| **7** | MST Edge Cost | Tree Heavy-Light Decomposition / Binary Lifting | `O(E log V)` | `O(V log V)` |
| **8** | Nearest Shops | Multi-Source BFS | `O(V + E)` | `O(V)` |
| **9** | Transfer Speeds Sum | Tree Centroid / DSU Subtree Product Aggregation | `O(N log N)` | `O(N)` |
| **10** | Tree Isomorphism I | AHU Tree Canonical Subtree Hashing (Rooted) | `O(N log N)` | `O(N)` |
| **11** | Tree Isomorphism II | Tree Centers + AHU Double Hashing (Unrooted) | `O(N log N)` | `O(N)` |
| **12** | Tree Traversals | Preorder + Inorder to Postorder Linear Construction | `O(N)` | `O(N)` |
| **13** | Tree Coin Collecting I | Tree DP Subtree Aggregations | `O(N)` | `O(N)` |
| **14** | Course Schedule II | Kahn's Algorithm Ordering Recovery | `O(V + E)` | `O(V + E)` |
| **15** | Acyclic Graph Edges | Topological Ordering Orientation | `O(V + E)` | `O(V)` |
| **16** | Flight Route Requests | Weakly Connected Components + Cycle Detection | `O(V + E)` | `O(V)` |
| **17** | Graph Coloring | Backtracking / Chromatic Number DP | `O(N * 2^N)` | `O(2^N)` |
| **18** | Bus Companies | 2-SAT / SCC Implication Graph | `O(V + E)` | `O(V + E)` |

---

### 11. Sliding Window Problems

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Sliding Window Sum | Running Queue Add and Subtract | `O(N)` | `O(1)` |
| **2** | Sliding Window Minimum | Monotonic Increasing Deque | `O(N)` | `O(K)` |
| **3** | Sliding Window Median | Two Heaps (Max-Heap + Min-Heap) with Lazy Deletion | `O(N log K)` | `O(K)` |
| **4** | Sliding Window Cost | Two Heaps with Running Left/Right Sums | `O(N log K)` | `O(K)` |
| **5** | Sliding Window Distinct Values | Sliding Window Hash Map / Frequency Counter | `O(N)` | `O(K)` |
| **6** | Sliding Window Mex | Frequency Array + Set of Missing Numbers | `O(N log K)` | `O(K)` |
| **7** | Sliding Window Mode | Frequency Map + Count-to-Values Multi-Set | `O(N log K)` | `O(K)` |
| **8** | Sliding Window Inversions | Sliding Window + Fenwick Tree | `O(N log K)` | `O(K)` |
| **9** | Sliding Window Or | Double-Stack / Bit-Contribution Sliding Window | `O(N)` | `O(K)` |
| **10** | Sliding Window Xor | Running Prefix XOR Cancellation | `O(N)` | `O(1)` |

---

### 12. Bitwise Operations

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Counting Bits | Bit Position Arithmetic Math | `O(log N)` | `O(1)` |
| **2** | Maximum Xor Subarray | Binary Trie (Bit-Level Max Path Greedy Search) | `O(N * 30)` | `O(N * 30)` |
| **3** | Maximum Xor Subset | Linear Basis / Gaussian Elimination over GF(2) | `O(N * 60)` | `O(60)` |
| **4** | Number of Subset Xors | Linear Basis Span (2^rank) | `O(N * 60)` | `O(60)` |
| **5** | SOS Bit Problem | Sum Over Subsets (SOS) Dynamic Programming | `O(N * 2^N)` | `O(2^N)` |

---

### 13. Advanced Techniques

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Meet in the Middle | Split Search Space 2^(N/2) + Sorting Binary Search | `O(2^(N/2) * log(2^(N/2)))` | `O(2^(N/2))` |
| **2** | Hamming Distance | std::bitset Bitwise Population Count | `O(N^2 * (K/64))` | `O(N)` |
| **3** | Reachable Nodes | Bitset-Accelerated TopoSort Reachability | `O((V * (V + E))/64)` | `O(V * (V/64))` |
| **4** | Reachability Queries | Bitset DAG Reachability / Tarjan SCC + TopoSort | `O(V^2 / 64 + Q)` | `O(V^2 / 64)` |
| **5** | Eulerian Subgraphs | Spanning Forest Cycle Space Nullity (2^(E - V + C)) | `O(V + E)` | `O(V + E)` |
| **6** | Corner Subgrid Check | Bitset 2D Matrix Row AND Pair Intersection | `O(N^3 / 64)` | `O(N * (N/64))` |
| **7** | Corner Subgrid Count | Bitset Pairwise Common Bits Combination (c choose 2) | `O(N^3 / 64)` | `O(N * (N/64))` |
| **8** | Necessary Roads | Tarjan's Bridge-Finding Algorithm (DFS Low-Link) | `O(V + E)` | `O(V + E)` |
| **9** | Necessary Cities | Tarjan's Articulation Points Algorithm (DFS Low-Link) | `O(V + E)` | `O(V + E)` |
| **10** | New Roads Queries | Kruskal's Reconstruction Tree + Binary Lifting | `O((N + Q) log N)` | `O(N log N)` |
| **11** | Cut and Paste | Implicit Treap (Cartesian Tree with Random Priorities) | `O(N + Q log N)` | `O(N)` |
| **12** | Substring Reversals | Implicit Treap with Lazy Tag Propagation | `O(N + Q log N)` | `O(N)` |
| **13** | Reversals and Sums | Implicit Treap with Range Sum & Lazy Reversals | `O(N + Q log N)` | `O(N)` |

---

### 14. Additional Problems I

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Shortest Subsequence | Greedy Shortest String Not A Subsequence (Alphabet Sets) | `O(N)` | `O(Σ)` |
| **2** | Counting Bits | Periodic Bit Position Math | `O(log N)` | `O(1)` |
| **3** | Swap Game | 0-1 BFS on 9! Permutation Board Graph | `O(9!)` | `O(9!)` |
| **4** | Distinct Values Sum | Offline Fenwick Tree with Previous Occurrence Contribution | `O(N log N)` | `O(N)` |
| **5** | Writing Numbers | Binary Search on Digits Count + Digit DP Math | `O(log_10^2 N)` | `O(1)` |
| **6** | Subsets with Fixed Average | 0/1 Knapsack Shifted Value DP | `O(N^2 * max_A)` | `O(N * max_A)` |
| **7** | Advertisement | Monotonic Stack Max Rectangle under Histogram | `O(N)` | `O(N)` |
| **8** | Special Substrings | Prefix Balance Hash Map (count(c) - count('a')) | `O(N log N)` | `O(N)` |
| **9** | Permutation Subsequence | Patience Sorting / LIS Index Mapping | `O(N log N)` | `O(N)` |
| **10** | Maximum Building I | Largest Rectangle in Histogram over 2D Matrix Rows | `O(N * M)` | `O(M)` |
| **11** | Two Array Average | Fractional Knapsack / Binary Search on Average Slope | `O(N log N)` | `O(N)` |
| **12** | Sorting Methods | LIS, Inversions, Cycle Shifts, and Greedy Reversals | `O(N log N)` | `O(N)` |
| **13** | Cyclic Array | Doubling Binary Lifting over Greedy Partition Jumps | `O(N log N)` | `O(N log N)` |
| **14** | Subarray Sum Constraints | Difference Constraints System / Shortest Path | `O(V * E)` | `O(V)` |
| **15** | Multiplication Table | Binary Search on Value Range (<= mid count) | `O(N log(N^2))` | `O(1)` |
| **16** | Bit Inversions | Set Gap Intervals + Multiset Segment Lengths | `O(M log N)` | `O(N)` |

---

### 15. Additional Problems II

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Stick Divisions | Huffman Coding / Greedy Min-Heap Cost Merge | `O(K log K)` | `O(K)` |
| **2** | School Excursion | Subset Sum DP with std::bitset Acceleration | `O((N * sum(S_i))/64)` | `O(sum(S_i)/64)` |
| **3** | Coin Grid | Konig's Theorem (Bipartite Min Vertex Cover via Max Flow) | `O(V^2 * E)` | `O(V + E)` |
| **4** | Book Shop II | Bounded Knapsack with Binary Power Decomposition | `O(X * sum(log K_i))` | `O(X)` |
| **5** | Increasing Array II | Slope Trick with Max-Heap of Convex Inflection Points | `O(N log N)` | `O(N)` |
| **6** | GCD Subsets | Mobius Inversion / Fast Zeta Transform over GCD Multiples | `O(max_A log max_A)` | `O(max_A)` |
