# 1-Week Microsoft Interview Strategy & Pattern Decomposition Plan

You already have 150 Core Solved Problems (in `1.cpp` to `8.cpp`) and 55 High-Frequency Delta Problems (in `report.md`).
The remaining **586 Medium & Hard DSA questions** in `questions.md` are not meant to be solved linearly. 
Use the **6 Master Template Modules** in `DSA-Code/` as your primary study and implementation reference.

---

## Master Template Modules Quick Index

| Module | File Path | Focus Area | Implemented Algorithms |
|---|---|---|---|
| **Trees & BST** | `tree_template.cpp` | Traversals, Views, Subtree aggregation, LCA, Morris O(1), BST Operations | 46 Algorithms (1,588 lines) |
| **Graphs & Grids** | `graph_template.cpp` | BFS/DFS, Cycle checks, TopoSort, Dijkstra, 0-1 BFS, DSU, Tarjan, Bridges | 39 Algorithms (1,685 lines) |
| **Monotonic Stack & Queue** | `stack_template.cpp` | NGE/NSE/PGE/PSE, Histograms, Contribution technique, Parsing, LFU Cache | 27 Algorithms (984 lines) |
| **Sliding Window & Two Pointers** | `sliding_template.cpp` | Fixed/Variable Windows, Exact-K Reduction, Prefix Sum, Sweep-Line | 14 Algorithms (401 lines) |
| **Dynamic Programming** | `dp_template.cpp` | 1D, 2D Grid, String, LIS, Interval, Knapsack, State Machine, Game DP | 38 Algorithms (1,287 lines) |
| **Greedy & Intervals** | `greedy_template.cpp` | Interval Scheduling, Jump Game, Two-Pass Balance, Task Scheduler, Huffman | 35 Algorithms (900 lines) |

---

## Fact-Checked Pattern Decomposition (586 Remaining DSA Questions)

| Rank | Core Pattern Family | Questions Count | Share (%) | Primary Reference Module |
|---|---|---|---|---|
| 1 | Dynamic Programming | 96 | 16.4% | `dp_template.cpp` |
| 2 | Graph & Grid Search (BFS / DFS) | 88 | 15.0% | `graph_template.cpp` |
| 3 | Bit Manipulation, Math & Fast Arithmetic | 64 | 10.9% | `7.cpp` & `final.md` |
| 4 | Sliding Window & Two Pointers | 53 | 9.0% | `sliding_template.cpp` |
| 5 | Binary Trees & BSTs | 48 | 8.2% | `tree_template.cpp` |
| 6 | Binary Search (Array & Predicate on Answer) | 32 | 5.5% | `1.cpp` & `report.md` |
| 7 | Hash Map, Set & Prefix Sum | 20 | 3.4% | `sliding_template.cpp` |
| 8 | Monotonic Stack & Monotonic Queue | 17 | 2.9% | `stack_template.cpp` |
| 9 | Backtracking & Combinatorics | 17 | 2.9% | `5.cpp` & `report.md` |
| 10 | Greedy Algorithms & Interval Scheduling | 15 | 2.6% | `greedy_template.cpp` |
| 11 | Design & System Data Structures | 13 | 2.2% | `stack_template.cpp` & `8.cpp` |
| 12 | Heap / Priority Queue | 6 | 1.0% | `greedy_template.cpp` & `2.cpp` |
| 13 | General Array & String Simulation | 117 | 20.0% | `1.cpp`, `7.cpp` |
| **Total** | **Pure Non-Redundant DSA Archive** | **586** | **100%** | **6 Master Template Modules** |

---

## 7-Day Precision Execution Schedule

