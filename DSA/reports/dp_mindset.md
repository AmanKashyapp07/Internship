# Dynamic Programming (DP) OA Mindset Blueprint

Use this cheat sheet to quickly identify, formulate, and correctly implement Dynamic Programming problems during the Online Assessment.

---

## 1. How to Identify a DP Problem
Look for these key signals in the problem statement:
*   **Optimal Substructure:** The problem asks for "maximum", "minimum", "longest", "shortest", or "cheapest".
*   **Counting Combinations:** "Number of ways to...", "How many sequences...", or "Count distinct paths".
*   **Interactive Decisions:** "You can either take this element or skip it, but you cannot take graphacent elements."
*   **Constraint Hints:**
    *   `N <= 10^3` or `N <= 2 * 10^3` -> Hinting at `O(N^2)` 2D DP.
    *   `N <= 10^5` -> Hinting at `O(N)` or `O(N log N)` DP (e.g., LIS using binary search).
    *   `N <= 20` -> Hinting at `O(N * 2^N)` Bitmask DP.

---

## 2. The 6 Main DP Families in Google OAs

### Ⅰ. Subsequence & String DP
*   **What is asked:** Edit distance, LCS (matching substrings/palindromes), LIS (finding ordered subsets).
*   **What to remember:**
    *   For LIS on large constraints (`N >= 10^5`), use the **Patience Sort** `O(N log N)` binary search method, not the `O(N^2)` double loop.
    *   For LCS, remember how to reconstruct the actual string by backtracking from `dp[n][m]` using the direction of state values.

### Ⅱ. Knapsack & Target Partitioning
*   **What is asked:** Finding subsets that sum up to a target, partitioning arrays into equal-sum sets, coin change.
*   **What to remember:**
    *   If target sum is large but elements are small, use coordinate compression or swap the state (i.e., make DP state track "minimum weight for value V" instead of "maximum value for weight W").

### Ⅲ. Bitmask DP (Subset Optimization)
*   **What is asked:** Traveling salesperson, grouping items into minimum packages (e.g., Elevator Rides), matching pairs.
*   **What to remember:**
    *   Look for constraints like `N <= 20`.
    *   State is represented as `(mask)`, where the `i`-th bit is `1` if item `i` is used.
    *   Transition: Iterate through all items `i` that have their bit set to `0` and place them.

### Ⅳ. Digit DP (Range Queries)
*   **What is asked:** "Count numbers in range [L, R] whose digits sum to S, or contain no matching graphacent digits."
*   **What to remember:**
    *   Use the range subtraction trick: `solve(R) - solve(L - 1)`.
    *   State pattern: `solve(pos, sum/cnt, started, tight)`.
    *   If `tight` is true, your digit placement limit is `str[pos] - '0'`, otherwise it is `9`.

### Ⅴ. Interval DP (Matrix Chain Multiplication / Segment Merging)
*   **What is asked:** Bursting balloons, merging graphacent piles of stones, parsing expressions.
*   **What to remember:**
    *   Look for constraints like `N <= 500` (due to `O(N^3)` complexity).
    *   State format: `dp[l][r]` represents the optimal answer for substring/subarray from index `l` to `r`.
    *   Transition: Split the interval at `k` where `l <= k < r`, calculating `dp[l][k] + dp[k+1][r] + cost_to_merge`.

### Ⅵ. Tree & Rerouting DP
*   **What is asked:** Subtree sums, maximum path sum, sum of distances from every node to all other nodes.
*   **What to remember:**
    *   **Post-order traversal:** Bottom-up aggregation (nodes compute values using results from their children).
    *   **Rerouting (2-pass DFS):** First pass calculates values relative to root; second pass pushes parent changes down to calculate values relative to each node as the root.

---

## 3. Memoization vs Tabulation — When to Use Which
*   **Top-down Memoization (Recursion + Memo):** Easier to write under pressure. Use when the state space is sparse (not all states are visited) or the recurrence is complex.
*   **Bottom-up Tabulation (Iterative):** Safer for large inputs (no recursion depth limit). Use when all states need to be visited in a predictable order.

---

## 4. The 3-Step DP Formulation under Pressure
1.  **State Definition (Crucial):** Write down in plain English: *"What does `dp[i][j]` represent?"* (e.g., "The max profit using first `i` elements with remaining capacity `j`").
2.  **Transition formula:** Write down the transitions. Think: *"If I am at the current state, what decisions can I make to reach it from smaller subproblems?"*
3.  **Base Cases:** Initialize your trivial boundaries (empty strings, zero capacity, single elements) before looping.

---

## ⚡ Top DP Bugs to Avoid

1.  **Forgot to Reset:** Global DP tables retain values from previous test cases. Reset them inside the class entry function.
2.  **Pruning errors:** If pruning early (e.g., `sum > max_sum`), make sure you return `0` or appropriate values, but do not cache invalid states.
3.  **Space optimization off-by-one:** When optimizing 2D DP to 1D vectors, ensure you copy elements correctly to avoid using values from the current row before they are calculated.
4.  **Integer Overflow in Transitions:** DP accumulates multiplied values fast. Use `(long long)dp[i][j]` or declare DP arrays as `long long` when intermediate products can exceed `2 * 10^9`.
