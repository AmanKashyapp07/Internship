#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 5 (Problems 01 to 17)
// ============================================================================

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);
    if (!left) return right;
    if (!right) return left;
    return root; // p and q found in separate subtrees -> root is LCA
}
// Interview Explanation:
// - Problem Statement: Find the lowest common ancestor (LCA) node of two given nodes p and q in a binary tree.
// - Approach: Recursive DFS traversal.
// - Intuition: If the current root is null, p, or q, return root. Recurse on left and right subtrees. If both return non-null, p and q reside in different subtrees and `root` is their LCA; otherwise, pass through the non-null child.
// - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack.

TreeNode* mapParentsAndFindTarget(TreeNode* root, unordered_map<TreeNode*, TreeNode*>& parent, int start) {
    queue<TreeNode*> q;
    q.push(root);
    TreeNode *target_node = nullptr;

    while (!q.empty()) {
        TreeNode *curr = q.front();
        q.pop();
        if (curr->val == start) target_node = curr;
        if (curr->left) {
            parent[curr->left] = curr;
            q.push(curr->left);
        }
        if (curr->right) {
            parent[curr->right] = curr;
            q.push(curr->right);
        }
    }
    return target_node;
}

int minTimeToBurnTree(TreeNode* root, int start) {
    if (!root) return 0;
    unordered_map<TreeNode*, TreeNode*> parent;
    TreeNode *target = mapParentsAndFindTarget(root, parent, start);
    if (!target) return 0;

    unordered_set<TreeNode*> visited;
    queue<TreeNode*> q;
    q.push(target);
    visited.insert(target);

    int time = 0;
    while (!q.empty()) {
        int sz = q.size();
        bool spread = false;
        for (int i = 0; i < sz; i++) {
            TreeNode *curr = q.front();
            q.pop();

            if (curr->left && !visited.count(curr->left)) {
                visited.insert(curr->left);
                q.push(curr->left);
                spread = true;
            }
            if (curr->right && !visited.count(curr->right)) {
                visited.insert(curr->right);
                q.push(curr->right);
                spread = true;
            }
            if (parent.count(curr) && !visited.count(parent[curr])) {
                visited.insert(parent[curr]);
                q.push(parent[curr]);
                spread = true;
            }
        }
        if (spread) time++;
    }
    return time;
}
// Interview Explanation:
// - Problem Statement: Calculate the minimum time required to burn an entire binary tree starting from a given target node.
// - Approach: Parent Mapping BFS + Multi-directional BFS fire propagation.
// - Intuition: Fire spreads to 3 neighbors at each step: `left`, `right`, and `parent`. First map every node to its parent using BFS to convert the tree into an undirected graph, then run BFS from `target` tracking elapsed time per level.
// - Complexity: Time: O(N) two BFS passes, Space: O(N) parent map and visited set.

TreeNode* buildTreeHelper(const vi& preorder, int pre_start, int pre_end,
                         int in_start, int in_end, unordered_map<int, int>& in_map) {
    if (pre_start > pre_end || in_start > in_end) return nullptr;

    int root_val = preorder[pre_start];
    TreeNode *root = new TreeNode(root_val);

    int in_root_idx = in_map[root_val];
    int left_size = in_root_idx - in_start;

    root->left = buildTreeHelper(preorder, pre_start + 1, pre_start + left_size,
                                 in_start, in_root_idx - 1, in_map);
    root->right = buildTreeHelper(preorder, pre_start + left_size + 1, pre_end,
                                  in_root_idx + 1, in_end, in_map);

    return root;
}

TreeNode* buildTree(vi& preorder, vi& inorder) {
    unordered_map<int, int> in_map;
    for (int i = 0; i < (int)inorder.size(); i++) {
        in_map[inorder[i]] = i;
    }
    return buildTreeHelper(preorder, 0, preorder.size() - 1, 0, inorder.size() - 1, in_map);
}
// Interview Explanation:
// - Problem Statement: Construct and return the unique binary tree given its preorder and inorder traversal arrays.
// - Approach: Divide and Conquer with Hash Map index lookup for Inorder traversal.
// - Intuition: `preorder[0]` is always the root. Locate its index in `inorder` to determine the exact number of nodes in the left subtree (`left_size = in_root - in_start`). Recursively partition left and right subtrees.
// - Complexity: Time: O(N) with O(1) hash map lookups, Space: O(N) hash map and recursion stack.

