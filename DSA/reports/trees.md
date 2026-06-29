# Trees: Master Interview & OA Guide

## How to Use This Guide

Tree problems follow highly repeatable patterns. The workflow is:

1. Identify the tree type: binary tree, BST, general/N-ary tree, or weighted tree.
2. Identify what is being asked: path, subtree, ancestor, distance, ordering, construction.
3. Map it to one of the 15 categories below.
4. Apply the template for that category.

The pattern recognition table at the end gives you a 30-second lookup for any problem.

---

## Universal Problem-Solving Framework

Before writing code, answer these four questions:

**Q1: What type of tree?**
- Binary tree (each node has at most 2 children) -> general DFS/BFS templates
- BST (left < root < right) -> inorder gives sorted order; use BST property to prune
- General/N-ary tree (variable children) -> same DFS/BFS but iterate over children list
- Weighted tree (edges have costs) -> store weights on edges or nodes

**Q2: What direction does information flow?**
- Bottom-up: compute subtree info, pass result up to parent -> post-order DFS
- Top-down: pass parent info down to children -> pre-order DFS
- Both directions: rerooting DP -> two DFS passes

**Q3: Are you asked about a path, a subtree, or a node?**
- Root-to-leaf path -> DFS with running state (sum, count, etc.)
- Any path (may pass through any node) -> post-order DFS, update global answer at each node
- Subtree -> Euler tour or post-order aggregation
- Single node property -> BFS level order or simple DFS

**Q4: Is recursion the right tool?**
Almost always yes for trees. The recursive structure mirrors the tree structure.
Use iterative BFS (level order) only when you need to process level by level.

---

## 1. Tree Traversals

### When to Use
Visiting every node in a specific order; basis for almost all tree algorithms.

### Four Traversals

```cpp
class Solution {
private:
    void inorderHelper(TreeNode* root) {
        if (!root) return;
        inorderHelper(root->left);
        visit(root);
        inorderHelper(root->right);
    }
    void preorderHelper(TreeNode* root) {
        if (!root) return;
        visit(root);
        preorderHelper(root->left);
        preorderHelper(root->right);
    }
    void postorderHelper(TreeNode* root) {
        if (!root) return;
        postorderHelper(root->left);
        postorderHelper(root->right);
        visit(root);
    }

public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) return {};
        vector<vector<int>> result;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            vector<int> level;
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(level);
        }
        return result;
    }
};
```

