/*
    Problem: Binary Tree Basics

    Includes:
    - Height
    - Same Tree
    - Validate BST
    - Balanced Binary Tree
    - Invert Binary Tree
    - LCA in BST
    - Level Order Traversal
    - Vertical Order Traversal
    - Right View
*/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <set>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
}; // this will be given in leetcode problems, so we don't need to implement it ourselves

unordered_map<TreeNode*, int> id;
unordered_map<int, TreeNode*> node;

int idx = 0;

void assignIds(TreeNode* root) {
    if (!root) return;
    id[root] = idx;
    node[idx] = root;
    idx++;
    assignIds(root->left);
    assignIds(root->right);
}

int height(TreeNode* root) {
    if (!root) return 0;
    int L = height(root->left);
    int R = height(root->right);
    return 1 + max(L, R);
}

bool same(TreeNode* a, TreeNode* b) {
    if (!a || !b) return a == b; // if both are null, return true; if one is null, return false
    return a->val == b->val &&
           same(a->left, b->left) &&
           same(a->right, b->right);
}

bool isBST(TreeNode* root, long long lo = LLONG_MIN, long long hi = LLONG_MAX) {
    if (!root) return true;
    if (root->val <= lo || root->val >= hi) return false;

    return isBST(root->left, lo, root->val) &&
           isBST(root->right, root->val, hi);
}

int check(TreeNode* root) {
    if (!root) return 0;

    int l = check(root->left);
    int r = check(root->right);

    if (l == -1 || r == -1 || abs(l - r) > 1) return -1;

    return 1 + max(l, r);
} // checking if the tree is balanced or not, returns -1 if not balanced, else returns height of the tree
// just manipulate height dfs to check 


TreeNode* lcaBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root) return nullptr;
    
    if (p->val < root->val && q->val < root->val) return lcaBST(root->left, p, q);
    if (p->val > root->val && q->val > root->val) return lcaBST(root->right, p, q);

    return root;
}

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        vector<int> level;

        while (sz--) {
            TreeNode* cur = q.front();
            q.pop();
            level.push_back(cur->val);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        ans.push_back(level);
    }
    return ans;
}

int diameter = 0;
int maxPath = INT_MIN;
map<int, int> pos;

int dfsDiameter(TreeNode* root) {
    if (!root) return 0;

    int l = dfsDiameter(root->left);
    int r = dfsDiameter(root->right);
    diameter = max(diameter, l + r);
    return 1 + max(l, r);
} // it will return the height of the tree, but it will also update the diameter variable to store the maximum diameter found so far

int dfsMaxPath(TreeNode* root) {
    if (!root) return 0;

    int l = max(0, dfsMaxPath(root->left)); // why max(0, ...) ? because we want to ignore negative paths, if the left path is negative, we don't want to include it in the path sum
    int r = max(0, dfsMaxPath(root->right));

    maxPath = max(maxPath, root->val + l + r);

    return root->val + max(l, r);
} // it will return max path sum from root to leaf, but it will also update the maxPath variable to store the maximum path sum found so far

void kth(TreeNode* root, int k, int& cnt, int& ans) {
    if (!root || ans != -1) return;
    kth(root->left, k, cnt, ans);
    cnt++; // for counting root node as well
    if (cnt == k) {
        ans = root->val;
        return;
    }
    kth(root->right, k, cnt, ans);
}

int kthSmallest(TreeNode* root, int k) {
    int cnt = 0, ans = -1;
    kth(root, k, cnt, ans);
    return ans;
}

bool hasPathSum(TreeNode* root, int sum) {
    if (!root) return false;
    int value = root->val;
    if (!root->left && !root->right) return sum == value; // if it's a leaf node, check if the sum equals the value of the node
    
    return hasPathSum(root->left, sum - value) ||
           hasPathSum(root->right, sum - value);
} // checks if there is a root-to-leaf path in the tree such that the sum of the values along the path equals the given sum

void dfsPaths(TreeNode* root, vector<int>& path, vector<vector<int>>& ans) {
    if (!root) return;

    path.push_back(root->val);

    if (!root->left && !root->right)
        ans.push_back(path);

    dfsPaths(root->left, path, ans);
    dfsPaths(root->right, path, ans);

    path.pop_back();
} // returns all paths from root to leaf nodes in the binary tree, storing them in ans. The path vector keeps track of the current path being explored.

vector<vector<int>> allPaths(TreeNode* root) {
    vector<vector<int>> ans;
    vector<int> path;

    dfsPaths(root, path, ans);

    return ans;
}

int minBST(TreeNode* root) {
    if(!root) return INT_MAX;
    while(root && root->left) root = root->left;
    int value = root->val;
    return value;
}

int maxBST(TreeNode* root) {
    if(!root) return INT_MIN;
    while(root && root->right) root = root->right;
    int value = root->val;
    return value;
}

bool searchBST(TreeNode* root, int x) {
    while (root) {
        if (root->val == x) return true;
        if (x < root->val) root = root->left;
        else root = root->right;
    }
    return false;
}

void kth_2(TreeNode* root, int k, int& cnt, int& ans) {
    if (!root || ans != -1) return;
    kth_2(root->right, k, cnt, ans);
    cnt++;
    if (cnt == k) {
        ans = root->val;
        return;
    }
    kth_2(root->left, k, cnt, ans);
}

int kthLargest(TreeNode* root, int k) {
    int cnt = 0, ans = -1;
    kth_2(root, k, cnt, ans);
    return ans;
}

int burnTime(TreeNode* root, int target) {
    if (!root) return 0;

    unordered_map<TreeNode*, TreeNode*> par;
    unordered_set<TreeNode*> vis;
    queue<TreeNode*> q;

    TreeNode* start = nullptr;

    q.push(root);

    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();

        if (cur->val == target)
            start = cur;

        if (cur->left) {
            par[cur->left] = cur;
            q.push(cur->left);
        }

        if (cur->right) {
            par[cur->right] = cur;
            q.push(cur->right);
        }
    }

    if (!start) return 0;

    q.push(start);
    vis.insert(start);

    int time = 0;

    while (!q.empty()) {
        int sz = q.size();
        bool spread = false;

        while (sz--) {
            TreeNode* cur = q.front();
            q.pop();

            if (cur->left && vis.insert(cur->left).second) { // vis.insert(cur->left).second checks if the left child has already been visited. If it hasn't, it marks it as visited and returns true, allowing the fire to spread to that node.
                q.push(cur->left);
                spread = true;
            }

            if (cur->right && vis.insert(cur->right).second) {
                q.push(cur->right);
                spread = true;
            }

            if (par.count(cur) && vis.insert(par[cur]).second) {
                q.push(par[cur]);
                spread = true;
            }
        }

        if (spread)
            time++;
    }

    return time;
}