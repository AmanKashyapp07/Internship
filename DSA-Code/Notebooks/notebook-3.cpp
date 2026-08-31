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
const int MAXN = 1005;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Reconstruct Cycle                           | Predecessor Backtracking Traversal | O(V)     | O(V)     |
 | 2  | Nodes in Cycles (Topological Peeling)       | In-Degree 0 Queue Cascade (Kahn)  | O(V + E) | O(V)     |
 | 3  | Range Bitwise AND (LC 201)                  | Binary Prefix Bit-Shifts          | O(log R) | O(1)     |
 | 4  | Subarray Bitwise ORs (LC 898)               | Set DP / Monotonic Frontier Values| O(30 * N)| O(30 * N)|
 | 5  | Count Total Set Bits (1 to N)               | Periodic Bit Position Math        | O(log N) | O(1)     |
 | 6  | Minimum Subset Sum Difference               | Subset Sum DP <= Total / 2        | O(N*Total| O(Total) |
 | 7  | Count of Longest Increasing Subsequences    | 1D DP (Length & Count Arrays)     | O(N^2)   | O(N)     |
 | 8  | Lexicographical Topological Sort            | Min-Heap Kahn's BFS               | O(VlogV+E| O(V)     |
 | 9  | Shortest Path Visiting All Nodes (LC 847)   | Multi-Source Bitmask BFS          | O(V 2^V) | O(V 2^V) |
 | 10 | Shortest Common Supersequence (SCS) Length  | Reduction: |S1| + |S2| - LCS      | O(N * M) | O(N * M) |
 | 11 | Reconstruct Shortest Common Supersequence   | 2D LCS Table Backtracking (LC1092)| O(N * M) | O(N * M) |
 | 12 | Minimum Window Subsequence (LC 727)         | Forward Match + Backward Shrink   | O(N * M) | O(1)     |
 | 13 | Matrix Chain Multiplication                 | Interval DP over Chain Lengths    | O(N^3)   | O(N^2)   |
 | 14 | Maximum Sum BST in Binary Tree (LC 1373)    | Post-Order Bottom-Up DFS          | O(N)     | O(H)     |
 | 15 | Remove Leaf Nodes with Target Value (LC1325)| Post-Order Recursive Tree Pruning | O(N)     | O(H)     |
 | 16 | Bounded Knapsack (Binary Power Split)       | Binary Split + 1D 0/1 Knapsack    | O(W logK)| O(W)     |
 | 17 | LCS Length of 2 Permutations                | Index Map + Patience Sorting LIS  | O(N logN)| O(N)     |
 | 18 | Longest Common Increasing Subseq (LCIS)     | 1D DP with Optimal Prefix Tracker | O(N * M) | O(M)     |
 | 19 | Possible Path Lengths in DAG                | TopoSort + 2D Reachability DP     | O(VN+EN) | O(V * N) |
 ====================================================================================================
*/

// ============================================================
// 1. RECONSTRUCT CYCLE
// ============================================================

vi buildCycle(int start, const vi &parent) {
    vi cycle; int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];
        if (cur == start) break;
    }
    return cycle;
}
// Interview Explanation:
// - Problem Statement: Reconstruct a detected cycle sequence from a parent predecessor array.
// - Approach: Predecessor backtracking traversal until cycle start is revisited.
// - Intuition: Tracing parent pointers backward from collision node retraces the cycle loop.
// - Complexity: Time: O(L) <= O(V), Space: O(L).


// ============================================================
// 2. NODES IN CYCLES (TOPOLOGICAL PEELING)
// ============================================================

