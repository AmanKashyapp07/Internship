# CSES Dynamic Programming - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 20 problems in the **Dynamic Programming** topic.

---

## 1. Dice Combinations

- **Link**: [Dice Combinations](https://cses.fi/problemset/task/1633)
- **Problem Statement**: Count the number of ways to construct a sum $N$ by throwing a 6-sided die one or more times modulo $10^9+7$.
- **Interview Pattern**: 1D Linear Recurrence DP (Unbounded Permutation Sum).
- **Intuition**:
  - To reach target sum $i$, the last die throw can be any value $d \in \{1, 2, 3, 4, 5, 6\}$.
  - Recurrence: $\text{dp}[i] = \sum_{d=1}^{6} \text{dp}[i - d] \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Base case: `dp[0] = 1`.
  2. For $i$ from 1 to $N$:
     - For $d$ from 1 to 6: if $i - d \ge 0$, accumulate `dp[i] = (dp[i] + dp[i-d]) % MOD`.
  3. Output `dp[N]`.
- **Pseudocode**:
```text
dp[0] = 1
for i = 1 to N:
    for d = 1 to 6:
        if i >= d:
            dp[i] = (dp[i] + dp[i - d]) % MOD
return dp[N]
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 2. Minimizing Coins

- **Link**: [Minimizing Coins](https://cses.fi/problemset/task/1634)
- **Problem Statement**: Given $N$ coin values, find the minimum number of coins needed to produce a target sum $X$, or output -1 if impossible.
- **Interview Pattern**: Unbounded Knapsack (Min Coins DP).
- **Intuition**:
  - `dp[i]` = minimum coins to form sum $i$.
  - Transition: $\text{dp}[i] = \min_{c \in coins} (1 + \text{dp}[i - c])$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `dp[0] = 0`, `dp[1..X] = INF`.
  2. For $i = 1 \dots X$:
     - For each coin $c$: if $i \ge c$, `dp[i] = min(dp[i], 1 + dp[i - c])`.
  3. Return `dp[X]` if $\ne \text{INF}$, else -1.
- **Pseudocode**:
```text
dp[0..X] = INF, dp[0] = 0
for i = 1 to X:
    for c in coins:
        if i >= c:
            dp[i] = min(dp[i], 1 + dp[i - c])
return dp[X] if dp[X] != INF else -1
```
- **Complexity**: Time: $\mathcal{O}(N \cdot X)$, Space: $\mathcal{O}(X)$.

---

## 3. Coin Combinations I

- **Link**: [Coin Combinations I](https://cses.fi/problemset/task/1635)
- **Problem Statement**: Given $N$ coin values, count the number of distinct **ordered sequences** (permutations) of coins that sum up to $X$ modulo $10^9+7$.
- **Interview Pattern**: Unbounded Permutation DP (Outer Sum Loop).
- **Intuition**:
  - Order matters (e.g. 2+1 is distinct from 1+2). Loop target sum $i$ on the **outer** loop.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0] = 1`.
  2. Outer loop $i$ from 1 to $X$, Inner loop over each coin $c$:
     - If $i \ge c$, `dp[i] = (dp[i] + dp[i - c]) % MOD`.
  3. Return `dp[X]`.
- **Pseudocode**:
```text
dp[0] = 1
for i = 1 to X:
    for c in coins:
        if i >= c:
            dp[i] = (dp[i] + dp[i - c]) % MOD
return dp[X]
```
- **Complexity**: Time: $\mathcal{O}(N \cdot X)$, Space: $\mathcal{O}(X)$.

---

## 4. Coin Combinations II

- **Link**: [Coin Combinations II](https://cses.fi/problemset/task/1636)
- **Problem Statement**: Given $N$ coin values, count the number of distinct **unordered combinations** of coins that sum up to $X$ modulo $10^9+7$.
- **Interview Pattern**: Unbounded Combination DP (Outer Coin Loop).
- **Intuition**:
  - Order does NOT matter. Loop each coin $c$ on the **outer** loop to enforce non-decreasing coin processing.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0] = 1`.
  2. Outer loop over each coin $c$, Inner loop $i$ from $c$ to $X$:
     - `dp[i] = (dp[i] + dp[i - c]) % MOD`.
  3. Return `dp[X]`.
- **Pseudocode**:
```text
dp[0] = 1
for c in coins:
    for i = c to X:
        dp[i] = (dp[i] + dp[i - c]) % MOD
return dp[X]
```
- **Complexity**: Time: $\mathcal{O}(N \cdot X)$, Space: $\mathcal{O}(X)$.

---

## 5. Removing Digits

- **Link**: [Removing Digits](https://cses.fi/problemset/task/1637)
- **Problem Statement**: Given an integer $N$, count minimum steps to reduce $N$ to 0 by subtracting one of its own non-zero decimal digits in each step.
- **Interview Pattern**: Greedy / Top-Down Memoized DP.
- **Intuition**:
  - At state $x$, try subtracting each non-zero digit present in $x$.
  - Greedy choice: subtracting the maximum digit is always optimal! (DP or Greedy both work).
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0] = 0`.
  2. For $x = 1 \dots N$:
     - Extract all non-zero digits of $x$.
     - `dp[x] = 1 + min(dp[x - digit])`.
  3. Output `dp[N]`.
- **Pseudocode**:
```text
dp[0] = 0
for x = 1 to N:
    dp[x] = INF
    temp = x
    while temp > 0:
        d = temp % 10
        temp /= 10
        if d != 0:
            dp[x] = min(dp[x], 1 + dp[x - d])
return dp[N]
```
- **Complexity**: Time: $\mathcal{O}(N \log_{10} N)$, Space: $\mathcal{O}(N)$.

---

## 6. Grid Paths I

- **Link**: [Grid Paths I](https://cses.fi/problemset/task/1638)
- **Problem Statement**: Count paths from top-left $(1, 1)$ to bottom-right $(N, N)$ in an $N \times N$ grid moving only right and down, avoiding traps (`*`).
- **Interview Pattern**: 2D Grid DP.
- **Intuition**:
  - `dp[r][c]` = number of paths to cell $(r, c)$.
  - Recurrence: $\text{dp}[r][c] = \text{dp}[r-1][c] + \text{dp}[r][c-1]$ if cell is not trapped.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0][0] = 1` if `grid[0][0] == '.'` else 0.
  2. Iterate $r, c$ from 0 to $N-1$:
     - If `grid[r][c] == '*'`: `dp[r][c] = 0`.
     - Else: accumulate from top (`r-1, c`) and left (`r, c-1`).
  3. Output `dp[N-1][N-1]`.
- **Pseudocode**:
```text
if grid[0][0] == '.': dp[0][0] = 1
for r = 0 to N-1:
    for c = 0 to N-1:
        if grid[r][c] == '*': dp[r][c] = 0
        else:
            if r > 0: dp[r][c] = (dp[r][c] + dp[r-1][c]) % MOD
            if c > 0: dp[r][c] = (dp[r][c] + dp[r][c-1]) % MOD
return dp[N-1][N-1]
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N^2)$.