### Iterative Inorder (common in interviews)

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> s;
        TreeNode* curr = root;
        while (curr || !s.empty()) {
            while (curr) {
                s.push(curr);
                curr = curr->left;
            }
            curr = s.top();
            s.pop();
            result.push_back(curr->val);
            curr = curr->right;
        }
        return result;
    }
};
```

### Key Problems

| Problem | Traversal | Trick |
|---|---|---|
| 94. Inorder Traversal | Inorder | Iterative version is the common follow-up |
| 102. Level Order | BFS | Snapshot queue length at start of each level |
| 103. Zigzag Level Order | BFS | Alternate append direction using a flag |
| 107. Level Order II | BFS | Same as 102, reverse the result |

---

## 2. Tree DFS / BFS

### When to Use
Computing properties of the entire tree: depth, symmetry, shape matching.

### DFS Template (returns value bottom-up)

```cpp
class Solution {
public:
    int dfs(TreeNode* node) {
        if (!node) return base_value;
        int left = dfs(node->left);
        int right = dfs(node->right);
        return combine(left, right, node->val);
    }
};
```

### Key Problems

| Problem | Return Value | Recurrence |
|---|---|---|
| 104. Max Depth | int (depth) | 1 + max(left, right) |
| 111. Min Depth | int (depth) | Handle leaf vs internal node carefully |
| 100. Same Tree | bool | node1.val == node2.val and same(left) and same(right) |
| 101. Symmetric Tree | bool | Mirror check: left.left vs right.right, left.right vs right.left |
| 199. Right Side View | list | BFS; take last element of each level |

### Common Mistake: Min Depth
Min depth is NOT simply `1 + min(left, right)` because that would return 1 for a node with only one child (treating None as depth 0). A non-leaf node with one None child should not count that side.

```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        if (!root->left) return 1 + minDepth(root->right);
        if (!root->right) return 1 + minDepth(root->left);
        return 1 + min(minDepth(root->left), minDepth(root->right));
    }
};
```

---

## 3. Binary Search Tree (BST)

### When to Use
Any problem that mentions "BST", sorted order, search, insert, delete, or kth smallest/largest.

### Core Property
Inorder traversal of a BST gives elements in sorted ascending order. This is the key insight for almost every BST problem.

### Validate BST

```cpp
class Solution {
private:
    bool helper(TreeNode* root, long long lo, long long hi) {
        if (!root) return true;
        if (root->val <= lo || root->val >= hi) return false;
        return helper(root->left, lo, root->val) && helper(root->right, root->val, hi);
    }

public:
    bool isValidBST(TreeNode* root) {
        return helper(root, LLONG_MIN, LLONG_MAX);
    }
};
```

### Kth Smallest (inorder walk, stop early)

```cpp
class Solution {
private:
    void inorder(TreeNode* node, int k, int& count, int& result) {
        if (!node || result != -1) return;
        inorder(node->left, k, count, result);
        count++;
        if (count == k) {
            result = node->val;
            return;
        }
        inorder(node->right, k, count, result);
    }

public:
    int kthSmallest(TreeNode* root, int k) {
        int count = 0;
        int result = -1;
        inorder(root, k, count, result);
        return result;
    }
};
```

### Key Problems

| Problem | Key Insight |
|---|---|
| 98. Validate BST | Pass min/max bounds down; do not just compare with parent |
| 230. Kth Smallest | Inorder traversal; stop at kth visit |
| 450. Delete Node in BST | Three cases: no child, one child, two children (replace with inorder successor) |
| 235. LCA of BST | Both nodes less than root -> go left; both greater -> go right; else root is LCA |

---

## 4. Lowest Common Ancestor (LCA)

### When to Use
"Find the lowest (deepest) node that has both p and q as descendants."

### Binary Tree LCA (no BST property)

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
    }
};
```

### BST LCA (faster, uses BST property)

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (root) {
            if (p->val < root->val && q->val < root->val) {
                root = root->left;
            } else if (p->val > root->val && q->val > root->val) {
                root = root->right;
            } else {
                return root;
            }
        }
        return nullptr;
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 236. LCA Binary Tree | Post-order DFS; if both sides return non-null, current node is LCA |
| 235. LCA BST | Walk using BST property; no recursion needed |
| 1644. LCA II (node may not exist) | Track found flags; only return node when both flags are set |

---

## 5. Binary Lifting (Kth Ancestor / LCA in General Trees)

### When to Use
Efficiently answer "what is the kth ancestor of node v?" or LCA queries on general trees with many queries.

### Preprocessing: O(n log n) build, O(log n) per query

```cpp
class Solution {
public:
    vector<int> depth;
    vector<vector<int>> parent;
    int LOG;

    void buildBinaryLifting(int n, vector<vector<int>>& adj, int root = 0) {
        LOG = 32 - __builtin_clz(n);
        depth.assign(n, 0);
        parent.assign(LOG, vector<int>(n, -1));

        vector<bool> visited(n, false);
        visited[root] = true;
        queue<int> q;
        q.push(root);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    depth[v] = depth[u] + 1;
                    parent[0][v] = u;
                    q.push(v);
                }
            }
        }

        for (int k = 1; k < LOG; k++) {
            for (int v = 0; v < n; v++) {
                if (parent[k-1][v] != -1) {
                    parent[k][v] = parent[k-1][parent[k-1][v]];
                }
            }
        }
    }

    int kthAncestor(int v, int k) {
        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1) {
                v = parent[i][v];
                if (v == -1) return -1;
            }
        }
        return v;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) {
                u = parent[i][u];
            }
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (parent[i][u] != parent[i][v]) {
                u = parent[i][u];
                v = parent[i][v];
            }
        }
        return parent[0][u];
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 1483. Kth Ancestor | Direct application; build table then answer queries |
| CSES Company Queries I | Kth ancestor per query |
| CSES Company Queries II | LCA per query using binary lifting |
| CSES Distance Queries | dist(u, v) = depth[u] + depth[v] - 2 * depth[lca(u,v)] |

---

## 6. Tree Diameter

### When to Use
"Longest path between any two nodes in the tree."

### Two-Pass BFS/DFS (works on general trees)

```cpp
class Solution {
private:
    pair<int, int> bfs(int start, int n, vector<vector<int>>& adj) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        int farthest = start;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                    if (dist[v] > dist[farthest]) {
                        farthest = v;
                    }
                }
            }
        }

        return {farthest, dist[farthest]};
    }

