/*
    Problem: Lowest Common Ancestor of Deepest Leaves
    Link: https://leetcode.com/problems/lowest-common-ancestor-of-deepest-leaves/

    Approach:
    1. Assign an index to every node.
    2. Run DFS to compute depth and immediate parent.
    3. Build the binary lifting table.
    4. Find all deepest leaves using level-order traversal.
    5. Compute the LCA of all deepest leaves.

    Time: O(N log N)
    Space: O(N log N)
*/

#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    static const int LOG = 20;

    unordered_map<TreeNode*, int> id;
    vector<TreeNode*> nodes;
    vector<int> depth;
    vector<vector<int>> up;

    int idx = 0;

    int count(TreeNode* root) {
        if (!root) return 0;
        return 1 + count(root->left) + count(root->right);
    } // returns total number of nodes in tree

    void assign(TreeNode* root) {
        if (!root) return;

        id[root] = idx++;
        nodes.push_back(root);

        assign(root->left);
        assign(root->right);
    }

    void dfs(TreeNode* root, int p, int d) {
        if (!root) return;

        int u = id[root];
        depth[u] = d;
        up[u][0] = p;

        dfs(root->left, u, d + 1);
        dfs(root->right, u, d + 1);
    }

    void build(int n) {
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                if (up[i][j - 1] != -1)
                    up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }

    vector<int> deepest(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);

        vector<int> leaves;

        while (!q.empty()) {
            int sz = q.size();
            leaves.clear();

            while (sz--) {
                TreeNode* cur = q.front();
                q.pop();

                leaves.push_back(id[cur]);

                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
        }

        return leaves;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v])
            swap(u, v);

        int diff = depth[u] - depth[v];

        for (int j = LOG - 1; j >= 0; j--) {
            if (diff & (1 << j))
                u = up[u][j];
        }

        if (u == v)
            return u;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

public:
    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        if (!root) return nullptr;

        int n = count(root);

        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG, -1));

        assign(root);
        dfs(root, -1, 0);
        build(n);

        vector<int> leaves = deepest(root);

        int ans = leaves[0];
        for (int i = 1; i < leaves.size(); i++)
            ans = lca(ans, leaves[i]);

        return nodes[ans];
    }
};