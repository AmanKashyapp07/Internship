# 🧠 CSES Dynamic Programming - Complete Tabulation Master Reference

> **All 20 CSES Dynamic Programming Problems with Problem Statements, DP State Definitions, Core Tabulation Formulas, Time/Space Complexities, and Full Bottom-Up C++ Tabulation Templates.**

---

## Quick Reference Summary Table

| Problem | DP Pattern / State | Core Tabulation Transition | Time | Space |
| :--- | :--- | :--- | :--- | :--- |
| **Dice Combinations** | 1D Sum DP | $dp[i] = \sum_{k=1}^6 dp[i-k]$ | $O(N)$ | $O(N)$ |
| **Minimizing Coins** | Unbounded Knapsack (Min) | $dp[i] = \min_{c} (dp[i-c] + 1)$ | $O(N \cdot X)$ | $O(X)$ |
| **Coin Combinations I** | Permutations (Sum) | Outer sum, inner coins: $dp[i] += dp[i-c]$ | $O(N \cdot X)$ | $O(X)$ |
| **Coin Combinations II** | Combinations (Sum) | Outer coins, inner sum: $dp[i] += dp[i-c]$ | $O(N \cdot X)$ | $O(X)$ |
| **Removing Digits** | Min Step DP / Greedy | $dp[i] = 1 + \min_{d \in \text{digits}(i)} dp[i-d]$ | $O(N)$ | $O(N)$ |
| **Grid Paths I** | 2D Grid DP | $dp[i][j] = dp[i-1][j] + dp[i][j-1]$ (if no obstacle) | $O(N^2)$ | $O(N^2)$ |
| **Book Shop** | 0/1 Knapsack | Space-optimized 1D backward: $dp[w] = \max(dp[w], dp[w-price] + pages)$ | $O(N \cdot X)$ | $O(X)$ |
| **Array Description** | Adjacent Difference DP | $dp[i][v] = dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]$ | $O(N \cdot M)$ | $O(M)$ |
| **Counting Numbers** | Digit DP Tabulation | `dp[pos][prev][started][tight]` | $O(\log N)$ | $O(1)$ |
| **Edit Distance** | 2D Levenshtein DP | $dp[i][j] = \min(dp[i-1][j]+1, dp[i][j-1]+1, dp[i-1][j-1] + (A[i-1] \neq B[j-1]))$ | $O(N \cdot M)$ | $O(N \cdot M)$ |
| **Rectangle Cutting** | 2D Cut DP | $dp[w][h] = 1 + \min_k (dp[w-k][h] + dp[k][h], dp[w][h-k] + dp[w][k])$ | $O(A \cdot B \cdot (A+B))$ | $O(A \cdot B)$ |
| **Money Sums** | Subset Sum Reachable Set | 1D boolean DP backward: $dp[s] = dp[s] \mid dp[s - coin]$ | $O(N \cdot \sum C)$ | $O(\sum C)$ |
| **Removal Game** | Interval DP / Stone Game | $dp[l][r] = \max(A[l] - dp[l+1][r], A[r] - dp[l][r-1])$ | $O(N^2)$ | $O(N^2)$ |
| **Two Sets II** | 0/1 Knapsack Subset Sum | $dp[target]$ where $target = S/2$. Multiply by $\text{inv}(2)$. | $O(N \cdot S)$ | $O(S)$ |
| **Increasing Subsequence** | LIS $O(N \log N)$ | `lower_bound(tails.begin(), tails.end(), x)` | $O(N \log N)$ | $O(N)$ |
| **Increasing Subsequence II** | LIS Count on Values | Fenwick Tree point update & prefix sum query | $O(N \log N)$ | $O(N)$ |
| **Longest Common Subsequence** | LCS 2D DP | $dp[i][j] = 1 + dp[i-1][j-1]$ (if match) else $\max(\text{up}, \text{left})$ | $O(N \cdot M)$ | $O(N \cdot M)$ |
| **Projects** | Weighted Job Scheduling | Sort end times + Binary Search `upper_bound` + 1D DP | $O(N \log N)$ | $O(N)$ |
| **Elevator Rides** | Bitmask DP Tabulation | $dp[mask] = \{\text{rides}, \text{current\_weight}\}$ | $O(N \cdot 2^N)$ | $O(2^N)$ |
| **Minimal Grid Path** | Lexicographical Grid DP | Grid DP storing min lexicographical path | $O(N^2)$ | $O(N^2)$ |

