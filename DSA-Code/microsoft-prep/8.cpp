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

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// =========================================================
// 1. DESIGN HIT COUNTER (RATE LIMITER)
// =========================================================

class HitCounter {
    // Fixed size 300 ring buffers
    int times[300];
    int hits[300];

public:
    HitCounter() {
        fill(begin(times), end(times), 0);
        fill(begin(hits), end(hits), 0);
    }

    void hit(int timestamp) {
        int idx = timestamp % 300;
        if (times[idx] != timestamp) {
            times[idx] = timestamp;
            hits[idx] = 1;
        } else {
            hits[idx]++;
        }
    }

    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; ++i) {
            if (timestamp - times[i] < 300) {
                total += hits[i];
            }
        }
        return total;
    }
};
// Interview Explanation:
// - Problem Statement: Count hits received in the past 5 minutes (300 seconds).
// - Approach: Fixed-Size Circular Ring Buffer of size 300.
// - Intuition: Modulo arithmetic (timestamp % 300) keeps memory strictly O(1) regardless of total hit volume.
// - Complexity: Time: O(1) for hit, O(300) = O(1) for getHits, Space: O(300) = O(1).


// =========================================================
// 2. FIND DUPLICATE SUBTREES
// =========================================================

class SolutionFindDuplicateSubtrees {
    unordered_map<string, int> subtreeCount;
    vector<TreeNode*> duplicates;

    string serialize(TreeNode* root) {
        if (!root) return "#";
        string repr = to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
        if (++subtreeCount[repr] == 2) {
            duplicates.push_back(root);
        }
        return repr;
    }

public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        subtreeCount.clear();
        duplicates.clear();
        serialize(root);
        return duplicates;
    }
};
// Interview Explanation:
// - Problem Statement: Find all duplicate subtrees in a binary tree (return root of each unique duplicate structure).
// - Approach: Post-order DFS Subtree Serialization + Hash Map Frequency Counter.
// - Intuition: Convert each subtree to a unique canonical string; add to result when frequency hits exactly 2.
// - Complexity: Time: O(N^2) with strings (or O(N) using integer trie ID mapping), Space: O(N^2).


// =========================================================
// 3. BOUNDARY TRAVERSAL OF BINARY TREE
// =========================================================

bool isLeaf(TreeNode* node) {
    return !node->left && !node->right;
}

void addLeftBoundary(TreeNode* root, vector<int>& res) {
    TreeNode* curr = root->left;
    while (curr) {
        if (!isLeaf(curr)) res.push_back(curr->val);
        if (curr->left) curr = curr->left;
        else curr = curr->right;
    }
}

void addLeaves(TreeNode* root, vector<int>& res) {
    if (!root) return;
    if (isLeaf(root)) {
        res.push_back(root->val);
        return;
    }
    addLeaves(root->left, res);
    addLeaves(root->right, res);
}

void addRightBoundary(TreeNode* root, vector<int>& res) {
    TreeNode* curr = root->right;
    vector<int> temp;
    while (curr) {
        if (!isLeaf(curr)) temp.push_back(curr->val);
        if (curr->right) curr = curr->right;
        else curr = curr->left;
    }
    for (int i = (int)temp.size() - 1; i >= 0; --i) {
        res.push_back(temp[i]);
    }
}

vector<int> boundaryOfBinaryTree(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    if (!isLeaf(root)) res.push_back(root->val);

    addLeftBoundary(root, res);
    addLeaves(root, res);
    addRightBoundary(root, res);
    return res;
}
// Interview Explanation:
// - Problem Statement: Return boundary of binary tree anti-clockwise (root, left boundary, leaves, right boundary reverse).
// - Approach: 3-Stage Modular Traversal.
// - Intuition: Decompose problem into non-leaf left boundary, all leaves via DFS, and reversed non-leaf right boundary.
// - Complexity: Time: O(N), Space: O(H) recursion stack.


// =========================================================
// 4. LOWEST COMMON ANCESTOR OF DEEPEST LEAVES
// =========================================================

pair<int, TreeNode*> lcaDeepestLeavesHelper(TreeNode* root) {
    if (!root) return {0, nullptr};

    auto left = lcaDeepestLeavesHelper(root->left);
    auto right = lcaDeepestLeavesHelper(root->right);

    if (left.first == right.first) {
        return {left.first + 1, root}; // Both subtrees have equal max depth; root is their LCA
    } else if (left.first > right.first) {
        return {left.first + 1, left.second};
    } else {
        return {right.first + 1, right.second};
    }
}

