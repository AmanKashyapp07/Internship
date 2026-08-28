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

struct GraphNode {
    int val;
    vector<GraphNode*> neighbors;
    GraphNode() : val(0), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val) : val(_val), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val, vector<GraphNode*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 7 (Problems 01 to 17)
// ============================================================================

int rob(vi& nums) {
    int prev1 = 0, prev2 = 0;
    for (int x : nums) {
        int curr = max(prev1, prev2 + x);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Determine the maximum money you can rob from a street of houses without robbing any two adjacent houses.
// - Approach: Dynamic Programming with Space Optimization (two state variables).
// - Intuition: At each house `i`, we choose between skipping house `i` (keeping `prev1`) or robbing house `i` (adding `nums[i]` to `prev2`). Maintain only the previous two optimal values.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int ninjaTraining(int n, vvi& points) {
    vi prev(4, 0);

    for (int last = 0; last < 4; last++) {
        for (int task = 0; task < 3; task++) {
            if (task != last) {
                prev[last] = max(prev[last], points[0][task]);
            }
        }
    }

    for (int day = 1; day < n; day++) {
        vi curr(4, 0);
        for (int last = 0; last < 4; last++) {
            for (int task = 0; task < 3; task++) {
                if (task != last) {
                    curr[last] = max(curr[last], points[day][task] + prev[task]);
                }
            }
        }
        prev = curr;
    }

    return prev[3];
}
// Interview Explanation:
// - Problem Statement: Maximize total training points over N days with 3 possible daily activities without performing the same activity on consecutive days.
// - Approach: 2D Dynamic Programming with space compression to O(1) array of size 4.
// - Intuition: Maintain `prev[last_task]` representing the max score achievable up to the previous day if activity `last_task` was executed. For each day, iterate all valid transitions `task != last_task` and compress to 1D state.
// - Complexity: Time: O(N * 4 * 3) = O(N), Space: O(1) auxiliary space.

int minPathSum(vvi& grid) {
    int m = grid.size(), n = grid[0].size();
    vi dp(n, 0);

    for (int i = 0; i < m; i++) {
        vi curr(n, 0);
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) curr[j] = grid[0][0];
            else {
                int up = (i > 0) ? dp[j] : INT_MAX;
                int left = (j > 0) ? curr[j - 1] : INT_MAX;
                curr[j] = grid[i][j] + min(up, left);
            }
        }
        dp = curr;
    }

    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Find a path from top-left to bottom-right in an m x n grid that minimizes the sum of all numbers along the path (only moving right or down).
// - Approach: 2D Dynamic Programming with 1D row space compression.
// - Intuition: To reach cell `(i, j)`, one must come from either top `(i - 1, j)` or left `(i, j - 1)`. Transition: `dp[j] = grid[i][j] + min(dp[j], curr[j-1])`.
// - Complexity: Time: O(M * N), Space: O(N) using single row array.

bool subsetSumToK(int n, int k, vi& arr) {
    vector<bool> dp(k + 1, false);
    dp[0] = true;

    if (arr[0] <= k) dp[arr[0]] = true;

    for (int i = 1; i < n; i++) {
        vector<bool> curr(k + 1, false);
        curr[0] = true;
        for (int target = 1; target <= k; target++) {
            bool not_take = dp[target];
            bool take = (arr[i] <= target) ? dp[target - arr[i]] : false;
            curr[target] = take || not_take;
        }
        dp = curr;
    }

    return dp[k];
}
// Interview Explanation:
// - Problem Statement: Determine if there exists a subset of the given array whose elements sum up to target K.
// - Approach: 0/1 Knapsack Boolean DP with 1D row space optimization.
// - Intuition: For each element `arr[i]`, we either exclude it (`dp[target]`) or include it (`dp[target - arr[i]]`). Base case `dp[0] = true` accounts for the empty subset.
// - Complexity: Time: O(N * K), Space: O(K) space-optimized array.

int findContentChildren(vi& g, vi& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());

    int child = 0, cookie = 0;
    while (child < (int)g.size() && cookie < (int)s.size()) {
        if (s[cookie] >= g[child]) {
            child++; // satisfy this child with the smallest sufficient cookie
        }
        cookie++;
    }

    return child;
}
// Interview Explanation:
// - Problem Statement: Maximize the number of content children by assigning at most one cookie of size s[j] >= g[i] (greed factor) to child i.
// - Approach: Greedy Two Pointers on sorted arrays.
// - Intuition: Sort both children's greed factors and cookie sizes ascendingly. Greedily satisfy the child with the smallest greed factor using the smallest viable cookie to preserve larger cookies for greedier children.
// - Complexity: Time: O(N log N + M log M) sorting, Space: O(1) auxiliary space.

