#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// Finds the k-th smallest element in two sorted arrays using binary search partition.
// Time Complexity: O(log(min(m, n))), Space Complexity: O(1).
int kthElement(vector<int>& nums1, vector<int>& nums2, int k) {
    if (nums1.size() > nums2.size()) return kthElement(nums2, nums1, k);
    int m = nums1.size(), n = nums2.size(), left = max(0, k - n), right = min(k, m);
    while (left <= right) {
        int i = left + (right - left) / 2, j = k - i;
        int L1 = (i == 0) ? INT_MIN : nums1[i - 1], R1 = (i == m) ? INT_MAX : nums1[i];
        int L2 = (j == 0) ? INT_MIN : nums2[j - 1], R2 = (j == n) ? INT_MAX : nums2[j];
        if (L1 <= R2 && L2 <= R1) return max(L1, L2);
        if (L1 > R2) right = i - 1; else left = i + 1;
    }
    return -1;
}

// Rearranges vector into the lexicographically next greater permutation in-place.
// Time Complexity: O(n), Space Complexity: O(1).
void nextPermutation(vector<int>& nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;
    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}

// Finds the length of the longest substring without repeating characters.
// Time Complexity: O(n), Space Complexity: O(min(n, alphabet_size)).
int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> lastSeen;
    int left = 0, max_len = 0;
    for (int right = 0; right < s.size(); right++) {
        if (lastSeen.count(s[right]) && lastSeen[s[right]] >= left) left = lastSeen[s[right]] + 1;
        lastSeen[s[right]] = right;
        max_len = max(max_len, right - left + 1);
    }
    return max_len;
}

// Calculates minimum sum of absolute differences to make all elements equal (via median).
// Time Complexity: O(n log n), Space Complexity: O(1).
long long minimumAbsoluteDifferenceSum(vector<int> &a) {
    int n = a.size(); sort(a.begin(), a.end());
    int median = a[n / 2]; long long sum = 0;
    for (int x : a) sum += abs(1LL * x - median);
    return sum;
}

// Finds minimum moves to gather k consecutive 1s using median strategy and prefix sums.
// Time Complexity: O(n), Space Complexity: O(n).
int minMoves(vector<int>& nums, int k) {
    vector<long long> pos;
    for (int i = 0; i < nums.size(); i++) if (nums[i]) pos.push_back(i);
    int m = pos.size();
    vector<long long> graphusted(m), prefix(m + 1);
    for (int i = 0; i < m; i++) { graphusted[i] = pos[i] - i; prefix[i + 1] = prefix[i] + graphusted[i]; }
    long long ans = LLONG_MAX;
    for (int left = 0; left + k <= m; left++) {
        int right = left + k - 1, mid = (left + right) / 2;
        long long median = graphusted[mid];
        ans = min(ans, median * (mid - left) - (prefix[mid] - prefix[left]) + (prefix[right + 1] - prefix[mid + 1]) - median * (right - mid));
    }
    return ans;
}

