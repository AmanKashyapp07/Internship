#include <vector>
#include <unordered_map>
using namespace std;

struct TreeNode {
    int val; TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    unordered_map<int, int> pos; // Inorder index mapping for O(1) pivot lookups

    // Recursive helper that builds subtrees by checking if the next structural element 
    // falls within the current inorder [left, right] boundary.
    TreeNode* build(vector<int>& order, int& idx, int l, int r, bool is_pre) {
        if (l > r) return nullptr;

        TreeNode* root = new TreeNode(order[idx]);
        int mid = pos[order[idx]];
        
        // Move iterator pointer forward for preorder, backward for postorder
        idx += (is_pre ? 1 : -1); 

        if (is_pre) {
            // Preorder: Root -> Left -> Right (processes left subtree first)
            root->left  = build(order, idx, l, mid - 1, is_pre);
            root->right = build(order, idx, mid + 1, r, is_pre);
        } else {
            // Postorder: Left -> Right -> Root (processes right subtree first in reverse)
            root->right = build(order, idx, mid + 1, r, is_pre);
            root->left  = build(order, idx, l, mid - 1, is_pre);
        }
        return root;
    }

public:
    // Build Tree from Inorder and Postorder
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++) pos[inorder[i]] = i;
        
        int idx = postorder.size() - 1; // Start from the root at the end of postorder
        return build(postorder, idx, 0, inorder.size() - 1, false);
    }

    // Build Tree from Preorder and Inorder
    TreeNode* buildTreePre(vector<int>& preorder, vector<int>& inorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++) pos[inorder[i]] = i;
        
        int idx = 0; // Start from the root at the beginning of preorder
        return build(preorder, idx, 0, inorder.size() - 1, true);
    }
};