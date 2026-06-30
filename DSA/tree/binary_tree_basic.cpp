/**
 * Problem: Binary Tree Basics
 * Category: Tree Data Structure
 * * Description:
 * A comprehensive template containing standard interview/OA binary tree 
 * representations, traversals, properties, and view utilities.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <map>
#include <functional>

using namespace std;

// Standard Binary Tree Node Definition
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};


unordered_map<TreeNode*, int> nodeToId;
vector<TreeNode*> idToNode;

void assignIds(TreeNode* root) {
    if (!root) return;

    int id = idToNode.size();
    nodeToId[root] = id;
    idToNode.push_back(root);

    assignIds(root->left);
    assignIds(root->right);
}

// Global/Utility variables for specific problems
int diameter = 0;
int maxPath = INT_MIN;
int kthCnt = 0, kthAns = -1;
vector<vector<int>> allPaths;
map<int, int> pos; // Used for fast index lookup during tree reconstruction

// =========================================================================
// 1. BASIC PROPERTIES & VALIDATIONS
// =========================================================================

/**
 * Computes the maximum depth (height) of a binary tree.
 * Time Complexity: O(N) | Space Complexity: O(H) for recursion stack
 */
int maxDepth(TreeNode* root) {
    return root ? 1 + max(maxDepth(root->left), maxDepth(root->right)) : 0;
}

/**
 * Checks if two binary trees are structurally identical with identical values.
 */
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p || !q) return p == q;
    return p->val == q->val && isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

/**
 * Validates if a tree is a structurally valid Binary Search Tree (BST).
 * Uses long long boundaries to avoid integer overflow issues with INT_MIN/MAX.
 */
bool validBST(TreeNode* root, long long mn = LLONG_MIN, long long mx = LLONG_MAX) {
    if (!root) return true;
    if (root->val <= mn || root->val >= mx) return false;
    return validBST(root->left, mn, root->val) && validBST(root->right, root->val, mx);
}

/**
 * Checks if a tree is height-balanced.
 * Returns the height of the tree if balanced, otherwise returns -1.
 */
int balancedDFS(TreeNode* root) {
    if (!root) return 0;
    
    int L = balancedDFS(root->left);
    int R = balancedDFS(root->right);
    
    // If any subtree is unbalanced, or current node violates balance condition
    if (L == -1 || R == -1 || abs(L - R) > 1) return -1;
    
    return 1 + max(L, R);
}

// =========================================================================
// 2. TRANSFORMATIONS & ANCESTRY
// =========================================================================

/**
 * Inverts a binary tree (creates a mirror image) by swapping left and right subtrees.
 */
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left); 
    invertTree(root->right);
    return root;
}

/**
 * Finds the Lowest Common Ancestor (LCA) in a BINARY SEARCH TREE (BST).
 * Leverages the BST property to find the split point.
 */
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root) return nullptr;
    
    // If both nodes are smaller, LCA must be in the left subtree
    if (p->val < root->val && q->val < root->val)
        return lowestCommonAncestor(root->left, p, q);

    // If both nodes are larger, LCA must be in the right subtree
    if (p->val > root->val && q->val > root->val)
        return lowestCommonAncestor(root->right, p, q);

    // Split point found: one node is on left, one on right, or root is one of the nodes
    return root; 
}

// =========================================================================
// 3. TRAVERSALS & VIEWS (BFS / DFS / ITERATIVE)
// =========================================================================

/**
 * Standard Breadth-First Search (BFS) / Level Order Traversal.
 */
vector<vector<int>> levelOrder(TreeNode* root) {
    if (!root) return {};
    vector<vector<int>> ans;
    queue<TreeNode*> q; 
    q.push(root);
    
    while (!q.empty()) {
        int sz = q.size();
        vector<int> level;
        while (sz--) {
            TreeNode* cur = q.front(); q.pop();
            level.push_back(cur->val);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        ans.push_back(level);
    }
    return ans;
}

/**
 * Iterative Inorder Traversal (Left -> Root -> Right) using an explicit stack.
 */
vector<int> inorder(TreeNode* root) {
    vector<int> ans;
    stack<TreeNode*> st;
    TreeNode* cur = root;
    
    while (cur || !st.empty()) {
        while (cur) { 
            st.push(cur); 
            cur = cur->left; 
        }
        cur = st.top(); st.pop();
        ans.push_back(cur->val);
        cur = cur->right;
    }
    return ans;
}

/**
 * Iterative Postorder Traversal (Left -> Right -> Root) using a single stack.
 */
vector<int> postorder(TreeNode* root) {
    vector<int> ans;
    stack<TreeNode*> st;
    TreeNode* cur = root;
    TreeNode* prev = nullptr;
    
    while (cur || !st.empty()) {
        while (cur) { 
            st.push(cur); 
            cur = cur->left; 
        }
        cur = st.top();
        // If right child exists and we haven't visited it yet, move right
        if (cur->right && cur->right != prev) {
            cur = cur->right;
        } else {
            ans.push_back(cur->val);
            st.pop();
            prev = cur;
            cur = nullptr; // Prevents re-visiting the left subtree chain
        }
    }
    return ans;
}

/**
 * Groups node values by their vertical column coordinates.
 */
vector<vector<int>> verticalOrderTraversal(TreeNode* root) {
    if (!root) return {};
    map<int, vector<int>> mp; // Maps column index -> values
    queue<pair<TreeNode*, int>> q; // Pair of {Node, Column}
    
    q.push({root, 0});
    while (!q.empty()) {
        auto [cur, col] = q.front(); q.pop();
        mp[col].push_back(cur->val);
        if (cur->left) q.push({cur->left, col - 1});
        if (cur->right) q.push({cur->right, col + 1});
    }
    
    vector<vector<int>> ans;
    for (auto& [col, vals] : mp) ans.push_back(vals);
    return ans;
}

/**
 * Returns the right-side view of a binary tree (last node seen at each level).
 */
vector<int> rightView(TreeNode* root) {
    if (!root) return {};
    vector<int> ans;
    queue<TreeNode*> q; 
    q.push(root);
    
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            TreeNode* cur = q.front(); q.pop();
            // The last node processed in the current level queue loop is the rightmost node
            if (i == sz - 1) ans.push_back(cur->val);
            if (cur->left) q.push(cur->left); 
            if (cur->right) q.push(cur->right); 
        }
    }
    return ans;
}

