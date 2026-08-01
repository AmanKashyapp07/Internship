# 🧠 Master Dynamic Programming Patterns Guide (Interviews & OAs)

> **The ultimate cheat sheet for recognizing, formulating, and coding Dynamic Programming problems in tech interviews (FAANG, Top Tech) and Online Assessments (OAs).**

---

## 📋 Table of Contents
1. [The 5-Step DP Problem-Solving Framework](#1-the-5-step-dp-problem-solving-framework)
2. [Pattern 1: 0/1 Knapsack & Subset Sum Variations](#pattern-1-01-knapsack--subset-sum-variations)
3. [Pattern 2: Unbounded Knapsack & Coin Change](#pattern-2-unbounded-knapsack--coin-change)
4. [Pattern 3: Longest Common Subsequence (LCS) & String Alignment](#pattern-3-longest-common-subsequence-lcs--string-alignment)
5. [Pattern 4: Longest Increasing Subsequence (LIS) & Binary Search ($O(N \log N)$)](#pattern-4-longest-increasing-subsequence-lis--binary-search-on-log-n)
6. [Pattern 5: State Machine DP (Stock Trading & House Robber)](#pattern-5-state-machine-dp-stock-trading--house-robber)
7. [Pattern 6: Grid DP & 2-Path Synchronous DP](#pattern-6-grid-dp--2-path-synchronous-dp)
8. [Pattern 7: Interval DP (Range DP & Merging Subarrays)](#pattern-7-interval-dp-range-dp--merging-subarrays)
9. [Pattern 8: Tree DP & Tree Rerouting (2-Pass DFS)](#pattern-8-tree-dp--tree-rerouting-2-pass-dfs)
10. [Pattern 9: Bitmask DP (Subset Traversal & TSP)](#pattern-9-bitmask-dp-subset-traversal--tsp)
11. [Pattern 10: Digit DP ($N \le 10^{18}$)](#pattern-10-digit-dp-n-le-1018)
12. [Pattern 11: Advanced OA Optimization Techniques](#pattern-11-advanced-oa-optimization-techniques)
13. [Master DP Pattern Decision Matrix](#master-dp-pattern-decision-matrix)

---

## 1. The 5-Step DP Problem-Solving Framework

When confronted with a DP problem in an OA or interview, follow this systematic 5-step process:

```
┌────────────────────────┐    ┌────────────────────────┐    ┌────────────────────────┐
│ 1. Identify DP Signals │ ──►│ 2. Define DP State     │ ──►│ 3. Formulate Transition│
└────────────────────────┘    └────────────────────────┘    └────────────────────────┘
                                                                         │
┌────────────────────────┐    ┌────────────────────────┐                 ▼
│ 5. Space Optimization  │ ◄──│ 4. Base Cases & Order  │ ◄───────────────┘
└────────────────────────┘    └────────────────────────┘
```

1. **Recognize Signals**: Optimal choices + Overlapping choices (Min/Max, Total Ways, Feasibility).
2. **Define State ($dp[\dots]$)**: What parameters uniquely identify a subproblem? (e.g., index `i`, remaining target `w`, state flags).
3. **Formulate Transition Equation**: Express $dp[i]$ in terms of smaller solved subproblems (e.g., $dp[i] = \max(\text{take}, \text{skip})$).
4. **Identify Base Cases & Iteration Order**: Set boundary conditions ($dp[0]$, empty inputs) and fill order (left-to-right, right-to-left, length-wise).
5. **Optimize Space Complexity**: Can you reduce 2D $O(N \times W)$ to 1D $O(W)$ or 2 variables?

---

## Pattern 1: 0/1 Knapsack & Subset Sum Variations

### 🔍 Identification Signals
- Given a set of items, each item can be **chosen AT MOST ONCE**.
- Goal: Maximize/minimize total value or count total ways to reach target sum $W$.
- Constraints: $N \le 10^3, W \le 10^4$.

### 💡 Core Mechanics & Space Optimization Trick
* **2D DP**: $dp[i][w] = \max(dp[i-1][w], \, dp[i-1][w - wt[i-1]] + val[i-1])$
* **1D Space Optimization**: Iterate target capacity $w$ **BACKWARDS** (from $W$ down to $wt[i]$) so we use values from the *previous* item iteration, preventing multiple uses of the same item!

```
2D Matrix:   dp[i][w] depends on dp[i-1][w - wt[i]] (top-left)
1D Array:    Iterate w from W DOWN TO wt[i] (backward pass preserves previous state)
```

### 💻 Standard Templates

#### Subset Sum / Target Sum Equal Partition (0/1 Knapsack)
```cpp
// Returns true if there exists a subset with sum target
bool subsetSum(const vector<int>& nums, int target) {
    vector<bool> dp(target + 1, false);
    dp[0] = true; // Base case: 0 target is always possible (empty set)
    
    for (int num : nums) {
        // MUST iterate backwards for 0/1 knapsack!
        for (int j = target; j >= num; j--) {
            if (dp[j - num]) {
                dp[j] = true;
            }
        }
    }
    return dp[target];
}
```

#### Count Subsets with Given Difference / Target Sum (`+` and `-` signs)
```cpp
// Count ways to assign +/- to nums to achieve target sum
// Equation: Positive_Sum - Negative_Sum = target  => Positive_Sum = (Total_Sum + target) / 2
int findTargetSumWays(const vector<int>& nums, int target) {
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum < abs(target) || (sum + target) % 2 != 0) return 0;
    
    int P = (sum + target) / 2;
    vector<int> dp(P + 1, 0);
    dp[0] = 1;
    
    for (int num : nums) {
        for (int j = P; j >= num; j--) {
            dp[j] += dp[j - num];
        }
    }
    return dp[P];
}
```

---

## Pattern 2: Unbounded Knapsack & Coin Change

### 🔍 Identification Signals
- Each item / coin can be **REUSED INFINITELY MANY TIMES**.
- Single target amount/weight $X$.

### ⚡ Crucial OA Distinction: Permutations vs. Combinations Loop Order
* **Combinations** (Order does **NOT** matter, e.g. $\{1,2\}$ and $\{2,1\}$ are same):
  - **Outer Loop = Items**, **Inner Loop = Target Sum** (Iterate sum FORWARD).
* **Permutations** (Order **DOES** matter, e.g. $\{1,2\}$ is different from $\{2,1\}$):
  - **Outer Loop = Target Sum**, **Inner Loop = Items**.

```cpp
// 1. COMBINATIONS: Outer = Coins, Inner = Target Sum
int coinCombinations_II(const vector<int>& coins, int target) {
    vector<int> dp(target + 1, 0);
    dp[0] = 1;
    for (int c : coins) {
        for (int j = c; j <= target; j++) { // FORWARD iteration = Unbounded
            dp[j] = (dp[j] + dp[j - c]);
        }
    }
    return dp[target];
}

// 2. PERMUTATIONS: Outer = Target Sum, Inner = Coins
int coinCombinations_I(const vector<int>& coins, int target) {
    vector<int> dp(target + 1, 0);
    dp[0] = 1;
    for (int j = 1; j <= target; j++) {
        for (int c : coins) {
            if (j - c >= 0) dp[j] = (dp[j] + dp[j - c]);
        }
    }
    return dp[target];
}

// 3. MINIMUM COINS NEEDED
int coinChangeMinCoins(const vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, 1e9);
    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        for (int c : coins) {
            if (i - c >= 0) dp[i] = min(dp[i], dp[i - c] + 1);
        }
    }
    return dp[amount] >= 1e9 ? -1 : dp[amount];
}
```

---

## Pattern 3: Longest Common Subsequence (LCS) & String Alignment

### 🔍 Identification Signals
- Given 2 strings $S_1$ (length $N$) and $S_2$ (length $M$).
- Requires comparison, editing, alignment, or matching.
- Time Complexity: $O(N \cdot M)$. Space Complexity: $O(N \cdot M) \rightarrow O(\min(N, M))$ space optimized.

### 💻 Standard Templates

#### 1. Longest Common Subsequence (LCS)
```cpp
string longestCommonSubsequence(string text1, string text2) {
    int n = text1.size(), m = text2.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Build DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (text1[i - 1] == text2[j - 1])
                dp[i][j] = 1 + dp[i - 1][j - 1];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    // Reconstruct LCS
    string lcs = "";
    int i = n, j = m;

    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs += text1[i - 1];
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        }
        else {
            j--;
        }
    }

    reverse(lcs.begin(), lcs.end());
    return lcs;
}
```

#### 2. Edit Distance (Levenshtein Distance)
```cpp
int minDistance(string word1, string word2) {
    int n = word1.size(), m = word2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    for (int i = 0; i <= n; i++) dp[i][0] = i; // Deletions
    for (int j = 0; j <= m; j++) dp[0][j] = j; // Insertions
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // Match (no edit cost)
            } else {
                dp[i][j] = 1 + min({
                    dp[i - 1][j],     // Delete
                    dp[i][j - 1],     // Insert
                    dp[i - 1][j - 1]  // Replace
                });
            }
        }
    }
    return dp[n][m];
}
```

#### 3. Wildcard / Regular Expression Matching
- `dp[i][j]`: matches prefix `S[0..i-1]` with pattern `P[0..j-1]`.
- For `*` in Wildcard: $dp[i][j] = dp[i-1][j] \text{ (match 1+ chars)} \mid dp[i][j-1] \text{ (match 0 chars)}$.
- For `*` in Regex: $dp[i][j] = dp[i][j-2] \mid ((S[i-1] == P[j-2] \mid P[j-2] == '.') \ \&\& \ dp[i-1][j])$.
```cpp
class Solution {
public:
    bool solve(int i, int j, string &s, string &p,
               vector<vector<int>> &dp) {

        // Pattern finished
        if (j == p.size())
            return i == s.size();

        // String finished
        if (i == s.size()) {
            while (j < p.size()) {
                if (p[j] != '*')
                    return false;
                j++;
            }
            return true;
        }

        if (dp[i][j] != -1)
            return dp[i][j];

        if (p[j] == s[i] || p[j] == '?')
            return dp[i][j] = solve(i + 1, j + 1, s, p, dp);

        if (p[j] == '*')
            return dp[i][j] =
                solve(i, j + 1, s, p, dp) ||   // '*' matches empty
                solve(i + 1, j, s, p, dp);     // '*' matches one/more chars

        return dp[i][j] = false;
    }

    bool isMatch(string s, string p) {
        int n = s.size(), m = p.size();

        vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));

        return solve(0, 0, s, p, dp);
    }
};
```

---

## Pattern 4: Longest Increasing Subsequence (LIS) & Binary Search ($O(N \log N)$)

### 🔍 Identification Signals
- Finding maximum length of increasing / non-decreasing subsequence.
- $N$ up to $10^5 \implies O(N^2)$ DP will TLE! Must use $O(N \log N)$ binary search algorithm (Patience Sorting).

### 💻 Standard Templates

#### $O(N \log N)$ LIS (Patient Sorting via `std::lower_bound`)
```cpp
vector<int> longestIncreasingSubsequence(vector<int>& nums) {
    int n = nums.size();

    vector<int> tailIndex;
    vector<int> parent(n, -1);

    for (int i = 0; i < n; i++) {
        // Find position where nums[i] should go
        auto it = lower_bound(
            tailIndex.begin(),
            tailIndex.end(),
            nums[i],
            [&](int idx, int val) {
                return nums[idx] < val;
            });

        int pos = it - tailIndex.begin();

        if (it == tailIndex.end())
            tailIndex.push_back(i);
        else
            *it = i;

        // Set parent
        if (pos > 0)
            parent[i] = tailIndex[pos - 1];
    }

    // Reconstruct LIS
    vector<int> lis;
    int cur = tailIndex.back();

    while (cur != -1) {
        lis.push_back(nums[cur]);
        cur = parent[cur];
    }

    reverse(lis.begin(), lis.end());

    return lis;
}
```

#### Russian Doll Envelopes (2D LIS Trick)
- Envelopes $(w_i, h_i)$. Envelope $A$ fits inside $B$ if $w_A < w_B$ and $h_A < h_B$.
- **Trick**: Sort width **ascending**, but height **descending** for equal widths! Then run 1D LIS on heights.
- Why height descending for equal widths? Prevents picking two envelopes with the same width!

```cpp
int maxEnvelopes(vector<vector<int>>& envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] == b[0]) return a[1] > b[1]; // Descending height on equal width
        return a[0] < b[0]; // Ascending width
    });
    
    vector<int> tails;
    for (const auto& env : envelopes) {
        int h = env[1];
        auto it = lower_bound(tails.begin(), tails.end(), h);
        if (it == tails.end()) tails.push_back(h);
        else *it = h;
    }
    return tails.size();
}
```

---

## Pattern 5: State Machine DP (Stock Trading & House Robber)

### 🔍 Identification Signals
- At step $i$, you can transition between explicit **finite states** (e.g., `HOLD`, `EMPTY`, `COOLDOWN`, `PAINT_RED`).
- Choices on day $i$ depend strictly on your state on day $i-1$.

```
         ┌───────────────────┐
         │     0: UNHEAVY    │
         └─────────┬─────────┘
          buy      │      ▲ sell
          stock    ▼      │
         ┌───────────────────┐
         │      1: HELD      │
         └───────────────────┘
```

### 💻 Standard Templates

#### Best Time to Buy and Sell Stock with Cooldown
- 3 States:
  - `hold[i]`: Max profit on day $i$ holding a stock.
  - `sold[i]`: Max profit on day $i$ having just sold a stock today.
  - `rest[i]`: Max profit on day $i$ in cooldown / empty state.

```cpp
int maxProfitWithCooldown(const vector<int>& prices) {
    if (prices.empty()) return 0;
    
    int hold = -prices[0];
    int sold = 0;
    int rest = 0;
    
    for (int i = 1; i < prices.size(); i++) {
        int prev_hold = hold;
        int prev_sold = sold;
        int prev_rest = rest;
        
        hold = max(prev_hold, prev_rest - prices[i]); // Buy from rest state
        sold = prev_hold + prices[i];                 // Sell stock
        rest = max(prev_rest, prev_sold);             // Cooldown after sell or stay rest
    }
    return max(sold, rest);
}
```

#### House Robber II (Circular Array Trick)
- Houses are in a circle (House 0 and House $N-1$ are adjacent).
- **Trick**: Run Standard House Robber twice!
  1. Range $[0 \dots N-2]$ (Force excluding last house)
  2. Range $[1 \dots N-1]$ (Force excluding first house)
  - Result = $\max(\text{Rob}(0 \dots N-2), \, \text{Rob}(1 \dots N-1))$.

```cpp
int robLinear(const vector<int>& nums, int start, int end) {
    int prev2 = 0, prev1 = 0;
    for (int i = start; i <= end; i++) {
        int curr = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int robCircular(vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
}
```

---

## Pattern 6: Grid DP & 2-Path Synchronous DP

### 🔍 Identification Signals
- Moving on a 2D matrix $(R \times C)$ with constrained moves (e.g., Down, Right).
- **2-Path DP** (e.g., Cherry Pickup, 2 Grid Paths simultaneously): $dp[r_1][c_1][r_2]$ where $c_2 = r_1 + c_1 - r_2$ (since total steps $r_1+c_1 = r_2+c_2$).

### 💻 Standard Templates

#### Dungeon Game (Bottom-Right to Top-Left DP)
- In Dungeon Game, standard Top-Left DP fails because future health requirements depend on remaining path.
- **Solution**: Compute $dp[i][j]$ representing **minimum initial health needed before entering cell $(i, j)$**.

```cpp
int calculateMinimumHP(vector<vector<int>>& dungeon) {
    int r = dungeon.size(), c = dungeon[0].size();
    vector<vector<int>> dp(r + 1, vector<int>(c + 1, 1e9));
    
    dp[r][c - 1] = 1;
    dp[r - 1][c] = 1;
    
    for (int i = r - 1; i >= 0; i--) {
        for (int j = c - 1; j >= 0; j--) {
            int min_health_needed = min(dp[i + 1][j], dp[i][j + 1]) - dungeon[i][j];
            dp[i][j] = max(1, min_health_needed); // Health can never drop <= 0
        }
    }
    return dp[0][0];
}
```

#### Cherry Pickup (2 Synchronous Walkers DP)
- Two people walk from $(0,0)$ to $(N-1, N-1)$ picking cherries.
- Equivalent to: **Two people starting at $(0,0)$ walking to $(N-1, N-1)$ together**.
- State: $dp[r_1][c_1][r_2]$ (where $c_2 = r_1 + c_1 - r_2$).

---

## Pattern 7: Interval DP (Range DP & Merging Subarrays)

### 🔍 Identification Signals
- Subproblems are defined over subsegments/ranges $[i \dots j]$ of an array or string.
- You can split range $[i \dots j]$ at any partition $k \in [i, j-1]$ into $[i \dots k]$ and $[k+1 \dots j]$.
- Complexity: $O(N^3)$ Time, $O(N^2)$ Space. Loop over **length $L$ from 1 to $N$**.

```cpp
// Standard Range DP Nested Loop Structure
for (int L = 2; L <= n; L++) {         // Length of segment
    for (int i = 0; i <= n - L; i++) { // Left endpoint
        int j = i + L - 1;             // Right endpoint
        dp[i][j] = INF;
        for (int k = i; k < j; k++) {   // Partition point
            dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + cost(i, k, j));
        }
    }
}
```

### 💻 Standard Templates

#### Burst Balloons ($O(N^3)$ Interval DP)
- **Key Insight**: Instead of picking balloon to burst *first*, pick the balloon $k$ to burst **LAST** in range $[i \dots j]$!

```cpp
int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> val(n + 2, 1);
    for (int i = 0; i < n; i++) val[i + 1] = nums[i];
    
    int sz = n + 2;
    vector<vector<int>> dp(sz, vector<int>(sz, 0));
    
    // L is length of subsegment
    for (int L = 1; L <= n; L++) {
        for (int i = 1; i <= n - L + 1; i++) {
            int j = i + L - 1;
            for (int k = i; k <= j; k++) { // k is LAST balloon popped in range [i..j]
                int coins = val[i - 1] * val[k] * val[j + 1];
                dp[i][j] = max(dp[i][j], dp[i][k - 1] + dp[k + 1][j] + coins);
            }
        }
    }
    return dp[1][n];
}
```

---

## Pattern 8: Tree DP & Tree Rerouting (2-Pass DFS)

### 🔍 Identification Signals
- Problem on Tree structure ($N$ nodes, $N-1$ edges).
- **Subtree DP**: Post-order traversal (children computed before parent).
- **Tree Rerouting**: Calculate answer for ALL nodes as root in $O(N)$ time instead of naive $O(N^2)$.

### 💻 Standard Templates

#### 1. House Robber III (Tree Subtree DP)
```cpp
struct TreeNode {
    int val;
    TreeNode *left, *right;
};

// Returns pair<int, int>: {max money if rob current node, max money if DO NOT rob current node}
pair<int, int> dfsRob(TreeNode* root) {
    if (!root) return {0, 0};
    
    auto left = dfsRob(root->left);
    auto right = dfsRob(root->right);
    
    // Option 1: Rob root -> cannot rob direct children
    int robRoot = root->val + left.second + right.second;
    
    // Option 2: Skip root -> can choose to rob or skip children
    int skipRoot = max(left.first, left.second) + max(right.first, right.second);
    
    return {robRoot, skipRoot};
}

int rob(TreeNode* root) {
    auto res = dfsRob(root);
    return max(res.first, res.second);
}
```

#### 2. Tree Rerouting Technique (Sum of Distances in Tree $O(N)$)
- **Pass 1 (Bottom-Up DFS)**: Compute `count[u]` (size of subtree) and `ans[0]` (sum of distances from root node 0).
- **Pass 2 (Top-Down DFS)**: Reroot from parent `u` to child `v`:
$$\text{ans}[v] = \text{ans}[u] - \text{count}[v] + (N - \text{count}[v])$$

```cpp
void dfs1(int u, int p, vector<vector<int>>& adj, vector<int>& count, vector<int>& ans) {
    count[u] = 1;
    ans[u] = 0;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs1(v, u, adj, count, ans);
        count[u] += count[v];
        ans[u] += ans[v] + count[v];
    }
}

void dfs2(int u, int p, int n, vector<vector<int>>& adj, vector<int>& count, vector<int>& ans) {
    for (int v : adj[u]) {
        if (v == p) continue;
        // Rerooting transition formula
        ans[v] = ans[u] - count[v] + (n - count[v]);
        dfs2(v, u, n, adj, count, ans);
    }
}
```

---

## Pattern 9: Bitmask DP (Subset Traversal & TSP)

### 🔍 Identification Signals
- Small limits ($N \le 20$).
- State requires tracking which subset of items has been used/visited.
- Mask: $i$-th bit is `1` if item $i$ is included/visited, `0` otherwise. Total states $= 2^N$.

### 💻 Standard Templates

#### Traveling Salesperson Problem (TSP) / Shortest Hamiltonian Path
- $dp[\text{mask}][u]$: min cost to visit all nodes in `mask`, ending at node `u`.

```cpp
int tsp(int n, const vector<vector<int>>& dist) {
    int total_masks = 1 << n;
    vector<vector<int>> dp(total_masks, vector<int>(n, 1e9));
    
    dp[1][0] = 0; // Base case: Starting at node 0 with mask (000...001)
    
    for (int mask = 1; mask < total_masks; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue; // u must be in current mask
            if (dp[mask][u] == 1e9) continue;
            
            for (int v = 0; v < n; v++) {
                if (!(mask & (1 << v))) { // Move to unvisited node v
                    int next_mask = mask | (1 << v);
                    dp[next_mask][v] = min(dp[next_mask][v], dp[mask][u] + dist[u][v]);
                }
            }
        }
    }
    
    int ans = 1e9;
    int final_mask = (1 << n) - 1;
    for (int u = 1; u < n; u++) {
        ans = min(ans, dp[final_mask][u] + dist[u][0]); // Return to start
    }
    return ans;
}
```

#### Submask Enumeration Trick ($O(3^N)$ Submask DP)
Iterate all submasks of a bitmask efficiently:
```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
        // submask is a valid subset of mask!
    }
}
```

---

## Pattern 10: Digit DP ($N \le 10^{18}$)

### 🔍 Identification Signals
- Count numbers in range $[A, B]$ satisfying digit constraints (e.g. no adjacent repeated digits, digit sum divisible by $K$).
- Inputs given as large bounds up to $10^{18}$ (represented as strings/long long).
- Complexity: $O(\text{Length of number} \times \text{State Space})$.

### 💻 Standard Digit DP Template
```cpp
long long memo[20][2][2][100]; // pos, tight, leading_zero, custom_state
string S;

long long solveDigit(int pos, bool tight, bool leading_zero, int custom_state) {
    if (pos == S.size()) {
        return 1; // Valid number constructed
    }
    
    if (memo[pos][tight][leading_zero][custom_state] != -1)
        return memo[pos][tight][leading_zero][custom_state];
        
    int limit = tight ? (S[pos] - '0') : 9;
    long long ans = 0;
    
    for (int d = 0; d <= limit; d++) {
        bool next_tight = tight && (d == limit);
        bool next_leading_zero = leading_zero && (d == 0);
        
        // Check digit constraint here (e.g., skip if d violates condition)
        
        int next_state = custom_state + d; // example transition
        ans += solveDigit(pos + 1, next_tight, next_leading_zero, next_state);
    }
    
    return memo[pos][tight][leading_zero][custom_state] = ans;
}

long long countInRange(long long N) {
    if (N < 0) return 0;
    S = to_string(N);
    memset(memo, -1, sizeof(memo));
    return solveDigit(0, true, true, 0);
}
```

---

## Pattern 11: Advanced OA Optimization Techniques

In competitive programming and top-tier OAs (e.g., Arcesium, DE Shaw, Google, Codeforces), standard DP might TLE. Apply these optimizations:

### 1. Sliding Window / Monotone Queue DP Optimization
- **Trigger**: $dp[i] = \min_{i - k \le j < i} (dp[j]) + \text{cost}[i]$.
- **Optimization**: Use `std::deque` to maintain monotonic values in range of length $K$.
- **Time Complexity Reduction**: $O(N \cdot K) \longrightarrow O(N)$.

### 2. Matrix Exponentiation
- **Trigger**: Linear DP recurrences with $N \le 10^{18}$ and state size $K \le 100$ (e.g., $dp[i] = dp[i-1] + dp[i-2] + dp[i-3]$).
- **Formula**: Express transitions as matrix multiplication $V_n = M \times V_{n-1}$. Compute $M^N$ using binary exponentiation in $O(K^3 \log N)$.

### 3. Convex Hull Trick (CHT) / Li Chao Tree
- **Trigger**: DP state of form $dp[i] = \min_{j < i} (dp[j] + m_j \cdot x_i)$.
- **Optimization**: Insert linear equations $y = m_j \cdot x + c_j$ into a dynamic line container / Li Chao Tree.
- **Time Complexity Reduction**: $O(N^2) \longrightarrow O(N \log N)$ or $O(N)$.

---

## Master DP Pattern Decision Matrix

| Problem Feature / Keyword | Matching DP Pattern | Typical State Form | Transition / Trick | Time | Space |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Pick items once to reach weight $W$ | **0/1 Knapsack** | $dp[w]$ | Loop target $w$ **backwards** | $O(N \cdot W)$ | $O(W)$ |
| Unlimited item reuse, min coins | **Unbounded Knapsack** | $dp[w]$ | Loop target $w$ **forwards** | $O(N \cdot W)$ | $O(W)$ |
| Unlimited reuse, count combinations | **Coin Combinations II** | $dp[w]$ | Outer loop items, inner target | $O(N \cdot W)$ | $O(W)$ |
| Unlimited reuse, count permutations | **Coin Combinations I** | $dp[w]$ | Outer loop target, inner items | $O(N \cdot W)$ | $O(W)$ |
| Subsequence matching on 2 strings | **LCS / Edit Distance** | $dp[i][j]$ | Match vs Mismatch branches | $O(N \cdot M)$ | $O(\min(N,M))$ |
| Longest increasing sequence ($N \le 10^5$) | **LIS Binary Search** | `tails` array | `std::lower_bound` replacement | $O(N \log N)$ | $O(N)$ |
| Multiple states per step (Hold, Cooldown) | **State Machine DP** | `hold`, `sold`, `rest` | Explicit mutual transitions | $O(N)$ | $O(1)$ |
| 2D grid path matrix traversal | **Grid DP** | $dp[r][c]$ | $dp[r-1][c] + dp[r][c-1]$ | $O(R \cdot C)$ | $O(C)$ |
| Range merging $[i \dots j]$ / Burst Balloons | **Interval DP** | $dp[i][j]$ | Loop segment length $L = 1 \dots N$ | $O(N^3)$ | $O(N^2)$ |
| Tree nodes, pick parent vs child | **Tree Subtree DP** | `pair<int,int>` | Post-order DFS `{take, skip}` | $O(N)$ | $O(H)$ |
| Answers for ALL tree nodes as roots | **Tree Rerouting** | `ans[u]` | 2-pass DFS ($u \to v$ transition) | $O(N)$ | $O(N)$ |
| Subset selection with $N \le 20$ | **Bitmask DP** | $dp[\text{mask}][u]$ | Bitwise ops `mask \| (1 << v)` | $O(N^2 \cdot 2^N)$ | $O(N \cdot 2^N)$ |
| Count valid integers in $[A, B]$ | **Digit DP** | `memo[pos][tight][zero]` | DFS over digits $0 \dots 9$ | $O(\log N)$ | $O(\log N)$ |
