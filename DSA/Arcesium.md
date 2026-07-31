# 🚀 Arcesium DSA Pattern & Template Master Cheat Sheet

> **Targeted for Arcesium / D. E. Shaw Group Technical Rounds**
> *Pure pattern-focused reference with reusable C++ templates and complexity analysis.*

---

## ⚡ Master Pattern Recognition Table

| Problem Requirement / Property | Primary Pattern / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| Min / Max in sliding window of size $K$ | **Monotonic Deque** | $O(N)$ | $O(K)$ |
| Count subarrays with sum / count **exactly** $= K$ | **$\text{AtMost}(K) - \text{AtMost}(K-1)$** | $O(N)$ | $O(1)$ |
| Min substring containing all target characters | **Variable Sliding Window + Map** | $O(N)$ | $O(\Sigma)$ |
| Sum of Subarray Minimums / Maximums | **Monotonic Stack Contribution** | $O(N)$ | $O(N)$ |
| Prerequisites / Dependency ordering | **Kahn's Topological Sort (BFS)** | $O(V + E)$ | $O(V + E)$ |
| Lexicographically smallest Topo Order | **Min-Priority Queue Kahn's** | $O(V \log V + E)$ | $O(V + E)$ |
| Single point of failure / Critical bridges | **Tarjan's Bridges (`tin` & `low`)** | $O(V + E)$ | $O(V + E)$ |
| Shortest path with $K$ obstacle removals / stops | **Multi-State BFS / Dijkstra** | $O(V \cdot K)$ | $O(V \cdot K)$ |
| Partition array into $K$ equal sum subsets | **Bitmask DP / Backtracking** | $O(N \cdot 2^N)$ | $O(2^N)$ |
| Range merge cost / Cutting stick / Burst balloons | **Interval DP (Bottom-Up)** | $O(N^3)$ | $O(N^2)$ |
| Min cost to make array non-decreasing | **Slope Trick (Max-Heap)** | $O(N \log N)$ | $O(N)$ |
| Search over monotonic answer range | **Binary Search on Answer** | $O(N \log(\text{Range}))$ | $O(1)$ |
| Dynamic median over sliding window | **Two Multisets / Two Heaps** | $O(\log K)$ per ops | $O(K)$ |
| $K$-th ancestor of a node in a tree | **Binary Lifting (`up[u][j]`)** | $O(\log K)$ query | $O(N \log N)$ |
| Maximum XOR pair in an array | **Binary Trie** | $O(30 N)$ | $O(30 N)$ |
| $O(1)$ Substring equality checking | **Polynomial Rolling Hash (FastHash)** | $O(1)$ query | $O(N)$ |

---

## 1. Graph Algorithms & Traversal Patterns

### 1.1 Topological Sort & DAG Processing

#### Pattern A: Kahn's Topological Sort & Cycle Detection
* **Trigger**: Finding valid task execution order, dependency resolution, cycle detection in directed graphs.
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
vector<int> kahnTopoSort(int n, vector<vector<int>>& adj, vector<int>& inDegree) {
    queue<int> q;
    for (int i = 0; i < n; i++) if (inDegree[i] == 0) q.push(i);
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (int v : adj[u]) if (--inDegree[v] == 0) q.push(v);
    }
    return order.size() == n ? order : vector<int>{}; // Empty if cycle exists!
}
```

#### Pattern B: Lexicographical Topological Sort
* **Trigger**: Smallest or largest valid topological sequence required.
* **Complexities**: $O(V \log V + E)$ Time | $O(V + E)$ Space
* **Template**: Replace `std::queue` with `priority_queue<int, vector<int>, greater<int>>`.

#### Pattern C: 2-Level Topological Sort (Group & Item Dependencies)
* **Trigger**: Items belong to groups, and both group dependencies and item dependencies must be respected.
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space
* **Template**: Build separate adjacency lists for Groups and Items $\to$ Topo-sort groups $\to$ Topo-sort items within each group.

#### Pattern D: Topological Sort + DP on DAG
* **Trigger**: Max color value on directed graph, longest flight path, counting paths in DAG.
* **Complexities**: $O(C \cdot (V + E))$ Time | $O(C \cdot V)$ Space

```cpp
// DP state dp[u][color] propagated along Topological Order
for (int v : adj[u]) {
    for (int c = 0; c < C; c++) {
        int add = (color[v] == c) ? 1 : 0;
        dp[v][c] = max(dp[v][c], dp[u][c] + add);
    }
    if (--inDegree[v] == 0) q.push(v);
}
```

---

### 1.2 Shortest Path Algorithms & State Extensions

#### Pattern A: Standard Dijkstra Algorithm
* **Trigger**: Single-source shortest path on non-negative weighted graphs.
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
vector<long long> dijkstra(int src, int n, vector<vector<pair<int, int>>>& adj) {
    vector<long long> dist(n + 1, 1e18); dist[src] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
        }
    }
    return dist;
}
```

