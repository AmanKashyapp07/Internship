#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const int MAXN = 1005;

struct Edge {
    int u, v;
    ll w;
};

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Reconstruct cycle from parent array
// Time: O(V), Space: O(V)

vi buildCycle(int start, const vi &parent) {
    vi cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];
        if (cur == start) break;
    }
    return cycle;
}

// Shortest cycle length (girth) in unweighted graph using BFS from each node
// Time: O(V * (V + E)), Space: O(V)

int findShortestCycle(int n, const vvi &g) {
    int min_cycle = 1e9;
    for (int src = 0; src < n; src++) {
        vi dist(n, -1), parent(n, -1);
        queue<int> q;
        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                } else if (parent[u] != v) {
                    min_cycle = min(min_cycle, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return min_cycle == 1e9 ? -1 : min_cycle;
}

// Find all nodes in graph cycles via topological peeling
// Time: O(V + E), Space: O(V)

vi getNodesInCycles(int n, const vvi &g, vi &indegree) {
    queue<int> q;
    vector<bool> is_acyclic(n, false);
    vi cyclic_nodes;

    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        is_acyclic[u] = true;
        for (int v : g[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }

    for (int i = 0; i < n; i++) {
        if (!is_acyclic[i]) cyclic_nodes.push_back(i);
    }

    return cyclic_nodes;
}

// Bipartite graph check using DFS 2-coloring
// Time: O(V + E), Space: O(V)

bool dfsBipartite(int u, int color, vi &colors, const vvi &g) {
    colors[u] = color;
    for (int v : g[u]) {
        if (colors[v] == -1) {
            if (!dfsBipartite(v, 1 - color, colors, g)) return false;
        } else if (colors[v] == color) {
            return false;
        }
    }
    return true;
}

// Single-source shortest path on weighted DAG in linear time
// Time: O(V + E), Space: O(V)

vi shortestPathDAG(int n, const vector<vector<pii>> &g, int src) {
    vi indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (auto &[v, wt] : g[u]) indeg[v]++;
    }

    vi dist(n, 1e9);
    dist[src] = 0;
    queue<int> q;

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto &[v, wt] : g[u]) {
            if (dist[u] != 1e9 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
            if (--indeg[v] == 0) q.push(v);
        }
    }

    return dist;
}

// Bitwise AND of numbers in range [left, right]
// Time: O(log(right)), Space: O(1)

int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
}

// Longest subarray where bitwise AND of any two elements is 0
// Time: O(N), Space: O(1)

int longestNiceSubarray(const vi &nums) {
    int n = nums.size(), l = 0, max_len = 0, mask = 0;
    for (int r = 0; r < n; r++) {
        while ((mask & nums[r]) != 0) {
            mask ^= nums[l];
            l++;
        }
        mask |= nums[r];
        max_len = max(max_len, r - l + 1);
    }
    return max_len;
}

// Number of distinct bitwise ORs of all non-empty subarrays
// Time: O(N log(max_val)), Space: O(N log(max_val))

int subarrayBitwiseORs(const vi &arr) {
    unordered_set<int> res, cur;
    for (int x : arr) {
        unordered_set<int> nxt = {x};
        for (int y : cur) nxt.insert(x | y);
        for (int z : nxt) res.insert(z);
        cur = nxt;
    }
    return res.size();
}

// Total set bits across all numbers from 1 to N
// Time: O(log N), Space: O(1)

int countTotalSetBits(int n) {
    int total_ones = 0;
    for (int i = 0; i < 60; i++) {
        ll cycle_len = 1LL << (i + 1);
        ll cycles = (n + 1) / cycle_len;
        ll ones = 1LL << i;
        ll rem = (n + 1) % cycle_len;

        total_ones += cycles * ones;
        if (rem > ones) total_ones += (rem - ones);
    }
    return total_ones;
}

// Count subsets with sum equal to K (0/1 Knapsack pattern)
// Time: O(N * K), Space: O(K)

int countSubsetsWithSumK(const vi &nums, int k) {
    vi dp(k + 1, 0);
    dp[0] = 1;
    for (int num : nums) {
        for (int j = k; j >= num; j--) {
            dp[j] += dp[j - num];
        }
    }
    return dp[k];
}

// Partition array into two subsets minimizing absolute sum difference
// Time: O(N * total_sum), Space: O(total_sum)