vi getNodesInCycles(int n, const vvi &g, vi &indegree) {
    queue<int> q;
    vector<bool> is_acyclic(n, false);
    vi cyclic_nodes;
    for (int i = 0; i < n; i++) if (indegree[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        is_acyclic[u] = true;
        for (int v : g[u]) if (--indegree[v] == 0) q.push(v);
    }
    for (int i = 0; i < n; i++) if (!is_acyclic[i]) cyclic_nodes.push_back(i);
    return cyclic_nodes;
}
// Interview Explanation:
// - Problem Statement: Identify and return all nodes that are part of at least one cycle in a directed graph.
// - Approach: Topological Peeling via in-degree reduction (Kahn's Algorithm variant).
// - Intuition: Peeling in-degree 0 nodes recursively eliminates acyclic branches, leaving only cyclic nodes.
// - Complexity: Time: O(V + E), Space: O(V).


// ============================================================
// 3. RANGE BITWISE AND
// ============================================================

int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left != right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
}
// Interview Explanation:
// - Problem Statement: Compute bitwise AND of all integers in inclusive range [left, right] (LC 201).
// - Approach: Bit shift to find common binary prefix of left and right.
// - Intuition: Lower bits flip across range; only the shared binary prefix survives continuous bitwise AND.
// - Complexity: Time: O(log R), Space: O(1).


// ============================================================
// 4. SUBARRAY BITWISE ORs
// ============================================================

int subarrayBitwiseORs(const vi &arr) {
    unordered_set<int> res, cur;
    for (int x : arr) {
        unordered_set<int> nxt = {x};
        for (int y : cur) nxt.insert(x | y);
        for (int z : nxt) res.insert(z);
        cur = nxt;
    }
    return res.size();
}
// Interview Explanation:
// - Problem Statement: Find number of distinct bitwise OR values produced by all non-empty contiguous subarrays (LC 898).
// - Approach: Dynamic Programming with Hash Set tracking frontier OR values.
// - Intuition: Bitwise OR is monotonically increasing, giving at most 32 distinct values in frontier set.
// - Complexity: Time: O(N * 30), Space: O(N * 30).


// ============================================================
// 5. COUNT TOTAL SET BITS (1 TO N)
// ============================================================

int countTotalSetBits(int n) {
    int total_ones = 0;
    for (int i = 0; i < 60; i++) {
        ll cycle_len = 1LL << (i + 1);
        ll cycles = (n + 1) / cycle_len;
        ll ones = 1LL << i;
        ll rem = (n + 1) % cycle_len;
        total_ones += cycles * ones + (rem > ones ? rem - ones : 0);
    }
    return total_ones;
}
// Interview Explanation:
// - Problem Statement: Count total set bits (1s) in binary representations of all numbers from 1 to N.
// - Approach: Bitwise Position Math / Periodic Cycle Counting.
// - Intuition: i-th bit alternates in blocks of 2^(i+1) with 2^i ones; count full cycles plus leftover ones.
// - Complexity: Time: O(log N), Space: O(1).


// ============================================================
// 6. MINIMUM SUBSET SUM DIFFERENCE
// ============================================================

int minSubsetSumDifference(const vi &nums) {
    int total_sum = accumulate(nums.begin(), nums.end(), 0);
    vector<bool> dp(total_sum / 2 + 1, false);
    dp[0] = true;
    for (int num : nums) {
        for (int j = total_sum / 2; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }
    for (int j = total_sum / 2; j >= 0; j--) {
        if (dp[j]) return total_sum - 2 * j;
    }
    return total_sum;
}
// Interview Explanation:
// - Problem Statement: Partition array into two subsets minimizing absolute difference of their sums.
// - Approach: Subset Sum boolean DP bounded by total_sum / 2.
// - Intuition: Find largest reachable subset sum S <= total/2 to minimize total - 2S.
// - Complexity: Time: O(N * total_sum), Space: O(total_sum).


// ============================================================
// 7. COUNT OF LONGEST INCREASING SUBSEQUENCES (LIS)
// ============================================================

int countOfLIS(const vi &nums) {
    int n = nums.size();
    if (n == 0) return 0;
    vi len(n, 1), count(n, 1);
    int max_len = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    count[i] = count[j];
                } else if (len[j] + 1 == len[i]) {
                    count[i] += count[j];
                }
            }
        }
        max_len = max(max_len, len[i]);
    }
    int total = 0;
    for (int i = 0; i < n; i++) if (len[i] == max_len) total += count[i];
    return total;
}
// Interview Explanation:
// - Problem Statement: Find number of longest increasing subsequences (LIS) in an array (LC 673).
// - Approach: 1D Dynamic Programming tracking both LIS length and count.
// - Intuition: len[i] stores max LIS length ending at i, count[i] stores ways to achieve it.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 8. LEXICOGRAPHICAL TOPOLOGICAL SORT
// ============================================================