### Day 1: Graph Traversal, Grid Search & Topological Sort
- **Archive Size**: 88 questions (15.0%)
- **Master Reference**: `graph_template.cpp`
  - Multi-Source BFS: Section 17 & Section 35
  - Cycle Detection (Undirected & Directed): Sections 4 & 5
  - Topological Sort (Kahn's BFS): Section 7 & Section 32 (Course Schedule)
  - Shortest Path Suite (Dijkstra, 0-1 BFS): Sections 9, 10, 11
  - Disjoint Set Union (DSU): Section 11 & Section 33 (Accounts Merge)
  - Live Interview Selection Matrix: Lines 1584-1685
- **3 Target Archive Problems to Prototype**:
  1. [Evaluate Division](https://leetcode.com/problems/evaluate-division) (Medium, ID: 399) - Graph BFS with weighted edge product.
  2. [Amount of Time for Binary Tree to Be Infected](https://leetcode.com/problems/amount-of-time-for-binary-tree-to-be-infected) (Medium, ID: 2385) - Tree to Graph + Multi-source BFS.
  3. [Number of Provinces](https://leetcode.com/problems/number-of-provinces) (Medium, ID: 547) - Connected components baseline.

---

### Day 2: Sliding Window, Two Pointers & Prefix Sum
- **Archive Size**: 53 questions (9.0%)
- **Master Reference**: `sliding_template.cpp`
  - Variable Sliding Window: Sections 2, 4, 8, 9
  - Minimum Window Substring: Section 5
  - Exact-K Reduction (`atMost(K) - atMost(K-1)`): Section 6
  - Prefix Sum Frequency Maps: Sections 11, 12, 13
  - Difference Array / Sweep Line: Section 14
- **3 Target Archive Problems to Prototype**:
  1. [Number of Substrings Containing All Three Characters](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters) (Medium, ID: 1358) - Counting subarrays via last seen indices.
  2. [Minimum Number of Flips to Make the Binary String Alternating](https://leetcode.com/problems/minimum-number-of-flips-to-make-the-binary-string-alternating) (Medium, ID: 1888) - Sliding window over doubled string `s + s`.
  3. [Boats to Save People](https://leetcode.com/problems/boats-to-save-people) (Medium, ID: 881) - Inward Two Pointers greedy pairing.

---

### Day 3: Dynamic Programming (Linear, Grid, Interval & State Machine)
- **Archive Size**: 96 questions (16.4%)
- **Master Reference**: `dp_template.cpp`
  - Basic 1D DP: Section 1 (Climbing Stairs, House Robber, Decode Ways)
  - 0/1 & Unbounded Knapsack: Section 2 (Coin Change I & II, Target Sum)
  - String DP: Section 3 (LCS, Edit Distance, Distinct Subsequences, Word Break)
  - LIS & Weighted Scheduling: Section 4 (LIS O(N log N), Job Scheduling)
  - 2D Grid DP: Section 5 (Unique Paths, Min Path Sum, Maximal Square)
  - Interval DP: Section 6 (MCM, Burst Balloons, Cost to Cut Stick)
  - State Machine & Game DP: Sections 8 & 11 (Stock Cooldown/Fee, Stone Games)
  - DP Cheat Sheet & Knapsack Loop Rule: Lines 1234-1280
- **3 Target Archive Problems to Prototype**:
  1. [Maximal Square](https://leetcode.com/problems/maximal-square) (Medium, ID: 221) - 2D Grid DP local min square extension.
  2. [Minimum Cost to Cut a Stick](https://leetcode.com/problems/minimum-cost-to-cut-a-stick) (Hard, ID: 1547) - Interval DP with sorted cut coordinates.
  3. [Maximum Non Negative Product in a Matrix](https://leetcode.com/problems/maximum-non-negative-product-in-a-matrix) (Medium, ID: 1594) - 2D DP tracking min and max products.

---

### Day 4: Binary Trees, BST Operations & Traversals
- **Archive Size**: 48 questions (8.2%)
- **Master Reference**: `tree_template.cpp`
  - Traversals (Recursive, Iterative 1-Stack, Morris O(1)): Sections 1, 26, 27, 30
  - BFS & Coordinate Views (Left/Right/Top/Bottom, Vertical, Width): Sections 2, 11, 12, 13, 28, 29, 32
  - Subtree Aggregations (Height, Diameter, Max Path Sum, Balanced): Sections 3, 6, 7, 8
  - Lowest Common Ancestor (LCA Tree & BST): Sections 10 & 20
  - Tree Modifications & Reconstructions: Sections 15, 21, 34, 35, 46
  - Complete BST Operations (Search, Insert, Delete, Validate, BST Iterators): Sections 16, 17, 18, 19, 39, 40, 41, 43
  - Tree Interview Cheat Sheet: Lines 1523-1588
- **3 Target Archive Problems to Prototype**:
  1. [Delete Tree Nodes](https://leetcode.com/problems/delete-tree-nodes) (Medium, ID: 1273) - Postorder subtree sum and node count aggregation.
  2. [Validate Binary Tree Nodes](https://leetcode.com/problems/validate-binary-tree-nodes) (Medium, ID: 1361) - Exactly 1 root with indegree 0, all others indegree 1, no cycles.
  3. [Binary Tree Zigzag Level Order Traversal](https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal) (Medium, ID: 103) - Deque / Vector reversing level traversal.

---

### Day 5: Monotonic Stack, Queue & Binary Search on Answer
- **Archive Size**: 49 questions (Monotonic Stack: 17, Binary Search on Answer: 32)
- **Master Reference**: `stack_template.cpp` & `greedy_template.cpp`
  - Next/Previous Greater & Smaller Elements: Sections 1, 2, 3, 4
  - Histogram & Subarray Bounds Contribution: Sections 7, 8, 9, 10 (LC 84, 85, 907, 2104)
  - Trapping Rain Water & Asteroid Collisions: Sections 11 & 12
  - Lexicographical Greedy Stack: Sections 16 & 17 (Remove K Digits, Remove Duplicate Letters)
  - Expression & Nested String Parsing: Sections 18, 19, 20 (RPN, Basic Calculator, Decode String)
  - Sliding Window Maximum (Monotonic Deque): Section 13
  - Monotonic Stack Decision Matrix: Lines 961-983
- **3 Target Archive Problems to Prototype**:
  1. [Magnetic Force Between Two Balls](https://leetcode.com/problems/magnetic-force-between-two-balls) (Medium, ID: 1552) - Predicate binary search on minimum distance.
  2. [Remove K Digits](https://leetcode.com/problems/remove-k-digits) (Medium, ID: 402) - Monotonic increasing stack to form smallest number.
  3. [Decode String](https://leetcode.com/problems/decode-string) (Medium, ID: 394) - Dual stack (multipliers & string contexts).

---

### Day 6: Greedy Algorithms, Intervals & System Design
- **Archive Size**: 41 questions (Greedy/Intervals: 15, Design: 13, Backtracking: 17)
- **Master Reference**: `greedy_template.cpp` & `stack_template.cpp`
  - Interval Scheduling & Merge: Section 1 (LC 435, 452, 56, 57, 253)
  - Reachability & Jump Game: Section 2 (LC 55, 45, 1024)
  - Two-Pass & Balance Greedy: Section 3 (Candy, Gas Station, Parentheses with '*')
  - Matching & Cooldown Formulas: Section 4 (Task Scheduler, Reorganize String, Largest Number)
  - Caching & Data Structure Design: `stack_template.cpp` (LFU Cache, Min Stack O(1), Array Stacks/Queues)
  - Greedy Cheat Sheet & Sorting Rules: Lines 455-495
- **3 Target Archive Problems to Prototype**:
  1. [Design Authentication Manager](https://leetcode.com/problems/design-authentication-manager) (Medium, ID: 1797) - Hash Map + Token expiry management.
  2. [Candy](https://leetcode.com/problems/candy) (Hard, ID: 135) - Two-Pass Greedy scan (Left-to-Right + Right-to-Left max).
  3. [Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses) (Medium, ID: 93) - Backtracking with segment validation [0..255].

---

### Day 7: 1-Day Before Interview - Pure Pattern Consolidation (Zero New Code)
- **Goal**: Solidify invariants, review templates, rehearse out-loud communication.
- **Morning (2 Hours)**:
  - Read `final.md`: Review all 20 Core Patterns, Invariants, and Canonical Templates.
  - Skim `report.md`: Verify you can verbally state the algorithm for all 55 recent delta questions.
- **Afternoon (2 Hours)**:
  - Review the cheat sheets at the bottom of each template file:
    - `tree_template.cpp Cheat Sheet`
    - `graph_template.cpp Selection Matrix`
    - `stack_template.cpp Decision Matrix`
    - `dp_template.cpp Recognition Matrix`
    - `greedy_template.cpp Sorting Rules`
  - Practice verbal explanation: State approach, invariants, edge cases, time/space complexity before coding.
- **Evening**:
  - Stop all prep. Rest, hydrate, sleep 8 hours. Confident execution beats last-minute grinding.

---

## Action Plan Summary

| Action Type | What to Do | What NOT to Do |
|---|---|---|
| **Remaining 586 Questions** | Treat as a pattern taxonomy and diagnostic lookup | Do NOT attempt to solve all 586 sequentially |
| **Daily Routine (Days 1–6)** | 1 Template Review + 2–3 Targeted Code Prototypes | Do NOT spend 2 hours stuck on 1 obscure problem |
| **Interview Day Mindset** | Think out loud, clarify constraints, write clean invariants | Do NOT jump straight to code without discussing approach |