int minSubsetSumDifference(const vi &nums) {
    int total_sum = accumulate(nums.begin(), nums.end(), 0);
    vector<bool> dp(total_sum / 2 + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = total_sum / 2; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    for (int j = total_sum / 2; j >= 0; j--) {
        if (dp[j]) return total_sum - 2 * j;
    }
    return total_sum;
}

// All possible money sums formed using given coins (Subset sum DP)
// Time: O(N * total_sum), Space: O(total_sum)

vi getMoneySums(const vi &coins) {
    int n = coins.size();
    if (n == 0) return {};
    int total = accumulate(coins.begin(), coins.end(), 0);
    vector<bool> dp(total + 1, false);
    dp[0] = true;

    for (int coin : coins) {
        for (int s = total; s >= coin; s--) {
            if (dp[s - coin]) dp[s] = true;
        }
    }

    vi possible;
    for (int s = 1; s <= total; s++) {
        if (dp[s]) possible.push_back(s);
    }
    return possible;
}

// Count of Longest Increasing Subsequences (LIS)
// Time: O(N^2), Space: O(N)

int countOfLIS(const vi &nums) {
    int n = nums.size();
    if (n == 0) return 0;
    vi len(n, 1), count(n, 1);
    int max_len = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    count[i] = count[j];
                } else if (len[j] + 1 == len[i]) {
                    count[i] += count[j];
                }
            }
        }
        max_len = max(max_len, len[i]);
    }

    int total = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == max_len) total += count[i];
    }
    return total;
}

// Dijkstra's single-source shortest path for non-negative weights
// Time: O((V + E) log V), Space: O(V + E)

vi dijkstra(int n, const vector<vector<pii>> &g, int src) {
    vi dist(n, 1e9);
    dist[src] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        for (auto &[v, wt] : g[u]) {
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Bellman-Ford algorithm with negative cycle detection
// Time: O(V * E), Space: O(V)

vl bellmanFord(int n, const vector<Edge> &edges, int src) {
    vl dist(n + 1, 1e18);
    dist[src] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (const auto &e : edges) {
            if (dist[e.u] != 1e18 && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }

    for (const auto &e : edges) {
        if (dist[e.u] != 1e18 && dist[e.u] + e.w < dist[e.v]) {
            throw runtime_error("Negative cycle detected");
        }
    }

    return vl(dist.begin() + 1, dist.end());
}

// Shortest path on unweighted graph using BFS
// Time: O(V + E), Space: O(V)

vi shortestPathUnweighted(int n, const vvi &g, int src) {
    vi dist(n, 1e9);
    dist[src] = 0;
    queue<int> q;
    q.push(src);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist[v] == 1e9) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// Floyd-Warshall All-Pairs Shortest Path
// Time: O(V^3), Space: O(V^2)

vvi floydWarshall(int n, const vvi &g) {
    vvi dist = g;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != 1e9 && dist[k][j] != 1e9) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}

// Detect negative cycles using Floyd-Warshall (dist[i][i] < 0)
// Time: O(V^3), Space: O(V^2)

bool cycleDetectionFloyd(int n, const vvi &g) {
    vvi dist = floydWarshall(n, g);
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) return true;
    }
    return false;
}

// Lexicographically smallest topological sort using max-heap Kahn's variant
// Time: O(V log V + E), Space: O(V)

vi lexicographicalTopoSort(int n, const vvi &g, vi indegree) {
    priority_queue<int> pq;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) pq.push(i);
    }

    vi order;
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indegree[v] == 0) pq.push(v);
        }
    }

    if ((int)order.size() != n) return {};
    reverse(order.begin(), order.end());
    return order;
}

// Shortest path on weighted DAG using Topological Order
// Time: O(V + E), Space: O(V)

vi dpOnDAG(int n, const vector<vector<pii>> &g, int src) {
    vi indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (auto &[v, wt] : g[u]) indeg[v]++;
    }

    queue<int> q;
    vi topo_order;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        for (auto &[v, wt] : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    vi dist(n, 1e9);
    dist[src] = 0;
    for (int u : topo_order) {
        if (dist[u] != 1e9) {
            for (auto &[v, wt] : g[u]) {
                dist[v] = min(dist[v], dist[u] + wt);
            }
        }
    }

    return dist;
}

