# DE Shaw OA: 1-Day Preparation & Game-Day Strategy (Coding Only)

Tomorrow is your DE Shaw Online Assessment (OA). Since there are **no MCQ sections this time** and the test consists **entirely of coding questions**, the bar for the coding section is exceptionally high. 

This document details the updated **1-day prep plan** and **game-day execution strategy** to maximize your score on a coding-only assessment.

---

## 1. D.E. Shaw OA Coding Focus Areas

With a coding-only format, the assessment typically consists of **2–3 coding questions** ranging from LeetCode Medium to very hard Codeforces problems (up to 2000–2400 rating).

### Core Coding Themes
DE Shaw coding questions are often "fancy" word problems wrapping advanced algorithms:
1. **Dynamic Programming (DP):**
   * *Range/Interval DP* (e.g., merging elements, matrix-chain style).
   * *Bitmask DP* (e.g., traveling salesman, matching/subset assignment).
   * *Digit DP* (e.g., numbers satisfying a condition in range $[L, R]$).
   * *Subsequence DP* (LIS, LCS variations with optimizations).
2. **Advanced Trees & Graphs:**
   * *Euler Tour / Tree Flattening* (for subtree and path query/updates).
   * *Lowest Common Ancestor (LCA)* using binary lifting or RMQ.
   * *Centroid Decomposition* or *Heavy-Light Decomposition (HLD)* (rare but possible for CF 2400 questions).
   * *Shortest Path* (Dijkstra, Bellman-Ford/negative cycle detection).
3. **Advanced Range Queries:**
   * *Segment Trees* with Lazy Propagation (range updates, custom node merges).
   * *Fenwick Trees (BIT)* with coordinate compression or binary lifting.
4. **Mathematics & Bit Manipulation:**
   * *Modular arithmetic / Combinatorics* ($nCr$, modular inverse).
   * *Sum Over Subsets (SOS) DP* or *XOR Basis* (linear basis for XOR maximization).
   * *Slope Trick / Priority Queue* (e.g., making array non-decreasing).

---

## 2. Updated 1-Day Strategy: Re-allocating MCQ Study Time

Since you do not need to study core CS fundamentals, **re-allocate those 2 hours** to deepen your coding template adaptation and dry-running core CSES solutions.

### Phase 1: Notebook Template Review & Adaptability (4 Hours)
Since you already have templates in your `DSA` directory (`notebook-1.cpp` to `notebook-4.cpp`), you must ensure you can adapt them instantly under pressure.
* **Segment Tree (`notebook-1.cpp` L292):** Re-familiarize yourself with the child node merging logic. Make sure you can write range-min, range-max, range-gcd, and lazy propagation on the fly.
* **Fenwick Tree (`notebook-1.cpp` L458):** Review how to run binary lifting on a BIT in $O(\log N)$ (e.g., finding the $k$-th active element) and coordinate compression (`CoordinateCompressor` L361).
* **Euler Tour (`notebook-1.cpp` L151):** Review how Tree Path flattening (Size $2N$) and Subtree flattening (Size $N$) work.
* **Digit DP & SOS DP (`notebook-2.cpp` L47 & L621):** Review the memoization state representation and how to transition bits.
* **Graph Cycles (`notebook-3.cpp` L58 & L173):** Review cycle reconstruction for directed/undirected graphs and negative weight cycle detection (Bellman-Ford).
* **Array Tricks (`notebook-4.cpp` L138 & L283):** Review the median-cost sliding window (`minMoves`) and the slope trick priority-queue approach (`IncreasingArray2`).

### Phase 2: Curated High-Yield CSES Problem Walkthroughs (4 Hours)
Read through your solutions for these specific problems. Focus on the core transitions and representation:
1. **Range Queries / Data Structures:**
   * *Hotel Queries* (Binary Search on Segment Tree to find the first position).
   * *Subarray Sum Queries* (Segment Tree node merging logic).
   * *Range Update Queries* (Difference array using BIT or Lazy SegTree).
   * *Nested Ranges Count* (Coordinate compression + sorting + BIT traversal).
2. **Trees & Graphs:**
   * *Distance Queries / Company Queries II* (LCA binary lifting).
   * *Subtree Queries & Path Queries* (Euler tour mappings to Segment Tree).
   * *Cycle Finding* (Reconstructing negative cycles).
   * *Flight Routes* (Dijkstra using a min-heap to find the $k$-th shortest path).
3. **Dynamic Programming:**
   * *Elevator Rides* (Bitmask DP tracking `{rides, weight}`).
   * *Projects* (Sorting + Coordinate Compression + Binary Search/Segment Tree + DP).
   * *Array Description* (Grid-based DP transitions).
   * *Increasing Subsequence* (Classic LIS with binary search).
   * *Bit Problem* (SOS DP / Sum Over Subsets).
   * *Exponentiation II* ($a^{b^c} \pmod p$ using Fermat's Little Theorem).

---

## 3. Game-Day Execution Strategy: Passing Partial Test Cases

In a difficult assessment (CF 2000-2400), few candidates pass 100% of the test cases on all problems. **Passing subtasks is your path to selection.**

### The Subtask / Partial Point Pipeline
1. **Identify the constraints immediately:**
   * If $N \le 1000$ or $N \le 2000$, an $O(N^2)$ solution is sufficient for partial points.
   * If $N \le 20$, backtracking $O(2^N)$ or $O(N!)$ will pass.
2. **Write the brute force first (if the optimal solution isn't obvious):**
   * Spend no more than 15 minutes attempting to design the optimal $O(N \log N)$ or $O(N)$ solution.
   * If it doesn't click, **stop** and write the $O(N^2)$ or $O(N^3)$ brute-force solution immediately.
   * Submit it. Ensure you get the 30% to 50% partial points. This locks in your baseline score.
3. **Optimize in a separate copy:**
   * Once partial points are secured, duplicate your code or work in a separate function to optimize the transitions, add segment trees, or apply coordinate compression.
   * If you run out of time or introduce bugs during optimization, your partial points are already safe in the system.

### Platform-Specific Reminders (HackerRank)
* **Non-Revisit Constraints:** Many DE Shaw OAs prevent you from returning to previous questions. If you submit a question, you may not be able to return to it. Allocate your time carefully.
* **Fast I/O & Types:**
  * Add `cin.tie(NULL); ios_base::sync_with_stdio(false);` at the top of your function.
  * Replace `endl` with `\n` to prevent TLE.
  * Use `long long` for all intermediate math calculations: `long long val = 1LL * a * b;`.
  * For wrapper classes, if you use global/static variables, **re-initialize them** inside the entry function since platforms run multiple test cases on the same object.

---

## 4. Final Evening Timeline (Sunday Night)

```mermaid
gantt
    title Sunday Night Revision Timeline
    dateFormat  HH:mm
    axisFormat %H:%M
    section Study
    Phase 1: Revise Notebook Templates     :active, 18:30, 22:30
    Phase 2: CSES Solved Problem Walkthrough : 22:30, 23:59
    section Rest
    Sleep (Ensure 7-8 hours)                : 00:00, 08:00
```

* **Prioritize sleep:** Solving complex algorithmic problems requires high mental acuity. A well-rested brain will spot DP transitions and subtask patterns far better than a sleep-deprived one.

Best of luck! You have built high-quality templates and solved a significant portion of CSES; trust your preparation and play for points systematically.