public:
    int treeDiameter(int n, vector<vector<int>>& adj) {
        auto [u, _] = bfs(0, n, adj);
        auto [v, diameter] = bfs(u, n, adj);
        return diameter;
    }
};
```

### Single-Pass DFS (binary tree, updates global answer)

```cpp
class Solution {
private:
    int depth(TreeNode* node, int& ans) {
        if (!node) return 0;
        int left = depth(node->left, ans);
        int right = depth(node->right, ans);
        ans = max(ans, left + right);
        return 1 + max(left, right);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        int ans = 0;
        depth(root, ans);
        return ans;
    }
};
```

### Common Mistake
The diameter does not have to pass through the root. Always update a global answer inside the DFS rather than returning it.

---

## 7. Tree DP

### When to Use
Optimize a value over the tree where the answer at each node depends on its subtree.

### Template: Return tuple of states

```cpp
class Solution {
public:
    struct State {
        // compute_state data members
    };

    State dfs(TreeNode* node) {
        if (!node) {
            return { /* state_if_null values */ };
        }
        State left = dfs(node->left);
        State right = dfs(node->right);
        return combine(left, right, node->val);
    }
};
```

### Key Problems

| Problem | States Returned | Trick |
|---|---|---|
| 337. House Robber III | (rob_node, skip_node) | rob = skip_left + skip_right + val; skip = max(rob_l, skip_l) + max(rob_r, skip_r) |
| 124. Max Path Sum | max_path_through_node | Update global with left + right + val; return max(left, right) + val to parent |
| 968. Binary Tree Cameras | 0=uncovered, 1=has camera, 2=covered | Place camera at parent of uncovered leaf; greedy via post-order |
| 1372. Longest ZigZag | (left_len, right_len) | At each node, zigzag going right = left_len + 1; zigzag going left = right_len + 1 |

### House Robber III Full Solution

```cpp
class Solution {
private:
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0}; // {rob_this, skip_this}
        auto [left_rob, left_skip] = dfs(node->left);
        auto [right_rob, right_skip] = dfs(node->right);
        int rob_curr = node->val + left_skip + right_skip;
        int skip_curr = max(left_rob, left_skip) + max(right_rob, right_skip);
        return {rob_curr, skip_curr};
    }

public:
    int rob(TreeNode* root) {
        auto [rob_root, skip_root] = dfs(root);
        return max(rob_root, skip_root);
    }
};
```

---

## 8. Rerooting DP

### When to Use
You need the answer for every node as if it were the root. Computing it naively for each root is O(n^2); rerooting does it in O(n).

### Two-Pass Pattern

```cpp
class Solution {
private:
    void dfs1(int u, int p, vector<vector<int>>& adj, vector<int>& down, vector<int>& size) {
        for (int v : adj[u]) {
            if (v != p) {
                dfs1(v, u, adj, down, size);
                size[u] += size[v];
                down[u] += down[v] + size[v];
            }
        }
    }

