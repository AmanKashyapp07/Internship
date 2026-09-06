# Microsoft Online Assessment (OA) Prep Strategy — Summer Internship Track

A focused, high-yield playbook for cracking the Microsoft Online Assessment (Codility / HackerRank) ahead of a summer internship offer, prioritizing recency-weighted questions, core algorithmic archetypes, and platform-specific grading behavior.

**Internship-specific note:** Intern OAs tend to skew slightly easier than full-time OAs — expect 2 problems in 60–90 minutes, weighted toward Easy/Medium, with Hards appearing less often than in the full dataset below. Don't over-invest in Hard-tier prep at the expense of nailing Easy/Medium correctness and edge cases.

---

## 0. If Your OA Is Imminent (Read This First)

If you have limited days left, do these in order — each step is a complete checkpoint, so stop wherever your time runs out:

1. **Today:** Skim Section 4 (the 5 archetypes) once, out loud if possible. This is pattern recognition, not memorization — 20 minutes.
2. **Next 1–2 days:** Solve as many of the 49 Tier 1 problems (Section 3) as you can, in order of confidence — weakest pattern first while your energy is highest.
3. **Day before:** One timed mock — 2 unseen Medium problems, 60 minutes, no autocomplete. Simulate real conditions (Section 7, Option A).
4. **Morning of:** Re-read Section 6 (scoring traps) and Section 8 (day-of checklist). These cost you nothing and prevent silent point loss.

If you have a full week or more, skip to Section 7, Option B.

---

## 1. The High-ROI Rule

Attempting to grind all **1,386 problems** in `csv/all.csv` yields diminishing returns and high burnout, especially on a compressed internship-prep timeline.

Microsoft OA question pools exhibit distinct, exploitable properties:

1. **Heavy recency weighting** — questions cluster around recent hiring windows (last 30–90 days).
2. **Platform constraints (Codility/HackerRank)** — 2–3 problems in 60–90 minutes, graded automatically against hidden tests for both **correctness** (edge cases) and **performance** (time/space complexity).
3. **Archetype bias** — heavy prevalence of Greedy, Interval operations, Two Pointers/Sliding Window, Prefix Sums/Hash Maps, and Multi-Source BFS. Hard Dynamic Programming, Bitmask DP, and advanced graph algorithms (SCC, Min-Cut, Segment Trees) are virtually absent.

---

## 2. Dataset Overview & Funnel

| Priority Tier | Dataset | Total | Easy | Medium | Hard | Strategy |
|---|---|---:|---:|---:|---:|---|
| **Tier 1 — Mandatory** | `csv/thirty-days.csv` | 49 | 19 | 27 | 3 | 100% completion target. Covers the active question pool. |
| **Tier 2 — Expansion** | `csv/three-months.csv` | 166 | 55 | 85 | 26 | Solve the Medium delta (+117 problems) if time allows; skip trivial Easies. |
| **Tier 3 — Safety Net** | `csv/six-months.csv` | 421 | 130 | 228 | 63 | Reference only, for drilling a specific weak pattern. |
| **Ignore for OA prep** | `csv/all.csv` | 1,386 | 392 | 764 | 230 | Low ROI on a timed prep schedule. |

---

## 3. Tier 1: The 49 Core Problems

