# Backtracking
## Master Interview & OA Guide (C++)

---

## How to Use This Guide

Backtracking is structured recursion with undoing. Every backtracking
problem uses the same skeleton — what changes is the choice set, the
constraint check, and what counts as a complete solution.

Master the skeleton first. Then the 8 patterns are variations of it.

---

## The Universal Skeleton

```
solve(current_state, remaining_choices):
    if current_state is a complete solution:
        record / return it
        return

    for each choice in remaining_choices:
        if choice is valid (constraint check):
            make the choice       <- modify state
            solve(next_state, ...)
            undo the choice       <- restore state (backtrack)
```

In C++:

```cpp
void backtrack(/* current state */) {
    if (/* base case: solution complete */) {
        result.push_back(current); // record answer
        return;
    }
    for (/* each candidate choice */) {
        if (/* pruning: skip invalid choices */) continue;

        // Make choice
        current.push_back(candidate);
        // mark used / modify state

        backtrack(/* next state */);

        // Undo choice
        current.pop_back();
        // unmark used / restore state
    }
}
```

The three things that vary per problem:
1. **What is the choice?** (which element, digit, direction, placement)
2. **What is the constraint?** (no duplicates, sum target, board rules)
3. **What is a complete solution?** (length k, sum == target, all cells filled)

---

## Master Decision Tree

```
Does the problem ask for ALL combinations / subsets / permutations?
  YES -> Backtracking (enumerate all)

Does the problem ask to BUILD something (string, board, path)?
  YES -> Backtracking with construction

Can you PRUNE early (stop before reaching a dead end)?
  YES -> Add pruning condition inside the loop

Does the problem have board constraints (N-Queens, Sudoku)?
  YES -> Pattern 7: Board / Placement Backtracking

Does order matter in the answer?
  YES -> Permutations pattern (each element can appear once, order matters)
  NO  -> Combinations / Subsets pattern (pick without regard to order)

Can elements be reused?
  YES -> start = same index (not i+1) in next call
  NO  -> start = i+1 or mark as visited
```

---

# PATTERN 1: Subsets (Power Set)

Every element is either included or excluded. No duplicates in input.

```cpp
// LC 78. Subsets
// Input has no duplicates. Generate all 2^n subsets.
class Solution {
private:
    void backtrack(int start, const vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        result.push_back(current); // every state is a valid subset

        for (int i = start; i < nums.size(); i++) {
            current.push_back(nums[i]); // choose
            backtrack(i + 1, nums, current, result); // next element must come after i
            current.pop_back();         // unchoose
        }
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, nums, current, result);
        return result;
    }
};
```

**With duplicates — sort first, skip same-level duplicates:**

```cpp
// LC 90. Subsets II
// Input may have duplicates. Each unique subset appears once.
class Solution {
private:
    void backtrack(int start, const vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        result.push_back(current);

        for (int i = start; i < nums.size(); i++) {
            // Skip duplicate at the same recursion level
            if (i > start && nums[i] == nums[i-1]) continue;
            current.push_back(nums[i]);
            backtrack(i + 1, nums, current, result);
            current.pop_back();
        }
    }
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end()); // sort to group duplicates
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, nums, current, result);
        return result;
    }
};
```

**The duplicate-skip rule:** `if (i > start && nums[i] == nums[i-1]) continue`
- `i > start` means we have already tried this value at this level.
- Without this, [1,2] and [1,2] (from two 2s) would both appear.

---

# PATTERN 2: Combinations

Choose exactly k elements. Order does not matter.

```cpp
// LC 77. Combinations
// Choose k numbers from [1, n]
class Solution {
private:
    void backtrack(int start, int n, int k, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == k) {
            result.push_back(current);
            return;
        }
        // Pruning: only go up to n - (k - current.size()) + 1
        // We need at least (k - current.size()) more elements
        int remaining = k - current.size();
        for (int i = start; i <= n - remaining + 1; i++) {
            current.push_back(i);
            backtrack(i + 1, n, k, current, result);
            current.pop_back();
        }
    }
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(1, n, k, current, result);
        return result;
    }
};
```

