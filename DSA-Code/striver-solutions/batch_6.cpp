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
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct DisjointSet {
    vi parent, rank;
    DisjointSet(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int u) {
        return parent[u] == u ? u : parent[u] = find(parent[u]);
    }
    bool unite(int u, int v) {
        int rootU = find(u), rootV = find(v);
        if (rootU == rootV) return false;
        if (rank[rootU] < rank[rootV]) swap(rootU, rootV);
        parent[rootV] = rootU;
        if (rank[rootU] == rank[rootV]) rank[rootU]++;
        return true;
    }
};

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 6 (Problems 01 to 17)
// ============================================================================

void dfsIsland(vector<vector<char>>& grid, int r, int c, int m, int n) {
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') return;
    grid[r][c] = '0'; // mark visited in-place
    dfsIsland(grid, r + 1, c, m, n);
    dfsIsland(grid, r - 1, c, m, n);
    dfsIsland(grid, r, c + 1, m, n);
    dfsIsland(grid, r, c - 1, m, n);
}

int numIslands(vector<vector<char>>& grid) {
    if (grid.empty()) return 0;
    int m = grid.size(), n = grid[0].size(), count = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfsIsland(grid, i, j, m, n);
            }
        }
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count the number of islands ('1's surrounded by '0's) in an m x n 2D grid.
// - Approach: DFS / Connected Components with in-place sinking of visited land cells.
// - Intuition: Iterate through every grid cell; whenever unvisited land '1' is encountered, increment island count and flood-fill (sink) all 4-directionally connected '1's to '0' to avoid double counting.
// - Complexity: Time: O(M * N) visiting each cell once, Space: O(M * N) worst case recursion stack.

vi findOrder(int numCourses, vvi& prerequisites) {
    vvi adj(numCourses);
    vi in_degree(numCourses, 0);

    for (auto& pre : prerequisites) {
        adj[pre[1]].push_back(pre[0]);
        in_degree[pre[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (in_degree[i] == 0) q.push(i);
    }

    vi order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) q.push(v);
        }
    }

    return (int)order.size() == numCourses ? order : vi{};
}
// Interview Explanation:
// - Problem Statement: Find a valid topological ordering of courses to finish all courses given prerequisite dependencies, or return empty list if cycle exists.
// - Approach: Kahn's Algorithm (BFS-based Topological Sort using in-degrees).
// - Intuition: Nodes with in-degree 0 have no prerequisites and can be taken immediately. Decrement in-degrees of neighbors as courses are completed; if in-degree becomes 0, enqueue. If total ordered courses < numCourses, a dependency cycle exists.
// - Complexity: Time: O(V + E), Space: O(V + E) for adjacency list and queue.

string findAlienOrder(vector<string>& words, int k) {
    vvi adj(k);
    vi in_degree(k, 0);
    vector<bool> present(k, false);

    for (const string& w : words) {
        for (char c : w) present[c - 'a'] = true;
    }

    for (int i = 0; i < (int)words.size() - 1; i++) {
        const string& w1 = words[i];
        const string& w2 = words[i + 1];
        int len = min(w1.size(), w2.size());
        bool found_diff = false;

        for (int j = 0; j < len; j++) {
            if (w1[j] != w2[j]) {
                int u = w1[j] - 'a', v = w2[j] - 'a';
                adj[u].push_back(v);
                in_degree[v]++;
                found_diff = true;
                break;
            }
        }
        if (!found_diff && w1.size() > w2.size()) return ""; // invalid prefix order
    }

    queue<int> q;
    for (int i = 0; i < k; i++) {
        if (present[i] && in_degree[i] == 0) q.push(i);
    }

    string order = "";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order += (char)('a' + u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) q.push(v);
        }
    }

    int total_chars = 0;
    for (int i = 0; i < k; i++) if (present[i]) total_chars++;

    return (int)order.size() == total_chars ? order : "";
}
// Interview Explanation:
// - Problem Statement: Derive the correct alphabetical ordering of characters from a lexicographically sorted list of alien language words.
// - Approach: Directed Graph Construction + Kahn's Topological Sort.
// - Intuition: Compare adjacent words `w1` and `w2` to find the first differing character `w1[j] != w2[j]`, creating a directed edge `w1[j] -> w2[j]`. Run topological sort on the resulting DAG; cycle detection or prefix mismatch flags an invalid dictionary.
// - Complexity: Time: O(N * L + K) where N is words count, L is word length, K is unique characters, Space: O(K + E).