---

## 7. Book Shop

- **Link**: [Book Shop](https://cses.fi/problemset/task/1158)
- **Problem Statement**: Given $N$ books with prices $h_i$ and page counts $s_i$, maximize total pages bought with maximum budget $X$.
- **Interview Pattern**: 0/1 Knapsack DP (1D Space-Optimized).
- **Intuition**:
  - Standard 0/1 Knapsack: `dp[cap]` = max pages for budget `cap`. Iterate budget backwards to prevent reusing same book.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0..X] = 0`.
  2. For each book $i$ from 1 to $N$:
     - For budget `cap` from $X$ down to $h_i$:
       `dp[cap] = max(dp[cap], dp[cap - h_i] + s_i)`.
  3. Output `dp[X]`.
- **Pseudocode**:
```text
dp[0..X] = 0
for i = 0 to N-1:
    for cap = X down to price[i]:
        dp[cap] = max(dp[cap], dp[cap - price[i]] + pages[i])
return dp[X]
```
- **Complexity**: Time: $\mathcal{O}(N \cdot X)$, Space: $\mathcal{O}(X)$.

---

## 8. Array Description

- **Link**: [Array Description](https://cses.fi/problemset/task/1746)
- **Problem Statement**: An array of size $N$ has values in $[1, M]$. Adjacent elements must satisfy $|a_i - a_{i-1}| \le 1$. Given partial array with unknown elements (0s), count total valid arrays modulo $10^9+7$.
- **Interview Pattern**: 2D State DP (`dp[i][v]` = ways for index $i$ ending in value $v$).
- **Intuition**:
  - Value at index $i$ depends only on value at $i-1$: $\text{dp}[i][v] = \text{dp}[i-1][v-1] + \text{dp}[i-1][v] + \text{dp}[i-1][v+1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Base case: for index 0, set `dp[0][v] = 1` for allowed $v$.
  2. For index $i$ from 1 to $N-1$:
     - For $v = 1 \dots M$: if $A[i] == 0$ or $A[i] == v$:
       `dp[i][v] = (dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]) % MOD`.
  3. Sum `dp[N-1][v]` over all $v \in [1, M]$.
- **Pseudocode**:
```text
for i = 1 to N-1:
    for v = 1 to M:
        if A[i] == 0 or A[i] == v:
            dp[i][v] = (dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]) % MOD