```cpp
// LC 39. Combination Sum
// Elements can be reused. Find all combinations summing to target.
class Solution {
private:
    void backtrack(int start, int remaining, const vector<int>& candidates, vector<int>& current, vector<vector<int>>& result) {
        if (remaining == 0) {
            result.push_back(current);
            return;
        }
        for (int i = start; i < candidates.size(); i++) {
            if (candidates[i] > remaining) break; // pruning: sorted, no point continuing
            current.push_back(candidates[i]);
            backtrack(i, remaining - candidates[i], candidates, current, result); // i not i+1: can reuse
            current.pop_back();
        }
    }
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, target, candidates, current, result);
        return result;
    }
};
```

```cpp
// LC 40. Combination Sum II
// Each element used at most once. Input has duplicates.
class Solution {
private:
    void backtrack(int start, int remaining, const vector<int>& candidates, vector<int>& current, vector<vector<int>>& result) {
        if (remaining == 0) { 
            result.push_back(current); 
            return; 
        }
        for (int i = start; i < candidates.size(); i++) {
            if (candidates[i] > remaining) break;
            if (i > start && candidates[i] == candidates[i-1]) continue; // skip dup
            current.push_back(candidates[i]);
            backtrack(i + 1, remaining - candidates[i], candidates, current, result); // i+1: each used once
            current.pop_back();
        }
    }
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, target, candidates, current, result);
        return result;
    }
};
```

```cpp
// LC 216. Combination Sum III
// Find k numbers that sum to n, digits 1-9, each used once
class Solution {
private:
    void backtrack(int start, int remaining, int k, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == k && remaining == 0) {
            result.push_back(current);
            return;
        }
        for (int i = start; i <= 9; i++) {
            if (i > remaining) break;
            current.push_back(i);
            backtrack(i + 1, remaining - i, k, current, result);
            current.pop_back();
        }
    }
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(1, n, k, current, result);
        return result;
    }
};
```

---

# PATTERN 3: Permutations

Order matters. All elements used exactly once (unless noted).

```cpp
// LC 46. Permutations
// No duplicates in input.
class Solution {
private:
    void backtrack(const vector<int>& nums, vector<bool>& used, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            used[i] = true;
            current.push_back(nums[i]);
            backtrack(nums, used, current, result);
            current.pop_back();
            used[i] = false;
        }
    }
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, current, result);
        return result;
    }
};
```

```cpp
// LC 47. Permutations II
// Input has duplicates. Generate unique permutations only.
class Solution {
private:
    void backtrack(const vector<int>& nums, vector<bool>& used, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            // Skip: same value as previous AND previous was not used
            // (meaning we already explored this branch at this level)
            if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
            used[i] = true;
            current.push_back(nums[i]);
            backtrack(nums, used, current, result);
            current.pop_back();
            used[i] = false;
        }
    }
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, current, result);
        return result;
    }
};
```

**Why `!used[i-1]` for permutation duplicates?**
If `used[i-1]` is false, we already finished and undid the branch where
`nums[i-1]` was placed at this position. Placing `nums[i]` (same value)
here again would produce an identical subtree. Skip it.

---

# PATTERN 4: String / Partition Backtracking

Build strings character by character, or partition a string into valid pieces.

```cpp
// LC 131. Palindrome Partitioning
// Partition s into all possible lists of palindromic substrings
class Solution {
private:
    bool isPalin(const string& s, int lo, int hi) {
        while (lo < hi) {
            if (s[lo++] != s[hi--]) return false;
        }
        return true;
    }

    void backtrack(int start, const string& s, vector<string>& current, vector<vector<string>>& result) {
        if (start == s.size()) {
            result.push_back(current);
            return;
        }
        for (int end = start; end < s.size(); end++) {
            if (isPalin(s, start, end)) {
                current.push_back(s.substr(start, end - start + 1));
                backtrack(end + 1, s, current, result);
                current.pop_back();
            }
        }
    }
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> current;
        backtrack(0, s, current, result);
        return result;
    }
};
```

