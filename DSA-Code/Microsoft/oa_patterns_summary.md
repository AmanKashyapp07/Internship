# Microsoft OA Core Patterns & Concepts Summary
> **Last-Minute Revision Guide — Remember Patterns and Invariants, Never Individual Problems.**  
> *Read this 2–3 hours before your Online Assessment to prime your pattern recognition and avoid common pitfalls.*

---

## The Microsoft OA Mindset

Online Assessment problems are engineered with **thematic camouflage** (social networks, delivery trucks, cloud tokens, product cubicles). Strip away the narrative immediately:

1. **Find the Data Types & Constraints**:
   - $N \le 20$: Bitmask DP, recursive backtracking, meet-in-the-middle ($O(2^N)$ or $O(N!)$).
   - $N \le 10^3$: $O(N^2)$ dynamic programming, all-pairs checking, interval DP.
   - $N \le 10^5$ or $10^6$: $O(N)$ or $O(N \log N)$ only. Linear scan, two pointers, sliding window, prefix sums, binary search, monotonic stack, DSU, or heap.
2. **Identify the Core Invariant**: Every problem has a mathematical truth that remains constant regardless of state transitions.
3. **Guard Against Edge Cases First**: In Microsoft test suites, test cases 1–5 are standard; test cases 6–15 aggressively target integer overflow, empty/singleton sets, negative offsets, and duplicates.

---

## Quick Pattern Recognition Matrix

| # | What the Problem Asks / Cues | Target Pattern | Core Invariant | Key PYQ Anchor |
|---|---|---|---|---|
| **1** | Range updates, multiple interval flips, prefix increments | **Difference Array** | `diff[l] += v; diff[r+1] -= v;` prefix sum reconstructs array | Roll String, Light Panel Toggle |
| **2** | "Can form palindrome by reordering", pairs matching parity | **26-Bit Parity Bitmask** | Bit `k` toggled for odd char; valid if `mask == 0` or `(mask & (mask-1)) == 0` | Palindromic Pairs, Palindromic Bitmask |
| **3** | Minimize maximum, maximize minimum, capacity/threshold | **Binary Search on Answer** | Monotonic predicate `check(mid)`: `F F F T T T` or `T T T F F F` | Magnetic Force, Alex Products |
| **4** | Next greater/smaller element, stock spans, subarray bounds | **Monotonic Stack** | Elements maintained in strict monotonic order; pop invalidates bounds | Student Regions, Shopkeeper Sale, Asteroids |
| **5** | Contiguous subarray with condition, budget constraint | **Sliding Window** | Window $[L, R]$ expands right greedily, contracts left until valid | Equal Substrings Budget, 3-Letter Substrings |
| **6** | Dynamic graph connectivity, cluster strength, accessible nodes | **Disjoint Set Union (DSU)** | Path compression + union by size/rank with component attributes | Visible Profiles Count, Component Strength |
| **7** | Maximize/minimize sum of products, optimal selection order | **Rearrangement Inequality & Greedy** | Sorting both sequences in same order maximizes $\sum A_i B_i$; opposite minimizes | Weighted Difference, Manufacturing Resources |
| **8** | Unordered sets with adjacency / neighbor constraints | **Disjoint Case Partitioning** | Partition index configurations into mutually exclusive, exhaustive cases | Product Constraint Triplets |
| **9** | Optimal prefix + suffix choices, deleting middle segment | **Two-Pointer Boundary Shrinking** | Fix valid distinct prefix $s[0..L]$, slide valid distinct suffix $s[R..N-1]$ | Shortest Substring Deletion |
| **10** | Manhattan distance minimization across 2D points | **Independent Dimension Decoupling** | $|x_i - x| + |y_i - y|$ splits into 1D $X$ and 1D $Y$; optimal point is median | Weighted Manhattan Distance City |
| **11** | Subarray/string subsequence matching, interval transitions | **Dynamic Programming Archetypes** | 1D rolling array, 2-state transitions, or prefix/suffix state splits | Subseq of X / Substr of Y, Palindromic Subseq |
| **12** | Event rate limiting, message deduplication, TTL caches | **Stateful Timestamp Tracking** | Map key $\to$ last successful timestamp; dropped events never advance clock | Message Delivery Rate Limiter, Token TTL |

