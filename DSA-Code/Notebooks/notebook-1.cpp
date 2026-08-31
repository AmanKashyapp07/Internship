#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Tree Centroid                               | Subtree Size DFS + Heavy Descent  | O(N)     | O(N)     |
 | 2  | Longest Paths & Path Counts on a DAG        | Topological Sort + DP Relaxation  | O(V + E) | O(V + E) |
 | 3  | Tree Rerooting DP (All-Nodes Distance Sum)  | 2-Pass Tree Rerooting DP          | O(N)     | O(N)     |
 | 4  | Tree Diameter & Node Eccentricities         | 2-Pass Tree BFS/DFS               | O(N)     | O(N)     |
 | 5  | Functional Graph Decomposition & Queries    | Floyd's Cycle + Binary Lifting    | O(N logN)| O(N logN)|
 | 6  | Tree Isomorphism (Double-Hashing)           | Canonical Subtree Hashing (AHU)   | O(N logN)| O(N)     |
 | 7  | Tree Centers by Leaf Trimming               | Topological Leaf Peeling (Deg 1)  | O(N)     | O(N)     |
 | 8  | Reconstruct Cycle                           | Predecessor Backtracking DFS/BFS  | O(V)     | O(V)     |
 | 9  | Nodes in Cycles (Topological Peeling)       | In-Degree 0 Queue Cascade (Kahn)  | O(V + E) | O(V)     |
 | 10 | Lexicographical Topological Sort            | Min-Heap Kahn's BFS               | O(V logV)| O(V)     |
 | 11 | Shortest Path Visiting All Nodes (LC 847)   | Multi-Source Bitmask BFS          | O(V 2^V) | O(V 2^V) |
 | 12 | Maximum Sum BST in Binary Tree (LC 1373)    | Post-Order Bottom-Up DFS          | O(N)     | O(H)     |
 | 13 | Remove Leaf Nodes with Target Value (1325)  | Post-Order Recursive Tree Pruning | O(N)     | O(H)     |
 | 14 | Possible Path Lengths in DAG                | TopoSort + 2D Reachability DP     | O(VN+EN) | O(V * N) |
 | 15 | Max Components Tree Split with Equal Sum    | Divisors + Subtree DFS Cuts       | O(N div) | O(N)     |
 | 16 | Binary Lifting (LCA, K-th Ancestor)         | Ancestor Doubling Table up[u][j]  | O(N logN)| O(N logN)|
 | 17 | Cycle Detection & Reconstruction            | DFS Parent Trace / Bellman-Ford   | O(V + E) | O(V)     |
 | 18 | Bidirectional BFS Template                  | Two-Ended BFS (Smaller Frontier)  | O(B^(D/2))| O(B^(D/2))|
 | 19 | Jump Game III (LeetCode 1306)               | Graph BFS Reachability            | O(N)     | O(N)     |
 | 20 | Jump Game IV (LeetCode 1345)               | BFS + Same-Value Pruning          | O(N)     | O(N)     |
 | 21 | Jump Game V (LeetCode 1340)                | Memoized DFS / DAG Longest Path   | O(N * D) | O(N)     |
 | 22 | Jump Game VII (LeetCode 1871)              | Sliding Window Reachability DP    | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. TREE CENTROID
// ============================================================

struct TreeCentroid {
    int n;
    vvi adj;
    vi sz;
    int centroid;

    TreeCentroid(int n) : n(n), adj(n), sz(n, 0), centroid(-1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs_size(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p) {
                dfs_size(v, u);
                sz[u] += sz[v];
            }
        }
    }

    int find_centroid(int u, int p, int total_sz) {
        for (int v : adj[u]) {
            if (v != p && sz[v] > total_sz / 2)
                return find_centroid(v, u, total_sz);
        }
        return u;
    }

    int get_centroid() {
        dfs_size(0, -1);
        centroid = find_centroid(0, -1, sz[0]);
        return centroid;
    }
};
// Interview Explanation:
// - Problem Statement: Find the centroid of a tree (a node whose removal splits the tree into components each of size <= N/2).
// - Approach: Compute subtree sizes using DFS, then walk down towards any child with size > N/2.
// - Intuition: A tree always has 1 or 2 centroids. Walking down the heavy child guarantees reaching the centroid in O(N).
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 2. LONGEST PATHS & PATH COUNTS ON A DAG
// ============================================================

struct DAGPaths {
    int n;
    vector<vector<pair<int, ll>>> adj;
    vi in_deg;