// Computes 2D prefix sum matrix for fast 2D range sum queries.
// Time Complexity: O(m * n), Space Complexity: O(m * n).
vvi prefixSum2D(vvi& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vvi prefix(m, vi(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            prefix[i][j] = matrix[i][j];
            if (i > 0) prefix[i][j] += prefix[i - 1][j];
            if (j > 0) prefix[i][j] += prefix[i][j - 1];
            if (i > 0 && j > 0) prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }
    return prefix;
}

// Counts subarrays with bitwise AND equal to k using map-based DP.
// Time Complexity: O(n log(max_val)), Space Complexity: O(n).
long long countSubarraysWithAND(vector<int>& nums, int k) {
    long long ans = 0; unordered_map<int, long long> prev, cur;
    for (int x : nums) {
        cur.clear(); cur[x]++;
        for (auto [v, cnt] : prev) cur[v & x] += cnt;
        ans += cur[k]; prev = move(cur);
    }
    return ans;
}

// Multiplies two 2D matrices A (m x n) and B (n x p).
// Time Complexity: O(m * n * p), Space Complexity: O(m * p).
vvi MultiplyMatrices(vvi& A, vvi& B) {
    int m = A.size(), n = A[0].size(), p = B[0].size();
    vvi C(m, vi(p, 0));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            for (int k = 0; k < n; k++) C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Counts the number of subsequences that sum up to k (0/1 Knapsack DP).
// Time Complexity: O(n * k), Space Complexity: O(k).
long long countSubsequences(vector<int>& nums, int k) {
    vector<long long> dp(k + 1, 0); dp[0] = 1;
    for (int x : nums) for (int sum = k; sum >= x; sum--) dp[sum] += dp[sum - x];
    return dp[k];
}

// Tree Node structure for BST / AVL tree representations.
// Time Complexity: O(1), Space Complexity: O(1).
struct Node {
    int value, height; Node *left, *right;
    Node(int x) : value(x), height(1), left(nullptr), right(nullptr) {}
};

// Finds the row index with maximum number of 1s in a row-sorted binary matrix.
// Time Complexity: O(n + m), Space Complexity: O(1).
int rowWithMax1s(vvi &mat) {
    int n = mat.size(), m = mat[0].size(), row = 0, col = m - 1, ans = -1;
    while (row < n && col >= 0) {
        if (mat[row][col] == 1) { ans = row; col--; }
        else row++;
    }
    return ans;
}

// Splits tree into maximum components such that sum of values in each component is equal.
// Time Complexity: O(N * divisors(Sum)), Space Complexity: O(N).
static bool ok;
long long dfsMaxComp(int u, int p, vvi& g, vi& val, long long target) {
    long long sum = val[u];
    for (int v : g[u]) { if (v == p) continue; sum += dfsMaxComp(v, u, g, val, target); }
    if (sum == target) return 0;
    if (sum > target) ok = false;
    return sum;
}
int maxComponents(vi& val, vvi& g) {
    long long total = accumulate(val.begin(), val.end(), 0LL);
    vector<long long> divisors;
    for (long long d = 1; d * d <= total; d++) {
        if (total % d == 0) { divisors.push_back(d); if (d * d != total) divisors.push_back(total / d); }
    }
    sort(divisors.begin(), divisors.end());
    for (long long target : divisors) {
        ok = true;
        if (dfsMaxComp(0, -1, g, val, target) == 0 && ok) return total / target - 1;
    }
    return 0;
}

// Wildcard pattern matching supporting '?' (any single char) and '*' (any sequence).
// Time Complexity: O(|s| * |p|), Space Complexity: O(|s| * |p|).
bool wildcardHelper(string &s, string &p, int i, int j, vvi &dp) {
    if (i == s.size() && j == p.size()) return true;
    if (j == p.size()) return false;
    if (i == s.size()) { for (int k = j; k < p.size(); ++k) if (p[k] != '*') return false; return true; }
    if (dp[i][j] != -1) return dp[i][j];
    if (p[j] == s[i] || p[j] == '?') dp[i][j] = wildcardHelper(s, p, i + 1, j + 1, dp);
    else if (p[j] == '*') dp[i][j] = wildcardHelper(s, p, i + 1, j, dp) || wildcardHelper(s, p, i, j + 1, dp);
    else dp[i][j] = false;
    return dp[i][j];
}
bool wildcardMatch(string s, string p) {
    vvi dp(s.size(), vi(p.size(), -1));
    return wildcardHelper(s, p, 0, 0, dp);
}

// Finds the next greater element for each index using a monotonic stack.
// Time Complexity: O(n), Space Complexity: O(n).
vi nextGreaterElement(const vi& nums) {
    int n = nums.size(); vi ans(n, -1); stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) { ans[st.top()] = nums[i]; st.pop(); }
        st.push(i);
    }
    return ans;
}

// Finds the next smaller element for each index using a monotonic stack.
// Time Complexity: O(n), Space Complexity: O(n).
vi nextSmallerElement(const vi& nums) {
    int n = nums.size(); vi ans(n, -1); stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) { ans[st.top()] = nums[i]; st.pop(); }
        st.push(i);
    }
    return ans;
}

// Finds the previous greater element for each index using a monotonic stack.
// Time Complexity: O(n), Space Complexity: O(n).
vi previousGreaterElement(const vi& nums) {
    int n = nums.size(); vi ans(n, -1); stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] <= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}

