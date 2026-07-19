# Dynamic Programming: Master Interview & OA Guide

## How to Use This Guide

DP problems feel hard until you recognize the pattern. The workflow is:

1. Identify if the problem has **optimal substructure** (optimal answer built from optimal sub-answers) and **overlapping subproblems** (same subproblem solved multiple times).
2. Identify which of the 15 categories it belongs to.
3. Define the state clearly before writing any code.
4. Write the recurrence, then decide top-down (memo) or bottom-up (tabulation).

---

## Universal Problem-Solving Framework

Before writing code, answer these four questions:

**Q1: What is the state?**
The minimum information needed to describe a subproblem uniquely.
- Index in array -> 1D DP
- Two indices (two strings, or start/end) -> 2D DP
- (index, remaining capacity) -> Knapsack
- (index, set of used items) -> Bitmask DP
- (position, digit tight constraint) -> Digit DP

**Q2: What is the recurrence?**
How does the current state relate to smaller states?
Write it in plain English first: "dp[i] = best answer for the first i elements"

**Q3: What are the base cases?**
Empty input, single element, zero capacity.

**Q4: Top-down or bottom-up?**
- Top-down (memoization): easier to write, natural recursion, use when not all states are visited.
- Bottom-up (tabulation): faster in practice, no recursion overhead, easier to optimize space.

---

## 1. 1D DP

### When to Use
Linear sequence, each state depends on a fixed number of previous states.

### Signal Words
"ways to climb", "minimum cost", "rob houses", "decode", "coin change"

### Template

```cpp
class Solution {
public:
    int solve(int n, vector<int>& arr) {
        vector<int> dp(n + 1, 0);
        dp[0] = base_case; // replace with actual base case

        for (int i = 1; i <= n; i++) {
            dp[i] = some_function(dp[i-1], dp[i-2]); // look back fixed steps
        }

        return dp[n];
    }
};
```

### Key Problems

| Problem | Recurrence | Trick |
|---|---|---|
| 70. Climbing Stairs | dp[i] = dp[i-1] + dp[i-2] | Fibonacci pattern |
| 198. House Robber | dp[i] = max(dp[i-1], dp[i-2] + val[i]) | Skip or take |
| 213. House Robber II | Run 198 on [0..n-2] and [1..n-1], take max | Circular: exclude first or last |
| 322. Coin Change | dp[i] = min(dp[i - coin] + 1) for each coin | Unbounded knapsack variant |
| 91. Decode Ways | dp[i] += dp[i-1] if valid 1-digit; dp[i] += dp[i-2] if valid 2-digit | String indexing is off-by-one trap |
| 740. Delete and Earn | Convert to house robber on frequency array | Earn all of a value or none |

### Common Mistakes
- Off-by-one: decide if dp[i] means "up to index i inclusive" or "exactly at i".
- Coin Change: initialize dp with infinity except dp[0] = 0.

---

## 2. 2D DP (Grid DP)

### When to Use
Moving through a grid, two-index problems, triangle paths.

### Signal Words
"unique paths", "minimum path", "grid", "triangle", "falling path"

### Template

```cpp
class Solution {
public:
    int solve(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        vector<vector<int>> dp(rows, vector<int>(cols, 0));
        dp[0][0] = grid[0][0];

        // Fill first row and column
        for (int c = 1; c < cols; c++) {
            dp[0][c] = dp[0][c-1] + grid[0][c];
        }
        for (int r = 1; r < rows; r++) {
            dp[r][0] = dp[r-1][0] + grid[r][0];
        }

        for (int r = 1; r < rows; r++) {
            for (int c = 1; c < cols; c++) {
                dp[r][c] = grid[r][c] + min(dp[r-1][c], dp[r][c-1]);
            }
        }

        return dp[rows-1][cols-1];
    }
};
```

### Space Optimization (rolling array)
If dp[r][c] only depends on row r-1, you only need two rows (or even one row updated in place).