    DAGPaths(int n) : n(n), adj(n), in_deg(n, 0) {}

    void add_edge(int u, int v, ll w = 1) {
        adj[u].push_back({v, w});
        in_deg[v]++;
    }

    pair<vl, vl> solve(int src) {
        vl dist(n, -1e18), ways(n, 0);
        dist[src] = 0;
        ways[src] = 1;

        queue<int> q;
        vi deg = in_deg;
        for (int i = 0; i < n; i++) if (deg[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& edge : adj[u]) {
                int v = edge.first;
                ll w = edge.second;
                if (dist[u] != -1e18) {
                    if (dist[u] + w > dist[v]) {
                        dist[v] = dist[u] + w;
                        ways[v] = ways[u];
                    } else if (dist[u] + w == dist[v]) {
                        ways[v] = (ways[v] + ways[u]) % MOD;
                    }
                }
                if (--deg[v] == 0) q.push(v);
            }
        }
        return {dist, ways};
    }
};
// Interview Explanation:
// - Problem Statement: Find the longest path and number of longest paths from a source in a DAG.
// - Approach: Topological sort + dynamic programming relaxation.
// - Intuition: DAG has no cycles, so relaxing in topological order guarantees optimal prefix subproblems.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ============================================================
// 3. TREE REROOTING DP (ALL-NODES DISTANCE SUM)
// ============================================================

struct TreeRerooting {
    int n;
    vvi adj;
    vi sz;
    vl ans, dp;

    TreeRerooting(int n) : n(n), adj(n), sz(n, 0), ans(n, 0), dp(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs1(int u, int p) {
        sz[u] = 1;
        dp[u] = 0;
        for (int v : adj[u]) {
            if (v != p) {
                dfs1(v, u);
                sz[u] += sz[v];
                dp[u] += dp[v] + sz[v];
            }
        }
    }

    void dfs2(int u, int p) {
        for (int v : adj[u]) {
            if (v != p) {
                ans[v] = ans[u] - sz[v] + (n - sz[v]);
                dfs2(v, u);
            }
        }
    }

    vl solve() {
        dfs1(0, -1);
        ans[0] = dp[0];
        dfs2(0, -1);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Compute sum of distances from every node to all other nodes in a tree.
// - Approach: 2-pass tree DP (Bottom-up subtree aggregation, Top-down rerooting transfer).
// - Intuition: Moving root from u to child v increases distance for (N - sz[v]) nodes and decreases for sz[v] nodes.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 4. TREE DIAMETER & NODE ECCENTRICITIES
// ============================================================

struct TreeDiameter {
    int n;
    vvi adj;

    TreeDiameter(int n) : n(n), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    pair<int, int> bfs(int src) {
        vi dist(n, -1);
        queue<int> q;
        q.push(src);
        dist[src] = 0;
        int farthest = src;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] > dist[farthest]) farthest = u;
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return {farthest, dist[farthest]};
    }

    int diameter() {
        auto p1 = bfs(0);
        auto p2 = bfs(p1.first);
        return p2.second;
    }
};
// Interview Explanation:
// - Problem Statement: Find diameter of an unweighted tree.
// - Approach: Double BFS/DFS (from node 0 -> farthest node u -> farthest node v).
// - Intuition: The farthest node from ANY starting node in a tree is always one endpoint of a diameter.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 5. FUNCTIONAL GRAPH DECOMPOSITION & QUERIES
// ============================================================

struct FunctionalGraph {
    int n;
    vi succ;
    static constexpr int LOG = 30;
    vvi up;

    FunctionalGraph(int n, vi& f) : n(n), succ(f), up(LOG, vi(n)) {
        for (int i = 0; i < n; i++) up[0][i] = succ[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                up[j][i] = up[j - 1][up[j - 1][i]];
            }
        }
    }

    int jump(int u, ll k) {
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) u = up[j][u];
        }
        return u;
    }
};
// Interview Explanation:
// - Problem Statement: Move k steps in a functional graph (where out-degree = 1 for each node).
// - Approach: Binary lifting (jump doubling table).
// - Intuition: Any transition sequence can be binary decomposed in O(log K) per query.
// - Complexity: Build: O(N log N), Query: O(log K), Space: O(N log N).


// ============================================================
// 6. TREE ISOMORPHISM (DOUBLE-HASHING)
// ============================================================

struct TreeIsomorphism {
    int n;
    vvi adj;
    map<vector<ull>, ull> memo;