---

## The 12 Algorithmic Archetypes in Depth

---

### 1. Difference Arrays & Prefix Accumulation
- **Trigger Cues**: "Apply $Q$ operations to roll/shift prefixes of length $k$", "Toggle lights over ranges $[L, R]$", "Add value to subsegments".
- **Core Invariant**: Adding $+v$ at index $L$ and $-v$ at index $R+1$ propagates $+v$ strictly across the range $[L, R]$ upon computing prefix sums.
- **Mental Model**:
  ```cpp
  // Range increment [l, r] with value v in O(1)
  diff[l] += v;
  if (r + 1 < n) diff[r + 1] -= v;

  // Reconstruct in O(N)
  int running = 0;
  for (int i = 0; i < n; ++i) {
      running += diff[i];
      arr[i] = (arr[i] + running) % MOD;
  }
  ```
- **Microsoft Gotchas**:
  - *Circular shifts*: When rolling lowercase characters, take `(original - 'a' + running % 26 + 26) % 26 + 'a'`.
  - *XOR version*: If toggling boolean state, `diff[l] ^= 1; diff[r+1] ^= 1;` with running prefix XOR.

---

### 2. 26-Bit Parity Bitmask & Frequency Maps
- **Trigger Cues**: "Can strings be concatenated/rearranged to form a palindrome?", "Count pairs whose character frequencies have at most one odd count".
- **Core Invariant**: A string can be rearranged into a palindrome if and only if **at most one** character occurs an odd number of times. Represent odd/even parities as an integer mask $\in [0, 2^{26}-1]$.
- **Mental Model**:
  ```cpp
  int mask = 0;
  for (char c : s) mask ^= (1 << (c - 'a'));

  // Target pairings for current mask:
  // 1. Exact match (all characters have even counts together)
  ans += freq[mask];
  // 2. Differs by exactly 1 bit (one character has odd count)
  for (int b = 0; b < 26; ++b) {
      ans += freq[mask ^ (1 << b)];
  }
  freq[mask]++;
  ```
- **Microsoft Gotchas**:
  - Unordered pairs $(i, j)$ with $i < j$: Count matches in `freq` *before* inserting `mask` to avoid double counting and self-pairing.

---

### 3. Binary Search on Monotonic Answer
- **Trigger Cues**: "Maximize the minimum distance", "Minimize the maximum workload", "Find smallest capacity/budget to achieve condition".
- **Core Invariant**: If target threshold $X$ is feasible, then all $X' < X$ (or $X' > X$) are automatically feasible. The feasibility function `check(X)` is monotonic.
- **Mental Model**:
  ```cpp
  int low = min_possible, high = max_possible, ans = low;
  while (low <= high) {
      int mid = low + (high - low) / 2;
      if (canAchieve(mid)) {
          ans = mid;         // Record candidate
          low = mid + 1;     // Try larger (if maximizing)
      } else {
          high = mid - 1;    // Back off
      }
  }
  ```
- **Microsoft Gotchas**:
  - *Greedy check step*: Always place the first item at index `0` (extremum) to leave maximal space for remaining items.
  - *Range bounds*: Make sure `high` covers the theoretical maximum ($10^9$ or $10^{14}$), and use `long long` to prevent arithmetic overflow in `mid`.

---

### 4. Monotonic Stacks (PGE / NGE & Histogram Spans)
- **Trigger Cues**: "Find previous/next greater/smaller element", "Count subarrays where element is max/min", "Asteroids/particles colliding based on direction".
- **Core Invariant**: A monotonic decreasing stack maintains elements in decreasing order; incoming larger element pops and claims all smaller elements as their Next Greater Element.
- **Mental Model**:
  ```cpp
  vector<int> nge(n, n), pge(n, -1);
  stack<int> st;

  for (int i = 0; i < n; ++i) {
      while (!st.empty() && arr[st.top()] < arr[i]) {
          nge[st.top()] = i; // i is the Next Greater
          st.pop();
      }
      if (!st.empty()) pge[i] = st.top(); // Current top is Previous Greater
      st.push(i);
  }
  // Span of arr[i] being the maximum is: (nge[i] - 1) - (pge[i] + 1) + 1
  ```
