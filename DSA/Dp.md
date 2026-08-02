# CSES Dynamic Programming - Core Algorithm & Pattern Reference

> **All 20 CSES Dynamic Programming Problems with Classic Pattern Identifiers, Precise Problem Statements, Core Algorithms & Recurrence Templates, Time/Space Complexities, and Code Snippets (only where essential).**

---

## Quick Reference Summary Table

| # | Problem | Classic Pattern | Core Recurrence / Algorithm Strategy | Time | Space |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | **Dice Combinations** | 1D Unbounded Sum DP | $dp[i] = \sum_{k=1}^6 dp[i-k]$ | $O(N)$ | $O(N)$ |
| 2 | **Minimizing Coins** | Unbounded Knapsack (Min Coins) | $dp[i] = 1 + \min_{c} dp[i-c]$ | $O(N \cdot X)$ | $O(X)$ |
| 3 | **Coin Combinations I** | Unbounded Knapsack (Permutations) | Outer Sum $i$, Inner Coins $c$: $dp[i] += dp[i-c]$ | $O(N \cdot X)$ | $O(X)$ |
| 4 | **Coin Combinations II** | Unbounded Knapsack (Combinations) | Outer Coins $c$, Inner Sum $i$: $dp[i] += dp[i-c]$ | $O(N \cdot X)$ | $O(X)$ |
| 5 | **Removing Digits** | Min Step Subtraction DP | $dp[i] = 1 + \min_{d \in \text{digits}(i)} dp[i-d]$ | $O(N \log_{10} N)$ | $O(N)$ |
| 6 | **Grid Paths I** | 2D Grid DP with Obstacles | $dp[i][j] = dp[i-1][j] + dp[i][j-1]$ (if no `'*'`) | $O(N^2)$ | $O(N^2)$ |
| 7 | **Book Shop** | 0/1 Knapsack (1D Backward) | $dp[w] = \max(dp[w], dp[w - \text{price}] + \text{pages})$ | $O(N \cdot X)$ | $O(X)$ |
| 8 | **Array Description** | 2D Multi-State Constraint DP | $dp[i][v] = dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]$ | $O(N \cdot M)$ | $O(M)$ |
| 9 | **Counting Numbers** | Digit DP (Non-Repeating Constraint) | `dp[pos][prev][started][tight]` | $O(\log_{10} B)$ | $O(1)$ |
| 10 | **Edit Distance** | 2D Levenshtein Distance DP | $dp[i][j] = 1 + \min(\text{ins}, \text{del}, \text{rep})$ | $O(N \cdot M)$ | $O(N \cdot M)$ |
| 11 | **Rectangle Cutting** | 2D Range Cut DP | $dp[w][h] = 1 + \min_k (dp[w-k][h]+dp[k][h], \dots)$ | $O(A \cdot B \cdot (A+B))$ | $O(A \cdot B)$ |
| 12 | **Money Sums** | 0/1 Subset Sum Reachable DP | 1D Boolean backward: $dp[s] = dp[s] \lor dp[s - coin]$ | $O(N \cdot \sum C)$ | $O(\sum C)$ |
| 13 | **Removal Game** | Interval DP / Stone Game | $dp[l][r] = \max(A[l] - dp[l+1][r], A[r] - dp[l][r-1])$ | $O(N^2)$ | $O(N^2)$ |
| 14 | **Two Sets II** | 0/1 Subset Sum Equal Partition | 0/1 Knapsack for target $S/2$, multiply by $\text{inv}(2)$ | $O(N \cdot S)$ | $O(S)$ |
| 15 | **Increasing Subsequence**| LIS $O(N \log N)$ (Patience Sort) | `lower_bound(tails.begin(), tails.end(), x)` | $O(N \log N)$ | $O(N)$ |
| 16 | **Increasing Subsequence II**| Value-Indexed LIS Count + BIT | $\text{dp}[x] = 1 + \text{query}(x-1)$ via Fenwick Tree | $O(N \log N)$ | $O(N)$ |
| 17 | **Longest Common Subseq**| 2D Sequence Alignment (LCS) | $dp[i][j] = 1 + dp[i-1][j-1]$ (if match) | $O(N \cdot M)$ | $O(N \cdot M)$ |
| 18 | **Projects** | Weighted Job Scheduling DP + BS | $dp[i] = \max(dp[i-1], dp[\text{prev}] + \text{reward})$ | $O(N \log N)$ | $O(N)$ |
| 19 | **Elevator Rides** | Bitmask DP (Partition / Bin Pack) | $dp[mask] = \{\text{rides}, \text{current\_weight}\}$ | $O(N \cdot 2^N)$ | $O(2^N)$ |
20 | **Minimal Grid Path** | Lexicographical 2D String DP | $dp[i][j] = \min(dp[i-1][j], dp[i][j-1]) + grid[i][j]$ | $O(N^2)$ | $O(N^2)$ |