```cpp
// LC 93. Restore IP Addresses
// Insert 3 dots into a string to form a valid IP address
class Solution {
private:
    bool isValid(const string& seg) {
        if (seg.empty() || seg.size() > 3) return false;
        if (seg.size() > 1 && seg[0] == '0') return false; // no leading zeros
        return stoi(seg) <= 255;
    }

    void backtrack(int start, const string& s, vector<string>& parts, vector<string>& result) {
        if (parts.size() == 4 && start == s.size()) {
            result.push_back(parts[0]+"."+parts[1]+"."+parts[2]+"."+parts[3]);
            return;
        }
        if (parts.size() == 4 || start == s.size()) return; // prune

        for (int len = 1; len <= 3; len++) {
            if (start + len > s.size()) break;
            string seg = s.substr(start, len);
            if (!isValid(seg)) continue;
            parts.push_back(seg);
            backtrack(start + len, s, parts, result);
            parts.pop_back();
        }
    }
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        vector<string> parts;
        backtrack(0, s, parts, result);
        return result;
    }
};
```

```cpp
// LC 22. Generate Parentheses
// Generate all valid combinations of n pairs of parentheses
class Solution {
private:
    void backtrack(int open, int close, int n, string& current, vector<string>& result) {
        if (current.size() == 2 * n) {
            result.push_back(current);
            return;
        }
        if (open < n) {
            current += '(';
            backtrack(open + 1, close, n, current, result);
            current.pop_back();
        }
        if (close < open) { // can only close if more opens than closes
            current += ')';
            backtrack(open, close + 1, n, current, result);
            current.pop_back();
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        string current;
        backtrack(0, 0, n, current, result);
        return result;
    }
};
```

```cpp
// LC 17. Letter Combinations of a Phone Number
class Solution {
private:
    void backtrack(int idx, const string& digits, const vector<string>& phone, string& current, vector<string>& result) {
        if (idx == digits.size()) {
            result.push_back(current);
            return;
        }
        for (char c : phone[digits[idx] - '0']) {
            current += c;
            backtrack(idx + 1, digits, phone, current, result);
            current.pop_back();
        }
    }
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> phone = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
        vector<string> result;
        string current;
        backtrack(0, digits, phone, current, result);
        return result;
    }
};
```

---

# PATTERN 5: Word Search / Grid Backtracking

DFS on a grid, marking cells as visited, then unmarking on backtrack.

```cpp
// LC 79. Word Search
class Solution {
private:
    bool backtrack(int r, int c, int idx, const string& word, vector<vector<char>>& board) {
        if (idx == word.size()) return true;
        if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size()) return false;
        if (board[r][c] != word[idx]) return false;

        char temp = board[r][c];
        board[r][c] = '#'; // mark visited

        bool found = backtrack(r+1, c, idx+1, word, board) ||
                     backtrack(r-1, c, idx+1, word, board) ||
                     backtrack(r, c+1, idx+1, word, board) ||
                     backtrack(r, c-1, idx+1, word, board);

        board[r][c] = temp; // restore (backtrack)
        return found;
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        int rows = board.size(), cols = board[0].size();
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                if (backtrack(r, c, 0, word, board)) return true;
        return false;
    }
};
```

```cpp
// LC 212. Word Search II
// Find all words from a dictionary that exist in the board
// Build a Trie of all words, then DFS on the board

struct TrieNode {
    TrieNode* children[26] = {};
    string word = ""; // non-empty at end of a word
};

class Solution {
private:
    void dfs(int r, int c, TrieNode* node, vector<vector<char>>& board, vector<string>& result) {
        if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size() || board[r][c] == '#') return;
        char ch = board[r][c];
        TrieNode* next = node->children[ch - 'a'];
        if (!next) return;

        if (!next->word.empty()) {
            result.push_back(next->word);
            next->word = ""; // avoid duplicates
        }

        board[r][c] = '#';
        dfs(r+1, c, next, board, result); 
        dfs(r-1, c, next, board, result);
        dfs(r, c+1, next, board, result); 
        dfs(r, c-1, next, board, result);
        board[r][c] = ch;
    }
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = new TrieNode();
        for (auto& w : words) {
            TrieNode* node = root;
            for (char c : w) {
                int i = c - 'a';
                if (!node->children[i]) node->children[i] = new TrieNode();
                node = node->children[i];
            }
            node->word = w;
        }

        int rows = board.size(), cols = board[0].size();
        vector<string> result;

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                dfs(r, c, root, board, result);

        return result;
    }
};
```

