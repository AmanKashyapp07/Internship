#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const int MAXN = 1005;

struct Edge { int u, v; ll w; };
struct TreeNode { int val; TreeNode *left, *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };

// Backtracks through parent array to reconstruct cycle starting and ending at node start.
// Time Complexity: O(V), Space Complexity: O(V).
vector<int> buildCycle(int start, const vector<int> &parent) {
    vector<int> cycle; int cur = start;
    while (true) { cycle.push_back(cur); cur = parent[cur]; if (cur == start) break; }
    return cycle;
}

// Finds the length of the shortest cycle (girth) in an unweighted graph using BFS from every node.
// Time Complexity: O(V * (V + E)), Space Complexity: O(V).
int findShortestCycle(int n, const vvi &graph) {
    int minCycleLen = INT_MAX;
    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1), parent(n, -1); queue<int> q; dist[src] = 0; q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : graph[u]) {
                if (dist[v] == -1) { dist[v] = dist[u] + 1; parent[v] = u; q.push(v); }
                else if (parent[u] != v) minCycleLen = min(minCycleLen, dist[u] + dist[v] + 1);
            }
        }
    }
    return minCycleLen == INT_MAX ? -1 : minCycleLen;
}

// Identifies all nodes involved in cyclic dependencies using Kahn's topological peeling.
// Time Complexity: O(V + E), Space Complexity: O(V).
vector<int> getNodesInCycles(int n, const vvi &graph, vector<int> &indegree) {
    queue<int> q; vector<bool> isAcyclic(n, false); vector<int> cyclicNodes;
    for (int i = 0; i < n; i++) if (indegree[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop(); isAcyclic[u] = true;
        for (int v : graph[u]) if (--indegree[v] == 0) q.push(v);
    }
    for (int i = 0; i < n; i++) if (!isAcyclic[i]) cyclicNodes.push_back(i);
    return cyclicNodes;
}

// Checks if a graph is bipartite (2-colorable) using DFS graph coloring.
// Time Complexity: O(V + E), Space Complexity: O(V).
bool dfs_bipartite(int u, int color, vector<int> &colors, const vvi &graph) {
    colors[u] = color;
    for (int v : graph[u]) {
        if (colors[v] == -1) { if (!dfs_bipartite(v, 1 - color, colors, graph)) return false; }
        else if (colors[v] == color) return false;
    }
    return true;
}

// Computes single-source shortest path on a Directed Acyclic Graph (DAG) in linear time.
// Time Complexity: O(V + E), Space Complexity: O(V).
vi shortestPathDAG(int n, vector<vector<pair<int, int>>> &graph, int src) {
    vi indeg(n); for (int u = 0; u < n; u++) for (auto [v, wt] : graph[u]) indeg[v]++;
    vi dist(n, INT_MAX); dist[src] = 0; queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, wt] : graph[u]) {
            if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) dist[v] = dist[u] + wt;
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return dist;
}

// Computes bitwise AND of all numbers in range [left, right] by finding common prefix.
// Time Complexity: O(log(right)), Space Complexity: O(1).
int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) { left >>= 1; right >>= 1; shift++; }
    return left << shift;
}

// Finds the length of the longest nice subarray where bitwise AND of any two elements is 0.
// Time Complexity: O(n), Space Complexity: O(1).
int longestNiceSubarray(const vi &nums) {
    int n = nums.size(), left = 0, maxLength = 0, mask = 0;
    for (int i = 0; i < n; i++) {
        while ((mask & nums[i]) != 0) { mask ^= nums[left]; left++; }
        mask |= nums[i]; maxLength = max(maxLength, i - left + 1);
    }
    return maxLength;
}

// Computes the number of distinct bitwise ORs of all non-empty subarrays.
// Time Complexity: O(n log(max_val)), Space Complexity: O(n log(max_val)).
int subarrayBitwiseORs(vi &arr) {
    unordered_set<int> s1, s2;
    for (auto c : arr) {
        unordered_set<int> s3 = {c};
        for (auto v : s2) s3.insert(c | v);
        for (auto x : s3) s1.insert(x);
        s2 = s3;
    }
    return s1.size();
}