    void dfs2(int u, int p, int n, vector<vector<int>>& adj, vector<int>& down, vector<int>& size, vector<int>& ans) {
        ans[u] = down[u];
        for (int v : adj[u]) {
            if (v != p) {
                int old_down_u = down[u];
                down[v] += (down[u] - down[v] - size[v]) + (n - size[v]);
                size[v] = n;
                dfs2(v, u, n, adj, down, size, ans);
                down[u] = old_down_u;
                // size[v] restoration
            }
        }
    }

public:
    vector<int> rerooting(int n, vector<vector<int>>& adj) {
        vector<int> down(n, 0);
        vector<int> size(n, 1);
        vector<int> ans(n, 0);
        dfs1(0, -1, adj, down, size);
        dfs2(0, -1, n, adj, down, size, ans);
        return ans;
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 834. Sum of Distances in Tree | Pass 1: sum of distances in subtree. Pass 2: add distances from above |
| CSES Tree Distances II | Same pattern |

---

## 9. Tree Distances

### When to Use
Distance between two nodes, distance from every node to all others, K-distance queries.

### Distance Between Two Nodes via LCA

```cpp
class Solution {
public:
    int distance(int u, int v, vector<int>& depth, function<int(int, int)> lca_func) {
        int ancestor = lca_func(u, v);
        return depth[u] + depth[v] - 2 * depth[ancestor];
    }
};
```

### All Nodes Distance K (863)
Convert tree to undirected graph, BFS from target node for K steps.

```cpp
class Solution {
private:
    void build(TreeNode* node, TreeNode* parent, unordered_map<int, vector<int>>& adj) {
        if (parent) {
            adj[node->val].push_back(parent->val);
            adj[parent->val].push_back(node->val);
        }
        if (node->left) build(node->left, node, adj);
        if (node->right) build(node->right, node, adj);
    }

public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        unordered_map<int, vector<int>> adj;
        build(root, nullptr, adj);

        unordered_set<int> visited = {target->val};
        queue<pair<int, int>> q;
        q.push({target->val, 0});
        vector<int> result;

        while (!q.empty()) {
            auto [node, dist] = q.front();
            q.pop();

            if (dist == k) {
                result.push_back(node);
            }
            for (int neighbor : adj[node]) {
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    q.push({neighbor, dist + 1});
                }
            }
        }

        return result;
    }
};
```

---

## 10. Subtree DP / Euler Tour

### When to Use
Subtree queries (sum of values in subtree, count of nodes), subtree updates, offline LCA.

### Euler Tour (Flatten subtree to array range)

```cpp
class Solution {
private:
    void dfs(int u, int parent, vector<vector<int>>& adj, vector<int>& entry, vector<int>& exit_pos, vector<int>& order, int& timer) {
        entry[u] = timer;
        order.push_back(u);
        timer++;
        for (int v : adj[u]) {
            if (v != parent) {
                dfs(v, u, adj, entry, exit_pos, order, timer);
            }
        }
        exit_pos[u] = timer - 1;
    }

public:
    tuple<vector<int>, vector<int>, vector<int>> eulerTour(vector<vector<int>>& adj, int n) {
        vector<int> entry(n, 0);
        vector<int> exit_pos(n, 0);
        vector<int> order;
        int timer = 0;
        dfs(0, -1, adj, entry, exit_pos, order, timer);
        return {entry, exit_pos, order};
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 572. Subtree of Another Tree | Serialize both trees (preorder string); check if one is substring of other |
| 333. Largest BST Subtree | Return (is_bst, size, min_val, max_val) from each node |
| CSES Subtree Queries | Euler tour + BIT for point updates and subtree sum queries |

---

## 11. Path Problems

### When to Use
Root-to-leaf paths, path sums, counting paths with a target sum.

### Root-to-Leaf Path Sum

```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int target) {
        if (!root) return false;
        if (!root->left && !root->right) return root->val == target;
        return hasPathSum(root->left, target - root->val) || hasPathSum(root->right, target - root->val);
    }
};
```

### Path Sum III (any path, not just root-to-leaf)
Use prefix sums: at each node, how many ancestors have prefix sum = current_sum - target?

```cpp
class Solution {
private:
    void dfs(TreeNode* node, long long curr_sum, int target, unordered_map<long long, int>& prefix, int& count) {
        if (!node) return;
        curr_sum += node->val;
        if (prefix.count(curr_sum - target)) {
            count += prefix[curr_sum - target];
        }
        prefix[curr_sum]++;
        dfs(node->left, curr_sum, target, prefix, count);
        dfs(node->right, curr_sum, target, prefix, count);
        prefix[curr_sum]--; // backtrack
    }

public:
    int pathSum(TreeNode* root, int targetSum) {
        unordered_map<long long, int> prefix;
        prefix[0] = 1;
        int count = 0;
        dfs(root, 0, targetSum, prefix, count);
        return count;
    }
};
```

### Key Problems

| Problem | Technique |
|---|---|
| 112. Path Sum | DFS, subtract from target going down |
| 113. Path Sum II | DFS + backtracking to collect all paths |
| 437. Path Sum III | Prefix sum hash map with backtracking |
| 129. Sum Root to Leaf Numbers | DFS, carry running number (curr * 10 + node.val) |

---

## 12. Serialization / Tree Construction

### When to Use
Reconstruct a tree from traversal arrays, encode/decode a tree.

### Construct from Preorder + Inorder

```cpp
class Solution {
private:
    TreeNode* build(vector<int>& preorder, int preStart, int preEnd, vector<int>& inorder, int inStart, int inEnd, unordered_map<int, int>& inMap) {
        if (preStart > preEnd || inStart > inEnd) return nullptr;
        int rootVal = preorder[preStart];
        TreeNode* root = new TreeNode(rootVal);
        int mid = inMap[rootVal];
        int numLeft = mid - inStart;

        root->left = build(preorder, preStart + 1, preStart + numLeft, inorder, inStart, mid - 1, inMap);
        root->right = build(preorder, preStart + numLeft + 1, preEnd, inorder, mid + 1, inEnd, inMap);
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        unordered_map<int, int> inMap;
        for (int i = 0; i < inorder.size(); i++) {
            inMap[inorder[i]] = i;
        }
        return build(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1, inMap);
    }
};
```

### Serialize / Deserialize

```cpp
class Codec {
private:
    void serializeHelper(TreeNode* root, ostringstream& out) {
        if (!root) {
            out << "N,";
            return;
        }
        out << root->val << ",";
        serializeHelper(root->left, out);
        serializeHelper(root->right, out);
    }