---

# PATTERN 6: Path / Graph Backtracking

Find all paths in a graph, reconstruct paths, visit all nodes.

```cpp
// LC 797. All Paths From Source to Target (DAG)
class Solution {
private:
    void backtrack(int node, const vector<vector<int>>& graph, vector<int>& path, vector<vector<int>>& result) {
        if (node == graph.size() - 1) {
            result.push_back(path);
            return;
        }
        for (int next : graph[node]) {
            path.push_back(next);
            backtrack(next, graph, path, result);
            path.pop_back();
        }
    }
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result;
        vector<int> path = {0};
        backtrack(0, graph, path, result);
        return result;
    }
};
```

```cpp
// LC 332. Reconstruct Itinerary (Eulerian path via backtracking / Hierholzer)
// Use backtracking to try departures in lexicographic order
class Solution {
private:
    void dfs(string airport, unordered_map<string, multiset<string>>& adj, vector<string>& result) {
        while (!adj[airport].empty()) {
            string next = *adj[airport].begin();
            adj[airport].erase(adj[airport].begin());
            dfs(next, adj, result);
        }
        result.push_back(airport);
    }
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        unordered_map<string, multiset<string>> adj;
        for (auto& t : tickets)
            adj[t[0]].insert(t[1]);

        vector<string> result;
        dfs("JFK", adj, result);
        reverse(result.begin(), result.end());
        return result;
    }
};
```

---

# PATTERN 7: Board Placement Backtracking

Place items on a board subject to constraints. Classic: N-Queens, Sudoku.

```cpp
// LC 51. N-Queens
class Solution {
private:
    vector<string> buildBoard(int n, const vector<int>& queens) {
        vector<string> board(n, string(n, '.'));
        for (int r = 0; r < n; r++)
            board[r][queens[r]] = 'Q';
        return board;
    }

    void backtrack(int row, int n, vector<int>& queens, unordered_set<int>& cols, 
                   unordered_set<int>& diag1, unordered_set<int>& diag2, 
                   vector<vector<string>>& result) {
        if (row == n) {
            result.push_back(buildBoard(n, queens));
            return;
        }
        for (int col = 0; col < n; col++) {
            if (cols.count(col) || diag1.count(row - col) || diag2.count(row + col))
                continue; // under attack

            queens[row] = col;
            cols.insert(col);
            diag1.insert(row - col); // top-left to bottom-right diagonal
            diag2.insert(row + col); // top-right to bottom-left diagonal

            backtrack(row + 1, n, queens, cols, diag1, diag2, result);

            queens[row] = -1;
            cols.erase(col);
            diag1.erase(row - col);
            diag2.erase(row + col);
        }
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1); // queens[row] = col where queen is placed
        unordered_set<int> cols, diag1, diag2; // occupied columns and diagonals
        backtrack(0, n, queens, cols, diag1, diag2, result);
        return result;
    }
};
```

```cpp
// LC 52. N-Queens II (just count solutions)
class Solution {
private:
    void backtrack(int row, int n, int cols, int d1, int d2, int& count) {
        if (row == n) { count++; return; }
        int available = ((1 << n) - 1) & ~(cols | d1 | d2); // free columns
        while (available) {
            int bit = available & (-available); // lowest set bit = next free column
            available &= available - 1;         // clear that bit
            backtrack(row + 1, n, cols | bit, (d1 | bit) << 1, (d2 | bit) >> 1, count);
        }
    }
public:
    int totalNQueens(int n) {
        int count = 0;
        backtrack(0, n, 0, 0, 0, count);
        return count;
    }
};
```

