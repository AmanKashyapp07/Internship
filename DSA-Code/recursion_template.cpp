/**
 * ====================================================================================================
 *                 ULTIMATE RECURSION & BACKTRACKING MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Subsets, Combinations, Permutations, String Partitioning,
 * Grid Pathfinding, N-Queens, Sudoku Solver, Graph M-Coloring, and Bitmask Memoization.
 */

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
const ll P = 31;

// ====================================================================================================
// SECTION 1: SUBSETS & SUBSEQUENCES
// ====================================================================================================

// 1. Subsets / Power Set (LeetCode 78)
class SubsetsSolver {
    void dfs(int start, const vi &nums, vi &cur, vvi &ans) {
        ans.push_back(cur);
        for (int i = start; i < (int)nums.size(); i++) {
            cur.push_back(nums[i]);
            dfs(i + 1, nums, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi subsets(const vi &nums) {
        vvi ans;
        vi cur;
        dfs(0, nums, cur, ans);
        return ans;
    }
};

// 2. Subsets II with Duplicates (LeetCode 90)
class SubsetsIISolver {
    void dfs(int start, const vi &nums, vi &cur, vvi &ans) {
        ans.push_back(cur);
        for (int i = start; i < (int)nums.size(); i++) {
            if (i > start && nums[i] == nums[i - 1]) continue;
            cur.push_back(nums[i]);
            dfs(i + 1, nums, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi subsetsWithDup(vi nums) {
        sort(nums.begin(), nums.end());
        vvi ans;
        vi cur;
        dfs(0, nums, cur, ans);
        return ans;
    }
};

// 3. Non-Decreasing Subsequences (LeetCode 491)
class NonDecreasingSubsequencesSolver {
    void dfs(int idx, const vi &nums, vi &cur, vvi &ans) {
        if (cur.size() >= 2) ans.push_back(cur);
        unordered_set<int> usedInCurrentLevel;
        for (int i = idx; i < (int)nums.size(); i++) {
            if (!cur.empty() && nums[i] < cur.back()) continue;
            if (usedInCurrentLevel.count(nums[i])) continue;
            usedInCurrentLevel.insert(nums[i]);
            cur.push_back(nums[i]);
            dfs(i + 1, nums, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi findSubsequences(const vi &nums) {
        vvi ans;
        vi cur;
        dfs(0, nums, cur, ans);
        return ans;
    }
};


// ====================================================================================================
// SECTION 2: COMBINATIONS
// ====================================================================================================

// 1. Combinations of K numbers out of 1..N (LeetCode 77)
class CombinationsSolver {
    void dfs(int start, int n, int k, vi &cur, vvi &ans) {
        if ((int)cur.size() == k) { ans.push_back(cur); return; }
        for (int i = start; i <= n; i++) {
            cur.push_back(i);
            dfs(i + 1, n, k, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi combine(int n, int k) {
        vvi ans;
        vi cur;
        dfs(1, n, k, cur, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Given n and k, return all possible combinations of k numbers chosen from range 1..n (LeetCode 77).
// - Approach: Backtracking combination generator.
// - Intuition: Systematically pick numbers from start to n; when `cur.size() == k`, record combination.
// - Complexity: Time: O(\binom{N}{K} \cdot K), Space: O(K) call stack.

// 2. Combination Sum I (LeetCode 39 - Unlimited Element Reuse)
class CombinationSumISolver {
    void dfs(int idx, int target, const vi &nums, vi &cur, vvi &ans) {
        if (target == 0) { ans.push_back(cur); return; }
        if (idx == (int)nums.size() || target < 0) return;

        cur.push_back(nums[idx]);
        dfs(idx, target - nums[idx], nums, cur, ans);
        cur.pop_back();

        dfs(idx + 1, target, nums, cur, ans);
    }
public:
    vvi combinationSum(const vi &candidates, int target) {
        vvi ans;
        vi cur;
        dfs(0, target, candidates, cur, ans);
        return ans;
    }
};

// 3. Combination Sum II (LeetCode 40 - Single Use + Duplicates)
class CombinationSumIISolver {
    void dfs(int start, int target, const vi &nums, vi &cur, vvi &ans) {
        if (target == 0) { ans.push_back(cur); return; }
        for (int i = start; i < (int)nums.size(); i++) {
            if (i > start && nums[i] == nums[i - 1]) continue;
            if (nums[i] > target) break;
            cur.push_back(nums[i]);
            dfs(i + 1, target - nums[i], nums, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi combinationSum2(vi candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vvi ans;
        vi cur;
        dfs(0, target, candidates, cur, ans);
        return ans;
    }
};

// 4. Combination Sum III (LeetCode 216 - Digits 1..9)
class CombinationSumIIISolver {
    void dfs(int start, int k, int target, vi &cur, vvi &ans) {
        if ((int)cur.size() == k && target == 0) { ans.push_back(cur); return; }
        if ((int)cur.size() > k || target < 0) return;
        for (int i = start; i <= 9; i++) {
            cur.push_back(i);
            dfs(i + 1, k, target - i, cur, ans);
            cur.pop_back();
        }
    }
public:
    vvi combinationSum3(int k, int n) {
        vvi ans;
        vi cur;
        dfs(1, k, n, cur, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all valid combinations of k numbers that sum up to n using digits 1..9 each at most once (LeetCode 216).
// - Approach: Range [1..9] backtracking with target and count invariants.
// - Intuition: Recursively pick digits from start..9, pruning when count > k or target < 0.
// - Complexity: Time: O(\binom{9}{K} \cdot K), Space: O(K).


// ====================================================================================================
// SECTION 3: PERMUTATIONS
// ====================================================================================================

// 1. Permutations of Array (LeetCode 46 - In-place Swap)
class PermutationsSolver {
    void dfs(int idx, vi &nums, vvi &ans) {
        if (idx == (int)nums.size()) { ans.push_back(nums); return; }
        for (int i = idx; i < (int)nums.size(); i++) {
            swap(nums[idx], nums[i]);
            dfs(idx + 1, nums, ans);
            swap(nums[idx], nums[i]);
        }
    }
public:
    vvi permute(vi nums) {
        vvi ans;
        dfs(0, nums, ans);
        return ans;
    }
};

// 2. Permutations II with Duplicates (LeetCode 47)
class PermutationsIISolver {
    void dfs(int idx, vi nums, vvi &ans) {
        if (idx == (int)nums.size()) { ans.push_back(nums); return; }
        unordered_set<int> visited;
        for (int i = idx; i < (int)nums.size(); i++) {
            if (visited.count(nums[i])) continue;
            visited.insert(nums[i]);
            swap(nums[idx], nums[i]);
            dfs(idx + 1, nums, ans);
        }
    }
public:
    vvi permuteUnique(vi nums) {
        sort(nums.begin(), nums.end());
        vvi ans;
        dfs(0, nums, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Return all unique permutations of an array containing duplicates (LeetCode 47).
// - Approach: Level-specific unordered_set deduplication during in-place swapping.
// - Intuition: Prevent swapping identical values into position `idx` at the same recursion depth.
// - Complexity: Time: O(N! \cdot N), Space: O(N).

// 3. K-th Permutation Sequence (LeetCode 60 - Factorial Number System)
string getPermutation(int n, int k) {
    vi nums;
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        nums.push_back(i);
        if (i < n) fact *= i;
    }
    k--;
    string ans;
    while (!nums.empty()) {
        int pos = k / fact;
        ans += to_string(nums[pos]);
        nums.erase(nums.begin() + pos);
        if (nums.empty()) break;
        k %= fact;
        fact /= nums.size();
    }
    return ans;
}


// ====================================================================================================
// SECTION 4: PARTITIONING & STRING SEGMENTATION
// ====================================================================================================

// 1. Palindrome Partitioning (LeetCode 131 - DP Table + Backtracking)
class PalindromePartitioningSolver {
    void dfs(int idx, const string &s, const vector<vector<bool>> &pal, vector<string> &cur, vector<vector<string>> &ans) {
        if (idx == (int)s.size()) { ans.push_back(cur); return; }
        for (int j = idx; j < (int)s.size(); j++) {
            if (pal[idx][j]) {
                cur.push_back(s.substr(idx, j - idx + 1));
                dfs(j + 1, s, pal, cur, ans);
                cur.pop_back();
            }
        }
    }
public:
    vector<vector<string>> partition(const string &s) {
        int n = s.size();
        vector<vector<bool>> pal(n, vector<bool>(n, false));
        for (int i = 0; i < n; i++) pal[i][i] = true;
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) pal[i][j] = (len <= 3) || pal[i + 1][j - 1];
            }
        }
        vector<vector<string>> ans;
        vector<string> cur;
        dfs(0, s, pal, cur, ans);
        return ans;
    }
};

// 2. Word Break II - All Valid Sentences (LeetCode 140)
class WordBreakIISolver {
    unordered_map<int, vector<string>> memo;
    unordered_set<string> dict;

    vector<string> dfs(int start, const string &s) {
        if (memo.count(start)) return memo[start];
        vector<string> validSentences;
        if (start == (int)s.size()) { validSentences.push_back(""); return validSentences; }

        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                vector<string> subSentences = dfs(end, s);
                for (const string &sub : subSentences) {
                    validSentences.push_back(word + (sub.empty() ? "" : " ") + sub);
                }
            }
        }
        return memo[start] = validSentences;
    }
public:
    vector<string> wordBreak(string s, vector<string> &wordDict) {
        dict = unordered_set<string>(wordDict.begin(), wordDict.end());
        return dfs(0, s);
    }
};
// Interview Explanation:
// - Problem Statement: Reconstruct all valid sentences formed by space-separated dictionary words (LeetCode 140).
// - Approach: Memoized Backtracking storing suffixes.
// - Intuition: Memoizing `memo[start]` avoids re-constructing sentence combinations for overlapping suffixes.
// - Complexity: Time: O(2^N + N^2), Space: O(2^N).


// ====================================================================================================
// SECTION 5: GRID PATHFINDING & BOARD BACKTRACKING
// ====================================================================================================

// 1. Rat in a Maze
class RatInAMazeSolver {
    const int dr[4] = {1, 0, 0, -1};
    const int dc[4] = {0, -1, 1, 0};
    const char dir[4] = {'D', 'L', 'R', 'U'};

    void dfs(int r, int c, int n, const vvi &grid, vvi &vis, string &path, vector<string> &ans) {
        if (r == n - 1 && c == n - 1) { ans.push_back(path); return; }
        vis[r][c] = 1;
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 1 && !vis[nr][nc]) {
                path.push_back(dir[k]);
                dfs(nr, nc, n, grid, vis, path, ans);
                path.pop_back();
            }
        }
        vis[r][c] = 0;
    }
public:
    vector<string> findPath(const vvi &grid) {
        int n = grid.size();
        if (n == 0 || grid[0][0] == 0 || grid[n - 1][n - 1] == 0) return {};
        vvi vis(n, vi(n, 0));
        vector<string> ans;
        string path;
        dfs(0, 0, n, grid, vis, path, ans);
        sort(ans.begin(), ans.end());
        return ans;
    }
};

// 2. Word Search in 2D Grid (LeetCode 79)
class WordSearchSolver {
    bool dfs(int r, int c, int idx, vector<vector<char>> &board, const string &word) {
        if (idx == (int)word.size()) return true;
        if (r < 0 || r >= (int)board.size() || c < 0 || c >= (int)board[0].size() || board[r][c] != word[idx])
            return false;

        char temp = board[r][c];
        board[r][c] = '#';

        bool found = dfs(r + 1, c, idx + 1, board, word) ||
                     dfs(r - 1, c, idx + 1, board, word) ||
                     dfs(r, c + 1, idx + 1, board, word) ||
                     dfs(r, c - 1, idx + 1, board, word);

        board[r][c] = temp;
        return found;
    }
public:
    bool exist(vector<vector<char>> &board, const string &word) {
        int n = board.size(), m = board[0].size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (board[i][j] == word[0] && dfs(i, j, 0, board, word)) return true;
            }
        }
        return false;
    }
};

// 3. N-Queens Problem (LeetCode 51)
class NQueensSolver {
    void dfs(int row, int n, vi &cols, vi &diag1, vi &diag2, vector<string> &board, vector<vector<string>> &ans) {
        if (row == n) { ans.push_back(board); return; }
        for (int col = 0; col < n; col++) {
            int d1 = row - col + n, d2 = row + col;
            if (!cols[col] && !diag1[d1] && !diag2[d2]) {
                board[row][col] = 'Q';
                cols[col] = diag1[d1] = diag2[d2] = 1;
                dfs(row + 1, n, cols, diag1, diag2, board, ans);
                board[row][col] = '.';
                cols[col] = diag1[d1] = diag2[d2] = 0;
            }
        }
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> ans;
        vector<string> board(n, string(n, '.'));
        vi cols(n, 0), diag1(2 * n, 0), diag2(2 * n, 0);
        dfs(0, n, cols, diag1, diag2, board, ans);
        return ans;
    }
};

// 4. Sudoku Solver (LeetCode 37)
class SudokuSolver {
    bool isValid(int r, int c, char ch, const vector<vector<char>> &board) {
        for (int i = 0; i < 9; i++) {
            if (board[r][i] == ch) return false;
            if (board[i][c] == ch) return false;
            if (board[3 * (r / 3) + i / 3][3 * (c / 3) + i % 3] == ch) return false;
        }
        return true;
    }

    bool solve(vector<vector<char>> &board) {
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if (board[r][c] == '.') {
                    for (char ch = '1'; ch <= '9'; ch++) {
                        if (isValid(r, c, ch, board)) {
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
    void solveSudoku(vector<vector<char>> &board) {
        solve(board);
    }
};

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: RECURSION & BACKTRACKING
 ====================================================================================================
 1. CANONICAL BACKTRACKING BLUEPRINT:
    Choose -> Explore -> Unchoose.
 ====================================================================================================
*/


// ====================================================================================================
// SECTION: ADVANCED BACKTRACKING EXTENSIONS
// ====================================================================================================

// 1. Next Permutation (LeetCode 31)
void nextPermutation(vi& nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;
    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}

// 2. Restore IP Addresses (LeetCode 93)
class RestoreIPSolver {
    void dfs(int idx, int dots, string& s, string curr, vector<string>& ans) {
        if (dots == 4 && idx == (int)s.size()) {
            curr.pop_back(); // Remove trailing dot
            ans.push_back(curr);
            return;
        }
        if (dots > 4) return;
        for (int len = 1; len <= 3 && idx + len <= (int)s.size(); ++len) {
            string segment = s.substr(idx, len);
            if ((segment.size() > 1 && segment[0] == '0') || stoi(segment) > 255) continue;
            dfs(idx + len, dots + 1, s, curr + segment + ".", ans);
        }
    }
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> ans;
        dfs(0, 0, s, "", ans);
        return ans;
    }
};


// ====================================================================================================
// SECTION 6: STRIVER SDE SHEET RECURSION ALGORITHMS (CATEGORY 4)
// ====================================================================================================

// 1. Subset Sums (GFG / Striver SDE #47)
void subsetSumsHelper(int idx, int sum, const vi& arr, vi& ans) {
    if (idx == (int)arr.size()) {
        ans.push_back(sum);
        return;
    }
    subsetSumsHelper(idx + 1, sum + arr[idx], arr, ans); // Pick
    subsetSumsHelper(idx + 1, sum, arr, ans);            // Non-pick
}

vi subsetSums(const vi& arr) {
    vi ans;
    subsetSumsHelper(0, 0, arr, ans);
    sort(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Calculate sum of all possible subsets of an array and return in sorted order.
// - Approach: Pick / Non-Pick Binary Tree Recursion.
// - Intuition: Each element has two choices (include or exclude in current subset sum).
// - Complexity: Time: O(2^N + 2^N \log(2^N)) = O(2^N \cdot N), Space: O(N) recursion stack.

// 2. M-Coloring Problem (GFG / Striver SDE #57)
bool isColorSafe(int node, int col, const vector<int>& color, const vector<vector<int>>& graph) {
    for (int neighbor : graph[node]) {
        if (color[neighbor] == col) return false;
    }
    return true;
}

bool solveMColoring(int node, vector<int>& color, int m, int n, const vector<vector<int>>& graph) {
    if (node == n) return true;

    for (int col = 1; col <= m; col++) {
        if (isColorSafe(node, col, color, graph)) {
            color[node] = col;
            if (solveMColoring(node + 1, color, m, n, graph)) return true;
            color[node] = 0; // backtrack
        }
    }
    return false;
}

bool graphColoring(vector<vector<int>>& graph, int m, int n) {
    vector<int> color(n, 0);
    return solveMColoring(0, color, m, n, graph);
}
// Interview Explanation:
// - Problem Statement: Determine if an undirected graph can be colored with at most M colors such that no adjacent vertices share the same color.
// - Approach: Backtracking vertex assignment from 1 to M.
// - Intuition: Try colors 1..M on vertex `node`; if safe, recursively color `node + 1`. If no color works, backtrack.
// - Complexity: Time: O(M^N), Space: O(N) recursion stack and color array.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
