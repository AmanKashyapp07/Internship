#include <vector>
#include <unordered_map>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    unordered_map<int, int> pos;

    TreeNode* buildPost(vector<int>& in, vector<int>& post,
                        int il, int ir, int pl, int pr) {
        if (il > ir) return nullptr;

        int val = post[pr];
        TreeNode* root = new TreeNode(val);

        int mid = pos[val];
        int left = mid - il;

        root->left = buildPost(in, post,
                               il, mid - 1,
                               pl, pl + left - 1);

        root->right = buildPost(in, post,
                                mid + 1, ir,
                                pl + left, pr - 1);

        return root;
    }

    TreeNode* buildPre(vector<int>& pre, vector<int>& in,
                       int pl, int pr, int il, int ir) {
        if (pl > pr) return nullptr;

        int val = pre[pl];
        TreeNode* root = new TreeNode(val);

        int mid = pos[val];
        int left = mid - il;

        root->left = buildPre(pre, in,
                              pl + 1, pl + left,
                              il, mid - 1);

        root->right = buildPre(pre, in,
                               pl + left + 1, pr,
                               mid + 1, ir);

        return root;
    }

public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++)
            pos[inorder[i]] = i;

        return buildPost(inorder, postorder,
                         0, inorder.size() - 1,
                         0, postorder.size() - 1);
    }

    TreeNode* buildTreePre(vector<int>& preorder, vector<int>& inorder) {
        pos.clear();
        for (int i = 0; i < inorder.size(); i++)
            pos[inorder[i]] = i;

        return buildPre(preorder, inorder,
                        0, preorder.size() - 1,
                        0, inorder.size() - 1);
    }
};