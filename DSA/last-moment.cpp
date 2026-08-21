/*
================================================================================
  LAST-MOMENT DSA REVISION SHEET FOR ONLINE ASSESSMENTS (TOP 62 TEMPLATES)
  File: DSA/last-moment.cpp
  Language: C++17 / C++20
================================================================================
  CONTENTS / INDEX:
  01. Fast I/O, PBDS & Custom Hash
  02. Binary Search Foundations
  03. Two Pointers & Fast-Slow Pointers
  04. Basic Sliding Window
  05. Monotonic Stack
  06. Monotonic Queue
  07. Prefix Sums & Difference Array
  08. Intervals Patterns
  09. Tree DP & Traversal (Diameter, Max Path Sum, LCA)
  10. Binary Search Tree (BST)
  11. Trie & Bitwise Trie
  12. Graph BFS (Standard, Multi-Source, 0-1 BFS)
  13. Graph DFS & Grid Traversal (Cycle Detection)
  14. Topological Sort (Kahn's & Cycle Detection)
  15. Disjoint Set Union (DSU)
  16. Dijkstra's Algorithm
  17. Bellman-Ford & SPFA (Negative Cycles)
  18. Floyd-Warshall (All-Pairs Shortest Path)
  19. Minimum Spanning Tree (Kruskal's & Prim's)
  20. 0/1 & Unbounded Knapsack
  21. String / Subsequence DP (LCS, Edit Distance, LIS in O(N log N))
  22. Grid DP (Unique Paths, Min Path Sum)
  23. Interval DP / MCM (Matrix Chain, Burst Balloons)
  24. Bitmask DP (Travelling Salesperson Problem - TSP)
  25. Digit DP Template
  26. KMP String Matching (Prefix Function)
  27. Rabin-Karp Rolling Hash (Double Hash)
  28. Z-Algorithm
  29. Heaps & Two Heaps (Median Finder)
  30. Backtracking Template (Subsets, Permutations, Combination Sum)
  31. Bit Manipulation Core Tricks
  32. Math: Binary Exponentiation, Ext GCD, Modular Inverse
  33. Sieve of Eratosthenes & SPF (Prime Factorization O(log N))
  34. Segment Tree & Fenwick Tree (Point Update)
  35. Tarjan's Bridges, Articulation Points & SCC
  36. Greedy Algorithms
  37. Kadane's Algorithm & Variants
  38. Linked List Patterns
  39. Stack / Expression Patterns
  40. Binary Tree Construction & Advanced Traversal
  41. Advanced Binary Search
  42. Advanced Sliding Window
  43. Coordinate Compression
  44. Sweep Line
  45. Meet in the Middle
  46. Advanced Segment Tree (Lazy Propagation)
  47. Tree Techniques (Euler Tour, Rerooting DP, Tree Difference)
  48. Advanced Graph Patterns (SCC Condensation, DAG DP, 2-SAT)
  49. Offline Query Techniques (Mo's Algorithm, Offline Fenwick)
  50. Number Theory & Combinatorics (nCr Mod, Euler Totient, Catalan)
  51. Advanced Bitmask Techniques (Submask Enumeration, XOR Basis)
  52. Manacher's Algorithm (O(N) Longest Palindromic Substring)
  53. Binary Lifting & Kth Ancestor (Tree Queries)
  54. Sparse Table (Static O(1) Range Minimum Query - RMQ)
  55. Sprague-Grundy Theorem & Nim Game (Game Theory)
  56. Boyer-Moore Majority Voting Algorithm (> N/2 & > N/3)
  57. Matrix Exponentiation (O(K^3 log N) Linear Recurrences)
  58. Quickselect Algorithm (O(N) Average Kth Element)
  59. Inversion Count (Merge Sort / Fenwick Tree)
  60. DSU on Tree / Sack (Small-to-Large Subtree Merging)
  61. LRU Cache Design (Doubly Linked List + Hash Map)
  62. Palindrome Partitioning DP (Min Cuts)

  LAST-MINUTE PATTERN SELECTION GUIDE (AT THE BOTTOM OF FILE)
================================================================================
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <chrono>

#if __has_include(<ext/pb_ds/assoc_container.hpp>)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
#endif

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;
using vvll = vector<vector<ll>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = 1e9 + 7;
const ll LINF = 1e18 + 7;
const ll MOD = 1e9 + 7;

// ============================================================
// 01. FAST I/O, PBDS & CUSTOM HASH
// Pattern: Speedup I/O, Order Statistics Set, Anti-Hack Hash
// Complexity: O(1) per op for Hash, O(log N) for PBDS
// ============================================================
void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

#if __has_include(<ext/pb_ds/assoc_container.hpp>)
// Supports: find_by_order(k) [0-indexed iterator], order_of_key(k) [count < k]
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#endif

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};
// Usage: unordered_map<ll, int, custom_hash> safe_map;

// ============================================================
// 02. BINARY SEARCH FOUNDATIONS
// Pattern: Lower/Upper Bound & Search on Answer
// Complexity: O(log N) time, O(1) space
// ============================================================
int bs_lower_bound(const vi& arr, int target) {
    int low = 0, high = arr.size();
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] >= target) high = mid;
        else low = mid + 1;
    }
    return low; // First index where arr[index] >= target
}

int bs_upper_bound(const vi& arr, int target) {
    int low = 0, high = arr.size();
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] > target) high = mid;
        else low = mid + 1;
    }
    return low; // First index where arr[index] > target
}

// Binary Search on Answer Template
bool isValidPredicate(ll mid, const vi& arr, int k) {
    // Return true if configuration 'mid' is feasible
    return true;
}

ll bs_on_answer(const vi& arr, int k, ll low, ll high) {
    ll ans = -1;
    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (isValidPredicate(mid, arr, k)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

// ============================================================
// 03. TWO POINTERS & FAST-SLOW POINTERS
// Pattern: Sorted array search, Floyd's cycle detection
// Complexity: O(N) time, O(1) space
// ============================================================
vvi threeSum(vi& nums) {
    sort(all(nums));
    vvi res;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                res.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++; right--;
            } else if (sum < 0) left++;
            else right--;
        }
    }
    return res;
}

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

bool hasCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// ============================================================
// 04. BASIC SLIDING WINDOW
// Pattern: Dynamic or fixed contiguous subarray processing
// Complexity: O(N) time, O(1) space
// ============================================================
int minSubArrayLen(int target, const vi& nums) {
    int n = nums.size(), left = 0, sum = 0, minLen = INF;
    for (int right = 0; right < n; right++) {
        sum += nums[right];
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left++];
        }
    }
    return minLen == INF ? 0 : minLen;
}

// ============================================================
// 05. MONOTONIC STACK
// Pattern: Next Greater/Smaller Element, Histogram Area
// Complexity: O(N) time, O(N) space
// ============================================================
vi nextGreaterElement(const vi& nums) {
    int n = nums.size();
    vi res(n, -1);
    stack<int> st; // Stores indices
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            res[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return res;
}

int largestRectangleArea(vi& heights) {
    heights.push_back(0); // Dummy sentinel
    int n = heights.size(), maxArea = 0;
    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && heights[st.top()] >= heights[i]) {
            int h = heights[st.top()];
            st.pop();
            int w = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    heights.pop_back();
    return maxArea;
}

// ============================================================
// 06. MONOTONIC QUEUE
// Pattern: Sliding Window Maximum / Minimum
// Complexity: O(N) time, O(K) space
// ============================================================
vi maxSlidingWindow(const vi& nums, int k) {
    deque<int> dq; // Indices with decreasing values
    vi res;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}

// ============================================================
// 07. PREFIX SUMS & DIFFERENCE ARRAY
// Pattern: Range Sum Queries, Range Updates
// Complexity: O(1) query, O(N) precomputation / update
// ============================================================
int subarraySumEqualsK(const vi& nums, int k) {
    unordered_map<int, int> prefCount;
    prefCount[0] = 1;
    int currSum = 0, count = 0;
    for (int x : nums) {
        currSum += x;
        if (prefCount.count(currSum - k)) count += prefCount[currSum - k];
        prefCount[currSum]++;
    }
    return count;
}

struct DifferenceArray {
    vi diff;
    DifferenceArray(int n) : diff(n + 1, 0) {}
    void add(int l, int r, int val) {
        diff[l] += val;
        diff[r + 1] -= val;
    }
    vi getArray() {
        vi res(diff.size() - 1);
        int sum = 0;
        for (size_t i = 0; i < res.size(); i++) {
            sum += diff[i];
            res[i] = sum;
        }
        return res;
    }
};

// ============================================================
// 08. INTERVALS PATTERNS
// Pattern: Merge Overlapping Intervals, Interval Intersection
// Complexity: O(N log N) time, O(N) space
// ============================================================
vvi mergeIntervals(vvi& intervals) {
    if (intervals.empty()) return {};
    sort(all(intervals));
    vvi res;
    res.push_back(intervals[0]);
    for (size_t i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] <= res.back()[1]) {
            res.back()[1] = max(res.back()[1], intervals[i][1]);
        } else {
            res.push_back(intervals[i]);
        }
    }
    return res;
}

// ============================================================
// 09. TREE DP & TRAVERSAL (DIAMETER, MAX PATH SUM, LCA)
// Pattern: Binary Tree DP & LCA via Binary Lifting
// Complexity: O(N) / Query O(log N) time
// ============================================================
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int treeDiameter(TreeNode* root, int& maxDia) {
    if (!root) return 0;
    int lh = treeDiameter(root->left, maxDia);
    int rh = treeDiameter(root->right, maxDia);
    maxDia = max(maxDia, lh + rh);
    return 1 + max(lh, rh);
}

struct TreeLCA {
    int n, LOG;
    vvi adj;
    vi depth;
    vvi up;

    TreeLCA(int n, const vvi& adj, int root = 1) : n(n), adj(adj) {
        LOG = 20;
        depth.resize(n + 1, 0);
        up.assign(n + 1, vi(LOG, 0));
        dfs(root, root, 0);
    }

    void dfs(int u, int p, int d) {
        depth[u] = d;
        up[u][0] = p;
        for (int j = 1; j < LOG; j++) up[u][j] = up[up[u][j - 1]][j - 1];
        for (int v : adj[u]) {
            if (v != p) dfs(v, u, d + 1);
        }
    }

    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        for (int j = LOG - 1; j >= 0; j--) {
            if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
        }
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }
};

// ============================================================
// 10. BINARY SEARCH TREE (BST)
// Pattern: BST Validation, Range Search
// Complexity: O(H) time, O(H) recursion stack
// ============================================================
bool isValidBST(TreeNode* root, long long minVal = -1e18, long long maxVal = 1e18) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isValidBST(root->left, minVal, root->val) && isValidBST(root->right, root->val, maxVal);
}

// ============================================================
// 11. TRIE & BITWISE TRIE
// Pattern: Prefix Match, Maximum XOR Pair
// Complexity: O(L) insertion/search (L = word length / 32 bits)
// ============================================================
struct TrieNode {
    TrieNode* children[26] = {};
    bool isEnd = false;
};

class Trie {
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }
    void insert(string word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->isEnd = true;
    }
    bool search(string word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr->isEnd;
    }
};

struct BitTrieNode {
    BitTrieNode* child[2] = {};
};

class BitTrie {
    BitTrieNode* root;
public:
    BitTrie() { root = new BitTrieNode(); }
    void insert(int num) {
        BitTrieNode* curr = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!curr->child[bit]) curr->child[bit] = new BitTrieNode();
            curr = curr->child[bit];
        }
    }
    int getMaxXOR(int num) {
        BitTrieNode* curr = root;
        int maxXor = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int opp = 1 - bit;
            if (curr->child[opp]) {
                maxXor |= (1 << i);
                curr = curr->child[opp];
            } else {
                curr = curr->child[bit];
            }
        }
        return maxXor;
    }
};

// ============================================================
// 12. GRAPH BFS (STANDARD, MULTI-SOURCE, 0-1 BFS)
// Pattern: Shortest Path Unweighted / 0-1 Weighted / Grid Waves
// Complexity: O(V + E) time, O(V) space
// ============================================================
vi zeroOneBFS(int start, int n, const vector<vector<pair<int, int>>>& adj) {
    vi dist(n + 1, INF);
    deque<int> dq;
    dist[start] = 0;
    dq.push_back(start);

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto& edge : adj[u]) {
            int v = edge.first, w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}

int multiSourceBFS(vvi& grid) {
    int r = grid.size(), c = grid[0].size();
    queue<pii> q;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] == 2) q.push({i, j});
        }
    }
    int steps = 0;
    int dR[] = {-1, 1, 0, 0}, dC[] = {0, 0, -1, 1};
    while (!q.empty()) {
        int sz = q.size();
        while (sz--) {
            auto [x, y] = q.front(); q.pop();
            for (int d = 0; d < 4; d++) {
                int nx = x + dR[d], ny = y + dC[d];
                if (nx >= 0 && nx < r && ny >= 0 && ny < c && grid[nx][ny] == 1) {
                    grid[nx][ny] = 2;
                    q.push({nx, ny});
                }
            }
        }
        if (!q.empty()) steps++;
    }
    return steps;
}

// ============================================================
// 13. GRAPH DFS & GRID TRAVERSAL (CYCLE DETECTION)
// Pattern: Connected Components, Directed Graph Cycle Detection
// Complexity: O(V + E) time, O(V) space
// ============================================================
bool dfsCycleDirected(int u, const vvi& adj, vi& state) {
    state[u] = 1; // Visiting
    for (int v : adj[u]) {
        if (state[v] == 1) return true; // Back edge
        if (state[v] == 0 && dfsCycleDirected(v, adj, state)) return true;
    }
    state[u] = 2; // Visited
    return false;
}

// ============================================================
// 14. TOPOLOGICAL SORT (KAHN'S & CYCLE DETECTION)
// Pattern: Dependency Ordering / Cycle Detection
// Complexity: O(V + E) time, O(V) space
// ============================================================
vi topoSortKahn(int n, const vvi& adj) {
    vi inDegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++) if (inDegree[i] == 0) q.push(i);

    vi topoOrder;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topoOrder.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    if ((int)topoOrder.size() != n) return {}; // Cycle detected
    return topoOrder;
}

// ============================================================
// 15. DISJOINT SET UNION (DSU)
// Pattern: Dynamic Connectivity, Merging Sets
// Complexity: O(alpha(N)) ~ O(1) amortized
// ============================================================
struct DSU {
    vi parent, sz;
    int components;
    DSU(int n) {
        components = n;
        parent.resize(n + 1);
        iota(all(parent), 0);
        sz.assign(n + 1, 1);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        i = find(i); j = find(j);
        if (i == j) return false;
        if (sz[i] < sz[j]) swap(i, j);
        parent[j] = i;
        sz[i] += sz[j];
        components--;
        return true;
    }
};

// ============================================================
// 16. DIJKSTRA'S ALGORITHM
// Pattern: Non-negative weighted single-source shortest path
// Complexity: O((V + E) log V) time, O(V) space
// ============================================================
vll dijkstra(int start, int n, const vector<vector<pair<int, ll>>>& adj) {
    vll dist(n + 1, LINF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& edge : adj[u]) {
            int v = edge.first;
            ll w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// ============================================================
// 17. BELLMAN-FORD & SPFA (NEGATIVE CYCLES)
// Pattern: Negative edge shortest path & negative cycle detection
// Complexity: SPFA Average O(E), Worst O(V * E)
// ============================================================
bool spfa(int start, int n, const vector<vector<pair<int, int>>>& adj, vi& dist) {
    dist.assign(n + 1, INF);
    vi count(n + 1, 0);
    vector<bool> inQueue(n + 1, false);
    queue<int> q;

    dist[start] = 0;
    q.push(start);
    inQueue[start] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        inQueue[u] = false;

        for (auto& edge : adj[u]) {
            int v = edge.first, w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (!inQueue[v]) {
                    q.push(v);
                    inQueue[v] = true;
                    if (++count[v] > n) return true; // Negative cycle
                }
            }
        }
    }
    return false;
}

// ============================================================
// 18. FLOYD-WARSHALL (ALL-PAIRS SHORTEST PATH)
// Pattern: All pairs shortest path for small V (V <= 400)
// Complexity: O(V^3) time, O(V^2) space
// ============================================================
void floydWarshall(vvi& dist, int n) {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

// ============================================================
// 19. MINIMUM SPANNING TREE (KRUSKAL'S & PRIM'S)
// Pattern: MST on connected weighted undirected graph
// Complexity: O(E log E) time
// ============================================================
struct Edge { int u, v; ll weight; };
ll kruskalMST(int n, vector<Edge>& edges) {
    sort(all(edges), [](const Edge& a, const Edge& b) { return a.weight < b.weight; });
    DSU dsu(n);
    ll mstWeight = 0, edgesAdded = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mstWeight += e.weight;
            if (++edgesAdded == n - 1) break;
        }
    }
    return (edgesAdded == n - 1) ? mstWeight : -1;
}

// ============================================================
// 20. 0/1 & UNBOUNDED KNAPSACK
// Pattern: Subset selection optimization (1D space optimized)
// Complexity: O(N * W) time, O(W) space
// ============================================================
int knapsack01(const vi& wt, const vi& val, int W) {
    vi dp(W + 1, 0);
    for (size_t i = 0; i < wt.size(); i++) {
        for (int w = W; w >= wt[i]; w--) { // Reverse for 0/1
            dp[w] = max(dp[w], val[i] + dp[w - wt[i]]);
        }
    }
    return dp[W];
}

int unboundedKnapsack(const vi& wt, const vi& val, int W) {
    vi dp(W + 1, 0);
    for (size_t i = 0; i < wt.size(); i++) {
        for (int w = wt[i]; w <= W; w++) { // Forward for unbounded
            dp[w] = max(dp[w], val[i] + dp[w - wt[i]]);
        }
    }
    return dp[W];
}

// ============================================================
// 21. STRING / SUBSEQUENCE DP (LCS, EDIT DISTANCE, LIS O(N log N))
// Pattern: String alignment, Longest Increasing Subsequence
// Complexity: LCS O(N*M), LIS O(N log N)
// ============================================================
int lcs(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    vi prev(n + 1, 0), curr(n + 1, 0);
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) curr[j] = 1 + prev[j - 1];
            else curr[j] = max(prev[j], curr[j - 1]);
        }
        prev = curr;
    }
    return prev[n];
}

int lis(const vi& nums) {
    vi tails;
    for (int x : nums) {
        auto it = lower_bound(all(tails), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

// ============================================================
// 22. GRID DP (UNIQUE PATHS, MIN PATH SUM)
// Pattern: Matrix traversal DP
// Complexity: O(R * C) time, O(C) space
// ============================================================
int minPathSum(const vvi& grid) {
    int r = grid.size(), c = grid[0].size();
    vi dp(c, INF);
    dp[0] = 0;
    for (int i = 0; i < r; i++) {
        dp[0] += grid[i][0];
        for (int j = 1; j < c; j++) {
            dp[j] = grid[i][j] + min(dp[j], dp[j - 1]);
        }
    }
    return dp[c - 1];
}

// ============================================================
// 23. INTERVAL DP / MCM (MATRIX CHAIN, BURST BALLOONS)
// Pattern: Range DP over subsegments [i, j]
// Complexity: O(N^3) time, O(N^2) space
// ============================================================
int matrixChainMultiplication(const vi& arr) {
    int n = arr.size();
    vvi dp(n, vi(n, 0));
    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INF;
            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + arr[i - 1] * arr[k] * arr[j];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    return dp[1][n - 1];
}

// ============================================================
// 24. BITMASK DP (TRAVELLING SALESPERSON PROBLEM - TSP)
// Pattern: Exponential state reduction using bitmasks (N <= 20)
// Complexity: O(N^2 * 2^N) time, O(N * 2^N) space
// ============================================================
int tsp(int u, int mask, int n, const vvi& dist, vvi& memo) {
    if (mask == (1 << n) - 1) return dist[u][0];
    if (memo[u][mask] != -1) return memo[u][mask];
    int ans = INF;
    for (int v = 0; v < n; v++) {
        if (!(mask & (1 << v))) {
            ans = min(ans, dist[u][v] + tsp(v, mask | (1 << v), n, dist, memo));
        }
    }
    return memo[u][mask] = ans;
}

// ============================================================
// 25. DIGIT DP TEMPLATE
// Pattern: Counting valid numbers in range [0, N]
// Complexity: O(Digits * State * 10) time
// ============================================================
string digitNumStr;
int memoDigit[20][180][2];

int solveDigitDP(int idx, int currSum, bool isTight, int targetSum) {
    if (idx == (int)digitNumStr.size()) return currSum == targetSum;
    if (memoDigit[idx][currSum][isTight] != -1) return memoDigit[idx][currSum][isTight];

    int limit = isTight ? (digitNumStr[idx] - '0') : 9;
    int ans = 0;
    for (int d = 0; d <= limit; d++) {
        ans += solveDigitDP(idx + 1, currSum + d, isTight && (d == limit), targetSum);
    }
    return memoDigit[idx][currSum][isTight] = ans;
}

// ============================================================
// 26. KMP STRING MATCHING (PREFIX FUNCTION)
// Pattern: String matching via Pi-table
// Complexity: O(N + M) time, O(M) space
// ============================================================
vi computePrefixFunction(string s) {
    int n = s.size();
    vi pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// ============================================================
// 27. RABIN-KARP ROLLING HASH (DOUBLE HASH)
// Pattern: Substring hash equality queries in O(1)
// Complexity: O(N) precomputation, O(1) query
// ============================================================
struct DoubleStringHash {
    string s;
    int n;
    const ll M1 = 1e9 + 7, M2 = 1e9 + 9;
    const ll P1 = 31, P2 = 37;
    vll h1, h2, p1, p2;

    DoubleStringHash(string str) : s(str), n(str.size()) {
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * P1 + (s[i] - 'a' + 1)) % M1;
            h2[i + 1] = (h2[i] * P2 + (s[i] - 'a' + 1)) % M2;
            p1[i + 1] = (p1[i] * P1) % M1;
            p2[i + 1] = (p2[i] * P2) % M2;
        }
    }

    pll getHash(int l, int r) {
        ll hash1 = (h1[r + 1] - h1[l] * p1[r - l + 1] % M1 + M1) % M1;
        ll hash2 = (h2[r + 1] - h2[l] * p2[r - l + 1] % M2 + M2) % M2;
        return {hash1, hash2};
    }
};

// ============================================================
// 28. Z-ALGORITHM
// Pattern: Exact match lengths at every suffix
// Complexity: O(N) time, O(N) space
// ============================================================
vi zAlgorithm(string s) {
    int n = s.size();
    vi z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }
    return z;
}

// ============================================================
// 29. HEAPS & TWO HEAPS (MEDIAN FINDER)
// Pattern: Continuous median tracking / Dual PQ
// Complexity: O(log N) per update, O(1) query
// ============================================================
class MedianFinder {
    priority_queue<int> maxHeap; // Lower half
    priority_queue<int, vi, greater<int>> minHeap; // Upper half
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

// ============================================================
// 30. BACKTRACKING TEMPLATE
// Pattern: Generating Subsets, Permutations, Combinations
// Complexity: O(2^N) or O(N!)
// ============================================================
void generateSubsets(int idx, vi& nums, vi& current, vvi& result) {
    result.push_back(current);
    for (size_t i = idx; i < nums.size(); i++) {
        current.push_back(nums[i]);
        generateSubsets(i + 1, nums, current, result);
        current.pop_back();
    }
}

// ============================================================
// 31. BIT MANIPULATION CORE TRICKS
// Pattern: Kernighan, LSB, Submask enumeration
// Complexity: O(Set bits) / O(3^N for all submasks)
// ============================================================
void bitTricks(int mask) {
    // Count set bits
    int count = __builtin_popcount(mask);
    // Lowest set bit
    int lsb = mask & (-mask);
    // Iterate submasks of mask
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // sub is a valid non-empty submask of mask
    }
}

// ============================================================
// 32. MATH: BINARY EXPONENTIATION, EXT GCD, MOD INVERSE
// Pattern: Modular Arithmetic, Inverse
// Complexity: O(log EXP) time
// ============================================================
ll power(ll base, ll exp, ll mod = MOD) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

ll modInverse(ll a, ll mod = MOD) {
    return power(a, mod - 2, mod);
}

// ============================================================
// 33. SIEVE OF ERATOSTHENES & SPF (FACTORIZATION O(log N))
// Pattern: Fast Prime Factorization up to 1e6
// Complexity: O(N log log N) precomp, O(log X) query
// ============================================================
const int MAXN = 1e6 + 5;
int spf[MAXN];

void buildSPF() {
    for (int i = 1; i < MAXN; i++) spf[i] = i;
    for (int i = 2; i * i < MAXN; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j < MAXN; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

vi getPrimeFactorization(int x) {
    vi factors;
    while (x > 1) {
        factors.push_back(spf[x]);
        x /= spf[x];
    }
    return factors;
}

// ============================================================
// 34. SEGMENT TREE & FENWICK TREE (POINT UPDATE)
// Pattern: Dynamic Range Sum / Query
// Complexity: O(log N) update/query
// ============================================================
struct FenwickTree {
    int n;
    vi tree;
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void update(int i, int delta) { for (; i <= n; i += i & -i) tree[i] += delta; }
    int query(int i) { int s = 0; for (; i > 0; i -= i & -i) s += tree[i]; return s; }
    int queryRange(int l, int r) { return query(r) - query(l - 1); }
};

// ============================================================
// 35. TARJAN'S BRIDGES, ARTICULATION POINTS & SCC
// Pattern: Bridges, Articulation Points & Strongly Connected Components
// Complexity: O(V + E) time, O(V) space
// ============================================================
void dfsBridges(int u, int p, int& timer, vi& tin, vi& low, const vvi& adj, vector<pii>& bridges) {
    tin[u] = low[u] = ++timer;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (tin[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            dfsBridges(v, u, timer, tin, low, adj, bridges);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back({u, v});
        }
    }
}

// ============================================================
// 36. GREEDY ALGORITHMS
// Pattern: Activity Selection, Jump Game I/II, Gas Station, Fractional Knapsack
// Complexity: O(N) or O(N log N) time
// ============================================================
int maxActivities(vvi& intervals) {
    sort(all(intervals), [](const vi& a, const vi& b) { return a[1] < b[1]; });
    int count = 0, lastEnd = -1;
    for (const auto& in : intervals) {
        if (in[0] >= lastEnd) { count++; lastEnd = in[1]; }
    }
    return count;
}

bool canJump(const vi& nums) {
    int maxReach = 0, n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i > maxReach) return false;
        maxReach = max(maxReach, i + nums[i]);
    }
    return true;
}

int jumpMin(const vi& nums) {
    int jumps = 0, currEnd = 0, farthest = 0;
    for (int i = 0; i < (int)nums.size() - 1; i++) {
        farthest = max(farthest, i + nums[i]);
        if (i == currEnd) {
            jumps++;
            currEnd = farthest;
        }
    }
    return currEnd >= (int)nums.size() - 1 ? jumps : -1;
}

int canCompleteCircuit(const vi& gas, const vi& cost) {
    int total = 0, curr = 0, start = 0;
    for (size_t i = 0; i < gas.size(); i++) {
        int diff = gas[i] - cost[i];
        total += diff;
        curr += diff;
        if (curr < 0) { start = i + 1; curr = 0; }
    }
    return total >= 0 ? start : -1;
}

// ============================================================
// 37. KADANE'S ALGORITHM & VARIANTS
// Pattern: Max Subarray, Circular Max Subarray, Max Product Subarray
// Complexity: O(N) time, O(1) space
// ============================================================
int maxSubArray(const vi& nums) {
    int maxSoFar = nums[0], curr = nums[0];
    for (size_t i = 1; i < nums.size(); i++) {
        curr = max(nums[i], curr + nums[i]);
        maxSoFar = max(maxSoFar, curr);
    }
    return maxSoFar;
}

int maxSubarraySumCircular(const vi& nums) {
    int total = 0, maxSoFar = nums[0], maxCurr = 0;
    int minSoFar = nums[0], minCurr = 0;
    for (int x : nums) {
        maxCurr = max(x, maxCurr + x);
        maxSoFar = max(maxSoFar, maxCurr);
        minCurr = min(x, minCurr + x);
        minSoFar = min(minSoFar, minCurr);
        total += x;
    }
    return maxSoFar > 0 ? max(maxSoFar, total - minSoFar) : maxSoFar;
}

int maxProductSubarray(const vi& nums) {
    int res = nums[0], maxP = nums[0], minP = nums[0];
    for (size_t i = 1; i < nums.size(); i++) {
        if (nums[i] < 0) swap(maxP, minP);
        maxP = max(nums[i], maxP * nums[i]);
        minP = min(nums[i], minP * nums[i]);
        res = max(res, maxP);
    }
    return res;
}

// ============================================================
// 38. LINKED LIST PATTERNS
// Pattern: In-place Reversal, Merge K Lists, Cycle, Palindrome
// Complexity: O(N) time, O(1) auxiliary space
// ============================================================
ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr, *curr = head;
    while (curr) {
        ListNode* nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    return prev;
}

ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode* curr = head;
    int count = 0;
    while (curr && count < k) { curr = curr->next; count++; }
    if (count < k) return head;

    ListNode *prev = nullptr, *node = head;
    for (int i = 0; i < k; i++) {
        ListNode* nextNode = node->next;
        node->next = prev;
        prev = node;
        node = nextNode;
    }
    head->next = reverseKGroup(node, k);
    return prev;
}

ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    for (auto l : lists) if (l) pq.push(l);

    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!pq.empty()) {
        ListNode* node = pq.top(); pq.pop();
        tail->next = node;
        tail = tail->next;
        if (node->next) pq.push(node->next);
    }
    return dummy.next;
}

// ============================================================
// 39. STACK / EXPRESSION PATTERNS
// Pattern: Valid Parentheses, Min Stack, Evaluate Postfix, Decode String
// Complexity: O(N) time, O(N) space
// ============================================================
class MinStack {
    stack<pair<int, int>> st; // {val, minSoFar}
public:
    void push(int val) {
        int m = st.empty() ? val : min(val, st.top().second);
        st.push({val, m});
    }
    void pop() { st.pop(); }
    int top() { return st.top().first; }
    int getMin() { return st.top().second; }
};

string decodeString(string s) {
    stack<int> countSt;
    stack<string> strSt;
    string currStr = "";
    int k = 0;
    for (char ch : s) {
        if (isdigit(ch)) { k = k * 10 + (ch - '0'); }
        else if (ch == '[') {
            countSt.push(k);
            strSt.push(currStr);
            currStr = "";
            k = 0;
        } else if (ch == ']') {
            string temp = currStr;
            currStr = strSt.top(); strSt.pop();
            int count = countSt.top(); countSt.pop();
            while (count--) currStr += temp;
        } else { currStr += ch; }
    }
    return currStr;
}

// ============================================================
// 40. BINARY TREE CONSTRUCTION & ADVANCED TRAVERSAL
// Pattern: Serialize/Deserialize, Pre+In Build, Morris Traversal
// Complexity: O(N) time, O(N) or O(1) space
// ============================================================
TreeNode* buildTreePreIn(vi& preorder, vi& inorder) {
    unordered_map<int, int> inMap;
    for (int i = 0; i < (int)inorder.size(); i++) inMap[inorder[i]] = i;

    function<TreeNode*(int, int, int, int)> build = [&](int preS, int preE, int inS, int inE) -> TreeNode* {
        if (preS > preE || inS > inE) return nullptr;
        TreeNode* root = new TreeNode(preorder[preS]);
        int inRoot = inMap[root->val];
        int numsLeft = inRoot - inS;

        root->left = build(preS + 1, preS + numsLeft, inS, inRoot - 1);
        root->right = build(preS + numsLeft + 1, preE, inRoot + 1, inE);
        return root;
    };
    return build(0, preorder.size() - 1, 0, inorder.size() - 1);
}

vi morrisInorder(TreeNode* root) {
    vi res;
    TreeNode* curr = root;
    while (curr) {
        if (!curr->left) {
            res.push_back(curr->val);
            curr = curr->right;
        } else {
            TreeNode* prev = curr->left;
            while (prev->right && prev->right != curr) prev = prev->right;
            if (!prev->right) {
                prev->right = curr;
                curr = curr->left;
            } else {
                prev->right = nullptr;
                res.push_back(curr->val);
                curr = curr->right;
            }
        }
    }
    return res;
}

// ============================================================
// 41. ADVANCED BINARY SEARCH
// Pattern: Rotated array, Peak element, 2D matrix, Median of 2 arrays
// Complexity: O(log N) or O(log(min(M, N)))
// ============================================================
int searchRotated(const vi& nums, int target) {
    int low = 0, high = (int)nums.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) return mid;
        if (nums[low] <= nums[mid]) {
            if (nums[low] <= target && target < nums[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (nums[mid] < target && target <= nums[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return -1;
}

int findPeakElement(const vi& nums) {
    int low = 0, high = (int)nums.size() - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] < nums[mid + 1]) low = mid + 1;
        else high = mid;
    }
    return low;
}

// ============================================================
// 42. ADVANCED SLIDING WINDOW
// Pattern: Exactly K = AtMost(K) - AtMost(K-1), Min Window Substring
// Complexity: O(N) time, O(1) or O(K) space
// ============================================================
string minWindowSubstring(string s, string t) {
    vi freq(128, 0);
    for (char c : t) freq[c]++;
    int count = t.size(), left = 0, minLen = INF, startIdx = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        if (freq[s[right]]-- > 0) count--;
        while (count == 0) {
            if (right - left + 1 < minLen) {
                minLen = right - left + 1;
                startIdx = left;
            }
            if (++freq[s[left++]] > 0) count++;
        }
    }
    return minLen == INF ? "" : s.substr(startIdx, minLen);
}

// ============================================================
// 43. COORDINATE COMPRESSION
// Pattern: Mapping large domain [1, 1e9] to compact [0, N-1]
// Complexity: O(N log N) time
// ============================================================
vi coordinateCompress(const vi& vals) {
    vi sorted = vals;
    sort(all(sorted));
    sorted.erase(unique(all(sorted)), sorted.end());
    vi res(vals.size());
    for (size_t i = 0; i < vals.size(); i++) {
        res[i] = lower_bound(all(sorted), vals[i]) - sorted.begin();
    }
    return res;
}

// ============================================================
// 44. SWEEP LINE
// Pattern: Event Processing / Interval Overlap / Meeting Rooms
// Complexity: O(N log N) time
// ============================================================
int maxOverlappingIntervals(const vvi& intervals) {
    vector<pair<int, int>> events;
    for (const auto& in : intervals) {
        events.push_back({in[0], +1});
        events.push_back({in[1], -1});
    }
    sort(all(events), [](const pii& a, const pii& b) {
        return a.first == b.first ? a.second < b.second : a.first < b.first;
    });

    int active = 0, maxActive = 0;
    for (const auto& ev : events) {
        active += ev.second;
        maxActive = max(maxActive, active);
    }
    return maxActive;
}

// ============================================================
// 45. MEET IN THE MIDDLE
// Pattern: Split array N <= 40 into two N/2 halves -> 2^(N/2)
// Complexity: O(2^(N/2) * N) time
// ============================================================
ll minSubsetSumDiff(const vll& arr, ll target) {
    int n = arr.size();
    int mid = n / 2;
    vll leftSums, rightSums;

    for (int mask = 0; mask < (1 << mid); mask++) {
        ll s = 0;
        for (int i = 0; i < mid; i++) if (mask & (1 << i)) s += arr[i];
        leftSums.push_back(s);
    }
    for (int mask = 0; mask < (1 << (n - mid)); mask++) {
        ll s = 0;
        for (int i = 0; i < n - mid; i++) if (mask & (1 << i)) s += arr[mid + i];
        rightSums.push_back(s);
    }

    sort(all(rightSums));
    ll ans = LINF;
    for (ll l : leftSums) {
        auto it = lower_bound(all(rightSums), target - l);
        if (it != rightSums.end()) ans = min(ans, abs(target - (l + *it)));
        if (it != rightSums.begin()) ans = min(ans, abs(target - (l + *prev(it))));
    }
    return ans;
}

// ============================================================
// 46. ADVANCED SEGMENT TREE (LAZY PROPAGATION)
// Pattern: Range Add / Set Update + Range Sum Query
// Complexity: O(N) build, O(log N) range update / query
// ============================================================
struct LazySegTree {
    int n;
    vll tree, lazy;
    LazySegTree(int n) : n(n), tree(4 * n, 0), lazy(4 * n, 0) {}

    void push(int node, int start, int end) {
        if (lazy[node] != 0) {
            tree[node] += (end - start + 1) * lazy[node];
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void updateRange(int node, int start, int end, int l, int r, ll val) {
        push(node, start, end);
        if (start > end || start > r || end < l) return;
        if (start >= l && end <= r) {
            lazy[node] += val;
            push(node, start, end);
            return;
        }
        int mid = start + (end - start) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    ll queryRange(int node, int start, int end, int l, int r) {
        push(node, start, end);
        if (start > end || start > r || end < l) return 0;
        if (start >= l && end <= r) return tree[node];
        int mid = start + (end - start) / 2;
        return queryRange(2 * node, start, mid, l, r) + queryRange(2 * node + 1, mid + 1, end, l, r);
    }
};

// ============================================================
// 47. TREE TECHNIQUES (EULER TOUR, REROOTING DP, TREE DIFFERENCE)
// Pattern: Flatten tree to array segment [tin[u], tout[u]] for range queries
// Complexity: O(N) traversal, O(log N) query
// ============================================================
struct EulerTour {
    int timer = 0;
    vi tin, tout;
    EulerTour(int n) : tin(n + 1), tout(n + 1) {}
    void dfs(int u, int p, const vvi& adj) {
        tin[u] = ++timer;
        for (int v : adj[u]) if (v != p) dfs(v, u, adj);
        tout[u] = timer;
    }
};

// ============================================================
// 48. ADVANCED GRAPH PATTERNS (SCC CONDENSATION, DAG DP, 2-SAT)
// Pattern: Condense SCCs into DAG for DP
// Complexity: O(V + E)
// ============================================================
struct SCC {
    int n, timer = 0, sccCount = 0;
    vvi adj;
    vi tin, low, sccId;
    stack<int> st;
    vector<bool> inStack;

    SCC(int n, const vvi& adj) : n(n), adj(adj), tin(n + 1, 0), low(n + 1, 0), sccId(n + 1, 0), inStack(n + 1, false) {
        for (int i = 1; i <= n; i++) if (!tin[i]) dfs(i);
    }

    void dfs(int u) {
        tin[u] = low[u] = ++timer;
        st.push(u);
        inStack[u] = true;

        for (int v : adj[u]) {
            if (!tin[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], tin[v]);
            }
        }

        if (low[u] == tin[u]) {
            sccCount++;
            while (true) {
                int v = st.top(); st.pop();
                inStack[v] = false;
                sccId[v] = sccCount;
                if (u == v) break;
            }
        }
    }
};

// ============================================================
// 49. OFFLINE QUERY TECHNIQUES (MO'S ALGORITHM)
// Pattern: Sort queries by block to process efficiently
// Complexity: O((N + Q) * sqrt(N))
// ============================================================
struct Query {
    int l, r, id, block;
    bool operator<(const Query& other) const {
        if (block != other.block) return block < other.block;
        return (block & 1) ? r < other.r : r > other.r;
    }
};

vi mosAlgorithm(const vi& arr, vector<Query>& queries) {
    int n = arr.size(), q = queries.size();
    int blockSize = max(1, (int)(n / sqrt(q)));
    for (int i = 0; i < q; i++) queries[i].block = queries[i].l / blockSize;
    sort(all(queries));

    vi res(q);
    int curL = 0, curR = -1;
    return res;
}

// ============================================================
// 50. NUMBER THEORY & COMBINATORICS
// Pattern: nCr % P via Factorial Precomputation & Euler Totient
// Complexity: O(N) precomp, O(1) per nCr query
// ============================================================
const int MAXCOMB = 1e6 + 5;
ll fact[MAXCOMB], invFact[MAXCOMB];

void precomputeCombinatorics() {
    fact[0] = invFact[0] = 1;
    for (int i = 1; i < MAXCOMB; i++) fact[i] = (fact[i - 1] * i) % MOD;
    invFact[MAXCOMB - 1] = modInverse(fact[MAXCOMB - 1]);
    for (int i = MAXCOMB - 2; i >= 1; i--) invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

int phi(int n) {
    int result = n;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// ============================================================
// 51. ADVANCED BITMASK TECHNIQUES (XOR LINEAR BASIS)
// Pattern: Find maximum XOR subset or linear independence
// Complexity: O(BITS) per insertion
// ============================================================
struct XORBasis {
    int basis[32] = {};
    int sz = 0;

    void insert(int mask) {
        for (int i = 31; i >= 0; i--) {
            if (!(mask & (1 << i))) continue;
            if (!basis[i]) {
                basis[i] = mask;
                sz++;
                return;
            }
            mask ^= basis[i];
        }
    }

    int getMaxXOR() {
        int maxXor = 0;
        for (int i = 31; i >= 0; i--) {
            if ((maxXor ^ basis[i]) > maxXor) maxXor ^= basis[i];
        }
        return maxXor;
    }
};

// ============================================================
// 52. MANACHER'S ALGORITHM
// Pattern: Longest Palindromic Substring in linear time
// Complexity: O(N) time, O(N) space
// ============================================================
string manacher(string s) {
    string t = "^";
    for (char c : s) { t += "#"; t += c; }
    t += "#$";
    int n = t.size();
    vi p(n, 0);
    int c = 0, r = 0;
    for (int i = 1; i < n - 1; i++) {
        int i_mirror = 2 * c - i;
        if (r > i) p[i] = min(r - i, p[i_mirror]);
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
        if (i + p[i] > r) { c = i; r = i + p[i]; }
    }
    int maxLen = 0, centerIdx = 0;
    for (int i = 1; i < n - 1; i++) {
        if (p[i] > maxLen) { maxLen = p[i]; centerIdx = i; }
    }
    int start = (centerIdx - maxLen) / 2;
    return s.substr(start, maxLen);
}

// ============================================================
// 53. BINARY LIFTING (KTH ANCESTOR & LCA)
// Pattern: O(log N) tree queries for Kth parent & LCA
// Complexity: O(N log N) precomp, O(log N) per query
// ============================================================
struct BinaryLiftingTree {
    int n, LOG;
    vvi up;
    vi depth;

    BinaryLiftingTree(int n, int root, const vvi& adj) : n(n) {
        LOG = 20;
        up.assign(n + 1, vi(LOG, 0));
        depth.assign(n + 1, 0);
        dfs(root, root, 0, adj);
    }

    void dfs(int u, int p, int d, const vvi& adj) {
        depth[u] = d;
        up[u][0] = p;
        for (int j = 1; j < LOG; j++) up[u][j] = up[up[u][j - 1]][j - 1];
        for (int v : adj[u]) {
            if (v != p) dfs(v, u, d + 1, adj);
        }
    }

    int getKthAncestor(int node, int k) {
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) {
                node = up[node][j];
                if (!node) break;
            }
        }
        return node;
    }

    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        for (int j = LOG - 1; j >= 0; j--) {
            if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
        }
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
        }
        return up[u][0];
    }
};

// ============================================================
// 54. SPARSE TABLE (STATIC RMQ)
// Pattern: Range Minimum Query / GCD without updates in O(1)
// Complexity: O(N log N) precomp, O(1) query
// ============================================================
struct SparseTable {
    int n, LOG;
    vvi st;
    vi lg;

    SparseTable(const vi& arr) {
        n = arr.size();
        LOG = 20;
        st.assign(n, vi(LOG));
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;

        for (int i = 0; i < n; i++) st[i][0] = arr[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    int queryMin(int l, int r) {
        int j = lg[r - l + 1];
        return min(st[l][j], st[r - (1 << j) + 1][j]);
    }
};

// ============================================================
// 55. SPRAGUE-GRUNDY THEOREM / NIM GAME
// Pattern: Impartial Game Theory (Nim sum & MEX)
// Complexity: O(N) for Nim, O(States * Transitions) for Grundy
// ============================================================
bool nimGameWins(const vi& piles) {
    int xorSum = 0;
    for (int p : piles) xorSum ^= p;
    return xorSum != 0;
}

int calculateMEX(const unordered_set<int>& s) {
    int mex = 0;
    while (s.count(mex)) mex++;
    return mex;
}

int getGrundy(int n, vi& memo, const vi& moves) {
    if (n == 0) return 0;
    if (memo[n] != -1) return memo[n];
    unordered_set<int> nextStates;
    for (int m : moves) {
        if (n >= m) nextStates.insert(getGrundy(n - m, memo, moves));
    }
    return memo[n] = calculateMEX(nextStates);
}

// ============================================================
// 56. BOYER-MOORE MAJORITY VOTING ALGORITHM
// Pattern: Find elements appearing > N/2 or > N/3 times
// Complexity: O(N) time, O(1) space
// ============================================================
int majorityElement(const vi& nums) {
    int candidate = 0, count = 0;
    for (int num : nums) {
        if (count == 0) candidate = num;
        count += (num == candidate) ? 1 : -1;
    }
    return candidate;
}

vi majorityElementNBy3(const vi& nums) {
    int cand1 = 0, cand2 = 0, count1 = 0, count2 = 0;
    for (int num : nums) {
        if (num == cand1) count1++;
        else if (num == cand2) count2++;
        else if (count1 == 0) { cand1 = num; count1 = 1; }
        else if (count2 == 0) { cand2 = num; count2 = 1; }
        else { count1--; count2--; }
    }
    count1 = count2 = 0;
    for (int num : nums) {
        if (num == cand1) count1++;
        else if (num == cand2) count2++;
    }
    vi res;
    int n = nums.size();
    if (count1 > n / 3) res.push_back(cand1);
    if (count2 > n / 3) res.push_back(cand2);
    return res;
}

// ============================================================
// 57. MATRIX EXPONENTIATION
// Pattern: Fast computation of linear recurrences in O(K^3 log N)
// Complexity: O(K^3 log N) time where K is matrix size
// ============================================================
vvi multiplyMatrix(const vvi& A, const vvi& B, ll mod = MOD) {
    int n = A.size(), m = B[0].size(), p = B.size();
    vvi C(n, vi(m, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < p; k++) {
            for (int j = 0; j < m; j++) {
                C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % mod;
            }
        }
    }
    return C;
}

vvi powerMatrix(vvi A, ll p, ll mod = MOD) {
    int n = A.size();
    vvi res(n, vi(n, 0));
    for (int i = 0; i < n; i++) res[i][i] = 1;
    while (p > 0) {
        if (p & 1) res = multiplyMatrix(res, A, mod);
        A = multiplyMatrix(A, A, mod);
        p >>= 1;
    }
    return res;
}

ll fibonacciMatrix(ll n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    vvi T = {{1, 1}, {1, 0}};
    T = powerMatrix(T, n - 1);
    return T[0][0];
} // F(0) = 0, F(1) = 1, F(2) = 1, ...


// ============================================================
// 58. QUICKSELECT ALGORITHM
// Pattern: Kth smallest/largest element without sorting
// Complexity: O(N) average time, O(N^2) worst-case, O(1) space
// ============================================================
int partitionArray(vi& nums, int left, int right) {
    int pivot = nums[right], pIndex = left;
    for (int i = left; i < right; i++) {
        if (nums[i] <= pivot) {
            swap(nums[i], nums[pIndex++]);
        }
    }
    swap(nums[pIndex], nums[right]);
    return pIndex;
}

int quickselect(vi& nums, int left, int right, int k) {
    if (left == right) return nums[left];
    int pIndex = left + rand() % (right - left + 1);
    swap(nums[pIndex], nums[right]);
    int pivotIdx = partitionArray(nums, left, right);
    if (k == pivotIdx) return nums[k];
    else if (k < pivotIdx) return quickselect(nums, left, pivotIdx - 1, k);
    else return quickselect(nums, pivotIdx + 1, right, k);
}

// ============================================================
// 59. INVERSION COUNT (MERGE SORT)
// Pattern: Count pairs (i < j) with A[i] > A[j] / Min Swaps
// Complexity: O(N log N) time, O(N) space
// ============================================================
ll mergeSortInversions(vi& arr, int l, int r) {
    if (l >= r) return 0;
    int mid = l + (r - l) / 2;
    ll invCount = mergeSortInversions(arr, l, mid) + mergeSortInversions(arr, mid + 1, r);
    vi temp(r - l + 1);
    int i = l, j = mid + 1, k = 0;
    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            invCount += (mid - i + 1);
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= r) temp[k++] = arr[j++];
    copy(all(temp), arr.begin() + l);
    return invCount;
}

// ============================================================
// 60. DSU ON TREE / SACK (SMALL-TO-LARGE MERGING)
// Pattern: Offline subtree queries merging color/freq maps
// Complexity: O(N log^2 N) or O(N log N) time, O(N) space
// ============================================================
void mergeMaps(map<int, int>& heavyMap, map<int, int>& lightMap) {
    for (auto& [val, count] : lightMap) {
        heavyMap[val] += count;
    }
}

map<int, int> dfsSack(int u, int p, const vvi& adj, const vi& color, vi& ans) {
    map<int, int> uMap;
    uMap[color[u]] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        map<int, int> vMap = dfsSack(v, u, adj, color, ans);
        if (uMap.size() < vMap.size()) swap(uMap, vMap);
        mergeMaps(uMap, vMap);
    }
    ans[u] = uMap.size(); // Count distinct colors in subtree u
    return uMap;
}

// ============================================================
// 61. LRU CACHE DESIGN
// Pattern: Fast O(1) Cache eviction using HashMap + Doubly LinkedList
// Complexity: O(1) per get/put, O(Capacity) space
// ============================================================
class LRUCache {
    int cap;
    list<pair<int, int>> dll;
    unordered_map<int, list<pair<int, int>>::iterator> cacheMap;

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if (!cacheMap.count(key)) return -1;
        dll.splice(dll.begin(), dll, cacheMap[key]);
        return cacheMap[key]->second;
    }

    void put(int key, int value) {
        if (cacheMap.count(key)) {
            cacheMap[key]->second = value;
            dll.splice(dll.begin(), dll, cacheMap[key]);
            return;
        }
        if ((int)dll.size() == cap) {
            int delKey = dll.back().first;
            dll.pop_back();
            cacheMap.erase(delKey);
        }
        dll.push_front({key, value});
        cacheMap[key] = dll.begin();
    }
};

// ============================================================
// 62. PALINDROME PARTITIONING DP
// Pattern: Min cuts to partition s into palindromes
// Complexity: O(N^2) time, O(N^2) space
// ============================================================
int minCutPalindromePartition(string s) {
    int n = s.size();
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i; j < n; j++) {
            if (s[i] == s[j]) {
                isPal[i][j] = (j - i <= 2) || isPal[i + 1][j - 1];
            }
        }
    }
    vi dp(n, 0);
    for (int i = 0; i < n; i++) {
        if (isPal[0][i]) { dp[i] = 0; continue; }
        dp[i] = i;
        for (int j = 0; j < i; j++) {
            if (isPal[j + 1][i]) {
                dp[i] = min(dp[i], 1 + dp[j]);
            }
        }
    }
    return dp[n - 1];
}

/*
================================================================================
  LAST-MINUTE PATTERN SELECTION GUIDE (5-MINUTE OA CHEAT SHEET)
================================================================================
  - Sorted array / Search space monotonicity -> Binary Search / Two Pointers
  - Contiguous subarray / Substring optimization -> Sliding Window / Prefix Sum / Kadane
  - Next greater / smaller element -> Monotonic Stack
  - Sliding window min/max -> Monotonic Queue (deque)
  - Range sum / range update queries -> Fenwick Tree / Segment Tree (Lazy)
  - Static Range Min/Max/GCD Queries -> Sparse Table (O(1) query)
  - Unweighted shortest path / Grid min steps -> BFS / Multi-Source BFS
  - 0/1 weighted shortest path -> 0-1 BFS (deque)
  - Non-negative weighted shortest path -> Dijkstra (priority_queue)
  - Negative weighted edges / Negative cycles -> Bellman-Ford / SPFA
  - Connectivity / Dynamic merging -> Disjoint Set Union (DSU)
  - Topological ordering / Dependency DAG -> Topological Sort (Kahn's BFS)
  - Subset choices / Small N (N <= 20) -> Bitmask DP / Backtracking
  - Split N <= 40 into halves -> Meet in the Middle (2^(N/2))
  - Tree subtree queries -> Euler Tour (Flattening) / DSU on Tree (Small-to-Large)
  - Tree Kth parent / LCA queries -> Binary Lifting (O(log N))
  - Intervals / Overlapping events -> Sort by start/end / Sweep Line / Priority Queue
  - Substring matching / Prefix match -> KMP / Z-Algorithm / Rolling Hash / Trie
  - Longest Palindromic Substring -> Manacher's Algorithm (O(N))
  - Game theory / Taking turns -> Nim Game XOR Sum / Sprague-Grundy (MEX)
  - Majority element (> N/2 or > N/3) -> Boyer-Moore Majority Voting (O(N) time, O(1) space)
  - Linear Recurrences / Generalized Fibonacci -> Matrix Exponentiation (O(K^3 log N))
  - Kth smallest/largest in unsorted array -> Quickselect (O(N) avg time)
  - Inverted pairs / Adjacent swaps count -> Inversion Count via Merge Sort (O(N log N))
  - O(1) Get/Put Cache -> LRU Cache (Doubly Linked List + Map)
  - String partition into palindromes -> Palindrome Partitioning DP (O(N^2))
================================================================================
*/