// Finds the previous smaller element for each index using a monotonic stack.
// Time Complexity: O(n), Space Complexity: O(n).
vi previousSmallerElement(const vi& nums) {
    int n = nums.size(); vi ans(n, -1); stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] >= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}

// Calculates stock span (number of consecutive days price was <= today's price).
// Time Complexity: O(n), Space Complexity: O(n).
vi stockSpan(const vi& prices) {
    int n = prices.size(); vi span(n); stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && prices[st.top()] <= prices[i]) st.pop();
        span[i] = st.empty() ? i + 1 : i - st.top();
        st.push(i);
    }
    return span;
}

// Finds the next greater element in a circular array by iterating 2*n times.
// Time Complexity: O(n), Space Complexity: O(n).
vi nextGreaterCircular(const vi& nums) {
    int n = nums.size(); vi ans(n, -1); stack<int> st;
    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) { ans[st.top()] = nums[i % n]; st.pop(); }
        if (i < n) st.push(i);
    }
    return ans;
}

// Computes sum of subarray minimums modulo MOD using contribution technique with stack.
// Time Complexity: O(n), Space Complexity: O(n).
ll sumSubarrayMins(const vi& arr) {
    int n = arr.size(); stack<int> st; ll ans = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top(); st.pop();
            int left = st.empty() ? -1 : st.top(), right = i;
            ans = (ans + 1LL * arr[mid] * (mid - left) % MOD * (right - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Maximum score of a good subarray containing index k (min_val * length).
// Time Complexity: O(n), Space Complexity: O(n).
int maximumScore(vector<int>& nums, int k) {
    int n = nums.size(); stack<int> st; ll ans = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || nums[st.top()] >= nums[i])) {
            int mid = st.top(); st.pop();
            int left = st.empty() ? -1 : st.top(), right = i;
            if (left < k && k < right) ans = max(ans, 1LL * nums[mid] * (right - left - 1));
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Computes sum of subarray maximums modulo MOD using contribution technique with stack.
// Time Complexity: O(n), Space Complexity: O(n).
ll sumSubarrayMaxs(const vi& arr) {
    int n = arr.size(); stack<int> st; ll ans = 0;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top(); st.pop();
            int left = st.empty() ? -1 : st.top(), right = i;
            ans = (ans + 1LL * arr[mid] * (mid - left) % MOD * (right - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Finds maximum of minimums for every window size from 1 to n.
// Time Complexity: O(n), Space Complexity: O(n).
vector<int> maxOfMins(vector<int>& arr) {
    int n = arr.size(); vector<int> ans(n + 1, INT_MIN); stack<int> st;
    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top(); st.pop();
            int left = st.empty() ? -1 : st.top(), right = i;
            ans[right - left - 1] = max(ans[right - left - 1], arr[mid]);
        }
        if (i < n) st.push(i);
    }
    for (int len = n - 1; len >= 1; len--) ans[len] = max(ans[len], ans[len + 1]);
    vector<int> res;
    for (int len = 1; len <= n; len++) res.push_back(ans[len]);
    return res;
}

// Computes median of an array in linear expected time using QuickSelect partition.
// Time Complexity: O(n) average, Space Complexity: O(1).
int partition(vector<int>& arr, int low, int high) {
    int randomIndex = low + rand() % (high - low + 1); swap(arr[randomIndex], arr[high]);
    int pivot = arr[high], i = low;
    for (int j = low; j < high; ++j) if (arr[j] <= pivot) swap(arr[i++], arr[j]);
    swap(arr[i], arr[high]); return i;
}
int quickSelect(vector<int>& arr, int low, int high, int k) {
    if (low == high) return arr[low];
    int pivotIndex = partition(arr, low, high);
    if (k == pivotIndex) return arr[k];
    return k < pivotIndex ? quickSelect(arr, low, pivotIndex - 1, k) : quickSelect(arr, pivotIndex + 1, high, k);
}
double findMedian(vector<int>& arr) {
    int n = arr.size();
    if (n % 2 == 1) return quickSelect(arr, 0, n - 1, n / 2);
    return (quickSelect(arr, 0, n - 1, n / 2 - 1) + quickSelect(arr, 0, n - 1, n / 2)) / 2.0;
}

// Constructs an n x n MEX grid where grid[i][j] = i ^ j.
// Time Complexity: O(n^2), Space Complexity: O(n^2).
vvi mexGridConstruction(int n) {
    vvi grid(n, vi(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) grid[i][j] = (i ^ j);
    return grid;
}

// Tree Binary Lifting for LCA, distance, kth ancestor, and path queries.
// Time Complexity: O(N log N) build, O(log N) query, Space Complexity: O(N log N).
struct BinaryLifting {
    static const int MAXLOG = 21; int n, root;
    vector<array<int, MAXLOG>> up, info; vector<int> depth;
    static constexpr int ID = 0; static constexpr bool IS_EDGE_WEIGHT = false;
    static int combine(int a, int b) { return a + b; }
    BinaryLifting(int n, int root, const vvi &g, const vi &val)
        : n(n), root(root), up(n + 1), info(n + 1), depth(n + 1, 0) {
        for (int i = 0; i <= n; i++) { up[i].fill(0); info[i].fill(ID); }
        vector<int> q = {root}; up[root][0] = 0; info[root][0] = val[root];
        int head = 0;
        while (head < (int)q.size()) {
            int u = q[head++];
            for (int v : g[u]) {
                if (v != up[u][0]) { up[v][0] = u; depth[v] = depth[u] + 1; info[v][0] = val[v]; q.push_back(v); }
            }
        }
        for (int j = 1; j < MAXLOG; j++)
            for (int i = 1; i <= n; i++) {
                int p = up[i][j - 1]; up[i][j] = up[p][j - 1]; info[i][j] = combine(info[i][j - 1], info[p][j - 1]);
            }
    }
    int kthAncestor(int u, int k) {
        for (int j = 0; j < MAXLOG && u != 0; j++) if ((k >> j) & 1) u = up[u][j];
        return u == 0 ? -1 : u;
    }
    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = kthAncestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = MAXLOG - 1; j >= 0; j--) if (up[a][j] != up[b][j]) { a = up[a][j]; b = up[b][j]; }
        return up[a][0];
    }
    int dist(int a, int b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
    int queryUp(int u, int k) {
        int res = ID;
        for (int j = 0; j < MAXLOG && u != 0; j++) if ((k >> j) & 1) { res = combine(res, info[u][j]); u = up[u][j]; }
        return res;
    }
    int queryPath(int a, int b) {
        int l = lca(a, b);
        int res = combine(queryUp(a, depth[a] - depth[l]), queryUp(b, depth[b] - depth[l]));
        if (!IS_EDGE_WEIGHT) res = combine(res, info[l][0]);
        return res;
    }
};

// Digit DP template for computing digit properties of numbers <= N.
// Time Complexity: O(digits * tight * started * state_space), Space Complexity: O(digits * state_space).
class DigitDP {
    string s; long long memo[20][2][2][100];
    long long dp(int pos, bool started, bool tight, int state) {
        if (pos == s.size()) return state;
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];
        int limit = tight ? s[pos] - '0' : 9; long long ans = 0;
        if (!started) ans += dp(pos + 1, false, tight && (0 == limit), state);
        for (int d = (started ? 0 : 1); d <= limit; d++) ans += dp(pos + 1, true, tight && (d == limit), state + d);
        return memo[pos][started][tight][state] = ans;
    }
public:
    long long solve(long long n) {
        if (n < 0) return 0;
        s = to_string(n); memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

// Sum Over Subsets (SOS) DP for bitmask subset and superset range queries.
// Time Complexity: O(B * 2^B), Space Complexity: O(2^B).
struct SOS {
    int B = 20, N = 1 << B;
    vector<int> f, sub, sup;
    SOS() : f(N), sub(N), sup(N) {}
    void add(int x) { f[x]++; }
    void build() {
        sub = sup = f;
        for (int b = 0; b < B; b++)
            for (int m = 0; m < N; m++)
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)];
                else sup[m] += sup[m | (1 << b)];
    }
    int subsets(int x) { return sub[x]; }
    int supersets(int x) { return sup[x]; }
    int disjoint(int x) { return sub[(N - 1) ^ x]; }
    int intersect(int x, int n) { return n - disjoint(x); }
};

// Bottom-up Interval DP template (e.g., Matrix Chain Multiplication / Merge Stones).
// Time Complexity: O(n^3), Space Complexity: O(n^2).
long long solveIntervalDPBottomUp(int n) {
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len <= n; l++) {
            int r = l + len - 1; dp[l][r] = LLONG_MAX;
            for (int k = l; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
        }
    }
    return dp[0][n - 1];
}

// Cycle detection and reconstruction utilities for Undirected, Directed, and Negative Cycles.
// Time Complexity: O(V + E) / Bellman-Ford O(V*E), Space Complexity: O(V + E).
vector<int> buildCycle(int s, int e, const vector<int>& par) {
    vector<int> cyc;
    for (int u = e; u != s; u = par[u]) cyc.push_back(u);
    cyc.push_back(s); reverse(cyc.begin(), cyc.end()); return cyc;
}
class UndirectedCycle {
    int n, s = -1, e = -1; vvi g; vi vis, par;
    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) { par[v] = u; if (dfs(v, u)) return true; }
            else { s = v; e = u; return true; }
        }
        return false;
    }
