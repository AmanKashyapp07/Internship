#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
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
#endif
using namespace std;

// ====================================================================================================
//                                    STANDARD DATA STRUCTURE DEFINITIONS
// ====================================================================================================

// Standard Binary Tree Node
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Node with Next Pointer (LC 116 / LC 117)
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node() : val(0), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};

// Node with Parent Pointer (LC 1650)
class ParentNode {
public:
    int val;
    ParentNode* left;
    ParentNode* right;
    ParentNode* parent;
    ParentNode(int _val) : val(_val), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Singly Linked List Node (LC 109)
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | LC #   | Core Pattern / Technique           | Time     | Space    |
 |----|---------------------------------------------|--------|------------------------------------|----------|----------|
 | 01 | Binary Tree Level Order Traversal           | LC 102 | BFS Queue snapshot with q.size()   | O(N)     | O(W)     |
 | 02 | Populating Next Right Pointers I & II       | LC 116 | Level-by-level O(1) space traversal| O(N)     | O(1)     |
 | 03 | Binary Tree Zigzag Level Order Traversal    | LC 103 | BFS + Direct Index Placement       | O(N)     | O(W)     |
 | 04 | Binary Tree Right/Left Side View            | LC 199 | DFS (Root->Right->Left) / BFS      | O(N)     | O(H)     |
 | 05 | Vertical Order Traversal of a Binary Tree   | LC 987 | BFS Coordinate Mapping + Sorting   | O(N logN)| O(N)     |
 | 06 | Symmetric Tree                              | LC 101 | Dual-Pointer Mirror DFS / BFS      | O(N)     | O(H)     |
 | 07 | LCA of a Binary Search Tree                 | LC 235 | BST Split-Value Partitioning       | O(H)     | O(1)     |
 | 08 | LCA of a Binary Tree                        | LC 236 | Postorder Divide-and-Conquer DFS   | O(N)     | O(H)     |
 | 09 | LCA with Parent Pointers                    | LC 1650| Linked List Intersection Cycle Ptr | O(H)     | O(1)     |
 | 10 | Validate Binary Search Tree                 | LC 98  | Node* Bound Range DFS / Inorder    | O(N)     | O(H)     |
 | 11 | Recover Binary Search Tree                  | LC 99  | Inorder Inversion Tracking         | O(N)     | O(H)/O(1)|
 | 12 | Inorder Successor / Predecessor in BST      | LC 285 | BST Search Path Tracing            | O(H)     | O(1)     |
 | 13 | Kth Smallest Element in a BST               | LC 230 | Iterative Inorder Stack (Early Exit)| O(H + k) | O(H)     |
 | 14 | Trim a Binary Search Tree                   | LC 669 | BST Recursive Branch Pruning       | O(N)     | O(H)     |
 | 15 | Construct Binary Tree from Pre & Inorder    | LC 105 | Preorder Root + Inorder Hash Split | O(N)     | O(N)     |
 | 16 | Construct Binary Tree from In & Postorder   | LC 106 | Postorder Root + Build Right First | O(N)     | O(N)     |
 | 17 | Serialize and Deserialize Binary Tree       | LC 297 | Preorder DFS with Sentinel Tokens  | O(N)     | O(N)     |
 | 18 | Convert Sorted List to BST                  | LC 109 | Simulated Inorder Traversal        | O(N)     | O(log N) |
 | 19 | Convert Sorted Array to BST                 | LC 108 | Divide-and-Conquer Midpoint Picking| O(N)     | O(log N) |
 | 20 | Binary Tree Maximum Path Sum                | LC 124 | Postorder Tree DP + Split Max Path | O(N)     | O(H)     |
 | 21 | Diameter of Binary Tree                     | LC 543 | Postorder Tree DP + Depth Return   | O(N)     | O(H)     |
 | 22 | Path Sum III                                | LC 437 | Prefix Sum Hash Map + Backtracking | O(N)     | O(H)     |
 | 23 | All Nodes Distance K in Binary Tree         | LC 863 | Parent Mapping + Radial BFS        | O(N)     | O(N)     |
 | 24 | Balanced Binary Tree                        | LC 110 | Bottom-Up Postorder Sentinel (-1)  | O(N)     | O(H)     |
 | 25 | Flatten Binary Tree to Linked List          | LC 114 | In-Place Predecessor Rewiring      | O(N)     | O(1)     |
 | 26 | Convert BST to Sorted Doubly Linked List    | LC 426 | Inorder Rewiring + Circular Close  | O(N)     | O(H)     |
 | 27 | Invert Binary Tree                          | LC 226 | Pre/Postorder Child Swapping       | O(N)     | O(H)     |
 ====================================================================================================
*/


// ====================================================================================================
// GROUP 1: TRAVERSAL & VIEW PATTERNS (BFS / DFS)
// ====================================================================================================

// =========================================================
// 1. BINARY TREE LEVEL ORDER TRAVERSAL (LC 102)
// =========================================================

class Solution1 {
public:
    // Iterative BFS: Standard, production-grade approach
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) return {};
        
        vector<vector<int>> result;
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size(); // Freeze size: processes exactly one level
            vector<int> currentLevel;
            currentLevel.reserve(levelSize);
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* curr = q.front();
                q.pop();
                