return sum(dp[N-1][1..M]) % MOD
```
- **Complexity**: Time: $\mathcal{O}(N \cdot M)$, Space: $\mathcal{O}(N \cdot M)$.

---

## 9. Counting Numbers

- **Link**: [Counting Numbers](https://cses.fi/problemset/task/2220)
- **Problem Statement**: Count integers in range $[A, B]$ that have no two adjacent digits equal.
- **Interview Pattern**: Digit DP (`dp[pos][prev_digit][started][tight]`).
- **Intuition**:
  - Answer = `count(B) - count(A - 1)`.
  - Process digits left-to-right maintaining previous digit to enforce $d \neq \text{prev\_digit}$.
- **Step-by-Step Interview Walkthrough**:
  1. Convert number to digit string.
  2. Memoized recursion `solve(pos, prev_digit, leading_zeros, tight)`:
     - If `pos == len`, return 1.
     - Try placing digit $d \in [0, \text{limit}]$. If not leading zero and $d == \text{prev\_digit}$, skip.
  3. Compute `solve(B) - solve(A - 1)`.
- **Pseudocode**:
```text
function solve(pos, prev, leading_zero, tight):
    if pos == len: return 1
    limit = digits[pos] if tight else 9
    ans = 0
    for d = 0 to limit:
        if not leading_zero and d == prev: continue
        next_tight = tight and (d == limit)
        next_lz = leading_zero and (d == 0)
        ans += solve(pos + 1, d, next_lz, next_tight)
    return ans
```
- **Complexity**: Time: $\mathcal{O}(\log_{10} B)$, Space: $\mathcal{O}(\log_{10} B)$.

---

## 10. Edit Distance

- **Link**: [Edit Distance](https://cses.fi/problemset/task/1639)
- **Problem Statement**: Find the minimum edit operations (insert, delete, replace) to transform string $S_1$ into string $S_2$.
- **Interview Pattern**: Levenshtein 2D DP.
- **Intuition**:
  - `dp[i][j]` = edit distance for prefixes $S_1[0..i-1]$ and $S_2[0..j-1]$.
  - If $S_1[i-1] == S_2[j-1]$, `dp[i][j] = dp[i-1][j-1]`.
  - Else `dp[i][j] = 1 + min(replace, delete, insert)` = $1 + \min(\text{dp}[i-1][j-1], \text{dp}[i-1][j], \text{dp}[i][j-1])$.
- **Step-by-Step Interview Walkthrough**:
  1. Base cases: `dp[i][0] = i`, `dp[0][j] = j`.
  2. Double loop $i \in [1, N]$, $j \in [1, M]$:
     - Match: `dp[i][j] = dp[i-1][j-1]` if equal.
     - Mismatch: `dp[i][j] = 1 + min({dp[i-1][j-1], dp[i-1][j], dp[i][j-1]})`.
  3. Output `dp[N][M]`.
- **Pseudocode**:
```text
for i = 0 to N: dp[i][0] = i
for j = 0 to M: dp[0][j] = j

for i = 1 to N:
    for j = 1 to M:
        if S1[i-1] == S2[j-1]: dp[i][j] = dp[i-1][j-1]
        else: dp[i][j] = 1 + min({dp[i-1][j-1], dp[i-1][j], dp[i][j-1]})