// Calculates total set bits (1s) from 1 to n across all bit positions.
// Time Complexity: O(log n), Space Complexity: O(1).
int countTotalSetBits(int n) {
    int total_ones = 0;
    for (int i = 0; i < 60; i++) {
        int lengthCycle = 1LL << (i + 1), totalCycles = (n + 1) / lengthCycle;
        int one = 1LL << i, zeroes = 1LL << i, remainder = (n + 1) % lengthCycle;
        total_ones += totalCycles * one;
        if (remainder > zeroes) total_ones += (remainder - zeroes);
    }
    return total_ones;
}

// Counts the number of subsets with sum equal to k using 0/1 Knapsack DP.
// Time Complexity: O(n * k), Space Complexity: O(k).
int CountSubsetsWithSumK(const vi &nums, int k) {
    vi dp(k + 1, 0); dp[0] = 1;
    for (int num : nums) for (int j = k; j >= num; j--) dp[j] += dp[j - num];
    return dp[k];
}

// Partitions array into two subsets minimizing absolute difference of their sums.
// Time Complexity: O(n * totalSum), Space Complexity: O(totalSum).
int minSubsetSumDifference(const vi &nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    vector<bool> dp(totalSum / 2 + 1, false); dp[0] = true;
    for (int num : nums) for (int j = totalSum / 2; j >= num; j--) dp[j] = dp[j] || dp[j - num];
    for (int j = totalSum / 2; j >= 0; j--) if (dp[j]) return totalSum - 2 * j;
    return totalSum;
}

// Finds all possible money sums that can be formed using given coins (Subset Sum DP).
// Time Complexity: O(n * sum(coins)), Space Complexity: O(sum(coins)).
vi getMoneySums(vi &coins) {
    int n = coins.size(); if (n == 0) return {};
    int total = accumulate(coins.begin(), coins.end(), 0);
    vector<bool> dp(total + 1, false); dp[0] = true;
    for (int coin : coins) for (int sum = total; sum >= coin; sum--) if (dp[sum - coin]) dp[sum] = true;
    vi possibleSums;
    for (int s = 1; s <= total; s++) if (dp[s]) possibleSums.push_back(s);
    return possibleSums;
}

// Counts the number of Longest Increasing Subsequences (LIS) in an array.
// Time Complexity: O(n^2), Space Complexity: O(n).
int CountOfLIS(const vector<int> &nums) {
    int n = nums.size(); if (n == 0) return 0;
    vector<int> length(n, 1), count(n, 1); int maxLength = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (length[j] + 1 > length[i]) { length[i] = length[j] + 1; count[i] = count[j]; }
                else if (length[j] + 1 == length[i]) count[i] += count[j];
            }
        }
        maxLength = max(maxLength, length[i]);
    }
    int totalCount = 0;
    for (int i = 0; i < n; i++) if (length[i] == maxLength) totalCount += count[i];
    return totalCount;
}

// Dijkstra's Algorithm for non-negative weighted single-source shortest paths.
// Time Complexity: O((V + E) log V), Space Complexity: O(V + E).
vi dijkstra(int n, const vector<vector<pair<int, int>>> &graph, int src) {
    vi dist(n, INT_MAX); dist[src] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, wt] : graph[u]) {
            if (dist[u] + wt < dist[v]) { dist[v] = dist[u] + wt; pq.push({dist[v], v}); }
        }
    }
    return dist;
}

// Bellman-Ford algorithm with negative cycle detection.
// Time Complexity: O(V * E), Space Complexity: O(V).
vi bellman_ford(int n, const vector<Edge> &edges, int src) {
    vector<long long> dist(n + 1, LLONG_MAX); dist[src] = 0;
    for (int i = 1; i <= n - 1; i++)
        for (const auto &e : edges) if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) dist[e.v] = dist[e.u] + e.w;
    for (const auto &e : edges)
        if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) throw runtime_error("Negative cycle detected");
    return vi(dist.begin() + 1, dist.end());
}

// BFS for single-source shortest paths on unweighted graphs.
// Time Complexity: O(V + E), Space Complexity: O(V).
vi shortestPathUnweighted(int n, const vvi &graph, int src) {
    vi dist(n, INT_MAX); dist[src] = 0; queue<int> q; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : graph[u]) if (dist[v] == INT_MAX) { dist[v] = dist[u] + 1; q.push(v); }
    }
    return dist;
}