int cutRod(vi& price, int n) {
    vi dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            dp[i] = max(dp[i], price[j - 1] + dp[i - j]);
        }
    }

    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Maximize total profit obtainable by cutting a rod of length n into pieces of lengths with given prices.
// - Approach: Unbounded Knapsack Dynamic Programming.
// - Intuition: For a rod of length `i`, try making the first cut of length `j` (yielding profit `price[j - 1]`), then solve recursively for the remaining rod of length `i - j`. Transition: `dp[i] = max(dp[i], price[j-1] + dp[i-j])`.
// - Complexity: Time: O(N^2), Space: O(N) 1D DP table.

int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vi dp(n + 1, 0);

    for (int i = 1; i <= m; i++) {
        vi curr(n + 1, 0);
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                curr[j] = 1 + dp[j - 1];
            } else {
                curr[j] = max(dp[j], curr[j - 1]);
            }
        }
        dp = curr;
    }

    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest subsequence present in both strings text1 and text2.
// - Approach: 2D Dynamic Programming with 1D row space compression.
// - Intuition: If characters match (`text1[i-1] == text2[j-1]`), extend LCS: `1 + dp[j-1]`. If they differ, take the maximum of excluding one character from either string: `max(dp[j], curr[j-1])`.
// - Complexity: Time: O(M * N), Space: O(N) space-optimized row.

int longestPalindromeSubseq(string s) {
    string rev_s = s;
    reverse(rev_s.begin(), rev_s.end());
    int n = s.size();
    vi dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        vi curr(n + 1, 0);
        for (int j = 1; j <= n; j++) {
            if (s[i - 1] == rev_s[j - 1]) {
                curr[j] = 1 + dp[j - 1];
            } else {
                curr[j] = max(dp[j], curr[j - 1]);
            }
        }
        dp = curr;
    }

    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest subsequence in string s that reads the same forwards and backwards.
// - Approach: Reduction to Longest Common Subsequence (LCS) between string `s` and `reverse(s)`.
// - Intuition: A palindrome is identical when reversed. Thus, finding the longest common subsequence between `s` and its reverse produces the longest palindromic subsequence directly.
// - Complexity: Time: O(N^2), Space: O(N) 1D space-optimized DP array.

int minDistance(string word1, string word2) {
    int m = word1.size(), n = word2.size();
    vi dp(n + 1, 0);

    for (int j = 0; j <= n; j++) dp[j] = j;

    for (int i = 1; i <= m; i++) {
        vi curr(n + 1, 0);
        curr[0] = i;

        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                curr[j] = dp[j - 1];
            } else {
                int insert_op = curr[j - 1];
                int delete_op = dp[j];
                int replace_op = dp[j - 1];
                curr[j] = 1 + min({insert_op, delete_op, replace_op});
            }
        }
        dp = curr;
    }

    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Find the minimum operations (insert, delete, replace) to convert word1 to word2 (Levenshtein Distance).
// - Approach: 2D Dynamic Programming with 1D row space compression.
// - Intuition: If characters match, no operation is needed (`dp[j-1]`). Otherwise, cost is `1 + min(insert: curr[j-1], delete: dp[j], replace: dp[j-1])`.
// - Complexity: Time: O(M * N), Space: O(N) single row array.

int maxProfitIV(int k, vi& prices) {
    int n = prices.size();
    if (n == 0 || k == 0) return 0;

    if (k >= n / 2) {
        int profit = 0;
        for (int i = 1; i < n; i++) {
            if (prices[i] > prices[i - 1]) profit += prices[i] - prices[i - 1];
        }
        return profit;
    }

    vvi dp(k + 1, vi(2, 0));

    for (int i = n - 1; i >= 0; i--) {
        vvi curr(k + 1, vi(2, 0));
        for (int cap = 1; cap <= k; cap++) {
            curr[cap][1] = max(-prices[i] + dp[cap][0], dp[cap][1]);
            curr[cap][0] = max(prices[i] + dp[cap - 1][1], dp[cap][0]);
        }
        dp = curr;
    }

    return dp[k][1];
}
// Interview Explanation:
// - Problem Statement: Find maximum stock trading profit with at most k complete transactions (buy and sell pairs).
// - Approach: 3D Dynamic Programming compressed to 2D state `[k + 1][2]`.
// - Intuition: State is defined by `(day, transaction_capacity, buy_state)`. When buying: profit reduces by `prices[i]`. When selling: profit increases by `prices[i]` and capacity decrements `cap - 1`. If `k >= n/2`, simplifies to unlimited transactions greedy pass.
// - Complexity: Time: O(N * K), Space: O(K) space-optimized state matrix.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