    TreeIsomorphism(int n) : n(n), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    ull hash_subtree(int u, int p) {
        vector<ull> child_hashes;
        for (int v : adj[u]) {
            if (v != p) child_hashes.push_back(hash_subtree(v, u));
        }
        sort(child_hashes.begin(), child_hashes.end());
        ull h = 0x9e3779b97f4a7c15ULL;
        for (ull ch : child_hashes) {
            h ^= ch + 0x517cc1b727220a95ULL + (h << 6) + (h >> 2);
        }
        return h;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if two unrooted trees are isomorphic (structurally identical).
// - Approach: Canonical subtree hashing (AHU Algorithm) with centroid root normalization.
// - Intuition: Sort child hashes recursively; two subtrees match iff their canonical tuple hashes match.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 7. TREE CENTERS BY LEAF TRIMMING
// ============================================================

vi findTreeCenters(int n, vvi& adj) {
    if (n == 1) return {0};
    vi deg(n, 0);
    for (int i = 0; i < n; i++) deg[i] = adj[i].size();

    queue<int> q;
    for (int i = 0; i < n; i++) if (deg[i] == 1) q.push(i);

    int remaining = n;
    while (remaining > 2) {
        int sz = q.size();
        remaining -= sz;
        for (int i = 0; i < sz; i++) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (--deg[v] == 1) q.push(v);
            }
        }
    }

    vi centers;
    while (!q.empty()) { centers.push_back(q.front()); q.pop(); }
    return centers;
}
// Interview Explanation:
// - Problem Statement: Find the center(s) of a tree (minimizing maximum distance to any other node).
// - Approach: Iterative leaf peeling (Topological BFS layer reduction).
// - Intuition: Trimming leaves symmetrically preserves tree center; 1 or 2 nodes remain at the core.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 8. RECONSTRUCT CYCLE
// ============================================================

vector<int> reconstructCycle(int n, vector<vector<int>>& adj) {
    vector<int> parent(n, -1), color(n, 0); // 0: unvisited, 1: visiting, 2: visited
    int cycleStart = -1, cycleEnd = -1;

    function<bool(int, int)> dfs = [&](int u, int p) {
        color[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (color[v] == 1) {
                cycleStart = v;
                cycleEnd = u;
                return true;
            }
            if (color[v] == 0) {
                parent[v] = u;
                if (dfs(v, u)) return true;
            }
        }
        color[u] = 2;
        return false;
    };

    for (int i = 0; i < n; i++) {
        if (color[i] == 0 && dfs(i, -1)) break;
    }

    if (cycleStart == -1) return {};
    vector<int> cycle;
    cycle.push_back(cycleStart);
    for (int curr = cycleEnd; curr != cycleStart; curr = parent[curr]) {
        cycle.push_back(curr);
    }
    cycle.push_back(cycleStart);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}
// Interview Explanation:
// - Problem Statement: Detect and reconstruct any cycle in an undirected graph.
// - Approach: 3-Color DFS with Parent Pointers.
// - Intuition: Back-edge to an in-stack ancestor (color 1) identifies a cycle. Backtrack via parent pointers.
// - Complexity: Time: O(V + E), Space: O(V).


// ============================================================
// 9. NODES IN CYCLES (TOPOLOGICAL PEELING)
// ============================================================

vector<bool> findNodesInCycles(int n, vector<vector<int>>& adj) {
    vector<int> inDegree(n, 0);
    vector<vector<int>> revAdj(n);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            inDegree[v]++;
            revAdj[v].push_back(u);
        }
    }
    queue<int> q;
    for (int i = 0; i < n; i++) if (inDegree[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }

    vector<bool> inCycle(n, false);
    for (int i = 0; i < n; i++) {
        if (inDegree[i] > 0) inCycle[i] = true;
    }
    return inCycle;
}
// Interview Explanation:
// - Problem Statement: Identify all vertices that participate in at least one directed cycle.
// - Approach: Kahn's Algorithm / In-Degree 0 Peeling.
// - Intuition: Nodes with inDegree == 0 cannot be part of a cycle. After iterative queue peeling, all remaining nodes with inDegree > 0 belong to or reach cycles.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ============================================================
// 10. LEXICOGRAPHICAL TOPOLOGICAL SORT
// ============================================================