TreeNode* lcaDeepestLeaves(TreeNode* root) {
    return lcaDeepestLeavesHelper(root).second;
}
// Interview Explanation:
// - Problem Statement: Find LCA of all deepest leaf nodes.
// - Approach: Bottom-up Post-Order DFS returning {depth, LCA_node}.
// - Intuition: If left and right subtrees have equal maximum depth, current node is the LCA of all deepest leaves.
// - Complexity: Time: O(N) single pass, Space: O(H) recursion stack.


// =========================================================
// 5. FLATTEN BINARY TREE TO LINKED LIST
// =========================================================

void flatten(TreeNode* root) {
    TreeNode* curr = root;
    while (curr) {
        if (curr->left) {
            // Find predecessor in left subtree
            TreeNode* pred = curr->left;
            while (pred->right) pred = pred->right;

            // Connect predecessor's right to current's right
            pred->right = curr->right;
            curr->right = curr->left;
            curr->left = nullptr;
        }
        curr = curr->right;
    }
}
// Interview Explanation:
// - Problem Statement: Flatten binary tree into a right-skewed linked list in-place in preorder.
// - Approach: Morris Traversal / Predecessor rewiring.
// - Intuition: The right subtree of `curr` must come after the rightmost node of `curr->left`. Rewire in O(1) space.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 6. BINARY TREE ZIGZAG LEVEL ORDER TRAVERSAL
// =========================================================

vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        int sz = q.size();
        vector<int> row(sz);

        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front();
            q.pop();

            int idx = leftToRight ? i : (sz - 1 - i);
            row[idx] = node->val;

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        res.push_back(std::move(row));
        leftToRight = !leftToRight;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Traverse tree level-order in alternating zigzag (left-to-right, then right-to-left).
// - Approach: BFS with Directional Index Mapping.
// - Intuition: Direct index calculation `row[leftToRight ? i : (sz - 1 - i)]` avoids vector reversals.
// - Complexity: Time: O(N), Space: O(W).


// =========================================================
// 7. PATH SUM III (TREE PATHS SUMMING TO TARGET)
// =========================================================

class SolutionPathSumIII {
    unordered_map<long long, int> prefixMap;
    int totalPaths = 0;

    void dfs(TreeNode* root, long long currSum, int target) {
        if (!root) return;

        currSum += root->val;
        if (prefixMap.find(currSum - target) != prefixMap.end()) {
            totalPaths += prefixMap[currSum - target];
        }

        prefixMap[currSum]++;
        dfs(root->left, currSum, target);
        dfs(root->right, currSum, target);
        prefixMap[currSum]--; // Backtrack
    }

public:
    int pathSum(TreeNode* root, int targetSum) {
        prefixMap.clear();
        prefixMap[0] = 1;
        totalPaths = 0;
        dfs(root, 0, targetSum);
        return totalPaths;
    }
};
// Interview Explanation:
// - Problem Statement: Find number of downward paths in a binary tree summing to targetSum.
// - Approach: Prefix Sum + Hash Map on Tree DFS with Backtracking.
// - Intuition: Generalizes Subarray Sum Equals K to trees; backtrack map state when returning from a subtree.
// - Complexity: Time: O(N), Space: O(H) recursion + hash map.


// =========================================================
// 8. SHORTEST BRIDGE
// =========================================================

int shortestBridge(vector<vector<int>>& grid) {
    int n = grid.size();
    queue<pair<int, int>> q;
    bool foundFirst = false;

    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // DFS to paint first island to 2 and push all cells to BFS queue
    function<void(int, int)> dfs = [&](int r, int c) {
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) return;
        grid[r][c] = 2;
        q.push({r, c});
        for (auto& d : dirs) dfs(r + d[0], c + d[1]);
    };

    for (int r = 0; r < n && !foundFirst; ++r) {
        for (int c = 0; c < n && !foundFirst; ++c) {
            if (grid[r][c] == 1) {
                dfs(r, c);
                foundFirst = true;
            }
        }
    }

    // Multi-source BFS to reach second island
    int steps = 0;
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            auto [r, c] = q.front();
            q.pop();

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                    if (grid[nr][nc] == 1) return steps;
                    if (grid[nr][nc] == 0) {
                        grid[nr][nc] = 2; // Visited water
                        q.push({nr, nc});
                    }
                }
            }
        }
        steps++;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum 0s to flip to connect two islands.
