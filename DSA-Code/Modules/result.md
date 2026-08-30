# DSA Modules — Problem Summary & Complexity Reference

A comprehensive, unified reference catalog of **436 essential Data Structures & Algorithms problems and algorithms** across all 14 topic modules in `DSA-Code/Modules/`. Every entry includes standard problem names, algorithmic patterns, optimal time complexities, and auxiliary space complexities.

## 📘 Module-by-Module Analysis

### 1. Alphabet Trie (`alphabet_trie.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Implement Trie I (Prefix Tree) (LC 208) | 26-Way Character Array Trie | `O(L)` | `O(N * L)` |
| **2** | Implement Trie II (Counts & Erase) (LC 1804) | Prefix & End Occurrence Counting | `O(L)` | `O(N * L)` |
| **3** | Design Add & Search Words (LC 211) | Wildcard '.' Branching Backtrack | `O(26^D)` | `O(N * L)` |
| **4** | Word Search II (Boggle Grid) (LC 212) | 2D DFS + Trie In-Place Node Pruning | `O(R * C * 4^L)` | `O(W * L)` |
| **5** | Replace Words / Shortest Root (LC 648) | Earliest Matching Root Prefix DFS | `O(N * L)` | `O(N * L)` |
| **6** | Longest Word With All Prefixes (LC 720) | Complete Prefix Chain Validation | `O(N * L)` | `O(N * L)` |
| **7** | Count Distinct Substrings (LC 1698) | Suffix Insertion Substring Count | `O(N^2)` | `O(N^2)` |
| **8** | Search Suggestions System (LC 1268) | Trie Node Sorted Top-3 List Cache | `O(N * L log 3)` | `O(N * L)` |
| **9** | Stream of Characters (LC 1032) | Reversed Word Trie Suffix Match | `O(L)` | `O(W * L)` |
| **10** | Map Sum Pairs (LC 677) | Trie Delta Propagation / DFS Sum | `O(L)` | `O(N * L)` |
| **11** | Concatenated Words (LC 472) | Trie + Memoized DFS Word Splitting | `O(N * L^2)` | `O(N * L)` |
| **12** | Word Break via Trie (LC 139) | 1D DP + Trie Prefix Fast Slicing | `O(N^2)` | `O(W * L)` |
| **13** | Palindrome Pairs (LC 336) | Reverse Trie + Palindrome Suffixes | `O(N * L^2)` | `O(N * L)` |
| **14** | Word Squares (LC 425) | Backtracking with Trie Prefix Lookup | `O(N * 26^L)` | `O(N * L)` |
| **15** | Extra Characters in a String (LC 2707) | Trie + 1D Min-Deletion DP | `O(N^2)` | `O(W * L)` |

---

### 2. Binary Trie (`binary_trie.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Maximum XOR of Two Numbers (LC 421) | 0-1 Bitwise Binary Trie | `O(31 * N)` | `O(31 * N)` |
| **2** | Max XOR With Element Query (LC 1707) | Offline Sorted Queries + Dynamic Trie | `O(Q log Q)` | `O(31 * N)` |
| **3** | Maximum Subarray XOR | Prefix XOR Trie Max Query | `O(31 * N)` | `O(31 * N)` |
| **4** | Count Pairs With XOR in Range (LC 1803) | Range Difference: [0..R] - [0..L-1] | `O(31 * N)` | `O(31 * N)` |
| **5** | Count Subarrays With XOR < K | Prefix XOR + countLessThanK Trie | `O(31 * N)` | `O(31 * N)` |
| **6** | Max Genetic Difference Query (LC 1938) | Tree DFS + Backtracking Trie | `O(31 * (N + Q))` | `O(31 * N)` |
| **7** | Minimum XOR Value Pair | Min Bit Branching Invariant | `O(31 * N)` | `O(31 * N)` |
| **8** | Max XOR Two Non-Overlapping Subarrays | Left Prefix & Right Suffix Tries | `O(31 * N)` | `O(31 * N)` |
| **9** | K-th Smallest Element in Multiset | Subtree Count Bit-Rank Traversal | `O(31)` | `O(31 * N)` |
| **10** | Max XOR Path in Tree | Tree Root-Path Prefix XOR Trie | `O(31 * N)` | `O(31 * N)` |
| **11** | Subarray XOR Equal to K (LC 560 / GFG) | Prefix XOR Frequency Invariant | `O(N)` | `O(N)` |
| **12** | Minimize XOR (LC 2429) | Bit Match & Least Significant Set | `O(31)` | `O(1)` |
| **13** | Maximum XOR After Operations (LC 2317) | Bitwise OR Property Exploitation | `O(N)` | `O(1)` |
| **14** | Minimum XOR Sum of Two Arrays (LC 1879) | Bitmask DP / Min-Cost Match | `O(N * 2^N)` | `O(2^N)` |
| **15** | Persistent 0-1 Trie Range Query | Functional Persistent Binary Trie | `O(31 log N)` | `O(31 * N)` |

---