---

## Problem Breakdown: Algorithms & Recurrence Templates

### 1. Dice Combinations
* **Classic Pattern**: 1D Unbounded Sum DP (Fibonacci Transition Variant)
* **Problem Statement**: Given a target sum $N$ ($1 \le N \le 10^6$), count the number of ways to form sum $N$ using dice rolls ($1 \dots 6$). Output modulo $10^9+7$.
* **Algorithm & Template**:
  - State: $dp[i]$ = ways to form sum $i$. Base case: $dp[0] = 1$.
  - Transition:
    $$dp[i] = \sum_{k=1}^6 dp[i - k] \pmod{10^9 + 7} \quad \text{for } i - k \ge 0$$
* **Complexities**: $O(N)$ Time | $O(N)$ Space

---

### 2. Minimizing Coins
* **Classic Pattern**: Unbounded Knapsack (Minimum Coins / Coin Change I)
* **Problem Statement**: Given $N$ coin values and a target sum $X$, find the minimum number of coins needed to produce sum $X$. Each coin can be used infinitely many times. Print `-1` if impossible.
* **Algorithm & Template**:
  - State: $dp[i]$ = min coins to form sum $i$. Base cases: $dp[0] = 0$, $dp[i] = \infty$.
  - Transition:
    $$dp[i] = 1 + \min_{c \in \text{coins}, c \le i} dp[i - c]$$
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

---

### 3. Coin Combinations I & 4. Coin Combinations II
* **Problem Statement**: Count ways to form target sum $X$ using $N$ coins.
  - *Coin Combinations I*: Order **matters** (Permutations).
  - *Coin Combinations II*: Order **does NOT matter** (Combinations).
* **Algorithm & Permutation vs Combination Template**:
  ```cpp
  // 1. Permutations: Outer loop sum i, Inner loop coins c
  for (int i = 1; i <= x; i++)
      for (int c : coins) if (i - c >= 0) dp[i] = (dp[i] + dp[i - c]) % MOD;

  // 2. Combinations: Outer loop coins c, Inner loop sum i (forward pass)
  for (int c : coins)
      for (int i = c; i <= x; i++) dp[i] = (dp[i] + dp[i - c]) % MOD;
  ```
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

---

### 5. Removing Digits
* **Classic Pattern**: Min Step Subtraction DP / Greedy Digit Subtraction
* **Problem Statement**: Count the minimum steps to reduce $N$ to $0$ by repeatedly subtracting any non-zero digit present in the current number.
* **Algorithm & Template**:
  - State: $dp[i]$ = min steps to reduce $i$ to $0$. Base case: $dp[0] = 0$.
  - Transition:
    $$dp[i] = 1 + \min_{d \in \text{digits}(i), d > 0} dp[i - d]$$
  - *Greedy Alternative*: Subtracting the maximum digit at each step is also provably optimal in $O(\text{steps})$.
* **Complexities**: $O(N \log_{10} N)$ Time | $O(N)$ Space

---

### 6. Grid Paths I
* **Classic Pattern**: 2D Grid Path Counting DP with Obstacles
* **Problem Statement**: Count valid paths from $(0, 0)$ to $(N-1, N-1)$ in an $N \times N$ grid moving only Right and Down, avoiding obstacles (`'*'`). Output modulo $10^9+7$.
* **Algorithm & Template**:
  - State: $dp[i][j]$ = number of paths to cell $(i, j)$. Base case: $dp[0][0] = 1$.
  - Transition:
    $$dp[i][j] = (dp[i-1][j] + dp[i][j-1]) \pmod{10^9+7} \quad \text{if } grid[i][j] \neq \text{'*'}$$
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space