/*
================================================================================
  PRE-OA CHECKLIST — READ THIS 10 MINUTES BEFORE THE TEST STARTS
================================================================================

  [MINDSET / TIME MANAGEMENT]
  - Read ALL questions first (skim 2-3 min) before writing code for any one.
    Solve easiest/highest-confidence problem first to bank points early.
  - Hard cap per question: if stuck > 15-20 min with no progress, move on.
    Partial credit exists in most OAs (many test cases pass != all).
  - Note constraints FIRST, always. They tell you the expected complexity:
      N <= 10          -> exponential / bitmask / brute force OK
      N <= 20          -> bitmask DP (2^N * N)
      N <= 500-1000     -> O(N^2) or O(N^2 log N)
      N <= 1e5          -> O(N log N)
      N <= 1e6-1e7       -> O(N) or O(N log N) with small constant
      N <= 1e9 (value)  -> O(log N) / binary search / math, NOT O(N)
  - Time limit is usually 1-2 sec => ~1e8 simple ops is roughly the ceiling.

  [BEFORE YOU CODE — 60 SECOND SANITY PASS]
  - Re-read problem statement twice. Misreading > algorithm mistakes as
    the #1 cause of failed OAs.
  - Check: 0-indexed or 1-indexed? Inclusive or exclusive ranges?
  - Identify EXACT output format (space-separated? newline? "-1" vs "NO"?
    trailing newline? case sensitivity like "Yes" vs "YES"?).
  - Look for hidden edge cases in the statement: empty array, single
    element, all same elements, negative numbers, duplicates, N=0.

  [WHILE CODING]
  - Use long long by default for anything that could overflow
    (sums, products, N > ~1e5 with counting). Cast BEFORE multiplying:
        long long x = (long long)a * b;   // not (a*b) then cast
  - Initialize variables (esp. min/max accumulators) with safe sentinels:
        long long best = LLONG_MIN; / LLONG_MAX;
  - Watch off-by-one in loop bounds, mid = (l+r)/2 vs l+(r-l)/2 (overflow-safe).
  - Vector/array bounds: double check i+1, i-1 accesses near edges.
  - Modulo arithmetic: take mod after EVERY addition/multiplication,
    and handle negative mod: ((x % MOD) + MOD) % MOD.

  [EDGE CASES TO MANUALLY TEST BEFORE SUBMITTING]
  - Empty input / empty array / empty string
  - Single element
  - All elements identical
  - Already sorted / reverse sorted (for sorting-based problems)
  - Negative numbers, zero, very large numbers
  - Duplicate values (esp. for two-pointer / binary search problems)
  - Minimum and maximum constraint values (N=1 and N=max)

  [DEBUGGING UNDER TIME PRESSURE]
  - If wrong answer on hidden tests but sample passes: re-check assumptions,
    not just logic — did you assume sorted input? Did you assume no dupes?
  - If TLE: check nested loops, check if you're doing O(N) work inside a
    binary search / sliding window when it should be O(1) amortized.
  - If RE (runtime error): almost always array out-of-bounds, division by
    zero, or empty container .back()/.front()/.top() access.
  - Print statements are fine for local debug — just REMOVE or comment
    them out before final submit (some judges fail on extra stdout).

  [SUBMISSION HYGIENE]
  - Remove all debug prints / cerr statements.
  - Remove any hardcoded test-case values you used while debugging.
  - Make sure the function signature matches EXACTLY what's asked
    (return type, parameter order) if it's a function-based OA (not stdin/stdout).
  - Re-run against the given sample input/output one final time before submit.
  - If multiple test files are allowed, submit early even with partial
    solution — don't risk losing everything to a last-second crash/lag.

  [COMMON OA "GOTCHA" PATTERNS TO RECOGNIZE FAST]
  - "Count pairs/subarrays with sum/XOR = K"      -> prefix sum + hashmap
  - "Kth largest/smallest, streaming"              -> heap
  - "Min/max in every window of size K"            -> monotonic deque
  - "Next greater/smaller element"                 -> monotonic stack
  - "Connected components / grouping"              -> DSU or BFS/DFS
  - "Shortest path, unweighted grid"               -> BFS
  - "Shortest path, weighted, no negatives"        -> Dijkstra
  - "Min cost to connect all / MST-flavored"       -> Kruskal/Prim
  - "Optimal substructure + overlapping subproblems"-> DP (define state first!)
  - "Can we achieve X? Feasibility check"          -> binary search on answer
  - "Number of ways / arrangements"                -> combinatorics or DP + mod
  - "Contiguous subarray with max/min sum"         -> Kadane's
  - "Cycle detection in directed graph"            -> topo sort (Kahn's) fails => cycle
  - "String matching / pattern occurs in text"     -> KMP / Z-algorithm

  [LAST 2 MINUTES]
  - Submit something, even if incomplete — partial > zero.
  - Double-check you clicked "Submit" / "Run All Tests", not just "Run".
  - Don't leave the tab / lose internet — some platforms auto-submit on
    tab switch violations (proctoring). Keep the tab focused throughout.
================================================================================
*/