vi lexicographicalTopoSort(int n, const vvi &g, vi indegree) {
    priority_queue<int, vi, greater<int>> pq;
    for (int i = 1; i <= n; i++) if (indegree[i] == 0) pq.push(i);
    vi order;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int v : g[u]) if (--indegree[v] == 0) pq.push(v);
    }
    return (int)order.size() == n ? order : vi{};
}
// Interview Explanation:
// - Problem Statement: Find lexicographically smallest topological ordering of a DAG.
// - Approach: Priority Queue (Min-Heap variant) Kahn's BFS.
// - Intuition: Min-heap extracts smallest available in-degree 0 vertex at each step.
// - Complexity: Time: O(V \log V + E), Space: O(V).


// ============================================================
// 9. SHORTEST PATH VISITING ALL NODES (BITMASK BFS)
// ============================================================

int shortestPathVisitingAllNodes(int n, const vvi &g) {
    int target_mask = (1 << n) - 1;
    queue<pair<int, int>> q;
    vvi dist(n, vi(1 << n, 1e9));
    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i});
        dist[i][1 << i] = 0;
    }
    while (!q.empty()) {
        auto [u, mask] = q.front(); q.pop();
        int d = dist[u][mask];
        if (mask == target_mask) return d;
        for (int v : g[u]) {
            int nxt_mask = mask | (1 << v);
            if (dist[v][nxt_mask] > d + 1) {
                dist[v][nxt_mask] = d + 1;
                q.push({v, nxt_mask});
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find shortest path visiting every vertex in an unweighted undirected graph (LC 847).
// - Approach: Multi-source BFS on State Space (node, bitmask).
// - Intuition: Shortest path to mask == (1 << n) - 1 via BFS guarantees minimal steps.
// - Complexity: Time: O(V * 2^V), Space: O(V * 2^V).


// ============================================================
// 10. SHORTEST COMMON SUPERSEQUENCE (SCS) LENGTH
// ============================================================

int shortestCommonSubsequenceLength(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
    return n + m - dp[n][m];
}
// Interview Explanation:
// - Problem Statement: Find length of shortest common supersequence of two strings.
// - Approach: Reduction to LCS via formula |S1| + |S2| - LCS(S1, S2).
// - Intuition: Common characters (LCS) appear once, giving length N + M - LCS.
// - Complexity: Time: O(|S1| * |S2|), Space: O(|S1| * |S2|).


// ============================================================
// 11. RECONSTRUCT SHORTEST COMMON SUPERSEQUENCE (SCS)
// ============================================================

string SCS(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (s1[i - 1] == s2[j - 1]) ? 1 + dp[i - 1][j - 1] : max(dp[i - 1][j], dp[i][j - 1]);
    string scs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            scs += s1[i - 1]; i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            scs += s1[i - 1]; i--;
        } else {
            scs += s2[j - 1]; j--;
        }
    }
    while (i > 0) { scs += s1[i - 1]; i--; }
    while (j > 0) { scs += s2[j - 1]; j--; }
    reverse(scs.begin(), scs.end());
    return scs;
}
// Interview Explanation:
// - Problem Statement: Reconstruct shortest common supersequence string of two strings (LC 1092).
// - Approach: 2D LCS Table Backtracking.
// - Intuition: Backtrack from (N, M); append matching characters once, non-matching appropriately.
// - Complexity: Time: O(|S1| * |S2|), Space: O(|S1| * |S2|).


// ============================================================
// 12. MINIMUM WINDOW SUBSEQUENCE
// ============================================================

string minWindowSubsequence(const string &s, const string &t) {
    if (t.empty()) return "";
    int n = s.size(), m = t.size(), min_len = 1e9, start_idx = -1, i = 0, j = 0;
    while (i < n) {
        if (s[i] == t[j]) {
            j++;
            if (j == m) {
                int end = i + 1; j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                i++; j++;
                if (end - i < min_len) { min_len = end - i; start_idx = i; }
            }
        }
        i++;
    }
    return start_idx == -1 ? "" : s.substr(start_idx, min_len);
}
// Interview Explanation:
// - Problem Statement: Find minimum length substring of s that contains t as a subsequence (LC 727).
// - Approach: Two-pointer forward match + backward window contraction.
// - Intuition: Scan forward to match t, then scan backwards to minimize left boundary.
// - Complexity: Time: O(|S| * |T|), Space: O(1).


// ============================================================
// 13. MATRIX CHAIN MULTIPLICATION
// ============================================================

int matrixChainOrder(const vi &p) {
    int n = p.size() - 1;
    vvi dp(n, vi(n, 0));
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = 1e9;
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1]);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find minimum scalar multiplications needed to multiply a chain of matrices.
// - Approach: Interval Dynamic Programming over chain lengths 2 to N.
// - Intuition: dp[i][j] = min(dp[i][k] + dp[k+1][j] + p[i]*p[k+1]*p[j+1]) over all split points k.
// - Complexity: Time: O(N^3), Space: O(N^2).