// Graph M-Coloring backtracking to count valid colorings
// Time: O(M^V), Space: O(V)

struct GraphMColouring {
    bool isValid(int node, int color, const vvi &g, const vi &colors) {
        for (int v : g[node]) {
            if (colors[v] == color) return false;
        }
        return true;
    }

    void dfs(int node, int m, const vvi &g, vi &colors, int &count) {
        if (node == (int)g.size()) {
            count++;
            return;
        }
        for (int color = 1; color <= m; color++) {
            if (isValid(node, color, g, colors)) {
                colors[node] = color;
                dfs(node + 1, m, g, colors, count);
                colors[node] = 0;
            }
        }
    }

    int countColorings(int n, int m, const vvi &g) {
        vi colors(n, 0);
        int count = 0;
        dfs(0, m, g, colors, count);
        return count;
    }
};

// Cheapest flights with at most K stops (Dijkstra / State BFS)
// Time: O(E * K), Space: O(V * K)

int findCheapestPrice(int n, const vector<vector<int>> &flights, int src, int dst, int k) {
    vector<vector<pii>> g(n);
    for (auto &f : flights) g[f[0]].push_back({f[1], f[2]});

    int max_flights = k + 1;
    vvi dist(n, vi(max_flights + 1, 1e9));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    dist[src][0] = 0;
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [cost, u, taken] = pq.top();
        pq.pop();

        if (cost > dist[u][taken]) continue;
        if (u == dst) return cost;

        for (auto &[v, price] : g[u]) {
            if (taken + 1 <= max_flights && cost + price < dist[v][taken + 1]) {
                dist[v][taken + 1] = cost + price;
                pq.push({cost + price, v, taken + 1});
            }
        }
    }
    return -1;
}

// Shortest path visiting all nodes (Bitmask BFS)
// Time: O(V * 2^V), Space: O(V * 2^V)

int shortestPathVisitingAllNodes(int n, const vvi &g) {
    int target_mask = (1 << n) - 1;
    queue<pair<int, int>> q;
    vvi dist(n, vi(1 << n, 1e9));

    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i});
        dist[i][1 << i] = 0;
    }

    while (!q.empty()) {
        auto [u, mask] = q.front();
        q.pop();

        int d = dist[u][mask];
        if (mask == target_mask) return d;

        for (int v : g[u]) {
            int nxt_mask = mask | (1 << v);
            if (dist[v][nxt_mask] > d + 1) {
                dist[v][nxt_mask] = d + 1;
                q.push({v, nxt_mask});
            }
        }
    }
    return -1;
}

// Length of Shortest Common Supersequence (SCS)
// Time: O(|S1| * |S2|), Space: O(|S1| * |S2|)

int shortestCommonSubsequenceLength(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return n + m - dp[n][m];
}

// Reconstruct Shortest Common Supersequence (SCS) string
// Time: O(|S1| * |S2|), Space: O(|S1| * |S2|)

string SCS(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    string scs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            scs += s1[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            scs += s1[i - 1];
            i--;
        } else {
            scs += s2[j - 1];
            j--;
        }
    }
    while (i > 0) { scs += s1[i - 1]; i--; }
    while (j > 0) { scs += s2[j - 1]; j--; }

    reverse(scs.begin(), scs.end());
    return scs;
}

// Minimum Window Subsequence (shortest substring of s containing t as subsequence)
// Time: O(|S| * |T|), Space: O(1)

string minWindowSubsequence(const string &s, const string &t) {
    if (t.empty()) return "";
    int n = s.size(), m = t.size();
    int min_len = 1e9, start_idx = -1;
    int i = 0, j = 0;

    while (i < n) {
        if (s[i] == t[j]) {
            j++;
            if (j == m) {
                int end = i + 1;
                j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                i++; j++;
                if (end - i < min_len) {
                    min_len = end - i;
                    start_idx = i;
                }
            }
        }
        i++;
    }
    return start_idx == -1 ? "" : s.substr(start_idx, min_len);
}

// Minimum Window Substring (shortest substring of s containing all characters of t)
// Time: O(|S| + |T|), Space: O(|S| + |T|)