vector<int> lexicographicalTopoSort(int n, vector<vector<int>>& adj) {
    vector<int> inDegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }

    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) minHeap.push(i);
    }

    vector<int> order;
    while (!minHeap.empty()) {
        int u = minHeap.top(); minHeap.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) minHeap.push(v);
        }
    }

    return (int)order.size() == n ? order : vector<int>{};
}
// Interview Explanation:
// - Problem Statement: Find the lexicographically smallest topological sort ordering in a DAG.
// - Approach: Kahn's Algorithm with Min-Heap Priority Queue.
// - Intuition: Always extract the smallest available in-degree 0 node from a min-heap.
// - Complexity: Time: O(V log V + E), Space: O(V + E).


// ============================================================
// 11. SHORTEST PATH VISITING ALL NODES — LeetCode 847
// ============================================================

int shortestPathLength(vector<vector<int>>& graph) {
    int n = graph.size();
    int targetMask = (1 << n) - 1;
    queue<tuple<int, int, int>> q; // {node, mask, dist}
    vector<vector<bool>> visited(n, vector<bool>(1 << n, false));

    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i, 0});
        visited[i][1 << i] = true;
    }

    while (!q.empty()) {
        auto [u, mask, dist] = q.front(); q.pop();
        if (mask == targetMask) return dist;

        for (int v : graph[u]) {
            int nextMask = mask | (1 << v);
            if (!visited[v][nextMask]) {
                visited[v][nextMask] = true;
                q.push({v, nextMask, dist + 1});
            }
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Find shortest path length visiting all nodes in an undirected graph (LeetCode 847).
// - Approach: Multi-Source Bitmask BFS.
// - Intuition: State is pair (node, bitmask). BFS guarantees finding minimal step count when all bits are set.
// - Complexity: Time: O(V * 2^V), Space: O(V * 2^V).


// ============================================================
// 12. MAXIMUM SUM BST IN BINARY TREE — LeetCode 1373
// ============================================================

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

struct BSTSubtreeInfo {
    bool isBST;
    int minVal, maxVal, sumVal;
};

int maxSumBST(TreeNode* root) {
    int globalMaxSum = 0;

    function<BSTSubtreeInfo(TreeNode*)> dfs = [&](TreeNode* node) -> BSTSubtreeInfo {
        if (!node) return {true, INT_MAX, INT_MIN, 0};

        auto left = dfs(node->left);
        auto right = dfs(node->right);

        if (left.isBST && right.isBST && node->val > left.maxVal && node->val < right.minVal) {
            int currentSum = node->val + left.sumVal + right.sumVal;
            globalMaxSum = max(globalMaxSum, currentSum);
            return {true, min(node->val, left.minVal), max(node->val, right.maxVal), currentSum};
        }
        return {false, 0, 0, 0};
    };

    dfs(root);
    return globalMaxSum;
}
// Interview Explanation:
// - Problem Statement: Find maximum sum of all keys of any sub-tree which is also a Binary Search Tree (LeetCode 1373).
// - Approach: Post-Order Bottom-Up DFS returning {isBST, minVal, maxVal, sumVal}.
// - Intuition: A node forms a BST iff its left and right children are BSTs and left.max < node.val < right.min.
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 13. REMOVE LEAF NODES WITH TARGET VALUE — LeetCode 1325
// ============================================================

TreeNode* removeLeafNodes(TreeNode* root, int target) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, target);
    root->right = removeLeafNodes(root->right, target);
    if (!root->left && !root->right && root->val == target) return nullptr;
    return root;
}
// Interview Explanation:
// - Problem Statement: Delete all leaf nodes with value target, repeating until no such leaves remain (LeetCode 1325).
// - Approach: Post-Order Recursive Tree Pruning.
// - Intuition: Recursively prune subtrees first. If current node becomes a leaf and matches target, delete it by returning null.
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 14. POSSIBLE PATH LENGTHS IN DAG
// ============================================================

vector<bool> possiblePathLengths(int n, vector<vector<int>>& adj, int src, int dest) {
    vector<int> inDegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) if (inDegree[i] == 0) q.push(i);

    vector<bitset<1001>> dp(n);
    dp[src].set(0);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            dp[v] |= (dp[u] << 1);
            if (--inDegree[v] == 0) q.push(v);
        }
    }

    vector<bool> reachable(n + 1, false);
    for (int len = 0; len <= n; len++) {
        if (dp[dest].test(len)) reachable[len] = true;
    }
    return reachable;
}
// Interview Explanation:
// - Problem Statement: Determine all possible path lengths from src to dest in a DAG.
// - Approach: TopoSort + 2D Bitset Reachability DP.
// - Intuition: `dp[v] |= (dp[u] << 1)` shifts bitmask by 1 step for every edge u -> v in topological order.
// - Complexity: Time: O((V + E) * N / 64), Space: O(V * N / 64).