---

## Detailed Problems & Complete Bottom-Up Tabulation Codes

### 1. Dice Combinations
* **Problem Statement**: Given an integer $N$, count the number of distinct ways to construct sum $N$ by throwing a standard 6-sided dice ($1 \dots 6$) multiple times. Answer modulo $10^9+7$.
* **DP State**: $dp[i]$ = number of ways to form sum $i$.
* **Tabulation Transition**: $dp[i] = \sum_{k=1}^6 dp[i - k] \pmod{10^9 + 7}$. Base Case: $dp[0] = 1$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
int diceCombinations(int n) {
    const int MOD = 1e9 + 7;
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= 6; k++) {
            if (i - k >= 0) {
                dp[i] = (dp[i] + dp[i - k]) % MOD;
            }
        }
    }
    return dp[n];
}
```

---

### 2. Minimizing Coins
* **Problem Statement**: Given $N$ coin values and a target sum $X$, find the minimum number of coins needed to produce sum $X$. Each coin can be used infinitely many times. Print `-1` if impossible.
* **DP State**: $dp[i]$ = minimum coins needed to form sum $i$.
* **Tabulation Transition**: $dp[i] = 1 + \min_{c \in \text{coins}} dp[i - c]$. Base Case: $dp[0] = 0$.
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

```cpp
int minimizingCoins(int n, int x, const vector<int>& coins) {
    vector<int> dp(x + 1, 1e9);
    dp[0] = 0;
    for (int i = 1; i <= x; i++) {
        for (int c : coins) {
            if (i - c >= 0) {
                dp[i] = min(dp[i], dp[i - c] + 1);
            }
        }
    }
    return dp[x] >= 1e9 ? -1 : dp[x];
}
```

---

### 3. Coin Combinations I & 4. Coin Combinations II
* **Problem Statements**:
  - *Coin Combinations I*: Count number of ways to construct sum $X$ where order of coins matters (permutations).
  - *Coin Combinations II*: Count number of ways to construct sum $X$ where order of coins does **not** matter (combinations).
* **Pattern**: **Permutation vs. Combination Loop Order**
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

```cpp
// Coin Combinations I (Permutations: Outer loop sum, Inner loop coins)
int coinCombinationsI(int n, int x, const vector<int>& coins) {
    const int MOD = 1e9 + 7;
    vector<int> dp(x + 1, 0); dp[0] = 1;
    for (int i = 1; i <= x; i++) {
        for (int c : coins) {
            if (i - c >= 0) dp[i] = (dp[i] + dp[i - c]) % MOD;
        }
    }
    return dp[x];
}

