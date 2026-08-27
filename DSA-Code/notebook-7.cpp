#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:

    // =========================================================
    // 1. DFS TRAVERSALS
    // =========================================================

    // Inorder: Left -> Root -> Right
    void inorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        inorder(root->left, ans);
        ans.push_back(root->val);
        inorder(root->right, ans);
    }
    // Interview Explanation:
    // - Problem Statement: Perform Inorder traversal of a binary tree (Left -> Root -> Right).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Recursively visit left child, record current node value, then visit right child; produces ascending sorted order in a BST.
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion stack where H is tree height.

    // Preorder: Root -> Left -> Right
    void preorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        ans.push_back(root->val);
        preorder(root->left, ans);
        preorder(root->right, ans);
    }
    // Interview Explanation:
    // - Problem Statement: Perform Preorder traversal of a binary tree (Root -> Left -> Right).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Process current root first before descending into left and right subtrees; useful for duplicating trees and prefix expressions.
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion call stack.

    // Postorder: Left -> Right -> Root
    void postorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        postorder(root->left, ans);
        postorder(root->right, ans);
        ans.push_back(root->val);
    }
    // Interview Explanation:
    // - Problem Statement: Perform Postorder traversal of a binary tree (Left -> Right -> Root).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Process both children fully before visiting the root; essential for bottom-up computations (e.g., node deletion, tree height).
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion call stack.


    // =========================================================
    // 2. LEVEL ORDER / BFS
    // =========================================================

    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            while (size--) {
                TreeNode* node = q.front(); q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            ans.push_back(level);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the level order (breadth-first) traversal of nodes' values layer by layer.
    // - Approach: Breadth-First Search (BFS) using a Queue.
    // - Intuition: Snapshot `size = q.size()` at each level to process all nodes of the current depth before enqueuing children for the next depth.
    // - Complexity: Time: O(N) visiting each node once, Space: O(W) queue space where W is the maximum width of the tree (up to N/2).


    // =========================================================
    // 3. MAXIMUM DEPTH / HEIGHT
    // =========================================================

    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
    // Interview Explanation:
    // - Problem Statement: Find the maximum depth (number of nodes along longest root-to-leaf path) of a binary tree.
    // - Approach: Divide and Conquer / Bottom-up DFS.
    // - Intuition: Depth of current node is `1 + max(maxDepth(left), maxDepth(right))`; empty node base case returns 0.
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion stack space.


    // =========================================================
    // 4. SAME TREE
    // =========================================================

    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) return true;
        if (!p || !q) return false;
        return p->val == q->val && isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
    // Interview Explanation:
    // - Problem Statement: Check if two binary trees are structurally identical with identical node values.
    // - Approach: Simultaneous recursive DFS traversal.
    // - Intuition: Base case: both null is true, one null is false. Current values must match, and both left and right subtrees must recursively match.
    // - Complexity: Time: O(N) where N is minimum number of nodes in either tree, Space: O(H) recursion stack space.


    // =========================================================
    // 5. SYMMETRIC TREE
    // =========================================================

    bool mirror(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->val == b->val && mirror(a->left, b->right) && mirror(a->right, b->left);
    }

    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return mirror(root->left, root->right);
    }
    // Interview Explanation:
    // - Problem Statement: Determine whether a binary tree is a mirror image of itself (symmetric around its center).
    // - Approach: Two-tree mirror comparison using recursive DFS.
    // - Intuition: Subtrees `a` and `b` are mirrors if `a->val == b->val`, `mirror(a->left, b->right)`, and `mirror(a->right, b->left)`.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion call stack.


    // =========================================================
    // 6. DIAMETER OF BINARY TREE
    // =========================================================

    int diameter = 0;

    int height(TreeNode* root) {
        if (!root) return 0;
        int left = height(root->left), right = height(root->right);
        diameter = max(diameter, left + right);
        return 1 + max(left, right);
    }

    int diameterOfBinaryTree(TreeNode* root) {
        diameter = 0;
        height(root);
        return diameter;
    }
    // Interview Explanation:
    // - Problem Statement: Find the length of the longest path between any two nodes in a binary tree.
    // - Approach: Post-order DFS computing heights while updating global maximum diameter.
    // - Intuition: At each node, longest path through it is `left_height + right_height`; return `1 + max(left, right)` to parent.
    // - Complexity: Time: O(N) single bottom-up pass, Space: O(H) recursion stack space.


    // =========================================================
    // 7. BALANCED BINARY TREE
    // =========================================================

    int checkHeight(TreeNode* root) {
        if (!root) return 0;
        int left = checkHeight(root->left);
        if (left == -1) return -1;
        int right = checkHeight(root->right);
        if (right == -1) return -1;
        if (abs(left - right) > 1) return -1;
        return 1 + max(left, right);
    }

    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if a binary tree is height-balanced (subtree depths differ by at most 1 at every node).
    // - Approach: Bottom-up DFS with early termination sentinel (-1).
    // - Intuition: Return height if balanced; if any subtree has `|left - right| > 1` or returns -1, propagate -1 immediately to prevent redundant work.
    // - Complexity: Time: O(N) single bottom-up traversal (avoids O(N^2) top-down depth checks), Space: O(H) recursion stack.


    // =========================================================
    // 8. MAXIMUM PATH SUM
    // =========================================================

    int maxPath = INT_MIN;

    int maxGain(TreeNode* root) {
        if (!root) return 0;
        int left = max(0, maxGain(root->left)), right = max(0, maxGain(root->right));
        maxPath = max(maxPath, root->val + left + right);
        return root->val + max(left, right);
    }

    int maxPathSum(TreeNode* root) {
        maxPath = INT_MIN;
        maxGain(root);
        return maxPath;
    }
    // Interview Explanation:
    // - Problem Statement: Find the maximum path sum between any two nodes in a non-empty binary tree.
    // - Approach: Post-order DFS tracking maximum gain from subtrees.
    // - Intuition: Clamp negative gains to 0 (`max(0, gain)`); update global answer with `root->val + left + right`, but return `root->val + max(left, right)` to parent.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion call stack.


    // =========================================================
    // 9. ROOT TO NODE PATH
    // =========================================================

    bool findPath(TreeNode* root, int target, vector<int>& path) {
        if (!root) return false;
        path.push_back(root->val);
        if (root->val == target || findPath(root->left, target, path) || findPath(root->right, target, path)) return true;
        path.pop_back();
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Find the path of node values from the root to a target node in a binary tree.
    // - Approach: Backtracking Depth-First Search.
    // - Intuition: Push current node to path; return true if target is found or if found in left/right child; otherwise pop node (backtrack) and return false.
    // - Complexity: Time: O(N) searching tree nodes, Space: O(H) recursion stack and path array storage.


    // =========================================================
    // 10. LOWEST COMMON ANCESTOR - BINARY TREE
    // =========================================================

    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
    }
    // Interview Explanation:
    // - Problem Statement: Find the lowest common ancestor (LCA) of two given nodes p and q in a general binary tree.
    // - Approach: Post-order recursive DFS.
    // - Intuition: If root is null, p, or q, return root; if both left and right return non-null, root is LCA; otherwise propagate non-null child.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion call stack.


    // =========================================================
    // 11. LEFT VIEW
    // =========================================================

    vector<int> leftView(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                if (i == 0) ans.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the values of nodes visible when looking at the binary tree from the left side.
    // - Approach: Level Order Traversal (BFS) using a Queue.
    // - Intuition: For each level of width `size`, the first element encountered (`i == 0`) is the leftmost node visible from the outside.
    // - Complexity: Time: O(N) visiting each node once, Space: O(W) where W is maximum width of tree.


    // =========================================================
    // 12. RIGHT VIEW
    // =========================================================

    vector<int> rightView(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                if (i == size - 1) ans.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the values of nodes visible when looking at the binary tree from the right side.
    // - Approach: Level Order Traversal (BFS) using a Queue.
    // - Intuition: For each level of width `size`, the last element encountered (`i == size - 1`) is the rightmost node visible from the outside.
    // - Complexity: Time: O(N) visiting each node once, Space: O(W) where W is maximum width of tree.


    // =========================================================
    // 13. VERTICAL ORDER TRAVERSAL
    // =========================================================

    vector<vector<int>> verticalOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        map<int, vector<int>> mp;
        queue<pair<TreeNode*, int>> q;
        q.push({root, 0});

        while (!q.empty()) {
            auto [node, col] = q.front(); q.pop();
            mp[col].push_back(node->val);
            if (node->left) q.push({node->left, col - 1});
            if (node->right) q.push({node->right, col + 1});
        }
        for (auto& [col, nodes] : mp) ans.push_back(nodes);
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the vertical column order traversal of a binary tree from leftmost to rightmost column.
    // - Approach: BFS with coordinate tracking (`col`) + ordered map (`std::map<int, vector<int>>`).
    // - Intuition: Root is at col 0; left child is `col - 1`, right child is `col + 1`; BFS guarantees top-to-bottom ordering within each column.
    // - Complexity: Time: O(N \log C) where C is number of columns, Space: O(N) for queue and column map.


    // =========================================================
    // 14. ZIGZAG LEVEL ORDER
    // =========================================================

    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;

        while (!q.empty()) {
            int size = q.size();
            vector<int> level(size);
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                int index = leftToRight ? i : size - 1 - i;
                level[index] = node->val;
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            ans.push_back(level);
            leftToRight = !leftToRight;
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the zigzag level order traversal (alternating left-to-right and right-to-left across levels).
    // - Approach: BFS Level Order with boolean flag and direct index placement.
    // - Intuition: Allocate level vector of size `size`; place node at index `i` if left-to-right, else at `size - 1 - i`; flip flag after each level.
    // - Complexity: Time: O(N) visiting each node once, Space: O(W) queue and level vector storage.


    // =========================================================
    // 15. BINARY TREE -> INVERT / MIRROR
    // =========================================================

    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        swap(root->left, root->right);
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
    // Interview Explanation:
    // - Problem Statement: Invert a binary tree in-place such that left and right subtrees are swapped at all nodes.
    // - Approach: Preorder / Postorder recursive DFS.
    // - Intuition: Swap `root->left` and `root->right` pointers directly, then recursively invert the left and right subtrees.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack space.


    // =========================================================
    // 16. BST SEARCH
    // =========================================================

    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root || root->val == val) return root;
        return val < root->val ? searchBST(root->left, val) : searchBST(root->right, val);
    }
    // Interview Explanation:
    // - Problem Statement: Search for a node with a given value in a Binary Search Tree (BST).
    // - Approach: Binary search tree traversal exploiting BST property.
    // - Intuition: If `val < root->val`, target must lie in left subtree; if greater, in right subtree; else current node is target.
    // - Complexity: Time: O(H) where H is tree height (O(log N) balanced, O(N) skewed), Space: O(H) recursion stack (or O(1) iterative).


    // =========================================================
    // 17. BST INSERT
    // =========================================================

    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);
        if (val < root->val) root->left = insertIntoBST(root->left, val);
        else root->right = insertIntoBST(root->right, val);
        return root;
    }
    // Interview Explanation:
    // - Problem Statement: Insert a value into a Binary Search Tree while preserving the BST invariant.
    // - Approach: Recursive BST traversal down to null position.
    // - Intuition: If `val < root->val`, recursively insert into left subtree (`root->left = ...`); else right; create new `TreeNode(val)` at empty leaf.
    // - Complexity: Time: O(H) where H is tree height, Space: O(H) recursion stack space.


    // =========================================================
    // 18. VALIDATE BST
    // =========================================================

    bool validate(TreeNode* root, long long low, long long high) {
        if (!root) return true;
        if (root->val <= low || root->val >= high) return false;
        return validate(root->left, low, root->val) && validate(root->right, root->val, high);
    }

    bool isValidBST(TreeNode* root) {
        return validate(root, LLONG_MIN, LLONG_MAX);
    }
    // Interview Explanation:
    // - Problem Statement: Determine whether a given binary tree is a valid Binary Search Tree (BST).
    // - Approach: DFS range validation tracking allowable minimum and maximum bounds (`(low, high)`).
    // - Intuition: Invariant: every node in left subtree must be `< root->val` and right subtree `> root->val`; use `long long` bounds to avoid overflow.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack space.


    // =========================================================
    // 19. KTH SMALLEST IN BST
    // =========================================================

    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        while (true) {
            while (root) { st.push(root); root = root->left; }
            root = st.top(); st.pop();
            if (--k == 0) return root->val;
            root = root->right;
        }
    }
    // Interview Explanation:
    // - Problem Statement: Find the k-th smallest element in a Binary Search Tree (1-indexed).
    // - Approach: Iterative Inorder Traversal using an explicit Stack.
    // - Intuition: Inorder traversal of a BST yields values in strictly ascending order; decrement `k` on each pop and return when `k == 0`.
    // - Complexity: Time: O(H + K) stopping as soon as k-th element is reached, Space: O(H) stack space.


    // =========================================================
    // 20. LCA IN BST
    // =========================================================

    TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (root) {
            if (p->val < root->val && q->val < root->val) root = root->left;
            else if (p->val > root->val && q->val > root->val) root = root->right;
            else return root;
        }
        return nullptr;
    }
    // Interview Explanation:
    // - Problem Statement: Find the lowest common ancestor (LCA) of two nodes p and q in a Binary Search Tree.
    // - Approach: Iterative BST directional navigation.
    // - Intuition: If both p and q are smaller than root, LCA is in left subtree; if both are larger, in right subtree; first split node is LCA.
    // - Complexity: Time: O(H) single path descent, Space: O(1) auxiliary space (iterative).


    // =========================================================
    // 21. BUILD TREE FROM PREORDER + INORDER
    // =========================================================

    unordered_map<int, int> inorderIndex;
    int preIndex = 0;

    TreeNode* build(vector<int>& preorder, int left, int right) {
        if (left > right) return nullptr;
        int value = preorder[preIndex++];
        TreeNode* root = new TreeNode(value);
        int mid = inorderIndex[value];
        root->left = build(preorder, left, mid - 1);
        root->right = build(preorder, mid + 1, right);
        return root;
    }

    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderIndex.clear();
        preIndex = 0;
        for (int i = 0; i < inorder.size(); i++) inorderIndex[inorder[i]] = i;
        return build(preorder, 0, inorder.size() - 1);
    }
    // Interview Explanation:
    // - Problem Statement: Construct a binary tree from its preorder and inorder traversal arrays.
    // - Approach: Divide and Conquer with Hash Map index lookup for inorder positions.
    // - Intuition: Next element in preorder is root; finding its index in inorder splits left and right subtrees for recursive sub-construction.
    // - Complexity: Time: O(N) with O(1) hash map lookup, Space: O(N) for hash map and recursion stack.


    // =========================================================
    // 22. COUNT NODES
    // =========================================================

    int countNodes(TreeNode* root) {
        if (!root) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
    // Interview Explanation:
    // - Problem Statement: Count the total number of nodes in a binary tree.
    // - Approach: Divide and Conquer recursive DFS (`1 + count(left) + count(right)`).
    // - Intuition: Total nodes in any subtree is 1 (for current node) plus total nodes in left subtree and right subtree; base case 0 for null.
    // - Complexity: Time: O(N) visiting every node, Space: O(H) recursion stack (can be O(log^2 N) for complete binary trees).


    // =========================================================
    // 23. SUM OF ROOT-TO-LEAF NUMBERS
    // =========================================================

    int rootToLeafSum(TreeNode* root, int current) {
        if (!root) return 0;
        current = current * 10 + root->val;
        if (!root->left && !root->right) return current;
        return rootToLeafSum(root->left, current) + rootToLeafSum(root->right, current);
    }

    int sumNumbers(TreeNode* root) {
        return rootToLeafSum(root, 0);
    }
    // Interview Explanation:
    // - Problem Statement: Calculate the total sum of all numbers formed by root-to-leaf paths where each node represents a digit.
    // - Approach: Preorder DFS carrying accumulated path value.
    // - Intuition: Update path value as `current * 10 + root->val`; when leaf is reached return `current`; otherwise sum recursive left and right results.
    // - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack space.


    // =========================================================
    // 24. PATH SUM
    // =========================================================

    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) return false;
        if (!root->left && !root->right) return targetSum == root->val;
        return hasPathSum(root->left, targetSum - root->val) || hasPathSum(root->right, targetSum - root->val);
    }
    // Interview Explanation:
    // - Problem Statement: Determine if the tree has a root-to-leaf path such that adding all values along the path equals targetSum.
    // - Approach: Recursive DFS with target reduction.
    // - Intuition: At leaf node, check if `targetSum == root->val`; for internal nodes, recursively check left or right with `targetSum - root->val`.
    // - Complexity: Time: O(N) in worst case visiting all nodes, Space: O(H) recursion stack space.


    // =========================================================
    // 25. SERIALIZE / DESERIALIZE
    // =========================================================

    void serializeDFS(TreeNode* root, string& s) {
        if (!root) { s += "#,"; return; }
        s += to_string(root->val) + ",";
        serializeDFS(root->left, s);
        serializeDFS(root->right, s);
    } // it's just a preorder traversal with null markers

    string serialize(TreeNode* root) {
        string s;
        serializeDFS(root, s);
        return s;
    }

    TreeNode* deserializeDFS(stringstream& ss) {
        string value;
        getline(ss, value, ',');
        if (value == "#") return nullptr;
        TreeNode* root = new TreeNode(stoi(value));
        root->left = deserializeDFS(ss);
        root->right = deserializeDFS(ss);
        return root;
    } // it's just a preorder traversal with null markers

    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeDFS(ss);
    }
    // Interview Explanation:
    // - Problem Statement: Design an algorithm to serialize a binary tree to a string and deserialize that string back to the original tree.
    // - Approach: Preorder DFS with delimiter (',') and null markers ('#') using `stringstream`.
    // - Intuition: Preorder traversal uniquely determines tree structure when null pointers are explicitly serialized; deserializer consumes token-by-token.
    // - Complexity: Time: O(N) for both serialize and deserialize, Space: O(N) for string representation and recursion stack.
};