#### Pattern B: Multi-State BFS / Dijkstra
* **Trigger**: Shortest path with $K$ obstacle eliminations, $K$ stops, or 1 coupon edge halving ($W/2$).
* **Complexities**: $O(V \cdot K \log(V \cdot K))$ Time | $O(V \cdot K)$ Space
* **Template**: Expand state to `dist[u][k]`.

#### Pattern C: Bellman-Ford Negative Cycle Detection & Path Reconstruction
* **Trigger**: Graphs with negative edge weights or negative cycle detection.
* **Complexities**: $O(V \cdot E)$ Time | $O(V)$ Space
* **Template**: Relax all edges $V-1$ times. On the $V$-th pass, if any edge relaxes, backtrack $V$ times from the last relaxed node to guarantee being inside the cycle $\to$ trace using `parent[]`.

#### Pattern D: Dual Dijkstra Pattern
* **Trigger**: Shortest path with 1 modified edge (e.g., CSES Flight Discount halving 1 edge).
* **Template**: Compute `dist1[]` from $1$ on $G$, `dist2[]` from $N$ on reversed graph $G^R$. Min cost $= \min_{(u,v,w)} (dist1[u] + w/2 + dist2[v])$.

---

### 1.3 Graph Structural Properties & Eulerian Circuits