// Coin Combinations II (Combinations: Outer loop coins, Inner loop sum)
int coinCombinationsII(int n, int x, const vector<int>& coins) {
    const int MOD = 1e9 + 7;
    vector<int> dp(x + 1, 0); dp[0] = 1;
    for (int c : coins) {
        for (int i = c; i <= x; i++) {
            dp[i] = (dp[i] + dp[i - c]) % MOD;
        }
    }
    return dp[x];
}
```

---

### 5. Removing Digits
* **Problem Statement**: Given an integer $N$, count the minimum number of steps to reduce $N$ to $0$. In each step, you can subtract any single non-zero digit present in the current number.
* **DP State**: $dp[i]$ = min steps to reduce $i$ to $0$.
* **Tabulation Transition**: $dp[i] = 1 + \min_{d \in \text{digits}(i)} dp[i - d]$. Base Case: $dp[0] = 0$.
* **Complexities**: $O(N \cdot \text{digits})$ Time | $O(N)$ Space

```cpp
int removingDigits(int n) {
    vector<int> dp(n + 1, 1e9);
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        int temp = i;
        while (temp > 0) {
            int d = temp % 10;
            temp /= 10;
            if (d > 0) dp[i] = min(dp[i], dp[i - d] + 1);
        }
    }
    return dp[n];
}
```

---

### 6. Grid Paths I
* **Problem Statement**: Given an $N \times N$ grid where some cells contain obstacles (`'*'`), count the number of valid paths from top-left cell $(0, 0)$ to bottom-right cell $(N-1, N-1)$ moving only right or down. Answer modulo $10^9+7$.
* **DP State**: $dp[i][j]$ = number of paths from $(0, 0) \to (i, j)$.
* **Tabulation Transition**: $dp[i][j] = (dp[i-1][j] + dp[i][j-1]) \pmod{10^9+7}$ if cell $(i, j) \neq \text{'*'}$.
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space

```cpp
int gridPaths(int n, const vector<string>& grid) {
    const int MOD = 1e9 + 7;
    if (grid[0][0] == '*' || grid[n - 1][n - 1] == '*') return 0;
    vector<vector<int>> dp(n, vector<int>(n, 0));
    dp[0][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '*') continue;
            if (i > 0) dp[i][j] = (dp[i][j] + dp[i - 1][j]) % MOD;
            if (j > 0) dp[i][j] = (dp[i][j] + dp[i][j - 1]) % MOD;
        }
    }
    return dp[n - 1][n - 1];
}
```

---

### 7. Book Shop (0/1 Knapsack)
* **Problem Statement**: Given $N$ books, where book $i$ has cost $\text{price}[i]$ and page count $\text{pages}[i]$, find the maximum number of pages you can buy with a budget of $X$. Each book can be bought at most once.
* **DP State**: $dp[w]$ = max pages obtained using budget $w$.
* **Tabulation Transition**: Space-optimized 1D DP iterating budget **backward**:
  $$dp[w] = \max(dp[w], dp[w - \text{price}[i]] + \text{pages}[i])$$
* **Complexities**: $O(N \cdot X)$ Time | $O(X)$ Space

```cpp
int bookShop(int n, int x, const vector<int>& price, const vector<int>& pages) {
    vector<int> dp(x + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int w = x; w >= price[i]; w--) {
            dp[w] = max(dp[w], dp[w - price[i]] + pages[i]);
        }
    }
    return dp[x];
}
```

---

### 8. Array Description
* **Problem Statement**: Given an array of length $N$ with values in range $[1, M]$ where some elements are $0$ (unknown). Adjacent elements can differ by at most $1$. Count the number of valid arrays modulo $10^9+7$.
* **DP State**: $dp[i][v]$ = number of valid prefixes of length $i$ with value $v$ at position $i$.
* **Tabulation Transition**: $dp[i][v] = (dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]) \pmod{10^9+7}$.
* **Complexities**: $O(N \cdot M)$ Time | $O(M)$ Space

```cpp
int arrayDescription(int n, int m, const vector<int>& a) {
    const int MOD = 1e9 + 7;
    vector<int> dp(m + 2, 0);
    if (a[0] == 0) { for (int v = 1; v <= m; v++) dp[v] = 1; }
    else { dp[a[0]] = 1; }

    for (int i = 1; i < n; i++) {
        vector<int> nextDp(m + 2, 0);
        if (a[i] == 0) {
            for (int v = 1; v <= m; v++) {
                nextDp[v] = ((long long)dp[v - 1] + dp[v] + dp[v + 1]) % MOD;
            }
        } else {
            int v = a[i];
            nextDp[v] = ((long long)dp[v - 1] + dp[v] + dp[v + 1]) % MOD;
        }
        dp = nextDp;
    }
    long long ans = 0;
    for (int v = 1; v <= m; v++) ans = (ans + dp[v]) % MOD;
    return ans;
}
```

---

### 9. Counting Numbers (Digit DP Tabulation)
* **Problem Statement**: Count the number of integers in the range $[A, B]$ such that no two adjacent digits are equal.
* **DP State**: `dp[pos][prev_digit][started][tight]`
* **Complexities**: $O(\log_{10} B)$ Time | $O(1)$ Space

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
                    if (ways == 0) continue;
                    int maxD = tight ? limit : 9;
                    
                    // Option 1: Place leading zero
                    if (!started) dp[pos + 1][10][0][tight && (0 == maxD)] += ways;
                    
                    // Option 2: Place digit d
                    for (int d = (started ? 0 : 1); d <= maxD; d++) {
                        if (started && d == prev) continue; // Skip adjacent duplicate
                        dp[pos + 1][d][1][tight && (d == maxD)] += ways;
                    }
                }
            }
        }
    }
    long long total = 0;
    for (int d = 0; d <= 10; d++)
        for (int tight = 0; tight <= 1; tight++) total += dp[len][d][1][tight];
    return total + 1; // +1 for 0
}
```

