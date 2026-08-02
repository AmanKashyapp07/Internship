# Top 30 OA Patterns for Conquering Online Assessments

> **The definitive master guide to recognizing, formulating, and writing bug-free code for the 30 most frequently tested Data Structures & Algorithms patterns in tech Online Assessments (Google, Amazon, Uber, Atlassian, Salesforce, Sprinklr, DE Shaw, Arcesium, etc.).**

---

## Table of Contents

### 1. Sliding Window & Two Pointers
- [Pattern 1: Variable-Length Sliding Window](#pattern-1-variable-length-sliding-window)
- [Pattern 2: Fixed-Length Sliding Window & Monotonic Deque](#pattern-2-fixed-length-sliding-window--monotonic-deque)
- [Pattern 3: Two Pointers & Two-Way Sweep](#pattern-3-two-pointers--two-way-sweep)

### 2. Binary Search & Optimization
- [Pattern 4: Binary Search on Answer / Solution Space](#pattern-4-binary-search-on-answer--solution-space)
- [Pattern 5: Binary Search on Real Numbers / Precision](#pattern-5-binary-search-on-real-numbers--precision)
- [Pattern 6: Ternary Search for Unimodal Functions](#pattern-6-ternary-search-for-unimodal-functions)

### 3. Monotonic Stack & Queue
- [Pattern 7: Monotonic Stack for Next Greater / Smaller Element](#pattern-7-monotonic-stack-for-next-greater--smaller-element)
- [Pattern 8: Subarray Contribution Technique (Histogram Pattern)](#pattern-8-subarray-contribution-technique-histogram-pattern)

### 4. Dynamic Programming Staples
- [Pattern 9: 0/1 & Unbounded Knapsack Variants](#pattern-9-01--unbounded-knapsack-variants)
- [Pattern 10: Longest Common Subsequence (LCS) & String Alignment](#pattern-10-longest-common-subsequence-lcs--string-alignment)
- [Pattern 11: Longest Increasing Subsequence (LIS) with Binary Search](#pattern-11-longest-increasing-subsequence-lis-with-binary-search)
- [Pattern 12: Digit DP](#pattern-12-digit-dp)
- [Pattern 13: Bitmask DP ($N \le 20$)](#pattern-13-bitmask-dp-n-le-20)
- [Pattern 14: Interval DP](#pattern-14-interval-dp)
- [Pattern 15: Grid DP with State Extensions](#pattern-15-grid-dp-with-state-extensions)

### 5. Graph Algorithms
- [Pattern 16: Multi-Source & 0-1 BFS](#pattern-16-multi-source--0-1-bfs)
- [Pattern 17: Dijkstra's Algorithm with Augmented State](#pattern-17-dijkstras-algorithm-with-augmented-state)
- [Pattern 18: Disjoint Set Union (DSU) & Offline Queries](#pattern-18-disjoint-set-union-dsu--offline-queries)
- [Pattern 19: Topological Sort & Kahn's Algorithm](#pattern-19-topological-sort--kahns-algorithm)
- [Pattern 20: Cycle Detection & Functional Graph Traversal](#pattern-20-cycle-detection--functional-graph-traversal)

### 6. Trees & Hierarchy
- [Pattern 21: Subtree DP & Bottom-Up DFS](#pattern-21-subtree-dp--bottom-up-dfs)
- [Pattern 22: Binary Lifting & Lowest Common Ancestor (LCA)](#pattern-22-binary-lifting--lowest-common-ancestor-lca)
- [Pattern 23: Tree Rerooting DP (2-Pass DFS)](#pattern-23-tree-rerooting-dp-2-pass-dfs)

### 7. Heaps, Intervals & Greedy
- [Pattern 24: Line Sweep Algorithm & Interval Overlaps](#pattern-24-line-sweep-algorithm--interval-overlaps)
- [Pattern 25: Two-Heap Pattern (Median & Dynamic Order)](#pattern-25-two-heap-pattern-median--dynamic-order)
- [Pattern 26: Greedy Scheduling & Priority Queue](#pattern-26-greedy-scheduling--priority-queue)

### 8. Advanced Structures & Strings
- [Pattern 27: Trie & Bitwise XOR Trie](#pattern-27-trie--bitwise-xor-trie)
- [Pattern 28: Prefix Sums & 1D/2D Difference Arrays](#pattern-28-prefix-sums--1d2d-difference-arrays)
- [Pattern 29: Polynomial Rolling Hash & KMP Pattern Matching](#pattern-29-polynomial-rolling-hash--kmp-pattern-matching)
- [Pattern 30: Fenwick Tree (BIT) & Segment Tree](#pattern-30-fenwick-tree-bit--segment-tree)

---

## OA Pattern Decision Matrix

| Problem Trait / Constraint | Recommended Pattern | Time Complexity |
| :--- | :--- | :--- |
| Subarray with condition (at most $K$ distinct, min len) | **Pattern 1: Variable Sliding Window** | $O(N)$ |
| Max/Min element in sliding window of size $K$ | **Pattern 2: Monotonic Deque** | $O(N)$ |
| "Minimize maximum", "Maximize minimum", $N \le 10^5$, answer range large | **Pattern 4: Binary Search on Answer** | $O(N \log (\text{range}))$ |
| Next greater element, nearest smaller element | **Pattern 7: Monotonic Stack** | $O(N)$ |
| Sum of all subarray minimums/maximums | **Pattern 8: Contribution Technique** | $O(N)$ |
| $N \le 20$, subset selection, TSP | **Pattern 13: Bitmask DP** | $O(2^N \cdot N)$ |
| Count valid numbers in $[L, R]$ with digit rules | **Pattern 12: Digit DP** | $O(\text{digits} \times \text{states})$ |
| Unweighted shortest path from multiple start points | **Pattern 16: Multi-Source BFS** | $O(V + E)$ |
| Shortest path with edge weights 0 or 1 | **Pattern 16: 0-1 BFS (Deque)** | $O(V + E)$ |
| Shortest path with costs + state (e.g. $K$ stops, fuel left) | **Pattern 17: State Dijkstra** | $O(E \log(V \times S))$ |
| Dynamic connectivity, component merging, offline queries | **Pattern 18: DSU with Rollback/Offline** | $O(N \alpha(N))$ |
| Maximum XOR pair/subarray | **Pattern 27: Bitwise XOR Trie** | $O(N \cdot 31)$ |
| Substring matching, repeating patterns, palindrome | **Pattern 29: String Hashing / KMP** | $O(N)$ |
| Dynamic range queries with point/range updates | **Pattern 30: Fenwick / Segment Tree** | $O(\log N)$ per query |

---

## 1. Sliding Window & Two Pointers

### Pattern 1: Variable-Length Sliding Window

#### Identification Signals
- Problem asks for longest / shortest subarray or substring meeting a specified criteria (e.g., at most $K$ distinct elements, sum $\ge S$).
- All elements are non-negative (ensures monotonicity when expanding window).

#### Standard C++ Template
```cpp
int variableSlidingWindow(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    int left = 0, maxLen = 0;
    
    for (int right = 0; right < nums.size(); ++right) {
        freq[nums[right]]++; // Expand right boundary
        
        // Shrink left boundary until window condition is restored
        while (freq.size() > k) { // Example condition: > k distinct elements
            freq[nums[left]]--;
            if (freq[nums[left]] == 0) freq.erase(nums[left]);
            left++;
        }
        
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
```

#### Standard OA Problems
- **LeetCode 340**: Longest Substring with At Most K Distinct Characters
- **LeetCode 1004**: Max Consecutive Ones III
- **LeetCode 209**: Minimum Size Subarray Sum

#### Complexity: Time $O(N)$, Space $O(K)$

---

### Pattern 2: Fixed-Length Sliding Window & Monotonic Deque

#### Identification Signals
- Exact window size $K$ is fixed.
- Needs maximum, minimum, or median inside every moving window of size $K$.

#### Standard C++ Template
```cpp
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    deque<int> dq; // Stores indices, elements maintain strictly decreasing order
    vector<int> res;
    
    for (int i = 0; i < nums.size(); ++i) {
        // 1. Remove elements outside current window
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        
        // 2. Maintain monotonic property (pop smaller elements)
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        // 3. Record result for valid window
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}
```

#### Standard OA Problems
- **LeetCode 239**: Sliding Window Maximum
- **LeetCode 1438**: Longest Continuous Subarray With Absolute Diff $\le$ Limit

#### Complexity: Time $O(N)$, Space $O(K)$

---

### Pattern 3: Two Pointers & Two-Way Sweep

#### Identification Signals
- Sorted array searching for target pairs/triplets, or calculating trapped volume from boundaries inward.
- Prefix & Suffix arrays for pre-computing max/min from left and right.

#### Standard C++ Template
```cpp
// Trapping Rain Water Two-Pointer Template
int trap(const vector<int>& height) {
    int l = 0, r = height.size() - 1;
    int leftMax = 0, rightMax = 0, water = 0;
    
    while (l < r) {
        if (height[l] < height[r]) {
            height[l] >= leftMax ? leftMax = height[l] : water += (leftMax - height[l]);
            l++;
        } else {
            height[r] >= rightMax ? rightMax = height[r] : water += (rightMax - height[r]);
            r--;
        }
    }
    return water;
}
```

#### Standard OA Problems
- **LeetCode 42**: Trapping Rain Water
- **LeetCode 11**: Container With Most Water
- **LeetCode 15**: 3Sum

#### Complexity: Time $O(N)$, Space $O(1)$

---

## 2. Binary Search & Optimization

### Pattern 4: Binary Search on Answer / Solution Space

#### Identification Signals
- Keywords: "Minimize the maximum value", "Maximize the minimum value", "Find maximum capacity/speed".
- Predicate function `check(mid)` is monotonic (`true, true, ..., false, false`).

#### Standard C++ Template
```cpp
bool isValid(long long mid, const vector<int>& nums, int k) {
    int count = 1;
    long long currentSum = 0;
    for (int num : nums) {
        if (currentSum + num > mid) {
            count++;
            currentSum = num;
        } else {
            currentSum += num;
        }
    }
    return count <= k;
}

long long binarySearchAnswer(const vector<int>& nums, int k) {
    long long low = *max_element(nums.begin(), nums.end());
    long long high = accumulate(nums.begin(), nums.end(), 0LL);
    long long ans = high;
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (isValid(mid, nums, k)) {
            ans = mid;        // Try to minimize further
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}
```

#### Standard OA Problems
- **LeetCode 1011**: Capacity To Ship Packages Within D Days
- **LeetCode 410**: Split Array Largest Sum
- **LeetCode 875**: Koko Eating Bananas

#### Complexity: Time $O(N \log(\text{High} - \text{Low}))$, Space $O(1)$

---

### Pattern 5: Binary Search on Real Numbers / Precision

#### Identification Signals
- Answer requires output accurate within $10^{-6}$ or floating point target.
- Loop condition relies on fixed iterations (e.g. 80-100 loops) to avoid floating point infinite loops.

#### Standard C++ Template
```cpp
double binarySearchReal(double low, double high) {
    for (int iter = 0; iter < 100; iter++) { // 100 iterations gives ~10^-30 accuracy
        double mid = low + (high - low) / 2.0;
        if (checkCondition(mid)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return low;
}
```

#### Standard OA Problems
- **CSES**: Factory Machines / Real value variants
- **LeetCode 693 / Math OAs**: Maximum Average Subarray II

#### Complexity: Time $O(N \times \text{iterations})$, Space $O(1)$

---

### Pattern 6: Ternary Search for Unimodal Functions

#### Identification Signals
- Function strictly increases then decreases (or vice versa), e.g., parabolic loss function, geometric distance minimization.

#### Standard C++ Template
```cpp
double ternarySearch(double l, double r) {
    double eps = 1e-9;
    while (r - l > eps) {
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;
        if (f(m1) < f(m2)) {
            l = m1; // For maximum, drop left third
        } else {
            r = m2;
        }
    }
    return f(l);
}
```

#### Standard OA Problems
- **Codeforces / OA**: Minimized Manhattan / Euclidean distance to point on curve
- **LeetCode 1515**: Best Position for a Service Centre

#### Complexity: Time $O(\log_{1.5}(\frac{\text{Range}}{\epsilon}))$, Space $O(1)$

---

## 3. Monotonic Stack & Queue

### Pattern 7: Monotonic Stack for Next Greater / Smaller Element

#### Identification Signals
- Need to find nearest element to the left or right that is strictly greater or smaller.

#### Standard C++ Template
```cpp
vector<int> nextGreaterElement(const vector<int>& arr) {
    int n = arr.size();
    vector<int> nge(n, -1);
    stack<int> st; // Stores indices
    
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[st.top()] < arr[i]) {
            nge[st.top()] = arr[i];
            st.pop();
        }
        st.push(i);
    }
    return nge;
}
```

#### Standard OA Problems
- **LeetCode 739**: Daily Temperatures
- **LeetCode 503**: Next Greater Element II (Circular Array)
- **LeetCode 901**: Online Stock Span

#### Complexity: Time $O(N)$, Space $O(N)$

---

### Pattern 8: Subarray Contribution Technique (Histogram Pattern)

#### Identification Signals
- Asking for sum of `min(subarray)` or `max(subarray)` over all possible subarrays.
- Key Insight: Calculate how many subarrays treat element `arr[i]` as the minimum/maximum.

#### Standard C++ Template
```cpp
long long sumSubarrayMins(const vector<int>& arr) {
    int n = arr.size();
    vector<int> left(n), right(n);
    stack<pair<int, int>> s1, s2;
    
    // Count elements strictly greater to left
    for (int i = 0; i < n; ++i) {
        int count = 1;
        while (!s1.empty() && s1.top().first >= arr[i]) {
            count += s1.top().second;
            s1.pop();
        }
        s1.push({arr[i], count});
        left[i] = count;
    }
    
    // Count elements greater to right
    for (int i = n - 1; i >= 0; --i) {
        int count = 1;
        while (!s2.empty() && s2.top().first > arr[i]) {
            count += s2.top().second;
            s2.pop();
        }
        s2.push({arr[i], count});
        right[i] = count;
    }
    
    long long totalSum = 0;
    long long mod = 1e9 + 7;
    for (int i = 0; i < n; ++i) {
        long long totalSubarrays = (long long)left[i] * right[i];
        totalSum = (totalSum + totalSubarrays * arr[i]) % mod;
    }
    return totalSum;
}
```

#### Standard OA Problems
- **LeetCode 907**: Sum of Subarray Minimums
- **LeetCode 84**: Largest Rectangle in Histogram
- **LeetCode 2104**: Sum of Subarray Ranges

#### Complexity: Time $O(N)$, Space $O(N)$

---

## 4. Dynamic Programming Staples

### Pattern 9: 0/1 & Unbounded Knapsack Variants

#### Identification Signals
- Select items with weights & values under weight capacity limit $W$.
- Items used at most once $\to$ iterate capacity backwards. Items used infinite times $\to$ iterate capacity forwards.

#### Standard C++ Template
```cpp
// 0/1 Knapsack (Space Optimized 1D DP)
int knapsack01(const vector<int>& wt, const vector<int>& val, int W) {
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < wt.size(); ++i) {
        for (int w = W; w >= wt[i]; --w) { // REVERSE loop for 0/1
            dp[w] = max(dp[w], val[i] + dp[w - wt[i]]);
        }
    }
    return dp[W];
}
```

#### Standard OA Problems
- **LeetCode 416**: Partition Equal Subset Sum
- **LeetCode 322**: Coin Change (Unbounded)
- **LeetCode 494**: Target Sum

#### Complexity: Time $O(N \times W)$, Space $O(W)$

---

### Pattern 10: Longest Common Subsequence (LCS) & String Alignment

#### Identification Signals
- Comparing two strings/sequences $S1$ and $S2$ for deletions, insertions, or edit distance.

#### Standard C++ Template
```cpp
int lcs(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[m][n];
}
```

#### Standard OA Problems
- **LeetCode 1143**: Longest Common Subsequence
- **LeetCode 72**: Edit Distance
- **LeetCode 1092**: Shortest Common Supersequence

#### Complexity: Time $O(M \times N)$, Space $O(M \times N)$

---

### Pattern 11: Longest Increasing Subsequence (LIS) with Binary Search

#### Identification Signals
- Finding length of longest strictly increasing or non-decreasing sequence in $O(N \log N)$.

#### Standard C++ Template
```cpp
int lengthOfLIS(const vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
```

#### Standard OA Problems
- **LeetCode 300**: Longest Increasing Subsequence
- **LeetCode 354**: Russian Doll Envelopes (2D sorting + LIS)
- **LeetCode 1691**: Maximum Height by Stacking Cuboids

#### Complexity: Time $O(N \log N)$, Space $O(N)$

---

### Pattern 12: Digit DP

#### Identification Signals
- Count valid integers in range $[A, B]$ satisfying digit properties (e.g., sum of digits is prime, no adjacent duplicate digits).
- Constraints: $A, B \le 10^{18}$.

#### Standard C++ Template
```cpp
long long memo[20][2][200]; // pos, tight, sum
string numStr;

long long solveDigit(int pos, bool tight, int sum) {
    if (pos == numStr.size()) return checkProperty(sum) ? 1 : 0;
    if (memo[pos][tight][sum] != -1) return memo[pos][tight][sum];
    
    long long ans = 0;
    int limit = tight ? (numStr[pos] - '0') : 9;
    
    for (int d = 0; d <= limit; ++d) {
        ans += solveDigit(pos + 1, tight && (d == limit), sum + d);
    }
    return memo[pos][tight][sum] = ans;
}
```

#### Standard OA Problems
- **LeetCode 233**: Number of Digit One
- **LeetCode 902**: Numbers At Most N Given Digit Set
- **LeetCode 600**: Non-negative Integers without Consecutive Ones

#### Complexity: Time $O(\text{digits} \times 2 \times \text{states})$, Space $O(\text{digits} \times \text{states})$

---

### Pattern 13: Bitmask DP ($N \le 20$)

#### Identification Signals
- Small limits: $N \le 20$.
- Involves tracking visited set, set partitioning, or Travelling Salesperson Problem (TSP).

#### Standard C++ Template
```cpp
int memo[1 << 20][20];

int tsp(int mask, int u, int n, const vector<vector<int>>& dist) {
    if (mask == (1 << n) - 1) return dist[u][0]; // Return to start
    if (memo[mask][u] != -1) return memo[mask][u];
    
    int ans = 1e9;
    for (int v = 0; v < n; ++v) {
        if (!(mask & (1 << v))) {
            ans = min(ans, dist[u][v] + tsp(mask | (1 << v), v, n, dist));
        }
    }
    return memo[mask][u] = ans;
}
```

#### Standard OA Problems
- **LeetCode 847**: Shortest Path Visiting All Nodes
- **LeetCode 1879**: Minimum XOR Sum of Two Arrays
- **LeetCode 1494**: Parallel Courses II

#### Complexity: Time $O(2^N \cdot N^2)$, Space $O(2^N \cdot N)$

---

### Pattern 14: Interval DP

#### Identification Signals
- Subproblems defined over continuous intervals $[i, j]$.
- Merging adjacent elements or destroying elements from boundaries inwards.

#### Standard C++ Template
```cpp
int intervalDP(const vector<int>& arr) {
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    for (int len = 2; len <= n; ++len) { // Interval length
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            dp[i][j] = 1e9;
            for (int k = i; k < j; ++k) { // Partition point
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + cost(i, k, j));
            }
        }
    }
    return dp[0][n - 1];
}
```

#### Standard OA Problems
- **LeetCode 312**: Burst Balloons
- **LeetCode 1000**: Minimum Cost to Merge Stones
- **LeetCode 877**: Stone Game

#### Complexity: Time $O(N^3)$, Space $O(N^2)$

---

### Pattern 15: Grid DP with State Extensions

#### Identification Signals
- Matrix path finding with additional state parameters (e.g. number of turns remaining, obstacles cleared, key collected).

#### Standard C++ Template
```cpp
// Grid DP carrying extra state k (e.g. remaining obstacle eliminations)
int dp[100][100][11]; // r, c, k

int getMinPath(const vector<vector<int>>& grid, int K) {
    int R = grid.size(), C = grid[0].size();
    memset(dp, 0x3f, sizeof(dp));
    dp[0][0][K] = grid[0][0];
    
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            for (int k = 0; k <= K; ++k) {
                if (r + 1 < R) {
                    int nk = k - grid[r+1][c];
                    if (nk >= 0) dp[r+1][c][nk] = min(dp[r+1][c][nk], dp[r][c][k] + grid[r+1][c]);
                }
                if (c + 1 < C) {
                    int nk = k - grid[r][c+1];
                    if (nk >= 0) dp[r][c+1][nk] = min(dp[r][c+1][nk], dp[r][c][k] + grid[r][c+1]);
                }
            }
        }
    }
    // Return min across all remaining k
    int ans = 1e9;
    for (int k = 0; k <= K; ++k) ans = min(ans, dp[R-1][C-1][k]);
    return ans;
}
```

#### Standard OA Problems
- **LeetCode 741**: Cherry Pickup
- **LeetCode 1293**: Shortest Path in a Grid with Obstacles Elimination
- **LeetCode 63**: Unique Paths II

#### Complexity: Time $O(R \times C \times K)$, Space $O(R \times C \times K)$

---

## 5. Graph Algorithms

### Pattern 16: Multi-Source & 0-1 BFS

#### Identification Signals
- Unweighted shortest distance starting simultaneously from multiple sources (Multi-Source BFS).
- Edges have binary weight 0 or 1 $\to$ Use `std::deque` (push front for 0 weight, push back for 1 weight).

#### Standard C++ Template
```cpp
// 0-1 BFS Template
vector<int> dist(V, 1e9);
deque<int> dq;

dist[source] = 0;
dq.push_back(source);

while (!dq.empty()) {
    int u = dq.front(); dq.pop_front();
    
    for (auto& [v, weight] : adj[u]) {
        if (dist[u] + weight < dist[v]) {
            dist[v] = dist[u] + weight;
            if (weight == 0) dq.push_front(v); // Zero weight goes to front
            else dq.push_back(v);              // One weight goes to back
        }
    }
}
```

#### Standard OA Problems
- **LeetCode 994**: Rotting Oranges (Multi-Source)
- **LeetCode 1368**: Minimum Cost to Make at Least One Valid Path (0-1 BFS)
- **LeetCode 542**: 01 Matrix

#### Complexity: Time $O(V + E)$, Space $O(V)$

---

### Pattern 17: Dijkstra's Algorithm with Augmented State

#### Identification Signals
- Weighted shortest path with state dimensions, e.g., `dist[node][remaining_k_skips]` or `dist[node][fuel_level]`.

#### Standard C++ Template
```cpp
struct State {
    int cost, u, k;
    bool operator>(const State& other) const { return cost > other.cost; }
};

int dijkstraWithState(int n, int src, int target, int maxK, const vector<vector<pair<int,int>>>& adj) {
    vector<vector<int>> dist(n, vector<int>(maxK + 1, 1e9));
    priority_queue<State, vector<State>, greater<State>> pq;
    
    dist[src][maxK] = 0;
    pq.push({0, src, maxK});
    
    while (!pq.empty()) {
        auto [c, u, k] = pq.top(); pq.pop();
        if (c > dist[u][k]) continue;
        if (u == target) return c;
        
        for (auto& [v, w] : adj[u]) {
            // Option 1: Normal move
            if (dist[u][k] + w < dist[v][k]) {
                dist[v][k] = dist[u][k] + w;
                pq.push({dist[v][k], v, k});
            }
            // Option 2: Skip edge weight using 1 k-token
            if (k > 0 && dist[u][k] < dist[v][k - 1]) {
                dist[v][k - 1] = dist[u][k];
                pq.push({dist[v][k - 1], v, k - 1});
            }
        }
    }
    return -1;
}
```

#### Standard OA Problems
- **LeetCode 787**: Cheapest Flights Within K Stops
- **LeetCode 1928**: Minimum Cost to Reach Destination in Time
- **LeetCode 864**: Shortest Path to Get All Keys

#### Complexity: Time $O(E \cdot K \log(V \cdot K))$, Space $O(V \cdot K)$

---

### Pattern 18: Disjoint Set Union (DSU) & Offline Queries

#### Identification Signals
- Dynamic connectivity, grouping connected components, or processing queries offline by sorting them by threshold weight.

#### Standard C++ Template
```cpp
class DSU {
public:
    vector<int> parent, rank;
    int components;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        rank.assign(n, 1);
        components = n;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]); // Path compression
    }
    bool unite(int i, int j) {
        int root_i = find(i), root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            if (rank[root_i] == rank[root_j]) rank[root_i]++;
            components--;
            return true;
        }
        return false;
    }
};
```

#### Standard OA Problems
- **LeetCode 1697**: Checking Existence of Edge Length Limited Paths (Offline DSU)
- **LeetCode 684**: Redundant Connection
- **LeetCode 721**: Accounts Merge

#### Complexity: Time $O(N \cdot \alpha(N))$, Space $O(N)$

---

### Pattern 19: Topological Sort & Kahn's Algorithm

#### Identification Signals
- Dependency resolution, DAG order execution, detecting cycles in directed graph using indegrees.

#### Standard C++ Template
```cpp
vector<int> kahnTopoSort(int n, const vector<vector<int>>& adj) {
    vector<int> inDegree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) inDegree[v]++;
    }
    
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    
    if (order.size() != n) return {}; // Cycle detected!
    return order;
}
```

#### Standard OA Problems
- **LeetCode 210**: Course Schedule II
- **LeetCode 269**: Alien Dictionary
- **LeetCode 1857**: Largest Color Value in a Directed Graph

#### Complexity: Time $O(V + E)$, Space $O(V + E)$

---

### Pattern 20: Cycle Detection & Functional Graph Traversal

#### Identification Signals
- Directed graph where every node has exactly 1 outgoing edge (Functional Graph / Successor Graph).
- Floyd's Cycle Detection (Tortoise & Hare) or Binary Lifting for $K$ steps jump.

#### Standard C++ Template
```cpp
// Floyd's Cycle Detection on Functional Graph
int getCycleLength(int start, const vector<int>& nextNode) {
    int slow = start, fast = start;
    
    do {
        slow = nextNode[slow];
        fast = nextNode[nextNode[fast]];
    } while (slow != fast);
    
    int cycleLen = 0;
    do {
        slow = nextNode[slow];
        cycleLen++;
    } while (slow != fast);
    
    return cycleLen;
}
```

#### Standard OA Problems
- **LeetCode 287**: Find the Duplicate Number
- **LeetCode 2127**: Maximum Employees to Be Invited to a Meeting
- **LeetCode 2360**: Longest Cycle in a Graph

#### Complexity: Time $O(N)$, Space $O(1)$

---

## 6. Trees & Hierarchy

### Pattern 21: Subtree DP & Bottom-Up DFS

#### Identification Signals
- Calculating metrics (diameter, max subtree sum, matching) where answer at parent depends on child subtree computations.

#### Standard C++ Template
```cpp
int maxDiameter = 0;

int dfsSubtree(int u, int p, const vector<vector<int>>& adj) {
    int max1 = 0, max2 = 0; // Top two child depths
    
    for (int v : adj[u]) {
        if (v == p) continue;
        int depth = dfsSubtree(v, u, adj);
        if (depth > max1) { max2 = max1; max1 = depth; }
        else if (depth > max2) { max2 = depth; }
    }
    
    maxDiameter = max(maxDiameter, max1 + max2);
    return 1 + max1;
}
```

#### Standard OA Problems
- **LeetCode 124**: Binary Tree Maximum Path Sum
- **LeetCode 543**: Diameter of Binary Tree
- **LeetCode 834**: Sum of Distances in Tree

#### Complexity: Time $O(N)$, Space $O(H)$

---

### Pattern 22: Binary Lifting & Lowest Common Ancestor (LCA)

#### Identification Signals
- Tree queries: "Find $K$-th ancestor of node $U$", "Find distance / LCA between nodes $U$ and $V$" in $O(\log N)$.

#### Standard C++ Template
```cpp
const int LOGN = 20;
int up[100005][LOGN];
int depth[100005];

void dfsLCA(int u, int p, int d, const vector<vector<int>>& adj) {
    depth[u] = d;
    up[u][0] = p;
    for (int j = 1; j < LOGN; ++j) {
        up[u][j] = up[up[u][j - 1]][j - 1];
    }
    for (int v : adj[u]) {
        if (v != p) dfsLCA(v, u, d + 1, adj);
    }
}

int getLCA(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    // 1. Bring u and v to same depth
    for (int j = LOGN - 1; j >= 0; --j) {
        if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
    }
    if (u == v) return u;
    // 2. Lift simultaneously
    for (int j = LOGN - 1; j >= 0; --j) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
}
```

#### Standard OA Problems
- **LeetCode 1483**: Kth Ancestor of a Tree Node
- **LeetCode 236**: Lowest Common Ancestor of a Binary Tree
- **CSES**: Distance Queries

#### Complexity: Preprocessing $O(N \log N)$, Query $O(\log N)$

---

### Pattern 23: Tree Rerooting DP (2-Pass DFS)

#### Identification Signals
- Output a result for **every node** if that node were chosen as the tree root.
- Pass 1: Compute subtree answers bottom-up. Pass 2: Transfer parent contribution top-down.

#### Standard C++ Template
```cpp
vector<long long> countNodes, ans;

void dfs1(int u, int p, const vector<vector<int>>& adj) {
    countNodes[u] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs1(v, u, adj);
        countNodes[u] += countNodes[v];
        ans[u] += ans[v] + countNodes[v];
    }
}

void dfs2(int u, int p, int N, const vector<vector<int>>& adj) {
    for (int v : adj[u]) {
        if (v == p) continue;
        // Rerooting transformation: Moving root from u to v
        ans[v] = ans[u] - countNodes[v] + (N - countNodes[v]);
        dfs2(v, u, N, adj);
    }
}
```

#### Standard OA Problems
- **LeetCode 834**: Sum of Distances in Tree
- **LeetCode 2581**: Count Number of Possible Root Nodes
- **Codeforces / OA**: Tree Distance II

#### Complexity: Time $O(N)$, Space $O(N)$

---

## 7. Heaps, Intervals & Greedy

### Pattern 24: Line Sweep Algorithm & Interval Overlaps

#### Identification Signals
- Given intervals $[start, end]$, find max overlapping intervals, skyline, or room allocation.
- Convert intervals into events: `(start, +1)` and `(end, -1)`, then sort by coordinate.

#### Standard C++ Template
```cpp
int minMeetingRooms(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;
    for (const auto& iv : intervals) {
        events.push_back({iv[0], +1}); // Room requested
        events.push_back({iv[1], -1}); // Room released
    }
    
    // Sort events; if times tie, process end (-1) before start (+1)
    sort(events.begin(), events.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    
    int maxRooms = 0, currentRooms = 0;
    for (const auto& [time, type] : events) {
        currentRooms += type;
        maxRooms = max(maxRooms, currentRooms);
    }
    return maxRooms;
}
```

#### Standard OA Problems
- **LeetCode 253**: Meeting Rooms II
- **LeetCode 218**: The Skyline Problem
- **LeetCode 1851**: Minimum Interval to Include Each Query

#### Complexity: Time $O(N \log N)$, Space $O(N)$

---

### Pattern 25: Two-Heap Pattern (Median & Dynamic Order)

#### Identification Signals
- Continuously compute median or maintain dynamic partition of elements into lower half and upper half.

#### Standard C++ Template
```cpp
class MedianFinder {
    priority_queue<int> maxHeap;                            // Lower half
    priority_queue<int, vector<int>, greater<int>> minHeap; // Upper half

public:
    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        if (maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) return maxHeap.top();
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```

#### Standard OA Problems
- **LeetCode 295**: Find Median from Data Stream
- **LeetCode 480**: Sliding Window Median
- **LeetCode 1801**: Number of Orders in the Backlog

#### Complexity: Insertion $O(\log N)$, Find Median $O(1)$

---

### Pattern 26: Greedy Scheduling & Priority Queue

#### Identification Signals
- Make locally optimal choices (e.g. choose task with earliest deadline, maximum deadline reward, minimum fuel needed).

#### Standard C++ Template
```cpp
// Minimum Refuel Stops pattern
int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    priority_queue<int> maxFuelPQ;
    int i = 0, stops = 0, currentCap = startFuel;
    
    while (currentCap < target) {
        while (i < stations.size() && stations[i][0] <= currentCap) {
            maxFuelPQ.push(stations[i][1]);
            i++;
        }
        if (maxFuelPQ.empty()) return -1;
        currentCap += maxFuelPQ.top();
        maxFuelPQ.pop();
        stops++;
    }
    return stops;
}
```

#### Standard OA Problems
- **LeetCode 871**: Minimum Number of Refueling Stops
- **LeetCode 630**: Course Schedule III
- **LeetCode 1353**: Maximum Number of Events That Can Be Attended

#### Complexity: Time $O(N \log N)$, Space $O(N)$

---

## 8. Advanced Structures & Strings

### Pattern 27: Trie & Bitwise XOR Trie

#### Identification Signals
- Prefix searching for strings or finding pair with Maximum XOR value ($a_i \oplus a_j$).

#### Standard C++ Template
```cpp
struct TrieNode {
    TrieNode* children[2] = {nullptr, nullptr};
};

class BitwiseTrie {
    TrieNode* root = new TrieNode();
public:
    void insert(int num) {
        TrieNode* curr = root;
        for (int i = 31; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (!curr->children[bit]) curr->children[bit] = new TrieNode();
            curr = curr->children[bit];
        }
    }
    
    int getMaxXOR(int num) {
        TrieNode* curr = root;
        int maxXor = 0;
        for (int i = 31; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int oppBit = 1 - bit;
            if (curr->children[oppBit]) {
                maxXor |= (1 << i);
                curr = curr->children[oppBit];
            } else {
                curr = curr->children[bit];
            }
        }
        return maxXor;
    }
};
```

#### Standard OA Problems
- **LeetCode 421**: Maximum XOR of Two Numbers in an Array
- **LeetCode 1707**: Maximum XOR With an Element From Array
- **LeetCode 208**: Implement Trie (Prefix Tree)

#### Complexity: Insertion & Search $O(32) \approx O(1)$, Space $O(32 \cdot N)$

---

### Pattern 28: Prefix Sums & 1D/2D Difference Arrays

#### Identification Signals
- Range updates on subsegment $[L, R]$ by adding value $V$, followed by final array reconstruction.
- 2D Submatrix sum queries in $O(1)$.

#### Standard C++ Template
```cpp
// 1D Difference Array
void rangeUpdate(vector<int>& diff, int L, int R, int val) {
    diff[L] += val;
    if (R + 1 < diff.size()) diff[R + 1] -= val;
}

vector<int> reconstructArray(const vector<int>& diff) {
    vector<int> res(diff.size());
    int runningSum = 0;
    for (int i = 0; i < diff.size(); ++i) {
        runningSum += diff[i];
        res[i] = runningSum;
    }
    return res;
}
```

#### Standard OA Problems
- **LeetCode 370**: Range Addition
- **LeetCode 304**: Range Sum Query 2D - Immutable
- **LeetCode 1109**: Corporate Flight Bookings

#### Complexity: Range Update $O(1)$, Reconstruction $O(N)$

---

### Pattern 29: Polynomial Rolling Hash & KMP Pattern Matching

#### Identification Signals
- Fast substring equality comparison in $O(1)$ after $O(N)$ preprocessing.
- Finding repeating patterns, longest prefix which is also suffix ($LPS$).

#### Standard C++ Template
```cpp
struct StringHash {
    string s;
    long long B = 31, M = 1e9 + 7;
    vector<long long> hashVal, powB;
    
    StringHash(string str) : s(str) {
        int n = s.size();
        hashVal.assign(n + 1, 0);
        powB.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            hashVal[i + 1] = (hashVal[i] * B + (s[i] - 'a' + 1)) % M;
            powB[i + 1] = (powB[i] * B) % M;
        }
    }
    
    // Get hash of substring s[l...r] (0-indexed)
    long long getHash(int l, int r) {
        long long res = (hashVal[r + 1] - hashVal[l] * powB[r - l + 1]) % M;
        return (res + M) % M;
    }
};
```

#### Standard OA Problems
- **LeetCode 28**: Find the Index of the First Occurrence in a String (KMP)
- **LeetCode 1044**: Longest Duplicate Substring (Rolling Hash + BS)
- **LeetCode 214**: Shortest Palindrome

#### Complexity: Preprocessing $O(N)$, Query $O(1)$

---

### Pattern 30: Fenwick Tree (BIT) & Segment Tree

#### Identification Signals
- Dynamic array where point updates and range queries (sum, min, max, GCD) occur concurrently.

#### Standard C++ Template
```cpp
class FenwickTree {
    int n;
    vector<long long> tree;
public:
    FenwickTree(int size) : n(size), tree(size + 1, 0) {}
    
    void add(int i, long long delta) {
        for (; i <= n; i += i & (-i)) tree[i] += delta;
    }
    
    long long query(int i) {
        long long sum = 0;
        for (; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }
    
    long long queryRange(int l, int r) {
        return query(r) - query(l - 1);
    }
};
```

#### Standard OA Problems
- **LeetCode 307**: Range Sum Query - Mutable
- **LeetCode 315**: Count of Smaller Numbers After Self
- **CSES**: Dynamic Range Minimum Queries

#### Complexity: Update $O(\log N)$, Query $O(\log N)$, Space $O(N)$

---

## Final OA Exam Strategy Checklist

1. **Check N first (Constraints indicate complexity)**:
   - $N \le 10 \implies O(N!)$ or $O(N^6)$ (Backtracking / Permutations)
   - $N \le 20 \implies O(2^N \cdot N)$ (Bitmask DP)
   - $N \le 40 \implies O(2^{N/2})$ (Meet in the Middle)
   - $N \le 1000 \implies O(N^2)$ (Matrix DP, All Pairs BFS/Dijkstra)
   - $N \le 10^5 \implies O(N \log N)$ or $O(N)$ (Sliding Window, Sorting, Monotonic Stack, Segment Tree)
   - $N \le 10^9 \implies O(\log N)$ or $O(1)$ (Math, Binary Search on Answer)
2. **Watch for Edge Cases**: Empty arrays, negative integers, 0 weight edges, single element inputs, integer overflow (`long long`).
3. **Use Fast I/O in C++**: `ios_base::sync_with_stdio(false); cin.tie(NULL);`.
