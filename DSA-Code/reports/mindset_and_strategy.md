# Google OA Mindset & Strategy Guide

Google OAs are designed to be challenging, but you have built templates for the exact logic blocks used to construct them. Use this guide to stay calm, reverse-engineer difficult questions, and optimize your scoring strategy.

---

## 1. How to De-construct "Out-of-the-Box" Problems

Google wraps standard algorithmic patterns in complex real-world stories (networks, signals, boards, game play). If the solution isn't obvious, use this system to reverse-engineer it:

### Step 1: Look at the Constraints First (They dictate the algorithm)
*   **N <= 20**: The solution is exponential. Look for:
    *   **Bitmask DP** (e.g., choosing subsets, TSP variants)
    *   **Backtracking / Permutations / Subsets**
*   **N <= 40**: Look for:
    *   **Meet-in-the-Middle** (split search space to 2^(N/2))
*   **N <= 10^5 or 10^6**: The solution must be O(N) or O(N log N). Look for:
    *   **Monotonic Stack / Queue** (if looking for subarray boundaries, mins, maxes)
    *   **Binary Search on Answer** (if the predicate function has a monotonic true/false transition, usually signaled by "minimize the maximum" or "maximize the minimum")
    *   **Topological Sort / DSU / Dijkstra / SCC** (if graph dependencies or components are involved)
*   **N <= 10^18**: The solution must be logarithmic O(log N). Look for:
    *   **Digit DP** (counting numbers satisfying digit constraints in a range)
    *   **Matrix Exponentiation**

### Step 2: Ask the Universal Framework Questions
*   **For Graph problems:**
    *   *What represent the nodes? (Cells, states, masks, pairs)*
    *   *What are the edges? Are they weighted? (If weighted, BFS is not enough; use Dijkstra)*
    *   *Do I need to carry "state" variables (like K skips remaining) in my BFS/DFS?*
*   **For DP problems:**
    *   *What is the state? (index, current sum, mask of visited nodes, digit bounds)*
    *   *What is the recurrence relation? How does current state depend on subproblems?*

---

## 2. Game-Day Execution Strategy

### 1. Secure Partial Points First (Subtasks)
Google OAs score you based on how many test cases you pass.
*   If the optimal O(N log N) solution isn't clear, write a naive O(N^2) or backtracking solution first.
*   Submit it to get partial credits for the smaller constraints.
*   Once you have secured these points, duplicate the code and optimize it. **Never leave a blank page.**

### 2. Time Management
*   **Spend the first 5 minutes reading both problems.** Q2 can sometimes be easier than Q1 depending on your strengths.
*   If you get stuck on a problem for more than 15-20 minutes without making progress, switch to the other one. Don't sink the entire time into a single problem.

### 3. Avoid Common Coding Gotchas
*   **Reset Member Variables:** Since the OA uses LeetCode-style wrapper classes, you MUST re-initialize/clear all class-member variables and global vectors inside the main entry function before executing any logic.
*   **Use `long long` Generously:** If intermediate calculations or sums can exceed 2 * 10^9, cast to `long long` immediately using `1LL * a * b` to prevent overflow.
*   **Avoid `std::endl`:** Use `\n` to prevent repeatedly flushing the output buffer and causing TLE on large outputs.

---

Trust your preparation, leverage your cheat sheets, and focus on securing points methodically. Good luck!
