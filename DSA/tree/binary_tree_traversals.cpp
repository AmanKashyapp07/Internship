/*
    Problem: Binary Tree Traversals

    Includes:
    - Preorder
    - Inorder
    - Postorder
    - Level Order (BFS)
    - Height of Binary Tree
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
// pre order -> root, left, right
// in order -> left, root, right
// post order -> left, right, root
void preorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;

    ans.push_back(root->val);
    preorder(root->left, ans);
    preorder(root->right, ans);
}

void inorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;

    inorder(root->left, ans);
    ans.push_back(root->val);
    inorder(root->right, ans);
}

void postorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;

    postorder(root->left, ans);
    postorder(root->right, ans);
    ans.push_back(root->val);
}

vector<int> levelOrder(TreeNode* root) {
    vector<int> ans;
    if (!root) return ans;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();
        ans.push_back(cur->val);

        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }

    return ans;
}

int height(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

void print(vector<int>& v) {
    for (int x : v)
        cout << x << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    vector<int> pre, in, post;

    preorder(root, pre);
    inorder(root, in);
    postorder(root, post);

    cout << "Preorder: ";
    print(pre);

    cout << "Inorder: ";
    print(in);

    cout << "Postorder: ";
    print(post);

    cout << "Level Order: ";
    vector<int> level = levelOrder(root);
    print(level);

    cout << "Height: " << height(root) << '\n';

    return 0;
}