    TreeNode* deserializeHelper(istringstream& in) {
        string val;
        getline(in, val, ',');
        if (val == "N") {
            return nullptr;
        }
        TreeNode* node = new TreeNode(stoi(val));
        node->left = deserializeHelper(in);
        node->right = deserializeHelper(in);
        return node;
    }

public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        ostringstream out;
        serializeHelper(root, out);
        return out.str();
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserializeHelper(in);
    }
};
```

### Key Problems

| Problem | Trick |
|---|---|
| 105. Construct from Preorder+Inorder | Preorder root splits inorder into left/right subtrees |
| 106. Construct from Inorder+Postorder | Last element of postorder is always root |
| 297. Serialize/Deserialize | Preorder with null markers; reconstruct with iterator |

---

## 13. Segment Tree on Trees (HLD Intro)

### When to Use
Path update/query on trees: "update all values on path u to v", "sum of values on path u to v".
Heavy-Light Decomposition (HLD) decomposes the tree into chains so each chain maps to a contiguous array range, enabling segment tree operations.

### HLD Concept

```
1. Compute subtree sizes.
2. At each node, the child with the largest subtree is the "heavy child" -> heavy edge.
3. Following heavy edges from any node forms a "heavy chain".
4. Assign positions in a linear array by traversing heavy chains first (preorder).
5. Any root-to-node path uses O(log n) chains -> O(log^2 n) per path query with segment tree.
```

### Key CSES Problems
- Path Queries: sum of node values on root-to-node path (can be solved with Euler tour + BIT)
- Path Queries II: arbitrary path u to v (requires HLD)
- Fixed-Length Paths I & II: count paths of exactly length k (centroid decomposition)

---

## 14. N-ary / General Trees

### When to Use
Tree where each node has an arbitrary number of children.

### Template

```cpp
class Node {
public:
    int val;
    vector<Node*> children;
    Node() {}
    Node(int _val) {
        val = _val;
    }
    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};