Master these first — they represent the active Microsoft question pool. *(For the complete combined checklist of all 166 problems across Tier 1 + Tier 2, see [`consolidated_problems.md`](file:///Users/amankashyap/Documents/internship/DSA-Code/Microsoft/consolidated_problems.md)).*

### A. Arrays, Prefix Sums & Hashing
- [1. Two Sum](https://leetcode.com/problems/two-sum) *(Easy)*
- [128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence) *(Medium)*
- [169. Majority Element](https://leetcode.com/problems/majority-element) *(Easy)*
- [242. Valid Anagram](https://leetcode.com/problems/valid-anagram) *(Easy)*
- [451. Sort Characters By Frequency](https://leetcode.com/problems/sort-characters-by-frequency) *(Medium)*
- [645. Set Mismatch](https://leetcode.com/problems/set-mismatch) *(Easy)*
- [1929. Concatenation of Array](https://leetcode.com/problems/concatenation-of-array) *(Easy)*
- [3737. Count Subarrays With Majority Element I](https://leetcode.com/problems/count-subarrays-with-majority-element-i) *(Medium)*

### B. Two Pointers & Sliding Window
- [3. Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters) *(Medium)*
- [11. Container With Most Water](https://leetcode.com/problems/container-with-most-water) *(Medium)*
- [15. 3Sum](https://leetcode.com/problems/3sum) *(Medium)*
- [344. Reverse String](https://leetcode.com/problems/reverse-string) *(Easy)*
- [438. Find All Anagrams in a String](https://leetcode.com/problems/find-all-anagrams-in-a-string) *(Medium)*
- [567. Permutation in String](https://leetcode.com/problems/permutation-in-string) *(Medium)*
- [239. Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum) *(Hard)*

### C. Greedy, Sorting & Intervals (Codility Signature)
- [31. Next Permutation](https://leetcode.com/problems/next-permutation) *(Medium)*
- [56. Merge Intervals](https://leetcode.com/problems/merge-intervals) *(Medium)*
- [134. Gas Station](https://leetcode.com/problems/gas-station) *(Medium)*
- [763. Partition Labels](https://leetcode.com/problems/partition-labels) *(Medium)*
- [1288. Remove Covered Intervals](https://leetcode.com/problems/remove-covered-intervals) *(Medium)*

### D. Matrices & Grids
- [48. Rotate Image](https://leetcode.com/problems/rotate-image) *(Medium)*
- [73. Set Matrix Zeroes](https://leetcode.com/problems/set-matrix-zeroes) *(Medium)*
- [74. Search a 2D Matrix](https://leetcode.com/problems/search-a-2d-matrix) *(Medium)*
- [994. Rotting Oranges](https://leetcode.com/problems/rotting-oranges) *(Medium)*

### E. Strings & Parsing
- [5. Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring) *(Medium)*
- [13. Roman to Integer](https://leetcode.com/problems/roman-to-integer) *(Easy)*
- [14. Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix) *(Easy)*
- [28. Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string) *(Easy)*
- [205. Isomorphic Strings](https://leetcode.com/problems/isomorphic-strings) *(Easy)*
- [1768. Merge Strings Alternately](https://leetcode.com/problems/merge-strings-alternately) *(Easy)*

### F. Linked Lists & Fast/Slow Pointers
- [2. Add Two Numbers](https://leetcode.com/problems/add-two-numbers) *(Medium)*
- [21. Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists) *(Easy)*
- [25. Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group) *(Hard)*
- [92. Reverse Linked List II](https://leetcode.com/problems/reverse-linked-list-ii) *(Medium)*
- [141. Linked List Cycle](https://leetcode.com/problems/linked-list-cycle) *(Easy)*
- [146. LRU Cache](https://leetcode.com/problems/lru-cache) *(Medium)*

### G. Binary Search & Binary Search on Answer
- [33. Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array) *(Medium)*
- [1011. Capacity To Ship Packages Within D Days](https://leetcode.com/problems/capacity-to-ship-packages-within-d-days) *(Medium)*

### H. Trees, Graphs & BFS/DFS
- [110. Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree) *(Easy)*
- [230. Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst) *(Medium)*
- [543. Diameter of Binary Tree](https://leetcode.com/problems/diameter-of-binary-tree) *(Easy)*
- [3532. Path Existence Queries in a Graph I](https://leetcode.com/problems/path-existence-queries-in-a-graph-i) *(Medium)*

### I. Dynamic Programming & Math/Bit Manipulation
- [42. Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water) *(Hard)*
- [53. Maximum Subarray](https://leetcode.com/problems/maximum-subarray) *(Medium)*
- [62. Unique Paths](https://leetcode.com/problems/unique-paths) *(Medium)*
- [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle) *(Easy)*
- [121. Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock) *(Easy)*
- [136. Single Number](https://leetcode.com/problems/single-number) *(Easy)*
- [509. Fibonacci Number](https://leetcode.com/problems/fibonacci-number) *(Easy)*
- [779. K-th Symbol in Grammar](https://leetcode.com/problems/k-th-symbol-in-grammar) *(Medium)*

---

## 4. Top 5 High-Yield Archetypes

### Archetype 1: Greedy Intervals & Scheduling
- **Trigger:** overlapping time intervals, minimum rooms, remove covered intervals, merge ranges.
- **Rule of thumb:** sort by `start_time` when merging/expanding intervals; sort by `end_time` when greedily scheduling non-overlapping activities. Track the maximum boundary reached dynamically.
- **Invariant:** for `[s1, e1]` and `[s2, e2]`, if `s2 <= e1`, merge into `[s1, max(e1, e2)]`.

### Archetype 2: Sliding Window (Match Counter & Last-Seen Index)
- **Trigger:** substrings with conditions — at most *k* distinct characters, no duplicates, anagram permutations.
- **Rule of thumb:** never re-compare whole frequency tables each iteration. Track a running `matches` counter, or store `last_seen[char]` to jump the left pointer directly instead of stepping it one at a time.

### Archetype 3: Prefix Sum with Modulo / Frequency Map
- **Trigger:** "subarray sum equals K", "subarray sum divisible by K", negative integers present.
- **Rule of thumb:** plain two pointers fails once negatives are allowed. Key invariant: `sum(i..j) = prefix[j] - prefix[i-1] = K` implies `prefix[i-1] = prefix[j] - K`. Always initialize `prefixCount[0] = 1` to capture prefixes starting at index 0.

### Archetype 4: Multi-Source Grid BFS
- **Trigger:** simultaneous spread (Rotting Oranges, 01 Matrix, shortest time to reach all nodes).
- **Rule of thumb:** push all source cells at `t = 0` into the queue together, then increment time level-by-level by snapshotting `queue.size()` at the start of each level. Mutate the grid in place instead of maintaining a separate `visited[][]` array.

### Archetype 5: Binary Search on Answer
- **Trigger:** find the minimum capacity/speed/threshold satisfying a condition, where the search space is monotonic: `[false, false, ..., true, true]`.
- **Rule of thumb:** identify bounds `[low, high]` (e.g. `low = max(weights)`, `high = sum(weights)`), then write a standalone `canFulfill(mid)` predicate and binary-search over it.

---

## 5. Low-ROI Traps to Avoid

Don't spend prep hours on:

1. **Complex 2D/3D or bitmask DP** (e.g. TSP, digit DP) — Microsoft OAs almost exclusively test 1D DP, Kadane's, and simple grid DP.
2. **Niche graph algorithms** — Tarjan's SCC, Bellman-Ford, Edmonds-Karp, heavy tree decompositions.
3. **Advanced data structures** — Fenwick/segment trees, skip lists, treaps. Standard library containers (`vector`, `unordered_map`, `priority_queue`, `deque`) are sufficient.
4. **Solving all 1,386 questions** — returns diminish sharply after the first 150–200 high-frequency problems.

---

## 6. Codility / OA Scoring Traps

Codility grades automatically against hidden test suites. Correct intuition can still lose points if you miss these:

1. **Integer overflow** — a constraint like N ≤ 10⁵ can still let a cumulative sum or product exceed 2³¹−1. Default cumulative sums/products to 64-bit integers (`long long` in C++).
2. **Quadratic complexity degradation** — Codility splits scoring into Correctness (50%) and Performance (50%). An O(N²) solution where O(N log N) or O(N) is expected scores 0% on performance, capping the result near 50% even if logically correct.
3. **Unchecked edge cases:**
   - empty input, or N = 0, 1, 2
   - all elements identical (e.g. `[5, 5, 5, 5]`)
   - all-negative arrays (Kadane's initialized to `0` instead of `nums[0]` breaks here)
   - already-sorted (ascending or descending) input
   - `INT_MIN` / `INT_MAX` boundary values

---

## 7. Recommended Prep Schedules

### Option A: 3-Day Intensive Sprint (OA in under 72 hours)
- **Day 1:** Solve all 49 problems in `csv/thirty-days.csv`. Reference `report.md` for quick algorithmic templates.
- **Day 2:** Review the 20 core patterns in `final.md`; drill interval/greedy problems in `greedy.md`.
- **Day 3:** Timed mock — pick 3 Medium problems from `csv/three-months.csv`, solve in 60 minutes, no autocomplete.

### Option B: 7-Day Structured Sprint
- **Days 1–3:** Work through the 49 Tier 1 problems (~16/day).
- **Days 4–5:** Solve the top Medium delta from `csv/three-months.csv`, focused on Intervals, Sliding Window, and Grids.
- **Day 6:** Study pattern recognition in `final.md`; review communication and edge-case tips in `tip.md`.
- **Day 7:** Full mock test in Codility/HackerRank format, explicitly validating boundary conditions.

---

## 8. Day-of Checklist

- Read the constraints twice before coding — they tell you the required time complexity.
- Write the brute-force approach mentally first, then identify which archetype (Section 4) upgrades it.
- Handle edge cases (Section 6, item 3) before optimizing further — hidden correctness tests usually target these first.
- Submit a working, if suboptimal, solution before attempting to optimize further — partial credit exists on Codility, zero credit does not.
- If stuck for more than 10–15 minutes, restate the problem in your own words or try a smaller manual example — this alone frequently surfaces the right pattern.

---

## 9. Repository Quick Links

- Consolidated Tier 1 + Tier 2 Master List: `consolidated_problems.md`
- 30-Day Question Pool: `csv/thirty-days.csv`
- 3-Month Question Pool: `csv/three-months.csv`
- Algorithmic Solutions & Complexities: `report.md`
- Core 20 Recognition Invariants: `final.md`
- Greedy & Interval Templates: `greedy.md`
- Interview Tactics & Edge Cases: `tip.md`
- 150 Master List & Solution Maps: `150_PROBLEMS_MASTER_LIST.md`