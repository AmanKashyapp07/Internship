#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <bitset>
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
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

// Definition for a binary tree node
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 70 | Construct Tree from Preorder & Inorder      | Divide-and-Conquer + Index Map    | O(N)     | O(N)     |
 | 71 | Balanced Binary Tree                        | Bottom-Up Depth Check             | O(N)     | O(H)     |
 | 72 | Binary Tree Maximum Path Sum                | Postorder Subtree Max Gain        | O(N)     | O(H)     |
 | 73 | Kth Smallest Element in a BST               | Iterative Inorder Traversal       | O(H + k) | O(H)     |
 | 74 | Serialize and Deserialize Binary Tree       | Preorder String Stream Traversal  | O(N)     | O(N)     |
 | 75 | Diameter of Binary Tree                     | Postorder Max Path Combination    | O(N)     | O(H)     |
 | 76 | Two Sum IV - Input is a BST                 | BST Iterator Two Pointers         | O(N)     | O(H)     |
 | 77 | All Nodes Distance K in Binary Tree         | Parent Mapping + BFS Radial Wave  | O(N)     | O(N)     |
 | 78 | Vertical Order Traversal of a Binary Tree   | Coordinate DFS + Multiset Sorting | O(N logN)| O(N)     |
 | 79 | Merge BSTs to Create Single BST             | Leaf-Root Map + In-Order BST Valid| O(N)     | O(N)     |
 | 80 | Redundant Connection                        | Disjoint Set Union (DSU) Cycle    | O(N α(N))| O(N)     |
 | 81 | Accounts Merge                              | DSU Connected Components on Emails| O(AlogA) | O(A)     |
 | 82 | Find Eventual Safe States                   | Reverse Graph Topological Sort    | O(V + E) | O(V + E) |
 | 83 | Making A Large Island                       | Component Sizing + Boundary Probe | O(M * N) | O(M * N) |
 | 84 | Min Edge Reversals for Reachability         | Tree Re-Rooting Dynamic Prog.     | O(N)     | O(N)     |
 | 85 | Path Existence Queries in a Graph I         | Union-Find Component Equivalence  | O(N + Q) | O(N)     |
 | 86 | Longest Valid Parentheses                   | Monotonic Index Stack / DP        | O(N)     | O(N)     |
 | 87 | Largest Rectangle in Histogram              | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 88 | Maximal Rectangle                           | Row-Wise Histogram Height Matrix  | O(M * N) | O(N)     |
 | 89 | Min Stack                                   | Pair Value-Min Tracking Stack     | O(1) ops | O(N)     |
 | 90 | Basic Calculator                            | Sign & Running Total Stack State  | O(N)     | O(N)     |
 | 91 | Sliding Window Maximum                      | Monotonic Decreasing Deque        | O(N)     | O(k)     |
 | 92 | Find K Pairs with Smallest Sums             | Priority Queue Frontier Expansion | O(K logK)| O(K)     |
 | 93 | Next Greater Element I                      | Monotonic Stack + Hash Map Lookup | O(N + M) | O(M)     |
 ====================================================================================================
*/

// =========================================================
// 70. CONSTRUCT BINARY TREE FROM PREORDER AND INORDER (LC 105)
// =========================================================