return dp[N][M]
```
- **Complexity**: Time: $\mathcal{O}(N \cdot M)$, Space: $\mathcal{O}(N \cdot M)$.

---

## 11. Rectangle Cutting

- **Link**: [Rectangle Cutting](https://cses.fi/problemset/task/1744)
- **Problem Statement**: Find minimum cuts to split an $A \times B$ rectangle into $1 \times 1$ squares.
- **Interview Pattern**: Interval DP (`dp[i][j]` for $i \times j$ rectangle).
- **Intuition**:
  - Base case: `dp[i][i] = 0` (already a square).
  - Try all horizontal cuts $k \in [1, i-1]$ and vertical cuts $k \in [1, j-1]$.
- **Step-by-Step Interview Walkthrough**:
  1. If $i == j$, `dp[i][j] = 0`.
  2. Horizontal cuts: `ans = min(ans, 1 + dp[k][j] + dp[i-k][j])`.
  3. Vertical cuts: `ans = min(ans, 1 + dp[i][k] + dp[i][j-k])`.
- **Pseudocode**:
```text
for i = 1 to A:
    for j = 1 to B:
        if i == j: dp[i][j] = 0; continue
        dp[i][j] = INF
        for k = 1 to i-1: dp[i][j] = min(dp[i][j], 1 + dp[k][j] + dp[i-k][j])
        for k = 1 to j-1: dp[i][j] = min(dp[i][j], 1 + dp[i][k] + dp[i][j-k])
return dp[A][B]
```
- **Complexity**: Time: $\mathcal{O}(A \cdot B \cdot (A + B))$, Space: $\mathcal{O}(A \cdot B)$.

---

## 12. Money Sums

- **Link**: [Money Sums](https://cses.fi/problemset/task/1745)
- **Problem Statement**: Given $N$ coin values, output all distinct total money sums that can be produced using a subset of coins.
- **Interview Pattern**: 0/1 Subset Sum Reachability DP.
- **Intuition**:
  - `dp[s]` = boolean reachability of sum $s$. Iterate coins and update `dp[s]` backwards.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0] = true`, all other `dp[s] = false`.
  2. For each coin $c$: for sum $s$ from $\text{MAX\_SUM}$ down to $c$:
     - `dp[s] = dp[s] || dp[s - c]`.
  3. Collect all $s > 0$ where `dp[s] == true`.
- **Pseudocode**:
```text
dp[0] = true
for c in coins:
    for s = MAX_SUM down to c:
        dp[s] = dp[s] or dp[s - c]

collect all s > 0 where dp[s] is true
```
- **Complexity**: Time: $\mathcal{O}(N \cdot \text{Sum})$, Space: $\mathcal{O}(\text{Sum})$.

---

## 13. Removal Game