```cpp
// LC 37. Sudoku Solver
class Solution {
private:
    bool isValid(int r, int c, char d, const vector<vector<char>>& board) {
        for (int i = 0; i < 9; i++) {
            if (board[r][i] == d || board[i][c] == d) return false;
        }
        int br = (r/3)*3, bc = (c/3)*3;
        for (int i = br; i < br+3; i++) {
            for (int j = bc; j < bc+3; j++) {
                if (board[i][j] == d) return false;
            }
        }
        return true;
    }

    bool backtrack(vector<vector<char>>& board) {
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if (board[r][c] != '.') continue; // already filled

                for (char d = '1'; d <= '9'; d++) {
                    if (isValid(r, c, d, board)) {
                        board[r][c] = d;
                        if (backtrack(board)) return true;
                        board[r][c] = '.';
                    }
                }
                return false; // no digit worked: backtrack
            }
        }
        return true; // all cells filled
    }
public:
    void solveSudoku(vector<vector<char>>& board) {
        backtrack(board);
    }
};
```

---

# PATTERN 8: Bitmask / State Backtracking

Track visited states with a bitmask when n is small (n <= 20).

```cpp
// LC 526. Beautiful Arrangement
// Count permutations where position i is divisible by nums[i] or vice versa
class Solution {
private:
    void backtrack(int pos, int n, vector<bool>& used, int& count) {
        if (pos > n) { count++; return; }
        for (int i = 1; i <= n; i++) {
            if (used[i]) continue;
            if (pos % i == 0 || i % pos == 0) { // valid placement
                used[i] = true;
                backtrack(pos + 1, n, used, count);
                used[i] = false;
            }
        }
    }
public:
    int countArrangement(int n) {
        int count = 0;
        vector<bool> used(n + 1, false);
        backtrack(1, n, used, count);
        return count;
    }
};
```

```cpp
// LC 980. Unique Paths III
// Visit every non-obstacle cell exactly once
class Solution {
private:
    void backtrack(int r, int c, int visited, int total, int& count, const vector<vector<int>>& grid) {
        if (grid[r][c] == 2) {
            if (visited == total) count++;
            return;
        }
        int rows = grid.size(), cols = grid[0].size();
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == -1) continue;
            // Use (nr*cols + nc) as a bitmask key
            int bit = nr * cols + nc;
            if (visited & (1 << bit)) continue; // already visited
            backtrack(nr, nc, visited | (1 << bit), total, count, grid);
        }
    }
public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        int rows = grid.size(), cols = grid[0].size();
        int startR = 0, startC = 0, total = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] != -1) total++; // count non-obstacle cells
                if (grid[r][c] == 1) { startR = r; startC = c; }
            }
        }

        int count = 0;
        backtrack(startR, startC, 1 << (startR * cols + startC), total, count, grid);
        return count;
    }
};
```

---

# Pruning — The Performance Multiplier

Pruning is what makes backtracking practical. Without it you explore
every branch. With it you cut orders of magnitude of work.

**Three types of pruning:**

**1. Feasibility pruning** — stop if current state cannot lead to a solution.

```cpp
// In combination sum: if candidate > remaining, no point trying larger ones
if (candidates[i] > remaining) break; // requires sorted array
```

**2. Bound pruning** — stop if even the best possible completion won't beat
current best (used in optimization problems).

```cpp
// In TSP / scheduling: if current_cost + lower_bound >= best_found, prune
if (current_cost + lowerBound() >= best) return;
```

**3. Symmetry / duplicate pruning** — skip choices that produce results
already explored.

```cpp
// Skip same value at same recursion level (subsets/combinations with duplicates)
if (i > start && nums[i] == nums[i-1]) continue;

// Skip same value at same position level (permutations with duplicates)
if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
```

---

# Complexity Analysis

Backtracking problems are exponential in the worst case.
What matters for interviews is knowing the rough magnitude.

