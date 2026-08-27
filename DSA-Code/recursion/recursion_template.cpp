/**
 * ====================================================================================================
 *                 ULTIMATE RECURSION & BACKTRACKING MASTER BLUEPRINT
 * ====================================================================================================
 * Consolidated Master Template covering Subsets, Combinations, Permutations, String Partitioning,
 * Grid Pathfinding, N-Queens, Sudoku Solver, Graph M-Coloring, and Bitmask Memoization.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vvi = vector<vector<int>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

// ====================================================================================================
// SECTION 1: SUBSETS & SUBSEQUENCES
// ====================================================================================================

// 1. Subsets / Power Set (LeetCode 78)
class SubsetsSolver {
    void dfs(int start, const vi &nums, vi &cur, vvi &ans) {
        ans.push_back(cur);
        for (int i = start; i < nums.size(); i++) {
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
// Interview Explanation:
// - Problem Statement: Given an array of unique integers, return all 2^N possible subsets (the power set).
// - Approach: Backtracking DFS exploring all inclusion choices.
// - Intuition: At each depth, the accumulated path represents a valid subset; iterating from start to end systematically branches without duplicates.
// - Complexity: Time: O(N \cdot 2^N), Space: O(N) recursion stack.


// 2. Subsets II with Duplicates (LeetCode 90)
class SubsetsIISolver {
    void dfs(int start, const vi &nums, vi &cur, vvi &ans) {
        ans.push_back(cur);
        for (int i = start; i < nums.size(); i++) {
            if (i > (int)start && nums[i] == nums[i - 1]) continue; // Skip identical elements at same depth
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
// Interview Explanation:
// - Problem Statement: Generate all unique subsets from an array that may contain duplicates.
// - Approach: Sort array and skip duplicate siblings at the same recursion depth.
// - Intuition: `i > start && nums[i] == nums[i-1]` ensures duplicate values are only picked in sequential order, preventing redundant branches.
// - Complexity: Time: O(N \cdot 2^N), Space: O(N).


// 3. Non-Decreasing Subsequences (LeetCode 491)
class NonDecreasingSubsequencesSolver {
    void dfs(int idx, const vi &nums, vi &cur, vvi &ans) {
        if (cur.size() >= 2) ans.push_back(cur);
        unordered_set<int> usedInCurrentLevel;
        for (int i = idx; i < nums.size(); i++) {
            if (!cur.empty() && nums[i] < cur.back()) continue;
            if (usedInCurrentLevel.count(nums[i])) continue; // Deduplicate without sorting
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
// Interview Explanation:
// - Problem Statement: Find all unique non-decreasing subsequences with length >= 2 from an unsorted array.
// - Approach: Backtracking with local unordered_set deduplication at each level (cannot sort original array).
// - Intuition: Array ordering must be preserved, so duplicate values picked at the same recursion depth are filtered via a level-specific hash set.
// - Complexity: Time: O(2^N \cdot N), Space: O(N) recursion stack.


// ====================================================================================================
// SECTION 2: COMBINATIONS
// ====================================================================================================

// 1. Combination Sum I (LeetCode 39 - Unlimited Element Reuse)
class CombinationSumISolver {
    void dfs(int idx, int target, const vi &nums, vi &cur, vvi &ans) {
        if (target == 0) { ans.push_back(cur); return; }
        if (idx == (int)nums.size() || target < 0) return;

        // Choice 1: Include current candidate (stay at index idx for reuse)
        cur.push_back(nums[idx]);
        dfs(idx, target - nums[idx], nums, cur, ans);
        cur.pop_back();

        // Choice 2: Exclude current candidate (advance to idx + 1)
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
// Interview Explanation:
// - Problem Statement: Find all unique combinations of distinct candidates summing to target, allowing unlimited reuse.
// - Approach: Pick or skip backtracking; stay at index idx when picking to allow reuse.
// - Intuition: Branch into picking nums[idx] (target - nums[idx]) vs. skipping (idx + 1); base case target == 0 records combination.
// - Complexity: Time: O(2^{\text{target}/\min}), Space: O(\text{target}/\min) call stack.


// 2. Combination Sum II (LeetCode 40 - Single Use + Duplicate Elements)
class CombinationSumIISolver {
    void dfs(int start, int target, const vi &nums, vi &cur, vvi &ans) {
        if (target == 0) { ans.push_back(cur); return; }
        for (int i = start; i < nums.size(); i++) {
            if (i > (int)start && nums[i] == nums[i - 1]) continue; // Skip duplicates
            if (nums[i] > target) break; // Pruning: sorted array
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
// Interview Explanation:
// - Problem Statement: Find all unique combinations summing to target where each candidate may only be used once.
// - Approach: Sort candidates + loop-based backtracking skipping duplicate siblings (`i > start && nums[i] == nums[i-1]`).
// - Intuition: Sorting allows early termination when nums[i] > target and cleanly prunes duplicate combination branches.
// - Complexity: Time: O(2^N), Space: O(N) recursion stack.


// 3. Letter Combinations of a Phone Number (LeetCode 17)
class PhoneKeypadSolver {
    const vector<string> mapping = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    void dfs(int idx, const string &digits, string &cur, vector<string> &ans) {
        if (idx == (int)digits.size()) { ans.push_back(cur); return; }
        for (char ch : mapping[digits[idx] - '0']) {
            cur.push_back(ch);
            dfs(idx + 1, digits, cur, ans);
            cur.pop_back();
        }
    }
public:
    vector<string> letterCombinations(const string &digits) {
        if (digits.empty()) return {};
        vector<string> ans;
        string cur;
        dfs(0, digits, cur, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Generate all possible letter combinations that a digit string from 2-9 could represent on a phone keypad.
// - Approach: Digit-by-digit recursion branching across character mappings.
// - Intuition: Each digit branches by 3 or 4 choices; depth of recursion tree is exactly length of digits.
// - Complexity: Time: O(4^N \cdot N), Space: O(N) recursion stack.


// 4. Generate Parentheses (LeetCode 22)
class GenerateParenthesesSolver {
    void dfs(int open, int close, int n, string &cur, vector<string> &ans) {
        if ((int)cur.size() == 2 * n) { ans.push_back(cur); return; }
        if (open < n) {
            cur.push_back('(');
            dfs(open + 1, close, n, cur, ans);
            cur.pop_back();
        }
        if (close < open) {
            cur.push_back(')');
            dfs(open, close + 1, n, cur, ans);
            cur.pop_back();
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        vector<string> ans;
        string cur;
        dfs(0, 0, n, cur, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Generate all combinations of well-formed parentheses for n pairs.
// - Approach: Backtracking guided by open and close counter invariants.
// - Intuition: Can add '(' if open < n; can add ')' only if close < open (guaranteeing valid prefix balances).
// - Complexity: Time: O(\frac{1}{n+1}\binom{2n}{n} \cdot n) = O(C_n \cdot n), Space: O(n).


// ====================================================================================================
// SECTION 3: PERMUTATIONS
// ====================================================================================================

// 1. Permutations of Array (LeetCode 46 - In-place Swap)
class PermutationsSolver {
    void dfs(int idx, vi &nums, vvi &ans) {
        if (idx == (int)nums.size()) { ans.push_back(nums); return; }
        for (int i = idx; i < nums.size(); i++) {
            swap(nums[idx], nums[i]);
            dfs(idx + 1, nums, ans);
            swap(nums[idx], nums[i]); // Backtrack
        }
    }
public:
    vvi permute(vi nums) {
        vvi ans;
        dfs(0, nums, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Generate all N! permutations of an array of distinct integers.
// - Approach: Backtracking via in-place swapping without extra visited boolean arrays.
// - Intuition: Swap each remaining element into the current position idx, recurse for idx + 1, and swap back to restore state.
// - Complexity: Time: O(N! \cdot N), Space: O(N) recursion call stack.


// 2. K-th Permutation Sequence (LeetCode 60 - Factorial Number System)
string getPermutation(int n, int k) {
    vi nums;
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        nums.push_back(i);
        if (i < n) fact *= i; // Precompute (n-1)!
    }
    k--; // 0-based indexing
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
// Interview Explanation:
// - Problem Statement: Find the k-th permutation sequence of numbers [1..n] directly in mathematical order.
// - Approach: Factorial Number System block indexing (avoid generating all permutations).
// - Intuition: With (n-1)! permutations per block, the first digit index is k / (n-1)!; erase used digit and reduce k modulo factorial.
// - Complexity: Time: O(N^2) due to vector erasure, Space: O(N).


// ====================================================================================================
// SECTION 4: PARTITIONING & STRING SEGMENTATION
// ====================================================================================================

// 1. Palindrome Partitioning (LeetCode 131 - DP Table + Backtracking)
class PalindromePartitioningSolver {
    void dfs(int idx, const string &s, const vector<vector<bool>> &pal, vector<string> &cur, vector<vector<string>> &ans) {
        if (idx == (int)s.size()) { ans.push_back(cur); return; }
        for (int j = idx; j < s.size(); j++) {
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
// Interview Explanation:
// - Problem Statement: Partition string s such that every substring in the partition is a palindrome.
// - Approach: 2D DP palindrome table precomputation + Backtracking.
// - Intuition: Precomputing pal[i][j] in O(N^2) enables O(1) palindrome checks during recursion, pruning invalid branches immediately.
// - Complexity: Time: O(N \cdot 2^N), Space: O(N^2) for DP table.


// 2. Partition to K Equal Sum Subsets (LeetCode 698 - Bitmask DP Memoization)
class PartitionKSubsetsSolver {
    int target;
    vi memo;

    bool solve(int mask, const vi &nums) {
        if (mask == (1 << nums.size()) - 1) return true;
        if (memo[mask] != -1) return memo[mask];

        int current_sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (mask & (1 << i)) current_sum += nums[i];
        }
        current_sum %= target;

        for (int j = 0; j < nums.size(); j++) {
            if (!(mask & (1 << j))) {
                if (current_sum + nums[j] <= target) {
                    if (solve(mask | (1 << j), nums)) return memo[mask] = 1;
                }
            }
        }
        return memo[mask] = 0;
    }
public:
    bool canPartitionKSubsets(vi nums, int k) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % k != 0) return false;
        target = sum / k;
        sort(nums.rbegin(), nums.rend()); // Prune largest elements first
        if (nums[0] > target) return false;
        memo.assign(1 << nums.size(), -1);
        return solve(0, nums);
    }
};
// Interview Explanation:
// - Problem Statement: Determine if an integer array can be partitioned into k subsets with equal sum.
// - Approach: Bitmask Dynamic Programming with memoization over remaining element masks.
// - Intuition: Subset sum modulo target reveals current bucket load; descending sort forces large numbers to fail early.
// - Complexity: Time: O(N \cdot 2^N), Space: O(2^N).


// 3. Word Break with Rolling Hash (LeetCode 139)
struct WordBreakFastHash {
    vector<ll> p, h;
    WordBreakFastHash(const string &s) {
        int n = s.size();
        p.assign(n + 1, 1);
        h.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    ll get(int l, int r) const {
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }
    static ll hashWord(const string &s) {
        ll val = 0;
        for (char c : s) val = (val * P + (c - 'a' + 1)) % MOD;
        return val;
    }
};

class WordBreakSolver {
    vi dp;
    unordered_set<ll> dictHashes;
    unordered_set<int> uniqueLens;

    bool solve(int i, int n, const WordBreakFastHash &H) {
        if (i == n) return true;
        if (dp[i] != -1) return dp[i];
        for (int len : uniqueLens) {
            int j = i + len - 1;
            if (j < n && dictHashes.count(H.get(i, j)) && solve(j + 1, n, H)) {
                return dp[i] = 1;
            }
        }
        return dp[i] = 0;
    }
public:
    bool wordBreak(const string &s, const vector<string> &wordDict) {
        WordBreakFastHash H(s);
        for (const auto &w : wordDict) {
            dictHashes.insert(WordBreakFastHash::hashWord(w));
            uniqueLens.insert(w.size());
        }
        dp.assign(s.size(), -1);
        return solve(0, s.size(), H);
    }
};
// Interview Explanation:
// - Problem Statement: Determine if string s can be segmented into space-separated dictionary words.
// - Approach: Memoized recursion with polynomial rolling hashes for O(1) substring lookups.
// - Intuition: Testing only unique word lengths in wordDict avoids checking all possible substring splits, giving fast O(N * L) runtime.
// - Complexity: Time: O(N \cdot L) where L = unique word lengths, Space: O(N + M).


// ====================================================================================================
// SECTION 5: GRID PATHFINDING & BOARD BACKTRACKING
// ====================================================================================================

// 1. Rat in a Maze (4-Directional Grid Pathfinding)
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
        vis[r][c] = 0; // Unmark to allow other paths
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
// Interview Explanation:
// - Problem Statement: Find all possible paths from top-left (0, 0) to bottom-right (N-1, N-1) in a grid of 0s and 1s.
// - Approach: DFS backtracking in directions D, L, R, U with visited cell unmarking.
// - Intuition: Marking vis[r][c] = 1 prevents infinite cycle loops; resetting vis[r][c] = 0 upon return allows other paths to use the cell.
// - Complexity: Time: O(4^{N^2}), Space: O(N^2).


// 2. Word Search in 2D Grid (LeetCode 79)
class WordSearchSolver {
    bool dfs(int r, int c, int idx, vector<vector<char>> &board, const string &word) {
        if (idx == (int)word.size()) return true;
        if (r < 0 || r >= (int)board.size() || c < 0 || c >= (int)board[0].size() || board[r][c] != word[idx])
            return false;

        char temp = board[r][c];
        board[r][c] = '#'; // Mark visited in-place

        bool found = dfs(r + 1, c, idx + 1, board, word) ||
                     dfs(r - 1, c, idx + 1, board, word) ||
                     dfs(r, c + 1, idx + 1, board, word) ||
                     dfs(r, c - 1, idx + 1, board, word);

        board[r][c] = temp; // Restore cell
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
// Interview Explanation:
// - Problem Statement: Check if word exists in a grid of characters moving horizontally or vertically without revisiting cells.
// - Approach: In-place cell masking with '#' during DFS and restoring upon backtrack.
// - Intuition: Overwriting board[r][c] with '#' avoids allocating an external visited array; 4-directional search early exits on match.
// - Complexity: Time: O(N \cdot M \cdot 4^L), Space: O(L) where L = word length.


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
                cols[col] = diag1[d1] = diag2[d2] = 0; // Backtrack
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
// Interview Explanation:
// - Problem Statement: Place n non-attacking queens on an n x n chessboard.
// - Approach: Row-by-row backtracking with O(1) column and diagonal conflict checks.
// - Intuition: Queens on the same main diagonal share `row - col`; on the anti-diagonal they share `row + col`; track them with boolean arrays.
// - Complexity: Time: O(N!), Space: O(N) auxiliary.


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
                            board[r][c] = '.'; // Backtrack
                        }
                    }
                    return false; // No digit valid for this cell
                }
            }
        }
        return true; // All cells filled
    }
public:
    void solveSudoku(vector<vector<char>> &board) {
        solve(board);
    }
};
// Interview Explanation:
// - Problem Statement: Solve a 9x9 Sudoku puzzle by filling empty cells with digits '1'-'9'.
// - Approach: Cell-by-cell backtracking trying digits 1 to 9 with row, column, and 3x3 box collision checks.
// - Intuition: Finding the first empty cell and testing valid digits recursively propagates constraints; returns true when no empty cells remain.
// - Complexity: Time: O(9^{81}) theoretical upper bound (practically pruned fast), Space: O(81) call stack.


// 5. Graph M-Coloring (Backtracking Vertex Coloring)
class MColoringSolver {
    bool isSafe(int node, int color, const vi &colors, const vvi &graph) {
        for (int nbr : graph[node]) {
            if (colors[nbr] == color) return false;
        }
        return true;
    }

    bool dfs(int node, int m, int n, vi &colors, const vvi &graph) {
        if (node == n) return true;
        for (int c = 1; c <= m; c++) {
            if (isSafe(node, c, colors, graph)) {
                colors[node] = c;
                if (dfs(node + 1, m, n, colors, graph)) return true;
                colors[node] = 0; // Backtrack
            }
        }
        return false;
    }
public:
    bool graphColoring(const vvi &graph, int m) {
        int n = graph.size();
        vi colors(n, 0);
        return dfs(0, m, n, colors, graph);
    }
};
// Interview Explanation:
// - Problem Statement: Determine if an undirected graph can be colored with at most m colors such that no adjacent vertices share a color.
// - Approach: Vertex-by-vertex backtracking testing colors 1 to m.
// - Intuition: Check neighbor colors in O(deg(node)); assign first valid color and recurse; backtrack if no valid coloring exists.
// - Complexity: Time: O(M^N), Space: O(N) colors array.

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: RECURSION & BACKTRACKING
 ====================================================================================================

 1. THE CANONICAL BACKTRACKING BLUEPRINT:
    ```cpp
    void backtrack(State &state) {
        if (isSolution(state)) {
            recordSolution(state);
            return;
        }
        for (Choice &choice : availableChoices(state)) {
            if (isValid(choice, state)) {
                makeChoice(choice, state);       // 1. Choose
                backtrack(state);                // 2. Explore
                undoChoice(choice, state);       // 3. Unchoose (Backtrack)
            }
        }
    }
    ```

 2. SUBSETS VS. COMBINATIONS VS. PERMUTATIONS CHEAT SHEET:
    • Subsets (Power Set):
      - `for (int i = start; i < n; i++)` -> Recurse to `i + 1`. Every intermediate state is recorded!
    • Subsets / Combinations with Duplicates:
      - SORT FIRST. Use: `if (i > start && nums[i] == nums[i - 1]) continue;`
    • Combinations with Unlimited Reuse (e.g. Coin Change / Combination Sum I):
      - Recurse to `i` (NOT `i + 1`), subtract from target until target == 0.
    • Permutations:
      - Swap in-place: `swap(nums[idx], nums[i]); dfs(idx + 1); swap(nums[idx], nums[i]);`

 3. PRUNING & BRANCH-AND-BOUND TECHNIQUES:
    • Sort Descending for Bucket / Partition Problems: In problems like Partition to K Subsets,
      sorting descending puts large elements first, causing invalid sums to exceed `target` immediately!
    • In-Place Grid Visited Toggle: In Word Search, `board[r][c] = '#'` saves an O(N*M) visited array.
    • Diagonal Math for N-Queens:
      - Main Diagonal: `row - col + n`
      - Anti-Diagonal: `row + col`
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