#### Pattern A: Tarjan's Bridge & Articulation Point Detection
* **Trigger**: Critical connections, single point of failure in network.
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
void dfsBridge(int u, int p, vector<vector<int>>& g, vector<int>& tin, vector<int>& low, vector<pair<int,int>>& bridges, int& timer) {
    tin[u] = low[u] = ++timer;
    for (int v : g[u]) {
        if (v == p) continue;
        if (tin[v]) low[u] = min(low[u], tin[v]);
        else {
            dfsBridge(v, u, g, tin, low, bridges, timer);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back({u, v}); // Bridge condition!
        }
    }
}
```

#### Pattern B: Kosaraju's Strongly Connected Components (SCC)
* **Trigger**: Directed graph connectivity, condensation DAG construction.
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space
* **Template**: Pass 1 finish-time DFS on $G \to$ Reverse graph $G^R \to$ Pass 2 DFS in finish-time order.

#### Pattern C: Hierholzer's Eulerian Path & Circuit
* **Trigger**: Traverse every edge exactly once.
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space
* **Template**: Post-order DFS popping used edges. Reverse final path array.

---

## 2. Dynamic Programming Master Patterns

### 2.1 Knapsack Family & Bounded Binary Splitting

#### Pattern A: 0/1 Knapsack (1D Space-Optimized)
* **Trigger**: Select subset of items at most once to maximize value within weight limit.
* **Complexities**: $O(N \cdot W)$ Time | $O(W)$ Space

```cpp
vector<int> dp(W + 1, 0);
for (int i = 0; i < n; i++) {
    for (int w = W; w >= weight[i]; w--) { // Descending loop!
        dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
    }
}
```

#### Pattern B: Unbounded Knapsack
* **Trigger**: Items can be used unlimited times (Minimizing Coins, Coin Combinations).
* **Complexities**: $O(N \cdot W)$ Time | $O(W)$ Space
* **Template**: Ascending capacity loop `for (int w = weight[i]; w <= W; w++)`.

#### Pattern C: Bounded Knapsack Binary Splitting
* **Trigger**: Item $i$ can be used up to $K_i$ times.
* **Complexities**: $O(\sum \log K_i \cdot W)$ Time | $O(W)$ Space
* **Template**: Decompose $K_i$ into powers of 2 ($1, 2, 4, \dots$) to convert into $O(\log K_i)$ items $\to$ 0/1 Knapsack.

---

### 2.2 Subsequence & Interval DP

#### Pattern A: Longest Increasing Subsequence (LIS)
* **Trigger**: Longest increasing subsequence, envelope nesting, LCS of two permutations.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

```cpp
int LIS(const vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
```

#### Pattern B: Interval DP (Bottom-Up Matrix Chain / Range Merge)
* **Trigger**: Cutting stick, burst balloons, stone games, MCM.
* **Complexities**: $O(N^3)$ Time | $O(N^2)$ Space

```cpp
for (int len = 2; len <= n; len++) {
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1; dp[l][r] = INT_MAX;
        for (int k = l; k < r; k++) {
            dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + cost(l, k, r));
        }
    }
}
```

---

### 2.3 Bitmask, SOS & Digit DP

#### Pattern A: Bitmask DP / Assignment DP
* **Trigger**: Traveling Salesperson, smallest sufficient team, partition array into $K$ equal sum subsets, elevator rides.
* **Complexities**: $O(N \cdot 2^N)$ Time | $O(2^N)$ Space

```cpp
// Elevator Rides: dp[mask] = {rides, current_weight}
vector<pair<int,int>> dp(1 << n, {n + 1, 0}); dp[0] = {1, 0};
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) {
            auto prev = dp[mask];
            pair<int,int> cur = (prev.second + w[i] <= X) ? pair<int,int>{prev.first, prev.second + w[i]} : pair<int,int>{prev.first + 1, w[i]};
            dp[mask | (1 << i)] = min(dp[mask | (1 << i)], cur);
        }
    }
}
```

#### Pattern B: Digit DP
* **Trigger**: Count integers in $[A, B]$ satisfying digit constraints (no adjacent duplicate digits, digit sum constraints).
* **Complexities**: $O(\log_{10} N \cdot \text{States})$ Time | $O(\text{States})$ Space

```cpp
long long solveDigitDP(int pos, int prev_digit, bool started, bool tight) {
    if (pos == num.size()) return 1;
    if (dp[pos][prev_digit][started][tight] != -1) return dp[pos][prev_digit][started][tight];
    int limit = tight ? num[pos] - '0' : 9; long long ans = 0;
    if (!started) ans += solveDigitDP(pos + 1, 10, false, tight && (0 == limit));
    for (int d = (started ? 0 : 1); d <= limit; d++) {
        if (started && d == prev_digit) continue; // Constraint
        ans += solveDigitDP(pos + 1, d, true, tight && (d == limit));
    }
    return dp[pos][prev_digit][started][tight] = ans;
}
```

---

### 2.4 DP on Trees & Slope Trick

#### Pattern A: Tree Rerooting DP
* **Trigger**: Compute tree property (e.g. sum of distances to all other nodes) for every vertex in $O(N)$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space
* **Formula**:
  $$\text{dist}[v] = \text{dist}[u] + N - 2 \cdot \text{subtree\_size}[v]$$

#### Pattern B: Slope Trick (Min Cost to Make Array Non-Decreasing)
* **Trigger**: Min operations $\sum |A[i] - A'[i]|$ to make array non-decreasing.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

```cpp
long long slopeTrickNonDecreasing(const vector<int>& nums) {
    long long cost = 0; priority_queue<int> maxHeap;
    for (int x : nums) {
        if (!maxHeap.empty() && maxHeap.top() > x) {
            cost += maxHeap.top() - x; maxHeap.pop(); maxHeap.push(x);
        }
        maxHeap.push(x);
    }
    return cost;
}
```

---

## 3. Monotonic Stack & Deque Suite

### 3.1 Subarray Minimums / Maximums Contribution Pattern
* **Trigger**: Sum of minimums/maximums over all subarrays.
* **Complexities**: $O(N)$ Time | $O(N)$ Space
* **Formula**: $\text{Contribution} = (i - \text{PGE}) \times (\text{NSE} - i) \times A[i]$.

```cpp
long long sumSubarrayMins(const vector<int>& arr) {
    int n = arr.size(); stack<int> st; long long ans = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top(); st.pop();
            int left = st.empty() ? -1 : st.top();
            int right = i;
            long long leftCnt = mid - left, rightCnt = right - mid;
            ans = (ans + 1LL * arr[mid] * leftCnt % MOD * rightCnt) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}
