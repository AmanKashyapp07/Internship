# CSES Dynamic Programming - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 7 problems in the **Dynamic Programming** topic.

---

## 1. Removing Digits

- **Tier**: Tier 1
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

## 2. Array Description

- **Tier**: Tier 2
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

## 3. Counting Numbers

- **Tier**: Tier 3
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

## 4. Rectangle Cutting

- **Tier**: Tier 2
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

## 5. Elevator Rides

- **Tier**: Tier 3
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

## 6. Increasing Subsequence II

- **Tier**: Tier 3
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

## 7. Minimal Grid Path

- **Tier**: Tier 2
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