void inorderKth(TreeNode* root, int& k, int& ans) {
    if (!root || k <= 0) return;
    inorderKth(root->left, k, ans);
    if (--k == 0) {
        ans = root->val;
        return;
    }
    inorderKth(root->right, k, ans);
}

int kthSmallest(TreeNode* root, int k) {
    int ans = -1;
    inorderKth(root, k, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element in a Binary Search Tree (BST).
// - Approach: In-Order Traversal with counter decrement.
// - Intuition: Inorder traversal of a BST visits nodes in strictly increasing sorted order. Decrement `k` at each visited node; when `k == 0`, the current node is the k-th smallest.
// - Complexity: Time: O(H + K) where H is tree height, Space: O(H) recursion stack.

class WordDictionary {
private:
    struct TrieNode {
        TrieNode* links[26] = {nullptr};
        bool is_end = false;
    };
    TrieNode* root;

    bool searchInNode(const string& word, int idx, TrieNode* node) {
        if (!node) return false;
        if (idx == (int)word.size()) return node->is_end;

        char c = word[idx];
        if (c == '.') {
            for (int i = 0; i < 26; i++) {
                if (node->links[i] && searchInNode(word, idx + 1, node->links[i])) {
                    return true;
                }
            }
            return false;
        } else {
            int i = c - 'a';
            return node->links[i] && searchInNode(word, idx + 1, node->links[i]);
        }
    }

public:
    WordDictionary() { root = new TrieNode(); }

    void addWord(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->links[idx]) node->links[idx] = new TrieNode();
            node = node->links[idx];
        }
        node->is_end = true;
    }

    bool search(const string& word) {
        return searchInNode(word, 0, root);
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure that supports adding new words and finding if a string matches any previously added string with wildcard '.' support.
// - Approach: Trie + Backtracking / DFS for wildcard characters.
// - Intuition: Standard Trie for word storage. When encountering wildcard '.', branch DFS into all 26 non-null child nodes; for regular characters, follow the single child link.
// - Complexity: Time: O(L) for add, O(26^L) worst case for all '.' queries, Space: O(total characters * 26).

class WordSearchII {
private:
    struct TrieNode {
        TrieNode* links[26] = {nullptr};
        string word = "";
    };

    void insert(TrieNode* root, const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->links[idx]) node->links[idx] = new TrieNode();
            node = node->links[idx];
        }
        node->word = word;
    }

    void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node, vector<string>& result) {
        char ch = board[r][c];
        if (ch == '#' || !node->links[ch - 'a']) return;

        node = node->links[ch - 'a'];
        if (!node->word.empty()) {
            result.push_back(node->word);
            node->word = ""; // avoid duplicate additions
        }

        board[r][c] = '#'; // mark visited
        int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < (int)board.size() && nc >= 0 && nc < (int)board[0].size()) {
                dfs(board, nr, nc, node, result);
            }
        }
        board[r][c] = ch; // backtrack
    }

public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = new TrieNode();
        for (const string& w : words) insert(root, w);

        vector<string> result;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                dfs(board, i, j, root, result);
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Given an m x n board of characters and a list of words, find all words from the list present in the board.
// - Approach: Prefix Trie + 2D Grid DFS Backtracking.
// - Intuition: Build a Trie from `words` and traverse the board once. Prune DFS branches as soon as current grid path is not a prefix in the Trie. Store full word in Trie leaf for instant O(1) collection.
// - Complexity: Time: O(M * N * 4^(max_len)), Space: O(total characters in words).