string minWindowSubstring(const string &s, const string &t) {
    if (t.empty()) return "";
    unordered_map<char, int> need, window;
    for (char c : t) need[c]++;

    int l = 0, r = 0, valid = 0, start = 0, min_len = 1e9;
    while (r < (int)s.size()) {
        char c = s[r++];
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) valid++;
        }
        while (valid == (int)need.size()) {
            if (r - l < min_len) {
                start = l;
                min_len = r - l;
            }
            char d = s[l++];
            if (need.count(d)) {
                if (window[d] == need[d]) valid--;
                window[d]--;
            }
        }
    }
    return min_len == 1e9 ? "" : s.substr(start, min_len);
}

// Matrix Chain Multiplication minimum scalar multiplication cost
// Time: O(N^3), Space: O(N^2)

int matrixChainOrder(const vi &p) {
    int n = p.size() - 1;
    vvi dp(n, vi(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = 1e9;
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1]);
            }
        }
    }
    return dp[0][n - 1];
}

// Count distinct subsequences of s equal to t
// Time: O(|S| * |T|), Space: O(|T|)

int numDistinct(const string &s, const string &t) {
    int n = s.size(), m = t.size();
    vector<unsigned long long> dp(m + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = m; j >= 1; j--) {
            if (s[i - 1] == t[j - 1]) dp[j] += dp[j - 1];
        }
    }
    return dp[m];
}

// AVL / BST Tree Element Node
struct Element {
    int key, height;
    Element *left, *right;
    Element(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

// Maximum sum of keys in any valid BST subtree
// Time: O(N), Space: O(N)

struct MaxSumBST {
    int ans = 0;

    tuple<bool, int, int, int> dfs(TreeNode *node) {
        if (!node) return {true, INT_MAX, INT_MIN, 0};

        auto [l_bst, l_min, l_max, l_sum] = dfs(node->left);
        auto [r_bst, r_min, r_max, r_sum] = dfs(node->right);

        if (l_bst && r_bst && node->val > l_max && node->val < r_min) {
            int sum = l_sum + r_sum + node->val;
            ans = max(ans, sum);
            return {true, min(node->val, l_min), max(node->val, r_max), sum};
        }
        return {false, 0, 0, 0};
    }

    int maxSumBST(TreeNode *root) {
        ans = 0;
        dfs(root);
        return ans;
    }
};

// Delete all leaf nodes with specified target value
// Time: O(N), Space: O(N)

TreeNode* removeLeafNodes(TreeNode *root, int target) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, target);
    root->right = removeLeafNodes(root->right, target);
    if (!root->left && !root->right && root->val == target) return nullptr;
    return root;
}

// Minimum extra characters remaining after dictionary segmentation
// Time: O(N^2), Space: O(N + dict_len)

struct MinExtraChar {
    int n;
    vi dp;
    unordered_set<string> st;

    int solve(int i, const string &s) {
        if (i == n) return 0;
        if (dp[i] != -1) return dp[i];

        int ans = 1 + solve(i + 1, s);
        string cur = "";
        for (int j = i; j < n; j++) {
            cur += s[j];
            if (st.count(cur)) {
                ans = min(ans, solve(j + 1, s));
            }
        }
        return dp[i] = ans;
    }

    int minExtraChar(const string &s, const vector<string> &dictionary) {
        n = s.size();
        st.clear();
        for (const string &word : dictionary) st.insert(word);
        dp.assign(n, -1);
        return solve(0, s);
    }
};

// Binary splitting for Bounded Knapsack items
// Time: O(log K), Space: O(log K)

void addItemBinarySplit(int w, int v, int k, vi &weights, vi &values) {
    for (int take = 1; k > 0; take <<= 1) {
        int cnt = min(take, k);
        weights.push_back(cnt * w);
        values.push_back(cnt * v);
        k -= cnt;
    }
}

// 0/1 Knapsack 1D DP
// Time: O(N * W), Space: O(W)

