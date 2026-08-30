# CSES Problem Solutions — Problem Summary & Complexity Reference

A structured reference catalog of all **70 solved CSES problems** in `DSA-Code/Cses-Soln`. Every entry includes standard problem names, difficulty tier ratings (**T 1**, **T 2**, **T 3**), algorithmic patterns, optimal time complexities, and auxiliary space complexities categorized by domain.

## Category-by-Category Analysis

### 1. Introductory Problems

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Trailing Zeros | `T 1` | Legendre's Formula for Prime Factor 5. | `O(log_5 N)` | `O(1)` |
| **2** | Palindrome Reorder | `T 1` | Frequency Counting + Symmetric Half Mirroring. | `O(N)` | `O(N)` |
| **3** | Gray Code | `T 2` | Binary-to-Gray Code Formula G(i) = i \oplus (i \gg 1). | `O(2^N * N)` | `O(1)` |
| **4** | Apple Division | `T 1` | Bitmask Subset Sum Enumeration (2^N). | `O(N * 2^N)` | `O(N)` |
| **5** | Digit Queries | `T 2` | Bucket Range Decomposition + Arithmetic Offset. | `O(log_{10} K)` | `O(1)` |

---

### 2. Sorting and Searching

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Concert Tickets | `T 2` | Multiset Upper Bound Search. | `O((H + M) log H)` | `O(H)` |
| **2** | Collecting Numbers | `T 1` | Inverse Index Inversions. | `O(N)` | `O(N)` |
| **3** | Collecting Numbers II | `T 2` | Local Inversion Delta Update. | `O(N + M)` | `O(N)` |
| **4** | Towers | `T 2` | `std::multiset` Upper Bound Greedy (Patience Sorting). | `O(N log N)` | `O(N)` |
| **5** | Traffic Lights | `T 2` | `std::set` Position Maintenance + `std::multiset` Segment Lengths. | `O(N log N)` | `O(N)` |
| **6** | Josephus Problem II | `T 2` | Order Statistic Tree (Policy-Based Data Structure `pbds`) / Fenwick Binary Lifting. | `O(N log N)` | `O(N)` |
| **7** | Nested Ranges Check | `T 2` | Interval Sorting + Min/Max Sweeping. | `O(N log N)` | `O(N)` |
| **8** | Nested Ranges Count | `T 2` | Coordinate Compression + Fenwick Tree on Sorted Intervals. | `O(N log N)` | `O(N)` |
| **9** | Room Allocation | `T 2` | Min-Heap Event Processing (Departure Tracking). | `O(N log N)` | `O(N)` |
| **10** | Reading Books | `T 2` | Bottleneck Max Element vs Sum. | `O(N)` | `O(1)` |
| **11** | Subarray Divisibility | `T 2` | Prefix Remainder Frequency Map modulo N. | `O(N)` | `O(N)` |
| **12** | Distinct Values Subarrays | `T 2` | Sliding Window Frequency Map. | `O(N)` | `O(K)` |
| **13** | Distinct Values Subarrays II | `T 2` | Complementary Counting (Total Subarrays - Subarrays with < K distinct). | `O(N)` | `O(K)` |
| **14** | Distinct Values Subsequences | `T 2` | Element Frequency Product \prod (freq[x] + 1) - 1. | `O(N)` | `O(N)` |
| **15** | Movie Festival II | `T 3` | Greedy Sort by End Time + `std::multiset` Upper Bound. | `O(N log K)` | `O(K)` |
| **16** | Maximum Subarray Sum II | `T 2` | Prefix Sum + Sliding Window Monotonic Deque / `std::multiset`. | `O(N log(B - A))` | `O(B - A)` |

---

