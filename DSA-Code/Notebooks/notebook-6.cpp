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

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Contains Duplicate III (LeetCode 220)       | Bucket Sort / Width Partitioning  | O(N)     | O(K)     |
 | 2  | Move Zeroes (LeetCode 283)                  | Two Pointers (Slow/Fast In-Place) | O(N)     | O(1)     |
 | 3  | Construct Tree from Pre & Post (LC 889)     | Divide & Conquer Subtree Partition| O(N^2)   | O(N)     |
 | 4  | Numbers With Same Consec Diff (LC 967)      | Digit DP / Multi-Branching DFS    | O(2^N)   | O(2^N)   |
 | 5  | Largest Component by Common Factor (LC 952) | DSU + Prime Factor Indexing       | O(N sqrt)| O(N + P) |
 | 6  | Generalized K-Sum (LeetCode 18 / 15 / 1)    | Sort + Pruning + 2-Pointer Base   | O(N^(K-1))| O(K)    |
 | 7  | Delete Odd-Valued Nodes from Linked List    | Dummy Node + Single-Pass Deletion | O(N)     | O(1)     |
 | 8  | Equalize Heights with Asymmetric Costs      | Ternary Search on Convex Cost     | O(N logH)| O(1)     |
 | 9  | In-Place BST to Doubly Linked List          | In-Order Traversal In-Place Rewire| O(N)     | O(H)     |
 | 10 | Median of BST (Morris Traversal)            | 2-Pass Morris In-Order Threading   | O(N)     | O(1)     |
 ====================================================================================================
*/

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

using Node = TreeNode;


// ============================================================
// 1. CONTAINS DUPLICATE III — LeetCode 220
// ============================================================

ll getBucketID(ll x, ll w) {
    if (x >= 0) return x / w;
    return (x + 1) / w - 1;
}