// - Approach: DFS (identify & mark Island 1) + Multi-Source BFS (expand water until touching Island 2).
// - Intuition: BFS guarantees shortest path in unweighted grid.
// - Complexity: Time: O(N^2), Space: O(N^2).


// =========================================================
// 9. RECONSTRUCT ITINERARY (EULERIAN PATH)
// =========================================================

vector<string> findItinerary(vector<vector<string>>& tickets) {
    unordered_map<string, multiset<string>> adj;
    for (const auto& t : tickets) {
        adj[t[0]].insert(t[1]);
    }

    vector<string> route;
    stack<string> st;
    st.push("JFK");

    while (!st.empty()) {
        string curr = st.top();
        if (adj[curr].empty()) {
            route.push_back(curr);
            st.pop();
        } else {
            auto nextDest = adj[curr].begin();
            st.push(*nextDest);
            adj[curr].erase(nextDest);
        }
    }

    reverse(route.begin(), route.end());
    return route;
}
// Interview Explanation:
// - Problem Statement: Reconstruct flight itinerary using all tickets with smallest lexical order.
// - Approach: Hierholzer's Algorithm for Eulerian Path using multiset adjacency list.
// - Intuition: Post-order DFS / stack traversal; when a node has no outgoing edges, add to route and reverse at end.
// - Complexity: Time: O(E log E) sorting airport destinations, Space: O(V + E).


// =========================================================
// 10. WORD BREAK II (RETURN ALL SENTENCES)
// =========================================================

class SolutionWordBreakII {
    unordered_set<string> dict;
    unordered_map<string, vector<string>> memo;

    vector<string> dfs(const string& s) {
        if (memo.count(s)) return memo[s];
        if (s.empty()) return {""};

        vector<string> res;
        for (int len = 1; len <= (int)s.size(); ++len) {
            string word = s.substr(0, len);
            if (dict.count(word)) {
                string rest = s.substr(len);
                vector<string> restSentences = dfs(rest);
                for (const string& sent : restSentences) {
                    res.push_back(word + (sent.empty() ? "" : " ") + sent);
                }
            }
        }
        return memo[s] = res;
    }

public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        dict = unordered_set<string>(wordDict.begin(), wordDict.end());
        memo.clear();
        return dfs(s);
    }
};
// Interview Explanation:
// - Problem Statement: Return all valid space-separated sentences formed by words in wordDict.
// - Approach: Top-Down DFS with Memoization.
// - Intuition: Memoizing sub-string solutions avoids re-computing overlapping suffixes.
// - Complexity: Time: O(N * 2^N), Space: O(N * 2^N).


// =========================================================
// 11. COIN CHANGE II (NUMBER OF COMBINATIONS)
// =========================================================

int change(int amount, vector<int>& coins) {
    // dp[i] stores number of ways to make amount i
    vector<unsigned long long> dp(amount + 1, 0);
    dp[0] = 1;

    // Loop over coins FIRST to ensure combination order (no duplicate permutations)
    for (int coin : coins) {
        for (int i = coin; i <= amount; ++i) {
            dp[i] += dp[i - coin];
        }
    }
    return dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find number of combinations that make up amount using unbounded coins.
// - Approach: 1D Dynamic Programming (Coin outer loop).
// - Intuition: Putting coins in the outer loop enforces unique non-decreasing combinations (prevents counting 1+2 and 2+1 separately).
// - Complexity: Time: O(N * Amount), Space: O(Amount).


// =========================================================
// 12. TARGET SUM
// =========================================================

int findTargetSumWays(vector<int>& nums, int target) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    // P - N = target => P - (total - P) = target => 2P = target + total => P = (target + total) / 2
    if ((total + target) < 0 || (total + target) % 2 != 0) return 0;
    int s1 = (total + target) / 2;

    vector<int> dp(s1 + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int i = s1; i >= x; --i) {
            dp[i] += dp[i - x];
        }
    }
    return dp[s1];
}
// Interview Explanation:
// - Problem Statement: Assign '+' and '-' to array elements to evaluate to target.
// - Approach: Math Transformation to 0/1 Subset Sum Knapsack.
// - Intuition: Let subset P be positives and N be negatives. P = (total + target) / 2. Count subsets summing to P.
// - Complexity: Time: O(N * (Total + Target) / 2), Space: O(P).