---

### 7. Book Shop
* **Classic Pattern**: 0/1 Knapsack Problem (Space-Optimized 1D Backward Pass)
* **Problem Statement**: Given $N$ books with prices and page counts, find the maximum pages you can buy with budget $X$. Each book can be bought at most once.
* **Algorithm & Template**:
  - State: $dp[w]$ = max pages for budget $w$. Base case: $dp[0 \dots X] = 0$.
  - Transition (Iterate budget **BACKWARD** to prevent reusing the same book):
    $$dp[w] = \max(dp[w], \; dp[w - \text{price}[i]] + \text{pages}[i]) \quad \text{for } w = X \text{ down to } \text{price}[i]$$
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

---

### 8. Array Description
* **Classic Pattern**: 2D Multi-State Constraint DP (Adjacent Difference Constraint)
* **Problem Statement**: Given an array of length $N$ with values in $[1, M]$ where some elements are $0$ (unknown) and adjacent elements differ by at most $1$ ($|a_i - a_{i-1}| \le 1$), count valid complete arrays $\pmod{10^9+7}$.
* **Algorithm & Template**:
  - State: $dp[i][v]$ = ways to fill prefix up to index $i$ ending with value $v$.
  - Transition:
    $$dp[i][v] = (dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]) \pmod{10^9+7}$$
    *(If $a_i \neq 0$, set $dp[i][k] = 0$ for all $k \neq a_i$)*.
* **Complexities**: $O(N \cdot M)$ Time | $O(M)$ Space

---

### 9. Counting Numbers
* **Classic Pattern**: Digit DP (Non-Repeating Constraint)
* **Problem Statement**: Count integers in range $[A, B]$ ($0 \le A \le B \le 10^{18}$) with no two adjacent digits equal.
* **Algorithm & Template (Code Required due to Digit DP state complexity)**:
  - State: `dp[pos][prev_digit][started][tight]`
  - Compute answer as `solve(B) - solve(A - 1)`.

```cpp
long long solveDigitDP(const string& num) {
    int len = num.size();
    long long dp[20][11][2][2]; memset(dp, 0, sizeof(dp));
    dp[0][10][0][1] = 1;

    for (int pos = 0; pos < len; pos++) {
        int limit = num[pos] - '0';
        for (int prev = 0; prev <= 10; prev++) {
            for (int started = 0; started <= 1; started++) {
                for (int tight = 0; tight <= 1; tight++) {
                    long long ways = dp[pos][prev][started][tight];
                    if (!ways) continue;
                    int maxD = tight ? limit : 9;
                    if (!started) dp[pos + 1][10][0][tight && (0 == maxD)] += ways;
                    for (int d = (started ? 0 : 1); d <= maxD; d++) {
                        if (started && d == prev) continue; // No adjacent equal digits
                        dp[pos + 1][d][1][tight && (d == maxD)] += ways;
                    }
                }
            }
        }
    }
    long long total = 0;
    for (int d = 0; d <= 10; d++) for (int t = 0; t <= 1; t++) total += dp[len][d][1][t];
    return total + 1; // +1 for 0
}
```
* **Complexities**: $O(\log_{10} B)$ Time | $O(1)$ Space

---

### 10. Edit Distance
* **Classic Pattern**: 2D String Alignment / Levenshtein Distance DP
* **Problem Statement**: Find min edit operations (insert, delete, replace) to transform string $S_1$ into string $S_2$.
* **Algorithm & Template**:
  - State: $dp[i][j]$ = min edit distance between $S_1[0 \dots i-1]$ and $S_2[0 \dots j-1]$.
  - Transition:
    $$dp[i][j] = \begin{cases} dp[i-1][j-1] & \text{if } S_1[i-1] == S_2[j-1] \\ 1 + \min(dp[i-1][j], \; dp[i][j-1], \; dp[i-1][j-1]) & \text{otherwise} \end{cases}$$
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

