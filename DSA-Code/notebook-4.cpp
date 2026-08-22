#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// K-th smallest element in two sorted arrays via binary search
// Time: O(log(min(M, N))), Space: O(1)
// k is 1-indexed
int kthElement(const vi &nums1, const vi &nums2, int k) {
    if (nums1.size() > nums2.size()) return kthElement(nums2, nums1, k); // keeps nums1 as the smaller array
    int m = nums1.size(), n = nums2.size();
    int l = max(0, k - n); // the minimum number of elements we can take from nums1
    int r = min(k, m); // the maximum number of elements we can take from nums1

    while (l <= r) {
        int i = l + (r - l) / 2;
        int j = k - i;

        int l1 = (i == 0) ? INT_MIN : nums1[i - 1];
        int r1 = (i == m) ? INT_MAX : nums1[i];
        int l2 = (j == 0) ? INT_MIN : nums2[j - 1];
        int r2 = (j == n) ? INT_MAX : nums2[j];

        if (l1 <= r2 && l2 <= r1) return max(l1, l2);
        if (l1 > r2) r = i - 1;
        else l = i + 1;
    }
    return -1;
}

int median(const vi &nums1, const vi &nums2) {
    int total = nums1.size() + nums2.size();
    if (total % 2 == 1) {
        return kthElement(nums1, nums2, total / 2 + 1);
    } else {
        int left = kthElement(nums1, nums2, total / 2);
        int right = kthElement(nums1, nums2, total / 2 + 1);
        return (left + right) / 2;
    }
}

// Next greater lexicographical permutation in-place
// Time: O(N), Space: O(1)

void nextPermutation(vi &nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;

    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}

// Length of longest substring without repeating characters
// Time: O(N), Space: O(min(N, alphabet))

int lengthOfLongestSubstring(const string &s) {
    unordered_map<char, int> last_seen;
    int l = 0, max_len = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        if (last_seen.count(s[r]) && last_seen[s[r]] >= l) {
            l = last_seen[s[r]] + 1;
        }
        last_seen[s[r]] = r;
        max_len = max(max_len, r - l + 1);
    }
    return max_len;
}


// Minimum moves to gather K consecutive 1s using prefix sum of shifted positions
// Time: O(N), Space: O(N)

ll minMoves(const vi &nums, int k) {
    vl pos;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (nums[i]) pos.push_back(i);
    }

    int m = pos.size();
    vl shifted(m), pref(m + 1, 0);
    for (int i = 0; i < m; i++) {
        shifted[i] = pos[i] - i;
        pref[i + 1] = pref[i] + shifted[i];
    }

    ll ans = 1e18;
    for (int l = 0; l + k <= m; l++) {
        int r = l + k - 1, mid = (l + r) / 2;
        ll median = shifted[mid];
        ll cost = median * (mid - l) - (pref[mid] - pref[l]) + (pref[r + 1] - pref[mid + 1]) - median * (r - mid);
        ans = min(ans, cost);
    }
    return ans;
}

// 2D Prefix Sum matrix construction
// Time: O(M * N), Space: O(M * N)

vvi prefixSum2D(const vvi &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vvi pref(m, vi(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            pref[i][j] = matrix[i][j];
            if (i > 0) pref[i][j] += pref[i - 1][j];
            if (j > 0) pref[i][j] += pref[i][j - 1];
            if (i > 0 && j > 0) pref[i][j] -= pref[i - 1][j - 1];
        }
    }
    return pref;
}

// Count subarrays with bitwise AND equal to K
// Time: O(N log(max_val)), Space: O(N)

ll countSubarraysWithAND(const vi &nums, int k) {
    ll ans = 0;
    unordered_map<int, ll> prev, cur;

    for (int x : nums) {
        cur.clear();
        cur[x]++;
        for (auto [v, cnt] : prev) {
            cur[v & x] += cnt;
        }
        ans += cur[k];
        prev = move(cur);
    }
    return ans;
}

// Multiply two 2D matrices A (M x N) and B (N x P)
// Time: O(M * N * P), Space: O(M * P)