/*
================================================================================
  STL CONTAINER SYNTAX CHEAT SHEET — QUICK REFERENCE BEFORE OA
================================================================================

  [VECTOR]
  vector<int> v;                          // empty
  vector<int> v(n);                       // size n, all 0
  vector<int> v(n, -1);                   // size n, all -1
  vector<vector<int>> grid(n, vector<int>(m, 0));   // 2D n x m
  v.push_back(x); v.pop_back();
  v.size(); v.empty();
  v.front(); v.back();
  v.begin(); v.end(); v.rbegin(); v.rend();
  sort(v.begin(), v.end());
  sort(v.begin(), v.end(), greater<int>());          // descending
  sort(v.begin(), v.end(), [](int a, int b){ return a > b; });
  reverse(v.begin(), v.end());
  v.erase(v.begin() + i);                 // remove index i
  v.insert(v.begin() + i, x);             // insert x at index i
  auto it = find(v.begin(), v.end(), x);  // returns v.end() if not found
  int mn = *min_element(v.begin(), v.end());
  int mx = *max_element(v.begin(), v.end());
  long long s = accumulate(v.begin(), v.end(), 0LL);
  v.resize(newSize);
  v.clear();

  [PAIR]
  pair<int,int> p = {1, 2};
  pair<int,int> p = make_pair(1, 2);
  p.first; p.second;
  vector<pair<int,int>> vp;
  sort(vp.begin(), vp.end());             // sorts by first, then second
  sort(vp.begin(), vp.end(), [](auto &a, auto &b){ return a.second < b.second; });

  [TUPLE]
  tuple<int,int,int> t = {1, 2, 3};
  get<0>(t); get<1>(t); get<2>(t);
  auto [a, b, c] = t;                     // structured binding (C++17)

  [MAP — ordered, O(log n), sorted by key]
  map<int,int> mp;
  mp[key] = value;
  mp[key]++;                              // auto-inits missing key to 0
  if (mp.find(key) != mp.end())           // check existence
  if (mp.count(key))                      // simpler existence check
  mp.erase(key);
  for (auto &[k, v] : mp) { ... }         // iterates sorted by key
  mp.begin()->first;                      // smallest key
  prev(mp.end())->first;                  // largest key

  [UNORDERED_MAP — O(1) avg, no order]
  unordered_map<int,int> ump;
  // same interface as map, use whenever order doesn't matter (faster)

  [SET — ordered, unique, sorted]
  set<int> s;
  s.insert(x); s.erase(x);
  s.count(x);                             // 0 or 1
  s.find(x) != s.end();
  auto it = s.lower_bound(x);             // first element >= x
  auto it = s.upper_bound(x);             // first element > x
  *s.begin();                             // smallest
  *prev(s.end());                         // largest

  [UNORDERED_SET — O(1) avg, no order]
  unordered_set<int> us;
  // same interface as set, faster when order irrelevant

  [MULTISET / MULTIMAP — allow duplicates]
  multiset<int> ms;
  ms.erase(ms.find(x));                   // erase ONE occurrence
                                           // ms.erase(x) removes ALL occurrences

  [STACK]
  stack<int> st;
  st.push(x); st.pop();                   // pop returns void!
  st.top();                               // peek
  st.empty(); st.size();

  [QUEUE]
  queue<int> q;
  q.push(x); q.pop();                     // pop returns void!
  q.front(); q.back();
  q.empty(); q.size();

  [DEQUE — double ended]
  deque<int> dq;
  dq.push_back(x); dq.push_front(x);
  dq.pop_back(); dq.pop_front();
  dq.front(); dq.back();

  [PRIORITY_QUEUE — max-heap by default]
  priority_queue<int> pq;                          // max-heap
  priority_queue<int, vector<int>, greater<int>> pq; // min-heap
  pq.push(x); pq.pop();                            // pop returns void!
  pq.top();                                        // peek
  // custom comparator for pair (min-heap by first element):
  priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

  [STRING]
  string s = "hello";
  s.substr(start, len);                   // substring, len optional (to end)
  s.length(); s.size();
  s += "abc";                             // concat
  s.push_back('a');
  reverse(s.begin(), s.end());
  sort(s.begin(), s.end());
  to_string(123);                         // int -> string
  stoi(s); stol(s); stoll(s);             // string -> int/long/long long
  s.find("sub");                          // returns string::npos if not found
  s.find("sub") != string::npos           // existence check
  isalpha(c); isdigit(c); isupper(c); islower(c);
  tolower(c); toupper(c);

  [BITSET]
  bitset<32> b(num);
  b.count();                              // number of set bits
  b.to_ullong(); b.to_string();
  b[i];                                   // access/set bit i

  [ARRAY (fixed size, C++11)]
  array<int, 5> a = {1,2,3,4,5};
  // same methods as vector but fixed size, no push_back

  [ITERATING]
  for (int x : v) { ... }                 // read-only
  for (int &x : v) { ... }                // modifiable
  for (auto &[k, v] : mp) { ... }         // map/pair structured binding
  for (auto it = v.begin(); it != v.end(); ++it) { ... }

  [COMMON GOTCHAS]
  - stack/queue/pq .pop() returns VOID — never do x = st.pop()
  - map[] operator INSERTS the key if missing (careful in read-only checks;
    use .count() or .find() instead if you don't want to insert)
  - multiset.erase(x) removes ALL matching elements, use erase(find(x)) for one
  - vector out-of-bounds with [] is UB (no error) — use .at(i) while
    debugging to catch it, switch back to [] for speed if needed
  - comparing floating point directly (==) is unsafe, use abs(a-b) < eps
================================================================================
*/