```

---

### 3.2 Monotonic Stack Core Variants

#### Pattern A: Largest Rectangle in Histogram
* **Trigger**: Max rectangle area in histogram or 2D binary matrix.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size(); stack<int> st; int maxArea = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || heights[st.top()] >= heights[i])) {
            int h = heights[st.top()]; st.pop();
            int w = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    return maxArea;
}
```

#### Pattern B: Lexicographical Monotonic Stack + Freq Map
* **Trigger**: Remove duplicate letters / smallest subsequence of distinct characters.
* **Complexities**: $O(N)$ Time | $O(26)$ Space

```cpp
string removeDuplicateLetters(string s) {
    vector<int> freq(26, 0); vector<bool> inStack(26, false);
    for (char c : s) freq[c - 'a']++;
    string st = "";
    for (char c : s) {
        freq[c - 'a']--;
        if (inStack[c - 'a']) continue;
        while (!st.empty() && st.back() > c && freq[st.back() - 'a'] > 0) {
            inStack[st.back() - 'a'] = false; st.pop_back();
        }
        st.push_back(c); inStack[c - 'a'] = true;
    }
    return st;
}
```

---

## 4. Binary Search, Sorting & Two Pointers

### 4.1 Binary Search Master Blueprints

#### Pattern A: Rotated Sorted Array Search
* **Trigger**: Search element in rotated sorted array.
* **Complexities**: $O(\log N)$ Time | $O(1)$ Space

```cpp
int searchRotated(vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) return mid;
        if (nums[low] <= nums[mid]) {
            if (target >= nums[low] && target < nums[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (target > nums[mid] && target <= nums[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return -1;
}
```

#### Pattern B: Binary Search on Answer Space
* **Trigger**: Minimize maximum or maximize minimum over monotonic search range.
* **Complexities**: $O(N \log(\text{Range}))$ Time | $O(1)$ Space

```cpp
int binarySearchOnAnswer(int low, int high, auto&& canFinish) {
    int ans = high;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (canFinish(mid)) { ans = mid; high = mid - 1; }
        else low = mid + 1;
    }
    return ans;
}
```

---

### 4.2 Essential Sorting & Selection

#### Pattern A: Dutch National Flag (3-Pointer Partitioning)
* **Trigger**: Sort array of 3 distinct numbers in-place (0, 1, 2).
* **Complexities**: $O(N)$ Time | $O(1)$ Space

```cpp
void sortColors(vector<int>& nums) {
    int low = 0, mid = 0, high = nums.size() - 1;
    while (mid <= high) {
        if (nums[mid] == 0) swap(nums[low++], nums[mid++]);
        else if (nums[mid] == 1) mid++;
        else swap(nums[mid], nums[high--]);
    }
}
```

---

## 5. Sliding Window Blueprints