int knapsack01(int n, int W, const vi &weights, const vi &values) {
    vi dp(W + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int w = W; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[W];
}

// LCS length of two permutations via LIS mapping
// Time: O(N log N), Space: O(N)

int LCSLengthOf2Permutations(const vi &a, const vi &b) {
    unordered_map<int, int> pos;
    for (int i = 0; i < (int)a.size(); i++) pos[a[i]] = i;

    vi dp;
    for (int x : b) {
        if (!pos.count(x)) continue;
        auto it = lower_bound(dp.begin(), dp.end(), pos[x]);
        if (it == dp.end()) dp.push_back(pos[x]);
        else *it = pos[x];
    }
    return dp.size();
}

// Reconstruct LCS sequence of two permutations via LIS
// Time: O(N log N), Space: O(N)

vi LCSof2Permutations(const vi &a, const vi &b) {
    unordered_map<int, int> pos;
    for (int i = 0; i < (int)a.size(); i++) pos[a[i]] = i;

    vi p, val;
    for (int x : b) {
        if (!pos.count(x)) continue;
        p.push_back(pos[x]);
        val.push_back(x);
    }

    vi tail, tail_idx, parent(p.size(), -1);
    for (int i = 0; i < (int)p.size(); i++) {
        int j = lower_bound(tail.begin(), tail.end(), p[i]) - tail.begin();
        if (j == (int)tail.size()) {
            tail.push_back(p[i]);
            tail_idx.push_back(i);
        } else {
            tail[j] = p[i];
            tail_idx[j] = i;
        }
        if (j > 0) parent[i] = tail_idx[j - 1];
    }

    vi lcs;
    if (tail.empty()) return lcs;

    for (int cur = tail_idx.back(); cur != -1; cur = parent[cur]) {
        lcs.push_back(val[cur]);
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

// Length of Longest Increasing Subsequence (LIS)
// Time: O(N log N), Space: O(N)

int lis(const vi &arr) {
    vi tail;
    for (int x : arr) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end()) tail.push_back(x);
        else *it = x;
    }
    return tail.size();
}

// Reconstruct Longest Increasing Subsequence (LIS) array elements
// Time: O(N log N), Space: O(N)

vi lisSequence(const vi &arr) {
    vi tail, parent(arr.size(), -1), lis_indices;
    for (int i = 0; i < (int)arr.size(); i++) {
        auto it = lower_bound(tail.begin(), tail.end(), arr[i]);
        int idx = it - tail.begin();
        if (it == tail.end()) {
            tail.push_back(arr[i]);
            lis_indices.push_back(i);
        } else {
            *it = arr[i];
            lis_indices[idx] = i;
        }
        if (idx > 0) parent[i] = lis_indices[idx - 1];
    }

    vi seq;
    for (int i = lis_indices.back(); i != -1; i = parent[i]) {
        seq.push_back(arr[i]);
    }
    reverse(seq.begin(), seq.end());
    return seq;
}

// Longest Common Increasing Subsequence (LCIS) of arrays A and B
// Time: O(N * M), Space: O(M)

vi LCIS(const vi &a, const vi &b) {
    int n = a.size(), m = b.size();
    vi dp(m, 0), parent(m, -1);

    for (int i = 0; i < n; i++) {
        int best_len = 0, best_prev = -1;
        for (int j = 0; j < m; j++) {
            if (a[i] == b[j]) {
                if (best_len + 1 > dp[j]) {
                    dp[j] = best_len + 1;
                    parent[j] = best_prev;
                }
            } else if (a[i] > b[j]) {
                if (dp[j] > best_len) {
                    best_len = dp[j];
                    best_prev = j;
                }
            }
        }
    }

    int end = max_element(dp.begin(), dp.end()) - dp.begin();
    if (dp[end] == 0) return {};

    vi ans;
    while (end != -1) {
        ans.push_back(b[end]);
        end = parent[end];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

// Topological sort using Kahn's algorithm
// Time: O(V + E), Space: O(V + E)

vi topoSort(int n, const vvi &g) {
    vi indegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : g[u]) indegree[v]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    vi topo;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for (int v : g[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }
    return (int)topo.size() == n ? topo : vi{};
}

// All possible path lengths from node 1 to node N in DAG
// Time: O(V * N + E * N), Space: O(V * N)

vi possibleLengths(int n, const vvi &g) {
    vi topo = topoSort(n, g);
    static bool dp[MAXN][MAXN];
    memset(dp, 0, sizeof(dp));
    dp[1][0] = true;

    for (int u : topo) {
        for (int v : g[u]) {
            for (int len = 0; len < n; len++) {
                if (dp[u][len]) dp[v][len + 1] = true;
            }
        }
    }

    vi ans;
    for (int len = 0; len < n; len++) {
        if (dp[n][len]) ans.push_back(len);
    }
    return ans;
}