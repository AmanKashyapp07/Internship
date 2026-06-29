struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    static const int LOG = 20;

    unordered_map<TreeNode*, int> id;
    vector<TreeNode*> node;

    vector<int> depth;
    vector<vector<int>> up; // up[i][j] = 2^j-th ancestor of node i, when j=0, it refers to the immediate parent

    int currId = 0;

    // Count total nodes
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    // Assign IDs
    void assignIds(TreeNode* root) {
        if (!root) return;

        id[root] = currId;
        currId++;
        node.push_back(root);

        assignIds(root->left);
        assignIds(root->right);
    }

    // DFS to store depth and immediate parent
    void dfs(TreeNode* root, int parent, int d) {
        if (!root) return;

        int u = id[root];
        depth[u] = d;
        up[u][0] = parent;

        dfs(root->left, u, d + 1);
        dfs(root->right, u, d + 1);
    }

    // Binary lifting table
    void build(int n) {
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                if (up[i][j - 1] != -1)
                    up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }

    // Get deepest leaves
    vector<int> getDeepestLeaves(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);

        vector<int> deepest;

        while (!q.empty()) {
            int sz = q.size();
            deepest.clear();

            while (sz--) {
                TreeNode* cur = q.front();
                q.pop();

                deepest.push_back(id[cur]);

                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
        }

        return deepest;
    }

    int LCA(int u, int v) {
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

    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        if (!root) return nullptr;

        int n = countNodes(root);

        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG, -1));

        assignIds(root);

        dfs(root, -1, 0);

        build(n);

        vector<int> deepest = getDeepestLeaves(root);

        int ans = deepest[0];
        for (int i = 1; i < deepest.size(); i++)
            ans = LCA(ans, deepest[i]);

        return node[ans];
    }
};