vi dijkstra(int n, vector<vector<pii>>& adj, int src) {
    vi dist(n, INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> pq; // {distance, node}

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue; // outdated entry

        for (auto& [v, weight] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}
// Interview Explanation:
// - Problem Statement: Compute the shortest path distances from a single source vertex to all vertices in a weighted graph with non-negative edge weights.
// - Approach: Greedy exploration with Min-Heap Priority Queue.
// - Intuition: Always expand the unvisited node with the smallest known distance. Relax outgoing edges: if `dist[u] + weight < dist[v]`, update `dist[v]` and push to min-heap.
// - Complexity: Time: O((V + E) log V), Space: O(V + E) for graph and priority queue.

int findCheapestPrice(int n, vvi& flights, int src, int dst, int k) {
    vector<vector<pii>> adj(n);
    for (auto& f : flights) {
        adj[f[0]].push_back({f[1], f[2]});
    }

    queue<tuple<int, int, int>> q; // {stops, node, cost}
    q.push({0, src, 0});

    vi min_cost(n, INT_MAX);
    min_cost[src] = 0;

    while (!q.empty()) {
        auto [stops, u, cost] = q.front();
        q.pop();

        if (stops > k) continue;

        for (auto& [v, price] : adj[u]) {
            if (cost + price < min_cost[v]) {
                min_cost[v] = cost + price;
                q.push({stops + 1, v, cost + price});
            }
        }
    }

    return min_cost[dst] == INT_MAX ? -1 : min_cost[dst];
}
// Interview Explanation:
// - Problem Statement: Find the cheapest flight price from src to dst with at most k intermediate stops.
// - Approach: BFS by stops (Queue-based modified Dijkstra / Bellman-Ford level traversal).
// - Intuition: Since stops increase by 1 at each step, regular BFS (level-by-level) inherently explores paths ordered by stop count. Push `{stops + 1, v, cost + price}` only if it improves `min_cost[v]`.
// - Complexity: Time: O(K * E), Space: O(V + E) for adjacency list and queue.

vi bellmanFord(int n, const vvi& edges, int src) {
    vi dist(n, 1e8);
    dist[src] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (auto& edge : edges) {
            int u = edge[0], v = edge[1], wt = edge[2];
            if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }

    for (auto& edge : edges) {
        int u = edge[0], v = edge[1], wt = edge[2];
        if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
            return {-1}; // negative cycle detected
        }
    }

    return dist;
}
// Interview Explanation:
// - Problem Statement: Compute shortest distances from source in a graph that may contain negative weights, and detect negative weight cycles.
// - Approach: Dynamic Programming edge relaxation over V - 1 iterations.
// - Intuition: In a simple graph without negative cycles, any shortest path contains at most V - 1 edges. Relaxing all edges V - 1 times guarantees convergence; an additional relaxation that improves distance proves the existence of a negative weight cycle.
// - Complexity: Time: O(V * E), Space: O(V) distance array.

void floydWarshall(vvi& matrix) {
    int n = matrix.size();
    const int INF = 1e9;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == -1 && i != j) matrix[i][j] = INF;
        }
    }

    for (int via = 0; via < n; via++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][via] != INF && matrix[via][j] != INF) {
                    matrix[i][j] = min(matrix[i][j], matrix[i][via] + matrix[via][j]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == INF) matrix[i][j] = -1;
        }
    }
}
// Interview Explanation:
// - Problem Statement: Find shortest distances between every pair of vertices in a weighted graph (All-Pairs Shortest Path).
// - Approach: Dynamic Programming with intermediate pivot vertex relaxation (`via`).
// - Intuition: For every pair of nodes `(i, j)`, check if going through intermediate vertex `via` yields a shorter path: `dist[i][j] = min(dist[i][j], dist[i][via] + dist[via][j])`. If `dist[i][i] < 0`, a negative cycle exists.
// - Complexity: Time: O(V^3) three nested loops, Space: O(1) in-place matrix updates.