vvi multiplyMatrices2D(const vvi &A, const vvi &B) {
    int m = A.size(), n = A[0].size(), p = B[0].size();
    vvi C(m, vi(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Count subsequences with sum equal to K (0/1 Knapsack pattern)
// Time: O(N * K), Space: O(K)

ll countSubsequences(const vi &nums, int k) {
    vl dp(k + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        for (int sum = k; sum >= x; sum--) {
            dp[sum] += dp[sum - x];
        }
    }
    return dp[k];
}

// Tree Node structure for BST / AVL trees
struct Node {
    int value, height;
    Node *left, *right;
    Node(int x) : value(x), height(1), left(nullptr), right(nullptr) {}
};

// Row index with maximum 1s in row-sorted binary matrix
// Time: O(N + M), Space: O(1)

int rowWithMax1s(const vvi &mat) {
    int n = mat.size(), m = mat[0].size();
    int row = 0, col = m - 1, ans = -1;

    while (row < n && col >= 0) {
        if (mat[row][col] == 1) {
            ans = row;
            col--;
        } else {
            row++;
        }
    }
    return ans;
}

// Maximum components tree split with equal value sum
// Time: O(N * divisors(Sum)), Space: O(N)

static bool ok_comp;

ll dfsMaxComp(int u, int p, const vvi &g, const vi &val, ll target) {
    ll sum = val[u];
    for (int v : g[u]) {
        if (v == p) continue;
        sum += dfsMaxComp(v, u, g, val, target);
    }
    if (sum == target) return 0;
    if (sum > target) ok_comp = false;
    return sum;
}

int maxComponents(const vi &val, const vvi &g) {
    ll total = accumulate(val.begin(), val.end(), 0LL);
    vl divisors;

    for (ll d = 1; d * d <= total; d++) {
        if (total % d == 0) {
            divisors.push_back(d);
            if (d * d != total) divisors.push_back(total / d);
        }
    }
    sort(divisors.begin(), divisors.end());

    for (ll target : divisors) {
        ok_comp = true;
        if (dfsMaxComp(0, -1, g, val, target) == 0 && ok_comp) {
            return total / target - 1;
        }
    }
    return 0;
}

// Wildcard pattern matching with '?' and '*'
// Time: O(|S| * |P|), Space: O(|S| * |P|)

bool wildcardHelper(const string &s, const string &p, int i, int j, vvi &dp) {
    int n = s.size(), m = p.size();
    if (i == n && j == m) return true;
    if (j == m) return false;
    if (i == n) {
        for (int k = j; k < m; k++) {
            if (p[k] != '*') return false;
        }
        return true;
    }
    if (dp[i][j] != -1) return dp[i][j];

    if (p[j] == s[i] || p[j] == '?') {
        dp[i][j] = wildcardHelper(s, p, i + 1, j + 1, dp);
    } else if (p[j] == '*') {
        dp[i][j] = wildcardHelper(s, p, i + 1, j, dp) || wildcardHelper(s, p, i, j + 1, dp);
    } else {
        dp[i][j] = false;
    }
    return dp[i][j];
}

bool wildcardMatch(const string &s, const string &p) {
    vvi dp(s.size(), vi(p.size(), -1));
    return wildcardHelper(s, p, 0, 0, dp);
}

// Next greater element using monotonic stack
// Time: O(N), Space: O(N)

vi nextGreaterElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}

// Next smaller element using monotonic stack
// Time: O(N), Space: O(N)

vi nextSmallerElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}

// Previous greater element using monotonic stack
// Time: O(N), Space: O(N)

vi previousGreaterElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] <= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}

// Previous smaller element using monotonic stack
// Time: O(N), Space: O(N)

vi previousSmallerElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] >= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}

// Stock span (consecutive days price <= today's price)
// Time: O(N), Space: O(N)

vi stockSpan(const vi &prices) {
    int n = prices.size();
    vi span(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && prices[st.top()] <= prices[i]) st.pop();
        span[i] = st.empty() ? i + 1 : i - st.top();
        st.push(i);
    }
    return span;
}

// Next greater element in a circular array
// Time: O(N), Space: O(N)