---

### 11. Rectangle Cutting
* **Classic Pattern**: 2D Range / Partition Cut DP
* **Problem Statement**: Find minimum straight cuts to divide an $A \times B$ rectangle into squares.
* **Algorithm & Template**:
  - State: $dp[w][h]$ = min cuts for rectangle of size $w \times h$. Base case: $dp[x][x] = 0$.
  - Transition:
    $$dp[w][h] = 1 + \min \begin{cases} \min_{1 \le k < w} (dp[k][h] + dp[w-k][h]) \\ \min_{1 \le k < h} (dp[w][k] + dp[w][h-k]) \end{cases}$$
* **Complexities**: $O(A \cdot B \cdot (A+B))$ Time | $O(A \cdot B)$ Space

---

### 12. Money Sums
* **Classic Pattern**: 0/1 Knapsack Reachable Set / Subset Sum Existence DP
* **Problem Statement**: Find all distinct total sums that can be formed by any subset of $N$ given coins.
* **Algorithm & Template**:
  - State: Boolean array $dp[s] = \text{true}$ if sum $s$ is reachable. Base case: $dp[0] = \text{true}$.
  - Transition (Iterate sum $s$ **BACKWARD**):
    $$dp[s] = dp[s] \lor dp[s - c] \quad \text{for } s = \text{maxSum} \text{ down to } c$$
* **Complexities**: $O(N \cdot \sum C)$ Time | $O(\sum C)$ Space

---

### 13. Removal Game
* **Classic Pattern**: Interval DP / Subarray Game Theory (Minimax Stone Game)
* **Problem Statement**: Two players alternately pick the first or last element of array $A$. Find max score Player 1 can achieve assuming optimal play.
* **Algorithm & Template**:
  - State: $dp[l][r]$ = net score difference (Player 1 $-$ Player 2) on range $[l, r]$.
  - Transition (Outer loop length $len = 2 \dots N$):
    $$dp[l][r] = \max(A[l] - dp[l+1][r], \; A[r] - dp[l][r-1])$$
    $$\text{Player 1 Max Score} = \frac{\sum A + dp[0][N-1]}{2}$$
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space

---

### 14. Two Sets II
* **Classic Pattern**: 0/1 Subset Sum Equal Partition ($S/2$)
* **Problem Statement**: Count ways to divide $\{1 \dots N\}$ into two subsets of equal sum modulo $10^9+7$.
* **Algorithm & Template**:
  - Total sum $S = \frac{N(N+1)}{2}$. If $S$ is odd $\implies 0$.
  - Otherwise, run 0/1 Knapsack to find subset sum counts for $target = S/2$:
    $$dp[w] = (dp[w] + dp[w - i]) \pmod{10^9+7} \quad \text{for } i = 1 \dots N, w = target \text{ down to } i$$
  - Multiply final answer by modular inverse $\text{inv}(2) = \frac{\text{MOD}+1}{2}$.
* **Complexities**: $O(N \cdot S)$ Time | $O(S)$ Space

---

### 15. Increasing Subsequence
* **Classic Pattern**: Longest Increasing Subsequence (LIS $O(N \log N)$ Patience Sort)
* **Problem Statement**: Find length of longest strictly increasing subsequence in array of size $N$.
* **Algorithm & Template**:
  - Maintain `tails` array storing min tail of all increasing subsequences of length $k$.
  - For each $x \in A$, find `it = lower_bound(tails.begin(), tails.end(), x)`.
  - If `it == tails.end()`, `tails.push_back(x)`, else `*it = x`. Result is `tails.size()`.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

---

### 16. Increasing Subsequence II
* **Classic Pattern**: Value-Indexed LIS Counting DP + Fenwick Tree (BIT)
* **Problem Statement**: Count total number of strictly increasing subsequences modulo $10^9+7$.
* **Algorithm & Template (Code Required for BIT Point-Update/Prefix-Query pattern)**:
  - Coordinate compress values into range $[1 \dots U]$.
  - For each $x$: $\text{ways} = 1 + \text{queryBIT}(\text{idx} - 1)$, then $\text{updateBIT}(\text{idx}, \text{ways})$.