// =========================================================================
// 4. PATHS, ADVANCED METRICS, & SEARCHES
// =========================================================================

/**
 * Calculates the diameter of a binary tree (longest path between any two nodes).
 */
int diameterDFS(TreeNode* root) {
    if (!root) return 0;
    int L = diameterDFS(root->left);
    int R = diameterDFS(root->right);
    diameter = max(diameter, L + R); // Total path going through the current root
    return 1 + max(L, R);            // Return height to parent
}

/**
 * Finds the maximum path sum between any two nodes in a tree.
 * Uses max(0, ...) to discard sub-paths with negative net values.
 */
int maxPathDFS(TreeNode* root) {
    if (!root) return 0;
    int L = max(0, maxPathDFS(root->left));
    int R = max(0, maxPathDFS(root->right));
    maxPath = max(maxPath, root->val + L + R); // Split point calculation
    return root->val + max(L, R);              // Linear path returned to parent
}

/**
 * Finds the K-th smallest element in a BST using Inorder Traversal.
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right)
 *         : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int inorder(TreeNode* root, int k, int &cnt) {
        if (!root) return -1;

        int left = inorder(root->left, k, cnt);
        if (left != -1) return left;

        cnt++;
        if (cnt == k) return root->val;

        return inorder(root->right, k, cnt);
    } // returns -1 if not found, otherwise returns the k-th smallest value

    int kthSmallest(TreeNode* root, int k) {
        int cnt = 0;
        return inorder(root, k, cnt);
    }
};
/**
 * Checks if the tree has a root-to-leaf path matching the target sum.
 */
bool hasPathSum(TreeNode* root, int target) {
    if (!root) return false;
    if (!root->left && !root->right) return target == root->val;
    return hasPathSum(root->left, target - root->val) || hasPathSum(root->right, target - root->val);
}

/**
 * Finds and collects all root-to-leaf paths.
 */
vector<vector<int>> printAllroottoleafPaths(TreeNode* root) {
    vector<vector<int>> ans;
    vector<int> path;
    
    // Utilizing std::function for clean recursive lambda closure
    function<void(TreeNode*)> dfs = [&](TreeNode* node) {
        if (!node) return;
        path.push_back(node->val);
        if (!node->left && !node->right) ans.push_back(path);
        dfs(node->left); 
        dfs(node->right);
        path.pop_back(); // Backtrack step
    };
    
    dfs(root);
    return ans;
}

/**
 * Computes the maximum width of a binary tree.
 * Note: If using standard problem rules where empty gaps count, node indices
 * should be stored as `unsigned long long` inside the queue to prevent overflow.
 */
int maxWidth(TreeNode* root) {
    if (!root) return 0;
    queue<pair<TreeNode*, unsigned long long>> q; 
    q.push({root, 0});
    unsigned long long ans = 0;
    
    while (!q.empty()) {
        int sz = q.size();
        unsigned long long minIdx = q.front().second; // Normalize indices to avoid extreme growth overflow
        unsigned long long first = 0, last = 0;
        
        for (int i = 0; i < sz; ++i) {
            auto [cur, id] = q.front(); q.pop();
            unsigned long long curIdx = id - minIdx;
            if (i == 0) first = curIdx;
            if (i == sz - 1) last = curIdx;
            
            if (cur->left) q.push({cur->left, 2 * curIdx + 1});
            if (cur->right) q.push({cur->right, 2 * curIdx + 2});
        }
        ans = max(ans, last - first + 1);
    }
    return ans;
}

// =========================================================================
// 5. TREE RECONSTRUCTION UTILITIES
// =========================================================================