int spanningTreeKruskal(int n, vector<vector<int>>& edges) {
    sort(edges.begin(), edges.end(), [](const vi& a, const vi& b) {
        return a[2] < b[2];
    });

    DisjointSet ds(n);
    int mst_weight = 0, edges_count = 0;

    for (auto& edge : edges) {
        if (ds.unite(edge[0], edge[1])) {
            mst_weight += edge[2];
            edges_count++;
            if (edges_count == n - 1) break;
        }
    }

    return mst_weight;
}

int spanningTreePrim(int n, vector<vector<pii>>& adj) {
    priority_queue<pii, vector<pii>, greater<pii>> pq; // {weight, node}
    vector<bool> visited(n, false);
    pq.push({0, 0});
    int mst_weight = 0;

    while (!pq.empty()) {
        auto [wt, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;
        mst_weight += wt;

        for (auto& [v, weight] : adj[u]) {
            if (!visited[v]) {
                pq.push({weight, v});
            }
        }
    }

    return mst_weight;
}
// Interview Explanation:
// - Problem Statement: Find the total weight of the Minimum Spanning Tree (MST) in a connected undirected weighted graph.
// - Approach: Kruskal's Algorithm (Greedy Edge Sorting + DSU) & Prim's Algorithm (Greedy Node Expansion + Min-Heap).
// - Intuition: Kruskal greedily adds the smallest available edge that doesn't create a cycle using Union-Find. Prim greedily connects the closest unvisited vertex to the growing MST using a priority queue.
// - Complexity: Kruskal: O(E log E), Prim: O((V + E) log V), Space: O(V + E).

vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    DisjointSet ds(n);
    unordered_map<string, int> email_to_id;

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (email_to_id.count(email)) {
                ds.unite(i, email_to_id[email]);
            } else {
                email_to_id[email] = i;
            }
        }
    }

    unordered_map<int, vector<string>> merged_emails;
    for (auto& [email, id] : email_to_id) {
        int root = ds.find(id);
        merged_emails[root].push_back(email);
    }

    vector<vector<string>> ans;
    for (auto& [root_id, emails] : merged_emails) {
        sort(emails.begin(), emails.end());
        vector<string> account = {accounts[root_id][0]};
        account.insert(account.end(), emails.begin(), emails.end());
        ans.push_back(account);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Merge user accounts that share at least one email address, returning sorted emails under the owner's name.
// - Approach: Disjoint Set Union (DSU) connecting account indices sharing common emails.
// - Intuition: Map each email to the first account index that listed it. When seen again in another account, union the two account indices. Group all emails under their DSU root representative, sort alphabetically, and prepend user name.
// - Complexity: Time: O(N * M * log(N * M)) due to email sorting, Space: O(N * M) for DSU and hash maps.

void tarjanBridgesDFS(int u, int parent_node, int& timer, const vvi& adj,
                     vi& tin, vi& low, vector<bool>& vis, vvi& bridges) {
    vis[u] = true;
    tin[u] = low[u] = ++timer;

    for (int v : adj[u]) {
        if (v == parent_node) continue;
        if (vis[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            tarjanBridgesDFS(v, u, timer, adj, tin, low, vis, bridges);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) {
                bridges.push_back({u, v});
            }
        }
    }
}

vvi criticalConnections(int n, vvi& connections) {
    vvi adj(n);
    for (auto& edge : connections) {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    vi tin(n, 0), low(n, 0);
    vector<bool> vis(n, false);
    vvi bridges;
    int timer = 0;

    tarjanBridgesDFS(0, -1, timer, adj, tin, low, vis, bridges);
    return bridges;
}
// Interview Explanation:
// - Problem Statement: Find all critical connections (bridges) in an undirected graph whose removal disconnects the graph.
// - Approach: Tarjan's Bridge-Finding Algorithm using DFS discovery time (`tin`) and lowest reachable ancestor (`low`).
// - Intuition: For an edge `(u, v)`, if `low[v] > tin[u]`, it means the subtree rooted at `v` has no back-edge to reach `u` or any ancestor of `u`. Therefore, removing edge `(u, v)` completely isolates `v`, making it a bridge.
// - Complexity: Time: O(V + E) single DFS pass, Space: O(V + E) for adjacency list and DFS arrays.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
// Interview Explanation:
// - Problem Statement: Find the maximum depth (number of nodes along the longest path from root to leaf) of a binary tree.
// - Approach: Post-order DFS recursion.
// - Intuition: The depth of a node is 1 plus the maximum depth of its left and right subtrees. Base case: depth of nullptr is 0.
// - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack.

bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
// Interview Explanation:
// - Problem Statement: Given the roots of two binary trees p and q, check if they are structurally identical and have same node values.
// - Approach: Simultaneous recursive DFS traversal.
// - Intuition: If both nodes are null, trees match. If one is null or values mismatch, return false. Recursively check both left and right child pairs.
// - Complexity: Time: O(min(N, M)), Space: O(min(H1, H2)) recursion stack.

TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
// Interview Explanation:
// - Problem Statement: Invert/flip a binary tree into its mirror image.
// - Approach: Post-order or Pre-order recursive DFS pointer swapping.
// - Intuition: Swap the left and right child pointers of the current node, then recursively invert the left and right subtrees.
// - Complexity: Time: O(N) visiting every node, Space: O(H) recursion stack.

vvi levelOrder(TreeNode* root) {
    if (!root) return {};
    vvi ans;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        vi level;
        for (int i = 0; i < sz; i++) {
            TreeNode *node = q.front();
            q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ans.push_back(level);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Return the level order traversal of a binary tree's nodes' values as a list of lists.
// - Approach: Breadth-First Search (BFS) using a Queue.
// - Intuition: Capture `sz = q.size()` at the start of each level to iterate exactly the nodes belonging to the current depth level, pushing their children for the next level.
// - Complexity: Time: O(N) single BFS pass, Space: O(N) max nodes at any level.

class CodecTree {
private:
    TreeNode* deserializeHelper(stringstream& ss) {
        string val;
        if (!getline(ss, val, ',')) return nullptr;
        if (val == "#") return nullptr;

        TreeNode* node = new TreeNode(stoi(val));
        node->left = deserializeHelper(ss);
        node->right = deserializeHelper(ss);
        return node;
    }

public:
    string serialize(TreeNode* root) {
        if (!root) return "#";
        return to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
    }

    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }
};
// Interview Explanation:
// - Problem Statement: Design an algorithm to serialize a binary tree to a string and deserialize that string back to the original tree structure.
// - Approach: Pre-order DFS Traversal with comma delimiters and '#' for null nodes.
// - Intuition: Pre-order DFS `root, left, right` with explicit null markers preserves unique tree structure unambiguously without needing both inorder and preorder traversals.
// - Complexity: Time: O(N) for both serialize and deserialize, Space: O(N) for string representation and recursion stack.

bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if (!root) return false;
    if (isSameTree(root, subRoot)) return true;
    return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}
// Interview Explanation:
// - Problem Statement: Given roots of two binary trees root and subRoot, return true if there is a subtree of root with the same structure and node values as subRoot.
// - Approach: Tree DFS traversal calling `isSameTree` helper at each node.
// - Intuition: At each node in `root`, check if the subtree rooted here is identical to `subRoot`. If not, recursively search in left or right subtrees.
// - Complexity: Time: O(N * M) worst case, Space: O(H) recursion stack.

bool isValidBSTHelper(TreeNode* root, long long min_val, long long max_val) {
    if (!root) return true;
    if (root->val <= min_val || root->val >= max_val) return false;
    return isValidBSTHelper(root->left, min_val, root->val) &&
           isValidBSTHelper(root->right, root->val, max_val);
}

bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}
// Interview Explanation:
// - Problem Statement: Determine if a given binary tree is a valid Binary Search Tree (BST).
// - Approach: DFS validation propagating valid value range `(min_val, max_val)` using 64-bit bounds.
// - Intuition: Every node in left subtree must be `< root->val` and every node in right subtree must be `> root->val`. Propagate `(min_val, max_val)` downwards, narrowing the range at each child (`(min_val, root->val)` for left, `(root->val, max_val)` for right).
// - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack.