vi nextGreaterCircular(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            ans[st.top()] = nums[i % n];
            st.pop();
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Sum of subarray minimums modulo MOD
// Time: O(N), Space: O(N)

ll sumSubarrayMins(const vi &arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            // l is index of previous smaller element, r is index of next smaller element
            ans = (ans + 1LL * arr[mid] * (mid - l) % MOD * (r - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Maximum score of good subarray containing index K (min_val * length)
// Time: O(N), Space: O(N)

int maximumScore(const vi &nums, int k) {
    int n = nums.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || nums[st.top()] >= nums[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            if (l < k && k < r) {
                ans = max(ans, 1LL * nums[mid] * (r - l - 1));
            }
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Sum of subarray maximums modulo MOD
// Time: O(N), Space: O(N)

ll sumSubarrayMaxs(const vi &arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            ans = (ans + 1LL * arr[mid] * (mid - l) % MOD * (r - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}

// Maximum of minimums for every window size from 1 to N
// Time: O(N), Space: O(N)

vi maxOfMins(const vi &arr) {
    int n = arr.size();
    vi ans(n + 1, INT_MIN);
    stack<int> st;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            ans[r - l - 1] = max(ans[r - l - 1], arr[mid]);
        }
        if (i < n) st.push(i);
    }

    for (int len = n - 1; len >= 1; len--) {
        ans[len] = max(ans[len], ans[len + 1]);
    }

    vi res;
    for (int len = 1; len <= n; len++) {
        res.push_back(ans[len]);
    }
    return res;
}

// Array median using QuickSelect
// Time: O(N) average, Space: O(1)

int quickPartition(vi &arr, int l, int r) {
    int rand_idx = l + rand() % (r - l + 1);
    swap(arr[rand_idx], arr[r]);
    int pivot = arr[r], i = l;

    for (int j = l; j < r; j++) {
        if (arr[j] <= pivot) swap(arr[i++], arr[j]);
    }
    swap(arr[i], arr[r]);
    return i;
}

int quickSelect(vi &arr, int l, int r, int k) {
    if (l == r) return arr[l];
    int p_idx = quickPartition(arr, l, r);
    if (k == p_idx) return arr[k];
    return k < p_idx ? quickSelect(arr, l, p_idx - 1, k) : quickSelect(arr, p_idx + 1, r, k);
}

double findMedian(vi &arr) {
    int n = arr.size();
    if (n % 2 == 1) return quickSelect(arr, 0, n - 1, n / 2);
    return (quickSelect(arr, 0, n - 1, n / 2 - 1) + quickSelect(arr, 0, n - 1, n / 2)) / 2.0;
}

// N x N MEX grid construction (grid[i][j] = i ^ j)
// Time: O(N^2), Space: O(N^2)

vvi mexGridConstruction(int n) {
    vvi grid(n, vi(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = i ^ j;
        }
    }
    return grid;
}

// Binary Lifting for LCA, tree distance, K-th ancestor, and path queries
// Time: O(N log N) build, O(log N) query, Space: O(N log N)

struct BinaryLifting {
    static const int MAXLOG = 21;
    int n, root;
    vector<array<int, MAXLOG>> up, info;
    vi depth;

    static constexpr int ID = 0;
    static constexpr bool IS_EDGE_WEIGHT = false;

    static int combine(int a, int b) { return a + b; }

    BinaryLifting(int n, int root, const vvi &g, const vi &val)
        : n(n), root(root), up(n + 1), info(n + 1), depth(n + 1, 0) {
        for (int i = 0; i <= n; i++) {
            up[i].fill(0);
            info[i].fill(ID);
        }

        vi q = {root};
        up[root][0] = 0;
        info[root][0] = val[root];

        int head = 0;
        while (head < (int)q.size()) {
            int u = q[head++];
            for (int v : g[u]) {
                if (v != up[u][0]) {
                    up[v][0] = u;
                    depth[v] = depth[u] + 1;
                    info[v][0] = val[v];
                    q.push_back(v);
                }
            }
        }

        for (int j = 1; j < MAXLOG; j++) {
            for (int i = 1; i <= n; i++) {
                int p = up[i][j - 1];
                up[i][j] = up[p][j - 1];
                info[i][j] = combine(info[i][j - 1], info[p][j - 1]);
            }
        }
    }

    int kthAncestor(int u, int k) {
        for (int j = 0; j < MAXLOG && u != 0; j++) {
            if ((k >> j) & 1) u = up[u][j];
        }
        return u == 0 ? -1 : u;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = kthAncestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = MAXLOG - 1; j >= 0; j--) {
            if (up[a][j] != up[b][j]) {
                a = up[a][j];
                b = up[b][j];
            }
        }
        return up[a][0];
    }

    int dist(int a, int b) {
        return depth[a] + depth[b] - 2 * depth[lca(a, b)];
    }

    int queryUp(int u, int k) {
        int res = ID;
        for (int j = 0; j < MAXLOG && u != 0; j++) {
            if ((k >> j) & 1) {
                res = combine(res, info[u][j]);
                u = up[u][j];
            }
        }
        return res;
    }

    int queryPath(int a, int b) {
        int l = lca(a, b);
        int res = combine(queryUp(a, depth[a] - depth[l]), queryUp(b, depth[b] - depth[l]));
        if (!IS_EDGE_WEIGHT) res = combine(res, info[l][0]);
        return res;
    }
};

// Digit DP template for digit sums/properties of numbers <= N
// Time: O(digits * state_space), Space: O(digits * state_space)

struct DigitDP {
    string s;
    ll memo[20][2][2][100];

    ll dp(int pos, bool started, bool tight, int state) {
        if (pos == (int)s.size()) return state;
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];

        int limit = tight ? s[pos] - '0' : 9;
        ll ans = 0;

        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == limit), state);
        }

        for (int d = (started ? 0 : 1); d <= limit; d++) {
            ans += dp(pos + 1, true, tight && (d == limit), state + d);
        }

        return memo[pos][started][tight][state] = ans;
    }

    ll solve(ll n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

// Cycle detection and reconstruction in graphs
// Time: O(V + E), Space: O(V + E)

vi buildCycle(int s, int e, const vi &par) {
    vi cyc;
    cyc.push_back(e);
    while(true){
        cyc.push_back(s);
        if (s == e) break;
        s = par[s];
    }
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

struct UndirectedCycle {
    int n, s = -1, e = -1;
    vvi g;
    vi vis, par;

    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v, u)) return true;
            } else {
                s = v; e = u;
                return true;
            }
        }
        return false;
    }

    vi getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) return buildCycle(s, e, par);
        }
        return {};
    }
};

