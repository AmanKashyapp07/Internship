/**
 * Problem: Binary Tree Traversals
 * Category: Tree Data Structure
 * * Description:
 * Standard Depth-First Search (DFS) and Breadth-First Search (BFS) 
 * binary tree traversals including both recursive and iterative patterns.
 * * Approach:
 * - DFS: Preorder, Inorder, Postorder (System stack vs Explicit stack)
 * - BFS: Level Order Traversal (Queue-based)
 */

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

// Standard Binary Tree Node Definition
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// =========================================================================
// 1. RECURSIVE TRAVERSALS (DFS)
// =========================================================================

/**
 * Preorder Traversal: Root -> Left -> Right
 */
void preorderDFS(TreeNode* root, vector<int>& result) {
    if (!root) return;
    result.push_back(root->val);
    preorderDFS(root->left, result);
    preorderDFS(root->right, result);
}

/**
 * Inorder Traversal: Left -> Root -> Right
 */
void inorderDFS(TreeNode* root, vector<int>& result) {
    if (!root) return;
    inorderDFS(root->left, result);
    result.push_back(root->val);
    inorderDFS(root->right, result);
}

/**
 * Postorder Traversal: Left -> Right -> Root
 */
void postorderDFS(TreeNode* root, vector<int>& result) {
    if (!root) return;
    postorderDFS(root->left, result);
    postorderDFS(root->right, result);
    result.push_back(root->val);
}

// =========================================================================
// 2. ITERATIVE TRAVERSALS (DFS & BFS)
// =========================================================================

/**
 * Iterative Preorder: Uses a stack to simulate recursion.
 * Note: Right child is pushed before Left child so Left is processed first.
 */
vector<int> iterativePreorder(TreeNode* root) {
    if (!root) return {};
    vector<int> result;
    stack<TreeNode*> st;
    st.push(root);

    while (!st.empty()) {
        TreeNode* cur = st.top(); st.pop();
        result.push_back(cur->val);
        
        if (cur->right) st.push(cur->right);
        if (cur->left) st.push(cur->left);
    }
    return result;
}

/**
 * Iterative Inorder: Traverses down to the leftmost node before processing roots.
 */
vector<int> iterativeInorder(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* cur = root;

    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top(); st.pop();
        result.push_back(cur->val);
        cur = cur->right;
    }
    return result;
}

/**
 * Iterative Postorder: Two-stack implementation.
 * First stack processes nodes in Root->Right->Left sequence, 
 * pushing them into the second stack to output the reversed Left->Right->Root sequence.
 */
vector<int> iterativePostorder(TreeNode* root) {
    if (!root) return {};
    vector<int> result;
    stack<TreeNode*> visitingStack, outputStack;
    visitingStack.push(root);

    while (!visitingStack.empty()) {
        TreeNode* cur = visitingStack.top(); visitingStack.pop();
        outputStack.push(cur);

        if (cur->left) visitingStack.push(cur->left);
        if (cur->right) visitingStack.push(cur->right);
    }

    while (!outputStack.empty()) {
        result.push_back(outputStack.top()->val);
        outputStack.pop();
    }
    return result;
}

/**
 * Breadth-First Search (BFS): Level-by-level level order traversal.
 */
vector<int> levelOrder(TreeNode* root) {
    if (!root) return {};
    vector<int> result;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        result.push_back(cur->val);
        
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    return result;
}

// =========================================================================
// 3. UTILITY METHODS
// =========================================================================

/**
 * Computes the maximum height of the tree.
 */
int calculateHeight(TreeNode* root) {
    return root ? max(calculateHeight(root->left), calculateHeight(root->right)) + 1 : 0;
}

/**
 * Helper to display vector output streams cleanly.
 */
void printVector(const string& label, const vector<int>& vec) {
    cout << label << ": [ ";
    for (int val : vec) cout << val << " ";
    cout << "]\n";
}

// =========================================================================
// MAIN ENTRY POINT
// =========================================================================
int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Constructing a sample tree:
    //         1
    //        / \
    //       2   3
    //      / \  /
    //     4   5 6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2); 
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4); 
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    // Verification Vectors
    vector<int> recPre, recIn, recPost;
    preorderDFS(root, recPre);
    inorderDFS(root, recIn);
    postorderDFS(root, recPost);

    // Displaying Results
    printVector("Recursive Preorder ", recPre);
    printVector("Iterative Preorder ", iterativePreorder(root));
    cout << "---" << "\n";
    printVector("Recursive Inorder  ", recIn);
    printVector("Iterative Inorder  ", iterativeInorder(root));
    cout << "---" << "\n";
    printVector("Recursive Postorder", recPost);
    printVector("Iterative Postorder", iterativePostorder(root));
    cout << "---" << "\n";
    printVector("Level Order (BFS)  ", levelOrder(root));
    
    cout << "\nTree Height: " << calculateHeight(root) << "\n";

    return 0;
}