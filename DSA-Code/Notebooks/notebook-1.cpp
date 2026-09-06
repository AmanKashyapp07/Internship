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
 | 14 | Possible Path Lengths in DAG                | TopoSort + 2D Reachability DP     | O(V*N/64)| O(V*N/64)|
 | 15 | Max Components Tree Split with Equal Sum    | Divisors + Subtree DFS Cuts       | O(N*div) | O(N)     |
 | 16 | Binary Lifting (LCA, K-th Ancestor)         | Ancestor Doubling Table up[u][j]  | O(N logN)| O(N logN)|
 | 17 | Cycle Detection & Reconstruction            | DFS Parent Trace / Back-Edge Check| O(V + E) | O(V)     |
 | 18 | Bidirectional BFS Template                  | Two-Ended BFS (Smaller Frontier)  | O(B^(D/2))| O(B^(D/2))|
 | 19 | Jump Game III (LeetCode 1306)               | Graph BFS Reachability            | O(N)     | O(N)     |
 | 20 | Jump Game IV (LeetCode 1345)                | BFS + Same-Value Pruning          | O(N)     | O(N)     |
 | 21 | Jump Game V (LeetCode 1340)                 | Memoized DFS / DAG Longest Path   | O(N * D) | O(N)     |
 | 22 | Jump Game VII (LeetCode 1871)               | Sliding Window Reachability DP    | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. TREE CENTROID
// ============================================================

struct TreeCentroid {
    int n;
    vvi adj;
    vi sz;
    int ans;

    TreeCentroid(int n) : n(n), adj(n), sz(n, 0), ans(-1) {}

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

    int find_centroid(int u, int p, int tot) {
        for (int v : adj[u]) {
            if (v != p && sz[v] > tot / 2)
                return find_centroid(v, u, tot);
        }
        return u;
    }