### 3. Range Queries

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Static Range Sum Queries | `T 1` | 1D Prefix Sum Array. | `O(N + Q)` | `O(N)` |
| **2** | Static Range Minimum Queries | `T 2` | Sparse Table (\mathcal{O}(1) Overlapping Range Query). | `O(N)` | `O(N)` |
| **3** | Dynamic Range Sum Queries | `T 2` | Fenwick Tree (BIT) / Segment Tree. | `O(N)` | `O(N)` |
| **4** | Dynamic Range Minimum Queries | `T 2` | Segment Tree (Point Update, Range Min Query). | `O((N + Q) log N)` | `O(N)` |
| **5** | Range Update Queries | `T 2` | Fenwick Tree on Difference Array. | `O((N + Q) log N)` | `O(N)` |
| **6** | Range Xor Queries | `T 1` | 1D Prefix XOR Array. | `O(N + Q)` | `O(N)` |
| **7** | Forest Queries | `T 2` | 2D Prefix Sum Array (Inclusion-Exclusion). | `O(N^2 + Q)` | `O(N^2)` |
| **8** | Hotel Queries | `T 2` | Segment Tree Range Max + Binary Walk on Tree. | `O((N + M) log N)` | `O(N)` |
| **9** | List Removals | `T 2` | Fenwick Tree Binary Lifting (Find k-th Active Element). | `O(N log N)` | `O(N)` |
| **10** | Salary Queries | `T 2` | Coordinate Compression + Fenwick Tree. | `O((N + Q) log(N + Q))` | `O(N + Q)` |
| **11** | Prefix Sum Queries | `T 2` | Segment Tree with Augmented Node `(sum, max_prefix)`. | `O((N + Q) log N)` | `O(N)` |
| **12** | Subarray Sum Queries | `T 3` | Segment Tree Kadane's Node `(sum, pref, suff, max_ans)`. | `O((N + Q) log N)` | `O(N)` |
| **13** | Subarray Sum Queries II | `T 3` | Segment Tree Kadane's Range Query. | `O((N + Q) log N)` | `O(N)` |
| **14** | Distinct Values Queries | `T 3` | Offline Queries + Fenwick Tree + Last Occurrence Sweep. | `O((N + Q) log N)` | `O(N + Q)` |

---

### 4. Advanced Techniques

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Hamming Distance | `T 2` | Bitmask XOR + Fast Hardware Popcount. | `O(N^2)` | `O(N)` |
| **2** | Meet in the Middle | `T 2` | Meet-in-the-Middle (2^{N/2} Search Space Reduction). | `O(2^{N/2} * N)` | `O(2^{N/2})` |
| **3** | Corner Subgrid Check | `T 2` | Column-Pair Hash Map / Boolean Matrix Sweep. | `O(K * N^2)` | `O(N^2)` |
| **4** | Corner Subgrid Count | `T 2` | Bitset Row AND + Combinatorial \binom{C}{2}. | `O(N^3 / 64)` | `O(N^2 / 8)` |
| **5** | Eulerian Subgraphs | `T 3` | Linear Algebra over GF(2) / Graph Connected Components. | `O(V + E)` | `O(V + E)` |
| **6** | Necessary Cities | `T 3` | Tarjan's Articulation Points Algorithm via DFS. | `O(V + E)` | `O(V + E)` |
| **7** | Necessary Roads | `T 3` | Tarjan's Bridge-Finding Algorithm via DFS. | `O(V + E)` | `O(V + E)` |
| **8** | New Roads Queries | `T 3` | Parallel Binary Search + Disjoint Set Union (DSU). | `O((M + Q) log M)` | `O(N + M + Q)` |
| **9** | Reachability Queries | `T 3` | Kosaraju SCC Condensation + Bitset Reachability on DAG. | `O(V + E + C^2 / 64 + Q)` | `O(C^2 / 8)` |
| **10** | Reachable Nodes | `T 3` | Topological Sort + Bitset DP Propagation. | `O((V + E) * V / 64)` | `O(V^2 / 8)` |
| **11** | Substring Reversals | `T 3` | Implicit Treap with Lazy Reversal Propagation. | `O((N + M) log N)` | `O(N)` |
| **12** | Reversals and Sums | `T 3` | Implicit Treap with Lazy Reversal + Augmented Subtree Sum. | `O((N + M) log N)` | `O(N)` |
| **13** | Cut and Paste | `T 3` | Implicit Treap Split and Merge Reordering. | `O((N + M) log N)` | `O(N)` |