bool containsNearbyAlmostDuplicate(vi& a, int k, int t) {
    if (t < 0) return false;
    ll w = (ll)t + 1;
    unordered_map<ll, ll> b;

    for (int i = 0; i < (int)a.size(); i++) {
        ll x = a[i];
        ll id = getBucketID(x, w);

        if (b.count(id)) return true;
        if (b.count(id - 1) && abs(x - b[id - 1]) <= t) return true;
        if (b.count(id + 1) && abs(x - b[id + 1]) <= t) return true;

        b[id] = x;
        if (i >= k) {
            ll old = a[i - k];
            ll oldId = getBucketID(old, w);
            b.erase(oldId);
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if there exist indices i, j such that |i - j| <= k and |a[i] - a[j]| <= t (LeetCode 220).
// - Approach: Bucket Partitioning of width (t + 1) over sliding window of size k.
// - Intuition:
//   * Partitioning real line into discrete buckets of size w = t + 1 ensures that any two numbers in the same bucket satisfy |x - y| <= t.
//   * Hence, if a bucket already holds an element, a valid pair is discovered immediately.
//   * Numbers satisfying |x - y| <= t that fall into different buckets can only reside in immediately adjacent buckets: `id - 1` or `id + 1`.
//   * A sliding window of size k maintains at most k elements in hash map `b`, achieving linear time lookup.
// - Complexity: Time: O(N), Space: O(min(N, k)).


// ============================================================
// 2. MOVE ZEROES — LeetCode 283
// ============================================================

void moveZeroes(vi& a) {
    int n = a.size();
    int j = 0;
    // j is the index of the next position to place a non-zero element
    for (int i = 0; i < n; i++) {
        if (a[i] != 0) { // if a[i] is non-zero, swap it with the element at index j and increment j
            swap(a[i], a[j]);
            j++;
        }
    }
}
// Interview Explanation:
// - Problem Statement: Move all 0's to the end while maintaining the relative order of non-zero elements in-place (LeetCode 283).
// - Approach: Two Pointers (Slow write pointer j, fast read pointer i).
// - Intuition:
//   * Pointer j maintains the frontier boundary where the next non-zero element should be placed.
//   * Pointer i scans forward through the array inspecting each element.
//   * When `a[i] != 0`, swapping `a[i]` with `a[j]` shifts the non-zero element to the left boundary and preserves relative order.
//   * Naturally leaves all zeros pushed toward the right tail of the array in a single O(N) pass with O(1) auxiliary memory.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 5. LARGEST COMPONENT SIZE BY COMMON FACTOR — LeetCode 952
// ============================================================

struct DSUFactor {
    vi par, sz;
    DSUFactor(int n) : par(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }
    int find(int x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        par[b] = a;
        sz[a] += sz[b];
    }
};

int largestComponentSize(vi& a) {
    int n = a.size();
    DSUFactor dsu(n);
    unordered_map<int, int> pos;
    // pos[p] = index of first number in a that has prime factor p
    for (int i = 0; i < n; i++) {
        int x = a[i];
        for (int p = 2; p * p <= x; p++) {
            if (x % p != 0) continue;
            if (pos.count(p)) dsu.unite(i, pos[p]);
            else pos[p] = i;
            while (x % p == 0) x /= p;
        }
        if (x > 1) {
            if (pos.count(x)) dsu.unite(i, pos[x]);
            else pos[x] = i;
        }
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dsu.sz[dsu.find(i)]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find size of largest connected component where edge exists if gcd(a[i], a[j]) > 1 (LeetCode 952).
// - Approach: Disjoint Set Union (DSU) with Prime Factor Map Indexing.
// - Intuition:
//   * Directly checking gcd between all pairs takes O(N^2 log(max_val)), which causes TLE for N = 2e4.
//   * Instead, two numbers share an edge if and only if they share at least one common prime factor.
//   * For each number a[i], compute its prime factorization in O(sqrt(A)).
//   * For each prime factor p of a[i], union index i with the first index that had p as a factor (`pos[p]`).
//   * The size of the largest connected component in DSU represents the maximal group of mutually connected numbers.
// - Complexity: Time: O(N * sqrt(max_val)), Space: O(N + Total_Primes).


// ============================================================
// 6. GENERALIZED K-SUM (LEETCODE 18 / 15 / 1)
// ============================================================

vvi twoSumTarget(vi& a, ll target, int start) {
    vvi res;
    int n = a.size();
    int l = start, r = n - 1;
    while (l < r) {
        ll sum = a[l] + a[r];
        if (sum == target) {
            res.push_back({a[l], a[r]});
            int left = a[l];
            int right = a[r];
            while (l < r && a[l] == left) l++;
            while (l < r && a[r] == right) r--;
        } 
        else if (sum < target) l++;
        else r--;
    }
    return res;
} // res stores all unique pairs (a[l], a[r]) such that a[l] + a[r] == target, starting from index `start` to the end of array `a`.

vvi kSumHelper(vi& a, ll target, int start, int k) {
    vvi res;
    if (k == 2) return twoSumTarget(a, target, start);
    int n = a.size();
    for (int i = start; i <= n - k; i++) { // at least k elements must remain to form a k-sum
        if (i > start && a[i] == a[i - 1]) continue; // skip duplicates to ensure unique k-tuples
        vvi sub = kSumHelper(a, target - a[i], i + 1, k - 1); // recursively find (k-1)-sum for remaining elements
        for (auto& combination : sub) {
            combination.insert(combination.begin(), a[i]); // prepend a[i] to each (k-1)-tuple to form a k-tuple
            res.push_back(combination);
        }
    }
    return res;
}

vvi kSum(vi& a, ll target, int k) {
    sort(a.begin(), a.end());
    return kSumHelper(a, target, 0, k);
}

vvi threeSum(vi& a, ll target) {
    return kSum(a, target, 3);
}

vvi fourSum(vi& a, ll target) {
    return kSum(a, target, 4);
}

// Interview Explanation:
// - Problem Statement: Find all unique k-tuples in array a that sum to target t without duplicate sets (generalizes 2-Sum, 3-Sum, and 4-Sum).
// - Approach: Sort + Recursive K-Reduction + Two-Pointer Base Case (k = 2) + Bound Pruning.
// - Intuition:
//   * Sorting array upfront allows skipping identical adjacent values to guarantee uniqueness and enables two-pointer convergence.
//   * For any k > 2, pick element a[i] and recursively solve (k - 1)-Sum on suffix starting at i + 1 with remaining target t - a[i].
//   * Base case k = 2 executes in O(N) using two pointers (l, r) converging inward, skipping duplicates after each valid pair match.
//   * Powerful bound pruning: if the minimal possible sum (k * a[st]) > t or maximal possible sum (k * a.back()) < t, return immediately.
//   * Prevents 32-bit overflow by maintaining intermediate target t and pair sums as 64-bit integers (`ll`).
// - Complexity: Time: O(N^(k - 1)), Space: O(k) auxiliary recursion depth.


// ============================================================
// 8. EQUALIZE HEIGHTS WITH ASYMMETRIC COSTS
// ============================================================

__int128 computeBuildingCost(const vl& a, ll H, ll cInc, ll cDec) {
    __int128 tot = 0;
    for (ll h : a) {
        if (h < H) tot += (__int128)(H - h) * cInc;
        else if (h > H) tot += (__int128)(h - H) * cDec;
    }
    return tot;
}

string int128ToString(__int128 n) {
    if (n == 0) return "0";
    string s = "";
    while (n > 0) {
        s += (char)('0' + (n % 10));
        n /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

string minCostEqualizeHeights(const vl& a, ll cInc, ll cDec) {
    if (a.empty()) return "0";

    ll lo = *min_element(a.begin(), a.end());
    ll hi = *max_element(a.begin(), a.end());

    while (hi - lo > 2) { // ternary search until the search interval is small enough to check directly
        ll m1 = lo + (hi - lo) / 3; // first trisection point
        ll m2 = hi - (hi - lo) / 3; // second trisection point

        __int128 cost1 = computeBuildingCost(a, m1, cInc, cDec); // compute cost at first trisection point
        __int128 cost2 = computeBuildingCost(a, m2, cInc, cDec); // compute cost at second trisection point

        if (cost1 < cost2) // if cost at m1 is less than cost at m2, the minimum lies in [lo, m2]
            hi = m2;
        else // if cost at m2 is less than or equal to cost at m1, the minimum lies in [m1, hi]
            lo = m1;
    }

    __int128 ans = LLONG_MAX;

    for (ll H = lo; H <= hi; H++) { // [lo, hi] is now a small interval of at most 3 candidates; check each integer height directly
        ans = min(ans, computeBuildingCost(a, H, cInc, cDec)); // check costs at all integer heights in the final small interval
    }

    return int128ToString(ans);
}
// Interview Explanation:
// - Problem Statement: Find uniform building height H that minimizes total asymmetric cost of height increases and decreases.
// - Approach: Ternary Search over Integer Domain of strictly convex cost function + __int128 overflow prevention.
// - Intuition:
//   * For each building, cost(h_i, H) = max(0, H - h_i) * cInc + max(0, h_i - H) * cDec is a convex V-shaped function.
//   * The sum of convex functions f(H) = Sum(cost(h_i, H)) preserves strict convexity (unimodal with a single global minimum).
//   * Ternary search evaluates trisection points m1 and m2, shrinking the search range by 1/3 at each step in O(N log(range)).
//   * When the remaining interval shrinks to <= 2 candidates, test candidate heights directly to pinpoint the exact integer minimum.
//   * Height bounds reach 2e9 and N reaches 1e6, so cumulative costs exceed standard 64-bit integers; __int128 avoids overflow.
// - Complexity: Time: O(N log(max_H - min_H)), Space: O(1).


// ============================================================
// 9. IN-PLACE BST TO DOUBLY LINKED LIST
// ============================================================

Node* insertBST(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->val) root->left = insertBST(root->left, val);
    else root->right = insertBST(root->right, val);
    return root;
}

void bstToDllUtil(Node* root, Node*& prev, Node*& head) {
    if (!root) return;
    bstToDllUtil(root->left, prev, head);
    if (!prev) {
        head = root;
    } else {
        prev->right = root;
        root->left = prev;
    }
    prev = root;
    bstToDllUtil(root->right, prev, head);
}

Node* bstToDoublyLinkedList(Node* root) {
    Node* prev = nullptr;
    Node* head = nullptr;
    bstToDllUtil(root, prev, head);
    return head;
}
// Interview Explanation:
// - Problem Statement: Convert a Binary Search Tree (BST) into a sorted Doubly Linked List in-place from scratch.
// - Approach: In-Order Traversal with Running Predecessor Rewiring.
// - Intuition:
//   * An in-order traversal (Left -> Root -> Right) visits BST nodes in strictly sorted order.
//   * In a doubly linked list, node's left pointer acts as prev and right pointer acts as next.
//   * The very first node processed during in-order traversal has prev == nullptr and becomes the head of the DLL.
//   * For all subsequent nodes, link prev->right = root and root->left = prev, then update prev = root before recursing right.
//   * Transforms tree pointers directly without allocating new nodes or auxiliary node lists.
// - Complexity: Time: O(N), Space: O(H) recursion stack where H is tree height.


// ============================================================
// 10. MEDIAN OF BST (MORRIS IN-ORDER TRAVERSAL)
// ============================================================

int countNodesMorris(TreeNode* root) {
    int cnt = 0;
    TreeNode* cur = root;
    while (cur) {
        if (!cur->left) {
            cnt++;
            cur = cur->right;
        } else {
            TreeNode* prev = cur->left;
            while (prev->right && prev->right != cur) {
                prev = prev->right;
            }
            if (!prev->right) {
                prev->right = cur;
                cur = cur->left;
            } else {
                prev->right = nullptr;
                cnt++;
                cur = cur->right;
            }
        }
    }
    return cnt;
}

double findMedianBST(TreeNode* root) {
    if (!root) return 0.0;
    int tot = countNodesMorris(root);
    int t1 = (tot % 2 == 0) ? (tot / 2) : ((tot + 1) / 2);
    int t2 = (tot % 2 == 0) ? (tot / 2 + 1) : t1;

    TreeNode* cur = root;
    int idx = 0;
    double v1 = 0, v2 = 0;

    while (cur) {
        if (!cur->left) {
            idx++;
            if (idx == t1) v1 = cur->val;
            if (idx == t2) v2 = cur->val;
            cur = cur->right;
        } else {
            TreeNode* prev = cur->left;
            while (prev->right && prev->right != cur) {
                prev = prev->right;
            }
            if (!prev->right) {
                prev->right = cur;
                cur = cur->left;
            } else {
                prev->right = nullptr;
                idx++;
                if (idx == t1) v1 = cur->val;
                if (idx == t2) v2 = cur->val;
                cur = cur->right;
            }
        }
    }
    return (v1 + v2) / 2.0;
}
// Interview Explanation:
// - Problem Statement: Find the median of all node values in a Binary Search Tree (BST) using O(1) auxiliary space.
// - Approach: Two-Pass Morris In-Order Threaded Binary Tree Traversal.
// - Intuition:
//   * Standard BST in-order traversal yields values in sorted order, but recursive or stack approaches cost O(H) auxiliary memory.
//   * Morris Traversal temporarily threads predecessor's right pointer (`prev->right = cur`) to achieve O(1) space traversal.
//   * Pass 1: Count total nodes `tot` using Morris in-order traversal, deriving 1-based target median rank(s) `t1` and `t2`.
//   * Pass 2: Re-traverse via Morris in-order, capturing values `v1` and `v2` at ranks `t1` and `t2` while safely restoring original pointers.
//   * Average `(v1 + v2) / 2.0` yields the exact median for both odd and even sized BSTs in O(N) time without modifying tree structure.
// - Complexity: Time: O(N), Space: O(1).