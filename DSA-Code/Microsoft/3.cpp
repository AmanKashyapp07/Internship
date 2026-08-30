#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <climits>
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

// =========================================================
// 32. MAXIMUM DEPTH OF BINARY TREE
// =========================================================

int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
// Interview Explanation:
// - Problem Statement: Find maximum depth (number of nodes along longest root-to-leaf path).
// - Approach: Post-Order DFS Recursion.
// - Intuition: Depth at current node is 1 + max(leftDepth, rightDepth).
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 33. SAME TREE
// =========================================================

bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
// Interview Explanation:
// - Problem Statement: Check if two binary trees are structurally identical and have equal node values.
// - Approach: Simultaneous Preorder Tree DFS.
// - Intuition: Compare root values; recursively verify left subtrees and right subtrees.
// - Complexity: Time: O(min(N, M)), Space: O(H).


// =========================================================
// 34. INVERT BINARY TREE
// =========================================================

TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
// Interview Explanation:
// - Problem Statement: Invert binary tree (mirror left and right subtrees).
// - Approach: Post-order / Pre-order Child Pointer Swapping.
// - Intuition: Swap left and right pointers at current node, then recurse on children.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 35. BALANCED BINARY TREE
// =========================================================

int checkHeight(TreeNode* root) {
    if (!root) return 0;
    int leftH = checkHeight(root->left);
    if (leftH == -1) return -1;
    int rightH = checkHeight(root->right);
    if (rightH == -1) return -1;

    if (abs(leftH - rightH) > 1) return -1;
    return 1 + max(leftH, rightH);
}

bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}
// Interview Explanation:
// - Problem Statement: Check if binary tree is height-balanced.
// - Approach: Post-Order Depth DFS with -1 Early Exit Sentinel.
// - Intuition: Calculate subtree heights; return -1 immediately if any subtree is unbalanced to avoid redundant recalculations.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 36. PATH SUM
// =========================================================

bool hasPathSum(TreeNode* root, int targetSum) {
    if (!root) return false;
    if (!root->left && !root->right) return root->val == targetSum;
    return hasPathSum(root->left, targetSum - root->val) ||
           hasPathSum(root->right, targetSum - root->val);
}
// Interview Explanation:
// - Problem Statement: Check if tree has a root-to-leaf path summing to targetSum.
// - Approach: Recursive Pre-Order DFS with Target Subtraction.
// - Intuition: At leaf node, check if remaining target equals leaf value.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 37. DIAMETER OF BINARY TREE
// =========================================================

class SolutionDiameter {
    int maxDia = 0;
    int height(TreeNode* root) {
        if (!root) return 0;
        int l = height(root->left), r = height(root->right);
        maxDia = max(maxDia, l + r);
        return 1 + max(l, r);
    }
public:
    int diameterOfBinaryTree(TreeNode* root) {
        maxDia = 0;
        height(root);
        return maxDia;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest path between any two nodes in a tree.
// - Approach: Post-Order Depth DFS with Global Max Diameter Update.
// - Intuition: Longest path passing through node is leftHeight + rightHeight.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 38. LOWEST COMMON ANCESTOR OF BINARY TREE
// =========================================================

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}
// Interview Explanation:
// - Problem Statement: Find LCA of two nodes p and q in binary tree.
// - Approach: Post-Order DFS (Bottom-Up Value Propagation).
// - Intuition: If p and q are found in separate left/right subtrees, current node is the LCA.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 39. VALIDATE BINARY SEARCH TREE
// =========================================================

bool isValidBSTHelper(TreeNode* root, long long low, long long high) {
    if (!root) return true;
    if (root->val <= low || root->val >= high) return false;
    return isValidBSTHelper(root->left, low, root->val) &&
           isValidBSTHelper(root->right, root->val, high);
}

bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}
// Interview Explanation:
// - Problem Statement: Determine if binary tree is a valid BST.
// - Approach: Range Invariant DFS (low < root->val < high).
// - Intuition: Passing valid range bounds downward prevents false positives from local-only comparisons.
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 40. KTH SMALLEST ELEMENT IN BST
// =========================================================