- **Microsoft Gotchas**:
  - *Duplicate elements*: To avoid double-counting subarrays, use strict inequality `<` for PGE and non-strict `<=` for NGE (or vice versa).
  - *Asteroid collision*: Only collide when `stack.top() > 0` (moving right) and `incoming < 0` (moving left). Equal magnitude destroys both!

---

### 5. Sliding Window & Boundary Tracking
- **Trigger Cues**: "Longest substring with cost $\le K$", "Count substrings containing letters 'a', 'b', and 'c'", "Window of size $K$".
- **Core Invariant**: For monotonic positive quantities, expanding $R$ monotonically increases cost/presence; advancing $L$ monotonically restores feasibility.
- **Mental Model**:
  - **Dynamic Size Window**:
    ```cpp
    int l = 0, cost = 0, maxLen = 0;
    for (int r = 0; r < n; ++r) {
        cost += calc(arr[r]);
        while (cost > budget) {
            cost -= calc(arr[l]);
            l++;
        }
        maxLen = max(maxLen, r - l + 1);
    }
    ```
  - **Last Occurrence / Min-Offset Tracking**:
    ```cpp
    // Substrings ending at r containing 'a', 'b', 'c':
    // Any starting index <= min(lastA, lastB, lastC) is valid!
    int lastA = -1, lastB = -1, lastC = -1;
    long long count = 0;
    for (int r = 0; r < n; ++r) {
        if (s[r] == 'a') lastA = r;
        if (s[r] == 'b') lastB = r;
        if (s[r] == 'c') lastC = r;
        count += (1 + min({lastA, lastB, lastC}));
    }
    ```
- **Microsoft Gotchas**:
  - In string questions with fixed alphabet size (e.g. 3 characters), `last_pos` array of size 3 turns an $O(N)$ nested loop into an elegant single pass.

---

### 6. Disjoint Set Union (DSU) & Component Metrics
- **Trigger Cues**: "Users can view each other's profiles if connected directly or indirectly", "Return component size for queries", "Find max node weight after adding edges".
- **Core Invariant**: Every connected component is uniquely represented by its canonical root. Auxiliary properties (size, max element, sum) are maintained at the root.
- **Mental Model**:
  ```cpp
  struct DSU {
      vector<int> parent, sz, maxVal;
      DSU(int n, const vector<int>& vals) : parent(n + 1), sz(n + 1, 1), maxVal(n + 1) {
          iota(parent.begin(), parent.end(), 0);
          for (int i = 1; i <= n; ++i) maxVal[i] = vals[i];
      }
      int find(int x) {
          return parent[x] == x ? x : parent[x] = find(parent[x]);
      }
      bool unite(int a, int b) {
          int rootA = find(a), rootB = find(b);
          if (rootA == rootB) return false;
          if (sz[rootA] < sz[rootB]) swap(rootA, rootB);
          parent[rootB] = rootA;
          sz[rootA] += sz[rootB];
          maxVal[rootA] = max(maxVal[rootA], maxVal[rootB]);
          return true;
      }
  };
  ```
- **Microsoft Gotchas**:
  - 1-based vs 0-based indexing: Allocate size `n + 1` to prevent out-of-bounds segfaults.
  - Path compression (`parent[x] = find(parent[x])`) + union by size gives near $O(1)$ amortized runtime ($\alpha(N)$).

---

### 7. Greedy Choice & Rearrangement Inequalities
- **Trigger Cues**: "Pair elements to maximize/minimize total weighted sum", "Combine models to fulfill multi-feature requirements at lowest cost", "Order manufacturing steps".
- **Core Invariant**:
  - *Rearrangement Inequality*: Given sequences $A$ and $B$, $\sum A_i B_i$ is maximized when both are sorted in the same order, and minimized when sorted in opposite orders.
  - *Two-way capability merge*: If item `"11"` provides both features, and item `"01"` + item `"10"` together provide both features, compare $C("11")$ against $C("01") + C("10")$. Sort both streams and merge greedily.
  - *Resource Ordering*: If a job requires `threshold` to start and consumes `spend`, sort by `(threshold - spend)` descending.