class Solution70 {
    unordered_map<int, int> pos;
    TreeNode* build(vector<int>& preorder, int& idx, int left, int right) {
        if (left > right) return nullptr;
        int val = preorder[idx++];
        TreeNode* root = new TreeNode(val);
        int mid = pos[val];
        root->left = build(preorder, idx, left, mid - 1);
        root->right = build(preorder, idx, mid + 1, right);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        for (int i = 0; i < (int)inorder.size(); ++i) pos[inorder[i]] = i;
        int idx = 0;
        return build(preorder, idx, 0, (int)inorder.size() - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Construct binary tree given preorder and inorder traversal arrays.
// - Approach: Recursive Divide and Conquer + Inorder Hash Map Lookups.
// - Intuition:
//   * Preorder gives root value first.
//   * Inorder divides elements into left subtree (left of root) and right subtree (right of root).
//   * Pre-indexing inorder positions in a hash map yields O(1) subtree boundary partitioning.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 71. BALANCED BINARY TREE (LC 110)
// =========================================================

class Solution71 {
    int check(TreeNode* node) {
        if (!node) return 0;
        int l = check(node->left), r = check(node->right);
        if (l == -1 || r == -1 || abs(l - r) > 1) return -1;
        return 1 + max(l, r);
    }
public:
    bool isBalanced(TreeNode* root) {
        return check(root) != -1;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if binary tree is height-balanced (subtree height diff <= 1 for every node).
// - Approach: Bottom-Up Postorder Traversal with Sentinel -1 for Unbalance.
// - Intuition:
//   * Top-down calculation leads to repeated height calls (O(N^2)).
//   * Bottom-up checks balance while computing height; if any subtree is unbalanced, immediately bubble -1 up.
// - Complexity: Time: O(N), Space: O(H) recursion stack.

// =========================================================
// 72. BINARY TREE MAXIMUM PATH SUM (LC 124)
// =========================================================

class Solution72 {
    int globalMax = INT_MIN;
    int gain(TreeNode* node) {
        if (!node) return 0;
        int l = max(0, gain(node->left)), r = max(0, gain(node->right));
        globalMax = max(globalMax, node->val + l + r);
        return node->val + max(l, r);
    }
public:
    int maxPathSum(TreeNode* root) {
        globalMax = INT_MIN;
        gain(root);
        return globalMax;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum path sum of any non-empty path in a binary tree.
// - Approach: Postorder DFS Subtree Max Gain Propagation.
// - Intuition:
//   * At each node, compute max gain extending downwards through left and right children (clamped to 0).
//   * A path can turn at `node` (combining left + node + right) to update `globalMax`.
//   * The node can only return `node->val + max(left, right)` upwards to parent to form a valid non-branching path.
// - Complexity: Time: O(N), Space: O(H).

// =========================================================
// 73. KTH SMALLEST ELEMENT IN A BST (LC 230)
// =========================================================

class Solution73 {
  public:
    int countNodes(TreeNode *node) {
        if (!node)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int kthSmallest(TreeNode *root, int k) {
        stack<TreeNode *> st;
        TreeNode *curr = root;

        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();

            if (--k == 0) {
                return curr->val;
            }
            curr = curr->right;
        }
        return -1;
    }
    int kthlargest(TreeNode *root, int k) {
        return kthSmallest(root, countNodes(root) - k + 1);
    }
};
// Interview Explanation:
// - Problem Statement: Find kth smallest element in a Binary Search Tree (BST).
// - Approach: Iterative Inorder Traversal using Stack.
// - Intuition:
//   * Inorder traversal of a BST visits nodes in strictly sorted ascending order.
//   * An explicit stack halts traversal as soon as the kth element is reached, running in O(H + k) time without visiting the full tree.
// - Complexity: Time: O(H + k), Space: O(H).

// =========================================================
// 74. SERIALIZE AND DESERIALIZE BINARY TREE (LC 297)
// =========================================================

class Solution74 {
    void encode(TreeNode* root, ostringstream& out) {
        if (!root) { out << "# "; return; }
        out << root->val << " ";
        encode(root->left, out);
        encode(root->right, out);
    }
    TreeNode* decode(istringstream& in) {
        string s;
        if (!(in >> s) || s == "#") return nullptr;
        TreeNode* root = new TreeNode(stoi(s));
        root->left = decode(in);
        root->right = decode(in);
        return root;
    }
public:
    string serialize(TreeNode* root) {
        ostringstream out;
        encode(root, out);
        return out.str();
    }
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return decode(in);
    }
};
// Interview Explanation:
// - Problem Statement: Design algorithm to serialize and deserialize a binary tree to/from string.
// - Approach: Preorder Traversal with Sentinel Markers for Null Nodes.
// - Intuition:
//   * Preorder traversal explicitly preserving null sentinels ("#") unambiguously reconstructs the tree without requiring an inorder array.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 75. DIAMETER OF BINARY TREE (LC 543)
// =========================================================

class Solution75 {
    int maxDiameter = 0;
    int depth(TreeNode* node) {
        if (!node) return 0;
        int l = depth(node->left), r = depth(node->right);
        maxDiameter = max(maxDiameter, l + r);
        return 1 + max(l, r);
    }
public:
    int diameterOfBinaryTree(TreeNode* root) {
        maxDiameter = 0;
        depth(root);
        return maxDiameter;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of the longest path between any two nodes in a tree (number of edges).
// - Approach: Postorder Max Depth Aggregation.
// - Intuition:
//   * For any node, the longest path passing through it as root is leftDepth + rightDepth.
//   * Compute depth recursively bottom-up while maintaining the global maximum diameter.
// - Complexity: Time: O(N), Space: O(H).

// =========================================================
// 76. TWO SUM IV - INPUT IS A BST (LC 653)
// =========================================================
class Solution76 {
    unordered_set<int> seen;
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        if (seen.count(k - root->val)) return true;
        seen.insert(root->val);
        return findTarget(root->left, k) || findTarget(root->right, k);
    }
};
// Interview Explanation:
// - Problem Statement: Given BST root and target k, return true if there exist two elements whose sum equals k.
// - Approach: Two-Pointer Convergence via Dual BST Iterators.
// - Intuition:
//   * One iterator traverses smallest-to-largest (forward inorder), the other largest-to-smallest (reverse inorder).
//   * Simulates standard sorted two-pointer sweep in O(H) space rather than dumping the whole tree into an array (O(N) space).
// - Complexity: Time: O(N), Space: O(H).

// =========================================================
// 77. ALL NODES DISTANCE K IN BINARY TREE (LC 863)
// =========================================================

class Solution77 {
    unordered_map<TreeNode*, TreeNode*> parent;

    void setParent(TreeNode* node) {
        if (!node) return;
        if (node->left) { parent[node->left] = node; setParent(node->left); }
        if (node->right) { parent[node->right] = node; setParent(node->right); }
    }

public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        parent.clear();
        setParent(root);

        queue<TreeNode*> q;
        unordered_set<TreeNode*> visited;
        q.push(target);
        visited.insert(target);

        while (k-- && !q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto* node = q.front(); q.pop();
                for (auto* next : {node->left, node->right, parent[node]}) {
                    if (next && visited.insert(next).second) {
                        q.push(next);
                    }
                }
            }
        }

        vector<int> ans;
        while (!q.empty()) {
            ans.push_back(q.front()->val);
            q.pop();
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Return list of values of all nodes that have a distance k from target node.
// - Approach: Parent Pointer Graph Conversion + Multi-Directional BFS.
// - Intuition:
//   * A tree only provides child pointers. Adding parent pointers converts it to a general undirected graph.
//   * BFS radiating out from `target` up to radius k finds all nodes at distance k in level order.
// - Complexity: Time: O(N), Space: O(N).

class Solution78 {
    vector<tuple<int,int,int>> v;

    void dfs(TreeNode* root, int row, int col) {
        if (!root) return;
        v.push_back({col, row, root->val});
        dfs(root->left, row + 1, col - 1);
        dfs(root->right, row + 1, col + 1);
    }

public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        dfs(root, 0, 0);
        sort(v.begin(), v.end());

        vector<vector<int>> ans;
        int prev = INT_MIN;

        for (auto [col, row, val] : v) {
            if (col != prev) {
                ans.push_back({});
                prev = col;
            }
            ans.back().push_back(val);
        }

        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Calculate vertical order traversal of binary tree sorted by column, then row, then value.
// - Approach: Coordinate DFS + Nested Map / Multiset Sorting.
// - Intuition:
//   * Assign root coordinate (0, 0). Left child has (row + 1, col - 1), right has (row + 1, col + 1).
//   * Store in `map<col, map<row, multiset<val>>>` to automatically handle ordering constraints.
// - Complexity: Time: O(N log N), Space: O(N).

// =========================================================
// 79. MERGE BSTS TO CREATE SINGLE BST (LC 1932)
// =========================================================

class Solution79 {
    unordered_map<int, TreeNode*> mp;
    bool dfs(TreeNode* node, long long low, long long high) {
        if (!node) return true;
        if (node->val <= low || node->val >= high) return false;
        // Replace leaf with matching BST
        if (!node->left && !node->right && mp.count(node->val)) {
            TreeNode* t = mp[node->val];
            node->left = t->left;
            node->right = t->right;
            mp.erase(node->val);
        }
        return dfs(node->left, low, node->val) && dfs(node->right, node->val, high);
    }
public:
    TreeNode* canMerge(vector<TreeNode*>& trees) {
        unordered_set<int> leaves;
        for (auto root : trees) {
            mp[root->val] = root;
            if (root->left) leaves.insert(root->left->val);
            if (root->right) leaves.insert(root->right->val);
        }
        // Find the only root that is not a leaf
        TreeNode* root = nullptr;
        for (auto tree : trees) {
            if (!leaves.count(tree->val)) {
                if (root) return nullptr;
                root = tree;
            }
        }
        if (!root) return nullptr;
        mp.erase(root->val);
        if (!dfs(root, LLONG_MIN, LLONG_MAX) || !mp.empty()) return nullptr;
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Merge n disjoint BSTs into a single valid BST by replacing leaves with matching tree roots.
// - Approach: Leaf Frequency In-Degree Check + Recursive BST Splice Validation.
// - Intuition:
//   * The overall BST root must never be a leaf of any other tree (in-degree == 0).
//   * Expand matching leaves during a valid BST range check [minVal, maxVal]. Verify all n - 1 other trees merge cleanly.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 80. REDUNDANT CONNECTION (LC 684)
// =========================================================

class Solution80 {
    vector<int> parent;
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        parent.resize(edges.size() + 1);
        iota(parent.begin(), parent.end(), 0);
        for (const auto& e : edges) {
            int rx = find(e[0]), ry = find(e[1]);
            if (rx == ry) return e;
            parent[rx] = ry;
        }
        return {};
    }
};
// Interview Explanation:
// - Problem Statement: Find edge that can be removed so graph becomes a tree of n nodes.
// - Approach: Disjoint Set Union (DSU). Process edges sequentially; first edge connecting already-connected vertices is redundant.
// - Intuition: A tree with an extra edge contains exactly one cycle. The edge completing that cycle is the answer.
// - Complexity: Time: O(N * alpha(N)), Space: O(N).


// =========================================================
// 81. ACCOUNTS MERGE (LC 721)
// =========================================================

class Solution81 {
    vector<int> parent;
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        int n = accounts.size();
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);

        unordered_map<string, int> emailToId;
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < (int)accounts[i].size(); ++j) {
                const auto& email = accounts[i][j];
                if (emailToId.count(email)) {
                    parent[find(i)] = find(emailToId[email]);
                } else {
                    emailToId[email] = i;
                }
            }
        }

        unordered_map<int, vector<string>> leaderToEmails;
        for (const auto& [email, id] : emailToId) {
            leaderToEmails[find(id)].push_back(email);
        }

        vector<vector<string>> mergedAccounts;
        for (auto& [leader, emails] : leaderToEmails) {
            sort(emails.begin(), emails.end());
            vector<string> account = {accounts[leader][0]};
            account.insert(account.end(), emails.begin(), emails.end());
            mergedAccounts.push_back(std::move(account));
        }
        return mergedAccounts;
    }
};
// Interview Explanation:
// - Problem Statement: Merge accounts sharing common email addresses and return sorted email lists per owner.
// - Approach: Union-Find (DSU) where accounts are nodes and common emails create edges between account indices.
// - Intuition: Map each email to first account seen; if seen again, union current account with previous. Group by DSU component representative.
// - Complexity: Time: O(A log A) where A is total emails across all accounts (due to sorting), Space: O(A).


// =========================================================
// 82. FIND EVENTUAL SAFE STATES (LC 802)
// =========================================================

class Solution82 {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<vector<int>> rev(n);
        vector<int> out(n);
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            out[u] = graph[u].size();
            if (out[u] == 0) q.push(u);
            for (int v : graph[u]) rev[v].push_back(u);
        }

        vector<int> safe;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            safe.push_back(u);
            for (int p : rev[u]) {
                if (--out[p] == 0) q.push(p);
            }
        }
        sort(safe.begin(), safe.end());
        return safe;
    }
};
// Interview Explanation:
// - Problem Statement: Find all safe nodes (every path starting from the node leads to a terminal node).
// - Approach: Reverse Graph + Kahn's Topological Sort.
// - Intuition:
//   * Terminal nodes have out-degree 0.
//   * By reversing edges and processing nodes whose out-degree drops to 0, we peel away cycles and find all safe nodes.
// - Complexity: Time: O(V + E), Space: O(V + E).

// =========================================================
// 83. MAKING A LARGE ISLAND (LC 827)
// =========================================================

class Solution83 {
    int n;
    int dfs(vector<vector<int>>& g, int r, int c, int curId) {
        if (r < 0 || r >= n || c < 0 || c >= n || g[r][c] != 1) return 0;
        g[r][c] = curId;
        return 1 + dfs(g, r + 1, c, curId) + dfs(g, r - 1, c, curId)
                 + dfs(g, r, c + 1, curId) + dfs(g, r, c - 1, curId);
    }
public:
    int largestIsland(vector<vector<int>>& g) {
        n = g.size();
        int id = 2, ans = 0;
        vector<int> area(n * n + 2, 0);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (g[i][j] == 1) {
                    area[id] = dfs(g, i, j, id);
                    ans = max(ans, area[id++]);
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (g[i][j]) continue;
                int total = 1;
                unordered_set<int> seen;
                for (auto [dr, dc] : {pair<int, int>{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
                    int ni = i + dr, nj = j + dc;
                    if (ni >= 0 && ni < n && nj >= 0 && nj < n && g[ni][nj] > 1 && seen.insert(g[ni][nj]).second) {
                        total += area[g[ni][nj]];
                    }
                }
                ans = max(ans, total);
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum island area achievable by changing at most one 0 to 1.
// - Approach: Component Labeling + 4-Directional Boundary Bridge Probing.
// - Intuition:
//   * Label each island with a unique ID and store its area in a hash map.
//   * For every 0 cell, examine its 4 neighbors and sum the unique adjacent island areas + 1.
// - Complexity: Time: O(N^2), Space: O(N^2).


// =========================================================
// 84. MIN EDGE REVERSALS FOR REACHABILITY (LC 2858)
// =========================================================

class Solution84 {
    vector<vector<pair<int, int>>> g;
    vector<int> ans;

    int dfs1(int u, int p) {
        int cost = 0;
        for (auto [v, rev] : g[u]) {
            if (v != p) cost += rev + dfs1(v, u);
        }
        return cost;
    }

    void dfs2(int u, int p) {
        for (auto [v, rev] : g[u]) {
            if (v != p) {
                ans[v] = ans[u] + (rev ? -1 : 1);
                dfs2(v, u);
            }
        }
    }

public:
    vector<int> minEdgeReversals(int n, vector<vector<int>>& edges) {
        g.assign(n, {});
        ans.assign(n, 0);
        for (const auto& e : edges) {
            g[e[0]].push_back({e[1], 0}); // forward edge (cost 0)
            g[e[1]].push_back({e[0], 1}); // backward edge (cost 1 to reverse)
        }

        ans[0] = dfs1(0, -1);
        dfs2(0, -1);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find min edge reversals to reach all nodes from each node in directed tree.
// - Approach: Tree Re-Rooting Dynamic Programming.
// - Intuition:
//   * Calculate answer for root (node 0) using bottom-up DFS.
//   * When shifting root from u to neighbor v across edge u -> v (or v -> u), cost changes by +1 or -1.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 85. PATH EXISTENCE QUERIES IN A GRAPH I (LC 3532)
// =========================================================
class Solution85 {
public:
    vector<bool> pathExistenceQueries(
        int n,
        vector<int>& nums,
        int maxDiff,
        vector<vector<int>>& queries
    ) {
        vector<int> group(n);
        int id = 0;
        // Create connected components
        for (int i = 1; i < n; i++) {
            if (nums[i] - nums[i - 1] > maxDiff) id++;
            group[i] = id;
        }

        vector<bool> ans;

        for (auto& q : queries) {
            int u = q[0];
            int v = q[1];
            ans.push_back(group[u] == group[v]);
        }

        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Determine whether a path exists between u and v for multiple query pairs.
// - Approach: Disjoint Set Union (DSU) Connectivity.
// - Intuition:
//   * A path exists between two nodes in an undirected graph if and only if they reside within the same connected component.
//   * DSU answers each query in near O(1) time after O(V + E) preprocessing.
// - Complexity: Time: O(N + E + Q * α(N)), Space: O(N).

// =========================================================
// 86. LONGEST VALID PARENTHESES (LC 32)
// =========================================================

class Solution86 {
  public:
    int longestValidParentheses(string s) {
        stack<int> st;
        st.push(-1);
        int maxLen = 0, n = (int)s.size();
        for (int i = 0; i < n; ++i) {
            if (s[i] == '(') {
                st.push(i);
            } else {
                st.pop();
                if (st.empty()) {
                    st.push(i);
                } else {
                    maxLen = max(maxLen, i - st.top());
                }
            }
        }
        return maxLen;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest valid (well-formed) parentheses substring.
// - Approach: Monotonic Stack of Indices with Base Sentinel.
// - Intuition:
//   * Store index of last unmatched closing parenthesis (sentinel -1 initially).
//   * For '(', push index. For ')', pop. If stack is non-empty, current valid length is `i - st.top()`.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 87. LARGEST RECTANGLE IN HISTOGRAM (LC 84)
// =========================================================

class Solution87 {
  public:
    int largestRectangleArea(vector<int> &heights) {
        int n = (int)heights.size();
        stack<int> st;
        int maxArea = 0;

        for (int i = 0; i <= n; ++i) {
            int h = (i == n) ? 0 : heights[i];
            while (!st.empty() && h < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int width = st.empty() ? i : (i - st.top() - 1);
                maxArea = max(maxArea, height * width);
            }
            st.push(i);
        }
        return maxArea;
    }
};
// Interview Explanation:
// - Problem Statement: Find area of the largest rectangle in a histogram.
// - Approach: Monotonic Increasing Stack with Sentinel 0.
// - Intuition:
//   * For each bar, find first smaller bar to the left and right.
//   * When a smaller bar is encountered at index `i`, pop and compute maximum rectangle with popped bar as height and width `i - st.top() - 1`.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 88. MAXIMAL RECTANGLE (LC 85)
// =========================================================

class Solution88 {
  public:
    int maximalRectangle(vector<vector<char>> &matrix) {
        if (matrix.empty() || matrix[0].empty())
            return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        vector<int> height(n, 0);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                height[j] = (matrix[i][j] == '1') ? height[j] + 1 : 0;
            }
            stack<int> st;
            for (int j = 0; j <= n; j++) {
                int currHeight = (j == n ? 0 : height[j]);
                while (!st.empty() && currHeight < height[st.top()]) {
                    int h = height[st.top()];
                    st.pop();
                    int width = st.empty() ? j : j - st.top() - 1;
                    ans = max(ans, h * width);
                }
                st.push(j);
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find largest rectangle containing only 1s in a 2D binary matrix.
// - Approach: Row-Wise Cumulative Heights + Monotonic Histogram Stack.
// - Intuition:
//   * Cumulative consecutive 1s in each column form a dynamic histogram up to that row.
//   * Running LC 84 on each row solves 2D maximal rectangle in O(M * N).
// - Complexity: Time: O(M * N), Space: O(N).

// =========================================================
// 89. MIN STACK (LC 155)
// =========================================================

class Solution89 {
  public:
    class MinStack {
        stack<pair<int, int>> st; // {val, currentMin}
      public:
        MinStack() {}
        void push(int val) {
            int curMin = st.empty() ? val : min(val, st.top().second);
            st.push({val, curMin});
        }
        void pop() { st.pop(); }
        int top() { return st.top().first; }
        int getMin() { return st.top().second; }
    };
};
// Interview Explanation:
// - Problem Statement: Design a stack that supports push, pop, top, and retrieving the minimum element in O(1) time.
// - Approach: Stack of Value-PrefixMin Pairs.
// - Intuition:
//   * Storing the prefix minimum alongside each element retains historical minimums without extra state lookups during pop.
// - Complexity: Time: O(1) per operation, Space: O(N).

// =========================================================
// 90. BASIC CALCULATOR (LC 224)
// =========================================================

class Solution90 {
  public:
    int calculate(string s) {
        stack<int> st;
        int result = 0, num = 0, sign = 1;

        for (char c : s) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            } else if (c == '+') {
                result += sign * num;
                num = 0;
                sign = 1;
            } else if (c == '-') {
                result += sign * num;
                num = 0;
                sign = -1;
            } else if (c == '(') {
                st.push(result);
                st.push(sign);
                result = 0;
                sign = 1;
            } else if (c == ')') {
                result += sign * num;
                num = 0;
                result *= st.top();
                st.pop();
                result += st.top();
                st.pop();
            }
        }
        return result + sign * num;
    }
};
// Interview Explanation:
// - Problem Statement: Evaluate expression string containing digits, '+', '-', '(', ')', and spaces.
// - Approach: Sign Tracking + State Context Stack.
// - Intuition:
//   * Expressions evaluate linearly from left to right unless nested parentheses occur.
//   * Push `result` and `sign` onto stack when entering '(', reset context, then pop and resolve upon encountering ')'.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 91. SLIDING WINDOW MAXIMUM (LC 239)
// =========================================================

class Solution91 {
  public:
    vector<int> maxSlidingWindow(vector<int> &nums, int k) {
        deque<int> dq;
        vector<int> result;
        result.reserve(nums.size() - k + 1);

        for (int i = 0; i < (int)nums.size(); ++i) {
            while (!dq.empty() && dq.front() <= i - k)
                dq.pop_front();
            while (!dq.empty() && nums[dq.back()] <= nums[i])
                dq.pop_back();
            dq.push_back(i);
            if (i >= k - 1)
                result.push_back(nums[dq.front()]);
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Given array nums and window size k, return max sliding window at each step.
// - Approach: Monotonic Decreasing Deque of Indices.
// - Intuition:
//   * Front of deque always holds index of maximum element for current window.
//   * Elements smaller than current incoming element can never be window maximum -> pop from back.
// - Complexity: Time: O(N), Space: O(k).

// =========================================================
// 92. FIND K PAIRS WITH SMALLEST SUMS (LC 373)
// =========================================================

class Solution92 {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& a, vector<int>& b, int k) {
        vector<vector<int>> ans;

        using T = tuple<int,int,int>; // sum, i, j
        priority_queue<T, vector<T>, greater<T>> pq;

        for (int i = 0; i < min((int)a.size(), k); i++)
            pq.push({a[i] + b[0], i, 0});

        while (k-- && !pq.empty()) {
            auto [sum, i, j] = pq.top();
            pq.pop();

            ans.push_back({a[i], b[j]});

            if (j + 1 < b.size())
                pq.push({a[i] + b[j + 1], i, j + 1});
        }

        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find k pairs (u, v) with smallest sum where u belongs to nums1 and v belongs to nums2.
// - Approach: Min-Heap Frontier Expansion (similar to Dijkstra / multi-way merge).
// - Intuition:
//   * Array elements are sorted. Pairs (i, 0) represent the initial frontier.
//   * When (i, j) is extracted, the next smallest candidate in row i is (i, j + 1).
// - Complexity: Time: O(k log k), Space: O(k).

// =========================================================
// 93. NEXT GREATER ELEMENT I (LC 496)
// =========================================================

class Solution93 {
  public:
    vector<int> nextGreaterElement(vector<int> &nums1, vector<int> &nums2) {
        unordered_map<int, int> mp;
        stack<int> st;

        for (int x : nums2) {
            while (!st.empty() && st.top() < x) {
                mp[st.top()] = x;
                st.pop();
            }
            st.push(x);
        }

        vector<int> ans;
        ans.reserve(nums1.size());
        for (int x : nums1) {
            ans.push_back(mp.count(x) ? mp[x] : -1);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: For each element in nums1, find next greater element in nums2.
// - Approach: Monotonic Decreasing Stack + Hash Map Lookup.
// - Intuition:
//   * Traverse nums2. When an element x > stack top is seen, x is the immediate next greater element for all popped stack items.
// - Complexity: Time: O(N + M), Space: O(M).
