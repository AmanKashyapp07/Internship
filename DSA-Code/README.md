

## Repository Map: Notebooks & Specialized Templates

This repository contains a modular, production-ready, and competitive-programming-grade C++17 codebase designed for top-tier tech interviews (FAANG/MAANG, Tier-1 FinTech, Top Startups) and Online Assessments.

---

### 1. Master Notebook Series (`DSA-Code/notebook-*.cpp`)

| File | Primary Domain | Detailed Contents & Key Problems |
|---|---|---|
| [`notebook-1.cpp`](DSA-Code/notebook-1.cpp) | **Trees, Centroid, Rolling Hash & Graphs** | Tree Height & Diameter, Centroid Decomposition, Rabin-Karp Rolling Hash, Disjoint Set Union (DSU with rank & path compression), Minimum Spanning Tree (Prim's & Kruskal's), Tree Ancestor Queries, DAG Dynamic Programming. |
| [`notebook-2.cpp`](DSA-Code/notebook-2.cpp) | **Math, Sliding Window & Classical DP** | Binary Exponentiation ($O(\log N)$), Modular Inverse, Sieve of Eratosthenes, GCD/LCM Euclidean Algorithm, Sliding Window Maximum/Minimum, Kadane's Algorithm ($O(N)$ Max Subarray), Longest Increasing Subsequence (LIS), Longest Common Subsequence (LCS). |
| [`notebook-3.cpp`](DSA-Code/notebook-3.cpp) | **Graph Traversal, Shortest Path & Bit Hacks** | Graph Cycle Detection (DFS & BFS for Directed/Undirected), 0-1 BFS (Double-ended Queue), Topological Sorting (Kahn's & DFS), Dijkstra's Algorithm, Bellman-Ford, Floyd-Warshall ($O(V^3)$), Bitwise Tricks, Submask Enumeration. |
| [`notebook-4.cpp`](DSA-Code/notebook-4.cpp) | **Binary Search on Answer & Matrices** | Median of Two Sorted Arrays ($O(\log(\min(N,M)))$ partition cuts), K-th Element of Two Sorted Arrays, Next Permutation, QuickSelect ($O(N)$ K-th Largest), Next Greater Element I & II, Spiral Matrix Traversal, Matrix Transpose & 90° Rotation. |
| [`notebook-5.cpp`](DSA-Code/notebook-5.cpp) | **Intervals, Caches & Two Pointers** | Merge Overlapping Intervals, Insert Interval, Non-overlapping Interval Removals, LRU Cache ($O(1)$ Hash Map + Doubly Linked List), Container With Most Water, 3Sum, 4Sum, Wildcard Matching, Regular Expression Matching, Gas Station Circuit. |
| [`notebook-6.cpp`](DSA-Code/notebook-6.cpp) | **Arrays, Stacks & HashMaps** | Dutch National Flag (Sort 0s, 1s, 2s), Boyer-Moore Majority Element ($O(N)$ time, $O(1)$ space), Valid Parentheses, Longest Valid Parentheses, Move Zeroes In-Place, Custom Array-Based Stack & Hash Map implementations. |
| [`notebook-7.cpp`](DSA-Code/notebook-7.cpp) | **Tree & BST Master Suite (43 Problems)** | **Complete Tree Curriculum**: Recursive Traversals (Inorder, Preorder, Postorder), Morris Traversals ($O(1)$ space), Views (Left, Right, Top, Bottom), 3-in-1 Traversal, Boundary Traversal, Max Width, Children Sum, LCA (Binary Tree, BST, Parent Pointer), BST Operations (Search, Insert, Delete, Validate, Floor/Ceil, Iterator, Pair with Sum $K$), Path Sum I, II & Tree Paths, Tree Reconstructions, Largest BST in BT, Burn Binary Tree, House Robber III (Tree DP). |
| [`notebook-8.cpp`](DSA-Code/notebook-8.cpp) | **Graph Master Suite (35 Problems)** | **Complete Graph Curriculum**: BFS/DFS, Connected Components, Cycle Detection, Bipartite Graph Coloring, Kahn's & DFS Topo Sort, Dijkstra, Bellman-Ford, Floyd-Warshall, 0-1 BFS, Network Delay Time, DSU with size/rank, Kruskal's, Prim's, Dynamic Islands II, Tarjan's Bridges (Critical Connections LC 1192), Articulation Points, Kosaraju SCC (2-Pass Transpose), Cheapest Flights within $K$ Stops (LC 787), Alien Dictionary, Course Schedule I & II, Accounts Merge (LC 721), M-Coloring, Flood Fill. |
| [`notebook-9.cpp`](DSA-Code/notebook-9.cpp) | **Number Theory, Combinatorics & Bits** | Fast I/O, Prime Factorization, Sieve Variations (Linear Sieve, Segmented Sieve), Euler's Totient Function $\phi(N)$, Modular Arithmetic & Multiplicative Inverse, Combinatorics $n\text{C}r \pmod p$, Gray Code, Submask Bitmask DP templates. |
| [`notebook-10.cpp`](DSA-Code/notebook-10.cpp) | **Dynamic Programming Master Suite (12 Sections)** | **Complete DP Curriculum**: 1D DP (House Robber I & II, Coin Change I & II, Decode Ways, Word Break, Max Product Subarray), 2D Grid DP (Unique Paths I & II, Min Path Sum, Dungeon Game LC 174, Cherry Pickup LC 741), String DP (LCS, Edit Distance, Interleaving String LC 97, Wildcard & Regex), Knapsack & Subset Sums, LIS, Interval DP (MCM, Burst Balloons), Tree DP, State Machine DP, Digit DP, Bitmask DP, Game/Minimax DP. |
| [`notebook-11.cpp`](DSA-Code/notebook-11.cpp) | **SDE Sheet Consolidated Suite (29 Modules)** | **Unique SDE Sheet Master Problems**: Pascal's Triangle, Dutch National Flag, Gap Algorithm $O(1)$ Merge, Floyd's Cycle ($N+1$ integers), Majority Element II ($> N/3$), XOR $K$ Subarrays, Repeating & Missing Number, Reverse Pairs ($O(N \log N)$ Merge Sort), 4-Sum, 0-Sum Subarray, Greedy Schedulers, Backtracking (Subsets II, Combination Sum II, Palindrome Partitioning, K-th Permutation, Rat in a Maze), LFU Cache ($O(1)$), Min Stack ($O(1)$ space), String Parsing & Matching (KMP, Z-Algorithm, Rabin-Karp), Matrix Chain Multiplication, Palindrome Cuts II, Job Scheduling Max Profit. |

---

### 📦 2. Specialized Algorithmic Templates (`DSA-Code/*_template.cpp`)

| Template File | Primary Focus | Key Algorithms & Problem Patterns Included |
|---|---|---|
| [`stack_template.cpp`](DSA-Code/stack_template.cpp) | **Monotonic Stack, Queue & Expressions** | **21 Master Modules**: Next/Previous Greater/Smaller Elements (NGE, NSE, PGE, PSE), Circular NGE II (LC 503), Daily Temperatures (LC 739), 132 Pattern (LC 456), Largest Rectangle in Histogram (LC 84), Maximal Rectangle 2D (LC 85), Sum of Subarray Minimums (LC 907) & Ranges (LC 2104), Trapping Rain Water (LC 42), Asteroid Collision (LC 735), Sliding Window Max Deque (LC 239), Online Stock Span (LC 901), Valid Parentheses, Min Add (LC 921), Remove $K$ Digits (LC 402), Remove Duplicate Letters (LC 316), RPN Evaluator (LC 150), Basic Calculator II (LC 227), Decode String (LC 394), Min Stack Designs ($O(1)$ Space). |
| [`sliding_template.cpp`](DSA-Code/sliding_template.cpp) | **Sliding Window & Prefix Sum** | **14 Master Modules**: Max Sum Subarray of Size $K$, Longest Substring Without Repeating Characters (LC 3), Minimum Size Subarray Sum (LC 209), $K$ Distinct Characters (LC 340), Minimum Window Substring (LC 76), Subarrays with $K$ Distinct Integers (LC 992 Exact $K = \text{atMost}(K) - \text{atMost}(K-1)$), Anagrams (LC 438), Permutation in String (LC 567), Longest Repeating Character Replacement (LC 424), Max Consecutive Ones III (LC 1004), Fruit into Baskets (LC 904), Subarray Sum Equals $K$ (LC 560), Contiguous Array Equal 0s & 1s (LC 525), Subarrays Divisible by $K$ (LC 974), Difference Array / Range Addition (LC 370). |
| [`sorting_template.cpp`](DSA-Code/sorting_template.cpp) | **Sorting, Two Pointers & Binary Search** | Classic Sorting (Bubble, Selection, Insertion, Merge Sort & Inversions, Quick Sort, QuickSelect, Heap Sort, Cycle Sort, Counting Sort), Lower/Upper Bound, Search in Rotated Array I & II, Find Min in Rotated Array, Single Element in Sorted Array, Peak Element, 2D Matrix Search, Matrix Median, Capacity to Ship Packages / Split Array Largest Sum, Aggressive Cows, Two Sum, Group Anagrams, Product Except Self, In-Place Duplicate Detection, Valid Sudoku, Container With Most Water, 3Sum, 4Sum, Shortest Unsorted Subarray, Time Based Key-Value Store. |
| [`heap_interval_template.cpp`](DSA-Code/heap_interval_template.cpp) | **Heaps, Priority Queues & Intervals** | K-th Largest/Smallest Elements, Top $K$ Frequent Elements & Words (LC 347 & 692), $K$ Closest Points (LC 973), Median from Data Stream (Two Heaps LC 295), Sliding Window Median (Two Multisets LC 480), IPO Capital Maximization (LC 502), Minimum Interval Querying (LC 1851), Merge $K$ Sorted Arrays/Lists, $K$ Pairs with Smallest Sums, Reorganize String (LC 767), Merge Intervals (LC 56), Insert Interval (LC 57), Meeting Rooms I & II (LC 252 & 253), Employee Free Time (LC 759), Min Arrows to Burst Balloons (LC 452). |
| [`recursion_template.cpp`](DSA-Code/recursion_template.cpp) | **Recursion & Advanced Backtracking** | Subsets / Power Set (LC 78), Subsets II with Duplicates (LC 90), Non-Decreasing Subsequences (LC 491), Combinations $N$ choose $K$ (LC 77), Combination Sum I, II & III (LC 39, 40, 216), Permutations I & II (LC 46 & 47), Next Permutation (LC 31), $K$-th Permutation Sequence (LC 60), Palindrome Partitioning (LC 131), Restore IP Addresses (LC 93), Word Break II, Rat in a Maze, Word Search 2D Grid (LC 79), $N$-Queens (LC 51), Sudoku Solver (LC 37). |
| [`string_template.cpp`](DSA-Code/string_template.cpp) | **String Algorithms & Pattern Matching** | Rabin-Karp Rolling Hash, Knuth-Morris-Pratt (KMP / LPS Array), Z-Algorithm / Z-Function, Longest Palindromic Substring (Manacher's & Expansion), String Hashing Utilities, Substring Search. |
| [`alphabet_trie.cpp`](DSA-Code/alphabet_trie.cpp) | **Prefix Tree / Alphabet Trie** | Standard Alphabet Trie with lowercase character nodes, Prefix Lookup, Word Insertion, Search, Prefix Count, Word Search II (Trie + 2D Backtracking). |
| [`binary_trie.cpp`](DSA-Code/binary_trie.cpp) | **Bitwise Binary Trie** | 32-bit Binary Trie for Fast XOR Operations, Maximum XOR of Two Numbers in an Array (LC 421), Maximum XOR with an Element from Array (Offline Queries LC 1707). |

---

### 🎯 3. System Design & Interview Playbooks

| File | Primary Focus | Key Contents |
|---|---|---|
| [`interview.cpp`](DSA-Code/interview.cpp) | **System Design Data Structures & Strategy** | **In-Memory System Design**: In-Memory File System (LC 588), Design Twitter (LC 355), Read $N$ Characters Given Read4 II (LC 158), Insert Delete GetRandom $O(1)$ (LC 380), All $O(1)$ Data Structure (LC 432), Design HashMap & HashSet, Concurrency / TTL Follow-up Playbook, and Live Interview Communication Strategy. |
| [`striver.md`](DSA-Code/striver.md) | **Curated Interview Sheets (3 Lists)** | **List 1**: Striver 79 Sheet (79 Problems)  <br>**List 2**: Blind 75 Sheet (75 Problems)  <br>**List 3**: Striver's SDE Sheet (191 Problems across 27 Categories) with direct practice links. |

---

## 🛠️ Verification & Compilation Standards

Every single file in this repository is built with **modern C++17 portability standards** (`#if __has_include(<bits/stdc++.h>)` with full standard library fallbacks) and verifies cleanly:

```bash
# Check syntax and compilation across all core files:
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-1.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-2.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-3.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-4.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-5.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-6.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-7.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-8.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-9.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-10.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/notebook-11.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/interview.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/stack_template.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/sliding_template.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/sorting_template.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/heap_interval_template.cpp
g++ -std=c++17 -fsyntax-only DSA-Code/recursion_template.cpp
```