struct DirectedCycle {
    int n, s = -1, e = -1;
    vvi g;
    vi vis, in_path, par;

    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), in_path(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u) {
        vis[u] = in_path[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v)) return true;
            } else if (in_path[v]) {
                s = v; e = u;
                return true;
            }
        }
        in_path[u] = 0;
        return false;
    }

    vi getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) return buildCycle(s, e, par);
        }
        return {};
    }
};

struct Edge {
    int u, v;
    ll w;
};

vi findNegativeCycle(int n, const vector<Edge> &edges) {
    vl dist(n + 1, 0);
    vi parent(n + 1, -1);
    int last_relaxed = -1;

    for (int i = 1; i <= n; i++) {
        last_relaxed = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                last_relaxed = e.v;
            }
        }
    }

    if (last_relaxed == -1) return {};
    for (int i = 0; i < n; i++) last_relaxed = parent[last_relaxed];
    return buildCycle(last_relaxed, parent[last_relaxed], parent);
}

// Minimum cost to make array non-decreasing using max-heap strategy
// Time: O(N log N), Space: O(N)

ll makeArrayNonDecreasing(const vi &nums) {
    ll total_cost = 0;
    priority_queue<int> max_heap;

    for (int x : nums) {
        max_heap.push(x);
        if (max_heap.top() > x) {
            total_cost += max_heap.top() - x;
            max_heap.pop();
            max_heap.push(x);
        }
    }
    return total_cost;
}