```cpp
class Solution {
public:
    int solve(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        vector<int> prev(cols, 0);
        for (int r = 0; r < rows; r++) {
            vector<int> curr(cols, 0);
            for (int c = 0; c < cols; c++) {
                int prev_val = (r > 0) ? prev[c] : (c == 0 ? 0 : 1e9);
                curr[c] = grid[r][c] + min(prev_val, c > 0 ? curr[c-1] : 1e9);
            }
            prev = curr;
        }
        return prev[cols-1];
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 62. Unique Paths | dp[r][c] = dp[r-1][c] + dp[r][c-1] |
| 63. Unique Paths II | Same, but dp[r][c] = 0 if obstacle |
| 120. Triangle | Bottom-up; dp[i][j] = val + min(dp[i+1][j], dp[i+1][j+1]) |
| 1289. Min Falling Path Sum II | For each row, track top two minimums of previous row to avoid O(n^3) |

---

## 3. Knapsack DP

### When to Use
Select items with weights/values, fill a capacity, partition a set.

### Signal Words
"subset sum", "partition", "capacity", "at most W", "exactly S", "target"

### 0/1 Knapsack (each item used at most once)

```cpp
class Solution {
public:
    int knapsack_01(vector<int>& weights, vector<int>& values, int W) {
        int n = weights.size();
        vector<int> dp(W + 1, 0);

        for (int i = 0; i < n; i++) {
            for (int w = W; w >= weights[i]; w--) { // REVERSE to prevent reuse
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }

        return dp[W];
    }
};
```

### Unbounded Knapsack (each item used any number of times)

```cpp
class Solution {
public:
    int knapsack_unbounded(vector<int>& weights, vector<int>& values, int W) {
        vector<int> dp(W + 1, 0);

        for (int w = 1; w <= W; w++) {
            for (int i = 0; i < weights.size(); i++) {
                if (weights[i] <= w) {
                    dp[w] = max(dp[w], dp[w - weights[i]] + values[i]); // FORWARD
                }
            }
        }

        return dp[W];
    }
};
```

### The Only Difference Between 0/1 and Unbounded
In 0/1, iterate capacity in **reverse** (each item considered once).
In unbounded, iterate capacity **forward** (item can be reused).

### Key Problems

| Problem | Type | Trick |
|---|---|---|
| 416. Partition Equal Subset Sum | 0/1 | Target = sum/2; boolean DP |
| 494. Target Sum | 0/1 | Convert to subset sum: count subsets with sum = (total + target) / 2 |
| 1049. Last Stone Weight II | 0/1 | Same as 416; minimize difference between two groups |
| 474. Ones and Zeroes | 0/1 | 2D knapsack with two capacities (count of 0s and 1s) |
| 879. Profitable Schemes | 0/1 | 2D knapsack with min-profit constraint |

---

## 4. Subsequence DP

### When to Use
Finding longest/shortest increasing, non-contiguous matching, ordering problems.

### Signal Words
"subsequence", "increasing", "non-decreasing", "pair chain", "envelope"

### LIS O(n^2) Template

```cpp
class Solution {
public:
    int lis(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        vector<int> dp(n, 1);

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
```

### LIS O(n log n) with Patience Sorting

```cpp
class Solution {
public:
    int lis_fast(vector<int>& nums) {
        vector<int> tails;
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) {
                tails.push_back(x);
            } else {
                *it = x;
            }
        }
        return tails.size();
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 300. LIS | Classic O(n^2) or O(n log n) with bisect |
| 354. Russian Doll Envelopes | Sort by width asc, height desc; then LIS on height |
| 673. Number of LIS | Track both length and count arrays |
| 646. Max Length Pair Chain | Greedy sort by end, or DP like LIS |

---

## 5. String DP

### When to Use
Two strings being compared, transformed, or matched.

### Signal Words
"edit distance", "common subsequence", "delete", "insert", "replace", "interleave", "distinct subsequences"

### LCS Template (basis for most string DP)

```cpp
class Solution {
public:
    int lcs(string s, string t) {
        int m = s.size();
        int n = t.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i-1] == t[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }

        return dp[m][n];
    }
};
```

### Edit Distance Template

```cpp
class Solution {
public:
    int editDistance(string s, string t) {
        int m = s.size();
        int n = t.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 0; i <= m; i++) {
            dp[i][0] = i;
        }
        for (int j = 0; j <= n; j++) {
            dp[0][j] = j;
        }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i-1] == t[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                }
            }
        }

        return dp[m][n];
    }
};
```

### Key Problems

| Problem | Recurrence (mismatch case) | Trick |
|---|---|---|
| 1143. LCS | max(dp[i-1][j], dp[i][j-1]) | Base for 583, 712 |
| 72. Edit Distance | 1 + min(insert, delete, replace) | Standard |
| 583. Delete Operation | Convert to LCS; answer = m + n - 2*LCS | Only deletions allowed |
| 115. Distinct Subsequences | dp[i][j] = dp[i-1][j] + (dp[i-1][j-1] if match) | Count ways, not length |
| 97. Interleaving String | dp[i][j] = True if valid interleave using s1[:i] and s2[:j] | 2D boolean DP |

---

## 6. Palindrome DP

### When to Use
Palindrome detection, partitioning into palindromes, longest palindromic something.

### Signal Words
"palindrome", "partition", "symmetric"

### Palindrome Check Table (precompute all substrings)

```cpp
class Solution {
public:
    vector<vector<bool>> buildPalindromeTable(string s) {
        int n = s.size();
        vector<vector<bool>> is_pal(n, vector<bool>(n, false));

        for (int i = 0; i < n; i++) {
            is_pal[i][i] = true;
        }
        for (int i = 0; i < n - 1; i++) {
            is_pal[i][i+1] = (s[i] == s[i+1]);
        }

        for (int len = 3; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                is_pal[i][j] = (s[i] == s[j]) && is_pal[i+1][j-1];
            }
        }

        return is_pal;
    }
};
```

### Longest Palindromic Subsequence

```cpp
class Solution {
private:
    int lcs(string s, string t) {
        int m = s.size();
        int n = t.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i-1] == t[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[m][n];
    }

public:
    int lps(string s) {
        string t = s;
        reverse(t.begin(), t.end());
        return lcs(s, t); // LCS of s and its reverse
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 5. Longest Palindromic Substring | Expand around center O(n^2) or precompute table |
| 516. Longest Palindromic Subsequence | LCS(s, reverse(s)) |
| 132. Palindrome Partitioning II | Precompute is_pal table; dp[i] = min cuts for s[:i+1] |
| 647. Palindromic Substrings | Count all True entries in is_pal table |

---

## 7. Interval DP

### When to Use
Operations on contiguous subarrays, merging, splitting, removing in ranges.

### Signal Words
"burst balloons", "merge stones", "cut", "remove", "triangulation", "score of subarray"

### Template (all intervals by increasing length)

```cpp
class Solution {
public:
    int interval_dp(vector<int>& arr) {
        int n = arr.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));

        for (int len = 2; len <= n; len++) {          // subarray length
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;              // subarray [i..j]
                for (int k = i; k < j; k++) {           // split point
                    dp[i][j] = max(dp[i][j], dp[i][k] + dp[k+1][j] + cost(i, k, j));
                }
            }
        }