    int get_centroid() {
        dfs_size(0, -1);
        ans = find_centroid(0, -1, sz[0]);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find centroid of tree (node whose removal splits tree into components each of size <= N/2).
// - Approach: Subtree Size DFS + Heavy Child Descent.
// - Intuition:
//   * A tree always possesses either 1 or 2 centroids.
//   * Pass 1: Run DFS from arbitrary root (node 0) to compute subtree sizes sz[u].
//   * Pass 2: Start at root; examine all adjacent neighbors. If any child v has sz[v] > totalNodes / 2, move to v.
//   * Because at most one child can contain > N/2 nodes, this greedy descent never branches and terminates at the centroid.
//   * Total time is strictly O(N) with O(N) recursion stack.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 2. LONGEST PATHS & PATH COUNTS ON A DAG
// ============================================================

struct DAGPaths {
    int n;
    vector<vector<pair<int, ll>>> adj;
    vi deg;

    DAGPaths(int n) : n(n), adj(n), deg(n, 0) {}

    void add_edge(int u, int v, ll w = 1) {
        adj[u].push_back({v, w});
        deg[v]++;
    }

    pair<vl, vl> solve(int src) {
        vl dist(n, -1e18), ways(n, 0);
        dist[src] = 0;
        ways[src] = 1;

        queue<int> q;
        vi inDeg = deg;
        for (int i = 0; i < n; i++) if (inDeg[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& [v, w] : adj[u]) {
                if (dist[u] != -1e18) {
                    if (dist[u] + w > dist[v]) {
                        dist[v] = dist[u] + w;
                        ways[v] = ways[u];
                    } else if (dist[u] + w == dist[v]) {
                        ways[v] = (ways[v] + ways[u]) % MOD;
                    }
                }
                if (--inDeg[v] == 0) q.push(v);
            }
        }
        return {dist, ways};
    }
};
// Interview Explanation:
// - Problem Statement: Find longest path and count of distinct longest paths from src in a DAG.
// - Approach: Kahn's Topological Sort + Dynamic Programming Relaxation.
// - Intuition:
//   * Directed Acyclic Graphs have no cycles; topological ordering guarantees all predecessors of v are evaluated before v.
//   * Initialize dist[src] = 0, ways[src] = 1, and other distances to -infinity.
//   * When processing node u, relax outgoing edge (u, v, w):
//     - If dist[u] + w > dist[v]: update dist[v] and reset ways[v] = ways[u].
//     - If dist[u] + w == dist[v]: accumulate ways[v] = (ways[v] + ways[u]) % MOD.
//   * Resolves longest path in O(V + E) without Bellman-Ford or Dijkstra overhead.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ============================================================
// 3. TREE REROOTING DP (ALL-NODES DISTANCE SUM)
// ============================================================

struct TreeRerooting {
    int n;
    vvi adj;
    vi sz; // sz[i] = size of subtree rooted at i
    vl ans, dp; // ans[i] = sum of distances from i to all other nodes, dp[i] = sum of distances from i to all nodes in its subtree

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
                dp[u] += dp[v] + sz[v]; // why + sz[v]? because each node in v's subtree is 1 edge further from u than from v
            }
        }
    }

    void dfs2(int u, int p) {
        for (int v : adj[u]) {
            if (v != p) {
                // When shifting root from u to v, the sum of distances changes as follows:
                // - Nodes in v's subtree (size sz[v]) move 1 unit CLOSER to the root: subtract sz[v].
                // - All other nodes in the tree (size n - sz[v]) move 1 unit FURTHER: add (n - sz[v]).
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
// - Approach: 2-Pass Tree Rerooting DP (Bottom-Up Aggregation + Top-Down Transfer).
// - Intuition:
//   * Running independent BFS/DFS from every node takes O(N^2) time.
//   * Pass 1 (dfs1): Root arbitrarily at 0; compute subtree sizes sz[u] and distance sum dp[u] from u to all nodes in its subtree.
//   * Pass 2 (dfs2): When shifting root from u to neighbor v:
//     - Nodes in v's subtree (size sz[v]) move 1 unit CLOSER to the root: subtract sz[v].
//     - All other nodes in the tree (size N - sz[v]) move 1 unit FURTHER: add (N - sz[v]).
//     - Recurrence: ans[v] = ans[u] - sz[v] + (N - sz[v]).
//   * Computes exact distance sum for all N nodes in O(N) total time.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 4. TREE DIAMETER & NODE ECCENTRICITIES
// ============================================================

struct TreeDiameter {
    int n;
    vvi adj;
    // adj[i] contains the neighbors of node i
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
        int far = src;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] > dist[far]) far = u;
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return {far, dist[far]};
    }

    int diameter() {
        auto [u, _] = bfs(0);
        auto [v, d] = bfs(u);
        return d;
    }
};
// Interview Explanation:
// - Problem Statement: Find diameter (longest path between any two nodes) of an unweighted tree.
// - Approach: Double BFS / 2-Pass Traversal.
// - Intuition:
//   * Tree Property: Starting from ANY arbitrary node (e.g. node 0), the furthest reachable node u is guaranteed to be one endpoint of a diameter.
//   * Run BFS 1 from 0 to locate endpoint u.
//   * Run BFS 2 from u; the furthest node reached is endpoint v, and dist[v] is the exact tree diameter.
//   * Two standard linear traversals achieve O(N) time and O(N) space.
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
// - Problem Statement: Advance k steps in a functional graph (each node has out-degree 1).
// - Approach: Binary Lifting / Successor Doubling Table.
// - Intuition:
//   * Walking k steps one by one takes O(k) per query, which is too slow for large k (e.g. k <= 10^18).
//   * Precompute table up[j][i] = node reached after 2^j jumps from node i.
//   * Recurrence: up[j][i] = up[j - 1][up[j - 1][i]].
//   * Decompose query jump k into binary powers: jump 2^j wherever bit j of k is 1.
//   * Reduces query latency from O(k) to O(log k).
// - Complexity: Build: O(N log N), Query: O(log K), Space: O(N log N).


// ============================================================
// 6. TREE ISOMORPHISM (DOUBLE-HASHING)
// ============================================================

struct TreeIsomorphism {
    int n;
    vvi adj;

