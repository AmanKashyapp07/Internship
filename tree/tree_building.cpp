/**
 * Problem: Binary Tree Reconstruction
 * Category: Tree Data Structure / Divide and Conquer
 * * Description:
 * Reconstructs a unique binary tree from combination sequences:
 * 1. Inorder + Postorder Traversals
 * 2. Preorder + Inorder Traversals
 * * Approach:
 * Uses a hash map to look up root element split indices in O(1) time. 
 * Divides array ranges using calculated subtree sizes to build the tree recursively.
 */

#include <vector>
#include <unordered_map>

using namespace std;

// Definition for a binary tree node (Included for completeness)
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
private:
    unordered_map<int, int> pos; // Fast lookup cache for Inorder element indices

    // =========================================================================
    // HELPER METHODS (DIVIDE & CONQUER RECURSION)
    // =========================================================================

    /**
     * Recursive builder for Inorder + Postorder
     * - Inorder:   [Left Subtree] -> [Root] -> [Right Subtree]
     * - Postorder: [Left Subtree] -> [Right Subtree] -> [Root]
     */
    TreeNode* buildFromInPost(const vector<int>& inorder, const vector<int>& postorder,
                              int inL, int inR,
                              int postL, int postR) {
        if (inL > inR) return nullptr;

        // The last element of Postorder is always the root of the current subtree
        int rootVal = postorder[postR];
        TreeNode* root = new TreeNode(rootVal);

        // Find the boundary split point in the Inorder array
        int mid = pos[rootVal];
        int leftSize = mid - inL;

        // Build Left Subtree
        root->left = buildFromInPost(
            inorder, postorder,
            inL, mid - 1,
            postL, postL + leftSize - 1
        );

        // Build Right Subtree
        root->right = buildFromInPost(
            inorder, postorder,
            mid + 1, inR,
            postL + leftSize, postR - 1
        );

        return root;
    }

    /**
     * Recursive builder for Preorder + Inorder
     * - Preorder: [Root] -> [Left Subtree] -> [Right Subtree]
     * - Inorder:  [Left Subtree] -> [Root] -> [Right Subtree]
     */
    TreeNode* buildFromPreIn(const vector<int>& preorder, const vector<int>& inorder,
                             int preL, int preR,
                             int inL, int inR) {
        if (preL > preR) return nullptr;

        // The first element of Preorder is always the root of the current subtree
        int rootVal = preorder[preL];
        TreeNode* root = new TreeNode(rootVal);

        // Find the boundary split point in the Inorder array
        int mid = pos[rootVal];
        int leftSize = mid - inL;

        // Build Left Subtree
        root->left = buildFromPreIn(
            preorder, inorder,
            preL + 1, preL + leftSize,
            inL, mid - 1
        );

        // Build Right Subtree
        root->right = buildFromPreIn(
            preorder, inorder,
            preL + leftSize + 1, preR,
            mid + 1, inR
        );

        return root;
    }

public:
    // =========================================================================
    // PUBLIC API INTERFACES
    // =========================================================================

    /**
     * Reconstruct Tree using Inorder and Postorder arrays.
     * Time Complexity: O(N) | Space Complexity: O(N) for the lookup map
     */
    TreeNode* buildTreeFromInorderAndPostorder(vector<int>& inorder, vector<int>& postorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++) {
            pos[inorder[i]] = i;
        }

        return buildFromInPost(
            inorder, postorder,
            0, inorder.size() - 1,
            0, postorder.size() - 1
        );
    }

    /**
     * Reconstruct Tree using Preorder and Inorder arrays.
     * Time Complexity: O(N) | Space Complexity: O(N) for the lookup map
     */
    TreeNode* buildTreeFromPreorderAndInorder(vector<int>& preorder, vector<int>& inorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++) {
            pos[inorder[i]] = i;
        }

        return buildFromPreIn(
            preorder, inorder,
            0, preorder.size() - 1,
            0, inorder.size() - 1
        );
    }
};