        return dp[0][n-1];
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 312. Burst Balloons | Think in reverse: last balloon to burst; add padding [1] + arr + [1] |
| 1547. Min Cost to Cut a Stick | Add 0 and n as boundary cuts; dp[i][j] = stick length + min split |
| 1039. Triangulation | Fix one edge, try all triangle vertices in between |
| 664. Strange Printer | dp[i][j] = min turns; if s[i] == s[k], we can merge their prints |

---

## 8. DP on Trees

### When to Use
Optimization or counting on tree structure, subtree-based decisions.

### Signal Words
"tree", "subtree", "parent-child", "cameras", "matching", "distance in tree"

### Template (post-order DFS)

```cpp
class Solution {
public:
    int dfs(int node, int parent, vector<vector<int>>& graph, vector<int>& vals) {
        // Process children first, then combine
        int result = base_value;
        for (int child : graph[node]) {
            if (child != parent) {
                int child_result = dfs(child, node, graph, vals);
                result = combine(result, child_result);
            }
        }
        return result;
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 337. House Robber III | Return (rob_root, skip_root) pair from each node |
| 968. Binary Tree Cameras | Return state: 0=needs coverage, 1=has camera, 2=covered; greedy placement |
| 543. Diameter of Binary Tree | At each node: update global max with left+right depth; return max(left,right)+1 |
| 834. Sum of Distances in Tree | Two-pass rerooting; see category 14 |

---

## 9. DAG DP

### When to Use
Longest/shortest path with dependencies, counting paths in a DAG, topological order DP.

### Signal Words
"directed", "prerequisite", "dependency", "increasing path in grid"

### Template

```cpp
class Solution {
public:
    int dag_dp(vector<vector<pair<int, int>>>& graph, int node, vector<int>& memo) {
        if (memo[node] != -1) {
            return memo[node];
        }
        int result = base;
        for (auto& edge : graph[node]) {
            int neighbor = edge.first;
            int cost = edge.second;
            result = best(result, dag_dp(graph, neighbor, memo) + cost);
        }
        return memo[node] = result;
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 2050. Parallel Courses III | Topo sort + dp[node] = time[node] + max(dp[prerequisites]) |
| 1857. Largest Color Value | Topo sort + dp[node][color] = max count along path |
| 2328. Increasing Paths in Grid | Implicit DAG: edge exists if neighbor is strictly larger |

---

## 10. Bitmask DP

### When to Use
Small set of items (n <= 20), need to track which subset has been used.

### Signal Words
"visit all", "assign to groups", "team", "permutation with constraints", n <= 20

### Template

```cpp
class Solution {
public:
    int bitmask_dp(int n, vector<vector<int>>& cost) {
        vector<int> dp(1 << n, 1e9);
        dp[0] = 0;

        for (int mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] == 1e9) {
                continue;
            }
            int item = __builtin_popcount(mask); // next item index
            for (int j = 0; j < n; j++) {
                if (!(mask & (1 << j))) {
                    int new_mask = mask | (1 << j);
                    dp[new_mask] = min(dp[new_mask], dp[mask] + cost[item][j]);
                }
            }
        }

        return dp[(1 << n) - 1];
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 847. Shortest Path Visiting All Nodes | BFS with state (node, visited_mask); start from all nodes simultaneously |
| 698. Partition K Equal Subsets | dp[mask] = can we fill some buckets using this subset |
| 1125. Smallest Sufficient Team | dp[skill_mask] = min team; iterate over people, OR their skills into mask |
| 526. Beautiful Arrangement | dp[mask] = count of valid arrangements using this subset of numbers |

---

## 11. Digit DP

### When to Use
Count numbers in a range [L, R] satisfying some digit-by-digit constraint.

### Signal Words
"count numbers up to N", "digits", "at most", "sum of digits", "digit constraint"

### Template (top-down)

```cpp
class Solution {
private:
    int dp(int pos, bool tight, bool started, string& N, vector<vector<vector<int>>>& memo) {
        int n = N.size();
        if (pos == n) {
            return started ? 1 : 0; // or other base condition
        }
        if (memo[pos][tight][started] != -1) {
            return memo[pos][tight][started];
        }

        int limit = tight ? (N[pos] - '0') : 9;
        int result = 0;

        for (int digit = 0; digit <= limit; digit++) {
            bool new_tight = tight && (digit == limit);
            bool new_started = started || (digit != 0);
            result += dp(pos + 1, new_tight, new_started, N, memo);
        }

        return memo[pos][tight][started] = result;
    }

public:
    int countUpTo(string N) {
        int n = N.size();
        // graphust the dimensions of memo based on extra_state if needed
        vector<vector<vector<int>>> memo(n, vector<vector<int>>(2, vector<int>(2, -1)));
        return dp(0, true, false, N, memo);
    }
};
```

### Key State Variables
- `tight`: are we still constrained by the upper bound digit?
- `started`: have we placed a non-zero digit yet (handles leading zeros)?
- Extra state: digit sum, count of a specific digit, parity, etc.

### Key Problems

| Problem | Extra State | Trick |
|---|---|---|
| 233. Number of Digit One | count of 1s seen so far | Can also solve with math pattern |
| 902. Numbers At Most N Given Digit Set | none (just check valid digits) | Only digits in set are allowed |
| 2719. Count of Integers | digit sum bounded by min and max | Two digit DPs: count(max) - count(min-1) |

---

## 12. State Machine DP

### When to Use
Finite set of states that transition based on choices; stock problems, cooldown, fee.

### Signal Words
"buy and sell", "cooldown", "transaction fee", "at most k transactions", "states"

### General Stock Template

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int hold = -1e9; // max profit while holding a stock
        int cash = 0;    // max profit while not holding

        for (int price : prices) {
            hold = max(hold, cash - price);  // buy
            cash = max(cash, hold + price);  // sell
        }

        return cash;
    }
};
```

### Key Problems

| Problem | States | Trick |
|---|---|---|
| 121. Buy & Sell I | hold, cash | Only one transaction; hold = max(hold, -price) |
| 123. Buy & Sell III | hold1, cash1, hold2, cash2 | Exactly two transactions; chain two state machines |
| 188. Buy & Sell IV | dp[k][hold/cash] | At most k; reduce to II when k >= n/2 |
| 309. With Cooldown | hold, sold, rest | After selling, must rest one day before buying |
| 714. With Fee | hold, cash | cash = max(cash, hold + price - fee) |

---

## 13. Probability / Expectation DP

### When to Use
Expected value of random processes, probability of reaching a state.

### Signal Words
"probability", "expected", "dice", "random", "chance"

### Template

```cpp
class Solution {
public:
    // Expected value DP: E[state] = sum over transitions of (prob * E[next_state]) + reward
    // Often works backwards from terminal states
    double expected(int state, vector<double>& memo) {
        if (terminal(state)) {
            return 0;
        }
        if (memo[state] != -1.0) {
            return memo[state];
        }
        double result = 0.0;
        for (auto& transition : transitions(state)) {
            int next_s = transition.next_state;
            double p = transition.probability;
            double r = transition.reward;
            result += p * (r + expected(next_s, memo));
        }
        return memo[state] = result;
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 688. Knight Probability | dp[k][r][c] = probability of being at (r,c) after k moves |
| 837. New 21 Game | Sliding window to optimize the sum over previous states |
| 808. Soup Servings | For large N, probability approaches a constant; hardcode threshold |

---

## 14. Rerooting DP

### When to Use
Computing a value for every node as root, using information from two DFS passes.

### Signal Words
"sum of distances", "answer for each node as root", "re-root"

### Two-Pass Pattern

```cpp
class Solution {
private:
    void dfs1(int u, int p, vector<vector<int>>& graph, vector<int>& down, vector<int>& size) {
        for (int v : graph[u]) {
            if (v != p) {
                dfs1(v, u, graph, down, size);
                size[u] += size[v];
                down[u] += down[v] + size[v];
            }
        }
    }

    void dfs2(int u, int p, int n, vector<vector<int>>& graph, vector<int>& down, vector<int>& size, vector<int>& ans) {
        ans[u] = down[u]; // will add parent contribution
        for (int v : graph[u]) {
            if (v != p) {
                // When we reroot to v:
                // - v gains (n - size[v]) nodes from above
                // - u loses size[v] nodes from below
                int old_down_u = down[u];
                int old_down_v = down[v];
                int old_size_u = size[u];
                int old_size_v = size[v];

                down[v] = down[u] - size[v] + (n - size[v]);
                size[v] = n; // from v's perspective it sees all n nodes

                dfs2(v, u, n, graph, down, size, ans);

                // Restore states for backtracking if needed
                down[u] = old_down_u;
                down[v] = old_down_v;
                size[u] = old_size_u;
                size[v] = old_size_v;
            }
        }
    }

public:
    vector<int> reroot(int root, vector<vector<int>>& graph, int n) {
        vector<int> down(n, 0);
        vector<int> size(n, 1);
        vector<int> ans(n, 0);

        dfs1(root, -1, graph, down, size);
        dfs2(root, -1, n, graph, down, size, ans);
        return ans;
    }
};
```

---

## 15. Advanced DP Optimization

### When to Use
Standard O(n^2) or O(n^3) DP is too slow; constraints push you toward O(n log n) or O(n).

### Divide and Conquer DP
Use when: the optimal split point `opt(i)` is monotone: `opt(i) <= opt(i+1)`.
Reduces O(n^2) to O(n log n).

### Knuth's Optimization
Use when: cost function satisfies quadrangle inequality AND opt is monotone.
Reduces interval DP from O(n^3) to O(n^2).
Condition: `cost(a,c) + cost(b,d) <= cost(a,d) + cost(b,c)` for a<=b<=c<=d.

### Convex Hull Trick (CHT)
Use when: dp[i] = min over j of (dp[j] + b[j] * a[i]), and lines can be added/queried in order.
Reduces O(n^2) to O(n) with a monotone deque, or O(n log n) with a sorted structure.

```cpp
// Lines: y = m*x + b, query minimum y at x
class ConvexHullTrick {
public:
    vector<pair<long long, long long>> lines; // (slope, intercept)

    bool bad(pair<long long, long long> l1, pair<long long, long long> l2, pair<long long, long long> l3) {
        // (b3 - b1) / (m1 - m3) <= (b2 - b1) / (m1 - m2)
        // (b3 - b1) * (m1 - m2) <= (b2 - b1) * (m1 - m3)
        return (__int128)(l3.second - l1.second) * (l1.first - l2.first) <= (__int128)(l2.second - l1.second) * (l1.first - l3.first);
    }

    void add(long long m, long long b) {
        pair<long long, long long> line = {m, b};
        while (lines.size() >= 2 && bad(lines[lines.size() - 2], lines[lines.size() - 1], line)) {
            lines.pop_back();
        }
        lines.push_back(line);
    }

    long long query(long long x) {
        int lo = 0, hi = lines.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (lines[mid].first * x + lines[mid].second > lines[mid + 1].first * x + lines[mid + 1].second) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lines[lo].first * x + lines[lo].second;
    }
};
```

### Monotonic Queue DP
Use when: dp[i] = min/max of dp[j] over a sliding window [i-k, i-1].
Reduces O(nk) to O(n).

### Key Problems

| Problem | Optimization | Trick |
|---|---|---|
| 410. Split Array Largest Sum | Binary search on answer | Check feasibility instead of DP |
| 1335. Min Difficulty Job Schedule | Monotone stack or D&C DP | dp[d][i] = min difficulty for d days, first i jobs |
| 1478. Allocate Mailboxes | Knuth optimization | Precompute cost(i,j) = median-based |

---

## Pattern Recognition Quick-Reference

| Signal in Problem | Category | State Definition |
|---|---|---|
| Linear sequence, fixed lookback | 1D DP | dp[i] = answer for first i |
| Grid, row-by-row movement | 2D Grid DP | dp[r][c] = answer at cell |
| Subset sum, capacity, partition | Knapsack | dp[w] = best value at capacity w |
| Longest non-contiguous match | Subsequence DP | dp[i] = length ending at i |
| Two strings, edit/match | String DP | dp[i][j] = answer for s[:i], t[:j] |
| Palindrome, symmetric | Palindrome DP | dp[i][j] = is s[i..j] palindrome |
| Range/interval operations | Interval DP | dp[i][j] = answer for range [i,j] |
| Tree with subtree decisions | Tree DP | dp[node] = answer for subtree |
| Directed graph, dependencies | DAG DP | dp[node] = answer along best path |
| Small n, track used items | Bitmask DP | dp[mask] = answer for this subset |
| Count integers with digit rule | Digit DP | dp[pos][tight][...] |
| Buy/sell/cooldown states | State Machine DP | dp[state] = max profit in state |
| Probability, expected value | Expectation DP | dp[state] = E[outcome] |
| Every node as root of tree | Rerooting DP | Two DFS passes |
| DP too slow, monotone opt | Advanced Optimization | CHT, D&C DP, Knuth |

---

## Complexity Cheat Sheet

| Category | Typical Time | Space |
|---|---|---|
| 1D DP | O(n) or O(n * coins) | O(n), often O(1) |
| 2D Grid DP | O(rows * cols) | O(rows * cols), often O(cols) |
| 0/1 Knapsack | O(n * W) | O(W) |
| LIS O(n^2) | O(n^2) | O(n) |
| LIS O(n log n) | O(n log n) | O(n) |
| String DP (LCS, Edit) | O(m * n) | O(m * n), often O(n) |
| Palindrome DP | O(n^2) | O(n^2) |
| Interval DP | O(n^3) | O(n^2) |
| Tree DP | O(n) | O(n) |
| DAG DP | O(V + E) | O(V) |
| Bitmask DP | O(2^n * n) | O(2^n) |
| Digit DP | O(digits * states) | O(digits * states) |
| State Machine DP | O(n * states) | O(states) |
| Rerooting DP | O(n) | O(n) |
| CHT / D&C DP | O(n log n) | O(n) |

---

## Problem List by Category

### 1. 1D DP
**LeetCode:** 70, 746, 198, 213, 740, 91, 322, 279
**CSES:** Dice Combinations, Minimizing Coins, Coin Combinations I & II

### 2. 2D Grid DP
**LeetCode:** 62, 63, 64, 120, 931, 1289
**CSES:** Grid Paths

### 3. Knapsack
**LeetCode:** 416, 494, 1049, 474, 879
**CSES:** Book Shop, Money Sums

### 4. Subsequence DP
**LeetCode:** 300, 673, 354, 646
**CSES:** Increasing Subsequence

### 5. String DP
**LeetCode:** 1143, 72, 583, 115, 97, 712
**CSES:** Edit Distance

### 6. Palindrome DP
**LeetCode:** 5, 516, 131, 132, 647

### 7. Interval DP
**LeetCode:** 312, 1000, 1547, 1039, 664
**CSES:** Removal Game

### 8. DP on Trees
**LeetCode:** 337, 124, 543, 834, 968
**CSES:** Tree Matching, Tree Distances I & II

### 9. DAG DP
**LeetCode:** 2050, 797, 1857, 2328
**CSES:** Longest Flight Route, Game Routes, Coin Collector

### 10. Bitmask DP
**LeetCode:** 698, 847, 526, 1125
**CSES:** Elevator Rides, Hamiltonian Flights

### 11. Digit DP
**LeetCode:** 902, 233, 2719
**CSES:** Counting Numbers

### 12. State Machine DP
**LeetCode:** 121, 122, 123, 188, 309, 714

### 13. Probability / Expectation DP
**LeetCode:** 808, 688, 837

### 14. Rerooting DP
**LeetCode:** 834
**CSES:** Tree Distances II

### 15. Advanced DP Optimization
**LeetCode:** 410, 1335, 1478
**CSES:** Projects

---

## Top 17 CSES DP Problems (Priority Order)

1. Dice Combinations
2. Minimizing Coins
3. Coin Combinations I
4. Coin Combinations II
5. Removing Digits
6. Grid Paths
7. Book Shop
8. Array Description
9. Money Sums
10. Removal Game
11. Increasing Subsequence
12. Projects
13. Elevator Rides
14. Counting Numbers
15. Edit Distance
16. Hamiltonian Flights
17. Tree Matching

---

## LeetCode Priority List

### Beginner (start here)
70, 198, 746, 322, 62, 64

### Intermediate
300, 1143, 72, 416, 494, 516, 312, 1547, 121, 122, 123

### Advanced
698, 847, 1125, 2050, 834, 233, 410, 1335