/**
 * LeetCode 337 - House Robber III
 *
 * Description:
 * The thief has found himself a new place for his thievery. There is only one entrance to this area, called root.
 * Besides the root, each house has one and only one parent house.
 * Determine the maximum amount of money the thief can rob tonight without alerting the police (cannot rob adjacent houses).
 *
 * Approach:
 * - Dynamic Programming on Trees.
 * - Let `dfs(node)` compute the maximum money starting at `node`.
 * - For each node, we have two options:
 *   - Take: `node->val + (dfs of grandchildren)`.
 *   - Not Take: `(dfs of left child) + (dfs of right child)`.
 * - Use a memoization map/array (in this implementation, mapping `node->val` to DP results).
 *
 * Time Complexity: O(n) where n is the number of nodes in the tree.
 * Space Complexity: O(n) recursion stack and DP table.
 */

#include <algorithm>
#include <vector>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution
{
public:
    vector<int> memo;

    int dfs(TreeNode *node)
    {   
        if(!node) return 0;
        if(memo[node->val] != -1) return memo[node->val];
        int take = 0;
        // take
        take += node->val;
        int L = node->left ? dfs(node->left->left) + dfs(node->left->right) : 0;
        int R = node->right ? dfs(node->right->left) + dfs(node->right->right) : 0;
        take += L + R;

        // not take
        int notTake = dfs(node->left) + dfs(node->right);

        return memo[node->val] = max(take, notTake);
    }

    int rob(TreeNode *root)
    {
        memo.assign(1000, -1);
        return dfs(root);
    }
};