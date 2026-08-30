#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
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


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct NodeWithParent {
    int val;
    NodeWithParent* left;
    NodeWithParent* right;
    NodeWithParent* parent;
    NodeWithParent(int x = 0, NodeWithParent* l = nullptr, NodeWithParent* r = nullptr, NodeWithParent* p = nullptr)
        : val(x), left(l), right(r), parent(p) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | DFS Traversals (In/Pre/Post)                | Recursive & Iterative Stack Traver| O(N)     | O(H)     |
 | 2  | Level Order Traversal (BFS)                 | Queue-Based Level-by-Level Scan   | O(N)     | O(W)     |
 | 3  | Maximum Depth / Height of Tree              | Bottom-Up Post-Order DFS          | O(N)     | O(H)     |
 | 4  | Same Tree Verification                      | Dual Simultaneous Pre-Order DFS   | O(N)     | O(H)     |
 | 5  | Symmetric Tree (Mirror Symmetry)            | Mirror Branch Pair DFS Traversal  | O(N)     | O(H)     |
 | 6  | Diameter of Binary Tree                     | Bottom-Up Post-Order Max Diameter | O(N)     | O(H)     |
 | 7  | Balanced Binary Tree Check                  | Height-Balanced Bottom-Up Check   | O(N)     | O(H)     |
 | 8  | Maximum Path Sum                            | Post-Order DFS Branch Gain Tracker| O(N)     | O(H)     |
 | 9  | Root to Node Path                           | Backtracking Pre-Order Path Vector| O(N)     | O(H)     |
 | 10 | Lowest Common Ancestor (Binary Tree)        | Bottom-Up Node Value Bubbling DFS | O(N)     | O(H)     |
 | 11 | Left View of Binary Tree                    | Level First-Seen Pre-Order DFS    | O(N)     | O(H)     |
 | 12 | Right View of Binary Tree                   | Level First-Seen Reverse Pre-Order| O(N)     | O(H)     |
 | 13 | Vertical Order Traversal                    | BFS Queue with (Col, Row) TreeMap | O(N logN)| O(N)     |
 | 14 | Zigzag Level Order Traversal                | BFS Queue + Directional Inversion | O(N)     | O(W)     |
 | 15 | Invert / Mirror Binary Tree                 | Recursive Left-Right Subtree Swap | O(N)     | O(H)     |
 | 16 | Search in BST                               | Directional Key Branch Traversal  | O(H)     | O(1)     |
 | 17 | Insert into BST                             | Directional Iterative Insertion   | O(H)     | O(1)     |
 | 18 | Validate BST                                | Top-Down (Low, High) Range Check  | O(N)     | O(H)     |
 | 19 | K-th Smallest Element in BST                | Inorder Traversal Stack Counting  | O(H + K) | O(H)     |
 | 20 | Lowest Common Ancestor in BST               | Directional Range Branching       | O(H)     | O(1)     |
 | 21 | Build Tree from Preorder & Inorder          | Divide & Conquer + Inorder Hash   | O(N)     | O(N)     |
 | 22 | Count Complete Tree Nodes                   | Binary Search on Heights (2^H)    | O(log^2N)| O(log N) |
 | 23 | Sum of Root-to-Leaf Numbers                 | Top-Down Base-10 Accumulation DFS | O(N)     | O(H)     |
 | 24 | Path Sum (I, II & Tree Paths)               | Backtracking Target Path DFS      | O(N)     | O(H)     |
 | 25 | Serialize & Deserialize Binary Tree         | Preorder DFS + String Stream Token| O(N)     | O(N)     |
 | 26 | Morris Inorder Traversal                    | Threaded Binary Tree (O(1) Space) | O(N)     | O(1)     |
 | 27 | Morris Preorder Traversal                   | Threaded Binary Tree (O(1) Space) | O(N)     | O(1)     |
 | 28 | Top View of Binary Tree                     | BFS + Column First-Seen Hash Map  | O(N)     | O(N)     |
 | 29 | Bottom View of Binary Tree                  | BFS + Column Last-Seen Hash Map   | O(N)     | O(N)     |
 | 30 | All 3 Traversals in One Pass                | Single Stack with State Counter   | O(N)     | O(N)     |
 | 31 | Boundary Traversal of Binary Tree           | Left Boundary + Leaves + Right Rev| O(N)     | O(H)     |
 | 32 | Maximum Width of Binary Tree                | BFS with 0-Indexed Level Offsets  | O(N)     | O(W)     |
 | 33 | Children Sum Property Binary Tree           | Top-Down Increment + Bottom-Up Sum| O(N)     | O(H)     |
 | 34 | Build Tree from Postorder & Inorder         | Divide & Conquer + Inorder Hash   | O(N)     | O(N)     |
 | 35 | Flatten Binary Tree to Linked List          | Reverse Post-Order Traversal Rewire| O(N)    | O(H)     |
 | 36 | Populating Next Right Pointers              | Level-Order Pointer Rewiring      | O(N)     | O(1)     |
 | 37 | Inorder Successor & Predecessor in BST      | Directional Key Comparison Search | O(H)     | O(1)     |
 | 38 | Floor & Ceil in BST                         | Binary Search Candidate Tracking  | O(H)     | O(1)     |
 | 39 | Two Sum in BST (Pair with Sum K)            | Dual BST Iterators (Next/Prev)    | O(N)     | O(H)     |
 | 40 | BST Iterator (O(1) Amortized)               | Controlled Inorder Stack Push     | O(1) avg | O(H)     |
 | 41 | Largest BST in Binary Tree                  | Post-Order Node Info Quadruple    | O(N)     | O(H)     |
 | 42 | Minimum Time to Burn Binary Tree            | Parent Map + Multi-Source BFS     | O(N)     | O(N)     |
 | 43 | Delete a Node in BST                        | Inorder Successor / Child Splice  | O(H)     | O(H)     |
 | 44 | Convert Sorted Array to BST (LC 108)        | Midpoint Divide & Conquer Recursion| O(N)    | O(log N) |
 | 45 | Construct BST from Preorder (LC 1008)       | Upper-Bound Constrained DFS       | O(N)     | O(H)     |
 | 46 | Binary Tree to Doubly Linked List           | Inorder DFS Pointer Stitching     | O(N)     | O(H)     |
 | 47 | Subtree of Another Tree (LC 572)            | DFS Traversal + Same Tree Match   | O(N * M) | O(H)     |
 ====================================================================================================
*/


class Solution {
public:

    // =========================================================
    // 1. DFS TRAVERSALS
    // =========================================================

    // Inorder: Left -> Root -> Right (Recursive)
    void inorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        inorder(root->left, ans);
        ans.push_back(root->val);
        inorder(root->right, ans);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;
        inorder(root, ans);
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Perform Inorder traversal of a binary tree (Left -> Root -> Right).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Recursively visit left child, record current node value, then visit right child; produces ascending sorted order in a BST.
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion stack where H is tree height.

    // Preorder: Root -> Left -> Right (Recursive)
    void preorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        ans.push_back(root->val);
        preorder(root->left, ans);
        preorder(root->right, ans);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> ans;
        preorder(root, ans);
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Perform Preorder traversal of a binary tree (Root -> Left -> Right).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Process current root first before descending into left and right subtrees; useful for duplicating trees and prefix expressions.
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion call stack.

    // Postorder: Left -> Right -> Root (Recursive)
    void postorder(TreeNode* root, vector<int>& ans) {
        if (!root) return;
        postorder(root->left, ans);
        postorder(root->right, ans);
        ans.push_back(root->val);
    }

    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> ans;
        postorder(root, ans);
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Perform Postorder traversal of a binary tree (Left -> Right -> Root).
    // - Approach: Recursive Depth-First Search (DFS).
    // - Intuition: Process both children fully before visiting the root; essential for bottom-up computations (e.g., node deletion, tree height).
    // - Complexity: Time: O(N) visiting every node once, Space: O(H) recursion call stack.

    // Iterative Inorder Traversal using 1 Stack (LeetCode 94)
    vector<int> inorderIterative(TreeNode* root) {
        vector<int> ans;
        stack<TreeNode*> st;
        TreeNode* curr = root;

        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            ans.push_back(curr->val);
            curr = curr->right;
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Iterative Inorder traversal of a binary tree without recursion.
    // - Approach: Explicit Stack simulating call stack.
    // - Intuition: Drill down to leftmost leaf pushing nodes to stack. Pop, record value, and transition to right child.
    // - Complexity: Time: O(N) visiting each node, Space: O(H) stack space.

    // Iterative Preorder Traversal using 1 Stack (LeetCode 144)
    vector<int> preorderIterative(TreeNode* root) {
        if (!root) return {};
        vector<int> ans;
        stack<TreeNode*> st;
        st.push(root);

        while (!st.empty()) {
            TreeNode* curr = st.top();
            st.pop();
            ans.push_back(curr->val);

            if (curr->right) st.push(curr->right); // push right first so left is popped first
            if (curr->left) st.push(curr->left);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Iterative Preorder traversal of a binary tree without recursion.
    // - Approach: Explicit Stack pushing right child before left child.
    // - Intuition: LIFO stack pops top element (root), records it, and pushes right then left child so left is processed next.
    // - Complexity: Time: O(N), Space: O(H) stack space.

    // Iterative Postorder Traversal using 2 Stacks (LeetCode 145)
    vector<int> postorderIterative2Stacks(TreeNode* root) {
        if (!root) return {};
        vector<int> ans;
        stack<TreeNode*> s1, s2;
        s1.push(root);

        while (!s1.empty()) {
            TreeNode* curr = s1.top();
            s1.pop();
            s2.push(curr);

            if (curr->left) s1.push(curr->left);
            if (curr->right) s1.push(curr->right);
        }

        while (!s2.empty()) {
            ans.push_back(s2.top()->val);
            s2.pop();
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Iterative Postorder traversal using two stacks.
    // - Approach: Reverse modified preorder `(Root -> Right -> Left)` into second stack.
    // - Intuition: Pushing `Root -> Left -> Right` onto stack 1 and transferring popped nodes to stack 2 reverses order to `Left -> Right -> Root`.
    // - Complexity: Time: O(N), Space: O(N) for two stacks.


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

    NodeWithParent* lowestCommonAncestorWithParent(NodeWithParent* p, NodeWithParent* q) {
        NodeWithParent *a = p, *b = q;
        while (a != b) {
            a = a->parent ? a->parent : q;
            b = b->parent ? b->parent : p;
        }
        return a;
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
        for (int i = 0; i < (int)inorder.size(); i++) inorderIndex[inorder[i]] = i;
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
    // 24. PATH SUM (I, II & BINARY TREE PATHS)
    // =========================================================

    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) return false;
        if (!root->left && !root->right) return targetSum == root->val;
        return hasPathSum(root->left, targetSum - root->val) || hasPathSum(root->right, targetSum - root->val);
    }

    void dfsPathSumII(TreeNode* node, int targetSum, vector<int>& currPath, vector<vector<int>>& res) {
        if (!node) return;
        currPath.push_back(node->val);
        if (!node->left && !node->right && targetSum == node->val) {
            res.push_back(currPath);
        } else {
            dfsPathSumII(node->left, targetSum - node->val, currPath, res);
            dfsPathSumII(node->right, targetSum - node->val, currPath, res);
        }
        currPath.pop_back();
    }

    vector<vector<int>> pathSumII(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        vector<int> currPath;
        dfsPathSumII(root, targetSum, currPath, res);
        return res;
    }

    void dfsTreePaths(TreeNode* node, string path, vector<string>& res) {
        if (!node) return;
        path += to_string(node->val);
        if (!node->left && !node->right) {
            res.push_back(path);
            return;
        }
        path += "->";
        if (node->left) dfsTreePaths(node->left, path, res);
        if (node->right) dfsTreePaths(node->right, path, res);
    }

    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> res;
        if (root) dfsTreePaths(root, "", res);
        return res;
    }
    // Interview Explanation:
    // - Problem Statement: Path Sum I (boolean existence), Path Sum II (all matching paths), and Binary Tree Paths (all root-to-leaf string representations).
    // - Approach: Backtracking DFS with path accumulation and target reduction.
    // - Intuition: Push current node, recurse children; on reaching leaf with remaining target == 0, record path; backtrack on return.
    // - Complexity: Time: O(N), Space: O(H) recursion stack and path buffer.


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

    // =========================================================
    // 26. MORRIS INORDER TRAVERSAL
    // =========================================================

    vector<int> morrisInorder(TreeNode* root) {
        vector<int> inorder;
        TreeNode* curr = root;
        while (curr) {
            if (!curr->left) {
                inorder.push_back(curr->val);
                curr = curr->right;
            } else {
                TreeNode* prev = curr->left;
                while (prev->right && prev->right != curr) {
                    prev = prev->right;
                }
                if (!prev->right) {
                    prev->right = curr; // make temporary thread
                    curr = curr->left;
                } else {
                    prev->right = nullptr; // remove temporary thread
                    inorder.push_back(curr->val);
                    curr = curr->right;
                }
            }
        }
        return inorder;
    }
    // Interview Explanation:
    // - Problem Statement: Perform Inorder traversal of a binary tree in O(1) auxiliary space without recursion or stack.
    // - Approach: Morris Traversal using Threaded Binary Tree pointers.
    // - Intuition: Link rightmost node of left subtree to current root; visiting root when returning via thread before restoring nullptr restores original tree structure.
    // - Complexity: Time: O(N) amortized (each edge traversed at most 3 times), Space: O(1) auxiliary space.


    // =========================================================
    // 27. MORRIS PREORDER TRAVERSAL
    // =========================================================

    vector<int> morrisPreorder(TreeNode* root) {
        vector<int> preorder;
        TreeNode* curr = root;
        while (curr) {
            if (!curr->left) {
                preorder.push_back(curr->val);
                curr = curr->right;
            } else {
                TreeNode* prev = curr->left;
                while (prev->right && prev->right != curr) {
                    prev = prev->right;
                }
                if (!prev->right) {
                    preorder.push_back(curr->val); // record on first visit
                    prev->right = curr;            // establish thread
                    curr = curr->left;
                } else {
                    prev->right = nullptr;         // remove thread
                    curr = curr->right;
                }
            }
        }
        return preorder;
    }
    // Interview Explanation:
    // - Problem Statement: Perform Preorder traversal of a binary tree in O(1) auxiliary space without recursion or stack.
    // - Approach: Morris Preorder Traversal with Threaded Binary Tree links.
    // - Intuition: Record current node value immediately upon establishing thread before descending to left subtree; remove thread upon returning.
    // - Complexity: Time: O(N) amortized, Space: O(1) auxiliary space.


    // =========================================================
    // 28. TOP VIEW OF BINARY TREE
    // =========================================================

    vector<int> topView(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        map<int, int> topNodeMap; // hd -> node->val
        queue<pair<TreeNode*, int>> q; // {node, hd}
        q.push({root, 0});

        while (!q.empty()) {
            auto [node, hd] = q.front();
            q.pop();
            if (topNodeMap.find(hd) == topNodeMap.end()) {
                topNodeMap[hd] = node->val;
            }
            if (node->left) q.push({node->left, hd - 1});
            if (node->right) q.push({node->right, hd + 1});
        }

        for (auto& [hd, val] : topNodeMap) {
            ans.push_back(val);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the values of nodes visible when looking at the tree from top to bottom.
    // - Approach: BFS Level Order Traversal with Horizontal Distance (HD) mapping.
    // - Intuition: Root is HD=0; left child HD-1, right child HD+1. Level-order ensures the first node encountered at each HD is the topmost node.
    // - Complexity: Time: O(N log N) with ordered map (or O(N) with min/max HD tracking), Space: O(N) for queue and map.


    // =========================================================
    // 29. BOTTOM VIEW OF BINARY TREE
    // =========================================================

    vector<int> bottomView(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        map<int, int> bottomNodeMap; // hd -> node->val
        queue<pair<TreeNode*, int>> q; // {node, hd}
        q.push({root, 0});

        while (!q.empty()) {
            auto [node, hd] = q.front();
            q.pop();
            bottomNodeMap[hd] = node->val; // overwrite with lowest level node at horizontal distance
            if (node->left) q.push({node->left, hd - 1});
            if (node->right) q.push({node->right, hd + 1});
        }

        for (auto& [hd, val] : bottomNodeMap) {
            ans.push_back(val);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Return the values of nodes visible when looking at the tree from bottom to top.
    // - Approach: BFS Level Order Traversal continuously overwriting Horizontal Distance (HD) entries.
    // - Intuition: Overwriting map entry at each HD ensures the node processed in the deepest level remains as the bottom-most visible value.
    // - Complexity: Time: O(N log N), Space: O(N) for queue and coordinate map.


    // =========================================================
    // 30. PREORDER, INORDER, AND POSTORDER IN ONE TRAVERSAL
    // =========================================================

    void allThreeTraversals(TreeNode* root, vector<int>& pre, vector<int>& in, vector<int>& post) {
        if (!root) return;
        stack<pair<TreeNode*, int>> st; // {node, visitState: 1=pre, 2=in, 3=post}
        st.push({root, 1});

        while (!st.empty()) {
            auto& [node, state] = st.top();
            if (state == 1) {
                pre.push_back(node->val);
                state++;
                if (node->left) st.push({node->left, 1});
            } else if (state == 2) {
                in.push_back(node->val);
                state++;
                if (node->right) st.push({node->right, 1});
            } else {
                post.push_back(node->val);
                st.pop();
            }
        }
    }
    // Interview Explanation:
    // - Problem Statement: Compute Preorder, Inorder, and Postorder traversals of a binary tree in a single pass.
    // - Approach: Stack-based state machine tracking visit counts per node (1, 2, 3).
    // - Intuition: State 1 records Preorder and descends Left; State 2 records Inorder and descends Right; State 3 records Postorder and pops.
    // - Complexity: Time: O(N) each node visited 3 times, Space: O(H) stack frames.


    // =========================================================
    // 31. BOUNDARY TRAVERSAL OF BINARY TREE
    // =========================================================

    bool isLeaf(TreeNode* node) {
        return !node->left && !node->right;
    }

    void addLeftBoundary(TreeNode* root, vector<int>& res) {
        TreeNode* curr = root->left;
        while (curr) {
            if (!isLeaf(curr)) res.push_back(curr->val);
            if (curr->left) curr = curr->left;
            else curr = curr->right;
        }
    }

    void addLeaves(TreeNode* root, vector<int>& res) {
        if (isLeaf(root)) {
            res.push_back(root->val);
            return;
        }
        if (root->left) addLeaves(root->left, res);
        if (root->right) addLeaves(root->right, res);
    }

    void addRightBoundary(TreeNode* root, vector<int>& res) {
        TreeNode* curr = root->right;
        vector<int> tmp;
        while (curr) {
            if (!isLeaf(curr)) tmp.push_back(curr->val);
            if (curr->right) curr = curr->right;
            else curr = curr->left;
        }
        for (int i = (int)tmp.size() - 1; i >= 0; --i) {
            res.push_back(tmp[i]);
        }
    }

    vector<int> boundaryTraversal(TreeNode* root) {
        vector<int> res;
        if (!root) return res;
        if (!isLeaf(root)) res.push_back(root->val);
        addLeftBoundary(root, res);
        addLeaves(root, res);
        addRightBoundary(root, res);
        return res;
    }
    // Interview Explanation:
    // - Problem Statement: Return boundary nodes of a binary tree in anti-clockwise direction starting from root.
    // - Approach: Decomposition into Root -> Left Boundary -> All Leaf Nodes -> Reversed Right Boundary.
    // - Intuition: Exclude leaves during boundary walks to avoid double counting; reverse right boundary to ensure anti-clockwise order.
    // - Complexity: Time: O(N) visiting each node at most twice, Space: O(H) auxiliary recursion/buffer space.


    // =========================================================
    // 32. MAXIMUM WIDTH OF BINARY TREE
    // =========================================================

    int widthOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        long long maxWidth = 0;
        queue<pair<TreeNode*, unsigned long long>> q; // {node, index}
        q.push({root, 0});

        while (!q.empty()) {
            int size = q.size();
            unsigned long long minIdx = q.front().second;
            unsigned long long first = 0, last = 0;

            for (int i = 0; i < size; ++i) {
                unsigned long long currIdx = q.front().second - minIdx; // normalize to prevent 64-bit overflow
                TreeNode* node = q.front().first;
                q.pop();

                if (i == 0) first = currIdx;
                if (i == size - 1) last = currIdx;

                if (node->left) q.push({node->left, 2 * currIdx + 1});
                if (node->right) q.push({node->right, 2 * currIdx + 2});
            }
            maxWidth = max(maxWidth, (long long)(last - first + 1));
        }
        return (int)maxWidth;
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum width among all levels where width is defined as distance between leftmost and rightmost non-null nodes.
    // - Approach: BFS Level Order Traversal with normalized binary heap indexing.
    // - Intuition: Level indexing gives left child = 2*i + 1, right child = 2*i + 2. Subtracting first index of each level resets base and prevents integer overflow on skewed deep trees.
    // - Complexity: Time: O(N) one pass BFS, Space: O(W) queue space.


    // =========================================================
    // 33. CHILDREN SUM PROPERTY IN BINARY TREE
    // =========================================================

    void reorderChildrenSum(TreeNode* root) {
        if (!root) return;
        int childSum = 0;
        if (root->left) childSum += root->left->val;
        if (root->right) childSum += root->right->val;

        if (childSum >= root->val) {
            root->val = childSum;
        } else {
            if (root->left) root->left->val = root->val;
            if (root->right) root->right->val = root->val;
        }

        reorderChildrenSum(root->left);
        reorderChildrenSum(root->right);

        int total = 0;
        if (root->left) total += root->left->val;
        if (root->right) total += root->right->val;
        if (root->left || root->right) root->val = total;
    }
    // Interview Explanation:
    // - Problem Statement: Modify binary tree node values such that every node equals the sum of its left and right children values.
    // - Approach: Top-down value propagation + Bottom-up sum consolidation DFS.
    // - Intuition: Increase children to parent's value on descent to guarantee sufficient child values; on backtrack, set parent = sum(children).
    // - Complexity: Time: O(N) single postorder traversal, Space: O(H) recursion stack.


    // =========================================================
    // 34. CONSTRUCT BINARY TREE FROM POSTORDER AND INORDER TRAVERSAL
    // =========================================================

    TreeNode* buildTreePostInHelper(const vector<int>& inorder, int inStart, int inEnd,
                                    const vector<int>& postorder, int postStart, int postEnd,
                                    unordered_map<int, int>& inMap) {
        if (inStart > inEnd || postStart > postEnd) return nullptr;

        TreeNode* root = new TreeNode(postorder[postEnd]);
        int inRoot = inMap[root->val];
        int numsLeft = inRoot - inStart;

        root->left = buildTreePostInHelper(inorder, inStart, inRoot - 1,
                                           postorder, postStart, postStart + numsLeft - 1, inMap);
        root->right = buildTreePostInHelper(inorder, inRoot + 1, inEnd,
                                            postorder, postStart + numsLeft, postEnd - 1, inMap);
        return root;
    }

    TreeNode* buildTreePostIn(vector<int>& inorder, vector<int>& postorder) {
        unordered_map<int, int> inMap;
        for (int i = 0; i < (int)inorder.size(); ++i) inMap[inorder[i]] = i;
        return buildTreePostInHelper(inorder, 0, (int)inorder.size() - 1,
                                     postorder, 0, (int)postorder.size() - 1, inMap);
    }
    // Interview Explanation:
    // - Problem Statement: Reconstruct unique binary tree from Inorder and Postorder traversal arrays.
    // - Approach: Divide & Conquer with Hash Map index lookup.
    // - Intuition: Last element of Postorder is root; hash map splits Inorder into left and right subtree segments.
    // - Complexity: Time: O(N) with O(1) hash map partition lookups, Space: O(N) map and recursion stack.


    // =========================================================
    // 35. FLATTEN BINARY TREE TO LINKED LIST
    // =========================================================

    void flatten(TreeNode* root) {
        TreeNode* curr = root;
        while (curr) {
            if (curr->left) {
                TreeNode* prev = curr->left;
                while (prev->right) prev = prev->right;
                prev->right = curr->right;
                curr->right = curr->left;
                curr->left = nullptr;
            }
            curr = curr->right;
        }
    }
    // Interview Explanation:
    // - Problem Statement: Flatten binary tree into linked list in-place according to Preorder traversal.
    // - Approach: Morris In-Place Threading (O(1) Space).
    // - Intuition: Attach current root's right subtree to the rightmost node of its left subtree, then shift left subtree to right.
    // - Complexity: Time: O(N), Space: O(1) auxiliary space.


    // =========================================================
    // 36. POPULATING NEXT RIGHT POINTERS IN EACH NODE
    // =========================================================

    struct NodeWithNext {
        int val;
        NodeWithNext *left, *right, *next;
        NodeWithNext(int x) : val(x), left(nullptr), right(nullptr), next(nullptr) {}
    };

    NodeWithNext* connect(NodeWithNext* root) {
        if (!root) return nullptr;
        NodeWithNext* leftmost = root;
        while (leftmost->left) {
            NodeWithNext* curr = leftmost;
            while (curr) {
                curr->left->next = curr->right;
                if (curr->next) curr->right->next = curr->next->left;
                curr = curr->next;
            }
            leftmost = leftmost->left;
        }
        return root;
    }
    // Interview Explanation:
    // - Problem Statement: Populate each next pointer to point to its next right node in a perfect binary tree.
    // - Approach: O(1) Space Level Traversals using already established  pointers of parent level.
    // - Intuition: Connect siblings directly () and cousins across parents ().
    // - Complexity: Time: O(N) visiting each node once, Space: O(1) auxiliary space.


    // =========================================================
    // 37. INORDER SUCCESSOR AND PREDECESSOR IN BST
    // =========================================================

    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        TreeNode* successor = nullptr;
        while (root) {
            if (p->val >= root->val) {
                root = root->right;
            } else {
                successor = root;
                root = root->left;
            }
        }
        return successor;
    }

    TreeNode* inorderPredecessor(TreeNode* root, TreeNode* p) {
        TreeNode* predecessor = nullptr;
        while (root) {
            if (p->val <= root->val) {
                root = root->left;
            } else {
                predecessor = root;
                root = root->right;
            }
        }
        return predecessor;
    }
    // Interview Explanation:
    // - Problem Statement: Find inorder successor and predecessor of a given node in a BST in O(H) time without parent pointers.
    // - Approach: Binary Search Tree Property Navigation.
    // - Intuition: Successor is smallest node greater than p (descend left and record candidate); Predecessor is largest node smaller than p (descend right and record candidate).
    // - Complexity: Time: O(H), Space: O(1) auxiliary space.


    // =========================================================
    // 38. FLOOR AND CEIL IN BST
    // =========================================================

    int floorInBST(TreeNode* root, int key) {
        int floorVal = -1;
        while (root) {
            if (root->val == key) return root->val;
            if (key > root->val) {
                floorVal = root->val; // candidate floor
                root = root->right;
            } else {
                root = root->left;
            }
        }
        return floorVal;
    }

    int ceilInBST(TreeNode* root, int key) {
        int ceilVal = -1;
        while (root) {
            if (root->val == key) return root->val;
            if (key < root->val) {
                ceilVal = root->val; // candidate ceil
                root = root->left;
            } else {
                root = root->right;
            }
        }
        return ceilVal;
    }
    // Interview Explanation:
    // - Problem Statement: Find Floor (largest value <= key) and Ceil (smallest value >= key) in a BST.
    // - Approach: Iterative BST Descent.
    // - Intuition: When going right for Floor or left for Ceil, record current root as best candidate so far.
    // - Complexity: Time: O(H), Space: O(1) auxiliary space.


    // =========================================================
    // 39. TWO SUM IN BST / PAIR WITH SUM K
    // =========================================================

    class BSTIteratorTwoSum {
        stack<TreeNode*> st;
        bool reverse; // false -> normal inorder (asc), true -> reverse inorder (desc)
        void pushAll(TreeNode* node) {
            while (node) {
                st.push(node);
                node = reverse ? node->right : node->left;
            }
        }
    public:
        BSTIteratorTwoSum(TreeNode* root, bool isReverse) : reverse(isReverse) {
            pushAll(root);
        }
        int next() {
            TreeNode* tmp = st.top(); st.pop();
            if (!reverse) pushAll(tmp->right);
            else pushAll(tmp->left);
            return tmp->val;
        }
    };

    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        BSTIteratorTwoSum l(root, false);
        BSTIteratorTwoSum r(root, true);

        int i = l.next();
        int j = r.next();
        while (i < j) {
            if (i + j == k) return true;
            if (i + j < k) i = l.next();
            else j = r.next();
        }
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Find if there exist two nodes in BST whose sum equals target K in O(H) space.
    // - Approach: Two Pointers using Forward and Reverse BST Iterators.
    // - Intuition: Forward iterator gives smallest ascending values; reverse iterator gives largest descending values. Two-pointer convergence achieves optimal O(H) memory.
    // - Complexity: Time: O(N) amortized, Space: O(H) stack space.


    // =========================================================
    // 40. BST ITERATOR
    // =========================================================

    class BSTIterator {
        stack<TreeNode*> st;
        void pushAll(TreeNode* node) {
            while (node) {
                st.push(node);
                node = node->left;
            }
        }
    public:
        BSTIterator(TreeNode* root) {
            pushAll(root);
        }
        int next() {
            TreeNode* node = st.top(); st.pop();
            pushAll(node->right);
            return node->val;
        }
        bool hasNext() {
            return !st.empty();
        }
    };
    // Interview Explanation:
    // - Problem Statement: Implement an iterator over inorder traversal of a BST with next() and hasNext() in O(1) average time and O(H) space.
    // - Approach: Controlled Stack Descent pushing left spine.
    // - Intuition: Top of stack is next smallest element; popping it requires descending along the left spine of its right child.
    // - Complexity: Time: O(1) amortized per next(), Space: O(H) stack depth.


    // =========================================================
    // 41. SIZE OF LARGEST BST IN BINARY TREE
    // =========================================================

    struct BSTNodeInfo {
        bool isBST;
        int size;
        int minVal;
        int maxVal;
    };

    BSTNodeInfo largestBSTHelper(TreeNode* root, int& maxBSTSize) {
        if (!root) return {true, 0, INT_MAX, INT_MIN};

        auto left = largestBSTHelper(root->left, maxBSTSize);
        auto right = largestBSTHelper(root->right, maxBSTSize);

        if (left.isBST && right.isBST && root->val > left.maxVal && root->val < right.minVal) {
            int currSize = 1 + left.size + right.size;
            maxBSTSize = max(maxBSTSize, currSize);
            return {true, currSize, min(root->val, left.minVal), max(root->val, right.maxVal)};
        }
        return {false, 0, 0, 0};
    }

    int largestBST(TreeNode* root) {
        int maxBSTSize = 0;
        largestBSTHelper(root, maxBSTSize);
        return maxBSTSize;
    }
    // Interview Explanation:
    // - Problem Statement: Find the size (number of nodes) of the largest subtree that is a valid Binary Search Tree (BST).
    // - Approach: Bottom-up Postorder DFS returning {isBST, size, minVal, maxVal} tuple.
    // - Intuition: Subtree is BST if both left and right subtrees are valid BSTs and .
    // - Complexity: Time: O(N) single pass postorder, Space: O(H) recursion stack.


    // =========================================================
    // 42. MINIMUM TIME TAKEN TO BURN THE BINARY TREE FROM A GIVEN NODE
    // =========================================================

    TreeNode* mapParentsAndFindTarget(TreeNode* root, unordered_map<TreeNode*, TreeNode*>& parentTrack, int startVal) {
        queue<TreeNode*> q;
        q.push(root);
        TreeNode* targetNode = nullptr;

        while (!q.empty()) {
            TreeNode* curr = q.front(); q.pop();
            if (curr->val == startVal) targetNode = curr;
            if (curr->left) {
                parentTrack[curr->left] = curr;
                q.push(curr->left);
            }
            if (curr->right) {
                parentTrack[curr->right] = curr;
                q.push(curr->right);
            }
        }
        return targetNode;
    }

    int minTimeToBurnTree(TreeNode* root, int startVal) {
        unordered_map<TreeNode*, TreeNode*> parentTrack;
        TreeNode* target = mapParentsAndFindTarget(root, parentTrack, startVal);
        if (!target) return 0;

        unordered_map<TreeNode*, bool> visited;
        queue<TreeNode*> q;
        q.push(target);
        visited[target] = true;
        int time = 0;

        while (!q.empty()) {
            int sz = q.size();
            bool burnedAny = false;

            for (int i = 0; i < sz; i++) {
                TreeNode* curr = q.front(); q.pop();

                if (curr->left && !visited[curr->left]) {
                    visited[curr->left] = true;
                    q.push(curr->left);
                    burnedAny = true;
                }
                if (curr->right && !visited[curr->right]) {
                    visited[curr->right] = true;
                    q.push(curr->right);
                    burnedAny = true;
                }
                if (parentTrack.count(curr) && !visited[parentTrack[curr]]) {
                    visited[parentTrack[curr]] = true;
                    q.push(parentTrack[curr]);
                    burnedAny = true;
                }
            }
            if (burnedAny) time++;
        }
        return time;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum time to burn the entire binary tree given starting infection node.
    // - Approach: Parent pointer map construction + Multi-directional BFS spreading left, right, parent.
    // - Intuition: Tree becomes an undirected graph. BFS level by level from the target node tracks radial burn time step by step.
    // - Complexity: Time: O(N) two linear passes, Space: O(N) for parent map and visited tracking.


    // =========================================================
    // 43. DELETE A NODE IN BST
    // =========================================================

    TreeNode* findMinBST(TreeNode* root) {
        while (root->left) root = root->left;
        return root;
    }

    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;

        if (key < root->val) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            root->right = deleteNode(root->right, key);
        } else {
            // Case 1 & 2: 0 or 1 child
            if (!root->left) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }
            // Case 3: 2 children - replace with inorder successor (min of right subtree)
            TreeNode* successor = findMinBST(root->right);
            root->val = successor->val;
            root->right = deleteNode(root->right, successor->val);
        }
        return root;
    }
    // Interview Explanation:
    // - Problem Statement: Delete a key from a Binary Search Tree while preserving BST invariant.
    // - Approach: Recursive Search & Replace with Inorder Successor.
    // - Intuition: Node with two children is replaced with its inorder successor (minimum element in right subtree), then the successor node is recursively deleted from right subtree.
    // - Complexity: Time: O(H) search and splice, Space: O(H) recursion stack.


    // =========================================================
    // 44. CONVERT SORTED ARRAY TO BST (LEETCODE 108)
    // =========================================================

    TreeNode* sortedArrayToBSTHelper(const vector<int>& nums, int l, int r) {
        if (l > r) return nullptr;
        int mid = l + (r - l) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = sortedArrayToBSTHelper(nums, l, mid - 1);
        root->right = sortedArrayToBSTHelper(nums, mid + 1, r);
        return root;
    }

    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return sortedArrayToBSTHelper(nums, 0, (int)nums.size() - 1);
    }
    // Interview Explanation:
    // - Problem Statement: Convert a sorted array into a height-balanced Binary Search Tree (BST) (LeetCode 108).
    // - Approach: Divide and Conquer (Binary Search Midpoint Splitting).
    // - Intuition: Picking middle element `nums[mid]` as subtree root guarantees equal distribution of remaining elements to left and right subtrees, ensuring minimal height O(log N).
    // - Complexity: Time: O(N) creating each node once, Space: O(\log N) recursion stack.


    // =========================================================
    // 45. CONSTRUCT BST FROM PREORDER TRAVERSAL (LEETCODE 1008)
    // =========================================================

    TreeNode* bstFromPreorderHelper(const vector<int>& preorder, int& idx, long long bound) {
        if (idx >= (int)preorder.size() || preorder[idx] > bound) return nullptr;

        TreeNode* root = new TreeNode(preorder[idx++]);
        root->left = bstFromPreorderHelper(preorder, idx, root->val);
        root->right = bstFromPreorderHelper(preorder, idx, bound);
        return root;
    }

    TreeNode* bstFromPreorder(vector<int>& preorder) {
        int idx = 0;
        return bstFromPreorderHelper(preorder, idx, LLONG_MAX);
    }
    // Interview Explanation:
    // - Problem Statement: Construct Binary Search Tree from given preorder traversal in O(N) time (LeetCode 1008).
    // - Approach: Upper-Bound DFS traversal (passing upper limit bound).
    // - Intuition: In preorder traversal `(Root, Left, Right)`, elements `< root->val` belong to left subtree and elements `<= bound` belong to right subtree. Advancing single index pointer `idx` constructs tree in O(N) linear time.
    // - Complexity: Time: O(N) strictly linear single pass, Space: O(H) recursion stack.


    // =========================================================
    // 46. BINARY TREE TO DOUBLY LINKED LIST (IN-PLACE DLL)
    // =========================================================

    void bToDLLHelper(TreeNode* root, TreeNode*& head, TreeNode*& prev) {
        if (!root) return;
        bToDLLHelper(root->left, head, prev);

        if (!prev) {
            head = root; // leftmost node is head of DLL
        } else {
            root->left = prev;
            prev->right = root;
        }
        prev = root;

        bToDLLHelper(root->right, head, prev);
    }

    TreeNode* bToDLL(TreeNode* root) {
        TreeNode *head = nullptr, *prev = nullptr;
        bToDLLHelper(root, head, prev);
        return head;
    }
    // Interview Explanation:
    // - Problem Statement: Convert a binary tree to a doubly linked list in-place according to inorder traversal (GFG / Striver SDE #146).
    // - Approach: Inorder DFS with running `prev` pointer.
    // - Intuition: Inorder traversal visits nodes in sequential DLL order. Connect `root->left = prev` and `prev->right = root`, updating `prev = root`.
    // =========================================================
    // 47. SUBTREE OF ANOTHER TREE (LEETCODE 572)
    // =========================================================

    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!root) return false;
        if (isSameTree(root, subRoot)) return true;
        return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
    }
    // Interview Explanation:
    // - Problem Statement: Check if binary tree contains subRoot as a structural subtree (LeetCode 572).
    // - Approach: DFS tree traversal + isSameTree recursive structural comparison.
    // - Complexity: Time: O(N * M), Space: O(H) recursion stack.
};