class Solution {
private:
    void preorderHelper(Node* root, vector<int>& result) {
        if (!root) return;
        result.push_back(root->val);
        for (Node* child : root->children) {
            preorderHelper(child, result);
        }
    }

public:
    vector<int> preorder(Node* root) {
        vector<int> result;
        preorderHelper(root, result);
        return result;
    }

    vector<vector<int>> levelOrder(Node* root) {
        if (!root) return {};
        vector<vector<int>> result;
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            vector<int> level;
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                Node* node = q.front();
                q.pop();
                level.push_back(node->val);
                for (Node* child : node->children) {
                    q.push(child);
                }
            }
            result.push_back(level);
        }
        return result;
    }
};
```

---

## 15. Advanced Tree Algorithms

### Centroid Decomposition
Repeatedly find the centroid of the tree (removing it splits tree into parts of size <= n/2), solve subproblems, recurse. Used for counting paths of length k, paths with certain properties.

```cpp
class Solution {
private:
    void getSize(int u, int p, vector<vector<int>>& adj, vector<bool>& removed, vector<int>& size) {
        size[u] = 1;
        for (int v : adj[u]) {
            if (v != p && !removed[v]) {
                getSize(v, u, adj, removed, size);
                size[u] += size[v];
            }
        }
    }

    int getCentroid(int u, int p, int tree_size, vector<vector<int>>& adj, vector<bool>& removed, vector<int>& size) {
        for (int v : adj[u]) {
            if (v != p && !removed[v]) {
                if (size[v] > tree_size / 2) {
                    return getCentroid(v, u, tree_size, adj, removed, size);
                }
            }
        }
        return u;
    }