vi morrisPreorderTraversal(TreeNode* root) {
    vi preorder;
    TreeNode *curr = root;

    while (curr) {
        if (!curr->left) {
            preorder.push_back(curr->val);
            curr = curr->right;
        } else {
            TreeNode *prev = curr->left;
            while (prev->right && prev->right != curr) {
                prev = prev->right;
            }

            if (!prev->right) {
                prev->right = curr;            // create temporary thread
                preorder.push_back(curr->val); // visit root in preorder
                curr = curr->left;
            } else {
                prev->right = nullptr;         // destroy thread
                curr = curr->right;
            }
        }
    }

    return preorder;
}
// Interview Explanation:
// - Problem Statement: Perform Preorder Traversal of a binary tree in O(N) time and O(1) auxiliary space without recursion or stack.
// - Approach: Morris Traversal using temporary right-child threading on the inorder predecessor.
// - Intuition: For node `curr` with a left child, find its inorder predecessor (rightmost node in left subtree). If thread doesn't exist, record `curr->val`, create thread to `curr`, and move left. If thread already exists, cut thread and move right.
// - Complexity: Time: O(N) each edge traversed at most twice, Space: O(1) auxiliary space.

TreeNode* findMinNode(TreeNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // Case 1 & 2: 0 or 1 child
        if (!root->left) {
            TreeNode *temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            TreeNode *temp = root->left;
            delete root;
            return temp;
        }
        // Case 3: 2 children -> replace with inorder successor (min of right subtree)
        TreeNode *succ = findMinNode(root->right);
        root->val = succ->val;
        root->right = deleteNode(root->right, succ->val);
    }
    return root;
}
// Interview Explanation:
// - Problem Statement: Delete a node with given key in a Binary Search Tree (BST) while maintaining BST properties.
// - Approach: Recursive BST search and deletion handling 3 structural cases.
// - Intuition: Navigate BST to locate node. If 0 or 1 child, replace node with its non-null child. If 2 children, find its inorder successor (smallest node in right subtree), copy its value into current node, and recursively delete the successor from the right subtree.
// - Complexity: Time: O(H) where H is tree height, Space: O(H) recursion stack.

TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val) {
            root = root->left;  // both in left subtree
        } else if (p->val > root->val && q->val > root->val) {
            root = root->right; // both in right subtree
        } else {
            return root;        // split point -> root is LCA
        }
    }
    return nullptr;
}
// Interview Explanation:
// - Problem Statement: Find the lowest common ancestor of two nodes in a BST in O(H) time and O(1) space.
// - Approach: Iterative BST traversal based on value splitting.
// - Intuition: If both p and q are strictly smaller than `root`, LCA is in left subtree; if both are strictly greater, LCA is in right subtree. The first node where values split (or match `root`) is the LCA.
// - Complexity: Time: O(H) where H is height of BST, Space: O(1) iterative traversal.

class BSTIterator {
private:
    stack<TreeNode*> st;
    bool forward; // true for ascending (next), false for descending (before)

    void pushAll(TreeNode *node) {
        while (node) {
            st.push(node);
            node = forward ? node->left : node->right;
        }
    }

public:
    BSTIterator(TreeNode *root, bool is_forward) : forward(is_forward) {
        pushAll(root);
    }

    int next() {
        TreeNode *top_node = st.top();
        st.pop();
        if (forward) pushAll(top_node->right);
        else pushAll(top_node->left);
        return top_node->val;
    }
};