---

### 10. Edit Distance
* **Problem Statement**: Given two strings $S_1$ and $S_2$, find the minimum number of operations (insert, delete, replace) to transform $S_1$ into $S_2$.
* **DP State**: $dp[i][j]$ = min edit distance between prefix $S_1[0 \dots i-1]$ and $S_2[0 \dots j-1]$.
* **Tabulation Transition**:
  $$dp[i][j] = \begin{cases} dp[i-1][j-1] & \text{if } S_1[i-1] == S_2[j-1] \\ 1 + \min(dp[i-1][j], \; dp[i][j-1], \; dp[i-1][j-1]) & \text{otherwise} \end{cases}$$
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
int editDistance(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }
    return dp[n][m];
}
```

---

### 11. Rectangle Cutting
* **Problem Statement**: Given an $A \times B$ rectangle, find the minimum number of straight cuts to divide the rectangle into squares.
* **DP State**: $dp[w][h]$ = min cuts required for rectangle of dimensions $w \times h$.
* **Tabulation Transition**:
  $$dp[w][h] = 1 + \min \begin{cases} \min_{1 \le k < w} \Big( dp[w-k][h] + dp[k][h] \Big) \\ \min_{1 \le k < h} \Big( dp[w][h-k] + dp[w][k] \Big) \end{cases}$$
* **Complexities**: $O(A \cdot B \cdot (A+B))$ Time | $O(A \cdot B)$ Space

```cpp
int rectangleCutting(int a, int b) {
    vector<vector<int>> dp(a + 1, vector<int>(b + 1, 1e9));
    for (int i = 1; i <= a; i++) {
        for (int j = 1; j <= b; j++) {
            if (i == j) { dp[i][j] = 0; continue; }
            for (int k = 1; k < i; k++) dp[i][j] = min(dp[i][j], 1 + dp[k][j] + dp[i - k][j]);
            for (int k = 1; k < j; k++) dp[i][j] = min(dp[i][j], 1 + dp[i][k] + dp[i][j - k]);
        }
    }
    return dp[a][b];
}
```

---

### 12. Money Sums
* **Problem Statement**: Given $N$ coin values, find all possible distinct total sums that can be produced using a subset of coins. Output the number of sums and all sums in ascending order.
* **DP State**: 1D boolean array $dp[s]$ = true if sum $s$ is reachable.
* **Tabulation Transition**: Outer loop coins $c$, inner loop sum $s$ **descending**:
  $$dp[s] = dp[s] \lor dp[s - c]$$
* **Complexities**: $O(N \cdot \sum C)$ Time | $O(\sum C)$ Space

```cpp
vector<int> moneySums(int n, const vector<int>& coins) {
    int maxSum = accumulate(coins.begin(), coins.end(), 0);
    vector<bool> dp(maxSum + 1, false); dp[0] = true;
    for (int c : coins) {
        for (int s = maxSum; s >= c; s--) {
            if (dp[s - c]) dp[s] = true;
        }
    }
    vector<int> result;
    for (int s = 1; s <= maxSum; s++) if (dp[s]) result.push_back(s);
    return result;
}
```

---

### 13. Removal Game (Interval DP / Stone Game)
* **Problem Statement**: Two players play a game with an array of $N$ positive numbers. Players take turns removing either the first or last remaining element and adding it to their score. Calculate the maximum score Player 1 can achieve assuming optimal play.
* **DP State**: $dp[l][r]$ = max net score difference (Player 1 - Player 2) on subarray $[l, r]$.
* **Tabulation Transition**: Outer length $len = 1 \dots N$, inner left boundary $l$:
  $$dp[l][r] = \max \Big( A[l] - dp[l+1][r], \; A[r] - dp[l][r-1] \Big)$$
  $$\text{Player 1 Score} = \frac{\sum A + dp[0][N-1]}{2}$$
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space

```cpp
long long removalGame(int n, const vector<long long>& a) {
    long long totalSum = accumulate(a.begin(), a.end(), 0LL);
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) dp[i][i] = a[i];

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l <= n - len; l++) {
            int r = l + len - 1;
            dp[l][r] = max(a[l] - dp[l + 1][r], a[r] - dp[l][r - 1]);
        }
    }
    return (totalSum + dp[0][n - 1]) / 2;
}
```

---

### 14. Two Sets II
* **Problem Statement**: Count the number of ways to divide the numbers $1 \dots N$ into two subsets of equal sum modulo $10^9+7$.
* **Algorithm**: Total sum $S = \frac{N(N+1)}{2}$. If $S$ is odd $\implies 0$. Otherwise, run 0/1 Knapsack to find subsets summing to $S/2$, and multiply final answer by modular inverse of $2$ ($\text{inv}(2) = \frac{\text{MOD}+1}{2}$).
* **Complexities**: $O(N \cdot S)$ Time | $O(S)$ Space

```cpp
int twoSetsII(int n) {
    const int MOD = 1e9 + 7;
    int sum = n * (n + 1) / 2;
    if (sum % 2 != 0) return 0;
    int target = sum / 2;
    vector<int> dp(target + 1, 0); dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int w = target; w >= i; w--) {
            dp[w] = (dp[w] + dp[w - i]) % MOD;
        }
    }
    long long inv2 = (MOD + 1) / 2;
    return (dp[target] * inv2) % MOD;
}
```

---

### 15. Increasing Subsequence (LIS $O(N \log N)$)
* **Problem Statement**: Given an array of $N$ integers, find the length of the longest strictly increasing subsequence.
* **Algorithm**: Patient sorting maintaining `vector<int> tails` updated with `lower_bound`.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

```cpp
int longestIncreasingSubsequence(int n, const vector<int>& a) {
    vector<int> tails;
    for (int x : a) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
```

---

### 16. Increasing Subsequence II
* **Problem Statement**: Count the total number of strictly increasing subsequences of an array modulo $10^9+7$.
* **Algorithm**: Coordinate compress values. Maintain Fenwick tree over values: $\text{dp}[x] = 1 + \text{query}(x - 1)$, then $\text{update}(x, \text{dp}[x])$.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

```cpp
struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void update(int i, long long val) { for (; i <= n; i += i & -i) bit[i] = (bit[i] + val) % 1000000007; }
    long long query(int i) { long long s = 0; for (; i > 0; i -= i & -i) s = (s + bit[i]) % 1000000007; return s; }
};
int countIncreasingSubsequences(int n, vector<int>& a) {
    const int MOD = 1e9 + 7;
    vector<int> vals = a; sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    Fenwick ft(vals.size()); long long total = 0;
    for (int x : a) {
        int idx = lower_bound(vals.begin(), vals.end(), x) - vals.begin() + 1;
        long long ways = (1 + ft.query(idx - 1)) % MOD;
        ft.update(idx, ways);
        total = (total + ways) % MOD;
    }
    return total;
}
```

---

### 17. Longest Common Subsequence (LCS)
* **Problem Statement**: Given two strings $S_1$ and $S_2$, find the length of their longest common subsequence.
* **DP State**: $dp[i][j]$ = LCS length of prefixes $S_1[0 \dots i-1]$ and $S_2[0 \dots j-1]$.
* **Tabulation Transition**:
  $$dp[i][j] = \begin{cases} 1 + dp[i-1][j-1] & \text{if } S_1[i-1] == S_2[j-1] \\ \max(dp[i-1][j], \; dp[i][j-1]) & \text{otherwise} \end{cases}$$
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
int longestCommonSubsequence(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[n][m];
}
```