### 3. Dynamic Programming (`dp.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Climbing Stairs (LC 70) | Fibonacci 1D Space DP | `O(N)` | `O(1)` |
| **2** | Min Cost Climbing Stairs (LC 746) | Rolling 2-State Minimum DP | `O(N)` | `O(1)` |
| **3** | House Robber (LC 198) | Rob / Skip Alternating DP | `O(N)` | `O(1)` |
| **4** | House Robber II - Circular (LC 213) | Dual Pass: [0..N-2] & [1..N-1] | `O(N)` | `O(1)` |
| **5** | Decode Ways (LC 91) | 1-Digit / 2-Digit Branching DP | `O(N)` | `O(1)` |
| **6** | Delete and Earn (LC 740) | Frequency Transform + House Robber | `O(N + max_val)` | `O(max_val)` |
| **7** | Partition Equal Subset Sum (LC 416) | 1D 0/1 Knapsack (Backwards Loop) | `O(N * sum)` | `O(sum)` |
| **8** | Target Sum (LC 494) | Subset Sum Partition Reduction | `O(N * sum)` | `O(sum)` |
| **9** | Coin Change (LC 322) | 1D Unbounded Knapsack (Min Coins) | `O(N * amount)` | `O(amount)` |
| **10** | Coin Change II (LC 518) | 1D Unbounded (Combinations Outer) | `O(N * amount)` | `O(amount)` |
| **11** | Combination Sum IV (LC 377) | 1D Unbounded (Permutations Outer) | `O(N * amount)` | `O(amount)` |
| **12** | Longest Common Subsequence (LC 1143) | 2D DP Table / Rolling Row DP | `O(N * M)` | `O(min(N, M))` |
| **13** | Edit Distance (LC 72) | 2D Levenshtein Distance Matrix | `O(N * M)` | `O(N * M)` |
| **14** | Distinct Subsequences (LC 115) | 1D Space-Optimized DP (Backwards) | `O(N * M)` | `O(M)` |
| **15** | Word Break (LC 139) | 1D Prefix DP + Trie/Set Lookup | `O(N^2 * L)` | `O(N + D)` |
| **16** | Interleaving String (LC 97) | 2D Grid DP Match Propagation | `O(N * M)` | `O(M)` |
| **17** | Longest Palindromic Subsequence (LC 516) | 2D Interval DP / LCS with Reverse | `O(N^2)` | `O(N)` |
| **18** | Longest Increasing Subsequence (LC 300) | Patience Sorting (lower_bound) | `O(N log N)` | `O(N)` |
| **19** | Longest String Chain (LC 1048) | Length-Sorted DP + Predecessor Map | `O(N * L^2)` | `O(N)` |
| **20** | Largest Divisible Subset (LC 368) | Sorted 1D LIS Variant + Parent DP | `O(N^2)` | `O(N)` |
| **21** | Job Scheduling Max Profit (LC 1235) | End-Time Sort + DP + Binary Search | `O(N log N)` | `O(N)` |
| **22** | Unique Paths (LC 62) | Combinatorics / 1D Grid DP Roll | `O(M * N)` | `O(N)` |
| **23** | Unique Paths II - Obstacles (LC 63) | 1D Space-Optimized Grid DP | `O(M * N)` | `O(N)` |
| **24** | Minimum Path Sum (LC 64) | 1D Grid DP In-Place Accumulation | `O(M * N)` | `O(N)` |
| **25** | Maximal Square (LC 221) | 2D DP: min(up, left, diag) + 1 | `O(M * N)` | `O(N)` |
| **26** | Maximal Rectangle in Binary Grid (LC 85) | Row Heights Histogram + MonoStack | `O(M * N)` | `O(N)` |
| **27** | Burst Balloons (LC 312) | Interval DP (Last Balloon Picked) | `O(N^3)` | `O(N^2)` |
| **28** | Minimum Cost to Cut a Stick (LC 1547) | Interval DP on Sorted Cut Points | `O(C^3)` | `O(C^2)` |
| **29** | Palindrome Partitioning II (LC 132) | Precomputed Palindrome + 1D Min Cut | `O(N^2)` | `O(N^2)` |
| **30** | Binary Tree Maximum Path Sum (LC 124) | Post-Order DFS Gain Calculation | `O(N)` | `O(H)` |
| **31** | House Robber III - Tree (LC 337) | Post-Order DFS pair{rob, notRob} | `O(N)` | `O(H)` |
| **32** | Best Time to Buy Stock Cooldown (LC 309) | 3-State FSM (hold, sold, rest) | `O(N)` | `O(1)` |
| **33** | Stock with Transaction Fee (LC 714) | 2-State FSM (hold, cash) | `O(N)` | `O(1)` |
| **34** | Stock IV - At Most K Trans (LC 188) | 2K-State FSM / Min-Cost DP | `O(N * K)` | `O(K)` |
| **35** | Count Digit One (LC 233) | Positional Power-of-10 Math | `O(log_10 N)` | `O(1)` |
| **36** | Smallest Sufficient Team (LC 1125) | Bitmask DP (Subset Skill Cover) | `O(M * 2^N)` | `O(2^N)` |
| **37** | Removal Game / Stone Game I (CSES / LC 877) | 2D Interval Minimax DP | `O(N^2)` | `O(N)` |
| **38** | Predict the Winner (LC 486) | 1D Interval Minimax Net Score DP | `O(N^2)` | `O(N)` |
| **39** | Can I Win (LC 464) | Bitmask Memoization Minimax | `O(2^N)` | `O(2^N)` |
| **40** | Guess Number Higher or Lower II (LC 375) | Interval Minimax Cost DP | `O(N^3)` | `O(N^2)` |
| **41** | Stone Game II (LC 1140) | Memoized Suffix Minimax (i, M) | `O(N^3)` | `O(N^2)` |
| **42** | Stone Game III (LC 1406) | 1D Suffix Minimax 3-Step Choice | `O(N)` | `O(1)` |
| **43** | Divisor Game & Nim Game (LC 1025 / 292) | Mathematical Parity Invariant | `O(1)` | `O(1)` |
| **44** | Super Egg Drop (LC 887) | Inverse DP: moves & eggs | `O(K log N)` | `O(K)` |
| **45** | Decode Ways II with Wildcards (LC 639) | 1D DP Multi-Case Transition Mod | `O(N)` | `O(1)` |
| **46** | Minimum Cost For Tickets (LC 983) | 1D Travel Day Reachability DP | `O(365)` | `O(365)` |
| **47** | Dungeon Game (LC 174) | Bottom-Up Reverse Knight Health DP | `O(M * N)` | `O(M * N)` |
| **48** | Cherry Pickup (LC 741) | Synchronous 2-Agent Manhattan DP | `O(N^3)` | `O(N^2)` |
| **49** | Maximum Product Subarray (LC 152) | 2-State Max/Min Running DP | `O(N)` | `O(1)` |
| **50** | Ninja's Training (GeeksforGeeks) | 2D Activity Transitions O(1) Space | `O(N)` | `O(1)` |

---