    TreeIsomorphism(int n) : n(n), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    ull hash_subtree(int u, int p) {
        vector<ull> ch;
        for (int v : adj[u]) {
            if (v != p) ch.push_back(hash_subtree(v, u));
        }
        sort(ch.begin(), ch.end());
        ull h = 0x9e3779b97f4a7c15ULL;
        for (ull val : ch) {
            h ^= val + 0x517cc1b727220a95ULL + (h << 6) + (h >> 2);
        }
        return h;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if two unrooted trees are structurally isomorphic.
// - Approach: Canonical Subtree Hashing (AHU Algorithm) with Centroid Rooting.
// - Intuition:
//   * Unrooted trees must first be rooted at their centroids (at most 2 candidates) to fix root orientation.
//   * Recursively compute canonical hashes for every subtree.
//   * Sort child subtree hashes lexicographically to eliminate ordering ambiguity among sibling branches.
//   * Combine sorted hashes into parent hash using standard 64-bit splitmix/Murmur hash mixers.
//   * Two trees are isomorphic iff their root canonical hashes match.
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

    int rem = n;
    while (rem > 2) {
        int sz = q.size();
        rem -= sz;
        for (int i = 0; i < sz; i++) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (--deg[v] == 1) q.push(v);
            }
        }
    }

    vi ans;
    while (!q.empty()) {
        ans.push_back(q.front());
        q.pop();
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find center node(s) of a tree that minimize maximum distance to any other node.
// - Approach: Topological Leaf Peeling (Degree-1 BFS Layer Reduction).
// - Intuition:
//   * Tree centers always lie at the middle of every diameter; a tree has either 1 or 2 centers.
//   * Enqueue all current leaves (deg[i] == 1).
//   * Peel off outer leaf layers level-by-level, decrementing degrees of adjacent nodes.
//   * Continue peeling until at most 2 nodes remain in the core.
//   * The remaining 1 or 2 nodes are the exact tree centers.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 9. NODES IN CYCLES (TOPOLOGICAL PEELING)
// ============================================================

vector<bool> findNodesInCycles(int n, vector<vector<int>>& adj) {
    vi deg(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) deg[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++) if (deg[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (--deg[v] == 0) q.push(v);
        }
    }

    vector<bool> inC(n, false);
    for (int i = 0; i < n; i++) {
        if (deg[i] > 0) inC[i] = true; // if deg[i] > 0, node i is part of a cycle
    }
    return inC;
}
// Interview Explanation:
// - Problem Statement: Identify all vertices that belong to or are trapped within directed cycles.
// - Approach: Kahn's Algorithm / In-Degree Zero Peeling.
// - Intuition:
//   * A vertex with in-degree 0 can never be part of a cycle (it has no incoming edge).
//   * Enqueue all vertices with in-degree 0.
//   * When vertex u is removed, decrement in-degree of downstream neighbors; if any neighbor reaches 0, enqueue it.
//   * Nodes belonging to cycles can never have their in-degree drop to 0 because cycles maintain cyclic dependencies.
//   * After queue empties, any node with deg[i] > 0 belongs to a cycle.
// - Complexity: Time: O(V + E), Space: O(V + E).


// ============================================================
// 10. LEXICOGRAPHICAL TOPOLOGICAL SORT
// ============================================================

vector<int> lexicographicalTopoSort(int n, vector<vector<int>>& adj) {
    vi deg(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) deg[v]++;
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 0) pq.push(i);
    }

    vi ans;
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        ans.push_back(u);
        for (int v : adj[u]) {
            if (--deg[v] == 0) pq.push(v);
        }
    }

    return (int)ans.size() == n ? ans : vector<int>{};
}
// Interview Explanation:
// - Problem Statement: Find lexicographically smallest topological sort ordering in a DAG.
// - Approach: Kahn's Algorithm with Min-Heap Priority Queue.
// - Intuition:
//   * Standard topological sort uses a FIFO queue, which selects candidate zero-in-degree nodes arbitrarily.
//   * Replacing the queue with a min-heap guarantees that whenever multiple nodes have in-degree 0, the node with the smallest numerical label is chosen first.
//   * Decrement neighbor in-degrees and push newly freed nodes into min-heap.
//   * If ans.size() < n at the end, graph contains cycles; return empty array.
// - Complexity: Time: O(V log V + E), Space: O(V + E).


// ============================================================
// 11. SHORTEST PATH VISITING ALL NODES — LeetCode 847
// ============================================================