/*
================================================================================
  LAST-MINUTE REVISION GUIDE — DYNAMIC PROGRAMMING
================================================================================

  [HOW TO RECOGNIZE A DP PROBLEM]
  - Asks for: min/max value, count of ways, is it possible (yes/no),
    longest/shortest something, optimal way to partition/select/arrange.
  - Has "optimal substructure": answer to big problem built from answers
    to smaller subproblems.
  - Has "overlapping subproblems": brute force recursion recomputes the
    same state many times (if unsure, draw the recursion tree for N=4-5).
  - Keywords: "maximum/minimum", "number of ways", "can you reach/form",
    "longest/shortest subsequence", "partition into", "at each step choose".

  [THE 5-STEP DP FRAMEWORK — DO THIS ON PAPER FIRST]
  1. Define state:  dp[i] / dp[i][j] = "what does this cell MEAN in words"
     (this is the step people skip and then get stuck — always write it out)
  2. Identify choices at each state (usually 2-K options per step).
  3. Write recurrence: dp[state] = best/sum of (choice -> dp[smaller state])
  4. Base case(s): smallest state(s), usually dp[0] or dp[0][0].
  5. Answer = dp[final state] (not always dp[n][m] — sometimes max over dp[])

  [TOP-DOWN (MEMO) vs BOTTOM-UP (TABULATION)]
  - Top-down: easier to write correctly under time pressure, write the
    brute-force recursion first, then add memo (map or array of -1).
        if (memo[state] != -1) return memo[state];
        return memo[state] = /* recurrence
  - Bottom-up: needed when recursion depth might stack-overflow (N > ~1e5),
    or when you want O(1) space optimization after.
  - If stuck on transitions, ALWAYS write recursive brute force first,
    verify on sample, THEN memoize. Don't try to write tabulation directly
    for a new problem type.

  [SPACE OPTIMIZATION TRICK]
  - If dp[i] only depends on dp[i-1] (or dp[i-1], dp[i-2]), drop the array
    dimension to O(1) using rolling variables. Common in Knapsack, Fibonacci-
    style, House Robber-style problems. Do this ONLY after correctness works.

  [CLASSIC DP PATTERNS — MAP PROBLEM TO PATTERN FAST]
  - "Pick or skip each item, weight limit"         -> 0/1 Knapsack
  - "Pick or skip, unlimited use per item"         -> Unbounded Knapsack
  - "Two strings, match/replace/delete/insert"     -> Edit Distance / LCS family
  - "Longest increasing subsequence"               -> LIS (O(N log N) w/ binary search)
  - "Min/max path in grid, move right/down only"   -> Grid DP
  - "Partition array into K parts optimally"       -> Interval DP or prefix + DP
  - "Merge intervals/matrices optimally"           -> Interval DP (MCM style),
       dp[i][j] = min over k of dp[i][k] + dp[k+1][j] + cost(i,j)
  - "Count ways to reach N using steps {1,2,3..}"  -> 1D DP, dp[i]=sum(dp[i-step])
  - "Subset sum / can we make sum S"               -> boolean DP, dp[sum] = true/false
  - "N small (<=20), choose subset with constraint"-> Bitmask DP
  - "Count numbers in [L,R] with digit property"   -> Digit DP
  - "Max sum non-adjacent elements"                -> House Robber pattern,
       dp[i] = max(dp[i-1], dp[i-2] + a[i])
  - "Tree, best value using subtree choices"       -> Tree DP (post-order, combine children)

  [DP DEBUGGING CHECKLIST]
  - Print the DP table for a small example, verify by hand.
  - Check base case indices carefully (dp[0] vs dp[1], off-by-one is #1 bug).
  - Check iteration ORDER — are you using values not yet computed?
    (e.g., unbounded knapsack iterates weight ascending, 0/1 knapsack
    descending, when using 1D optimized array — this trips people up a lot)
  - Overflow: use long long for sums/counts, mod arithmetic for "count ways".
  - If "count ways" — remember answer might need % 1e9+7 at EVERY step,
    not just at the end.

================================================================================
  LAST-MINUTE REVISION GUIDE — GRAPH PROBLEMS
================================================================================

  [HOW TO RECOGNIZE + PICK THE RIGHT ALGORITHM FAST]
  - "Shortest path, unweighted"                    -> BFS
  - "Shortest path, weighted, all edges >= 0"       -> Dijkstra (priority_queue)
  - "Shortest path, negative edges allowed"         -> Bellman-Ford (or SPFA)
  - "Shortest path, negative cycle detection"       -> Bellman-Ford (N-1 relax,
       check if Nth relaxation still improves something)
  - "All-pairs shortest path, small N (<=400)"      -> Floyd-Warshall O(N^3)
  - "Min cost to connect all nodes (spanning tree)" -> Kruskal (DSU) or Prim
  - "Grid problem, 4/8-directional movement"        -> BFS/DFS on grid,
       treat each cell as a node, check bounds before recursing
  - "0/1 weighted edges only (0 or 1 cost)"         -> 0-1 BFS (deque, push
       front for 0-weight, back for 1-weight)
  - "Multiple starting points, same time"           -> Multi-source BFS
       (push all sources into queue with distance 0 initially)
  - "Order tasks with dependencies"                 -> Topological Sort (Kahn's
       BFS using in-degree, or DFS post-order + reverse)
  - "Detect cycle in directed graph"                -> Kahn's: if topo sort
       doesn't include all nodes => cycle exists
  - "Detect cycle in undirected graph"               -> DFS with parent tracking,
       or DSU (if union(u,v) finds u,v already same set => cycle)
  - "Group connected components / union queries"     -> DSU (Union-Find)
  - "Find bridges/articulation points"                -> Tarjan's (disc/low arrays)
  - "Strongly connected components"                   -> Tarjan's SCC or Kosaraju's
  - "Bipartite check"                                 -> BFS/DFS 2-coloring
  - "Min spanning tree, need actual edges used"       -> Kruskal + DSU (sort
       edges by weight, add if it doesn't form cycle)

  [BFS TEMPLATE — MENTAL CHECKLIST]
  - Use queue, NOT stack (stack = DFS, common silly mistake under pressure).
  - Mark visited WHEN PUSHING to queue, not when popping (avoids duplicate
    pushes of the same node -> can cause TLE or wrong distances).
  - Track distance/level either via a parallel dist[] array or by
    processing queue level-by-level (store queue size before the for loop).

  [DFS TEMPLATE — MENTAL CHECKLIST]
  - Recursive DFS can stack overflow if graph is a long chain and N > ~1e5;
    prefer iterative DFS with explicit stack for large inputs.
  - For cycle detection in directed graphs via DFS, track 3 states per node:
    unvisited / in current recursion stack / fully processed (not just
    visited/unvisited — 2-state DFS cycle check is WRONG for directed graphs).

  [GRAPH REPRESENTATION — PICK FAST]
  - Adjacency list: vector<vector<int>> adj(n);  -- default choice, O(V+E) space
  - Weighted: vector<vector<pair<int,int>>> adj(n); // {neighbor, weight}
  - Dense graph (E close to V^2) or need O(1) edge lookup: adjacency matrix
  - Always check: is graph DIRECTED or UNDIRECTED? (undirected needs edge
    added BOTH ways: adj[u].push_back(v); adj[v].push_back(u);)

  [DSU (UNION-FIND) QUICK TEMPLATE REMINDER]
  - Use path compression in find() AND union by rank/size for near-O(1) ops.
  - find(x): if (parent[x]!=x) parent[x]=find(parent[x]); return parent[x];
  - Without path compression + union by rank, DSU can degrade to O(N) per op
    -> TLE on large inputs. Always include both optimizations.

  [GRAPH DEBUGGING CHECKLIST]
  - Off-by-one: nodes numbered 0-indexed or 1-indexed in the problem?
    Mismatch here silently breaks everything.
  - Self-loops and multiple edges — does the problem guarantee simple graph?
    If not, your visited/dist logic must still handle them correctly.
  - Disconnected graph: does your BFS/DFS need to run from EVERY unvisited
    node (for "count components" style problems), not just node 0?
  - Dijkstra: skip processing if popped distance > dist[node] (stale entry
    in priority_queue) — forgetting this causes wrong answers, not just TLE.
  - Bellman-Ford: relax ALL edges exactly V-1 times, then do ONE more pass
    to detect negative cycles (if any edge still relaxes, cycle exists).
  - Check edge weights: can they be 0? negative? does that rule out Dijkstra?
================================================================================
*/

void QuickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int i = l, j = r;
    int pivot = a[l + (r - l) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;

        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (l < j) QuickSort(a, l, j);
    if (i < r) QuickSort(a, i, r);
}
// tc - O(n log n) average, O(n^2) worst case
// sc - O(log n) average, O(n) worst case (recursion stack)