/*
 ====================================================================================================
                    ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: TREES & BST (EXPANDED)
 ====================================================================================================

 1. PROBLEM IDENTIFICATION MATRIX (EXTENDED)
    | Pattern / Clue in Problem                   | Technique / Data Structure        | Template / Core Idea                                    |
    |:--------------------------------------------|:----------------------------------|:--------------------------------------------------------|
    | Level-by-level, views, width, shortest      | BFS with Queue                    | snapshot size = q.size(), iterate size--                |
    | Path sum, root-to-leaf, validation          | Preorder / Top-down DFS           | pass state down (e.g. sum - root->val, range)           |
    | Tree height, diameter, max path, balanced   | Postorder / Bottom-up DFS         | return values to parent, update global max              |
    | K-th smallest/largest, sorted order         | Inorder DFS (Iterative stack)     | left-root-right stack; K pops = K-th element            |
    | Reconstruct tree                            | Divide & Conquer + Hash Map       | Preorder root -> split Inorder via index map            |
    | Ancestor queries (LCA in general tree)      | Postorder DFS                     | if root==p||q return root; both!=null -> root           |
    | Ancestor queries (LCA in BST)               | Directional while loop            | p,q < root -> left; p,q > root -> right; else root      |
    | Ancestor queries (LCA, repeated, static)    | Binary Lifting                    | up[node][k] = up[up[node][k-1]][k-1]                    |
    | Serialize / Deserialize                     | Preorder DFS + '#' null sentinel  | comma-separated tokens via stringstream                 |
    | Tree with Parent Pointers / Node to Node    | Convert Tree to Graph (Hash Map)  | BFS from start node with visited set                    |
    | Vertical order / column traversal           | BFS + (col, row) map              | col-- for left, col++ for right, group by col           |
    | Boundary / left-right view                  | DFS with (depth, level) tracking  | first node at depth = left view, last = right           |
    | Flatten to linked list                      | Preorder Morris-style / recursion | rewire right pointer, set left = null                   |
    | Lowest cost to connect / build              | Huffman-style greedy + min-heap   | pop 2 smallest, merge, push back                        |
    | Tree DP (max independent set, rob houses)   | Postorder returning a pair        | {include, exclude} per subtree                          |
    | Rerooting (answer differs per root)         | Two-pass DFS (down then up)       | 1st pass: subtree info, 2nd: parent contribution        |
    | Trie-style prefix tree problems             | Trie (array[26] / hashmap kids)   | not binary tree — insert/search per character           |
    | Segment tree / range queries                | Segment Tree / BIT                | build O(N), query/update O(log N)                       |
    | N-ary tree traversal                        | DFS/BFS over children vector      | for (auto* child : node->children)                      |

 2. TOP-DOWN VS. BOTTOM-UP DFS (THE #1 INTERVIEW DECISION)
    • Top-Down (Preorder): Pass information from parent to children via parameters.
      - Use when: Problem asks about root-to-node path, prefix sums, depth from root, or valid range (e.g. Validate BST).
      - Signature pattern: void dfs(TreeNode* node, int currState, ...)
      - Nothing is returned — answer is recorded into a shared/global structure during traversal.
    • Bottom-Up (Postorder): Return aggregated values from children to parent.
      - Use when: Problem asks about subtree properties, tree height, diameter, maximum path sum, balance, or deleting nodes.
      - Signature pattern: ReturnType dfs(TreeNode* node) -> compute left, right, update ans, return to parent.
    • Rule of thumb: If the answer at a node needs info from BELOW it -> bottom-up. If the answer at a node needs info from ABOVE it (ancestors) -> top-down. Complex problems (rerooting, diameter with path reconstruction) combine both passes.

 3. BST SPECIFIC GOLDEN RULES
    • Inorder traversal of ANY valid BST is strictly increasing (strictly sorted, no duplicates unless explicitly allowed).
    • Validate BST: NEVER compare only node->left->val < node->val. MUST pass (low, high) range down to all descendants — a right-left grandchild can violate an ancestor's bound even if it satisfies its immediate parent.
    • Searching / Inserting in BST is O(H): if val < root->val go left; if val > root->val go right.
    • Predecessor: Rightmost node of left subtree (or nearest ancestor for which node is in the right subtree, if no left subtree exists).
    • Successor: Leftmost node of right subtree (or nearest ancestor for which node is in the left subtree, if no right subtree exists).
    • Deleting a node with two children: Replace value with inorder successor (or predecessor), then recursively delete that successor from the right subtree.
    • Balanced BST guarantee (AVL / Red-Black) applies only if explicitly stated — otherwise assume worst-case skewed input (H = N).

 4. LIVE INTERVIEW EXECUTION PROTOCOL (WHAT TO SAY & DO)
    • Step 1 — Clarify edge cases: Empty root nullptr? Single node? Negative values? Duplicate keys? Guaranteed valid tree (acyclic), or can input be malformed?
    • Step 2 — State traversal choice out loud: "I will use bottom-up postorder DFS because the parent needs aggregated values from subtrees."
    • Step 3 — Identify Return Value vs Global Tracker: e.g., in Diameter / MaxPathSum, recursive step returns single branch gain upward to parent, while global tracker evaluates combined path (left + right at the same node).
    • Step 4 — Write base cases first: `if (!root) return ...;` — decide what empty returns before recursive logic (0 for height, INT_MIN for max path, true for validity, etc.).
    • Step 5 — State complexity clearly: "Time is O(N) since each node is visited once; auxiliary space is O(H) on recursion stack, where H = log N for balanced trees and O(N) worst-case for skewed trees."
    • Step 6 — Narrate the recursive contract before coding: "dfs(node) returns X, and assumes dfs(node->left) and dfs(node->right) already correctly return X for their subtrees."
    • Step 7 — Watch for silent global-state bugs: Initialize global/member trackers before the initial DFS call, not inside the base case.
    • Step 8 — Dry-run on a 3-4 node skewed tree AND a small balanced tree — skewed trees expose recursion-order and stack-depth bugs that balanced trees hide.

 5. COMPLEXITY & PITFALL QUICK-REFERENCE
    • DFS/BFS on a tree: Always O(N) time — every node visited once; no visited set required because trees are acyclic by definition.
    • Recursion space is O(H), not O(N) — state "H, which degrades to N on a skewed tree" to show mastery of both average balanced and worst-case scenarios.
    • Off-by-one in range validation: Using <= / >= vs < / > at BST boundaries silently allows duplicates — verify with interviewer whether duplicates are permitted.
    • Return value vs Global tracker confusion: Diameter and max path sum evaluate both children locally, but return only the single best branch upward.
    • Integer overflow in Validate BST: Node values can equal INT_MIN / INT_MAX — always use `long long` for lower and upper bounds.
    • Iterative traversals: In iterative preorder stack traversal, push right child BEFORE left child (LIFO stack reverses order).
    • Morris Traversal: O(1) space traversal with no stack or recursion — prime follow-up answer when asked to optimize auxiliary space.
 ====================================================================================================
*/