// ============================================================
// 15. MAX COMPONENTS TREE SPLIT WITH EQUAL SUM — LeetCode 2440
// ============================================================

int componentValue(vector<int>& nums, vector<vector<int>>& edges) {
    int n = nums.size();
    int total = accumulate(nums.begin(), nums.end(), 0);
    vector<vector<int>> adj(n);
    for (auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }

    auto check = [&](int target) {
        function<int(int, int)> dfs = [&](int u, int p) -> int {
            int sum = nums[u];
            for (int v : adj[u]) {
                if (v != p) {
                    int sub = dfs(v, u);
                    if (sub == -1) return -1;
                    sum += sub;
                }
            }
            if (sum == target) return 0;
            if (sum > target) return -1;
            return sum;
        };
        return dfs(0, -1) == 0;
    };

    for (int k = n; k >= 1; k--) {
        if (total % k == 0 && check(total / k)) {
            return k - 1;
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Split a tree into maximum number of connected components with equal sum (LeetCode 2440).
// - Approach: Divisor Enumeration + Subtree DFS Cut Verification.
// - Intuition: Component sum `target` must divide `totalSum`. A bottom-up DFS verifies if valid cuts sum exactly to `target`.
// - Complexity: Time: O(N * divisors(Total)), Space: O(N).


// ============================================================
// 16. BINARY LIFTING (LCA, K-TH ANCESTOR)
// ============================================================

struct TreeLifting {
    int n, LOG;
    vector<vector<int>> adj;
    vector<vector<int>> up;
    vector<int> depth;

    TreeLifting(int n) : n(n), LOG(20), adj(n), depth(n, 0) {
        up.assign(n, vector<int>(LOG, 0));
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p, int d) {
        depth[u] = d;
        up[u][0] = p;
        for (int j = 1; j < LOG; j++) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }
        for (int v : adj[u]) {
            if (v != p) dfs(v, u, d + 1);
        }
    }

    void init(int root = 0) {
        dfs(root, root, 0);
    }

    int get_kth_ancestor(int node, int k) {
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) node = up[node][j];
        }
        return node;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = get_kth_ancestor(u, depth[u] - depth[v]);
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
// Interview Explanation:
// - Problem Statement: Compute Lowest Common Ancestor (LCA) and k-th ancestor in a tree.
// - Approach: Binary Lifting table `up[u][j] = 2^j`-th ancestor.
// - Intuition: Powers of 2 allow O(log N) jump queries for both k-th ancestor and LCA.
// - Complexity: Build: O(N log N), Query: O(log N), Space: O(N log N).


// ============================================================
// 17. CYCLE DETECTION & RECONSTRUCTION (UNDIRECTED / DIRECTED)
// ============================================================

bool detectCycleUndirected(int n, vector<vector<int>>& adj) {
    vector<bool> vis(n, false);
    function<bool(int, int)> dfs = [&](int u, int p) {
        vis[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) { if (dfs(v, u)) return true; }
            else if (v != p) return true;
        }
        return false;
    };
    for (int i = 0; i < n; i++) if (!vis[i] && dfs(i, -1)) return true;
    return false;
}
// Interview Explanation:
// - Problem Statement: Detect cycles in general graphs.
// - Approach: DFS back-edge inspection.
// - Intuition: Encountering an already visited node that is not the direct parent signifies a cycle.
// - Complexity: Time: O(V + E), Space: O(V).


// ============================================================
// 18. BIDIRECTIONAL BFS TEMPLATE
// ============================================================

int bidirectionalBFS(int start, int target, const function<vector<int>(int)>& getNeighbors) {
    if (start == target) return 0;
    unordered_set<int> forwardVisited = {start};
    unordered_set<int> backwardVisited = {target};
    queue<pair<int, int>> forwardQueue;
    queue<pair<int, int>> backwardQueue;

    forwardQueue.push({start, 0});
    backwardQueue.push({target, 0});

    while (!forwardQueue.empty() && !backwardQueue.empty()) {
        if (forwardQueue.size() <= backwardQueue.size()) {
            auto [current, dist] = forwardQueue.front(); forwardQueue.pop();
            for (int neighbor : getNeighbors(current)) {
                if (backwardVisited.count(neighbor)) return dist + 1;
                if (!forwardVisited.count(neighbor)) {
                    forwardVisited.insert(neighbor);
                    forwardQueue.push({neighbor, dist + 1});
                }
            }
        } else {
            auto [current, dist] = backwardQueue.front(); backwardQueue.pop();
            for (int neighbor : getNeighbors(current)) {
                if (forwardVisited.count(neighbor)) return dist + 1;
                if (!backwardVisited.count(neighbor)) {
                    backwardVisited.insert(neighbor);
                    backwardQueue.push({neighbor, dist + 1});
                }
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find shortest distance between two states when branching factor is high.
// - Approach: Bidirectional BFS expanding smaller frontier.
// - Intuition: Reduces search space from O(B^D) to O(B^(D/2)).
// - Complexity: Time: O(B^(D/2)), Space: O(B^(D/2)).


// ============================================================
// 19. JUMP GAME III — LeetCode 1306
// ============================================================

bool canReach(vector<int>& arr, int start) {
    int n = arr.size();
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(start); vis[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (arr[u] == 0) return true;
        for (int v : {u + arr[u], u - arr[u]}) {
            if (v >= 0 && v < n && !vis[v]) { vis[v] = true; q.push(v); }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if you can reach any index with value 0 from start, jumping +/- arr[i] (LeetCode 1306).
// - Approach: Breadth-First Search (BFS) / Graph Reachability.
// - Intuition: Treat array indices as graph vertices with directed edges to i + arr[i] and i - arr[i].
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 20. JUMP GAME IV — LeetCode 1345
// ============================================================

int minJumps(vector<int>& arr) {
    int n = arr.size();
    unordered_map<int, vector<int>> pos;
    for (int i = 0; i < n; i++) pos[arr[i]].push_back(i);
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(0); vis[0] = true;
    int steps = 0;
    while (!q.empty()) {
        for (int sz = q.size(); sz > 0; sz--) {
            int u = q.front(); q.pop();
            if (u == n - 1) return steps;
            if (u - 1 >= 0 && !vis[u - 1]) { vis[u - 1] = true; q.push(u - 1); }
            if (u + 1 < n && !vis[u + 1]) { vis[u + 1] = true; q.push(u + 1); }
            if (pos.count(arr[u])) {
                for (int v : pos[arr[u]]) if (!vis[v]) { vis[v] = true; q.push(v); }
                pos.erase(arr[u]);
            }
        }
        steps++;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum jumps to reach the last index jumping to i-1, i+1, or any j where arr[j] == arr[i] (LeetCode 1345).
// - Approach: Level-Order BFS with Value Group Pruning.
// - Intuition: Group indices by value in a hash map. Erase map entry once visited to prevent O(N^2) expansions.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 21. JUMP GAME V — LeetCode 1340
// ============================================================

int maxJumps(vector<int>& arr, int d) {
    int n = arr.size(), ans = 0;
    vector<int> dp(n, -1);
    function<int(int)> dfs = [&](int i) {
        if (dp[i] != -1) return dp[i];
        int best = 1;
        for (int j = i + 1; j < n && j <= i + d; j++) {
            if (arr[j] >= arr[i]) break;
            best = max(best, 1 + dfs(j));
        }
        for (int j = i - 1; j >= 0 && j >= i - d; j--) {
            if (arr[j] >= arr[i]) break;
            best = max(best, 1 + dfs(j));
        }
        return dp[i] = best;
    };
    for (int i = 0; i < n; i++) ans = max(ans, dfs(i));
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum indices you can visit jumping <= d steps strictly to smaller values (LeetCode 1340).
// - Approach: Memoized DFS / Dynamic Programming on DAG.
// - Intuition: Jumps go only to strictly smaller values, forming a DAG.
// - Complexity: Time: O(N * D), Space: O(N).


// ============================================================
// 22. JUMP GAME VII — LeetCode 1871
// ============================================================

bool canReach(string s, int minJump, int maxJump) {
    int n = s.size(), reach = 0;
    if (s[n - 1] != '0') return false;
    vector<bool> dp(n, false);
    dp[0] = true;
    for (int i = 1; i < n; i++) {
        if (i >= minJump && dp[i - minJump]) reach++;
        if (i > maxJump && dp[i - maxJump - 1]) reach--;
        if (s[i] == '0' && reach > 0) dp[i] = true;
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Check if last index can be reached where s[i] == '0' and jump range is [minJump, maxJump] (LeetCode 1871).
// - Approach: Sliding Window Reachability Count DP.
// - Intuition: Maintain running count `reach` of true DP states in the valid sliding window [i - maxJump, i - minJump].
// - Complexity: Time: O(N), Space: O(N).