bool findTarget(TreeNode* root, int k) {
    if (!root) return false;
    BSTIterator l(root, true);  // ascending iterator (left to right)
    BSTIterator r(root, false); // descending iterator (right to left)

    int left_val = l.next();
    int right_val = r.next();

    while (left_val < right_val) {
        int sum = left_val + right_val;
        if (sum == k) return true;
        if (sum < k) left_val = l.next();
        else right_val = r.next();
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if there exist two nodes in a BST whose sum equals target k.
// - Approach: Dual BST Iterators simulating Two Pointers (ascending & descending) on BST in-place.
// - Intuition: Instead of storing O(N) inorder array, use two custom stack-based iterators: one for `next()` (inorder) and one for `before()` (reverse inorder). Run two-pointer convergence in O(H) space.
// - Complexity: Time: O(N) amortized linear scan, Space: O(H) stack space for iterators (optimal vs O(N) array).

struct BSTNodeInfo {
    bool is_bst;
    int size;
    int min_val;
    int max_val;
};

BSTNodeInfo largestBSTHelper(TreeNode* root, int& max_bst_size) {
    if (!root) return {true, 0, INT_MAX, INT_MIN};

    auto left = largestBSTHelper(root->left, max_bst_size);
    auto right = largestBSTHelper(root->right, max_bst_size);

    if (left.is_bst && right.is_bst && root->val > left.max_val && root->val < right.min_val) {
        int current_size = 1 + left.size + right.size;
        max_bst_size = max(max_bst_size, current_size);
        return {true, current_size, min(root->val, left.min_val), max(root->val, right.max_val)};
    }

    return {false, max(left.size, right.size), 0, 0};
}

int largestBST(TreeNode* root) {
    int max_bst_size = 0;
    largestBSTHelper(root, max_bst_size);
    return max_bst_size;
}
// Interview Explanation:
// - Problem Statement: Find the size of the largest subtree in a binary tree that is also a valid Binary Search Tree (BST).
// - Approach: Post-Order DFS returning `{is_bst, size, min_val, max_val}` metadata.
// - Intuition: A subtree at `root` is a BST if and only if: left subtree is BST, right subtree is BST, and `left.max_val < root->val < right.min_val`. Passing bounds upwards allows O(1) validation per node.
// - Complexity: Time: O(N) single post-order pass, Space: O(H) recursion stack.

int orangesRotting(vvi& grid) {
    int m = grid.size(), n = grid[0].size();
    queue<pair<int, int>> q;
    int fresh_count = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 2) q.push({i, j});
            else if (grid[i][j] == 1) fresh_count++;
        }
    }

    if (fresh_count == 0) return 0;

    int minutes = 0;
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!q.empty()) {
        int sz = q.size();
        bool rotted_any = false;

        for (int i = 0; i < sz; i++) {
            auto [r, c] = q.front();
            q.pop();

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    grid[nr][nc] = 2;
                    fresh_count--;
                    q.push({nr, nc});
                    rotted_any = true;
                }
            }
        }
        if (rotted_any) minutes++;
    }

    return fresh_count == 0 ? minutes : -1;
}
// Interview Explanation:
// - Problem Statement: Find the minimum time required for all fresh oranges to rot, where rotten oranges rot 4-directionally adjacent fresh oranges every minute.
// - Approach: Multi-source Breadth-First Search (BFS).
// - Intuition: Enqueue all initially rotten oranges (level 0). Process layer by layer in BFS; rotting any fresh orange decrements `fresh_count` and pushes to queue. When queue is empty, if `fresh_count == 0`, return minutes, else return -1.
// - Complexity: Time: O(M * N) visiting each cell once, Space: O(M * N) queue capacity.

int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) return 0;

    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    dict.erase(beginWord);

    while (!q.empty()) {
        auto [word, len] = q.front();
        q.pop();

        if (word == endWord) return len;

        for (int i = 0; i < (int)word.size(); i++) {
            char original = word[i];
            for (char c = 'a'; c <= 'z'; c++) {
                word[i] = c;
                if (dict.count(word)) {
                    dict.erase(word); // mark visited to prevent cycles
                    q.push({word, len + 1});
                }
            }
            word[i] = original; // restore
        }
    }

    return 0;
}
// Interview Explanation:
// - Problem Statement: Find the shortest transformation sequence length from beginWord to endWord changing only one character at a time such that each word exists in wordList.
// - Approach: BFS for Shortest Path in an unweighted state-space graph.
// - Intuition: Model each word as a graph node with edges to words differing by 1 character. Level-by-level BFS from `beginWord` guarantees that the first time `endWord` is popped from queue, the path length is minimal.
// - Complexity: Time: O(N * L * 26) where N is number of words and L is word length, Space: O(N * L) for hash set and queue.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

void rotateMatrix(vvi& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    for (int i = 0; i < n; i++) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}
// Interview Explanation:
// - Problem Statement: Rotate an n x n 2D matrix by 90 degrees clockwise in-place.
// - Approach: Transpose matrix + Reverse each row.
// - Intuition: Clockwise 90-degree rotation maps `(i, j) -> (j, n - 1 - i)`. Transposing swaps `(i, j)` with `(j, i)`, and subsequently reversing each row maps `(j, i) -> (j, n - 1 - i)`, achieving in-place rotation.
// - Complexity: Time: O(N^2) in-place cell swaps, Space: O(1) in-place.

ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr, *curr = head;
    while (curr) {
        ListNode *next_temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_temp;
    }
    return prev;
}
// Interview Explanation:
// - Problem Statement: Reverse a singly linked list and return the new head.
// - Approach: Iterative 3-pointer reversal (`prev`, `curr`, `next_temp`).
// - Intuition: At each step, preserve `curr->next`, redirect `curr->next = prev`, advance `prev = curr`, and move `curr` forward. When `curr` is null, `prev` points to the new head.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place pointers.