GraphNode* cloneGraph(GraphNode* node) {
    if (!node) return nullptr;
    unordered_map<GraphNode*, GraphNode*> copies;
    queue<GraphNode*> q;

    copies[node] = new GraphNode(node->val);
    q.push(node);

    while (!q.empty()) {
        GraphNode* curr = q.front();
        q.pop();

        for (GraphNode* neighbor : curr->neighbors) {
            if (!copies.count(neighbor)) {
                copies[neighbor] = new GraphNode(neighbor->val);
                q.push(neighbor);
            }
            copies[curr]->neighbors.push_back(copies[neighbor]);
        }
    }

    return copies[node];
}
// Interview Explanation:
// - Problem Statement: Given a reference of a node in a connected undirected graph, return a deep copy (clone) of the graph.
// - Approach: BFS / DFS with Hash Map mapping original nodes to cloned nodes.
// - Intuition: Map `original -> copy` in an `unordered_map`. Traverse graph using BFS; create clone nodes upon first visit and connect neighbor edges between cloned nodes.
// - Complexity: Time: O(V + E), Space: O(V) for clone hash map and queue.

void dfsOcean(const vvi& heights, vector<vector<bool>>& reachable, int r, int c, int m, int n) {
    reachable[r][c] = true;
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& d : dirs) {
        int nr = r + d[0], nc = c + d[1];
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && !reachable[nr][nc] && heights[nr][nc] >= heights[r][c]) {
            dfsOcean(heights, reachable, nr, nc, m, n);
        }
    }
}

vvi pacificAtlantic(vvi& heights) {
    if (heights.empty()) return {};
    int m = heights.size(), n = heights[0].size();
    vector<vector<bool>> pacific(m, vector<bool>(n, false));
    vector<vector<bool>> atlantic(m, vector<bool>(n, false));

    for (int i = 0; i < m; i++) {
        dfsOcean(heights, pacific, i, 0, m, n);
        dfsOcean(heights, atlantic, i, n - 1, m, n);
    }
    for (int j = 0; j < n; j++) {
        dfsOcean(heights, pacific, 0, j, m, n);
        dfsOcean(heights, atlantic, m - 1, j, m, n);
    }

    vvi ans;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (pacific[i][j] && atlantic[i][j]) {
                ans.push_back({i, j});
            }
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all grid coordinates where water can flow to both the Pacific (top/left) and Atlantic (bottom/right) oceans.
// - Approach: Reverse DFS starting from ocean borders moving uphill (`heights[nr][nc] >= heights[r][c]`).
// - Intuition: Instead of checking flow from every cell downhill, simulate water flowing uphill from the oceans. Run one DFS from Pacific borders and one from Atlantic borders; cells marked in both oceans can flow to both.
// - Complexity: Time: O(M * N), Space: O(M * N) boolean visited matrices.

bool validTree(int n, vvi& edges) {
    if ((int)edges.size() != n - 1) return false;

    vi parent(n);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int u) {
        return parent[u] == u ? u : parent[u] = find(parent[u]);
    };

    for (auto& edge : edges) {
        int rootU = find(edge[0]), rootV = find(edge[1]);
        if (rootU == rootV) return false;
        parent[rootU] = rootV;
    }

    return true;
}
// Interview Explanation:
// - Problem Statement: Given n nodes and a list of undirected edges, check if the edges make up a valid tree.
// - Approach: Disjoint Set Union (DSU) verifying edge count and acyclicity.
// - Intuition: A valid undirected tree of n vertices must satisfy two conditions: (1) exactly `n - 1` edges, and (2) no cycles (fully connected). DSU checks for cycle detection in O(N alpha(N)).
// - Complexity: Time: O(N * alpha(N)), Space: O(N) parent array.

int countComponents(int n, vvi& edges) {
    vi parent(n);
    iota(parent.begin(), parent.end(), 0);
    int components = n;

    function<int(int)> find = [&](int u) {
        return parent[u] == u ? u : parent[u] = find(parent[u]);
    };

    for (auto& edge : edges) {
        int rootU = find(edge[0]), rootV = find(edge[1]);
        if (rootU != rootV) {
            parent[rootU] = rootV;
            components--;
        }
    }

    return components;
}
// Interview Explanation:
// - Problem Statement: Find the number of connected components in an undirected graph with n nodes and given edges.
// - Approach: Disjoint Set Union (DSU).
// - Intuition: Initialize `components = n`. For each edge `(u, v)`, if `find(u) != find(v)`, union them and decrement `components` by 1.
// - Complexity: Time: O(V + E * alpha(V)), Space: O(V) parent array.