- **Microsoft Gotchas**:
  - In pairing problems (e.g. $k$-capable models), models with `"00"` are completely useless — filter them out on day one.
  - Check whether $k$ can exceed total available valid units. If $k > \text{units.size()}$, return `-1`.

---

### 8. Disjoint Partitioning of Combinations
- **Trigger Cues**: "Count triplets with condition where at least two chosen indices are adjacent in the original array".
- **Core Invariant**: Never use inclusion-exclusion with messy overlaps if you can partition the search space into **mutually exclusive, exhaustive events**.
- **Mental Model**:
  For sorted indices $i < j < k$:
  - **Case 1**: $j = i + 1$ (first two adjacent). The third index $k$ can be any index in $[i + 2, N - 1]$.
  - **Case 2**: $k = j + 1$ (last two adjacent) AND $i \ne j - 1$ (first index is strictly non-adjacent to $j$, so $i \le j - 2$).
  - Every valid triplet falls into exactly one case. Zero double-counting.
- **Microsoft Gotchas**:
  - When matching products $A[i] \cdot A[j] \cdot A[k] = \text{target}$, always divide using `long long`:
    ```cpp
    long long p = 1LL * A[i] * A[j];
    if (p == 0) { /* target == 0 check */ }
    else if (target % p == 0 && countMap.count(target / p)) { ... }
    ```

---

### 9. Two-Pointer Distinct Boundary Shrinking
- **Trigger Cues**: "Find minimum length of contiguous substring to delete so that all remaining characters are unique".
- **Core Invariant**: The preserved characters consist of a prefix $s[0..L]$ and a suffix $s[R..N-1]$. Both must be internally distinct, and their character sets must have zero overlap.
- **Mental Model**:
  1. Find largest distinct suffix starting at index $R$ (scan right-to-left).
  2. Set initial `ans = R` (delete entire prefix $s[0..R-1]$).
  3. Scan prefix from $L = 0$ forward:
     - If $s[L]$ duplicates an earlier prefix char, terminate immediately.
     - While $s[L]$ exists in suffix set, shrink suffix (increment $R$ and remove $s[R]$ from set).
     - Update `ans = min(ans, R - L - 1)`.
- **Microsoft Gotchas**:
  - The deleted substring length between $[0..L]$ and $[R..N-1]$ is exactly $R - L - 1$.
  - If the initial string already has all distinct characters, the loop naturally gives `ans = 0`.

---

### 10. Independent Dimension Decoupling (Manhattan Metric)
- **Trigger Cues**: "Find a facility location $(X, Y)$ that minimizes the sum of Manhattan distances $\sum w_i (|x_i - X| + |y_i - Y|)$".
- **Core Invariant**: Manhattan distance decouples into two 100% independent 1D problems:
  $$\min \sum w_i |x_i - X| \quad \text{and} \quad \min \sum w_i |y_i - Y|$$
- **Mental Model**:
  - For 1D points with weights, the cost-minimizing coordinate is the **weighted median**.
  - Sort points by $x$-coordinate, accumulate weights until prefix sum $\ge \text{totalWeight} / 2$.
  - Repeat independently for $y$-coordinate.
- **Microsoft Gotchas**:
  - Do not search 2D grid space with BFS or gradient descent. Sorting 1D coordinates solves it in $O(N \log N)$.

---