ListNode* mergeTwoSortedLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);
    ListNode *tail = &dummy;

    while (list1 && list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    tail->next = list1 ? list1 : list2;
    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Merge two sorted linked lists into one sorted linked list.
// - Approach: Dummy head + Two pointer iteration.
// - Intuition: Compare current values of list1 and list2, attach the smaller node to `tail->next`, and advance that list. Append remaining elements in O(1) at the end.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.

ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto compare = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(compare)> min_heap(compare);

    for (ListNode* head : lists) {
        if (head) min_heap.push(head);
    }

    ListNode dummy(0);
    ListNode *tail = &dummy;

    while (!min_heap.empty()) {
        ListNode *smallest = min_heap.top();
        min_heap.pop();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next) {
            min_heap.push(smallest->next);
        }
    }

    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Merge k sorted linked lists into one sorted linked list and return its head.
// - Approach: Min-Heap Priority Queue holding heads of all k lists.
// - Intuition: Min-heap of size k always yields the smallest remaining element among all lists in O(log k) time. After popping the smallest node, push its `next` node into the heap.
// - Complexity: Time: O(N log K) where N is total nodes and K is number of lists, Space: O(K) min-heap.

void reorderList(ListNode* head) {
    if (!head || !head->next) return;

    // 1. Find midpoint
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Reverse second half
    ListNode *prev = nullptr, *curr = slow->next;
    slow->next = nullptr;
    while (curr) {
        ListNode *nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }

    // 3. Merge two halves alternatingly
    ListNode *first = head, *second = prev;
    while (second) {
        ListNode *tmp1 = first->next, *tmp2 = second->next;
        first->next = second;
        second->next = tmp1;
        first = tmp1;
        second = tmp2;
    }
}
// Interview Explanation:
// - Problem Statement: Reorder linked list to L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 ... in-place.
// - Approach: Midpoint splitting + Second half reversal + Interleaving merge.
// - Intuition: Find middle using fast/slow pointers, split list into two halves, reverse the second half, and interleave nodes from first and second halves.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place pointers.

vi topKFrequent(vi& nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    int n = nums.size();
    vector<vi> buckets(n + 1);
    for (auto& [val, count] : freq) {
        buckets[count].push_back(val);
    }

    vi ans;
    for (int i = n; i >= 0 && (int)ans.size() < k; i--) {
        for (int x : buckets[i]) {
            ans.push_back(x);
            if ((int)ans.size() == k) break;
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Given an integer array nums and an integer k, return the k most frequent elements in O(N) time.
// - Approach: Bucket Sort by frequency (optimal O(N) vs O(N log K) heap).
// - Intuition: Frequencies range from 1 to N. Create buckets `buckets[freq]`. Populate numbers into their frequency bucket and scan backwards from bucket N down to 1 to collect top k frequent elements.
// - Complexity: Time: O(N) strictly linear, Space: O(N) buckets and hash map.

class MedianFinder {
private:
    priority_queue<int> max_heap; // smaller half
    priority_queue<int, vi, greater<int>> min_heap; // larger half

public:
    MedianFinder() {}

    void addNum(int num) {
        max_heap.push(num);
        min_heap.push(max_heap.top());
        max_heap.pop();

        if (min_heap.size() > max_heap.size()) {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }
    }

    double findMedian() {
        if (max_heap.size() > min_heap.size()) {
            return max_heap.top();
        }
        return (max_heap.top() + min_heap.top()) / 2.0;
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure that supports adding numbers from a data stream and finding the median dynamically in O(1) time.
// - Approach: Two Heaps (Max-Heap for lower half, Min-Heap for upper half).
// - Intuition: Max-heap stores smaller half of elements and Min-heap stores larger half. Keep heaps balanced such that `max_heap` has either equal or 1 more element than `min_heap`. The median is either `max_heap.top()` (odd total) or average of heap tops (even total).
// - Complexity: Time: O(log N) for `addNum`, O(1) for `findMedian`, Space: O(N) heap elements.