// Floyd-Warshall All-Pairs Shortest Path algorithm.
// Time Complexity: O(V^3), Space Complexity: O(V^2).
vvi floyd_warshall(int n, const vvi &graph) {
    vvi dist = graph;
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    return dist;
}

// Detects negative cycles using Floyd-Warshall algorithm (dist[i][i] < 0).
// Time Complexity: O(V^3), Space Complexity: O(V^2).
bool cycle_detection_floyd(int n, const vvi &graph) {
    vvi dist = floyd_warshall(n, graph);
    for (int i = 0; i < n; i++) if (dist[i][i] < 0) return true;
    return false;
}

// Produces lexicographically smallest topological sort using a max-priority queue (Kahn's variant).
// Time Complexity: O(V log V + E), Space Complexity: O(V).
vi lexicographical_topo_sort(int n, const vvi &graph, vi indegree) {
    priority_queue<int> pq; for (int i = 1; i <= n; i++) if (indegree[i] == 0) pq.push(i);
    vi order;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop(); order.push_back(u);
        for (int v : graph[u]) if (--indegree[v] == 0) pq.push(v);
    }
    if ((int)order.size() != n) return {};
    reverse(order.begin(), order.end()); return order;
}

// Shortest path on DAG using Topological Ordering.
// Time Complexity: O(V + E), Space Complexity: O(V).
void DPonDAG(int n, const vector<vector<pair<int, int>>> &graph, int src) {
    vi indeg(n, 0); for (int u = 0; u < n; u++) for (auto [v, wt] : graph[u]) indeg[v]++;
    vi topo_order; queue<int> q; for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) { int u = q.front(); q.pop(); topo_order.push_back(u); for (auto [v, wt] : graph[u]) if (--indeg[v] == 0) q.push(v); }
    vi dist(n, INT_MAX); dist[src] = 0;
    for (auto u : topo_order) if (dist[u] != INT_MAX) for (auto [v, wt] : graph[u]) dist[v] = min((long long)dist[v], (long long)dist[u] + wt);
}

// Graph M-Coloring backtracking to count valid colorings of graph with m colors.
// Time Complexity: O(m^V), Space Complexity: O(V).
struct GraphMColouring {
    bool isValid(int node, int color, const vvi &graph, const vi &colors) {
        for (int neighbor : graph[node]) if (colors[neighbor] == color) return false;
        return true;
    }
    void dfs(int node, int m, const vvi &graph, vi &colors, int &count) {
        if (node == graph.size()) { count++; return; }
        for (int color = 1; color <= m; color++) {
            if (isValid(node, color, graph, colors)) {
                colors[node] = color; dfs(node + 1, m, graph, colors, count); colors[node] = 0;
            }
        }
    }
    int countColorings(int n, int m, const vvi &graph) {
        vi colors(n, 0); int count = 0; dfs(0, m, graph, colors, count); return count;
    }
};

// Finds cheapest flight price with at most k stops using Dijkstra / BFS queue state.
// Time Complexity: O(E * k), Space Complexity: O(V * k).
int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k) {
    vector<vector<pair<int, int>>> graph(n); for (auto &f : flights) graph[f[0]].push_back({f[1], f[2]});
    int maxFlights = k + 1; vvi dist(n, vi(maxFlights + 1, INT_MAX));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    dist[src][0] = 0; pq.push({0, src, 0});
    while (!pq.empty()) {
        auto [cost, node, flightsTaken] = pq.top(); pq.pop();
        if (cost > dist[node][flightsTaken]) continue;
        if (node == dst) return cost;
        for (auto &[next, price] : graph[node]) {
            if (flightsTaken + 1 <= maxFlights && cost + price < dist[next][flightsTaken + 1]) {
                dist[next][flightsTaken + 1] = cost + price; pq.push({cost + price, next, flightsTaken + 1});
            }
        }
    }
    return -1;
}