```cpp
struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void update(int i, long long val) { for (; i <= n; i += i & -i) bit[i] = (bit[i] + val) % 1000000007; }
    long long query(int i) { long long s = 0; for (; i > 0; i -= i & -i) s = (s + bit[i]) % 1000000007; return s; }
};
```
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

---

### 17. Longest Common Subsequence (LCS)
* **Classic Pattern**: 2D String Sequence Alignment (LCS DP)
* **Problem Statement**: Find max length of a subsequence common to $S_1$ and $S_2$.
* **Algorithm & Template**:
  - State: $dp[i][j]$ = LCS length of prefixes $S_1[0 \dots i-1]$ and $S_2[0 \dots j-1]$.
  - Transition:
    $$dp[i][j] = \begin{cases} 1 + dp[i-1][j-1] & \text{if } S_1[i-1] == S_2[j-1] \\ \max(dp[i-1][j], \; dp[i][j-1]) & \text{otherwise} \end{cases}$$
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

---

### 18. Projects
* **Classic Pattern**: Weighted Job Scheduling (Interval Scheduling DP + Binary Search)
* **Problem Statement**: $N$ projects with start day $a_i$, end day $b_i$, reward $p_i$. Find max total reward for non-overlapping projects.
* **Algorithm & Template (Code Required for Binary Search Integration)**:
  - Sort projects by `end_day`. $dp[i] = \max(dp[i-1], \; dp[p] + \text{reward}_i)$ where $p$ is found via `upper_bound` binary search on end times for $\le \text{start}_i - 1$.

```cpp
long long maxProjectReward(int n, vector<Project>& projects) {
    sort(projects.begin(), projects.end(), [](const Project& a, const Project& b) { return a.end < b.end; });
    vector<long long> dp(n + 1, 0); vector<int> endTimes(n);
    for (int i = 0; i < n; i++) endTimes[i] = projects[i].end;
    for (int i = 1; i <= n; i++) {
        int p = upper_bound(endTimes.begin(), endTimes.begin() + i - 1, projects[i - 1].start - 1) - endTimes.begin();
        dp[i] = max(dp[i - 1], dp[p] + projects[i - 1].reward);
    }
    return dp[n];
}
```
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

---

### 19. Elevator Rides
* **Classic Pattern**: Bitmask DP (Partitioning / Bin Packing with Min Bins)
* **Problem Statement**: $N$ people ($N \le 20$) with weights $w_i$ and elevator capacity $X$. Find min rides needed to transfer all people.
* **Algorithm & Template (Code Required for Bitmask State Pair handling)**:
  - State: $dp[mask] = \{\text{rides}, \text{current\_ride\_weight}\}$.

```cpp
int minElevatorRides(int n, int x, const vector<int>& w) {
    vector<pair<int, int>> dp(1 << n, {n + 1, 0}); dp[0] = {1, 0};
    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) continue;
            auto prev = dp[mask];
            pair<int, int> nextState = (prev.second + w[i] <= x) ? pair<int,int>{prev.first, prev.second + w[i]} : pair<int,int>{prev.first + 1, w[i]};
            dp[mask | (1 << i)] = min(dp[mask | (1 << i)], nextState);
        }
    }
    return dp[(1 << n) - 1].first;
}
```
* **Complexities**: $O(N \cdot 2^N)$ Time | $O(2^N)$ Space

---

### 20. Minimal Grid Path
* **Classic Pattern**: 2D Grid Lexicographical String Construction DP
* **Problem Statement**: Find the lexicographically smallest string from $(0,0) \to (N-1, N-1)$ in an $N \times N$ grid of characters moving Right and Down.
* **Algorithm & Template**:
  - State: $dp[i][j]$ = min string to cell $(i, j)$. Base case: $dp[0][0] = string(1, grid[0][0])$.
  - Transition:
    $$dp[i][j] = \min(dp[i-1][j], \; dp[i][j-1]) + grid[i][j]$$
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space