                currentLevel.push_back(curr->val);
                if (curr->left) q.push(curr->left);   // Enqueue left child for next level
                if (curr->right) q.push(curr->right); // Enqueue right child for next level
            }
            result.push_back(std::move(currentLevel));
        }
        return result;
    }

    // Recursive DFS Alternative
    void dfs(TreeNode* node, int level, vector<vector<int>>& result) {
        if (!node) return;
        if (level == (int)result.size()) {
            result.push_back({}); // First node encountered at this depth
        }
        result[level].push_back(node->val);
        dfs(node->left, level + 1, result);
        dfs(node->right, level + 1, result);
    }
    
    vector<vector<int>> levelOrderDFS(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return level-order traversal of nodes' values from left to right, level by level (LC 102).
// - Core Pattern: BFS using FIFO queue with a level-size snapshot (or DFS with depth indexing).
// - Intuition:
//   * Snapshot `q.size()` at the start of each while iteration to process all nodes belonging to the current level.
//   * Any children pushed during this iteration belong to the next level and will not be popped until the next round.
// - Complexity:
//   * Time: O(N) — Every node is pushed and popped exactly once.
//   * Space: O(W) = O(N) where W is max tree width (up to N/2 nodes at leaf level). DFS takes O(H) stack space.
// - Edge Cases: Empty tree (returns {}), single node, highly skewed tree (queue size never exceeds 1).
// - Likely Follow-ups:
//   1. How to return bottom-up level order (LC 107)? -> Reverse result at end or use std::deque.
//   2. How to stream nodes level-by-level across network sockets? -> Push delimiter/null tokens or serialize (val, depth).


// =========================================================
// 2. POPULATING NEXT RIGHT POINTERS IN EACH NODE I & II (LC 116 / LC 117)
// =========================================================

class Solution2 {
public:
    // Universal O(1) Auxiliary Space Solution (works for both LC 116 and LC 117)
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        Node* curr = root; // Start with head of current horizontal level
        
        while (curr) {
            Node dummy(0); // Sentinel head for stitching the next level's children
            Node* tail = &dummy;
            
            // Traverse current level horizontally via .next pointers
            while (curr) {
                if (curr->left) {
                    tail->next = curr->left; // Link child
                    tail = tail->next;       // Advance tail
                }
                if (curr->right) {
                    tail->next = curr->right;
                    tail = tail->next;
                }
                curr = curr->next; // Move horizontally across current level
            }
            
            // Descend to first child of the next level
            curr = dummy.next;
        }
        return root;
    }

    // LC 116 Only: Specialized for Perfect Binary Trees
    Node* connectPerfect(Node* root) {
        if (!root) return nullptr;
        Node* leftmost = root;
        
        while (leftmost->left) {
            Node* head = leftmost;
            while (head) {
                // Connection 1: Children of same parent
                head->left->next = head->right;
                // Connection 2: Cross-parent children across the gap
                if (head->next) {
                    head->right->next = head->next->left;
                }
                head = head->next;
            }
            leftmost = leftmost->left; // Descend to next level
        }
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Populate each node's `next` pointer to point to its next right node in O(1) extra space (LC 116 / 117).
// - Core Pattern: Level-by-level traversal treating the previous level as a linked list to wire the next level.
// - Intuition:
//   * A dummy node acts as the sentinel head for the next level. A `tail` pointer weaves children together.
//   * Traversal across `curr = curr->next` on the current level completely eliminates the need for an explicit BFS queue.
// - Complexity:
//   * Time: O(N) — Every node and child pointer is visited once.
//   * Space: O(1) auxiliary space — Zero recursion stack, zero heap allocation.
// - Edge Cases: Empty tree, single node, incomplete trees with missing subtrees (e.g., node has only right child).
// - Likely Follow-ups:
//   1. Can you use recursion? -> Yes, but recursion stack costs O(H) space, violating the strict O(1) requirement.
//   2. How does LC 116 differ? -> In LC 116, tree is perfect, so `curr->left->next = curr->right` without checking nulls.


// =========================================================
// 3. BINARY TREE ZIGZAG LEVEL ORDER TRAVERSAL (LC 103)
// =========================================================

class Solution3 {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root) return {};
        
        vector<vector<int>> result;
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel(levelSize); // Pre-allocate: enables direct index placement
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* curr = q.front();
                q.pop();
                
                // If leftToRight write from index i; else write from back (levelSize - 1 - i)
                int index = leftToRight ? i : (levelSize - 1 - i);
                currentLevel[index] = curr->val;
                
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            
            result.push_back(std::move(currentLevel));
            leftToRight = !leftToRight; // Alternate direction for subsequent level
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return zigzag level-order traversal (left-to-right, then right-to-left alternating) (LC 103).
// - Core Pattern: BFS queue with pre-allocated vector and direct index placement (avoids std::reverse).
// - Intuition:
//   * Pre-allocating `vector<int>(levelSize)` allows writing values in either direction with index math.
//   * This avoids extra memory copies and CPU branch prediction penalties compared to calling `std::reverse`.
// - Complexity:
//   * Time: O(N) — Every node processed once; each value written directly to its final destination.
//   * Space: O(W) = O(N) — Queue holds at most maximum level width.
// - Edge Cases: Single node, skewed tree, strictly alternating single-child branches.
// - Likely Follow-ups:
//   1. How to do with two stacks? -> Stack 1 pops L-to-R (pushes children L, then R to S2); S2 pops R-to-L (pushes R, then L to S1).
//   2. Why pre-allocate instead of deque? -> Vector pre-allocation is contiguous and cache-friendly; std::deque has chunked overhead.


// =========================================================
// 4. BINARY TREE RIGHT/LEFT SIDE VIEW (LC 199)
// =========================================================

class Solution4 {
    // DFS Reverse Preorder: Root -> Right -> Left
    void dfsRight(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        
        // The first node visited at this depth is guaranteed to be the rightmost visible node
        if (depth == (int)result.size()) {
            result.push_back(node->val);
        }
        
        dfsRight(node->right, depth + 1, result); // Prioritize right branch
        dfsRight(node->left, depth + 1, result);  // Left branch seen only if right is absent
    }

    // DFS Preorder for Left Side View: Root -> Left -> Right
    void dfsLeft(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        if (depth == (int)result.size()) {
            result.push_back(node->val);
        }
        dfsLeft(node->left, depth + 1, result);
        dfsLeft(node->right, depth + 1, result);
    }

public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfsRight(root, 0, result);
        return result;
    }

    vector<int> leftSideView(TreeNode* root) {
        vector<int> result;
        dfsLeft(root, 0, result);
        return result;
    }

    // BFS Alternative (takes the last element of each level)
    vector<int> rightSideViewBFS(TreeNode* root) {
        if (!root) return {};
        vector<int> result;
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* curr = q.front(); q.pop();
                if (i == levelSize - 1) result.push_back(curr->val); // Last node of current level
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return values visible when viewing the binary tree from the right (or left) side (LC 199).
// - Core Pattern: DFS (Root -> Right -> Left) tracking recursion depth vs result.size() (or BFS taking level's last node).
// - Intuition:
//   * If visiting right subtrees first, the first node encountered at depth `d` must be the rightmost node at that level.
//   * A simple check `depth == result.size()` acts as an idempotent guard for recording visible nodes.
// - Complexity:
//   * Time: O(N) — Visits each node once.
//   * Space: O(H) call stack for DFS (O(log N) balanced, O(N) skewed). BFS uses O(W).
// - Edge Cases: Left subtree deeper than right subtree (left nodes become visible once right subtree terminates), empty tree.
// - Likely Follow-ups:
//   1. How to get Left Side View? -> Traverse Root -> Left -> Right.
//   2. What if tree is skewed with depth 10^5? -> DFS overflows the call stack; mention BFS queue with `q.back()`.


// =========================================================
// 5. VERTICAL ORDER TRAVERSAL OF A BINARY TREE (LC 987)
// =========================================================

class Solution5 {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        if (!root) return {};
        
        // col -> list of (row, val)
        map<int, vector<pair<int, int>>> colTable;
        
        // BFS Queue stores: {node, {row, col}}
        queue<pair<TreeNode*, pair<int, int>>> q;
        q.push({root, {0, 0}});
        
        while (!q.empty()) {
            auto [curr, coords] = q.front();
            q.pop();
            int row = coords.first;
            int col = coords.second;
            
            colTable[col].push_back({row, curr->val});
            
            if (curr->left)  q.push({curr->left,  {row + 1, col - 1}});
            if (curr->right) q.push({curr->right, {row + 1, col + 1}});
        }
        
        vector<vector<int>> result;
        for (auto& [col, nodes] : colTable) {
            // Sort primary by row (top to bottom); tie-break by node value (ascending)
            sort(nodes.begin(), nodes.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
                if (a.first != b.first) return a.first < b.first;
                return a.second < b.second;
            });
            
            vector<int> colVals;
            colVals.reserve(nodes.size());
            for (const auto& p : nodes) {
                colVals.push_back(p.second);
            }
            result.push_back(std::move(colVals));
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Vertical order traversal where ties at same (row, col) are sorted by value in ascending order (LC 987).
// - Core Pattern: BFS coordinate mapping `(row, col)` with ordered column map and custom sorting.
// - Intuition:
//   * Assign root coordinate (0, 0). Left child is `(row + 1, col - 1)`, right is `(row + 1, col + 1)`.
//   * Group all entries by `col`. For identical `(col, row)` cells, sort by `val` as explicitly specified in LC 987.
// - Complexity:
//   * Time: O(N log N) — Map insertion and sorting multi-node overlaps.
//   * Space: O(N) — Storing coordinates and nodes in map and queue.
// - Edge Cases: Overlapping nodes at identical (row, col) with duplicate values, strictly skewed trees.
// - Likely Follow-ups:
//   1. How does this differ from LC 314? -> LC 314 preserves BFS insertion order without sorting by value on ties.
//   2. Can we eliminate std::map to drop log K factor? -> Pre-pass to find minCol and maxCol, then use array of vectors.


// =========================================================
// 6. SYMMETRIC TREE (LC 101)
// =========================================================

class Solution6 {
    // Helper: Validates mirror symmetry between two subtrees
    bool isMirror(TreeNode* t1, TreeNode* t2) {
        if (!t1 && !t2) return true;   // Both null -> symmetric
        if (!t1 || !t2) return false;  // One null, one present -> asymmetric
        if (t1->val != t2->val) return false;
        
        // Outer pairs match AND inner pairs match
        return isMirror(t1->left, t2->right) && isMirror(t1->right, t2->left);
    }

public:
    // Recursive DFS
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isMirror(root->left, root->right);
    }

    // Iterative BFS Alternative (Demonstrates explicit queue handling)
    bool isSymmetricIterative(TreeNode* root) {
        if (!root) return true;
        queue<TreeNode*> q;
        q.push(root->left);
        q.push(root->right);
        
        while (!q.empty()) {
            TreeNode* t1 = q.front(); q.pop();
            TreeNode* t2 = q.front(); q.pop();
            
            if (!t1 && !t2) continue;
            if (!t1 || !t2 || t1->val != t2->val) return false;
            
            q.push(t1->left);  q.push(t2->right); // Mirror outer
            q.push(t1->right); q.push(t2->left);  // Mirror inner
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Check whether a binary tree is symmetric around its vertical axis (mirror of itself) (LC 101).
// - Core Pattern: Dual-pointer mirror DFS/BFS checking `t1->val == t2->val && mirror(left, right) && mirror(right, left)`.
// - Intuition:
//   * Two trees are mirrors if their roots are equal and left subtree of one mirrors right subtree of the other.
// - Complexity:
//   * Time: O(N) — Every node visited at most once.
//   * Space: O(H) recursion stack (O(N) skewed). Iterative BFS uses O(N) queue space.
// - Edge Cases: Root only (true), asymmetric structure with identical values, mirror structure with mismatched values.
// - Likely Follow-ups:
//   1. Can you solve in O(1) auxiliary space? -> Not without destructive dual Morris traversal.
//   2. How to test N-ary tree symmetry? -> Check `children[i]` with `children[m - 1 - i]` for all i.


// ====================================================================================================
// GROUP 2: LOWEST COMMON ANCESTOR (LCA)
// ====================================================================================================

// =========================================================
// 7. LCA OF A BINARY SEARCH TREE (LC 235)
// =========================================================

class Solution7 {
public:
    // Iterative: O(1) Auxiliary Space (Best interview implementation)
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        TreeNode* curr = root;
        while (curr) {
            // Both nodes in left subtree
            if (p->val < curr->val && q->val < curr->val) {
                curr = curr->left;
            }
            // Both nodes in right subtree
            else if (p->val > curr->val && q->val > curr->val) {
                curr = curr->right;
            }
            // Split point found: one is on left, one on right, OR curr == p/q
            else {
                return curr;
            }
        }
        return nullptr;
    }

    // Recursive DFS Alternative
    TreeNode* lowestCommonAncestorRecursive(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) return nullptr;
        if (p->val < root->val && q->val < root->val) return lowestCommonAncestorRecursive(root->left, p, q);
        if (p->val > root->val && q->val > root->val) return lowestCommonAncestorRecursive(root->right, p, q);
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Find LCA of two nodes p and q in a Binary Search Tree (LC 235).
// - Core Pattern: BST Split-Value Partitioning.
// - Intuition:
//   * BST property guarantees that as long as both p and q are on the same side of curr, LCA lies in that subtree.
//   * The first node where p and q split (or where curr matches p or q) is the LCA.
// - Complexity:
//   * Time: O(H) — Follows a single downward path; O(log N) balanced, O(N) skewed.
//   * Space: O(1) auxiliary space for iterative; O(H) stack space for recursive.
// - Edge Cases: p is ancestor of q (returns p), p and q are direct siblings, nodes at tree extremes.
// - Likely Follow-ups:
//   1. What if p or q might not exist in the BST? -> Verify existence via binary search prior to returning split node.
//   2. What if duplicate values exist in the BST? -> Clarify whether duplicates go left or right; adjust strict `<` to `<=`.


// =========================================================
// 8. LCA OF A BINARY TREE (LC 236)
// =========================================================

class Solution8 {
public:
    // Postorder Bottom-Up Divide and Conquer
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // Base case: null or found target node
        if (!root || root == p || root == q) {
            return root;
        }
        
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        // If both subtrees returned non-null, root is the LCA
        if (left && right) {
            return root;
        }
        
        // Bubble up non-null result (or null if neither found)
        return left ? left : right;
    }
};
// Interview Explanation:
// - Problem Statement: Find LCA of nodes p and q in a generic binary tree (LC 236).
// - Core Pattern: Postorder DFS (Bottom-Up Divide and Conquer).
// - Intuition:
//   * If a subtree contains neither p nor q, return null.
//   * If it contains p or q, return that node pointer.
//   * When a node receives non-null pointers from BOTH left and right branches, it is the unique lowest common ancestor.
// - Complexity:
//   * Time: O(N) — Visits all nodes in worst case.
//   * Space: O(H) recursion stack (O(N) for skewed tree).
// - Edge Cases: p is direct parent of q (returns p immediately), p and q in opposite subtrees.
// - Likely Follow-ups:
//   1. What if p or q may NOT exist in the tree (LC 1644)? -> Track an integer count of targets seen before confirming LCA.
//   2. How to solve iteratively? -> Store parent pointers in a hash map, build ancestor path set for p, traverse q up.


// =========================================================
// 9. LCA WITH PARENT POINTERS (LC 1650)
// =========================================================

class Solution9 {
public:
    // O(1) Auxiliary Space: Reduction to Linked List Cycle / Intersection
    ParentNode* lowestCommonAncestor(ParentNode* p, ParentNode* q) {
        ParentNode* a = p;
        ParentNode* b = q;
        
        // Two-pointer switch: both traverse depth(p) + depth(q) and meet at LCA
        while (a != b) {
            a = (a == nullptr) ? q : a->parent;
            b = (b == nullptr) ? p : b->parent;
        }
        return a;
    }

    // Hash Set Alternative (O(H) Space)
    ParentNode* lowestCommonAncestorHashSet(ParentNode* p, ParentNode* q) {
        unordered_set<ParentNode*> ancestors;
        while (p) {
            ancestors.insert(p);
            p = p->parent;
        }
        while (q) {
            if (ancestors.count(q)) return q;
            q = q->parent;
        }
        return nullptr;
    }
};
// Interview Explanation:
// - Problem Statement: Find LCA of nodes p and q given parent pointers; root is not provided (LC 1650).
// - Core Pattern: Two-Pointer Linked List Intersection (Floyd's pointer redirection).
// - Intuition:
//   * Walking up via `parent` turns the problem into finding the intersection node of two converging linked lists.
//   * When pointer `a` reaches null, redirect to `q`; when `b` reaches null, redirect to `p`. Both traverse equal total distance.
// - Complexity:
//   * Time: O(H) where H is tree height.
//   * Space: O(1) auxiliary space (vs O(H) for hash set approach).
// - Edge Cases: p == q, one node is root, one node is ancestor of the other.
// - Likely Follow-ups:
//   1. What if nodes belong to different disconnected trees? -> Both pointers hit nullptr at step depth(p)+depth(q) and return null.
//   2. Can we compute depths explicitly? -> Yes: find depth(p) and depth(q), advance deeper node by difference, then step together.


// ====================================================================================================
// GROUP 3: BST PROPERTIES & VALIDATION
// ====================================================================================================

// =========================================================
// 10. VALIDATE BINARY SEARCH TREE (LC 98)
// =========================================================

class Solution10 {
    // DFS with Node* bounds (safe against INT_MIN / INT_MAX overflow)
    bool isValidHelper(TreeNode* node, TreeNode* minNode, TreeNode* maxNode) {
        if (!node) return true;
        
        // Strict inequality check
        if (minNode && node->val <= minNode->val) return false;
        if (maxNode && node->val >= maxNode->val) return false;
        
        return isValidHelper(node->left, minNode, node) &&
               isValidHelper(node->right, node, maxNode);
    }

public:
    bool isValidBST(TreeNode* root) {
        return isValidHelper(root, nullptr, nullptr);
    }

    // Iterative Inorder Traversal Check
    bool isValidBSTInorder(TreeNode* root) {
        stack<TreeNode*> st;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        
        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top(); st.pop();
            
            if (prev && curr->val <= prev->val) return false; // Inorder must be strictly increasing
            prev = curr;
            
            curr = curr->right;
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if a binary tree is a valid BST (strict ordering: left < root < right) (LC 98).
// - Core Pattern: Range bounding using TreeNode* bounds (or Iterative Inorder strictly increasing check).
// - Intuition:
//   * Never bound using raw `INT_MIN` / `INT_MAX` integers (fails when node value is INT_MIN).
//   * Using `TreeNode* minNode, maxNode` pointers makes null represent unbounded limits without arithmetic overflow.
// - Complexity:
//   * Time: O(N) — Every node visited once.
//   * Space: O(H) recursion/stack space (O(N) for skewed tree). Can be O(1) via Morris traversal.
// - Edge Cases: Node values equal to INT_MIN or INT_MAX, duplicate values (invalid in strict BST).
// - Likely Follow-ups:
//   1. Can you validate in O(1) space? -> Yes, with Morris Inorder traversal tracking `prev`.
//   2. Why not just check `node->left->val < node->val`? -> Local checks miss global violations across higher ancestors.


// =========================================================
// 11. RECOVER BINARY SEARCH TREE (LC 99)
// =========================================================

class Solution11 {
    TreeNode* first = nullptr;
    TreeNode* second = nullptr;
    TreeNode* prev = nullptr;

    void inorder(TreeNode* curr) {
        if (!curr) return;
        
        inorder(curr->left);
        
        // Detect anomaly: prev should be strictly smaller than curr
        if (prev && prev->val >= curr->val) {
            if (!first) {
                first = prev; // First violation: larger swapped node is prev
            }
            second = curr;    // Second violation (or adjacent): smaller swapped node is curr
        }
        prev = curr;
        
        inorder(curr->right);
    }

public:
    void recoverTree(TreeNode* root) {
        first = second = prev = nullptr;
        inorder(root);
        if (first && second) {
            swap(first->val, second->val);
        }
    }

    // Morris Traversal: Strictly O(1) Auxiliary Space
    void recoverTreeMorris(TreeNode* root) {
        TreeNode *firstNode = nullptr, *secondNode = nullptr, *prevNode = nullptr;
        TreeNode *curr = root;
        
        while (curr) {
            if (!curr->left) {
                // Visit curr
                if (prevNode && prevNode->val >= curr->val) {
                    if (!firstNode) firstNode = prevNode;
                    secondNode = curr;
                }
                prevNode = curr;
                curr = curr->right;
            } else {
                // Find inorder predecessor
                TreeNode* pred = curr->left;
                while (pred->right && pred->right != curr) {
                    pred = pred->right;
                }
                
                if (!pred->right) {
                    pred->right = curr; // Establish thread
                    curr = curr->left;
                } else {
                    pred->right = nullptr; // Tear down thread
                    if (prevNode && prevNode->val >= curr->val) {
                        if (!firstNode) firstNode = prevNode;
                        secondNode = curr;
                    }
                    prevNode = curr;
                    curr = curr->right;
                }
            }
        }
        if (firstNode && secondNode) swap(firstNode->val, secondNode->val);
    }
};
// Interview Explanation:
// - Problem Statement: Recover a BST where exactly two nodes were swapped by mistake without changing structure (LC 99).
// - Core Pattern: Inorder anomaly detection (in sorted array, swapped elements cause 1 or 2 inversions).
// - Intuition:
//   * For sorted sequence `[1, 2, 3, 4, 5]`, swapping 2 and 5 gives `[1, 5, 3, 4, 2]`.
//   * Inversions occur at (5 > 3) and (4 > 2). `first` is 5 (prev on 1st anomaly), `second` is 2 (curr on 2nd anomaly).
// - Complexity:
//   * Time: O(N) — Single inorder pass.
//   * Space: O(H) stack space for standard inorder; O(1) space using Morris traversal.
// - Edge Cases: Swapped nodes are adjacent in inorder sequence (only one anomaly triggered; first = prev, second = curr).
// - Likely Follow-ups:
//   1. How to achieve strictly O(1) space? -> Morris Inorder Traversal as implemented above.
//   2. What if values cannot be swapped directly (nodes immutable)? -> In-place pointer rewiring required.


// =========================================================
// 12. INORDER SUCCESSOR / PREDECESSOR IN BST (LC 285)
// =========================================================

class Solution12 {
public:
    // Inorder Successor: Smallest node strictly greater than p->val
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        TreeNode* successor = nullptr;
        TreeNode* curr = root;
        
        while (curr) {
            if (p->val < curr->val) {
                successor = curr;   // curr is a candidate; check left for a smaller one
                curr = curr->left;
            } else {
                curr = curr->right; // successor must be in right subtree
            }
        }
        return successor;
    }

    // Inorder Predecessor: Greatest node strictly smaller than p->val
    TreeNode* inorderPredecessor(TreeNode* root, TreeNode* p) {
        TreeNode* predecessor = nullptr;
        TreeNode* curr = root;
        
        while (curr) {
            if (p->val > curr->val) {
                predecessor = curr; // curr is a candidate; check right for a larger one
                curr = curr->right;
            } else {
                curr = curr->left;  // predecessor must be in left subtree
            }
        }
        return predecessor;
    }
};
// Interview Explanation:
// - Problem Statement: Find inorder successor (or predecessor) of node p in a BST (LC 285).
// - Core Pattern: BST Search Path Comparison (prunes half of the tree at each step).
// - Intuition:
//   * Successor: If `p->val < curr->val`, curr is a valid candidate, and we explore `curr->left` for a closer one.
//   * If `p->val >= curr->val`, curr cannot be a successor, so explore `curr->right`.
// - Complexity:
//   * Time: O(H) — Follows a single path down the tree; O(log N) balanced, O(N) skewed.
//   * Space: O(1) auxiliary space.
// - Edge Cases: p has right subtree (successor is leftmost child in right subtree), p is tree max (returns null).
// - Likely Follow-ups:
//   1. What if parent pointers exist? -> If right subtree exists, go right then leftmost. Else walk up parents until node is left child.
//   2. What if tree is NOT a BST? -> Inorder traversal with a boolean flag tracking when p is seen (O(N) time).


// =========================================================
// 13. KTH SMALLEST ELEMENT IN A BST (LC 230)
// =========================================================

class Solution13 {
public:
    // Iterative Stack: Early exit immediately once kth element is reached
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        TreeNode* curr = root;
        
        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left; // Reach leftmost element
            }
            
            curr = st.top();
            st.pop();
            
            if (--k == 0) {
                return curr->val; // Early exit without visiting remaining N - k nodes
            }
            
            curr = curr->right;
        }
        return -1;
    }
};

class OrderStatisticBST {
public:
    struct Node {
        int val;
        int subtreeSize;
        Node *left, *right;

        Node(int val) : val(val), subtreeSize(1), left(nullptr), right(nullptr) {}
    };

private:
    int size(Node* root) {
        return root ? root->subtreeSize : 0;
    }

    void update(Node* root) {
        if (root)
            root->subtreeSize = 1 + size(root->left) + size(root->right);
    }

    Node* insert(Node* root, int val) {
        if (!root)
            return new Node(val);

        if (val < root->val)
            root->left = insert(root->left, val);
        else
            root->right = insert(root->right, val);

        update(root);
        return root;
    }

    Node* getMin(Node* root) {
        while (root->left)
            root = root->left;
        return root;
    }

    Node* erase(Node* root, int val) {
        if (!root)
            return nullptr;

        if (val < root->val) {
            root->left = erase(root->left, val);
        } 
        else if (val > root->val) {
            root->right = erase(root->right, val);
        } 
        else {
            if (!root->left) {
                Node* right = root->right;
                delete root;
                return right;
            }

            if (!root->right) {
                Node* left = root->left;
                delete root;
                return left;
            }

            // Two children: replace with inorder successor
            Node* successor = getMin(root->right);
            root->val = successor->val;
            root->right = erase(root->right, successor->val);
        }

        update(root);
        return root;
    }

    int kthSmallest(Node* root, int k) {
        if (!root || k <= 0 || k > size(root))
            return -1;

        int leftSize = size(root->left);

        if (k == leftSize + 1)
            return root->val;

        if (k <= leftSize)
            return kthSmallest(root->left, k);

        return kthSmallest(root->right, k - leftSize - 1);
    }

public:
    Node* root = nullptr;

    void insert(int val) {
        root = insert(root, val);
    }

    void erase(int val) {
        root = erase(root, val);
    }

    int kthSmallest(int k) {
        return kthSmallest(root, k);
    }
};
// Interview Explanation:
// - Problem Statement: Return kth smallest value (1-indexed) in a BST (LC 230).
// - Core Pattern: Inorder Traversal with explicit stack and counter early termination.
// - Intuition:
//   * BST Inorder traversal yields keys in ascending sorted order.
//   * An iterative stack stops immediately at count k, avoiding the need to traverse the remaining N - k nodes.
// - Complexity:
//   * Time: O(H + k) — Moves down to minimum node (H), then steps k times.
//   * Space: O(H) stack space.
// - Edge Cases: k = 1 (minimum element), k = N (maximum element), completely skewed tree.
// - Likely Follow-ups:
//   1. What if BST undergoes frequent insertions/deletions? -> Augment nodes with `int subtreeSize` (Order-Statistic Tree) for O(H) queries.
//   2. How to solve in O(1) space? -> Morris Inorder Traversal with early return once thread restored.


// =========================================================
// 14. TRIM A BINARY SEARCH TREE (LC 669)
// =========================================================

class Solution14 {
public:
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        if (!root) return nullptr;
        
        // If root->val is too small, entire left subtree is also < low -> prune it
        if (root->val < low) {
            return trimBST(root->right, low, high);
        }
        
        // If root->val is too large, entire right subtree is also > high -> prune it
        if (root->val > high) {
            return trimBST(root->left, low, high);
        }
        
        // root is valid; recursively trim both left and right subtrees
        root->left = trimBST(root->left, low, high);
        root->right = trimBST(root->right, low, high);
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Trim BST so that all remaining elements lie within [low, high] (LC 669).
// - Core Pattern: Recursive Subtree Pruning using BST ordering property.
// - Intuition:
//   * If `root->val < low`, every node in left subtree is strictly less than low, so replace root with trimmed right child.
//   * If `root->val > high`, every node in right subtree is strictly greater than high, so replace root with trimmed left child.
// - Complexity:
//   * Time: O(N) — Every node visited at most once.
//   * Space: O(H) recursion stack.
// - Edge Cases: Root itself is outside [low, high], all nodes pruned (returns null), no nodes pruned.
// - Likely Follow-ups:
//   1. Memory leak in C++? -> In production, deleted subtrees should be cleaned up via `deleteSubtree()` helper.
//   2. Can we trim iteratively? -> Yes: find new root in range, then trim left and right downward paths iteratively.


// ====================================================================================================
// GROUP 4: TREE CONSTRUCTION & SERIALIZATION
// ====================================================================================================

// =========================================================
// 15. CONSTRUCT BINARY TREE FROM PREORDER AND INORDER TRAVERSAL (LC 105)
// =========================================================

class Solution15 {
    unordered_map<int, int> inMap; // val -> index in inorder array
    int preIndex = 0;

    TreeNode* build(const vector<int>& preorder, int inStart, int inEnd) {
        if (inStart > inEnd) return nullptr;
        
        int rootVal = preorder[preIndex++];
        TreeNode* root = new TreeNode(rootVal);
        int inIndex = inMap[rootVal]; // Subtree split point
        
        // Preorder is Root -> Left -> Right, so build left subtree FIRST
        root->left = build(preorder, inStart, inIndex - 1);
        root->right = build(preorder, inIndex + 1, inEnd);
        
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        preIndex = 0;
        inMap.clear();
        for (int i = 0; i < (int)inorder.size(); ++i) {
            inMap[inorder[i]] = i;
        }
        return build(preorder, 0, inorder.size() - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Construct binary tree from preorder and inorder traversals with unique values (LC 105).
// - Core Pattern: Preorder picks the root; Inorder splits left/right subtrees. O(1) hash map lookup.
// - Intuition:
//   * First element in preorder is always the root.
//   * Locate root in inorder array: elements to its left form left subtree, elements to its right form right subtree.
// - Complexity:
//   * Time: O(N) — Hash map gives O(1) index lookups; each node constructed once. (Without hash map: O(N^2)).
//   * Space: O(N) — Hash map storage plus O(H) recursion stack.
// - Edge Cases: Single node, skewed tree, empty inputs.
// - Likely Follow-ups:
//   1. What if duplicates exist? -> Tree cannot be uniquely determined.
//   2. Can we do O(1) auxiliary space? -> Iterative stack solution matching inorder pointer (O(H) stack, O(1) extra).


// =========================================================
// 16. CONSTRUCT BINARY TREE FROM INORDER AND POSTORDER TRAVERSAL (LC 106)
// =========================================================

class Solution16 {
    unordered_map<int, int> inMap;
    int postIndex = 0;

    TreeNode* build(const vector<int>& postorder, int inStart, int inEnd) {
        if (inStart > inEnd) return nullptr;
        
        int rootVal = postorder[postIndex--];
        TreeNode* root = new TreeNode(rootVal);
        int inIndex = inMap[rootVal];
        
        // CRITICAL: Postorder is Left -> Right -> Root
        // Walking backwards through postorder gives: Root -> RIGHT -> LEFT
        root->right = build(postorder, inIndex + 1, inEnd);
        root->left = build(postorder, inStart, inIndex - 1);
        
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        postIndex = (int)postorder.size() - 1;
        inMap.clear();
        for (int i = 0; i < (int)inorder.size(); ++i) {
            inMap[inorder[i]] = i;
        }
        return build(postorder, 0, inorder.size() - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Construct binary tree from inorder and postorder traversals (LC 106).
// - Core Pattern: Postorder root selection from back + Build right subtree BEFORE left subtree.
// - Intuition:
//   * Last element in postorder is the root.
//   * As postIndex decrements, we encounter right subtree roots before left subtree roots.
// - Complexity:
//   * Time: O(N) — Each node processed via O(1) hash map lookup.
//   * Space: O(N) — Hash map and O(H) recursion stack.
// - Edge Cases: Constructing left before right (desyncs postIndex and corrupts tree), single node.
// - Likely Follow-ups:
//   1. Why build right before left? -> Postorder is L-R-Root. Backwards traversal is Root-R-L.
//   2. Can preorder + postorder reconstruct unique tree? -> Only if every non-leaf node has 2 children (Full Tree, LC 889).


// =========================================================
// 17. SERIALIZE AND DESERIALIZE BINARY TREE (LC 297)
// =========================================================

class Solution17 {
    void serializeDFS(TreeNode* node, stringstream& ss) {
        if (!node) {
            ss << "#,";
            return;
        }
        ss << node->val << ",";
        serializeDFS(node->left, ss);
        serializeDFS(node->right, ss);
    }

    TreeNode* deserializeDFS(stringstream& ss) {
        string token;
        if (!getline(ss, token, ',')) return nullptr;
        if (token == "#") return nullptr;
        
        TreeNode* node = new TreeNode(stoi(token));
        node->left = deserializeDFS(ss);
        node->right = deserializeDFS(ss);
        return node;
    }

public:
    string serialize(TreeNode* root) {
        stringstream ss;
        serializeDFS(root, ss);
        return ss.str();
    }

    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeDFS(ss);
    }
};
// Interview Explanation:
// - Problem Statement: Design algorithm to serialize binary tree to string and deserialize back to tree (LC 297).
// - Core Pattern: Preorder DFS with sentinel tokens ('#') and delimiter (',') via std::stringstream.
// - Intuition:
//   * A unique preorder traversal requires null pointers to be explicitly recorded.
//   * Using stringstream makes token parsing robust and clean for multi-digit and negative numbers.
// - Complexity:
//   * Time: O(N) — Serialization and deserialization visit each node/token once.
//   * Space: O(N) — Output string of size 2N + 1 tokens; O(H) call stack.
// - Edge Cases: Empty tree ("#,"), negative values, multi-digit values.
// - Likely Follow-ups:
//   1. How to do BFS level order serialization? -> Queue-based producing "1,2,3,#,#,4,5"; deserializer matches parent to children.
//   2. How to compress serialized output? -> Use raw binary buffer (4-byte integers + bitmask for child presence flags).


// =========================================================
// 18. CONVERT SORTED LIST TO BINARY SEARCH TREE (LC 109)
// =========================================================

class Solution18 {
    ListNode* currList = nullptr;

    int countLength(ListNode* head) {
        int len = 0;
        while (head) {
            len++;
            head = head->next;
        }
        return len;
    }

    // Inorder Simulation: Left -> Root -> Right
    TreeNode* buildBST(int start, int end) {
        if (start > end) return nullptr;
        
        int mid = start + (end - start) / 2;
        
        // 1. Build left subtree
        TreeNode* leftChild = buildBST(start, mid - 1);
        
        // 2. Use current list node as Root
        TreeNode* root = new TreeNode(currList->val);
        root->left = leftChild;
        currList = currList->next; // Advance list pointer in sync with inorder traversal
        
        // 3. Build right subtree
        root->right = buildBST(mid + 1, end);
        
        return root;
    }

public:
    TreeNode* sortedListToBST(ListNode* head) {
        int n = countLength(head);
        currList = head;
        return buildBST(0, n - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Convert sorted singly linked list to height-balanced BST (LC 109).
// - Core Pattern: Simulated Inorder Traversal using global list iterator (O(N) time).
// - Intuition:
//   * The naive slow/fast pointer takes O(N log N) because finding the median requires O(N) at each level.
//   * Simulated inorder constructs tree left-to-right matching the exact sequence of the linked list in O(N).
// - Complexity:
//   * Time: O(N) — Single pass of list to count length, single pass during tree construction.
//   * Space: O(log N) stack space — Tree is strictly balanced by construction.
// - Edge Cases: Empty list (null), single node, even vs odd element counts.
// - Likely Follow-ups:
//   1. Why does inorder simulation work? -> A BST's inorder traversal produces sorted order; we mimic this order exactly.
//   2. What is the slow/fast pointer complexity? -> T(N) = 2T(N/2) + O(N) -> O(N log N).


// =========================================================
// 19. CONVERT SORTED ARRAY TO BINARY SEARCH TREE (LC 108)
// =========================================================

class Solution19 {
    TreeNode* build(const vector<int>& nums, int left, int right) {
        if (left > right) return nullptr;
        
        int mid = left + (right - left) / 2; // Midpoint guarantees height balance
        TreeNode* root = new TreeNode(nums[mid]);
        
        root->left = build(nums, left, mid - 1);
        root->right = build(nums, mid + 1, right);
        
        return root;
    }

public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return build(nums, 0, (int)nums.size() - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Convert sorted array to height-balanced BST (LC 108).
// - Core Pattern: Divide and Conquer / Binary Search midpoint picking.
// - Intuition:
//   * Picking the middle element `mid = left + (right - left) / 2` ensures left and right subtrees have sizes differing by at most 1.
// - Complexity:
//   * Time: O(N) — Every element converted into a node once.
//   * Space: O(log N) stack space.
// - Edge Cases: Empty array, single element, 2 elements (left or right leaning root).
// - Likely Follow-ups:
//   1. Does choosing `(left + right + 1) / 2` produce a valid balanced BST? -> Yes, both ceil and floor medians are valid AVL structures.
//   2. How to verify height balance? -> Run LC 110 check in O(N) time.


// ====================================================================================================
// GROUP 5: PATH SUMS & TREE DP
// ====================================================================================================

// =========================================================
// 20. BINARY TREE MAXIMUM PATH SUM (LC 124)
// =========================================================

class Solution20 {
    int maxPath = INT_MIN;

    int maxGain(TreeNode* node) {
        if (!node) return 0;
        
        // Ignore negative branches by flooring at 0
        int leftGain = max(0, maxGain(node->left));
        int rightGain = max(0, maxGain(node->right));
        
        // Price of path using current node as the highest ancestor (split path)
        int currentPathSum = node->val + leftGain + rightGain;
        maxPath = max(maxPath, currentPathSum);
        
        // Return single branch gain to parent caller (cannot branch both directions)
        return node->val + max(leftGain, rightGain);
    }

public:
    int maxPathSum(TreeNode* root) {
        maxPath = INT_MIN;
        maxGain(root);
        return maxPath;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum path sum of any non-empty path in a binary tree (LC 124).
// - Core Pattern: Postorder Bottom-Up Tree DP (distinguishing split path vs single branch return).
// - Intuition:
//   * A valid path can only branch once (at its highest ancestor).
//   * At node `u`, update global max with `u->val + leftGain + rightGain`.
//   * Return `u->val + max(leftGain, rightGain)` to parent because parent cannot fork into both left and right.
// - Complexity:
//   * Time: O(N) — Single postorder traversal.
//   * Space: O(H) recursion stack (O(N) for skewed tree).
// - Edge Cases: All negative values (e.g., [-3]; initializing maxPath to 0 bugs out!), single node.
// - Likely Follow-ups:
//   1. What if path must go from root to leaf? -> LC 112 / 113; drop split path logic and pass running sum downward.
//   2. How to reconstruct the actual path nodes? -> Track child choices or store parent pointers.


// =========================================================
// 21. DIAMETER OF BINARY TREE (LC 543)
// =========================================================

class Solution21 {
    int maxDiameter = 0;

    int maxDepth(TreeNode* node) {
        if (!node) return 0;
        
        int leftDepth = maxDepth(node->left);
        int rightDepth = maxDepth(node->right);
        
        // Diameter through current node = left edges + right edges
        maxDiameter = max(maxDiameter, leftDepth + rightDepth);
        
        // Return subtree depth to parent
        return 1 + max(leftDepth, rightDepth);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        maxDiameter = 0;
        maxDepth(root);
        return maxDiameter;
    }
};
// Interview Explanation:
// - Problem Statement: Return diameter of tree (length of longest path between any two nodes in edges) (LC 543).
// - Core Pattern: Postorder Bottom-Up Tree DP (side-effect diameter update during depth calculation).
// - Intuition:
//   * The longest path through node `u` has length `depth(u->left) + depth(u->right)`.
// - Complexity:
//   * Time: O(N) — Every node visited once.
//   * Space: O(H) recursion stack.
// - Edge Cases: Single node (diameter = 0 edges), skewed tree, diameter not passing through root.
// - Likely Follow-ups:
//   1. Difference between node count and edge count? -> Edge count = left + right; node count = left + right + 1.
//   2. How to solve for N-ary tree? -> Track top 2 largest child depths; sum them for diameter.


// =========================================================
// 22. PATH SUM III (LC 437)
// =========================================================

class Solution22 {
    unordered_map<long long, int> prefixSumMap;
    int count = 0;

    void dfs(TreeNode* node, long long currentSum, int targetSum) {
        if (!node) return;
        
        currentSum += node->val;
        
        // Check if prefix exists that satisfies: currentSum - prefix = targetSum
        if (prefixSumMap.count(currentSum - targetSum)) {
            count += prefixSumMap[currentSum - targetSum];
        }
        
        prefixSumMap[currentSum]++;
        
        dfs(node->left, currentSum, targetSum);
        dfs(node->right, currentSum, targetSum);
        
        // Backtrack: remove current sum before returning to parent branch
        prefixSumMap[currentSum]--;
    }

public:
    int pathSum(TreeNode* root, int targetSum) {
        count = 0;
        prefixSumMap.clear();
        prefixSumMap[0] = 1; // Base case: prefix sum of 0 has frequency 1
        dfs(root, 0LL, targetSum);
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Count paths going downwards that sum to targetSum (LC 437).
// - Core Pattern: Prefix Sum Hash Map + Backtracking on Trees (analogous to Subarray Sum Equals K).
// - Intuition:
//   * Brute force checking paths from every node takes O(N^2).
//   * Hash map of running prefix sums finds valid subpaths in O(1) per node.
//   * Backtracking `prefixSumMap[currentSum]--` prevents paths in left branch from leaking into right branch.
// - Complexity:
//   * Time: O(N) — Single pass using hash map.
//   * Space: O(H) — Map stores at most H ancestor sums; O(H) recursion stack.
// - Edge Cases: Large node values exceeding 32-bit int (use `long long` for currentSum), negative node values.
// - Likely Follow-ups:
//   1. Why is `long long` needed? -> Deep trees with sums exceeding 2^31 - 1 cause signed integer overflow.
//   2. Why decrement prefixSumMap? -> Paths must move strictly downwards; sibling branches cannot share prefix sums.


// =========================================================
// 23. ALL NODES DISTANCE K IN BINARY TREE (LC 863)
// =========================================================

class Solution23 {
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        if (!root || !target) return {};
        if (k == 0) return {target->val};
        
        // Step 1: Annotate parent pointers via BFS
        unordered_map<TreeNode*, TreeNode*> parentMap;
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* curr = q.front(); q.pop();
            if (curr->left) {
                parentMap[curr->left] = curr;
                q.push(curr->left);
            }
            if (curr->right) {
                parentMap[curr->right] = curr;
                q.push(curr->right);
            }
        }
        
        // Step 2: Radial BFS outward from target in all 3 directions (left, right, parent)
        unordered_set<TreeNode*> visited;
        queue<TreeNode*> bfsQueue;
        bfsQueue.push(target);
        visited.insert(target);
        
        int currentDist = 0;
        while (!bfsQueue.empty()) {
            int levelSize = bfsQueue.size();
            if (currentDist == k) break; // Reached distance K
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* curr = bfsQueue.front(); bfsQueue.pop();
                
                if (curr->left && !visited.count(curr->left)) {
                    visited.insert(curr->left);
                    bfsQueue.push(curr->left);
                }
                if (curr->right && !visited.count(curr->right)) {
                    visited.insert(curr->right);
                    bfsQueue.push(curr->right);
                }
                if (parentMap.count(curr) && !visited.count(parentMap[curr])) {
                    visited.insert(parentMap[curr]);
                    bfsQueue.push(parentMap[curr]);
                }
            }
            currentDist++;
        }
        
        vector<int> result;
        while (!bfsQueue.empty()) {
            result.push_back(bfsQueue.front()->val);
            bfsQueue.pop();
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return all nodes at distance K from target node in binary tree (LC 863).
// - Core Pattern: Tree to Undirected Graph conversion via parent map + Radial BFS from target.
// - Intuition:
//   * A binary tree only has downward pointers. Adding parent pointers converts it into a generic undirected graph.
//   * Running BFS from `target` outward (left, right, parent) finds all nodes at distance K in exact topological order.
// - Complexity:
//   * Time: O(N) — Pass 1 maps parents (N); Pass 2 BFS visits at most N nodes.
//   * Space: O(N) — Parent map and visited hash set.
// - Edge Cases: k = 0 (returns [target->val]), k exceeds tree height, target is root or leaf.
// - Likely Follow-ups:
//   1. Can you solve without parent map? -> Yes, DFS returning distance to target; search opposite branch with remaining k.
//   2. What if tree cannot be modified and memory is constrained? -> Find path from root to target first (O(H) space).


// =========================================================
// 24. BALANCED BINARY TREE (LC 110)
// =========================================================

class Solution24 {
    // Returns depth if balanced, or -1 immediately if ANY subtree is unbalanced
    int checkHeight(TreeNode* node) {
        if (!node) return 0;
        
        int leftHeight = checkHeight(node->left);
        if (leftHeight == -1) return -1; // Fast-exit propagation
        
        int rightHeight = checkHeight(node->right);
        if (rightHeight == -1) return -1;
        
        if (abs(leftHeight - rightHeight) > 1) return -1; // Imbalance detected
        
        return 1 + max(leftHeight, rightHeight);
    }

public:
    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if binary tree is height-balanced (subtree depths differ by <= 1) (LC 110).
// - Core Pattern: Bottom-Up Postorder DFS with -1 sentinel flag for early termination.
// - Intuition:
//   * Top-down calculation repeats depth computations leading to O(N^2) time.
//   * Bottom-up computes height from leaves upwards, returning -1 as soon as any imbalance occurs.
// - Complexity:
//   * Time: O(N) — Visits each node once; early exits on failure.
//   * Space: O(H) recursion stack (O(N) for skewed tree).
// - Edge Cases: Empty tree (true), single node (true), linked-list skewed tree.
// - Likely Follow-ups:
//   1. Why is top-down O(N^2)? -> depth() is called repeatedly on every node from root down.
//   2. What is maximum height of an AVL tree? -> ~1.44 log2(N).


// ====================================================================================================
// GROUP 6: IN-PLACE RESTRUCTURING
// ====================================================================================================

// =========================================================
// 25. FLATTEN BINARY TREE TO LINKED LIST (LC 114)
// =========================================================

class Solution25 {
public:
    // Approach 1: In-Place Predecessor Rewiring (O(1) Auxiliary Space — Microsoft Favorite)
    void flatten(TreeNode* root) {
        TreeNode* curr = root;
        while (curr) {
            if (curr->left) {
                // Find rightmost node in left subtree (inorder predecessor)
                TreeNode* pred = curr->left;
                while (pred->right) {
                    pred = pred->right;
                }
                
                // Wire predecessor's right to current's right
                pred->right = curr->right;
                // Move left subtree to the right
                curr->right = curr->left;
                curr->left = nullptr; // Left pointer must be set to nullptr
            }
            curr = curr->right; // Move down the right spine
        }
    }

    // Approach 2: Reverse Preorder DFS (Postorder variant: Right -> Left -> Root)
    TreeNode* prev = nullptr;
    void flattenDFS(TreeNode* root) {
        if (!root) return;
        flattenDFS(root->right);
        flattenDFS(root->left);
        
        root->right = prev;
        root->left = nullptr;
        prev = root;
    }
};
// Interview Explanation:
// - Problem Statement: Flatten binary tree to right-leaning linked list in-place following preorder traversal (LC 114).
// - Core Pattern: Morris-like In-Place Predecessor Rewiring (O(1) space) OR Reverse Preorder DFS.
// - Intuition:
//   * For any node with a left child, its right subtree must be attached to the rightmost leaf of its left subtree.
//   * Splicing the left subtree into `curr->right` and zeroing `curr->left` preserves preorder without stack.
// - Complexity:
//   * Time: O(N) — Each edge traversed at most twice.
//   * Space: O(1) auxiliary space for rewiring; O(H) for DFS.
// - Edge Cases: Tree already flattened, strictly left-skewed tree, single node, empty tree.
// - Likely Follow-ups:
//   1. Why does standard Preorder DFS fail with naive root->right modification? -> Overwrites right child before visiting it.
//   2. How does Approach 1 relate to Morris traversal? -> It connects inorder predecessor to curr->right permanently.


// =========================================================
// 26. CONVERT BST TO SORTED DOUBLY LINKED LIST (LC 426)
// =========================================================

class Solution26 {
    Node* head = nullptr;
    Node* prev = nullptr;

    void inorder(Node* curr) {
        if (!curr) return;
        
        inorder(curr->left);
        
        // Link prev and curr
        if (prev) {
            prev->right = curr; // prev->next = curr
            curr->left = prev;  // curr->prev = prev
        } else {
            head = curr;        // Smallest node encountered becomes head of DLL
        }
        prev = curr;
        
        inorder(curr->right);
    }

public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        
        head = nullptr;
        prev = nullptr;
        
        inorder(root);
        
        // Close circular loop: link head and tail (prev)
        prev->right = head;
        head->left = prev;
        
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Convert BST to sorted circular doubly linked list in-place (LC 426).
// - Core Pattern: Inorder DFS tracking `prev` and `head` pointers + Final circular link closure.
// - Intuition:
//   * BST inorder traversal processes keys in ascending order.
//   * Rewire `prev->right = curr` and `curr->left = prev` on the fly.
//   * After full traversal, `head` is min node and `prev` is max node; connect them circularly.
// - Complexity:
//   * Time: O(N) — Standard inorder traversal visiting each node once.
//   * Space: O(H) recursion stack (O(N) for skewed tree).
// - Edge Cases: Single node (points to itself circularly), empty tree (returns null).
// - Likely Follow-ups:
//   1. Can you do this iteratively? -> Yes, use an explicit stack for inorder traversal.
//   2. Can you do this in strictly O(1) auxiliary space? -> Yes, using Morris Inorder Traversal.


// =========================================================
// 27. INVERT BINARY TREE (LC 226)
// =========================================================

class Solution27 {
public:
    // Recursive DFS
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        // Swap left and right child pointers
        TreeNode* temp = root->left;
        root->left = invertTree(root->right);
        root->right = invertTree(temp);
        
        return root;
    }

    // Iterative BFS Alternative (Queue-based)
    TreeNode* invertTreeBFS(TreeNode* root) {
        if (!root) return nullptr;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* curr = q.front(); q.pop();
            
            swap(curr->left, curr->right); // Invert pointers
            
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        return root;
    }
};
// Interview Explanation:
// - Problem Statement: Invert a binary tree (mirror it) and return its root (LC 226).
// - Core Pattern: Preorder/Postorder traversal with pointer swap.
// - Intuition:
//   * For every node, swap its left and right subtrees recursively or iteratively using a queue.
// - Complexity:
//   * Time: O(N) — Visits every node once.
//   * Space: O(H) recursion stack / O(W) BFS queue space.
// - Edge Cases: Empty tree, leaf node, asymmetric trees.
// - Likely Follow-ups:
//   1. Can you use DFS stack instead of BFS queue? -> Yes, replace queue with stack; behavior is identical.
//   2. Why not swap during Inorder? -> Swapping during inorder will mistakenly invert one subtree twice.


/*
 ====================================================================================================
                                  A. CORE PATTERNS QUICK-GLANCE TABLE
 ====================================================================================================
 | Pattern                          | Problems Using It                 | Signal / Trigger to Recognize in Interviews |
 | :--------------------------------| :---------------------------------| :-------------------------------------------|
 | BFS Level-Order (q.size())       | LC 102, LC 103, LC 199, LC 116/117| "Level by level", "shortest path", "views from sides", "zigzag", "distance K" |
 | Inorder Traversal (Sorted order) | LC 98, LC 99, LC 230, LC 285, 426 | Any question on a BST asking for ordering, kth element, successor, validation |
 | Postorder Tree DP (Bottom-Up)    | LC 124, LC 543, LC 110, LC 236    | Parent requires computed answers from BOTH subtrees before deciding (max path, LCA) |
 | Prefix Sum + Backtracking on Tree| LC 437                            | "Paths summing to K going downwards" — converts O(N^2) search to O(N) |
 | Two-Pointer Range Bounding       | LC 98, LC 669                     | Validating or trimming BST based on intervals [min, max] using TreeNode* pointers |
 | Morris Traversal (Threaded Tree) | LC 98, LC 99, LC 114, LC 230      | Interviewer asks: "Can you optimize space to strictly O(1) without stack or heap?" |
 | Pointer Rewiring (In-Place)      | LC 114, LC 426, LC 116/117        | Flattening or rewiring tree nodes in-place without auxiliary containers |
 ====================================================================================================

 ====================================================================================================
                               B. MICROSOFT-SPECIFIC INTERVIEW CHECKLIST
 ====================================================================================================
 1. ITERATIVE VS RECURSIVE (STACK OVERFLOW DEFENSE):
    - Clarify tree constraints: "Can this tree be skewed or degenerate with depth up to 10^5?"
    - If N = 10^5 and tree is skewed, recursive DFS consumes 10^5 frames and crashes with Stack Overflow.
    - Mention recursive approach first for clarity, but be prepared to write iterative stack/queue code.

 2. INT_MIN / INT_MAX PITFALLS IN BSTs:
    - Never write `bool isValid(TreeNode* root, int minVal = INT_MIN, int maxVal = INT_MAX)`.
    - If the tree contains INT_MIN as a valid node value, `node->val <= minVal` falsely fails on `[-2147483648]`.
    - Always use `TreeNode* minNode = nullptr, TreeNode* maxNode = nullptr`. Null pointers represent unbounded limits safely.

 3. ZERO-ALLOCATION IN-PLACE POINTER REWIRING:
    - For flattening (LC 114) or BST-to-DLL (LC 426), NEVER copy node values to a vector<int> and allocate new nodes.
    - Wire `prev->right = curr` and `curr->left = prev` on the fly.
    - For LC 114, find the rightmost node of the left subtree (predecessor) and wire `pred->right = curr->right`.

 4. WHEN TO MENTION MORRIS TRAVERSAL (O(1) AUXILIARY SPACE):
    - Morris traversal temporarily threads the inorder predecessor's null right pointer to `curr`.
    - Do NOT write Morris traversal as your primary solution unless explicitly requested (it is slower and error-prone).
    - Proactively mention it as an optimization: "Our solution takes O(H) stack space. If strictly O(1) space is
      demanded, we can achieve it via Morris Traversal by threading predecessor pointers."
 ====================================================================================================
*/