// Shortest path visiting all nodes in an undirected graph using Bitmask BFS.
// Time Complexity: O(V * 2^V), Space Complexity: O(V * 2^V).
int shortestPathVisitingAllNodes(int n, const vvi &graph) {
    int allVisitedMask = (1 << n) - 1; queue<pair<int, int>> q; vvi dist(n, vi(1 << n, INT_MAX));
    for (int i = 0; i < n; i++) { q.push({i, 1 << i}); dist[i][1 << i] = 0; }
    while (!q.empty()) {
        auto [node, mask] = q.front(); q.pop(); int currentDist = dist[node][mask];
        if (mask == allVisitedMask) return currentDist;
        for (int neighbor : graph[node]) {
            int nextMask = mask | (1 << neighbor);
            if (dist[neighbor][nextMask] > currentDist + 1) { dist[neighbor][nextMask] = currentDist + 1; q.push({neighbor, nextMask}); }
        }
    }
    return -1;
}

// Computes length of Shortest Common Supersequence (SCS = |s1| + |s2| - LCS).
// Time Complexity: O(|s1| * |s2|), Space Complexity: O(|s1| * |s2|).
int ShortestCommonSubsequence(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size(); vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
    return n + m - dp[n][m];
}

// Reconstructs the Shortest Common Supersequence string from DP matrix.
// Time Complexity: O(|s1| * |s2|), Space Complexity: O(|s1| * |s2|).
string SCS(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size(); vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
    string scs; int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) { scs += s1[i - 1]; i--; j--; }
        else if (dp[i - 1][j] > dp[i][j - 1]) { scs += s1[i - 1]; i--; }
        else { scs += s2[j - 1]; j--; }
    }
    while (i > 0) { scs += s1[i - 1]; i--; } while (j > 0) { scs += s2[j - 1]; j--; }
    reverse(scs.begin(), scs.end()); return scs;
}

// Minimum Window Subsequence (finds shortest substring of s containing t as a subsequence).
// Time Complexity: O(|s| * |t|), Space Complexity: O(1).
string minWindowSubsequence(string s, string t) {
    if (t.empty()) return "";
    int n = s.size(), m = t.size(), minLen = INT_MAX, startIdx = -1, i = 0, j = 0;
    while (i < n) {
        if (s[i] == t[j]) {
            j++;
            if (j == m) {
                int end = i + 1; j--;
                while (j >= 0) { if (s[i] == t[j]) j--; i--; }
                i++; j++;
                if (end - i < minLen) { minLen = end - i; startIdx = i; }
            }
        }
        i++;
    }
    return startIdx == -1 ? "" : s.substr(startIdx, minLen);
}

// Minimum Window Substring (finds shortest substring of s containing all characters of t).
// Time Complexity: O(|s| + |t|), Space Complexity: O(|s| + |t|).
string minWindowSubstring(string s, string t) {
    if (t.empty()) return "";
    unordered_map<char, int> need, window; for (char c : t) need[c]++;
    int left = 0, right = 0, valid = 0, start = 0, minLen = INT_MAX;
    while (right < s.size()) {
        char c = s[right++]; if (need.count(c)) { window[c]++; if (window[c] == need[c]) valid++; }
        while (valid == need.size()) {
            if (right - left < minLen) { start = left; minLen = right - left; }
            char d = s[left++]; if (need.count(d)) { if (window[d] == need[d]) valid--; window[d]--; }
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

// Matrix Chain Multiplication optimal multiplication cost computation.
// Time Complexity: O(N^3), Space Complexity: O(N^2).
struct MatrixChainMultiplication {
    int matrixChainOrder(const vector<int> &p) {
        int n = p.size() - 1; vvi dp(n, vi(n, 0));
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1; dp[i][j] = INT_MAX;
                for (int k = i; k < j; k++) dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1]);
            }
        }
        return dp[0][n - 1];
    }
};

// Counts total distinct subsequences of string s that match string t.
// Time Complexity: O(|s| * |t|), Space Complexity: O(|t|).
class DistinctSubsequences {
public:
    int numDistinct(string s, string t) {
        int n = s.size(), m = t.size(); vector<unsigned long long> dp(m + 1, 0); dp[0] = 1;
        for (int i = 1; i <= n; i++) for (int j = m; j >= 1; j--) if (s[i - 1] == t[j - 1]) dp[j] += dp[j - 1];
        return dp[m];
    }
};