### 5.1 Sliding Window Master Templates

#### Pattern A: Monotonic Deque for Window Max / Min
* **Trigger**: Max/Min element in every sliding window of size $K$.
* **Complexities**: $O(N)$ Time | $O(K)$ Space

```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; vector<int> ans;
    for (int i = 0; i < nums.size(); i++) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) ans.push_back(nums[dq.front()]);
    }
    return ans;
}
```

#### Pattern B: Exact $K$ Subarrays Formula: $\text{AtMost}(K) - \text{AtMost}(K - 1)$
* **Trigger**: Count subarrays with sum/property **exactly** $= K$.
* **Complexities**: $O(N)$ Time | $O(1)$ Space

```cpp
int atMostK(vector<int>& nums, int k) {
    if (k < 0) return 0;
    int left = 0, sum = 0, ans = 0;
    for (int right = 0; right < nums.size(); right++) {
        sum += nums[right];
        while (sum > k) sum -= nums[left++];
        ans += right - left + 1;
    }
    return ans;
}
int exactK(vector<int>& nums, int k) { return atMostK(nums, k) - atMostK(nums, k - 1); }
```

#### Pattern C: Minimum Window Substring
* **Trigger**: Find smallest substring containing all characters of pattern string.
* **Complexities**: $O(N)$ Time | $O(\Sigma)$ Space

```cpp
string minWindow(string s, string t) {
    vector<int> need(128, 0), window(128, 0);
    for (char c : t) need[c]++;
    int required = 0; for (int i = 0; i < 128; i++) if (need[i] > 0) required++;
    int left = 0, right = 0, valid = 0, start = 0, minLen = INT_MAX;
    while (right < s.size()) {
        char c = s[right++];
        if (need[c] > 0) { window[c]++; if (window[c] == need[c]) valid++; }
        while (valid == required) {
            if (right - left < minLen) { start = left; minLen = right - left; }
            char d = s[left++];
            if (need[d] > 0) { if (window[d] == need[d]) valid--; window[d]--; }
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}
```

---

## 6. Heaps, Priority Queues & Interval Scheduling

### 6.1 Interval Scheduling Patterns

#### Pattern A: Non-Overlapping Intervals (Greedy Choice)
* **Trigger**: Maximize non-overlapping intervals. Sort by **end time**.
* **Complexities**: $O(N \log N)$ Time | $O(1)$ Space

```cpp
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
    int count = 0, lastEnd = INT_MIN;
    for (auto& inv : intervals) {
        if (inv[0] >= lastEnd) lastEnd = inv[1];
        else count++;
    }
    return count;
}
```

#### Pattern B: Course Schedule III (Greedy Heap Replacement)
* **Trigger**: Maximize duration items completed before deadline. Sort by **deadline**, replace max duration using Max-Heap if deadline exceeded.
* **Complexities**: $O(N \log N)$ Time | $O(N)$ Space

---

### 6.2 Dynamic Median (Two Multisets / Two Heaps)
* **Trigger**: Maintain dynamic median in streaming data or sliding window.
* **Complexities**: $O(N \log K)$ Time | $O(K)$ Space

```cpp
class SlidingMedian {
    multiset<int> low, high;
    void rebalance() {
        while (low.size() > high.size() + 1) { high.insert(*low.rbegin()); low.erase(prev(low.end())); }
        while (low.size() < high.size()) { low.insert(*high.begin()); high.erase(high.begin()); }
    }
public:
    void add(int val) {
        if (low.empty() || val <= *low.rbegin()) low.insert(val);
        else high.insert(val);
        rebalance();
    }
    void remove(int val) {
        auto it = low.find(val);
        if (it != low.end()) low.erase(it);
        else high.erase(high.find(val));
        rebalance();
    }
    double getMedian() { return low.size() > high.size() ? *low.rbegin() : ((double)*low.rbegin() + *high.begin()) / 2.0; }
};
```

---

## 7. Trees, Binary Lifting & Binary Trie

