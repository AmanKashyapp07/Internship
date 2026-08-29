// Link: https://cses.fi/problemset/task/1702
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val; Node *left = nullptr, *right = nullptr;
    Node(int v) : val(v) {}
};

Node* buildTree(const vector<int>& pre, const vector<int>& in, int inStart, int inEnd, int& preIdx, const unordered_map<int, int>& inMap) {
    if (inStart > inEnd) return nullptr;
    Node* node = new Node(pre[preIdx++]);
    if (inStart == inEnd) return node;
    int inIdx = inMap.at(node->val);
    node->left = buildTree(pre, in, inStart, inIdx - 1, preIdx, inMap);
    node->right = buildTree(pre, in, inIdx + 1, inEnd, preIdx, inMap);
    return node;
}

void printPostOrder(Node* root) {
    if (!root) return;
    printPostOrder(root->left);
    printPostOrder(root->right);
    cout << root->val << " ";
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> pre(n), in(n);
    for (int i = 0; i < n; i++) cin >> pre[i];
    unordered_map<int, int> inMap;
    for (int i = 0; i < n; i++) { cin >> in[i]; inMap[in[i]] = i; }

    int preIdx = 0;
    Node* root = buildTree(pre, in, 0, n - 1, preIdx, inMap);
    printPostOrder(root);
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Reconstruct a binary tree given its preorder and inorder traversals, and output its postorder traversal (CSES 1702).
// - Approach: Divide & Conquer Tree Reconstruction + Postorder DFS.
// - Intuition: The first element of preorder is the root; locating its index in the inorder traversal partitions left and right subtrees.
// - Complexity: Time: O(N), Space: O(N).