### 4. Graph Algorithms (`graph.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | DFS Traversal | Recursive Adjacency Traversal | `O(V + E)` | `O(V)` |
| **2** | BFS Traversal | Level-by-Level Queue Traversal | `O(V + E)` | `O(V)` |
| **3** | Number of Connected Components | Unvisited Component DFS / BFS | `O(V + E)` | `O(V)` |
| **4** | Cycle Detection (Undirected Graph) | Parent-Tracking DFS / BFS | `O(V + E)` | `O(V)` |
| **5** | Cycle Detection (Directed Graph) | 3-Color / Recursion Path Set DFS | `O(V + E)` | `O(V)` |
| **6** | Bipartite Graph Check (BFS / DFS) | 2-Color Alternate Parity Check | `O(V + E)` | `O(V)` |
| **7** | Topological Sort (Kahn's Algorithm) | In-Degree 0 Queue Reduction (BFS) | `O(V + E)` | `O(V)` |
| **8** | Topological Sort (DFS) | Post-Order DFS Reverse Order | `O(V + E)` | `O(V)` |
| **9** | Shortest Path in Unweighted Graph | Unit-Edge BFS Distance Array | `O(V + E)` | `O(V)` |
| **10** | Dijkstra's Shortest Path (Positive Weights) | Min-Heap Priority Queue (Greedy) | `O((V + E) log V)` | `O(V)` |
| **11** | 0-1 BFS | Double-Ended Queue (push_front/back) | `O(V + E)` | `O(V)` |
| **12** | Bellman-Ford Algorithm | (V - 1) Edge Relaxation + Cycle Det | `O(V * E)` | `O(V)` |
| **13** | Floyd-Warshall All-Pairs Shortest Path | 3D Matrix DP Relaxation (k, i, j) | `O(V^3)` | `O(V^2)` |
| **14** | Disjoint Set Union (DSU / Union-Find) | Path Compression & Union by Rank | `O(α(N))` | `O(N)` |
| **15** | Cycle Detection using DSU | Dynamic Edge Component Find Check | `O(E α(V))` | `O(V)` |
| **16** | Kruskal's Minimum Spanning Tree (MST) | Edge Weight Sort + DSU Cycle Avoid | `O(E log E)` | `O(V)` |
| **17** | Prim's Minimum Spanning Tree (MST) | Cut-Property Min-Heap Expansion | `O((V + E) log V)` | `O(V)` |
| **18** | Number of Islands (LC 200) | 2D Grid DFS / BFS Flood Fill | `O(R * C)` | `O(R * C)` |
| **19** | Shortest Path in Binary Matrix (LC 1091) | 8-Directional Grid BFS Queue | `O(R * C)` | `O(R * C)` |
| **20** | Word Ladder (LC 127) | BFS Shortest Transformation Path | `O(N * L * 26)` | `O(N * L)` |
| **21** | Multi-Source BFS (Rotting Oranges / 0-1 Mat) | Multi-Queue Seeded BFS Expansion | `O(R * C)` | `O(R * C)` |
| **22** | K Shortest Paths | Min-Heap Dijkstra with K-Visits | `O(K * E log V)` | `O(K * V)` |
| **23** | Eventual Safe States (LC 802) | Reverse Graph Kahn's Topo Sort | `O(V + E)` | `O(V)` |
| **24** | Shortest Path in DAG via Topo Sort | Linear Topo Order Edge Relaxation | `O(V + E)` | `O(V)` |
| **25** | Path with Minimum Effort (LC 1631) | Minimax Height Delta Dijkstra | `O(R * C log(R * C))` | `O(R * C)` |
| **26** | Network Delay Time (LC 743) | Single-Source Dijkstra Min-Heap | `O(E log V)` | `O(V)` |
| **27** | Number of Ways to Arrive at Destination | Dijkstra + Combinatorial Path DP | `O(E log V)` | `O(V)` |
| **28** | Number of Operations to Connect Network | DSU Redundant Edge Surplus Count | `O(V + E)` | `O(V)` |
| **29** | Most Stones Removed with Same Row/Col | Row & Col Coordinate Component DSU | `O(N α(N))` | `O(N)` |
| **30** | Number of Islands II (Dynamic Land) | Online 2D Grid DSU Union Merging | `O(K α(N))` | `O(R * C)` |
| **31** | Critical Connections / Bridges (LC 1192) | Tarjan's Low-Link DFS (low[v] > tin[u]) | `O(V + E)` | `O(V)` |
| **32** | Articulation Points (Cut Vertices) | Tarjan's Low-Link (low[v] >= tin[u]) | `O(V + E)` | `O(V)` |
| **33** | Strongly Connected Components (Kosaraju) | 2-Pass DFS + Transpose Graph Stack | `O(V + E)` | `O(V)` |
| **34** | Cheapest Flights Within K Stops (LC 787) | Bellman-Ford / BFS with Stop Budget | `O(K * E)` | `O(V)` |
| **35** | Alien Dictionary (LC 269) | Lexicographical DAG Topo Sort | `O(N * L + A)` | `O(A)` |
| **36** | Course Schedule I & II (LC 207 & 210) | Kahn's Algorithm / Cycle Topo Sort | `O(V + E)` | `O(V)` |
| **37** | Accounts Merge (LC 721) | Email-to-Owner DSU Component Group | `O(N α(N))` | `O(N)` |
| **38** | M-Coloring Problem (GFG) | Backtracking Vertex Color Assign | `O(M^V)` | `O(V)` |
| **39** | Flood Fill (LC 733) | Connected Component Color Replace | `O(R * C)` | `O(R * C)` |
| **40** | Pacific Atlantic Water Flow (LC 417) | Reverse Ocean Boundary DFS / BFS | `O(R * C)` | `O(R * C)` |
| **41** | Eulerian Path / Circuit (Hierholzer) | Hierholzer's In-Out Degree Stack | `O(V + E)` | `O(V + E)` |
| **42** | Tarjan's Strongly Connected Components | Single-Pass Low-Link & Stack SCC | `O(V + E)` | `O(V)` |
| **43** | Distinct Numbers in Sliding Window | Sliding Window Hash Map Frequencies | `O(N)` | `O(K)` |
| **44** | Graph Valid Tree (LC 261) | DSU Edge Count & Cycle Verification | `O(N α(N))` | `O(N)` |
| **45** | Clone Graph (LC 133) | BFS / DFS + Node Clone Hash Map | `O(V + E)` | `O(V)` |

---

### 5. Greedy Algorithms (`greedy.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Non-overlapping Intervals (LC 435) | Earliest End-Time Sorting | `O(N log N)` | `O(1)` |
| **2** | Minimum Arrows to Burst Balloons (LC 452) | End-Coordinate Interval Shooting | `O(N log N)` | `O(1)` |
| **3** | Merge Intervals (LC 56) | Start-Time Sort + Linear Merge | `O(N log N)` | `O(N)` |
| **4** | Insert Interval (LC 57) | 3-Phase Linear Partition & Merge | `O(N)` | `O(N)` |
| **5** | Meeting Rooms II (LC 253) | Two-Pointer Chronological Scan | `O(N log N)` | `O(N)` |
| **6** | N Meetings in One Room (GFG) | Greedy Earliest Finish Time | `O(N log N)` | `O(N)` |
| **7** | Minimum Platforms for Railway (GFG) | Event Chronological Sort (2 Pointers) | `O(N log N)` | `O(1)` |
| **8** | Jump Game I (LC 55) | Max Reachable Index Tracker | `O(N)` | `O(1)` |
| **9** | Jump Game II (LC 45) | BFS Range Expansion [curEnd, fwd] | `O(N)` | `O(1)` |
| **10** | Video Stitching (LC 1024) | Furthest Reachable Extension Jump | `O(N log N)` | `O(1)` |
| **11** | Min Taps to Water Garden (LC 1326) | Interval Max Reach Jump Game II | `O(N)` | `O(N)` |
| **12** | Candy (LC 135) | 2-Pass Greedy Rating Neighbor Sync | `O(N)` | `O(N)` |
| **13** | Gas Station / Circular Tour (LC 134) | Net Surplus Sum + Tank Reset | `O(N)` | `O(1)` |
| **14** | Valid Parenthesis String '*' (LC 678) | Min-Max Open Bracket Range Track | `O(N)` | `O(1)` |
| **15** | Min Deletions for Balanced String (LC 1653) | Running 'b' Count vs Deletion DP | `O(N)` | `O(1)` |
| **16** | Break a Palindrome (LC 1328) | First Non-'a' Prefix Mutation | `O(N)` | `O(1)` |
| **17** | Partition Labels (LC 763) | Last-Seen Index Boundary Expansion | `O(N)` | `O(1)` |
| **18** | Queue Reconstruction by Height (LC 406) | Descending Height + K-Index Insert | `O(N^2)` | `O(N)` |
| **19** | Assign Cookies (LC 455) | Greedy Two-Pointer Size Matching | `O(N log N)` | `O(1)` |
| **20** | Boats to Save People (LC 881) | Inward Converging Two Pointers | `O(N log N)` | `O(1)` |
| **21** | Two City Scheduling (LC 1029) | Cost Delta (costA - costB) Sorting | `O(N log N)` | `O(1)` |
| **22** | Bag of Tokens (LC 948) | Two Pointers: Buy Low, Sell High | `O(N log N)` | `O(1)` |
| **23** | Task Scheduler (LC 621) | Max Frequency Frame Geometry | `O(N)` | `O(1)` |
| **24** | Reorganize String (LC 767) | Max-Heap Frequency Interleaving | `O(N log A)` | `O(A)` |
| **25** | Largest Number (LC 179) | Custom String Concatenation Sort | `O(N log N)` | `O(N)` |
| **26** | Min Deletions for Unique Freqs (LC 1647) | Hash Set Seen Frequency Decrement | `O(N)` | `O(1)` |
| **27** | Min Swaps to Make Strings Equal (LC 1247) | XY and YX Mismatch Pair Counting | `O(N)` | `O(1)` |
| **28** | Shortest Unsorted Subarray (LC 581) | Running Prefix Max & Suffix Min | `O(N)` | `O(1)` |
| **29** | Fractional Knapsack (GFG) | Value-to-Weight Ratio Sorting | `O(N log N)` | `O(1)` |
| **30** | Job Sequencing with Deadlines (GFG) | Profit Sorting + Slot Allocation | `O(N^2)` | `O(max_deadline)` |
| **31** | Lemonade Change (LC 860) | Greedy Bill Register Tracking | `O(N)` | `O(1)` |
| **32** | Maximum Units on a Truck (LC 1710) | Units-Per-Box Descending Greedy | `O(N log N)` | `O(1)` |
| **33** | Wiggle Subsequence (LC 376) | Local Peak & Valley Count Tracker | `O(N)` | `O(1)` |
| **34** | Max Bags With Full Rocks (LC 2279) | Capacity Deficit Sorting | `O(N log N)` | `O(N)` |
| **35** | Course Schedule III (LC 630) | Deadline Sort + Max-Heap Regret | `O(N log N)` | `O(N)` |
| **36** | IPO - Maximize Capital (LC 502) | Min-Heap Capital + Max-Heap Profit | `O(N log N)` | `O(N)` |
| **37** | Connect Sticks / Huffman (LC 1167) | Min-Heap Pairwise Merge Reduction | `O(N log N)` | `O(N)` |
| **38** | Advantage Shuffle (LC 870) | Tian Ji Horse Racing Two Pointers | `O(N log N)` | `O(N)` |
| **39** | Min Increments to Form Target (LC 1526) | Positive Delta Level Accumulation | `O(N)` | `O(1)` |

---

### 6. Heaps & Priority Queues (`heap.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | K-th Largest & Smallest Elements | Min-Heap / Max-Heap of Size K | `O(N log K)` | `O(K)` |
| **2** | Top K Frequent Elements (LC 347) | Frequency Map + Min-Heap of Size K | `O(N log K)` | `O(N)` |
| **3** | K Closest Points to Origin (LC 973) | Max-Heap of Size K (Squared Dist) | `O(N log K)` | `O(K)` |
| **4** | Find Median from Data Stream (LC 295) | Two Heaps (Max-Left + Min-Right) | `O(log N)` | `O(N)` |
| **5** | IPO - Maximize Capital (LC 502) | Min-Heap (Cost) + Max-Heap (Profit) | `O(N log N)` | `O(N)` |
| **6** | Minimum Interval for Each Query (LC 1851) | Offline Sorted Queries + Min-Heap | `O(Q log Q)` | `O(N + Q)` |
| **7** | Merge K Sorted Arrays | Min-Heap K-Way Expansion | `O(N log K)` | `O(K)` |
| **8** | Find K Pairs with Smallest Sums (LC 373) | Min-Heap Frontier Expansion | `O(K log K)` | `O(K)` |
| **9** | N-th Ugly Number (LC 264) | 3-Pointer DP Multiples (2, 3, 5) | `O(N)` | `O(N)` |
| **10** | Connect Ropes / Sticks (LC 1168) | Huffman Greedy Min-Heap Reduction | `O(N log N)` | `O(N)` |
| **11** | Reorganize String (LC 767) | Max-Heap Frequency Interleaving | `O(N log A)` | `O(A)` |
| **12** | Course Schedule III (LC 630) | Greedy Deadline Sort + Max-Heap | `O(N log N)` | `O(N)` |
| **13** | Merge Overlapping Intervals (LC 56) | Start-Time Sort + Linear Merge | `O(N log N)` | `O(N)` |
| **14** | Insert Interval (LC 57) | 3-Phase Linear Partition & Merge | `O(N)` | `O(N)` |
| **15** | Minimum Meeting Rooms (LC 253) | Start-Time Sort + Min-Heap End | `O(N log N)` | `O(N)` |
| **16** | Top K Frequent Words (LC 692) | Min-Heap + Custom String Comp | `O(N log K)` | `O(N)` |
| **17** | Sliding Window Median (LC 480) | Two Balanced Multisets | `O(N log K)` | `O(K)` |
| **18** | Meeting Rooms I (LC 252) | Start-Time Sort + Overlap Check | `O(N log N)` | `O(1)` |
| **19** | Employee Free Time (LC 759) | Interval Flatten + Gap Scanning | `O(N log N)` | `O(N)` |
| **20** | Arrows to Burst Balloons (LC 452) | Greedy End-Coordinate Sorting | `O(N log N)` | `O(1)` |
| **21** | Maximum Sum Combinations (InterviewBit) | Max-Heap + Visited Pair Set | `O(K log K)` | `O(K)` |
| **22** | Kth Largest in Stream (LC 703) | Fixed Size-K Min-Heap Stream | `O(log K)` | `O(K)` |
| **23** | Kth Largest in Array (LC 215 - QuickSelect) | Lomuto Randomized QuickSelect | `O(N)` avg | `O(1)` |

---

### 7. Linked Lists (`linked_list.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Middle of the Linked List (LC 876) | Fast & Slow Pointers (Tortoise/Hare) | `O(N)` | `O(1)` |
| **2** | Detect Cycle in Linked List (LC 141) | Floyd's Cycle-Finding Algorithm | `O(N)` | `O(1)` |
| **3** | Remove N-th Node From End of List (LC 19) | Two Pointers with Dummy Sentinel | `O(N)` | `O(1)` |
| **4** | Intersection Point of Two Lists (LC 160) | Two Pointers Alternate Redirection | `O(L1 + L2)` | `O(1)` |
| **5** | Sort Linked List (Merge Sort on LL) (LC 148) | Top-Down Merge Sort + Midpoint Cut | `O(N log N)` | `O(log N)` |
| **6** | Segregate Odd & Even Nodes in LL (LC 328) | Two-Pointer Parity List Weaving | `O(N)` | `O(1)` |
| **7** | Reverse Linked List (LC 206) | Iterative 3-Pointer Reversal | `O(N)` | `O(1)` |
| **8** | Merge Two Sorted Linked Lists (LC 21) | Dummy Head + Linear Merge Step | `O(N + M)` | `O(1)` |
| **9** | Merge K Sorted Linked Lists (LC 23) | Min-Heap Priority Queue on Heads | `O(N log K)` | `O(K)` |
| **10** | Reorder List (LC 143) | Midpoint Split + Reverse + Weave | `O(N)` | `O(1)` |
| **11** | Add Two Numbers (LC 2) | Linear Digit Simulation + Carry | `O(max(N, M))` | `O(1)` |
| **12** | Delete Node in a Linked List O(1) (LC 237) | Value Copying + Next Node Bypass | `O(1)` | `O(1)` |
| **13** | Reverse Nodes in k-Group (LC 25) | Bounded K-Group Iterative Reversal | `O(N)` | `O(1)` |
| **14** | Palindrome Linked List (LC 234) | Midpoint Cut + Reverse Half + Match | `O(N)` | `O(1)` |
| **15** | Find Starting Point of Loop in LL (LC 142) | Floyd's Cycle + Head-Meeting Reset | `O(N)` | `O(1)` |
| **16** | Flattening a Linked List (GFG) | Bottom-Up Recursive Merge 2 Lists | `O(N * K)` | `O(1)` |
| **17** | Rotate List by K Places (LC 61) | Circular Ring Cut at (Len - K % Len) | `O(N)` | `O(1)` |
| **18** | Copy List with Random Pointer (LC 138) | 3-Pass In-Place Node Interweaving | `O(N)` | `O(1)` |

---

### 8. Mathematics & Bit Manipulation (`maths.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | GCD & LCM | Euclidean Modulo Algorithm | `O(log(min(A, B)))` | `O(1)` |
| **2** | Primality Check | Trial Division up to sqrt(N) | `O(sqrt(N))` | `O(1)` |
| **3** | Sieve of Eratosthenes | Multiples Cross-Off | `O(N log log N)` | `O(N)` |
| **4** | Prime Factorization | Trial Division with 2 & Odd Steps | `O(sqrt(N))` | `O(log N)` |
| **5** | Fast Power (Binary Exponentiation) | Bitwise Exponent Halving | `O(log B)` | `O(1)` |
| **6** | Modular Exponentiation | Modular Binary Exponentiation | `O(log B)` | `O(1)` |
| **7** | Modular Multiplicative Inverse | Fermat's Little Theorem (MOD - 2) | `O(log MOD)` | `O(1)` |
| **8** | Factorials & Combinatorics (nCr, nPr) | Precomputed Factorial Arrays | `O(N)` build, `O(1)` query | `O(N)` |
| **9** | Digit Operations (Sum, Reverse, Palindrome) | Base-10 Modulo & Division Loops | `O(log_10 N)` | `O(1)` |
| **10** | Check K-th Bit | Bitwise AND: (n & (1 << k)) != 0 | `O(1)` | `O(1)` |
| **11** | Set K-th Bit | Bitwise OR: n \| (1 << k) | `O(1)` | `O(1)` |
| **12** | Clear K-th Bit | Bitwise AND NOT: n & ~(1 << k) | `O(1)` | `O(1)` |
| **13** | Toggle K-th Bit | Bitwise XOR: n ^ (1 << k) | `O(1)` | `O(1)` |
| **14** | Check Power of 2 | Bitwise AND: n & (n - 1) == 0 | `O(1)` | `O(1)` |
| **15** | Count Set Bits (Kernighan's Algorithm) | Lowest-Bit Peeling Loop | `O(popcount)` | `O(1)` |
| **16** | Lowest Set Bit | Two's Complement Isolation: n & -n | `O(1)` | `O(1)` |
| **17** | Remove Lowest Set Bit | Rightmost Bit Clearing: n & (n - 1) | `O(1)` | `O(1)` |
| **18** | Single Number in Array | Cumulative Array XOR | `O(N)` | `O(1)` |
| **19** | Two Unique Numbers | XOR Sum + Lowest Set Bit Partition | `O(N)` | `O(1)` |
| **20** | Missing Number | Expected XOR vs Actual XOR Sum | `O(N)` | `O(1)` |
| **21** | Cumulative XOR from 1 to N | Periodicity of 4 in Binary XOR | `O(1)` | `O(1)` |
| **22** | Range XOR [L, R] | XOR(1..R) ^ XOR(1..L-1) | `O(1)` | `O(1)` |
| **23** | Power of 4 Check | Power of 2 + Odd Bitmask (0x55555555) | `O(1)` | `O(1)` |
| **24** | Check Even / Odd | Least Significant Bit: n & 1 | `O(1)` | `O(1)` |
| **25** | Swap Using XOR | In-Place 3-Step Bitwise XOR Swap | `O(1)` | `O(1)` |
| **26** | Reverse 32-Bit Integer | Bit-by-Bit Shift & Accumulate | `O(32)` | `O(1)` |
| **27** | Binary String Representation | Sequential Bit Shift to String | `O(32)` | `O(1)` |
| **28** | Gray Code Conversion | Binary-to-Gray: n ^ (n >> 1) | `O(1)` | `O(1)` |
| **29** | Generate All Subsets via Bitmask | Bitmask Loop [0...2^N - 1] | `O(N * 2^N)` | `O(N * 2^N)` |
| **30** | Iterate Through All Submasks | sub = (sub - 1) & mask Trick | `O(3^N)` | `O(1)` |
| **31** | Opposite Signs Check | Sign Bit XOR: (x ^ y) < 0 | `O(1)` | `O(1)` |
| **32** | Absolute Value via Bitwise Shifts | Arithmetic Shift Sign Masking | `O(1)` | `O(1)` |
| **33** | Next Power of 2 | Bitwise OR Smearing (1,2,4,8,16) | `O(1)` | `O(1)` |
| **34** | Built-in Bit Functions | __builtin_clz, ctz, popcount | `O(1)` | `O(1)` |
| **35** | Perfect Square Check | Integer Sqrt Rounding Check | `O(1)` | `O(1)` |
| **36** | Fast Fibonacci via Matrix Exponentiation | [[1, 1], [1, 0]] Binary Power | `O(log N)` | `O(1)` |
| **37** | Squared Euclidean Distance | (x1 - x2)^2 + (y1 - y2)^2 | `O(1)` | `O(1)` |
| **38** | 2D Vector Cross Product | ax * by - ay * bx | `O(1)` | `O(1)` |
| **39** | Orientation of 3 Points | 2D Signed Cross Product Area | `O(1)` | `O(1)` |
| **40** | Modulo 2^k | Bitmask AND: x & ((1 << k) - 1) | `O(1)` | `O(1)` |
| **41** | Find Missing & Repeating Number | XOR Bucket Separation Partition | `O(N)` | `O(1)` |
| **42** | Sum of Two Integers - Bitwise (LC 371) | Bitwise XOR & AND Carry Shift Loop | `O(1)` | `O(1)` |

---

### 9. Recursion & Backtracking (`recursion.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Subsets / Power Set (LC 78) | Pick / Non-Pick DFS Backtracking | `O(N * 2^N)` | `O(N)` |
| **2** | Subsets II with Duplicates (LC 90) | Sort + Skip Duplicate Branches | `O(N * 2^N)` | `O(N)` |
| **3** | Non-Decreasing Subsequences (LC 491) | Hash Set per Recursion Level | `O(2^N)` | `O(N)` |
| **4** | Combinations of K numbers (LC 77) | Bounded Depth Backtracking | `O(C(N, K))` | `O(K)` |
| **5** | Combination Sum I (LC 39) | Unbounded Element Reuse Backtrack | `O(2^T)` | `O(T / min_val)` |
| **6** | Combination Sum II (LC 40) | Sort + Single-Use Duplicate Skip | `O(2^N)` | `O(N)` |
| **7** | Combination Sum III (LC 216) | Digits 1..9 Backtracking (K nums) | `O(C(9, K))` | `O(K)` |
| **8** | Permutations of Array (LC 46) | In-Place Element Swapping DFS | `O(N! * N)` | `O(N)` |
| **9** | Permutations II with Duplicates (LC 47) | Hash Set Swaps per Level DFS | `O(N! * N)` | `O(N)` |
| **10** | K-th Permutation Sequence (LC 60) | Factorial Number System (Lehmer) | `O(N^2)` | `O(N)` |
| **11** | Palindrome Partitioning (LC 131) | 2D DP Table + DFS Backtracking | `O(N * 2^N)` | `O(N^2)` |
| **12** | Word Break II (LC 140) | Memoized DFS + Trie/Set Lookup | `O(2^N)` | `O(2^N)` |
| **13** | Rat in a Maze | 4-Directional DFS Grid Search | `O(4^(N^2))` | `O(N^2)` |
| **14** | Word Search in 2D Grid (LC 79) | In-Place Visited Matrix DFS | `O(N * M * 4^L)` | `O(L)` |
| **15** | N-Queens Problem (LC 51) | Column & Diagonal Bit/Array Masks | `O(N!)` | `O(N)` |
| **16** | Sudoku Solver (LC 37) | Row, Col & 3x3 Box Bitmasks | `O(9^(N^2))` | `O(1)` |
| **17** | Next Permutation (LC 31) | 3-Step Suffix Inversion Reversal | `O(N)` | `O(1)` |
| **18** | Restore IP Addresses (LC 93) | 4-Segment Valid Integer Parsing | `O(1)` | `O(1)` |
| **19** | Subset Sums (GFG) | Binary Choice Tree Recursion | `O(2^N log(2^N))` | `O(N)` |
| **20** | Graph M-Coloring (GFG) | Vertex-by-Vertex Safety DFS | `O(M^N)` | `O(N)` |

---

### 10. Sliding Window & Two Pointers (`sliding.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Maximum Sum Subarray of Size K | Fixed-Size Sliding Window | `O(N)` | `O(1)` |
| **2** | Longest Substring Without Repeating (LC 3) | Variable Window + ASCII Direct Map | `O(N)` | `O(1)` |
| **3** | Minimum Size Subarray Sum (LC 209) | Variable Window + Left Shrinking | `O(N)` | `O(1)` |
| **4** | Longest Substring At Most K Distinct (LC 340) | Sliding Window + Frequency Table | `O(N)` | `O(K)` |
| **5** | Minimum Window Substring (LC 76) | Sliding Window + Missing Match Count | `O(len(S) + len(T))` | `O(1)` |
| **6** | Subarrays with K Distinct Integers (LC 992) | Exactly(K) = AtMost(K) - AtMost(K-1) | `O(N)` | `O(K)` |
| **7** | Anagrams (LC 438) & Permutation (LC 567) | Fixed-Size Window Freq Matching | `O(len(S))` | `O(1)` |
| **8** | Longest Repeating Char Replacement (LC 424) | Window - MaxFreq <= K Condition | `O(N)` | `O(1)` |
| **9** | Max Consecutive Ones III (LC 1004) | Sliding Window Zero Counter <= K | `O(N)` | `O(1)` |
| **10** | Fruit Into Baskets (LC 904) | At Most 2 Types Sliding Window | `O(N)` | `O(1)` |
| **11** | Subarray Sum Equals K (LC 560) | Prefix Sum + Frequency Hash Map | `O(N)` | `O(N)` |
| **12** | Contiguous Array - Equal 0s & 1s (LC 525) | Cumulative Balance + First-Seen Map | `O(N)` | `O(N)` |
| **13** | Subarray Sums Divisible by K (LC 974) | Normalized Modulo Remainder Map | `O(N)` | `O(K)` |
| **14** | Difference Array / Range Addition (LC 370) | Sweep-Line Prefix Difference Array | `O(N + Q)` | `O(N)` |

---

### 11. Sorting & Binary Search (`sorting.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Bubble Sort | Adjacent Inversion Swaps | `O(N^2)` | `O(1)` |
| **2** | Selection Sort | Minimum Element Position Swap | `O(N^2)` | `O(1)` |
| **3** | Insertion Sort | Sorted Prefix Key Shift | `O(N^2)` | `O(1)` |
| **4** | Merge Sort & Inversion Counting | Divide & Conquer Merge Counting | `O(N log N)` | `O(N)` |
| **5** | Quick Sort | Randomized Pivot Partitioning | `O(N log N)` | `O(log N)` |
| **6** | Quick Select (Kth Smallest Element) | Lomuto In-Place Partitioning | `O(N)` avg | `O(1)` |
| **7** | Heap Sort | Max-Heap In-Place Sifting | `O(N log N)` | `O(1)` |
| **8** | Cycle Sort | In-Place Index Mapping Swap | `O(N)` | `O(1)` |
| **9** | Counting Sort | Non-Comparison Frequency Array | `O(N + K)` | `O(N + K)` |
| **10** | Lower Bound & Upper Bound | Binary Search Range Halving | `O(log N)` | `O(1)` |
| **11** | Median of Two Sorted Arrays (LC 4) | Binary Search on Partition Cut | `O(log(min(M, N)))` | `O(1)` |
| **12** | K-th Element of Two Sorted Arrays | Binary Search on Partition Cut | `O(log(min(M, N)))` | `O(1)` |
| **13** | Search in Rotated Sorted Array I (LC 33) | Half-Sorted Binary Search | `O(log N)` | `O(1)` |
| **14** | Search in Rotated Sorted Array II (LC 81) | Boundary Duplicate Trimming | `O(N)` worst | `O(1)` |
| **15** | Find Minimum in Rotated Sorted Array (LC 153)| Binary Search Inflection Point | `O(log N)` | `O(1)` |
| **16** | Single Element in Sorted Array (LC 540) | Even-Odd Index Parity Binary Srch | `O(log N)` | `O(1)` |
| **17** | Find Peak Element (LC 162) | Gradient Ascent Binary Search | `O(log N)` | `O(1)` |
| **18** | Search in 2D Matrix I & II (LC 74 / 240) | Staircase Top-Right Corner Scan | `O(M + N)` | `O(1)` |
| **19** | Matrix Median (Row-Wise Sorted) | Binary Search on Range + UB Count | `O(32 * R log C)` | `O(1)` |
| **20** | Split Array Largest Sum (LC 410 / 1011) | Binary Search on Answer + Greedy | `O(N log(sum))` | `O(1)` |
| **21** | Aggressive Cows (Max Min Distance) | Binary Search on Answer + Greedy | `O(N log(max_dist))` | `O(1)` |
| **22** | House Robber IV (Min Max Capability) | Binary Search on Answer + Greedy | `O(N log(max_val))` | `O(1)` |
| **23** | Dutch National Flag (LC 75) | 3-Pointer In-Place Partitioning | `O(N)` | `O(1)` |
| **24** | Boyer-Moore Majority Element (LC 169) | Vote Cancellation Algorithm | `O(N)` | `O(1)` |
| **25** | Merge Overlapping Intervals (LC 56) | Start-Time Sort + Linear Merge | `O(N log N)` | `O(N)` |
| **26** | Insert Interval (LC 57) | 3-Phase Linear Partition & Merge | `O(N)` | `O(N)` |
| **27** | Non-overlapping Intervals (LC 435) | Earliest End-Time Sorting | `O(N log N)` | `O(1)` |
| **28** | Sweep-Line Concurrent Events (LC 253) | Chronological Endpoint Sorting | `O(N log N)` | `O(N)` |
| **29** | Meet-in-the-Middle (LC 2035) | Split 2^(N/2) + Binary Search | `O(N * 2^(N/2))` | `O(2^(N/2))` |
| **30** | Smallest Impossible Subset Sum | Greedy Running Reachable Frontier | `O(N log N)` | `O(1)` |
| **31** | Candy Distribution (LC 135) | 2-Pass Greedy Rating Synchronization | `O(N)` | `O(N)` |
| **32** | Longest Consecutive Sequence (LC 128) | Hash Set Boundary (x - 1 lookup) | `O(N)` | `O(N)` |
| **33** | Two Sum (LC 1) | Hash Map Complement Lookup | `O(N)` | `O(N)` |
| **34** | Group Anagrams (LC 49) | Sorted String / Count Hash Bucket | `O(N * K log K)` | `O(N * K)` |
| **35** | Product of Array Except Self (LC 238) | Prefix & Suffix Products In-Place | `O(N)` | `O(1)` |
| **36** | Find All Duplicates in Array (LC 442) | In-Place Sign Inversion Hashing | `O(N)` | `O(1)` |
| **37** | Valid Sudoku (LC 36) | Bitmask / Set Validation | `O(1)` | `O(1)` |
| **38** | Rotate Array by K (LC 189) | 3-Step Range Reversal Algorithm | `O(N)` | `O(1)` |
| **39** | Container With Most Water (LC 11) | Inward Converging Two Pointers | `O(N)` | `O(1)` |
| **40** | 3Sum & 4Sum (LC 15 / 18) | Sorting + Two-Pointer Shrinking | `O(N^2) / O(N^3)` | `O(1)` |
| **41** | Shortest Unsorted Subarray (LC 581) | Running Prefix Max & Suffix Min | `O(N)` | `O(1)` |
| **42** | Time Based Key-Value Store (LC 981) | Hash Map + Binary Search UB | `O(log N)` | `O(N)` |
| **43** | Pascal's Triangle (LC 118) | Dynamic Programming Row Building | `O(N^2)` | `O(N^2)` |
| **44** | Merge Two Sorted Arrays In-Place (LC 88) | Backwards 3-Pointer Placement | `O(N + M)` | `O(1)` |
| **45** | Find Duplicate Number (LC 287) | Floyd's Tortoise & Hare Cycle Det | `O(N)` | `O(1)` |
| **46** | Pow(x, n) (LC 50) | Binary Exponentiation (Squaring) | `O(log N)` | `O(1)` |
| **47** | Reverse Pairs (LC 493) | Merge Sort Inversion Counting | `O(N log N)` | `O(N)` |
| **48** | Largest Subarray with 0 Sum (GFG) | Prefix Sum + Earliest Index Map | `O(N)` | `O(N)` |
| **49** | Remove Duplicates from Sorted Array (LC 26) | Two Pointers (Slow Write / Fast) | `O(N)` | `O(1)` |
| **50** | Max Consecutive Ones (LC 485) | Single-Pass Linear Counter | `O(N)` | `O(1)` |
| **51** | N Meetings in One Room (GFG) | Greedy Earliest Finish Time | `O(N log N)` | `O(N)` |
| **52** | Minimum Platforms for Railway (GFG) | Event Chronological Sort (2 Pointers) | `O(N log N)` | `O(1)` |
| **53** | Job Sequencing with Deadlines (GFG) | Profit Sorting + Slot Allocation | `O(N^2)` | `O(max_deadline)` |
| **54** | Fractional Knapsack (GFG) | Value-to-Weight Ratio Sorting | `O(N log N)` | `O(1)` |
| **55** | N-th Root of an Integer (GFG) | Binary Search on Answer [1, M] | `O(N log M)` | `O(1)` |
| **56** | Majority Element II (> N/3) (LC 229) | Extended Boyer-Moore Voting (2 Cands) | `O(N)` | `O(1)` |
| **57** | Minimize Max Distance to Gas Station (LC 774) | Continuous Binary Search (eps 1e-6) | `O(N log(max_dist / eps))` | `O(1)` |

---

### 12. Monotonic Stack & Deque (`stack.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | Next Greater Element (LC 496) | Monotonic Decreasing Stack | `O(N)` | `O(N)` |
| **2** | Next Greater Element II - Circular (LC 503) | Monotonic Stack on Doubled (2N) | `O(N)` | `O(N)` |
| **3** | Next Smaller Element (NSE) | Monotonic Increasing Stack | `O(N)` | `O(N)` |
| **4** | Previous Greater & Smaller (PGE / PSE) | Monotonic Stack Traversal | `O(N)` | `O(N)` |
| **5** | Daily Temperatures (LC 739) | Monotonic Decreasing Index Stack | `O(N)` | `O(N)` |
| **6** | 132 Pattern (LC 456) | Monotonic Stack + Max S3 Tracker | `O(N)` | `O(N)` |
| **7** | Largest Rectangle in Histogram (LC 84) | Monotonic Stack Boundary Indices | `O(N)` | `O(N)` |
| **8** | Maximal Rectangle in Binary Grid (LC 85) | 1D Histogram DP + Monotonic Stack | `O(M * N)` | `O(N)` |
| **9** | Sum of Subarray Minimums (LC 907) | Contribution Principle (PSE x NSE) | `O(N)` | `O(N)` |
| **10** | Sum of Subarray Ranges (LC 2104) | Contribution: Subarray(Max - Min) | `O(N)` | `O(N)` |
| **11** | Trapping Rain Water (LC 42) | Inward Two Pointers / Mono Stack | `O(N)` | `O(1)` |
| **12** | Asteroid Collision (LC 735) | Directional Collision Stack | `O(N)` | `O(N)` |
| **13** | Sliding Window Maximum (LC 239) | Monotonic Decreasing Deque | `O(N)` | `O(K)` |
| **14** | Online Stock Span (LC 901) | Monotonic Stack of (Price, Span) | `O(1)` avg | `O(N)` |
| **15** | Valid Parentheses & Min Add (LC 20 & 921) | Character Matching Stack / Counter | `O(N)` | `O(N) / O(1)` |
| **16** | Remove K Digits (LC 402) | Monotonic Increasing String Stack | `O(N)` | `O(N)` |
| **17** | Remove Duplicate Letters (LC 316 / 1081) | Monotonic Stack + Seen Set + Freq | `O(N)` | `O(1)` |
| **18** | Reverse Polish Notation (LC 150) | Operand Stack Evaluation | `O(N)` | `O(N)` |
| **19** | Basic Calculator II (LC 227) | Precedence Stack + Accumulator | `O(N)` | `O(N)` |
| **20** | Decode String (LC 394) | Count Stack + String Context Stack | `O(N)` | `O(N)` |
| **21** | Min Stack (LC 155) | 2*val - minVal Math / Aux Stack | `O(1)` all | `O(1) / O(N)` |
| **22** | Array Stack & Queue Implementation | Fixed Buffer + Pointer Tracking | `O(1)` all | `O(Cap)` |
| **23** | Stack via Queue & Queue via Stack (LC 225) | 2 Stacks / 1 Queue Cost Inversion | `O(1) / O(N)` | `O(N)` |
| **24** | Recursive Stack Sorting | Two-Level Recursive Insertion Sort | `O(N^2)` | `O(N)` |
| **25** | LFU Cache (LC 460) | Hash Map + Freq-to-List + minFreq | `O(1)` all | `O(Cap)` |
| **26** | The Celebrity Problem (LC 277) | Two-Pointer Candidate Elimination | `O(N)` | `O(1)` |
| **27** | Max of Mins Every Window Size | Monotonic Stack (PSE/NSE) + Suffix | `O(N)` | `O(N)` |
| **28** | Count Bracket Reversals for Balance (GFG) | Counter Balance Math ((o+1)/2+(c+1)/2) | `O(N)` | `O(1)` |
| **29** | LRU Cache (LC 146) | Hash Map + Doubly Linked List | `O(1)` get/put | `O(Cap)` |

---

### 13. String Algorithms (`string.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | FastHash Infrastructure | Polynomial Prefix Rolling Hash | `O(N)` build, `O(1)` query | `O(N)` |
| **2** | Rabin-Karp String Matching | Rolling Hash Substring Match | `O(N + M)` | `O(N)` |
| **3** | Finding Borders (Prefix == Suffix) | Hash Verification of Pref/Suff | `O(N)` | `O(N)` |
| **4** | Finding String Periods | Rolling Hash Substring Periodicity | `O(N)` | `O(N)` |
| **5** | O(1) Palindrome Range Queries | Forward & Backward Hash Arrays | `O(N)` build, `O(1)` query | `O(N)` |
| **6** | Minimum Window Substring (LC 76) | Sliding Window + Match Counter | `O(len(S) + len(T))` | `O(1)` |
| **7** | Longest Substring Without Repeating (LC 3) | Sliding Window + Direct Array Map | `O(N)` | `O(1)` |
| **8** | Group Anagrams (LC 49) | Sorted String / Count Hash Bucket | `O(N * K log K)` | `O(N * K)` |
| **9** | Trie Prefix Tree (LC 208) | Array-Based Trie Node Traversal | `O(L)` per op | `O(Σ * N)` |
| **10** | KMP Algorithm (Prefix Function Pi) | Failure Table State Transitions | `O(N)` | `O(N)` |
| **11** | Z-Algorithm (Longest Common Prefix) | Sliding Box [L, R] Segment Match | `O(N)` | `O(N)` |
| **12** | Manacher's Algorithm (Palindromic Radii) | Symmetry Center & Boundary Jump | `O(N)` | `O(N)` |
| **13** | Longest Palindromic Substring (LC 5) | Expand Around Centers (2N - 1) | `O(N^2)` | `O(1)` |
| **14** | Booth's Algorithm (Min String Rotation) | Modified KMP on Doubled (S + S) | `O(N)` | `O(N)` |
| **15** | Shortest Palindrome (LC 214) | KMP Pi Table on (S + '#' + revS) | `O(N)` | `O(N)` |
| **16** | Word Break Counting (CSES / LC 139) | 1D DP + Trie Multi-Pattern Match | `O(N * L)` | `O(TotalL)` |
| **17** | Substring Concatenation All Words (LC 30) | Multi-Offset Fixed Window HashMap | `O(N * K)` | `O(Words)` |
| **18** | Isomorphic Strings (LC 205) | Dual 256-ASCII Mapping Arrays | `O(N)` | `O(1)` |
| **19** | Reverse Words in a String (LC 151) | Word Tokenization & Suffix Build | `O(N)` | `O(N)` |
| **20** | Roman to Integer (LC 13) | Right-to-Left Value Subtraction | `O(N)` | `O(1)` |
| **21** | String to Integer / ATOI (LC 8) | Whitespace Strip + Overflow Clamp | `O(N)` | `O(1)` |
| **22** | Longest Common Prefix (LC 14) | Horizontal / Vertical Scan Match | `O(N * L)` | `O(1)` |
| **23** | Min Insertions for Palindrome (LC 1312) | len(S) - LCS(S, reverse(S)) DP | `O(N^2)` | `O(N^2)` |
| **24** | Count and Say (LC 38) | Run-Length Encoding Simulation | `O(2^N)` | `O(2^N)` |
| **25** | Compare Version Numbers (LC 165) | Dot-Separated Numerical Parsing | `O(N + M)` | `O(1)` |
| **26** | Encode and Decode Strings (LC 271) | Length-Prefixed Framing (<len>#<str>) | `O(N)` | `O(1)` |

---

### 14. Tree Algorithms & BST (`tree.cpp`)

| # | Problem Name | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :--- | :--- | :--- |
| **1** | DFS Traversals (In/Pre/Post) | Recursive & Iterative Stack Traversal | `O(N)` | `O(H)` |
| **2** | Level Order Traversal (BFS) | Queue-Based Level-by-Level Scan | `O(N)` | `O(W)` |
| **3** | Maximum Depth / Height of Tree | Bottom-Up Post-Order DFS | `O(N)` | `O(H)` |
| **4** | Same Tree Verification | Dual Simultaneous Pre-Order DFS | `O(N)` | `O(H)` |
| **5** | Symmetric Tree (Mirror Symmetry) | Mirror Branch Pair DFS Traversal | `O(N)` | `O(H)` |
| **6** | Diameter of Binary Tree | Bottom-Up Post-Order Max Diameter | `O(N)` | `O(H)` |
| **7** | Balanced Binary Tree Check | Height-Balanced Bottom-Up Check | `O(N)` | `O(H)` |
| **8** | Maximum Path Sum | Post-Order DFS Branch Gain Tracker | `O(N)` | `O(H)` |
| **9** | Root to Node Path | Backtracking Pre-Order Path Vector | `O(N)` | `O(H)` |
| **10** | Lowest Common Ancestor (Binary Tree) | Bottom-Up Node Value Bubbling DFS | `O(N)` | `O(H)` |
| **11** | Left View of Binary Tree | Level First-Seen Pre-Order DFS | `O(N)` | `O(H)` |
| **12** | Right View of Binary Tree | Level First-Seen Reverse Pre-Order | `O(N)` | `O(H)` |
| **13** | Vertical Order Traversal | BFS Queue with (Col, Row) TreeMap | `O(N log N)` | `O(N)` |
| **14** | Zigzag Level Order Traversal | BFS Queue + Directional Inversion | `O(N)` | `O(W)` |
| **15** | Invert / Mirror Binary Tree | Recursive Left-Right Subtree Swap | `O(N)` | `O(H)` |
| **16** | Search in BST | Directional Key Branch Traversal | `O(H)` | `O(1)` |
| **17** | Insert into BST | Directional Iterative Insertion | `O(H)` | `O(1)` |
| **18** | Validate BST | Top-Down (Low, High) Range Check | `O(N)` | `O(H)` |
| **19** | K-th Smallest Element in BST | Inorder Traversal Stack Counting | `O(H + K)` | `O(H)` |
| **20** | Lowest Common Ancestor in BST | Directional Range Branching | `O(H)` | `O(1)` |
| **21** | Build Tree from Preorder & Inorder | Divide & Conquer + Inorder Hash | `O(N)` | `O(N)` |
| **22** | Count Complete Tree Nodes | Binary Search on Heights (2^H) | `O(log^2 N)` | `O(log N)` |
| **23** | Sum of Root-to-Leaf Numbers | Top-Down Base-10 Accumulation DFS | `O(N)` | `O(H)` |
| **24** | Path Sum (I, II & Tree Paths) | Backtracking Target Path DFS | `O(N)` | `O(H)` |
| **25** | Serialize & Deserialize Binary Tree | Preorder DFS + String Stream Token | `O(N)` | `O(N)` |
| **26** | Morris Inorder Traversal | Threaded Binary Tree (O(1) Space) | `O(N)` | `O(1)` |
| **27** | Morris Preorder Traversal | Threaded Binary Tree (O(1) Space) | `O(N)` | `O(1)` |
| **28** | Top View of Binary Tree | BFS + Column First-Seen Hash Map | `O(N)` | `O(N)` |
| **29** | Bottom View of Binary Tree | BFS + Column Last-Seen Hash Map | `O(N)` | `O(N)` |
| **30** | All 3 Traversals in One Pass | Single Stack with State Counter | `O(N)` | `O(N)` |
| **31** | Boundary Traversal of Binary Tree | Left Boundary + Leaves + Right Rev | `O(N)` | `O(H)` |
| **32** | Maximum Width of Binary Tree | BFS with 0-Indexed Level Offsets | `O(N)` | `O(W)` |
| **33** | Children Sum Property Binary Tree | Top-Down Increment + Bottom-Up Sum | `O(N)` | `O(H)` |
| **34** | Build Tree from Postorder & Inorder | Divide & Conquer + Inorder Hash | `O(N)` | `O(N)` |
| **35** | Flatten Binary Tree to Linked List | Reverse Post-Order Traversal Rewire | `O(N)` | `O(H)` |
| **36** | Populating Next Right Pointers | Level-Order Pointer Rewiring | `O(N)` | `O(1)` |
| **37** | Inorder Successor & Predecessor in BST | Directional Key Comparison Search | `O(H)` | `O(1)` |
| **38** | Floor & Ceil in BST | Binary Search Candidate Tracking | `O(H)` | `O(1)` |
| **39** | Two Sum in BST (Pair with Sum K) | Dual BST Iterators (Next/Prev) | `O(N)` | `O(H)` |
| **40** | BST Iterator (O(1) Amortized) | Controlled Inorder Stack Push | `O(1)` avg | `O(H)` |
| **41** | Largest BST in Binary Tree | Post-Order Node Info Quadruple | `O(N)` | `O(H)` |
| **42** | Minimum Time to Burn Binary Tree | Parent Map + Multi-Source BFS | `O(N)` | `O(N)` |
| **43** | Delete a Node in BST | Inorder Successor / Child Splice | `O(H)` | `O(H)` |
| **44** | Convert Sorted Array to BST (LC 108) | Midpoint Divide & Conquer Recursion | `O(N)` | `O(log N)` |
| **45** | Construct BST from Preorder (LC 1008) | Upper-Bound Constrained DFS | `O(N)` | `O(H)` |
| **46** | Binary Tree to Doubly Linked List | Inorder DFS Pointer Stitching | `O(N)` | `O(H)` |
| **47** | Subtree of Another Tree (LC 572) | DFS Traversal + Same Tree Match | `O(N * M)` | `O(H)` |