    void solveCentroid(int u, vector<vector<int>>& adj, vector<bool>& removed, vector<int>& size) {
        getSize(u, -1, adj, removed, size);
        int c = getCentroid(u, -1, size[u], adj, removed, size);
        removed[c] = true;

        // Process all paths through centroid c here

        for (int v : adj[c]) {
            if (!removed[v]) {
                solveCentroid(v, adj, removed, size);
            }
        }
    }

public:
    void centroidDecomp(int n, vector<vector<int>>& adj) {
        vector<int> size(n, 1);
        vector<bool> removed(n, false);
        solveCentroid(0, adj, removed, size);
    }
};
```

### Heavy-Light Decomposition
Decomposes tree into chains. Supports O(log^2 n) path queries with a segment tree.

---

## Pattern Recognition Quick-Reference

| Signal in Problem | Category | Algorithm |
|---|---|---|
| Visit every node in order | Traversals | Inorder / Preorder / Postorder |
| Process level by level | Traversals | BFS / Level Order |
| Depth, symmetry, shape | DFS / BFS | Post-order DFS |
| BST, sorted order, kth element | BST | Inorder traversal |
| BST insert / delete / validate | BST | Recursive with bounds |
| Lowest common ancestor | LCA | Post-order DFS (binary tree) or BST walk |
| Kth ancestor, many LCA queries | Binary Lifting | Sparse table preprocessing |
| Longest path in tree | Diameter | Double BFS or single-pass DFS with global max |
| Choose nodes optimally (rob, camera) | Tree DP | Post-order DFS returning state tuple |
| Answer for every node as root | Rerooting DP | Two DFS passes |
| Distance between two nodes | Distances / LCA | depth[u] + depth[v] - 2*depth[lca] |
| All nodes at distance K | Distances | Convert to graph, BFS from target |
| Sum over subtree, subtree updates | Euler Tour | Flatten subtree to array range |
| Path sum (root to leaf) | Path Problems | DFS subtracting from target |
| Path sum (any path) | Path Problems | Prefix sum hash map with backtracking |
| Reconstruct tree from traversals | Construction | Split at root, recurse on left/right |
| Serialize / deserialize tree | Construction | Preorder with null markers |
| Path query on arbitrary u-v path | HLD | Heavy-Light Decomposition + Segment Tree |
| Count paths of length k | Centroid Decomp | Centroid Decomposition |

---

## Complexity Cheat Sheet

| Algorithm | Time | Space |
|---|---|---|
| DFS / BFS traversal | O(n) | O(h) for DFS, O(w) for BFS (h=height, w=max width) |
| Binary Lifting (build) | O(n log n) | O(n log n) |
| Binary Lifting (query) | O(log n) | - |
| LCA (naive DFS) | O(n) per query | O(h) |
| LCA (binary lifting) | O(log n) per query | O(n log n) preprocessing |
| Tree Diameter (double BFS) | O(n) | O(n) |
| Tree DP | O(n) | O(h) |
| Rerooting DP | O(n) | O(n) |
| Euler Tour | O(n) build | O(n) |
| Euler Tour + Segment Tree | O(n log n) | O(n) |
| HLD + Segment Tree | O(log^2 n) per query | O(n log n) |
| Centroid Decomposition | O(n log n) | O(n log n) |

---

## Problem List by Category

### 1. Tree Traversals
**LeetCode:** 94, 144, 145, 102, 103, 107
**CSES:** Tree Traversals

### 2. Tree DFS / BFS
**LeetCode:** 104, 111, 100, 101, 199
**CSES:** Subordinates

### 3. BST
**LeetCode:** 98, 700, 701, 450, 230, 235

### 4. LCA
**LeetCode:** 236, 235, 1644
**CSES:** Company Queries II, Distance Queries

### 5. Binary Lifting
**LeetCode:** 1483
**CSES:** Company Queries I, Company Queries II, Distance Queries

### 6. Tree Diameter
**LeetCode:** 543, 1245
**CSES:** Tree Diameter

### 7. Tree DP
**LeetCode:** 337, 124, 968, 1372
**CSES:** Tree Matching

### 8. Rerooting DP
**LeetCode:** 834
**CSES:** Tree Distances II

### 9. Tree Distances
**LeetCode:** 863, 834
**CSES:** Tree Distances I, Tree Distances II, Distance Queries

### 10. Subtree DP / Euler Tour
**LeetCode:** 333, 572
**CSES:** Subtree Queries

### 11. Path Problems
**LeetCode:** 112, 113, 437, 257, 129

### 12. Serialization / Construction
**LeetCode:** 105, 106, 297, 889
**CSES:** Tree Traversals

### 13. HLD
**CSES:** Path Queries, Path Queries II

### 14. N-ary Trees
**LeetCode:** 589, 590, 429

### 15. Advanced (Centroid Decomp / HLD)
**CSES:** Fixed-Length Paths I & II, Path Queries II

---

## Priority by Importance

### Must-Know (cover 90% of interview questions)
- DFS / BFS
- Tree Traversals (all four, including iterative inorder)
- BST operations
- LCA
- Binary Lifting
- Tree Diameter

### High Value (cover the rest of standard interviews)
- Tree DP
- Rerooting DP
- Euler Tour
- Subtree Queries
- Distance Queries

### Competitive Programming / Hard OAs
- HLD
- Centroid Decomposition
- Virtual Tree

---

## Top 14 CSES Tree Problems (Priority Order)

1. Subordinates
2. Tree Traversals
3. Tree Diameter
4. Tree Distances I
5. Tree Distances II
6. Tree Matching
7. Company Queries I
8. Company Queries II
9. Distance Queries
10. Subtree Queries
11. Path Queries
12. Path Queries II
13. Fixed-Length Paths I
14. Fixed-Length Paths II