---

### 5. Additional Problems I

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Shortest Subsequence | `T 2` | Greedy Character-Set Coverage (Alphabet Phase Completion). | `O(N)` | `O(1) auxiliary` |
| **2** | Advertisement | `T 2` | Monotonic Stack (Largest Rectangle in Histogram). | `O(N)` | `O(N)` |
| **3** | Bubble Sort Rounds I | `T 2` | Maximum Displacement of Inversions. | `O(N log N)` | `O(N)` |
| **4** | Multiplication Table | `T 2` | Binary Search on Answer / Counting Multiples. | `O(N log(N^2))` | `O(1)` |
| **5** | Bit Inversions | `T 3` | Segment Tree with Subsegment Merging / `std::set` Boundary Maintenance. | `O((N + M) log N)` | `O(N)` |
| **6** | Distinct Values Sum | `T 3` | Contribution Technique / Last Occurrence Indexing. | `O(N)` | `O(N)` |
| **7** | Swap Game | `T 2` | BFS State-Space Search / Permutation Hash. | `O(9!) states` | `O(9!)` |
| **8** | Maximum Building I | `T 2` | 2D Grid Monotonic Stack (Histogram Expansion per Row). | `O(N * M)` | `O(M)` |
| **9** | Sorting Methods | `T 2` | Permutation Inversions, Cycle Decomposition, LIS, LNDS. | `O(N log N)` | `O(N)` |
| **10** | Cyclic Array | `T 3` | Binary Lifting on Functional Jump Graph (Doubling). | `O(N log N)` | `O(N log N)` |
| **11** | Special Substrings | `T 3` | Difference Relative Frequency Map / Prefix Hash. | `O(N * Sigma)` | `O(N * Sigma) where Sigma is alphabet size` |
| **12** | Writing Numbers | `T 2` | Binary Search on Answer + Digit DP / Digit Counting. | `O(log(10^{18}) * log_{10}(10^{18}))` | `O(1)` |
| **13** | Permutation Subsequence | `T 2` | Index-Map Dynamic Programming. | `O(N)` | `O(N)` |
| **14** | Subarray Sum Constraints | `T 3` | Prefix Sum Graph / Constraint Graph 2-Coloring. | `O(N + M)` | `O(N + M)` |
| **15** | Subsets with Fixed Average | `T 3` | Target Shifted Subset Sum DP. | `O(N * MaxSum)` | `O(MaxSum)` |
| **16** | Two Array Average | `T 3` | Binary Search on Ratio (Dinkelbach's Algorithm). | `O(N log N * log(precision))` | `O(N)` |

---

### 6. Additional Problems II

| # | Problem Name | T | Pattern / Technique | Time Complexity | Space Complexity |
| :-: | :--- | :-: | :--- | :--- | :--- |
| **1** | Book Shop II | `T 2` | Bounded Knapsack with Binary Decomposition. | `O(X * \sum log k_i)` | `O(X)` |
| **2** | Coin Grid | `T 3` | Minimum Vertex Cover in Bipartite Graph (König's Theorem). | `O(V * E) = O(N^3)` | `O(N^2)` |
| **3** | GCD Subsets | `T 3` | Inclusion-Exclusion on Multiple Counts / Mobius Inversion. | `O(MAX log MAX + N)` | `O(MAX)` |
| **4** | Increasing Array II | `T 3` | Slope Trick / Priority Queue Greedy. | `O(N log N)` | `O(N)` |
| **5** | School Excursion | `T 2` | DSU Component Sizes + Bitset 0/1 Subset Sum Knapsack. | `O(N + N^2 / 64)` | `O(N / 64)` |
| **6** | Stick Divisions | `T 2` | Reverse Huffman Coding / Min-Heap Greedy. | `O(N log N)` | `O(N)` |