### 7.1 Tree Binary Lifting & LCA
* **Trigger**: $K$-th ancestor in tree, LCA queries, path aggregate queries.
* **Complexities**: $O(N \log N)$ Build | $O(\log K)$ Query

```cpp
class TreeAncestor {
    int LOG; vector<vector<int>> up;
public:
    TreeAncestor(int n, vector<int>& parent) {
        LOG = 32 - __builtin_clz(n); up.assign(n, vector<int>(LOG, -1));
        for (int i = 0; i < n; i++) up[i][0] = parent[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++)
                if (up[i][j - 1] != -1) up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
    int getKthAncestor(int node, int k) {
        for (int j = 0; j < LOG; j++) if ((k >> j) & 1) { node = up[node][j]; if (node == -1) break; }
        return node;
    }
};
```

---

### 7.2 Binary Trie for Max XOR Pair
* **Trigger**: Find pair with maximum XOR sum.
* **Complexities**: $O(30 N)$ Time | $O(30 N)$ Space

```cpp
struct TrieNode { TrieNode* children[2] = {nullptr, nullptr}; };
class BinaryTrie {
    TrieNode* root = new TrieNode();
public:
    void insert(int num) {
        TrieNode* curr = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!curr->children[bit]) curr->children[bit] = new TrieNode();
            curr = curr->children[bit];
        }
    }
    int getMaxXor(int num) {
        TrieNode* curr = root; int maxVal = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1, opp = 1 - bit;
            if (curr->children[opp]) { maxVal |= (1 << i); curr = curr->children[opp]; }
            else curr = curr->children[bit];
        }
        return maxVal;
    }
};
```

---

## 8. String Algorithms & Fast Rolling Hashing

### 8.1 Polynomial Rolling Hash (FastHash)
* **Trigger**: $O(1)$ substring hash comparison.
* **Complexities**: $O(N)$ Build | $O(1)$ Query

```cpp
struct FastHash {
    const long long P = 31, MOD = 1e9 + 7; vector<long long> p, h;
    FastHash(string s) : p(s.size() + 1, 1), h(s.size() + 1, 0) {
        for (int i = 0; i < s.size(); i++) {
            p[i + 1] = (p[i] * P) % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    long long get(int l, int r) { // Returns hash of s[l..r] in O(1)
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }
};
```

---

### 8.2 Booth's Minimal String Rotation
* **Trigger**: Find lexicographically smallest string rotation in $O(N)$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
string minimalRotation(string s) {
    string t = s + s; int n = s.size(), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) k++;
        else if (t[i + k] < t[j + k]) { j += k + 1; if (i == j) j++; k = 0; }
        else { i += k + 1; if (i == j) i++; k = 0; }
    }
    return t.substr(min(i, j), n);
}
```

---

## 9. Backtracking & Recursion Patterns

### 9.1 Partition to K Equal Sum Subsets
* **Trigger**: Partition array into $K$ equal sum subsets.
* **Complexities**: $O(N \cdot 2^N)$ Time | $O(N)$ Space

```cpp
bool canPartitionKSubsets(vector<int>& nums, int k) {
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum % k != 0) return false;
    int target = sum / k, n = nums.size();
    sort(nums.rbegin(), nums.rend());
    if (nums[0] > target) return false;
    vector<bool> vis(n, false);

    auto dfs = [&](auto& self, int idx, int count, int currSum) -> bool {
        if (count == k - 1) return true;
        if (currSum == target) return self(self, 0, count + 1, 0);
        for (int i = idx; i < n; i++) {
            if (vis[i] || currSum + nums[i] > target) continue;
            vis[i] = true;
            if (self(self, i + 1, count, currSum + nums[i])) return true;
            vis[i] = false;
            if (currSum == 0) break;
        }
        return false;
    };
    return dfs(dfs, 0, 0, 0);
}
```

---
*End of Arcesium DSA Pattern & Template Master Cheat Sheet*
