#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int data;
    Node *left, *right;

    Node(int x) {
        data = x;
        left = right = nullptr;
    }
};

class Solution {
    bool leaf(Node* root) {
        return !root->left && !root->right;
    }

    void leftBoundary(Node* root, vector<int>& ans) {
        Node* cur = root->left;

        while (cur) {
            if (!leaf(cur))
                ans.push_back(cur->data);

            if (cur->left)
                cur = cur->left;
            else
                cur = cur->right;
        }
    }

    void rightBoundary(Node* root, vector<int>& ans) {
        Node* cur = root->right;
        vector<int> temp;

        while (cur) {
            if (!leaf(cur))
                temp.push_back(cur->data);

            if (cur->right)
                cur = cur->right;
            else
                cur = cur->left;
        }

        for (int i = temp.size() - 1; i >= 0; i--)
            ans.push_back(temp[i]);
    }

    void leaves(Node* root, vector<int>& ans) {
        if (leaf(root)) {
            ans.push_back(root->data);
            return;
        }

        if (root->left)
            leaves(root->left, ans);

        if (root->right)
            leaves(root->right, ans);
    }

public:
    vector<int> boundary(Node* root) {
        vector<int> ans;

        if (!root)
            return ans;

        if (!leaf(root))
            ans.push_back(root->data);

        leftBoundary(root, ans);
        leaves(root, ans);
        rightBoundary(root, ans);

        return ans;
    }
};

int main() {
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);

    Solution obj;

    vector<int> ans = obj.boundary(root);

    for (int x : ans)
        cout << x << " ";
    cout << '\n';

    return 0;
}