| Problem | Time Complexity | Reason |
|---|---|---|
| Subsets | O(2^n) | Each element included or not |
| Combinations C(n,k) | O(C(n,k) * k) | C(n,k) solutions, each length k |
| Permutations | O(n! * n) | n! solutions, each length n |
| Combination Sum (reuse allowed) | O(n^(t/m)) | t=target, m=min candidate |
| Palindrome Partitioning | O(n * 2^n) | 2^(n-1) partitions |
| N-Queens | O(n!) | n choices row 1, n-1 row 2, etc. |
| Sudoku | O(9^m) | m = number of empty cells |
| Word Search | O(4^(rows*cols)) | 4 directions, cells*cells start points |

---

# The Duplicate Handling Reference

A source of most backtracking bugs. Two situations:

**Situation A — Combinations / Subsets (index-based, `i > start`):**
```cpp
sort(nums.begin(), nums.end());
// In the loop:
if (i > start && nums[i] == nums[i-1]) continue;
// "start" is the starting index of THIS call, not the global start.
// "i > start" means we already tried this value at this level.
```

**Situation B — Permutations (`!used[i-1]`):**
```cpp
sort(nums.begin(), nums.end());
// In the loop:
if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
// !used[i-1]: the previous identical element was already placed AND unplaced.
// That means we explored this subtree and came back. Don't repeat.
// If used[i-1] is true, it means i-1 is in the current path ABOVE us,
// and we are choosing i as a different element — that is valid.
```

---

# Pattern Recognition Quick-Reference

| Signal in Problem | Pattern | Key Decision |
|---|---|---|
| All subsets, power set | Subsets | Record at every node |
| All subsets, input has duplicates | Subsets II | Sort + skip same-level dups |
| Choose exactly k elements | Combinations | Stop at size k |
| Sum to target, elements reusable | Comb Sum I | Pass i not i+1; break if > remaining |
| Sum to target, each used once | Comb Sum II | Pass i+1; skip same-level dups |
| All orderings, no duplicates | Permutations | used[] array |
| All orderings, duplicates | Permutations II | Sort + skip `!used[i-1]` |
| Partition string into valid parts | String partition | End index loop; validate segment |
| Valid bracket sequences | String build | Track open and close counts |
| Phone keypad letters | String build | Map digit to chars |
| Find word in grid | Grid DFS | Mark '#', restore after |
| Find all words from dict in grid | Grid + Trie | Build Trie first, DFS with node |
| All paths in graph / DAG | Path | Push node, recurse, pop |
| Place queens / items on board | Board placement | Track cols + both diagonals |
| Solve Sudoku | Board placement | Try 1-9, backtrack on fail |
| Count valid arrangements (small n) | Bitmask / used[] | Check divisibility or constraint |
| Visit every cell exactly once | Grid bitmask | Bitmask of visited cells |

---

# Comparison: Subsets vs Combinations vs Permutations

| Property | Subsets | Combinations | Permutations |
|---|---|---|---|
| Fixed size? | No (all sizes) | Yes (exactly k) | Yes (all n) |
| Order matters? | No | No | Yes |
| Elements reused? | No | Depends | No |
| Duplicate input handling | Skip at same level | Skip at same level | Skip with `!used[i-1]` |
| Index advances | `i + 1` | `i + 1` (or `i` if reuse) | No index; use `used[]` |
| Record answer when | Every call | Size == k | Size == n |
| Rough complexity | O(2^n) | O(C(n,k)) | O(n!) |

---

# Complete Problem List

### Subsets
78, 90

### Combinations
77, 39, 40, 216, 377 (also DP)

### Permutations
46, 47, 31 (next permutation — iterative)

### String / Partition
131, 132 (also DP), 93, 22, 17, 320, 1593

### Grid / Word Search
79, 212

### Paths in Graph
797, 332 (Euler path)

### Board Placement
51, 52, 37

### Bitmask / State
526, 980, 698 (also DP), 351 (Android unlock)

### Mixed / Hard
491 (non-decreasing subsequences), 1239 (max length unique substrings),
784 (letter case permutation), 1219 (path in gold grid),
140 (word break II — backtrack + memo)