// ============================================================
// 14. MAXIMUM SUM BST IN BINARY TREE
// ============================================================

struct MaxSumBST {
    int ans = 0;
    tuple<bool, int, int, int> dfs(TreeNode *node) {
        if (!node) return {true, INT_MAX, INT_MIN, 0};
        auto [l_bst, l_min, l_max, l_sum] = dfs(node->left);
        auto [r_bst, r_min, r_max, r_sum] = dfs(node->right);
        if (l_bst && r_bst && node->val > l_max && node->val < r_min) {
            int sum = l_sum + r_sum + node->val;
            ans = max(ans, sum);
            return {true, min(node->val, l_min), max(node->val, r_max), sum};
        }
        return {false, 0, 0, 0};
    }
    int maxSumBST(TreeNode *root) {
        ans = 0;
        dfs(root);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum sum of node keys among all valid BST subtrees (LC 1373).
// - Approach: Post-order Bottom-Up DFS returning {is_bst, min_val, max_val, sum}.
// - Intuition: Valid BST if node->val > left_max and node->val < right_min; update global max sum.
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 15. REMOVE LEAF NODES WITH TARGET VALUE
// ============================================================

TreeNode* removeLeafNodes(TreeNode *root, int target) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, target);
    root->right = removeLeafNodes(root->right, target);
    return (!root->left && !root->right && root->val == target) ? nullptr : root;
}
// Interview Explanation:
// - Problem Statement: Delete leaf nodes with target value repeatedly until none remain (LC 1325).
// - Approach: Post-order Recursive DFS.
// - Intuition: Process subtrees first so newly formed leaves can be evaluated.
// - Complexity: Time: O(N), Space: O(H).


// ============================================================
// 16. BOUNDED KNAPSACK (BINARY POWER SPLIT) & 0/1 KNAPSACK
// ============================================================

void addItemBinarySplit(int w, int v, int k, vi &weights, vi &values) {
    for (int take = 1; k > 0; take <<= 1) {
        int cnt = min(take, k);
        weights.push_back(cnt * w);
        values.push_back(cnt * v);
        k -= cnt;
    }
}