---

### 18. Projects (Weighted Job Scheduling)
* **Problem Statement**: Given $N$ projects, where project $i$ has start day $a_i$, end day $b_i$, and reward $p_i$, find the maximum reward you can earn by taking a set of non-overlapping projects.
* **Algorithm**: Sort projects by `end_time`. Maintain $dp[i]$ = max reward taking a subset from first $i$ projects. Use `upper_bound` binary search to find last project finishing before current project's `start_time`.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

```cpp
struct Project { int start, end, reward; };
long long maxProjectReward(int n, vector<Project>& projects) {
    sort(projects.begin(), projects.end(), [](const Project& a, const Project& b) {
        return a.end < b.end;
    });
    vector<long long> dp(n + 1, 0);
    vector<int> endTimes(n);
    for (int i = 0; i < n; i++) endTimes[i] = projects[i].end;

    for (int i = 1; i <= n; i++) {
        // Find last project ending strictly before current project's start time
        int p = upper_bound(endTimes.begin(), endTimes.begin() + i - 1, projects[i - 1].start - 1) - endTimes.begin();
        dp[i] = max(dp[i - 1], dp[p] + projects[i - 1].reward);
    }
    return dp[n];
}
```

---

### 19. Elevator Rides (Bitmask DP Tabulation)
* **Problem Statement**: There are $N$ people ($N \le 20$) with weights $W_1 \dots W_N$ wanting to get to the top floor of a building. An elevator has maximum capacity $X$. Find the minimum number of elevator rides needed to transfer all $N$ people.
* **DP State**: $dp[mask] = \{\text{rides}, \text{current\_weight}\}$
* **Tabulation Transition**: Iterate submask $mask = 0 \dots 2^N - 1$. For person $i \notin mask$:
  - If $\text{current\_weight} + W_i \le X \implies \{\text{rides}, \text{current\_weight} + W_i\}$
  - Else $\implies \{\text{rides} + 1, W_i\}$