### 11. Dynamic Programming Archetypes
- **Pattern A: Subsequence vs Substring Matching (PYQ1 #28)**
  - Find max length string that is a subsequence of $X$ and a substring of $Y$.
  - *Invariant*: Substring requires continuous matching in $Y$; subsequence allows skipping in $X$.
  - $dp[j] =$ max length of match ending strictly at $Y[j-1]$ using prefix of $X$.
  - When $X[i] == Y[j-1]$, $cur[j] = prev[j-1] + 1$. Space optimized with 2 rolling vectors.
- **Pattern B: Rolling Interval DP for Palindromic Subsequences (PYQ2 #1)**
  - $dp[i][j]$ for range $[i..j]$. If $s[i] == s[j]$, $2 + dp[i+1][j-1]$; else $\max(dp[i+1][j], dp[i][j-1])$.
  - Compute row-by-row backwards or diagonally; rolling 2 rows reduces space from $O(N^2)$ to $O(N)$.
- **Pattern C: Jump Game VI / Sliding Window DP (PYQ2 #28)**
  - $dp[i] = \text{cost}[i] + \min_{i-k \le j < i} dp[j]$.
  - Maintain a **monotonic deque** storing pairs `(dp[j], j)` with increasing values to get the range minimum in $O(1)$.

---

### 12. Stateful Timestamp Rate Limiting & TTL
- **Trigger Cues**: "Allow message if not delivered within last $K$ seconds", "Dropped messages do not refresh timer", "Session token expiration".
- **Core Invariant**:
  - *Successful message*: Updates `lastDelivered[msg] = current_time`.
  - *Dropped message*: Does **NOT** update timestamp. The timer remains anchored to the last *successful* delivery.
- **Mental Model**:
  ```cpp
  unordered_map<string, int> lastDelivered;
  vector<bool> ans;
  for (int i = 0; i < n; ++i) {
      const string& msg = messages[i];
      int t = timestamps[i];
      if (lastDelivered.count(msg) && t - lastDelivered[msg] <= k) {
          ans.push_back(false); // Dropped! Do not update map
      } else {
          ans.push_back(true);  // Delivered!
          lastDelivered[msg] = t;
      }
  }
  ```
- **Microsoft Gotchas**:
  - Notice the strictness of the inequality: "within last $k$ seconds" means $t - \text{last} \le k$ (or $< k$ depending on problem text: "strictly within" vs "at least $k$ seconds apart"). Read carefully!

---

## The Top 7 Microsoft OA Edge-Case Killers

1. **Integer Multiplication Overflow**:
   - `x * y` where $x, y \approx 10^5$ overflows 32-bit signed integer.
   - **Fix**: Always cast to `long long` before multiplying: `1LL * x * y`.
2. **Binary Exponentiation / Bitshifts Overflow**:
   - `1 << 35` is undefined behavior in C++!
   - **Fix**: Always use `1ULL << k` or `1LL << k`.
3. **Prefix Sum Off-By-One**:
   - Range sum on $[L, R]$ (0-indexed) is `prefix[R + 1] - prefix[L]`.
   - Allocate `prefix` of size `N + 1` with `prefix[0] = 0`.
4. **Graph Node Indexing**:
   - OA graph problems alternate randomly between 0-indexed and 1-indexed nodes.
   - **Fix**: Default your vectors to `vector<int> parent(n + 1)` and check `u[0] == 0` or `1`.
5. **Floating Point Precision**:
   - Avoid `double` division when halving elements (e.g. Reduce array sum by half).
   - If using `priority_queue<double>`, beware precision loss. For integer halving, keep integers and track sum decrements.
6. **Odd/Even Symmetry Centers**:
   - In expand-around-center palindrome algorithms, there are $2N - 1$ centers: $N$ single characters `(i, i)` and $N - 1$ adjacent pairs `(i, i + 1)`.
7. **Empty/Boundary Input**:
   - Always check $N = 0, 1, 2$ and whether $K > N$.

---

## 45-Second Mental Checklist Before Hitting Submit

```text
[ ] Did I cast products to 1LL before multiplication?
[ ] Are my DSU / prefix arrays sized to N + 1 for 1-based indexing?
[ ] Did I handle negative modulo: ((rem % d) + d) % d ?
[ ] For rate limiters, did I verify that dropped events DO NOT update state?
[ ] For binary search, is `high` large enough and computed without overflow?
[ ] For sliding windows, does the while loop condition properly handle `l <= r`?
[ ] Does my return type match the expected function signature (long long vs int)?
```