// AVL/BST Tree Element structure representation.
// Time Complexity: O(1), Space Complexity: O(1).
struct Element {
    int key, height; Element *left, *right;
    Element(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

// Finds maximum sum of keys in any valid Binary Search Tree (BST) subtree.
// Time Complexity: O(N), Space Complexity: O(N).
class MaxSumBSTSolution {
public:
    int ans = 0;
    tuple<bool, int, int, int> dfs(TreeNode *node) {
        if (!node) return {true, INT_MAX, INT_MIN, 0};
        auto [leftBST, leftMin, leftMax, leftSum] = dfs(node->left);
        auto [rightBST, rightMin, rightMax, rightSum] = dfs(node->right);
        if (leftBST && rightBST && node->val > leftMax && node->val < rightMin) {
            int sum = leftSum + rightSum + node->val; ans = max(ans, sum);
            return {true, min(node->val, leftMin), max(node->val, rightMax), sum};
        }
        return {false, 0, 0, 0};
    }
    int maxSumBST(TreeNode *root) { dfs(root); return ans; }
};

// Recursively deletes all leaf nodes with a specified target value.
// Time Complexity: O(N), Space Complexity: O(N).
TreeNode *removeLeafNodes(TreeNode *root, int target) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, target); root->right = removeLeafNodes(root->right, target);
    if (!root->left && !root->right && root->val == target) return nullptr;
    return root;
}

// Finds minimum extra characters remaining after splitting string into dictionary words.
// Time Complexity: O(N^2), Space Complexity: O(N + total_dict_len).
class MinExtraCharSolution {
public:
    int n; vi dp; unordered_set<string> st;
    int solve(int i, string &s) {
        if (i == n) return 0; if (dp[i] != -1) return dp[i];
        int ans = 1 + solve(i + 1, s); string curr = "";
        for (int j = i; j < n; j++) {
            curr += s[j]; if (st.count(curr)) ans = min(ans, solve(j + 1, s));
        }
        return dp[i] = ans;
    }
    int minExtraChar(string s, vector<string> &dictionary) {
        n = s.size(); for (string &word : dictionary) st.insert(word);
        dp.assign(n, -1); return solve(0, s);
    }
};

// Converts bounded knapsack item with quantity k into O(log k) 0/1 items via binary splitting.
// Time Complexity: O(log k), Space Complexity: O(log k).
void addItemBinarySplit(int w, int v, int k, vi &weights, vi &values) {
    for (int take = 1; k > 0; take <<= 1) {
        int cnt = min(take, k); weights.push_back(cnt * w); values.push_back(cnt * v); k -= cnt;
    }
}

// Solves standard 0/1 Knapsack problem using 1D DP array.
// Time Complexity: O(N * W), Space Complexity: O(W).
int knapsack_01(int n, int W, const vi &weights, const vi &values) {
    vi dp(W + 1, 0);
    for (int i = 0; i < n; i++) for (int w = W; w >= weights[i]; w--) dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
    return dp[W];
}

// Calculates LCS length of two permutations in O(N log N) by mapping to LIS indices.
// Time Complexity: O(N log N), Space Complexity: O(N).
int LCSLengthOf2Permutations(vi &A, vi &B) {
    unordered_map<int, int> pos; for (int i = 0; i < A.size(); i++) pos[A[i]] = i;
    vi dp;
    for (int x : B) {
        if (!pos.count(x)) continue;
        auto it = lower_bound(dp.begin(), dp.end(), pos[x]);
        if (it == dp.end()) dp.push_back(pos[x]); else *it = pos[x];
    }
    return dp.size();
}