- **Link**: [Removal Game](https://cses.fi/problemset/task/1097)
- **Problem Statement**: Two players take coins from either end of an array. Find max score of player 1 assuming optimal play.
- **Interview Pattern**: Interval Minimax DP (`dp[i][j]` = max score difference).
- **Intuition**:
  - `dp[i][j]` stores max score difference player can achieve from subsegment $[i, j]$.
  - Recurrence: $\text{dp}[i][j] = \max(A[i] - \text{dp}[i+1][j], A[j] - \text{dp}[i][j-1])$.
  - Player 1 score = $(\text{totalSum} + \text{dp}[0][N-1]) / 2$.
- **Step-by-Step Interview Walkthrough**:
  1. Base cases: `dp[i][i] = A[i]`.
  2. Process range lengths $L$ from 2 to $N$:
     - `dp[i][j] = max(A[i] - dp[i+1][j], A[j] - dp[i][j-1])`.
  3. Return `(total_sum + dp[0][N-1]) / 2`.
- **Pseudocode**:
```text
for i = 0 to N-1: dp[i][i] = A[i]
for len = 2 to N:
    for i = 0 to N - len:
        j = i + len - 1
        dp[i][j] = max(A[i] - dp[i+1][j], A[j] - dp[i][j-1])
return (sum(A) + dp[0][N-1]) / 2
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N^2)$.

---

## 14. Two Sets II

- **Link**: [Two Sets II](https://cses.fi/problemset/task/1093)
- **Problem Statement**: Count ways to partition $1 \dots N$ into two sets of equal sum modulo $10^9+7$.
- **Interview Pattern**: 0/1 Subset Sum DP + Modular Division by 2.
- **Intuition**:
  - Total sum $S = N(N+1)/2$. If $S$ is odd, return 0.
  - Count subsets summing to $S/2$; divide answer by 2 (multiply by modular inverse of 2).
- **Step-by-Step Interview Walkthrough**:
  1. Check if total sum $S$ is even. Target $= S / 2$.
  2. Set `dp[0] = 1`.
  3. For $x = 1 \dots N$: for $s = \text{target}$ down to $x$:
     `dp[s] = (dp[s] + dp[s - x]) % MOD`.
  4. Return `dp[target] * INV2 % MOD`.
- **Pseudocode**:
```text
if (N * (N + 1) / 2) is odd: return 0
target = N * (N + 1) / 4
dp[0] = 1
for x = 1 to N:
    for s = target down to x:
        dp[s] = (dp[s] + dp[s - x]) % MOD
return dp[target] * INV2 % MOD
```
- **Complexity**: Time: $\mathcal{O}(N \cdot S)$, Space: $\mathcal{O}(S)$.

---

## 15. Increasing Subsequence

- **Link**: [Increasing Subsequence](https://cses.fi/problemset/task/1145)
- **Problem Statement**: Find the length of the Longest Increasing Subsequence (LIS) in an array of $N$ integers.
- **Interview Pattern**: Patience Sorting / Binary Search LIS ($\mathcal{O}(N \log N)$).
- **Intuition**:
  - Maintain array `tails` where `tails[i]` stores smallest tail element of all increasing subsequences of length $i+1$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize empty array `tails`.
  2. For each element $x \in A$:
     - Use `lower_bound` to find first element $\ge x$ in `tails`.
     - If found, replace it with $x$; else append $x$ to `tails`.
  3. LIS length is `tails.size()`.
- **Pseudocode**:
```text
tails = []
for x in A:
    idx = lower_bound(tails, x)
    if idx == tails.end(): tails.push(x)
    else: *idx = x
return tails.size()
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 16. Projects

- **Link**: [Projects](https://cses.fi/problemset/task/1140)
- **Problem Statement**: Given $N$ projects with start day $a_i$, end day $b_i$, and reward $p_i$, select non-overlapping projects to maximize total reward.
- **Interview Pattern**: Interval Scheduling DP + Binary Search (`upper_bound`).
- **Intuition**:
  - Sort projects by end day $b_i$.
  - `dp[i]` = max reward considering first $i$ projects.
  - Option 1: Skip project $i \to \text{dp}[i-1]$.
  - Option 2: Take project $i \to \text{reward}_i + \text{dp}[\text{last\_non\_overlapping\_project}]$.
- **Step-by-Step Interview Walkthrough**:
  1. Sort projects by end time.
  2. For project $i$: binary search for latest project $j$ ending strictly before project $i$'s start time.
  3. `dp[i] = max(dp[i-1], reward[i] + dp[j])`.
- **Pseudocode**:
```text
sort(projects by end_day)
for i = 1 to N:
    j = binary_search_last_ending_before(start_day[i])
    dp[i] = max(dp[i-1], reward[i] + dp[j])
return dp[N]
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 17. Elevator Rides

- **Link**: [Elevator Rides](https://cses.fi/problemset/task/1653)
- **Problem Statement**: Given $N$ people with weights $w_i$ and an elevator with maximum capacity $X$, find minimum number of elevator rides to transport all people.
- **Interview Pattern**: Bitmask DP storing pair `{rides, weight_of_last_ride}`.
- **Intuition**:
  - `dp[mask]` = `{min_rides, min_last_weight}` for subset of people represented by `mask`.
  - Try adding person $i \notin \text{mask}$ to `dp[mask]`.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dp[0] = {1, 0}`, all other `dp[mask] = {INF, INF}`.
  2. For `mask` from 0 to $2^N - 1$:
     - For person $i$ not in `mask`:
       - If `dp[mask].weight + w[i] <= X`: option = `{rides, weight + w[i]}`.
       - Else: option = `{rides + 1, w[i]}`.
       - `dp[mask | (1<<i)] = min(dp[mask | (1<<i)], option)`.
  3. Output `dp[(1<<N)-1].rides`.
- **Pseudocode**:
```text
dp[0] = {1, 0}
for mask = 0 to (1<<N) - 1:
    for i = 0 to N-1:
        if not (mask & (1 << i)):
            curr = dp[mask]
            if curr.weight + w[i] <= X:
                next_state = {curr.rides, curr.weight + w[i]}
            else:
                next_state = {curr.rides + 1, w[i]}
            dp[mask | (1 << i)] = min(dp[mask | (1 << i)], next_state)
return dp[(1<<N)-1].rides
```
- **Complexity**: Time: $\mathcal{O}(N \cdot 2^N)$, Space: $\mathcal{O}(2^N)$.

---

## 18. Increasing Subsequence II

- **Link**: [Increasing Subsequence II](https://cses.fi/problemset/task/1749)
- **Problem Statement**: Count the total number of increasing subsequences in an array of $N$ integers modulo $10^9+7$.
- **Interview Pattern**: Coordinate Compression + Fenwick Tree DP.
- **Intuition**:
  - `dp[v]` = number of increasing subsequences ending with value $v$.
  - Transition: $\text{dp}[v] = 1 + \sum_{u < v} \text{dp}[u]$. Range query using Fenwick tree!
- **Step-by-Step Interview Walkthrough**:
  1. Coordinate compress array elements to $1 \dots N$.
  2. Initialize Fenwick Tree BIT of size $N$.
  3. For each compressed element $x \in A$:
     - `ways = (1 + BIT.query(x - 1)) % MOD`.
     - `BIT.update(x, ways)`.
  4. Total sum = `BIT.query(N)`.
- **Pseudocode**:
```text
compress(A)
for x in A:
    ways = (1 + BIT.query(x - 1)) % MOD
    BIT.update(x, ways)
return BIT.query(N)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 19. Longest Common Subsequence

- **Link**: [Longest Common Subsequence](https://cses.fi/problemset/task/1145)
- **Problem Statement**: Find the length of the Longest Common Subsequence between two arrays $A$ and $B$, and output the sequence.
- **Interview Pattern**: Standard 2D LCS DP + Backtracking.
- **Intuition**:
  - `dp[i][j]` = length of LCS of prefixes $A[0..i-1]$ and $B[0..j-1]$.
  - Trace back from `dp[N][M]` to reconstruct the subsequence.
- **Step-by-Step Interview Walkthrough**:
  1. Compute 2D DP matrix: if $A[i-1] == B[j-1]$, `dp[i][j] = 1 + dp[i-1][j-1]`, else $\max(\text{dp}[i-1][j], \text{dp}[i][j-1])$.
  2. Backtrack from $(N, M)$ to collect elements.
- **Pseudocode**:
```text
for i = 1 to N:
    for j = 1 to M:
        if A[i-1] == B[j-1]: dp[i][j] = 1 + dp[i-1][j-1]
        else: dp[i][j] = max(dp[i-1][j], dp[i][j-1])

reconstruct elements by tracing back from (N, M)
```
- **Complexity**: Time: $\mathcal{O}(N \cdot M)$, Space: $\mathcal{O}(N \cdot M)$.

---

## 20. Minimal Grid Path

- **Link**: [Minimal Grid Path](https://cses.fi/problemset/task/3226)
- **Problem Statement**: Find the lexicographically smallest string formed by walking from $(0, 0)$ to $(N-1, N-1)$ in a grid of letters.
- **Interview Pattern**: Step-by-step Level BFS Greedy Choice.
- **Intuition**:
  - Distance from start to cell $(r, c)$ is $r + c$.
  - At step $k = 0 \dots 2N-2$, find the minimum character among all reachable cells at step $k$, and only keep those minimal cells for step $k+1$.
- **Step-by-Step Interview Walkthrough**:
  1. Start level 0 with cell $\{(0, 0)\}$.
  2. For step $k = 1 \dots 2N-2$:
     - Collect all valid right/down neighbor cells from current level.
     - Find minimum character `min_char` among these neighbors.
     - New level = all neighbors having character `min_char`.
  3. Concatenate `min_char` at each level to form the string.
- **Pseudocode**:
```text
curr_level = {(0, 0)}
result = grid[0][0]

for step = 1 to 2N-2:
    neighbors = collect_next_cells(curr_level)
    min_char = min(grid[r][c] for (r, c) in neighbors)
    result += min_char
    curr_level = {(r, c) in neighbors if grid[r][c] == min_char}

return result
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N)$.