// =========================================================
// 13. PARTITION EQUAL SUBSET SUM
// =========================================================

bool canPartition(vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total % 2 != 0) return false;
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int x : nums) {
        for (int i = target; i >= x; --i) {
            dp[i] = dp[i] || dp[i - x];
        }
    }
    return dp[target];
}
// Interview Explanation:
// - Problem Statement: Determine if array can be partitioned into two subsets with equal sum.
// - Approach: 0/1 Knapsack Boolean DP.
// - Intuition: Check if a subset of sum = total / 2 exists; traverse backwards to reuse space in 1D.
// - Complexity: Time: O(N * Sum / 2), Space: O(Sum / 2).


// =========================================================
// 14. PALINDROME PARTITIONING
// =========================================================

class SolutionPalindromePartitioning {
    bool isPalindrome(const string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }

    void backtrack(const string& s, int start, vector<string>& curr, vector<vector<string>>& res) {
        if (start == (int)s.size()) {
            res.push_back(curr);
            return;
        }

        for (int end = start; end < (int)s.size(); ++end) {
            if (isPalindrome(s, start, end)) {
                curr.push_back(s.substr(start, end - start + 1));
                backtrack(s, end + 1, curr, res);
                curr.pop_back();
            }
        }
    }

public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> curr;
        backtrack(s, 0, curr, res);
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Partition string such that every substring is a palindrome.
// - Approach: Backtracking with Substring Palindrome Check.
// - Intuition: For every prefix, if it is a palindrome, recurse on remaining suffix; backtrack when done.
// - Complexity: Time: O(N * 2^N), Space: O(N) recursion stack.


// =========================================================
// 15. N-QUEENS
// =========================================================

class SolutionNQueens {
    vector<vector<string>> result;
    vector<bool> cols, diag1, diag2;

    void backtrack(int r, int n, vector<string>& board) {
        if (r == n) {
            result.push_back(board);
            return;
        }

        for (int c = 0; c < n; ++c) {
            int d1 = r - c + n;
            int d2 = r + c;
            if (cols[c] || diag1[d1] || diag2[d2]) continue;

            board[r][c] = 'Q';
            cols[c] = diag1[d1] = diag2[d2] = true;

            backtrack(r + 1, n, board);

            board[r][c] = '.';
            cols[c] = diag1[d1] = diag2[d2] = false;
        }
    }

public:
    vector<vector<string>> solveNQueens(int n) {
        result.clear();
        cols.assign(n, false);
        diag1.assign(2 * n, false);
        diag2.assign(2 * n, false);

        vector<string> board(n, string(n, '.'));
        backtrack(0, n, board);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Place n queens on n x n chessboard with no two attacking each other.
// - Approach: Backtracking with O(1) Constraint Lookup Arrays.
// - Intuition: Diagonals are indexed by r - c + n and r + c; eliminates O(N) validity checks per placement.
// - Complexity: Time: O(N!), Space: O(N) state arrays.


// =========================================================
// 16. SUDOKU SOLVER
// =========================================================

class SolutionSudoku {
    bool isValid(vector<vector<char>>& board, int r, int c, char ch) {
        for (int i = 0; i < 9; ++i) {
            if (board[r][i] == ch) return false;
            if (board[i][c] == ch) return false;
            if (board[3 * (r / 3) + i / 3][3 * (c / 3) + i % 3] == ch) return false;
        }
        return true;
    }

    bool solve(vector<vector<char>>& board) {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] == '.') {
                    for (char ch = '1'; ch <= '9'; ++ch) {
                        if (isValid(board, r, c, ch)) {
                            board[r][c] = ch;
                            if (solve(board)) return true;
                            board[r][c] = '.';
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

public:
    void solveSudoku(vector<vector<char>>& board) {
        solve(board);
    }
};
// Interview Explanation:
// - Problem Statement: Solve standard 9x9 Sudoku puzzle.
// - Approach: Recursive Backtracking with Row/Col/Box Validity Invariant.
// - Intuition: Find empty cell, try digits '1'-'9', recurse; backtrack if dead-end reached.
// - Complexity: Time: O(9^(empty_cells)), Space: O(81) = O(1).


// =========================================================
// 17. TASK SCHEDULER
// =========================================================

int leastInterval(vector<char>& tasks, int n) {
    vector<int> freq(26, 0);
    int maxFreq = 0;
    for (char t : tasks) {
        freq[t - 'A']++;
        maxFreq = max(maxFreq, freq[t - 'A']);
    }

    int countMaxFreq = 0;
    for (int f : freq) {
        if (f == maxFreq) countMaxFreq++;
    }

    // (maxFreq - 1) chunks of (n + 1) slots + tasks that have maxFreq
    int emptySlots = (maxFreq - 1) * (n + 1) + countMaxFreq;
    return max((int)tasks.size(), emptySlots);
}
// Interview Explanation:
// - Problem Statement: Minimum CPU intervals to finish tasks with cooldown n between identical tasks.
// - Approach: Greedy Max-Frequency Math Formula.
// - Intuition: Bottleneck is highest frequency tasks; arrange them with n-gap frames, fill empty slots with remaining tasks.
// - Complexity: Time: O(N), Space: O(1) (26 letters).


// =========================================================
// 18. INSERT INTERVAL
// =========================================================

vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> result;
    int i = 0, n = intervals.size();

    // 1. Add all intervals ending before newInterval starts
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }

    // 2. Merge all overlapping intervals with newInterval
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);

    // 3. Add all remaining intervals starting after newInterval ends
    while (i < n) {
        result.push_back(intervals[i++]);
    }

    return result;
}
// Interview Explanation:
// - Problem Statement: Insert newInterval into sorted non-overlapping intervals and merge if necessary.
// - Approach: 3-Stage Linear Sweep.
// - Intuition: Strictly prior intervals -> overlapping interval union -> strictly subsequent intervals.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.


// =========================================================
// 19. NEXT GREATER ELEMENT II (CIRCULAR ARRAY)
// =========================================================

vector<int> nextGreaterElements(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, -1);
    stack<int> st; // stores indices

