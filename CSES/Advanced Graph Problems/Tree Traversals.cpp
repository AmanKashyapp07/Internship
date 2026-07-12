#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

Node* buildTree(const vector<int>& preorder, const vector<int>& inorder, int inStart, int inEnd, int& preIndex, const map<int, int>& inMap) {
    if (inStart > inEnd) return nullptr;

    Node* node = new Node(preorder[preIndex++]);

    if (inStart == inEnd) return node;

    int inIndex = inMap.at(node->val);

    node->left = buildTree(preorder, inorder, inStart, inIndex - 1, preIndex, inMap);
    node->right = buildTree(preorder, inorder, inIndex + 1, inEnd, preIndex, inMap);

    return node;
}

vector<int> postorderTraversal(Node* root) {
    vector<int> result;
    if (!root) return result;

    stack<Node*> s;
    Node* current = root;

    while (current || !s.empty()) {
        while (current) {
            s.push(current);
            current = current->left;
        }
        Node* temp = s.top()->right;
        if (!temp) {
            temp = s.top();
            s.pop();
            result.push_back(temp->val);
            while (!s.empty() && temp == s.top()->right) {
                temp = s.top();
                s.pop();
                result.push_back(temp->val);
            }
        } else {
            current = temp;
        }
    }

    return result;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    vector<int> preorder(n), inorder(n);
    for (int i = 0; i < n; i++) {
        cin >> preorder[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> inorder[i];
    }


    int preIndex = 0;
    map<int, int> inMap;
    for (int i = 0; i < n; i++) {
        inMap[inorder[i]] = i;
    }
    Node* root = buildTree(preorder, inorder, 0, n - 1, preIndex, inMap);
    vector<int> postorder = postorderTraversal(root);

    for (int val : postorder) {
        cout << val << " ";
    }
    cout << "\n";

    return 0;
}