* **Complexities**: $O(N \cdot 2^N)$ Time | $O(2^N)$ Space

```cpp
int minElevatorRides(int n, int x, const vector<int>& w) {
    vector<pair<int, int>> dp(1 << n, {n + 1, 0});
    dp[0] = {1, 0};

    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) continue;
            auto prev = dp[mask];
            pair<int, int> nextState;
            if (prev.second + w[i] <= x) {
                nextState = {prev.first, prev.second + w[i]};
            } else {
                nextState = {prev.first + 1, w[i]};
            }
            dp[mask | (1 << i)] = min(dp[mask | (1 << i)], nextState);
        }
    }
    return dp[(1 << n) - 1].first;
}
```

---

### 20. Minimal Grid Path (Lexicographical Grid DP)
* **Problem Statement**: Given an $N \times N$ grid of lowercase characters, find the lexicographically smallest string formed by walking from top-left cell $(0, 0)$ to bottom-right cell $(N-1, N-1)$ moving only right or down.
* **DP State**: $dp[i][j]$ = lexicographically smallest string from $(0, 0) \to (i, j)$.
* **Tabulation Transition**: $dp[i][j] = \min(dp[i-1][j], dp[i][j-1]) + \text{grid}[i][j]$.
* **Complexities**: $O(N^2)$ Time | $O(N^2)$ Space

```cpp
string minimalGridPath(int n, const vector<string>& grid) {
    vector<vector<string>> dp(n, vector<string>(n, ""));
    dp[0][0] = string(1, grid[0][0]);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) continue;
            string best = "";
            if (i > 0 && j > 0) best = min(dp[i - 1][j], dp[i][j - 1]);
            else if (i > 0) best = dp[i - 1][j];
            else best = dp[i][j - 1];
            dp[i][j] = best + grid[i][j];
        }
    }
    return dp[n - 1][n - 1];
}
```

---
*End of CSES Dynamic Programming Tabulation Master Reference*
