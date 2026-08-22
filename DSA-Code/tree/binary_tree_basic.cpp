#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Global mapping framework
unordered_map<TreeNode*, int> id;
unordered_map<int, TreeNode*> node;
int idx = 0;

void assignIds(TreeNode* root) {
    if (!root) return;
    id[root] = idx;
    node[idx++] = root;
    assignIds(root->left);
    assignIds(root->right);
}

TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
int height(TreeNode* r) {
    return r ? 1 + max(height(r->left), height(r->right)) : 0;
}

bool same(TreeNode* a, TreeNode* b) {
    if (!a || !b) return a == b;
    return a->val == b->val && same(a->left, b->left) && same(a->right, b->right);
}

bool isBST(TreeNode* r, long long lo = LLONG_MIN, long long hi = LLONG_MAX) {
    return !r || (r->val > lo && r->val < hi && isBST(r->left, lo, r->val) && isBST(r->right, r->val, hi));
}

int check(TreeNode* r) {
    if (!r) return 0;
    int l = check(r->left), w = check(r->right);
    return (l == -1 || w == -1 || abs(l - w) > 1) ? -1 : 1 + max(l, w);
}

TreeNode* lcaBST(TreeNode* r, TreeNode* p, TreeNode* q) {
    if (!r) return nullptr;
    if (p->val < r->val && q->val < r->val) return lcaBST(r->left, p, q);
    if (p->val > r->val && q->val > r->val) return lcaBST(r->right, p, q);
    return r;
}

vector<vector<int>> levelOrder(TreeNode* r) {
    vector<vector<int>> ans;
    if (!r) return ans;
    queue<TreeNode*> q;
    q.push(r);
    while (!q.empty()) {
        int sz = q.size();
        vector<int> lvl;
        while (sz--) {
            TreeNode* c = q.front(); q.pop();
            lvl.push_back(c->val);
            if (c->left) q.push(c->left);
            if (c->right) q.push(c->right);
        }
        ans.push_back(lvl);
    }
    return ans;
}

int dfsDiameter(TreeNode* r, int& d) {
    if (!r) return 0;
    int l = dfsDiameter(r->left, d), w = dfsDiameter(r->right, d);
    d = max(d, l + w);
    return 1 + max(l, w);
}

int dfsMaxPath(TreeNode* r, int& mx) {
    if (!r) return 0;
    int l = max(0, dfsMaxPath(r->left, mx)), w = max(0, dfsMaxPath(r->right, mx));
    mx = max(mx, r->val + l + w);
    return r->val + max(l, w);
}

void kth(TreeNode* r, int k, int& cnt, int& ans, bool rev = false) {
    if (!r || ans != -1) return;
    kth(rev ? r->right : r->left, k, cnt, ans, rev);
    if (++cnt == k) {
        ans = r->val;
        return;
    }
    kth(rev ? r->left : r->right, k, cnt, ans, rev);
}

int kthSmallest(TreeNode* r, int k) {
    int c = 0, a = -1;
    kth(r, k, c, a, false);
    return a;
}

int kthLargest(TreeNode* r, int k) {
    int c = 0, a = -1;
    kth(r, k, c, a, true);
    return a;
}

bool hasPathSum(TreeNode* r, int sum) {
    if (!r) return false;
    if (!r->left && !r->right) return sum == r->val;
    return hasPathSum(r->left, sum - r->val) || hasPathSum(r->right, sum - r->val);
}

void dfsPaths(TreeNode* r, vector<int>& p, vector<vector<int>>& ans) {
    if (!r) return;
    p.push_back(r->val);
    if (!r->left && !r->right) ans.push_back(p);
    dfsPaths(r->left, p, ans);
    dfsPaths(r->right, p, ans);
    p.pop_back();
}

vector<vector<int>> allPaths(TreeNode* r) {
    vector<vector<int>> ans;
    vector<int> p;
    dfsPaths(r, p, ans);
    return ans;
}

int minBST(TreeNode* r) {
    if (!r) return INT_MAX;
    while (r->left) r = r->left;
    return r->val;
}

int maxBST(TreeNode* r) {
    if (!r) return INT_MIN;
    while (r->right) r = r->right;
    return r->val;
}

bool searchBST(TreeNode* r, int x) {
    while (r) {
        if (r->val == x) return true;
        r = (x < r->val) ? r->left : r->right;
    }
    return false;
}

int burnTime(TreeNode* r, int target) {
    if (!r) return 0;
    unordered_map<TreeNode*, TreeNode*> par;
    queue<TreeNode*> q;
    TreeNode* start = nullptr;
    q.push(r);
    
    while (!q.empty()) {
        TreeNode* c = q.front(); q.pop();
        if (c->val == target) start = c;
        if (c->left) { par[c->left] = c; q.push(c->left); }
        if (c->right) { par[c->right] = c; q.push(c->right); }
    }
    if (!start) return 0;

    unordered_set<TreeNode*> vis{start};
    q.push(start);
    int time = 0;

    while (!q.empty()) {
        int sz = q.size();
        bool spread = false;
        while (sz--) {
            TreeNode* c = q.front(); q.pop();
            if (c->left && vis.insert(c->left).second) { q.push(c->left); spread = true; }
            if (c->right && vis.insert(c->right).second) { q.push(c->right); spread = true; }
            if (par.count(c) && vis.insert(par[c]).second) { q.push(par[c]); spread = true; }
        }
        if (spread) time++;
    }
    return time;
}

vector<int> deepestLeaves(TreeNode* r) {
    vector<int> ans;
    if (!r) return ans;
    queue<TreeNode*> q;
    q.push(r);
    while (!q.empty()) {
        int sz = q.size();
        ans.clear();
        while (sz--) {
            TreeNode* c = q.front(); q.pop();
            ans.push_back(c->val);
            if (c->left) q.push(c->left);
            if (c->right) q.push(c->right);
        }
    }
    return ans;
}
// Helper DFS: Calculates standard tree height and tracks where the max split occurs
int getHeight(TreeNode* r, int& max_d, TreeNode*& split_node) {
    if (!r) return 0;
    
    int lh = getHeight(r->left, max_d, split_node);
    int rh = getHeight(r->right, max_d, split_node);
    
    // Check if the path passing through the current node creates a new maximum diameter
    if (lh + rh > max_d) {
        max_d = lh + rh;
        split_node = r; 
    }
    return 1 + max(lh, rh);
}

// Main function: Returns {diameter_length, a_node_on_the_diameter_path}
pair<int, int> diameterAndNode(TreeNode* r) {
    if (!r) return {0, 0};
    
    int max_d = 0;
    TreeNode* split_node = nullptr;
    
    // First DFS: Locates the highest node where the maximum diameter split occurs
    getHeight(r, max_d, split_node);
    
    // Second DFS step (Implicit): Return the value of that tracked split node
    return {max_d, split_node ? split_node->val : 0};
}

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


int height(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

void print(vector<int>& v) {
    for (int x : v)
        cout << x << " ";
    cout << '\n';
}