int shortestPathLength(vector<vector<int>>& g) {
    int n = g.size();
    int target = (1 << n) - 1;
    queue<tuple<int, int, int>> q; // {node, mask, dist}
    vector<vector<bool>> vis(n, vector<bool>(1 << n, false)); // vis[node][mask] = true if state (node, mask) has been visited
    // Initialize the queue with all nodes and their initial bitmask
    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i, 0});
        vis[i][1 << i] = true;
    }

    while (!q.empty()) {
        // Dequeue the front element
        auto [u, mask, d] = q.front();
        q.pop();
        if (mask == target) return d;

        for (int v : g[u]) {
            int nxtMask = mask | (1 << v);
            if (!vis[v][nxtMask]) {
                vis[v][nxtMask] = true;
                q.push({v, nxtMask, d + 1});
            }
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Find shortest path visiting every node in an unweighted undirected graph (LeetCode 847).
// - Approach: Multi-Source State Bitmask BFS.
// - Intuition:
//   * Nodes and edges can be revisited; state must track both current node and bitmask of visited nodes.
//   * Start BFS simultaneously from all nodes with initial bitmasks (1 << i) and distance 0.
//   * At each transition to neighbor v, update bitmask to (mask | (1 << v)).
//   * BFS explores states in strictly non-decreasing step count.
//   * The first time any state with mask == (1 << n) - 1 is popped, distance d is guaranteed minimal.
// - Complexity: Time: O(V * 2^V), Space: O(V * 2^V).


// ============================================================
// 12. MAXIMUM SUM BST IN BINARY TREE (LEETCODE 1373)
// ============================================================

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

struct BSTSubtreeInfo {
    bool isBST;
    int mn, mx, sum;
};

BSTSubtreeInfo dfsMaxSumBST(TreeNode* node, int& ans) {
    if (!node) return {true, INT_MAX, INT_MIN, 0};

    auto l = dfsMaxSumBST(node->left, ans);
    auto r = dfsMaxSumBST(node->right, ans);

    if (l.isBST && r.isBST && node->val > l.mx && node->val < r.mn) {
        int curSum = node->val + l.sum + r.sum;
        ans = max(ans, curSum);
        return {true, min(node->val, l.mn), max(node->val, r.mx), curSum};
    }
    return {false, 0, 0, 0};
}

int maxSumBST(TreeNode* root) {
    int ans = 0;
    dfsMaxSumBST(root, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum sum of any subtree that is also a valid Binary Search Tree (LeetCode 1373).
// - Approach: Post-Order Bottom-Up DFS returning {isBST, minVal, maxVal, sumVal}.
// - Intuition:
//   * A subtree rooted at node is a BST iff: (1) left child is BST, (2) right child is BST, and (3) l.max < node->val < r.min.
//   * In a bottom-up post-order traversal, children report their min, max, and sum to parent.
//   * If valid BST condition holds, calculate curSum = node->val + l.sum + r.sum and update global max ans.
//   * If invalid, propagate isBST = false up the tree in O(1).
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 13. REMOVE LEAF NODES WITH TARGET VALUE — LeetCode 1325
// ============================================================

TreeNode* removeLeafNodes(TreeNode* root, int t) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, t);
    root->right = removeLeafNodes(root->right, t);
    if (!root->left && !root->right && root->val == t) return nullptr;
    return root;
}
// Interview Explanation:
// - Problem Statement: Delete all leaf nodes with value target, repeating until no such leaves remain (LeetCode 1325).
// - Approach: Post-Order Recursive Tree Pruning.
// - Intuition:
//   * Deleting leaves can cause their parent nodes to become new leaves.
//   * Post-order traversal processes children before the current node.
//   * Recurse on left and right subtrees first.
//   * If both children become null and current node value equals target t, delete current node by returning nullptr.
//   * Prunes bottom-up in a single pass without multiple iterations.
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 14. POSSIBLE PATH LENGTHS IN DAG
// ============================================================

vector<bool> possiblePathLengths(int n, vector<vector<int>>& adj, int src, int dst) {
    vi deg(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) deg[v]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) if (deg[i] == 0) q.push(i);

    vector<bitset<1001>> dp(n);
    dp[src].set(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            dp[v] |= (dp[u] << 1);
            if (--deg[v] == 0) q.push(v);
        }
    }

    vector<bool> ans(n + 1, false);
    for (int len = 0; len <= n; len++) {
        if (dp[dst].test(len)) ans[len] = true;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Determine all possible path lengths from src to dst in a DAG.
// - Approach: Kahn's TopoSort + Bitset Shift DP.
// - Intuition:
//   * Let dp[u] be a bitset where bit k is 1 if there exists a path of length k from src to u.
//   * Initialize dp[src][0] = 1.
//   * Process vertices in topological order.
//   * Traversing edge (u, v) increases path length by 1: dp[v] |= (dp[u] << 1).
//   * Bitset operations execute 64 bits in parallel, providing extreme speed and compact memory.
// - Complexity: Time: O((V + E) * N / 64), Space: O(V * N / 64).


// ============================================================
// 15. MAX COMPONENTS TREE SPLIT WITH EQUAL SUM — LeetCode 2440
// ============================================================

int dfsComponentValue(int u, int p, int t, const vi& a, const vvi& adj, bool& ok) {
    int sum = a[u];
    for (int v : adj[u]) {
        if (v == p) continue;
        sum += dfsComponentValue(v, u, t, a, adj, ok);
        if (!ok) return 0;
    }
    if (sum > t) { ok = false; return 0; }
    if (sum == t) return 0;
    return sum;
}

int componentValue(vector<int>& a, vector<vector<int>>& edges) {
    int n = a.size();
    vvi adj(n);
    for (auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    int tot = accumulate(a.begin(), a.end(), 0);

    for (int k = n; k >= 1; k--) {
        if (tot % k == 0) {
            bool ok = true;
            if (dfsComponentValue(0, -1, tot / k, a, adj, ok) == 0 && ok) return k - 1;
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Split tree into maximum number of components with equal sum (LeetCode 2440).
// - Approach: Divisor Enumeration + Bottom-Up Subtree Cut Verification.
// - Intuition:
//   * If tree splits into k components with equal sum, each component must sum to target t = total / k.
//   * Test k from n down to 1 (maximizing component count, which maximizes edges removed k - 1).
//   * Bottom-up DFS: accumulate subtree sum.
//   * When subtree sum equals t, cut it off (returns 0 to parent).
//   * If subtree sum exceeds t, valid split is impossible with this target.
// - Complexity: Time: O(N * divisors(Total)), Space: O(N).


// ============================================================
// 16. BINARY LIFTING (LCA, K-TH ANCESTOR)
// ============================================================

struct TreeLifting {
    int n, LOG;
    vvi adj;
    vvi up;
    vi dep;

    TreeLifting(int n) : n(n), LOG(20), adj(n), dep(n, 0) {
        up.assign(n, vi(LOG, 0));
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p, int d) {
        dep[u] = d;
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

    int get_kth_ancestor(int u, int k) {
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) u = up[u][j];
        }
        return u;
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        u = get_kth_ancestor(u, dep[u] - dep[v]);
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
// - Approach: Binary Lifting Table up[u][j] = 2^j-th ancestor.
// - Intuition:
//   * Precompute ancestor table where up[u][j] = up[up[u][j-1]][j-1].
//   * For LCA: equalize depths by lifting the deeper node up by (dep[u] - dep[v]).
//   * If nodes coincide, LCA is found.
//   * Otherwise, lift both u and v in decreasing powers of 2 (LOG - 1 down to 0) as long as their ancestors differ.
//   * The parent of either node up[u][0] is the exact LCA in O(log N).
// - Complexity: Build: O(N log N), Query: O(log N), Space: O(N log N).


// ============================================================
// 17. CYCLE DETECTION & RECONSTRUCTION (UNDIRECTED GRAPH)
// ============================================================

bool dfsDetectCycle(int u, int p, const vvi& adj, vector<bool>& vis, vi& par, int& cs, int& ce) {
    vis[u] = true;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (vis[v]) {
            cs = v;
            ce = u;
            return true;
        }
        par[v] = u;
        if (dfsDetectCycle(v, u, adj, vis, par, cs, ce)) return true;
    }
    return false;
}

vector<int> detectCycleUndirected(int n, vector<vector<int>>& adj) {
    vector<bool> vis(n, false);
    vi par(n, -1);
    int cs = -1, ce = -1;
    for (int i = 0; i < n; i++) {
        if (!vis[i] && dfsDetectCycle(i, -1, adj, vis, par, cs, ce)) break;
    }
    if (cs == -1) return {};
    vi cycle = {cs};
    for (int v = ce; v != cs; v = par[v]) cycle.push_back(v);
    cycle.push_back(cs);
    return cycle;
}
// Interview Explanation:
// - Problem Statement: Detect cycles in undirected graph and output vertex sequence.
// - Approach: DFS Back-Edge Inspection with Parent Tracing.
// - Intuition:
//   * Traverse undirected graph with DFS, passing immediate parent p.
//   * If neighbor v has already been visited and v != p, edge (u, v) is a back-edge forming a cycle.
//   * Record endpoints cs = v and ce = u.
//   * Trace backwards from ce to cs via parent pointers par to collect the cycle vertices.
// - Complexity: Time: O(V + E), Space: O(V).


// ============================================================
// 18. BIDIRECTIONAL BFS TEMPLATE
// ============================================================

template <typename NeighborFunc>
int bidirectionalBFS(int start, int target, const NeighborFunc& getNeighbors) {
    if (start == target) return 0;
    unordered_set<int> fwd = {start}, bwd = {target}; 
    unordered_set<int> vf = {start}, vb = {target}; // vf and vb are for checking visited nodes in each direction
    int steps = 0;

    while (!fwd.empty() && !bwd.empty()) {
        if (fwd.size() > bwd.size()) { 
            swap(fwd, bwd);
            swap(vf, vb);
        } // this is done to ensure we always expand the smaller frontier, minimizing branching factor
        unordered_set<int> nxt;
        steps++;
        for (int u : fwd) {
            for (int v : getNeighbors(u)) {
                if (bwd.count(v)) return steps; // if neighbor v is in the opposite frontier, we have found a connection
                if (!vf.count(v)) { // else if not in visited set, add to next frontier
                    vf.insert(v);
                    nxt.insert(v);
                }
            }
        }
        fwd = std::move(nxt); // move does not copy, it transfers ownership of the set to fwd
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find shortest distance between two states when branching factor is high.
// - Approach: Bidirectional BFS expanding smaller frontier.
// - Intuition:
//   * Unidirectional BFS visits O(B^D) states, which explodes with branching factor B and depth D.
//   * Bidirectional BFS expands frontiers from both start and target simultaneously.
//   * Always expand the smaller frontier set to minimize branching overhead.
//   * When a neighbor of one frontier collides with the other frontier, shortest path is found.
//   * Reduces complexity from O(B^D) to O(B^(D/2)).
// - Complexity: Time: O(B^(D/2)), Space: O(B^(D/2)).


// ============================================================
// 19. JUMP GAME III — LeetCode 1306
// ============================================================

bool canReach(vector<int>& a, int start) {
    int n = a.size();
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(start);
    vis[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (a[u] == 0) return true;
        for (int v : {u + a[u], u - a[u]}) {
            if (v >= 0 && v < n && !vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if you can reach any index with value 0 starting from start, jumping +/- a[i] (LeetCode 1306).
// - Approach: Queue BFS / Graph Reachability.
// - Intuition:
//   * Model indices as graph vertices with directed transitions u -> u + a[u] and u -> u - a[u].
//   * Push starting index into queue and mark visited.
//   * Pop index u; if a[u] == 0, target is reached, return true.
//   * Otherwise enqueue valid in-bounds unvisited neighbors.
//   * Linear traversal ensures every index is visited at most once.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 20. JUMP GAME IV — LeetCode 1345
// ============================================================

int minJumps(vector<int>& a) {
    int n = a.size();
    unordered_map<int, vector<int>> pos;
    for (int i = 0; i < n; i++) pos[a[i]].push_back(i);

    queue<int> q;
    vector<bool> vis(n, false);
    q.push(0);
    vis[0] = true;
    int ans = 0;

    while (!q.empty()) {
        for (int sz = q.size(); sz > 0; sz--) { // Process all nodes at current BFS level
            int u = q.front();
            q.pop();
            if (u == n - 1) return ans; // Reached last index

            if (u - 1 >= 0 && !vis[u - 1]) { // Check left neighbor
                vis[u - 1] = true;
                q.push(u - 1);
            }
            if (u + 1 < n && !vis[u + 1]) { // Check right neighbor
                vis[u + 1] = true;
                q.push(u + 1);
            }
            if (pos.count(a[u])) {
                for (int v : pos[a[u]]) {
                    if (!vis[v]) {
                        vis[v] = true;
                        q.push(v);
                    }
                }
                pos.erase(a[u]); // Critical: prune value list to prevent O(N^2) loops, this is important, otherwise we may revisit the same value's indices multiple times, leading to TLE.
            }
        }
        ans++;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Minimum jumps to reach last index jumping to i-1, i+1, or any j with matching value (LeetCode 1345).
// - Approach: Level-Order BFS with Value Group Pruning.
// - Intuition:
//   * Group all indices sharing identical values in hash map pos.
//   * Level-order BFS guarantees finding the minimum jump distance.
//   * Key optimization: once all indices for value a[u] are enqueued, erase pos[a[u]] immediately.
//   * Erasing prevents subsequent visits from re-iterating over the same large index lists, maintaining linear O(N) runtime.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 21. JUMP GAME V (LEETCODE 1340)
// ============================================================

int dfsMaxJumps(int i, int d, const vector<int>& a, vector<int>& dp) {
    if (dp[i] != 0) return dp[i];
    int n = a.size(), res = 1;
    for (int j = i + 1; j <= min(i + d, n - 1) && a[j] < a[i]; j++)
        res = max(res, 1 + dfsMaxJumps(j, d, a, dp));
    for (int j = i - 1; j >= max(i - d, 0) && a[j] < a[i]; j--)
        res = max(res, 1 + dfsMaxJumps(j, d, a, dp));
    return dp[i] = res;
}

int maxJumps(vector<int>& a, int d) {
    int n = a.size(), ans = 1;
    vector<int> dp(n, 0);
    for (int i = 0; i < n; i++) ans = max(ans, dfsMaxJumps(i, d, a, dp));
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum indices you can visit jumping <= d steps strictly to smaller values (LeetCode 1340).
// - Approach: Memoized DFS / Longest Path on DAG.
// - Intuition:
//   * Jumps are strictly valid only to lower bars without taller obstacles in between (a[j] < a[i]).
//   * Since height strictly decreases with every jump, cycles are impossible; transitions form a DAG.
//   * Let dp[i] be the maximum jumps starting from index i.
//   * Recurse in both left and right directions up to distance d, breaking early if a taller bar is met.
//   * Memoize results in dp[i]; evaluate each state once in O(N * D) time.
// - Complexity: Time: O(N * D), Space: O(N).


// ============================================================
// 22. JUMP GAME VII — LeetCode 1871
// ============================================================

bool canReach(string s, int minJump, int maxJump) {
    int n = s.size(), reach = 0; // reach denotes the count of reachable indices in the sliding window [i - maxJump, i - minJump]
    if (s[n - 1] != '0') return false;
    vector<bool> dp(n, false); // dp[i] = true if index i is reachable
    dp[0] = true; // starting index is always reachable

    for (int i = 1; i < n; i++) {
        if (i >= minJump && dp[i - minJump]) reach++;
        if (i > maxJump && dp[i - maxJump - 1]) reach--;
        // Update the reach count based on the current index
        if (s[i] == '0' && reach > 0) dp[i] = true;
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Check if last index can be reached where s[i] == '0' and jump is in [minJump, maxJump] (LeetCode 1871).
// - Approach: Sliding Window Reachability Count DP in O(N).
// - Intuition:
//   * Index i is reachable if s[i] == '0' and there is at least one reachable index in [i - maxJump, i - minJump].
//   * Naively checking all valid predecessors takes O(N * (maxJump - minJump)) = O(N^2).
//   * Maintain a sliding window counter reach tracking the count of reachable DP states in [i - maxJump, i - minJump].
//   * As i advances: increment reach if index i - minJump is reachable; decrement if index i - maxJump - 1 leaves window.
//   * If s[i] == '0' and reach > 0, set dp[i] = true in O(1) per index.
// - Complexity: Time: O(N), Space: O(N).