    for (int i = 0; i < 2 * n; ++i) {
        int val = nums[i % n];
        while (!st.empty() && nums[st.top()] < val) {
            res[st.top()] = val;
            st.pop();
        }
        if (i < n) st.push(i);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find next greater element in a circular array.
// - Approach: Monotonic Decreasing Stack over 2N Virtual Iterations.
// - Intuition: Simulating array concatenation (i % n) allows resolving wrapped cyclic lookups in linear time.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 20. SMALLEST RANGE COVERING ELEMENTS FROM K LISTS
// =========================================================

vector<int> smallestRange(vector<vector<int>>& nums) {
    // {val, listIdx, elemIdx}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    int currMax = INT_MIN;

    for (int i = 0; i < (int)nums.size(); ++i) {
        pq.push({nums[i][0], i, 0});
        currMax = max(currMax, nums[i][0]);
    }

    int start = 0, end = INT_MAX;

    while (pq.size() == nums.size()) {
        auto [currMin, listIdx, elemIdx] = pq.top();
        pq.pop();

        if (currMax - currMin < end - start) {
            start = currMin;
            end = currMax;
        }

        if (elemIdx + 1 < (int)nums[listIdx].size()) {
            int nextVal = nums[listIdx][elemIdx + 1];
            pq.push({nextVal, listIdx, elemIdx + 1});
            currMax = max(currMax, nextVal);
        }
    }
    return {start, end};
}
// Interview Explanation:
// - Problem Statement: Find smallest range [a, b] that includes at least one number from each of k sorted lists.
// - Approach: Min-Heap Multi-Pointer Sliding Window.
// - Intuition: Heap maintains current minimum across lists while currMax tracks upper bound. Narrow range greedily.
// - Complexity: Time: O(N log K), Space: O(K).


// =========================================================
// 21. THE SKYLINE PROBLEM
// =========================================================

vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
    vector<pair<int, int>> events; // {x, height} (negative height for start, positive for end)
    for (const auto& b : buildings) {
        events.push_back({b[0], -b[2]}); // Start
        events.push_back({b[1], b[2]});  // End
    }

    sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    multiset<int> heights = {0};
    int prevMax = 0;
    vector<vector<int>> result;

    for (const auto& [x, h] : events) {
        if (h < 0) {
            heights.insert(-h); // Add building
        } else {
            heights.erase(heights.find(h)); // Remove building
        }

        int currMax = *heights.rbegin();
        if (currMax != prevMax) {
            result.push_back({x, currMax});
            prevMax = currMax;
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Output key contour points of 2D city skyline buildings.
// - Approach: Sweep-Line Algorithm with Ordered Multiset of Active Heights.
// - Intuition: Sort critical X points; when maximum active height changes, record new skyline keypoint.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 22. LONGEST PALINDROMIC SUBSEQUENCE
// =========================================================

int longestPalindromeSubseq(string s) {
    int n = s.size();
    // dp[i][j] stores LPS length in s[i..j]
    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = n - 1; i >= 0; --i) {
        dp[i][i] = 1;
        for (int j = i + 1; j < n; ++j) {
            if (s[i] == s[j]) {
                dp[i][j] = 2 + dp[i + 1][j - 1];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find length of longest palindromic subsequence.
// - Approach: 2D Interval Dynamic Programming.
// - Intuition: If s[i] == s[j], LPS = 2 + dp[i+1][j-1]; otherwise, take max of dropping s[i] or s[j].
// - Complexity: Time: O(N^2), Space: O(N^2).


// =========================================================
// 23. MAXIMUM FREQUENCY STACK (FREQSTACK)
// =========================================================

class FreqStack {
    unordered_map<int, int> freq;               // val -> frequency
    unordered_map<int, stack<int>> groupStack; // freq -> stack of values
    int maxFreq;

public:
    FreqStack() : maxFreq(0) {}

    void push(int val) {
        int f = ++freq[val];
        maxFreq = max(maxFreq, f);
        groupStack[f].push(val);
    }

    int pop() {
        int val = groupStack[maxFreq].top();
        groupStack[maxFreq].pop();
        freq[val]--;
        if (groupStack[maxFreq].empty()) {
            maxFreq--;
        }
        return val;
    }
};
// Interview Explanation:
// - Problem Statement: Stack that pops the most frequent element; ties broken by closest to stack top.
// - Approach: Hash Map of Frequency Stacks + maxFreq tracker.
// - Intuition: Grouping elements into separate stacks per frequency achieves O(1) push and pop effortlessly.
// - Complexity: Time: O(1) for push and pop, Space: O(N).


// =========================================================
// 24. MINIMUM COST TO HIRE K WORKERS
// =========================================================

double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
    int n = quality.size();
    // {wage/quality ratio, quality}
    vector<pair<double, int>> workers(n);
    for (int i = 0; i < n; ++i) {
        workers[i] = {(double)wage[i] / quality[i], quality[i]};
    }

    sort(workers.begin(), workers.end());

    priority_queue<int> maxHeap; // stores qualities of current K workers
    int qualitySum = 0;
    double minCost = 1e18;

    for (const auto& [ratio, q] : workers) {
        maxHeap.push(q);
        qualitySum += q;

        if ((int)maxHeap.size() > k) {
            qualitySum -= maxHeap.top();
            maxHeap.pop();
        }

        if ((int)maxHeap.size() == k) {
            minCost = min(minCost, qualitySum * ratio);
        }
    }
    return minCost;
}
// Interview Explanation:
// - Problem Statement: Hire k workers minimizing total cost while satisfying wage-to-quality ratio constraints.
// - Approach: Greedy Ratio Sorting + Max-Heap of Qualities.
// - Intuition: Sort by ratio so current ratio is the bottleneck; greedily evict largest quality to minimize total sum.
// - Complexity: Time: O(N log N + N log K), Space: O(N + K).


// =========================================================
// 25. LONGEST DUPLICATE SUBSTRING (ROBIN-KARP BINARY SEARCH)
// =========================================================

string longestDupSubstring(string s) {
    int n = s.size();
    int low = 1, high = n - 1;
    string result = "";

    auto search = [&](int len) -> string {
        if (len == 0) return "";
        ll h = 0, p = 31, pPow = 1;
        for (int i = 0; i < len; ++i) {
            h = (h * p + (s[i] - 'a' + 1)) % MOD;
            if (i < len - 1) pPow = (pPow * p) % MOD;
        }

        unordered_map<ll, vector<int>> seen;
        seen[h].push_back(0);

        for (int i = len; i < n; ++i) {
            // Rolling hash update
            h = (h - (s[i - len] - 'a' + 1) * pPow % MOD + MOD) % MOD;
            h = (h * p + (s[i] - 'a' + 1)) % MOD;

            if (seen.count(h)) {
                string currSub = s.substr(i - len + 1, len);
                for (int prevStart : seen[h]) {
                    if (s.substr(prevStart, len) == currSub) {
                        return currSub;
                    }
                }
            }
            seen[h].push_back(i - len + 1);
        }
        return "";
    };

    while (low <= high) {
        int mid = low + (high - low) / 2;
        string dup = search(mid);
        if (!dup.empty()) {
            result = dup;
            low = mid + 1; // Try longer duplicate
        } else {
            high = mid - 1;
        }
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Find longest duplicate substring in s in O(N log N) time.
// - Approach: Binary Search on Substring Length + Rabin-Karp Polynomial Rolling Hash.
// - Intuition: If a duplicate of length L exists, duplicates of length < L must also exist (monotonicity).
// - Complexity: Time: O(N log N) average, Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Design Hit Counter (Rate Limiter)           | 300-Slot Circular Ring Buffer     | O(1)     | O(1)     |
 | 2  | Find Duplicate Subtrees                     | Post-Order Serialization Hashing  | O(N^2)   | O(N^2)   |
 | 3  | Boundary Traversal of Binary Tree           | 3-Stage Modular Boundary Traversal| O(N)     | O(H)     |
 | 4  | Lowest Common Ancestor of Deepest Leaves    | Bottom-Up Post-Order {depth, LCA} | O(N)     | O(H)     |
 | 5  | Flatten Binary Tree to Linked List          | Morris Traversal Rewiring         | O(N)     | O(1)     |
 | 6  | Binary Tree Zigzag Level Order Traversal    | Level BFS + Directional Mapping   | O(N)     | O(W)     |
 | 7  | Path Sum III                                | Tree DFS + Prefix Sum Map Backtrck| O(N)     | O(H)     |
 | 8  | Shortest Bridge                             | DFS (Paint) + Multi-Source BFS    | O(N^2)   | O(N^2)   |
 | 9  | Reconstruct Itinerary                       | Hierholzer's Algorithm (Euler Path| O(E logE)| O(V + E) |
 | 10 | Word Break II                               | Top-Down DFS with Memoization     | O(N*2^N) | O(N * 2^N|
 | 11 | Coin Change II                              | 1D DP (Coin Outer Loop)           | O(N*Amnt)| O(Amount)|
 | 12 | Target Sum                                  | 0/1 Knapsack Math Reduction       | O(N * P) | O(P)     |
 | 13 | Partition Equal Subset Sum                  | 0/1 Knapsack Boolean DP           | O(N * Sum| O(Sum)   |
 | 14 | Palindrome Partitioning                     | Backtracking DFS + Palindrome Chk | O(N*2^N) | O(N)     |
 | 15 | N-Queens                                    | Backtracking with O(1) Bitmaps    | O(N!)    | O(N)     |
 | 16 | Sudoku Solver                               | Backtracking + 9-Box Validity     | O(9^E)   | O(1)     |
 | 17 | Task Scheduler                              | Greedy Max-Frequency Math Formula | O(N)     | O(1)     |
 | 18 | Insert Interval                             | 3-Stage Linear Sweep              | O(N)     | O(1)     |
 | 19 | Next Greater Element II (Circular Array)    | Monotonic Decreasing Stack (2N)   | O(N)     | O(N)     |
 | 20 | Smallest Range from K Lists                 | Min-Heap Multi-Pointer Window     | O(N logK)| O(K)     |
 | 21 | The Skyline Problem                         | Sweep-Line + Ordered Multiset     | O(N logN)| O(N)     |
 | 22 | Longest Palindromic Subsequence             | 2D Interval Dynamic Programming   | O(N^2)   | O(N^2)   |
 | 23 | Maximum Frequency Stack (FreqStack)         | Frequency Stacks Map + maxFreq    | O(1)     | O(N)     |
 | 24 | Minimum Cost to Hire K Workers              | Greedy Ratio Sort + Max-Heap Qual | O(N logN)| O(N + K) |
 | 25 | Longest Duplicate Substring                 | Binary Search + Rabin-Karp Hash   | O(N logN)| O(N)     |
 ====================================================================================================
*/