int kthSmallest(TreeNode* root, int k) {
    vector<TreeNode*> st;
    TreeNode* curr = root;
    while (curr || !st.empty()) {
        while (curr) {
            st.push_back(curr);
            curr = curr->left;
        }
        curr = st.back(); st.pop_back();
        if (--k == 0) return curr->val;
        curr = curr->right;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find kth smallest element in BST.
// - Approach: Iterative Inorder Traversal with Explicit Stack.
// - Intuition: Inorder traversal of BST visits nodes in strictly sorted order.
// - Complexity: Time: O(H + k), Space: O(H).


// =========================================================
// 41. BINARY TREE LEVEL ORDER TRAVERSAL
// =========================================================

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(std::move(level));
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return level order traversal of binary tree node values.
// - Approach: Queue BFS with Level Size Snapshot.
// - Intuition: Snapshot queue size at start of each iteration to group nodes level by level.
// - Complexity: Time: O(N), Space: O(W).


// =========================================================
// 42. BINARY TREE RIGHT SIDE VIEW
// =========================================================

vector<int> rightSideView(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            if (i == sz - 1) res.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return node values visible looking from the right side.
// - Approach: Level BFS capturing the last element (i == sz - 1) of each level.
// - Intuition: Rightmost node of each level is the last node processed in queue snapshot.
// - Complexity: Time: O(N), Space: O(W).


// =========================================================
// 43. CONSTRUCT BINARY TREE FROM PREORDER AND INORDER
// =========================================================

class SolutionBuildTree {
    unordered_map<int, int> inMap;
    TreeNode* build(const vector<int>& pre, int pS, int pE,
                    const vector<int>& in, int iS, int iE) {
        if (pS > pE || iS > iE) return nullptr;
        int rootVal = pre[pS];
        TreeNode* root = new TreeNode(rootVal);
        int inIdx = inMap[rootVal];
        int leftSize = inIdx - iS;

        root->left = build(pre, pS + 1, pS + leftSize, in, iS, inIdx - 1);
        root->right = build(pre, pS + leftSize + 1, pE, in, inIdx + 1, iE);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inMap.clear();
        for (int i = 0; i < (int)inorder.size(); ++i) inMap[inorder[i]] = i;
        return build(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Construct binary tree from preorder and inorder traversal arrays.
// - Approach: Divide & Conquer with Inorder Index Hash Map.
// - Intuition: First element of preorder is root; its index in inorder splits left and right subtrees.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 44. BINARY TREE MAXIMUM PATH SUM
// =========================================================

class SolutionMaxPathSum {
    int maxGain(TreeNode* root, int& globalMax) {
        if (!root) return 0;
        int leftGain = max(maxGain(root->left, globalMax), 0);
        int rightGain = max(maxGain(root->right, globalMax), 0);

        globalMax = max(globalMax, root->val + leftGain + rightGain);
        return root->val + max(leftGain, rightGain);
    }
public:
    int maxPathSum(TreeNode* root) {
        int globalMax = INT_MIN;
        maxGain(root, globalMax);
        return globalMax;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum path sum of any non-empty path in binary tree.
// - Approach: Post-Order DFS Contribution Pattern.
// - Intuition: Discard negative gains with max(gain, 0); update global max with turn path (left + right + root).
// - Complexity: Time: O(N), Space: O(H).


// =========================================================
// 45. SERIALIZE AND DESERIALIZE BINARY TREE
// =========================================================

class Codec {
    void serializeHelper(TreeNode* root, ostringstream& out) {
        if (!root) { out << "# "; return; }
        out << root->val << " ";
        serializeHelper(root->left, out);
        serializeHelper(root->right, out);
    }

    TreeNode* deserializeHelper(istringstream& in) {
        string val;
        if (!(in >> val) || val == "#") return nullptr;
        TreeNode* root = new TreeNode(stoi(val));
        root->left = deserializeHelper(in);
        root->right = deserializeHelper(in);
        return root;
    }
public:
    string serialize(TreeNode* root) {
        ostringstream out;
        serializeHelper(root, out);
        return out.str();
    }
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserializeHelper(in);
    }
};
// Interview Explanation:
// - Problem Statement: Serialize binary tree to string and deserialize back to identical tree.
// - Approach: Preorder DFS with String Streams and '#' Sentinel for nulls.
// - Intuition: Preorder sequence with explicit null markers uniquely determines binary tree structure.
// - Complexity: Time: O(N), Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 32 | Maximum Depth of Binary Tree                | Post-Order DFS Recursion          | O(N)     | O(H)     |
 | 33 | Same Tree                                   | Simultaneous Preorder Tree DFS    | O(minN,M)| O(H)     |
 | 34 | Invert Binary Tree                          | Child Pointer Swapping            | O(N)     | O(H)     |
 | 35 | Balanced Binary Tree                        | Height DFS with -1 Early Sentinel | O(N)     | O(H)     |
 | 36 | Path Sum                                    | Root-to-Leaf Backtracking DFS     | O(N)     | O(H)     |
 | 37 | Diameter of Binary Tree                     | Post-Order Height Contribution DFS| O(N)     | O(H)     |
 | 38 | Lowest Common Ancestor of Binary Tree       | Post-Order DFS Propagation        | O(N)     | O(H)     |
 | 39 | Validate Binary Search Tree                 | Range Invariant DFS (low, high)   | O(N)     | O(H)     |
 | 40 | Kth Smallest Element in BST                 | Iterative Inorder Traversal Stack | O(H + k) | O(H)     |
 | 41 | Binary Tree Level Order Traversal           | Queue BFS Level Size Snapshot     | O(N)     | O(W)     |
 | 42 | Binary Tree Right Side View                 | Level BFS Last-Element Capture    | O(N)     | O(W)     |
 | 43 | Construct Tree from Preorder and Inorder    | Divide & Conquer + Hash Map       | O(N)     | O(N)     |
 | 44 | Binary Tree Maximum Path Sum                | Post-Order Path Contribution      | O(N)     | O(H)     |
 | 45 | Serialize and Deserialize Binary Tree       | Preorder DFS + Delimiters         | O(N)     | O(N)     |
 ====================================================================================================
*/