int knapsack01(int n, int W, const vi &weights, const vi &values) {
    vi dp(W + 1, 0);
    for (int i = 0; i < n; i++)
        for (int w = W; w >= weights[i]; w--)
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
    return dp[W];
}
// Interview Explanation:
// - Problem Statement: Solve Bounded Knapsack with item quantities efficiently.
// - Approach: Binary Power Splitting (1, 2, 4, ..., rem) + 1D 0/1 Knapsack DP.
// - Intuition: Decompose quantity k into powers of 2 items, reducing to standard 0/1 knapsack.
// - Complexity: Time: O(W * sum(log K)), Space: O(W).


// ============================================================
// 17. LCS LENGTH OF 2 PERMUTATIONS (LIS REDUCTION)
// ============================================================

int LCSLengthOf2Permutations(const vi &a, const vi &b) {
    unordered_map<int, int> pos;
    int n = a.size();
    for (int i = 0; i < n; i++) pos[a[i]] = i;
    vi dp;
    for (int x : b) {
        if (!pos.count(x)) continue;
        auto it = lower_bound(dp.begin(), dp.end(), pos[x]);
        if (it == dp.end()) dp.push_back(pos[x]);
        else *it = pos[x];
    }
    return dp.size();
}
// Interview Explanation:
// - Problem Statement: Find LCS length of two permutations of numbers 1 to N.
// - Approach: Permutation Index Mapping + LIS via Patience Sorting.
// - Intuition: Transform b by indices in a; LCS is isomorphic to LIS of transformed array.
// - Complexity: Time: O(N \log N), Space: O(N).


// ============================================================
// 18. LONGEST COMMON INCREASING SUBSEQUENCE (LCIS)
// ============================================================

vi LCIS(const vi &a, const vi &b) {
    int n = a.size(), m = b.size();
    vi dp(m, 0), parent(m, -1);
    for (int i = 0; i < n; i++) {
        int best_len = 0, best_prev = -1;
        for (int j = 0; j < m; j++) {
            if (a[i] == b[j]) {
                if (best_len + 1 > dp[j]) {
                    dp[j] = best_len + 1;
                    parent[j] = best_prev;
                }
            } else if (a[i] > b[j] && dp[j] > best_len) {
                best_len = dp[j];
                best_prev = j;
            }
        }
    }
    int end = max_element(dp.begin(), dp.end()) - dp.begin();
    if (dp[end] == 0) return {};
    vi ans;
    while (end != -1) {
        ans.push_back(b[end]);
        end = parent[end];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find and reconstruct Longest Common Increasing Subsequence (LCIS) of two arrays.
// - Approach: 1D Dynamic Programming with running optimal prefix tracking.
// - Intuition: Track best_len among b[j] < a[i]; extend dp[j] = best_len + 1 when a[i] == b[j].
// - Complexity: Time: O(N * M), Space: O(M).


// ============================================================
// 19. POSSIBLE PATH LENGTHS IN DAG
// ============================================================

vi topoSortHelper(int n, const vvi &g) {
    vi indeg(n + 1, 0), order;
    queue<int> q;
    for (int u = 1; u <= n; u++) for (int v : g[u]) indeg[v]++;
    for (int i = 1; i <= n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order;
}

vi possibleLengths(int n, const vvi &g) {
    vi topo = topoSortHelper(n, g);
    static bool dp[MAXN][MAXN];
    memset(dp, 0, sizeof(dp));
    dp[1][0] = true;
    for (int u : topo)
        for (int v : g[u])
            for (int len = 0; len < n; len++)
                if (dp[u][len]) dp[v][len + 1] = true;
    vi ans;
    for (int len = 0; len < n; len++) if (dp[n][len]) ans.push_back(len);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all possible path lengths from node 1 to node N in a DAG.
// - Approach: Topological Sort + 2D Reachability DP dp[node][len].
// - Intuition: Transition dp[v][len + 1] = true if dp[u][len] is true in topological order.
// - Complexity: Time: O(V * N + E * N), Space: O(V * N).
