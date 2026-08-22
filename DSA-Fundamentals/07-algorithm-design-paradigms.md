# Section 07: Algorithm Design Paradigms

> **Focus:** Major algorithmic design paradigms—Divide & Conquer, Greedy strategies, Dynamic Programming (Memoization vs. Tabulation, core patterns), Backtracking with state-space pruning, and Two Pointers / Sliding Window mechanics.

---

# Table of Contents
1. [Divide and Conquer](#1-divide-and-conquer)
2. [Greedy Algorithms](#2-greedy-algorithms)
3. [Dynamic Programming (DP Foundations & Patterns)](#3-dynamic-programming-dp)
4. [Backtracking & State-Space Search](#4-backtracking--state-space-search)
5. [Two Pointers & Sliding Window](#5-two-pointers--sliding-window)
6. [Quick Recap](#6-quick-recap)

---

# 1. Divide and Conquer

**Divide and Conquer** breaks a problem down into smaller independent subproblems of the same type, solves the subproblems recursively, and combines their solutions to solve the original problem.

```
+-----------------------------------------------------------------------------------------+
| THE 3 STEPS OF DIVIDE AND CONQUER                                                       |
| 1. DIVIDE:   Partition problem into smaller subproblems of size n/b.                     |
| 2. CONQUER:  Recursively solve subproblems (base case reached when size is small).       |
| 3. COMBINE:  Merge subproblem solutions into the global solution (e.g. merge step).      |
+-----------------------------------------------------------------------------------------+
```

---

### Classic Application: Fast Exponentiation ($x^n$ in $O(\log n)$ Time)
Instead of computing $x^n$ by multiplying $x$ sequentially $n$ times ($O(n)$ time), divide the exponent by 2:

$$x^n = \begin{cases} (x^{n/2})^2 & \text{if } n \text{ is even} \\ x \cdot (x^{(n-1)/2})^2 & \text{if } n \text{ is odd} \end{cases}$$

```cpp
// Binary Exponentiation (O(log N) Time, O(1) Space)
long long fastPower(long long x, long long n, long long mod) {
    long long res = 1;
    x %= mod;
    while (n > 0) {
        if (n & 1) res = (res * x) % mod; // If n is odd, multiply x
        x = (x * x) % mod;                // Square base: x^(2k)
        n >>= 1;                          // Divide exponent by 2
    }
    return res;
}
```

---

### Interview Follow-Up Questions
1. **"How does Divide and Conquer differ from Dynamic Programming?"**
   - *Answer:* Divide and Conquer partitions the problem into **Disjoint (independent)** subproblems (e.g. MergeSort never solves the same subarray twice). Dynamic Programming is used when subproblems **Overlap** (e.g. Fibonacci, shortest paths), requiring memoization to avoid redundant computations.

---

# 2. Greedy Algorithms

A **Greedy Algorithm** builds a solution piece by piece, always making the **locally optimal choice** at each step with the hope that this choice leads to a **globally optimal solution**.

```
Greedy Necessary Conditions:
1. Greedy Choice Property: A global optimum can be arrived at by selecting local optimums
   without ever reconsidering or backtracking on previous choices.
2. Optimal Substructure: An optimal solution to the problem contains within it optimal
   solutions to subproblems.
```

---

### Classic Case Study: Interval Scheduling (Activity Selection)
- **Problem:** Given $N$ intervals with start and end times `[start, end]`, select the maximum number of mutually non-overlapping intervals.
- **The Greedy Choice:** **Sort intervals by their Finish Times (`end`) in ascending order**. Always pick the interval that finishes earliest, leaving maximum possible time for remaining activities.
- **Complexity:** $O(N \log N)$ (dominated by sorting).

```cpp
// Interval Scheduling: O(N log N) Time, O(1) Space
int maxNonOverlappingIntervals(vector<pair<int, int>>& intervals) {
    // Sort by END time ascending!
    sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    int count = 0, lastEnd = INT_MIN;
    for (const auto& [start, end] : intervals) {
        if (start >= lastEnd) {
            count++;
            lastEnd = end; // Greedily take this interval
        }
    }
    return count;
}
```

---

### When Does Greedy FAIL? (Greedy vs. DP)

| Problem | Greedy Approach | Dynamic Programming Approach | Why Greedy Fails |
| :--- | :--- | :--- | :--- |
| **Fractional Knapsack** | **$O(N \log N)$ (Greedy Works)** | Overkill | You can take fractions of items; sorting by value/weight density is strictly optimal. |
| **0/1 Knapsack** | Fails | **$O(N \cdot W)$ (DP Required)** | Items cannot be broken; taking the highest density item can leave empty capacity that prevents taking a higher-total-value item. |
| **Coin Change (Canonical: 1, 5, 10, 25)** | **Greedy Works** | Overkill | US coins form a Matroid/canonical system where largest coin is always optimal. |
| **Coin Change (Non-Canonical: 1, 3, 4 with Amount = 6)** | Fails ($4 + 1 + 1 = 3$ coins) | **DP Returns $3 + 3 = 2$ coins** | Greedy choice of largest coin ($4$) leaves suboptimal remaining amount ($2$). |

---

# 3. Dynamic Programming (DP)

Dynamic Programming is an optimization paradigm used when a problem exhibits two mathematical properties:
1. **Optimal Substructure:** The optimal solution can be constructed from optimal solutions of subproblems.
2. **Overlapping Subproblems:** The same subproblems are solved repeatedly in a naive recursion tree ($2^N \to \text{Polynomial}$).

```
Recursion Tree for Fibonacci(5) demonstrating Overlapping Subproblems:
                         [ Fib(5) ]
                       /            \
               [ Fib(4) ]          [ Fib(3) ] <---+ (Computed in both subtrees!)
              /          \        /          \    |
         [ Fib(3) ]    Fib(2)  [ Fib(3) ]   Fib(1)|
        /          \
     Fib(2)       Fib(1)
```

---

### Memoization (Top-Down) vs. Tabulation (Bottom-Up)

```
+-----------------------------------------------------------------------------------------+
| 1. MEMOIZATION (TOP-DOWN)                                                               |
| - Starts from original problem and recurses downward.                                   |
| - Computes subproblems on-demand and caches results in a hash map / array.              |
| - Trade-off: Easy to formulate, but incurs OS call stack frame overhead (O(N) space).   |
+-----------------------------------------------------------------------------------------+
| 2. TABULATION (BOTTOM-UP)                                                               |
| - Starts from base cases and iteratively fills an iterative DP table from 0 up to N.    |
| - Eliminates recursion call stack overhead and enables direct Memory Compression!       |
| - Trade-off: Must determine topological dependency order of subproblems beforehand.     |
+-----------------------------------------------------------------------------------------+
```

---

### Master DP Patterns

#### Pattern 1: 0/1 Knapsack Problem
Given $N$ items with weights $w_i$ and values $v_i$, maximize value inside a knapsack of capacity $W$.
- **State Definition:** $\text{dp}[i][w]$ = Maximum value using a subset of first $i$ items with capacity $w$.
- **Transition:**
  $$\text{dp}[i][w] = \max(\underbrace{\text{dp}[i-1][w]}_{\text{Exclude item } i}, \underbrace{\text{dp}[i-1][w - w_i] + v_i}_{\text{Include item } i \text{ (if } w \ge w_i \text{)}})$$
- **Space Optimization to 1D Array:** Notice $\text{dp}[i][w]$ only depends on previous row $\text{dp}[i-1]$. By iterating $w$ **BACKWARDS from $W$ down to $w_i$**, we compress space from $O(N \cdot W)$ to **$O(W)$**:

```cpp
// 0/1 Knapsack Space-Optimized (O(N * W) Time, O(W) Space)
int knapsack01(int W, const vector<int>& wt, const vector<int>& val) {
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < wt.size(); i++) {
        // Iterate BACKWARDS to prevent using the same item multiple times!
        for (int w = W; w >= wt[i]; w--) {
            dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
        }
    }
    return dp[W];
}
```

---

#### Pattern 2: Longest Increasing Subsequence (LIS)
Find the length of the longest strictly increasing subsequence in array `nums`.

```cpp
// Approach 1: Classic DP -> O(N^2) Time, O(N) Space
// dp[i] = max length of increasing subsequence ending at index i
int lengthOfLIS_DP(vector<int>& nums) {
    int n = nums.size(), maxLen = 1;
    vector<int> dp(n, 1);
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) dp[i] = max(dp[i], dp[j] + 1);
        }
        maxLen = max(maxLen, dp[i]);
    }
    return maxLen;
}

// Approach 2: Binary Search (Patience Sorting) -> O(N log N) Time, O(N) Space
// tails[k] stores the smallest tail element of all increasing subsequences of length k+1
int lengthOfLIS_BinarySearch(vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x); // Extend LIS
        else *it = x;                              // Overwrite to create a smaller tail!
    }
    return tails.size(); // Length of tails array IS the LIS length!
}
```

---

#### Pattern 3: Longest Common Subsequence (LCS)
Given strings $S_1$ of length $M$ and $S_2$ of length $N$:
- **Transition:**
  $$\text{dp}[i][j] = \begin{cases} \text{dp}[i-1][j-1] + 1 & \text{if } S_1[i-1] == S_2[j-1] \\ \max(\text{dp}[i-1][j], \text{dp}[i][j-1]) & \text{if } S_1[i-1] \ne S_2[j-1] \end{cases}$$
- **Complexity:** $O(M \cdot N)$ Time, optimized to $O(\min(M, N))$ Space using 2 rolling rows.

---

# 4. Backtracking & State-Space Search

**Backtracking** is a systematic method for searching the entire **State-Space Tree** of a combinatorial problem. It explores candidates via DFS, builds a solution incrementally, and **backtracks (undoes changes)** the exact moment it determines that the current branch cannot lead to a valid global solution (**Pruning**).

```
General Backtracking Recursion Skeleton:
void backtrack(State& state, Choices choices) {
    if (isSolution(state)) {
        recordSolution(state);
        return;
    }
    for (auto& choice : choices) {
        if (isValid(choice, state)) { // PRUNING BOUNDING FUNCTION
            makeChoice(state, choice);
            backtrack(state, remainingChoices);
            undoChoice(state, choice); // BACKTRACK!
        }
    }
}
```

---

### Classic Case Study: N-Queens Problem
Place $N$ non-attacking queens on an $N \times N$ chessboard such that no two queens share the same row, column, or diagonal.

```cpp
// N-Queens with O(1) Bitset/Array Pruning
void solveNQueens(int row, int n, vector<string>& board, vector<bool>& cols,
                  vector<bool>& diag1, vector<bool>& diag2, vector<vector<string>>& results) {
    if (row == n) {
        results.push_back(board);
        return;
    }
    for (int col = 0; col < n; col++) {
        // Main diagonal: row - col + (n - 1), Anti-diagonal: row + col
        int d1 = row - col + n - 1;
        int d2 = row + col;
        
        if (!cols[col] && !diag1[d1] && !diag2[d2]) {
            board[row][col] = 'Q';
            cols[col] = diag1[d1] = diag2[d2] = true;
            
            solveNQueens(row + 1, n, board, cols, diag1, diag2, results);
            
            // Undo choice (Backtrack!)
            board[row][col] = '.';
            cols[col] = diag1[d1] = diag2[d2] = false;
        }
    }
}
```

---

# 5. Two Pointers & Sliding Window

Both paradigms optimize nested brute-force loops from **$O(N^2)$ to $O(N)$ linear time** by maintaining monotonic pointer movements across arrays.

```
+-----------------------------------------------------------------------------------------+
| 1. TWO POINTERS (CONVERGING / OPPOSITE)                                                 |
| - Left starts at 0, Right starts at N - 1. Pointers move inward towards each other.     |
| - Requires sorted arrays or geometric monotonicity (e.g. 2Sum II, Container With Water).|
+-----------------------------------------------------------------------------------------+
| 2. SLIDING WINDOW (DYNAMIC EXPAND & SHRINK)                                             |
| - Right pointer expands window to satisfy condition.                                    |
| - Left pointer contracts window to restore validity or find minimum length.            |
| - Total Steps: Left and Right each advance at most N times -> Strictly O(N) Time!      |
+-----------------------------------------------------------------------------------------+
```

```cpp
// Minimum Size Subarray Sum >= Target (Sliding Window in O(N) Time)
int minSubArrayLen(int target, const vector<int>& nums) {
    int left = 0, currentSum = 0, minLen = INT_MAX;
    
    for (int right = 0; right < nums.size(); right++) {
        currentSum += nums[right]; // Expand window
        
        while (currentSum >= target) {
            minLen = min(minLen, right - left + 1);
            currentSum -= nums[left++]; // Shrink window
        }
    }
    return minLen == INT_MAX ? 0 : minLen;
}
```

---

# 6. Quick Recap

- **Divide and Conquer:** Solves disjoint subproblems (Fast Exponentiation $O(\log N)$, MergeSort).
- **Greedy Strategy:** Makes irrevocable locally optimal choices; requires Optimal Substructure and Greedy Choice Property (Interval Scheduling, Huffman).
- **Dynamic Programming:** Solves overlapping subproblems via Memoization (Top-Down) or Tabulation (Bottom-Up).
- **Knapsack Space Compression:** Iterate capacity $w$ backwards ($W \to w_i$) to optimize 0/1 Knapsack space to 1D $O(W)$.
- **LIS Optimization:** Patience sorting with binary search (`lower_bound`) drops LIS complexity from $O(N^2)$ to **$O(N \log N)$**.
- **Backtracking Pruning:** Pruning functions discard invalid state-space branches early (N-Queens, Sudoku).
- **Sliding Window Guarantee:** Left and Right pointers each traverse the array at most once, guaranteeing strictly **$O(N)$ linear time**.