// Reconstructs the LCS sequence of two permutations in O(N log N) using LIS.
// Time Complexity: O(N log N), Space Complexity: O(N).
vi LCSof2Permutations(vi &A, vi &B) {
    unordered_map<int, int> pos; for (int i = 0; i < A.size(); i++) pos[A[i]] = i;
    vi p, val; for (int x : B) { if (!pos.count(x)) continue; p.push_back(pos[x]); val.push_back(x); }
    vi tail, tailIndex, parent(p.size(), -1);
    for (int i = 0; i < p.size(); i++) {
        int j = lower_bound(tail.begin(), tail.end(), p[i]) - tail.begin();
        if (j == tail.size()) { tail.push_back(p[i]); tailIndex.push_back(i); }
        else { tail[j] = p[i]; tailIndex[j] = i; }
        if (j > 0) parent[i] = tailIndex[j - 1];
    }
    vi lcs; if (tail.empty()) return lcs;
    for (int cur = tailIndex.back(); cur != -1; cur = parent[cur]) lcs.push_back(val[cur]);
    reverse(lcs.begin(), lcs.end()); return lcs;
}

// Computes length of Longest Increasing Subsequence (LIS) in O(N log N) using binary search.
// Time Complexity: O(N log N), Space Complexity: O(N).
int lis(vi &arr) {
    vi tail;
    for (int x : arr) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end()) tail.push_back(x); else *it = x;
    }
    return tail.size();
}

// Reconstructs the Longest Increasing Subsequence (LIS) array elements.
// Time Complexity: O(N log N), Space Complexity: O(N).
vi lisSequence(vi &arr) {
    vi tail, parent(arr.size(), -1), lis_indices;
    for (int i = 0; i < arr.size(); i++) {
        auto it = lower_bound(tail.begin(), tail.end(), arr[i]); int idx = it - tail.begin();
        if (it == tail.end()) { tail.push_back(arr[i]); lis_indices.push_back(i); }
        else { *it = arr[i]; lis_indices[idx] = i; }
        if (idx > 0) parent[i] = lis_indices[idx - 1];
    }
    vi seq; for (int i = lis_indices.back(); i != -1; i = parent[i]) seq.push_back(arr[i]);
    reverse(seq.begin(), seq.end()); return seq;
}

// Computes Longest Common Increasing Subsequence (LCIS) of arrays A and B.
// Time Complexity: O(N * M), Space Complexity: O(M).
vi LCIS(vi &A, vi &B) {
    int n = A.size(), m = B.size(); vi dp(m, 0), parent(m, -1);
    for (int i = 0; i < n; i++) {
        int bestLen = 0, bestPrev = -1;
        for (int j = 0; j < m; j++) {
            if (A[i] == B[j]) { if (bestLen + 1 > dp[j]) { dp[j] = bestLen + 1; parent[j] = bestPrev; } }
            else if (A[i] > B[j]) { if (dp[j] > bestLen) { bestLen = dp[j]; bestPrev = j; } }
        }
    }
    int end = max_element(dp.begin(), dp.end()) - dp.begin(); if (dp[end] == 0) return {};
    vi ans; while (end != -1) { ans.push_back(B[end]); end = parent[end]; }
    reverse(ans.begin(), ans.end()); return ans;
}

// Kahn's algorithm for Topological Sort on a directed graph.
// Time Complexity: O(V + E), Space Complexity: O(V + E).
vi topo_sort(int n, vvi &g) {
    vi indegree(n, 0); for (int u = 0; u < n; u++) for (int v : g[u]) indegree[v]++;
    queue<int> q; for (int i = 0; i < n; i++) if (indegree[i] == 0) q.push(i);
    vi topo;
    while (!q.empty()) {
        int u = q.front(); q.pop(); topo.push_back(u);
        for (int v : g[u]) if (--indegree[v] == 0) q.push(v);
    }
    return (int)topo.size() == n ? topo : vi{};
}

// Finds all possible path lengths from node 1 to node n in a DAG using DP.
// Time Complexity: O(V * N + E * N), Space Complexity: O(V * N).
vi possible_lengths(int n, vvi &graph) {
    vi topo = topo_sort(n, graph); static bool dp[MAXN][MAXN]; memset(dp, 0, sizeof(dp)); dp[1][0] = true;
    for (int u : topo)
        for (int v : graph[u])
            for (int len = 0; len < n; len++) if (dp[u][len]) dp[v][len + 1] = true;
    vi ans; for (int len = 0; len < n; len++) if (dp[n][len]) ans.push_back(len);
    return ans;
}