public:
    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    vi getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i, -1)) return buildCycle(s, e, par);
        return {};
    }
};
class DirectedCycle {
    int n, s = -1, e = -1; vvi g; vi vis, inPath, par;
    bool dfs(int u) {
        vis[u] = inPath[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) { par[v] = u; if (dfs(v)) return true; }
            else if (inPath[v]) { s = v; e = u; return true; }
        }
        inPath[u] = 0; return false;
    }
public:
    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), inPath(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); }
    vi getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i)) return buildCycle(s, e, par);
        return {};
    }
};
struct Edge { int u, v; ll w; };
vi findNegativeCycle(int n, const vector<Edge> &edges) {
    vector<ll> dist(n + 1, 0); vi parent(n + 1, -1); int lastRelaxedNode = -1;
    for (int i = 1; i <= n; i++) {
        lastRelaxedNode = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) { dist[e.v] = dist[e.u] + e.w; parent[e.v] = e.u; lastRelaxedNode = e.v; }
        }
    }
    if (lastRelaxedNode == -1) return {};
    while (n--) lastRelaxedNode = parent[lastRelaxedNode];
    return buildCycle(lastRelaxedNode, parent[lastRelaxedNode], parent);
}

// Calculates minimum cost to make an array non-decreasing using a max-heap strategy.
// Time Complexity: O(n log n), Space Complexity: O(n).
int inc_array(vector<int>& nums) {
    long long totalCost = 0; priority_queue<int> medianHeap;
    for (auto x : nums) {
        medianHeap.push(x);
        if (medianHeap.top() > x) {
            totalCost += medianHeap.top() - x; medianHeap.pop(); medianHeap.push(x);
        }
    }
    return totalCost;
}

// Custom implementation of Max-Heap supporting insert, pop, heapify, and heapSort.
// Time Complexity: O(log n) push/pop, O(n) build, Space Complexity: O(n).
struct MaxHeap {
    vector<int> heap;
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    void swapNodes(int i, int j) { swap(heap[i], heap[j]); }
    void heapifyDown(int i) {
        int largest = i, l = left(i), r = right(i);
        if (l < heap.size() && heap[l] > heap[largest]) largest = l;
        if (r < heap.size() && heap[r] > heap[largest]) largest = r;
        if (largest != i) { swapNodes(i, largest); heapifyDown(largest); }
    }
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) { swapNodes(i, parent(i)); i = parent(i); }
    }
    void insert(int val) { heap.push_back(val); heapifyUp(heap.size() - 1); }
    int top() { return heap[0]; }
    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back(); heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }
    void buildHeap(vector<int>& nums) {
        heap = nums;
        for (int i = heap.size() / 2 - 1; i >= 0; i--) heapifyDown(i);
    }
    vector<int> heapSort() {
        vector<int> original = heap, ans;
        while (!heap.empty()) { ans.push_back(top()); pop(); }
        heap = original; reverse(ans.begin(), ans.end());
        return ans;
    }
};