// Helper for Inorder + Postorder Reconstruction
TreeNode* buildFromPost(vector<int>& inorder, vector<int>& postorder, int l1, int r1, int l2, int r2) {
    if (l1 > r1) return nullptr;
    TreeNode* root = new TreeNode(postorder[r2]);
    int mid = pos[root->val], leftSize = mid - l1;
    
    root->left = buildFromPost(inorder, postorder, l1, mid - 1, l2, l2 + leftSize - 1);
    root->right = buildFromPost(inorder, postorder, mid + 1, r1, l2 + leftSize, r2 - 1);
    return root;
}

// Helper for Preorder + Inorder Reconstruction
TreeNode* buildFromPre(vector<int>& preorder, vector<int>& inorder, int l1, int r1, int l2, int r2) {
    if (l1 > r1) return nullptr;
    TreeNode* root = new TreeNode(preorder[l1]);
    int mid = pos[root->val], leftSize = mid - l2;
    
    root->left = buildFromPre(preorder, inorder, l1 + 1, l1 + leftSize, l2, mid - 1);
    root->right = buildFromPre(preorder, inorder, l1 + leftSize + 1, r1, mid + 1, r2);
    return root;
}

TreeNode* buildTreeFromInorderAndPostorder(vector<int>& inorder, vector<int>& postorder) {
    pos.clear();
    for (int i = 0; i < inorder.size(); ++i) pos[inorder[i]] = i;
    return buildFromPost(inorder, postorder, 0, inorder.size() - 1, 0, postorder.size() - 1);
}

TreeNode* buildTreeFromPreorderAndInorder(vector<int>& preorder, vector<int>& inorder) {
    pos.clear();
    for (int i = 0; i < inorder.size(); ++i) pos[inorder[i]] = i;
    return buildFromPre(preorder, inorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
}

int maxInBST(TreeNode* root) {
    if (!root) return INT_MIN;
    while (root->right) root = root->right;
    return root->val;
}

int minInBST(TreeNode* root) {
    if (!root) return INT_MAX;
    while (root->left) root = root->left;
    return root->val;
}

int searchInBST(TreeNode* root, int target) {
    while (root) {
        if (root->val == target) return 1;
        root = (target < root->val) ? root->left : root->right;
    }
    return 0;
}

int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st;

    while (root || !st.empty()) {
        while (root) {
            st.push(root);
            root = root->left;
        }

        root = st.top();
        st.pop();

        if (--k == 0) return root->val;

        root = root->right;
    }

    return -1;
}

int kthLargest(TreeNode* root, int k) {
    stack<TreeNode*> st;

    while (root || !st.empty()) {
        while (root) {
            st.push(root);
            root = root->right;
        }

        root = st.top();
        st.pop();

        if (--k == 0) return root->val;

        root = root->left;
    }

    return -1;
}

void printAllnodesAtDistanceK(TreeNode* root, int k, vector<int>& ans) {
    if (!root) return;

    queue<TreeNode*> q;
    q.push(root);

    int level = 0;

    while (!q.empty()) {
        int sz = q.size();

        if (level == k) {
            while (!q.empty()) {
                ans.push_back(q.front()->val);
                q.pop();
            }
            return;
        }

        for (int i = 0; i < sz; i++) {
            TreeNode* curr = q.front();
            q.pop();

            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }

        level++;
    }
}

int minTimeTakentoBurn(TreeNode* root, int target) {
    if (!root) return 0;

    map<TreeNode*, TreeNode*> parentMap;
    queue<TreeNode*> q;
    TreeNode* targetNode = nullptr;

    // Step 1: Build parent mapping and find the target node
    q.push(root);
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();

        if (curr->val == target) {
            targetNode = curr;
        }

        if (curr->left) {
            parentMap[curr->left] = curr;
            q.push(curr->left);
        }
        if (curr->right) {
            parentMap[curr->right] = curr;
            q.push(curr->right);
        }
    }

    // Step 2: BFS to simulate burning process
    set<TreeNode*> visited;
    q.push(targetNode);
    visited.insert(targetNode);

    int time = 0;

    while (!q.empty()) {
        int sz = q.size();
        bool burned = false;

        for (int i = 0; i < sz; i++) {
            TreeNode* curr = q.front();
            q.pop();

            // Check left child
            if (curr->left && visited.find(curr->left) == visited.end()) {
                visited.insert(curr->left);
                q.push(curr->left);
                burned = true;
            }

            // Check right child
            if (curr->right && visited.find(curr->right) == visited.end()) {
                visited.insert(curr->right);
                q.push(curr->right);
                burned = true;
            }

            // Check parent
            if (parentMap.find(curr) != parentMap.end() && visited.find(parentMap[curr]) == visited.end()) { // If parent exists and not visited
                visited.insert(parentMap[curr]);
                q.push(parentMap[curr]);
                burned = true;
            }
        }

        if (burned) time++;
    }

    return time;
}

// =========================================================================
// MAIN EXECUTIVE ENVIRONMENT
// =========================================================================
int main() {
    // Fast I/O configuration
    ios_base::sync_with_stdio(false); 
    cin.tie(nullptr);
    return 0;
}