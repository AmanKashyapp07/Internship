#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

/**
 * ============================================================================
 * DESIGN 07: BINARY SEARCH TREE (BST)
 * ============================================================================
 * 
 * A Binary Search Tree is a hierarchical node-based data structure where:
 * - The left subtree of a node contains only nodes with keys < node's key.
 * - The right subtree of a node contains only nodes with keys > node's key.
 * - Both left and right subtrees must also be binary search trees (No duplicates).
 * 
 * Key Interview Operations:
 * 1. Insert, Search, Delete (Handling 0, 1, and 2 children via Inorder Successor)
 * 2. Traversals: Inorder (yields sorted output!), Preorder, Postorder, Level-Order (BFS)
 * 3. Utility Algorithms: findMin, findMax, getHeight, isValidBST
 * 
 * Time Complexities:
 * - Search / Insert / Delete:
 *   - Average / Balanced: O(log N)
 *   - Worst-case (skewed / degenerate tree): O(N)
 * - Inorder/Preorder/Postorder/BFS: O(N)
 * Space Complexity:
 * - O(H) recursion stack space (where H is tree height, log N average, N worst-case)
 */

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    TreeNode* root;

    // Helper: Insert a value into BST: O(H)
    TreeNode* insertHelper(TreeNode* node, int val) {
        if (node == nullptr) {
            return new TreeNode(val);
        }
        if (val < node->val) {
            node->left = insertHelper(node->left, val);
        } else if (val > node->val) {
            node->right = insertHelper(node->right, val);
        }
        // If val == node->val, duplicate is ignored
        return node;
    }

    // Helper: Search for value in BST: O(H)
    TreeNode* searchHelper(TreeNode* node, int val) const {
        if (node == nullptr || node->val == val) {
            return node;
        }
        if (val < node->val) {
            return searchHelper(node->left, val);
        }
        return searchHelper(node->right, val);
    }

    // Helper: Find node with minimum value in a subtree (leftmost node): O(H)
    TreeNode* findMinNode(TreeNode* node) const {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Helper: Delete a node from BST: O(H)
    TreeNode* deleteHelper(TreeNode* node, int val) {
        if (node == nullptr) return nullptr;

        if (val < node->val) {
            node->left = deleteHelper(node->left, val);
        } else if (val > node->val) {
            node->right = deleteHelper(node->right, val);
        } else {
            // Found node to delete! Handle 3 structural cases:

            // Case 1: Node has NO children (Leaf node)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // Case 2: Node has only ONE child
            else if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Node has TWO children
            // Find in-order successor (smallest node in right subtree)
            TreeNode* successor = findMinNode(node->right);
            // Copy successor value to current node
            node->val = successor->val;
            // Delete successor node from right subtree
            node->right = deleteHelper(node->right, successor->val);
        }
        return node;
    }

    // Traversal Helpers
    void inorderHelper(TreeNode* node, vector<int>& res) const {
        if (!node) return;
        inorderHelper(node->left, res);
        res.push_back(node->val); // Inorder yields strictly sorted output!
        inorderHelper(node->right, res);
    }

    void preorderHelper(TreeNode* node, vector<int>& res) const {
        if (!node) return;
        res.push_back(node->val);
        preorderHelper(node->left, res);
        preorderHelper(node->right, res);
    }

    void postorderHelper(TreeNode* node, vector<int>& res) const {
        if (!node) return;
        postorderHelper(node->left, res);
        postorderHelper(node->right, res);
        res.push_back(node->val);
    }

    int getHeightHelper(TreeNode* node) const {
        if (!node) return 0;
        return 1 + max(getHeightHelper(node->left), getHeightHelper(node->right));
    }

    bool isValidBSTHelper(TreeNode* node, long long minVal, long long maxVal) const {
        if (!node) return true;
        if (node->val <= minVal || node->val >= maxVal) return false;
        return isValidBSTHelper(node->left, minVal, node->val) &&
               isValidBSTHelper(node->right, node->val, maxVal);
    }

    void destroyTree(TreeNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    // ========================================================================
    // PUBLIC INTERFACE
    // ========================================================================

    void insert(int val) {
        root = insertHelper(root, val);
    }

    bool search(int val) const {
        return searchHelper(root, val) != nullptr;
    }

    void remove(int val) {
        root = deleteHelper(root, val);
    }

    int getMin() const {
        if (!root) throw runtime_error("Tree is empty!");
        return findMinNode(root)->val;
    }

    int getMax() const {
        if (!root) throw runtime_error("Tree is empty!");
        TreeNode* curr = root;
        while (curr->right != nullptr) {
            curr = curr->right;
        }
        return curr->val;
    }

    int getHeight() const {
        return getHeightHelper(root);
    }

    bool isValidBST() const {
        return isValidBSTHelper(root, -1e18, 1e18);
    }

    // Traversals
    vector<int> inorder() const {
        vector<int> res;
        inorderHelper(root, res);
        return res;
    }

    vector<int> preorder() const {
        vector<int> res;
        preorderHelper(root, res);
        return res;
    }

    vector<int> postorder() const {
        vector<int> res;
        postorderHelper(root, res);
        return res;
    }

    // Level-Order Traversal (BFS): O(N)
    vector<vector<int>> levelOrder() const {
        vector<vector<int>> result;
        if (!root) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel;
            for (int i = 0; i < levelSize; i++) {
                TreeNode* curr = q.front();
                q.pop();
                currentLevel.push_back(curr->val);

                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            result.push_back(currentLevel);
        }
        return result;
    }
};
