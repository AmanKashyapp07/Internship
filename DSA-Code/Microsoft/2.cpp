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

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 32 | Partition Array According to Given Pivot    | Three-Pass Stable Reordering      | O(N)     | O(N)     |
 | 33 | Count Subarrays With Majority Element I     | Prefix Transform + Fenwick/Count  | O(N logN)| O(N)     |
 | 34 | Min Distance Between Mirror Pairs           | Hash Map + Digit Reversal         | O(N logV)| O(N)     |
 | 35 | Valid Sudoku                                | Bitmask / Frequency Matrices      | O(1)     | O(1)     |
 | 36 | Rotate Image                                | Transpose + Row Reversal          | O(N^2)   | O(1)     |
 | 37 | Set Matrix Zeroes                           | In-Place First Row/Col Markers    | O(M * N) | O(1)     |
 | 38 | Search a 2D Matrix                          | Virtual 1D Binary Search          | O(log MN)| O(1)     |
 | 39 | Surrounded Regions                          | Boundary Flood-Fill (DFS/BFS)     | O(M * N) | O(M * N) |
 | 40 | Number of Islands                           | Grid Connected Components DFS/BFS | O(M * N) | O(M * N) |
 | 41 | Rotting Oranges                             | Multi-Source BFS Matrix Traversal | O(M * N) | O(M * N) |
 | 42 | Longest Palindromic Substring               | Expand Around Center Search       | O(N^2)   | O(1)     |
 | 43 | Zigzag Conversion                           | Directional Row Simulation        | O(N)     | O(N)     |
 | 44 | Integer to Roman                            | Greedy Greedy Value-Symbol Match  | O(1)     | O(1)     |
 | 45 | Letter Combinations of a Phone Number       | Backtracking / Branch Recursion   | O(4^N * N)| O(N)    |
 | 46 | Valid Parentheses                           | Stack Matching LIFO Verification  | O(N)     | O(N)     |
 | 47 | Generate Parentheses                        | Constrained Backtracking (Catalan)| O(4^N/√N)| O(N)     |
 | 48 | Substring with Concatenation of All Words   | Offset Sliding Window + Freq Map  | O(N * W) | O(K * W) |
 | 49 | Group Anagrams                              | Sorted String Hash Map Grouping   | O(N KlogK| O(N * K) |
 | 50 | Isomorphic Strings                          | Bi-Directional Character Mapping  | O(N)     | O(1)     |
 | 51 | Median of Two Sorted Arrays                 | Binary Search on Partitioning     | O(log(min| O(1)     |
 | 52 | Search in Rotated Sorted Array              | Partitioned Binary Search Checks  | O(log N) | O(1)     |
 | 53 | Split Array Largest Sum                     | Binary Search on Answer + Greedy  | O(N logS)| O(1)     |
 | 54 | Single Element in a Sorted Array            | Even-Odd Index Binary Search      | O(log N) | O(1)     |
 | 55 | Koko Eating Bananas                         | Monotonic Speed Binary Search     | O(N logM)| O(1)     |
 | 56 | Capacity To Ship Packages Within D Days     | Capacity Feasibility Binary Search| O(N logS)| O(1)     |
 | 57 | Find Smallest Divisor Given Threshold       | Divisor Sum Binary Search Check   | O(N logM)| O(1)     |
 ====================================================================================================
*/

// =========================================================
// 32. PARTITION ARRAY ACCORDING TO GIVEN PIVOT (LC 2161)
// =========================================================

class Solution32 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> result;
        result.reserve(nums.size());
        for (int x : nums) if (x < pivot) result.push_back(x);
        for (int x : nums) if (x == pivot) result.push_back(x);
        for (int x : nums) if (x > pivot) result.push_back(x);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Rearrange nums such that elements < pivot appear first, then == pivot, then > pivot, preserving relative order.
// - Approach: Three-pass stable collection.
// - Intuition: Three linear scans collect elements in the exact specified partition categories without reordering within groups.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 33. COUNT SUBARRAYS WITH MAJORITY ELEMENT I (LC 3737)
// =========================================================

class Solution33 {
public:
    long long countSubarrays(vector<int>& nums, int target) {
        int n = nums.size();
        long long count = 0;
        for (int i = 0; i < n; i++) {
            int freq = 0;
            for (int j = i; j < n; j++) {
                if (nums[j] == target) freq++;
                int len = j - i + 1;
                if (freq > len / 2) count++;
            }
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Count subarrays where target appears strictly more than half the time (majority element).
// - Approach: Running frequency prefix transform / scan.
// - Intuition: For each subarray, check if target count > length / 2.
// - Complexity: Time: O(N^2), Space: O(1).


// =========================================================
// 34. MIN DISTANCE BETWEEN MIRROR PAIRS (LC 3761)
// =========================================================

class Solution34 {
    int reverseNum(int n) {
        int rev = 0;
        while (n > 0) {
            rev = rev * 10 + (n % 10);
            n /= 10;
        }
        return rev;
    }

public:
    int minMirrorPairDistance(vector<int>& nums) {
        unordered_map<int, int> lastSeen;
        int minDistance = INT_MAX;

        for (int j = 0; j < (int)nums.size(); j++) {
            if (lastSeen.count(nums[j])) {
                minDistance = min(minDistance, j - lastSeen[nums[j]]);
            }
            int rev = reverseNum(nums[j]);
            lastSeen[rev] = j;
        }
        return minDistance == INT_MAX ? -1 : minDistance;
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum |i - j| for pair (i, j) with i < j and reverse(nums[i]) == nums[j]. Return -1 if none.
// - Approach: Hash Map + Digit Reversal. Store latest index of reverse(nums[i]); match at index j with nums[j].
// - Intuition: Mapping reversed values to indices allows immediate lookup of valid mirror pair candidates.
// - Complexity: Time: O(N log10 V), Space: O(N).


// =========================================================
// 35. VALID SUDOKU (LC 36)
// =========================================================

class Solution35 {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int rows[9] = {0}, cols[9] = {0}, boxes[9] = {0};

        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if (board[r][c] == '.') continue;
                int digit = board[r][c] - '1';
                int mask = 1 << digit;
                int boxIndex = (r / 3) * 3 + (c / 3);

                if ((rows[r] & mask) || (cols[c] & mask) || (boxes[boxIndex] & mask)) {
                    return false;
                }
                rows[r] |= mask;
                cols[c] |= mask;
                boxes[boxIndex] |= mask;
            }
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if a 9x9 Sudoku board is valid according to row, column, and 3x3 sub-box rules.
// - Approach: Bitmask frequency matrices.
// - Intuition: Use 9-bit bitmask per row, column, and 3x3 box to test duplicate digits in O(1) time and memory.
// - Complexity: Time: O(1) (fixed 81 cells), Space: O(1).


// =========================================================
// 36. ROTATE IMAGE (LC 48)
// =========================================================

class Solution36 {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // Step 1: Transpose matrix (swap matrix[i][j] with matrix[j][i])
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(matrix[i][j], matrix[j][i]);
            }
        }
        // Step 2: Reverse each row
        for (int i = 0; i < n; i++) {
            ::reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};
// Interview Explanation:
// - Problem Statement: Rotate an N x N 2D matrix by 90 degrees clockwise in-place.
// - Approach: Transpose + Row Reversal.
// - Intuition: Matrix clockwise rotation is mathematically equivalent to reflection along the main diagonal followed by vertical axis flip.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.


// =========================================================
// 37. SET MATRIX ZEROES (LC 73)
// =========================================================

class Solution37 {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();
        bool firstRow = false, firstCol = false;

        // Check first row and first column
        for (int j = 0; j < m; j++)
            if (matrix[0][j] == 0) firstRow = true;

        for (int i = 0; i < n; i++)
            if (matrix[i][0] == 0) firstCol = true;

        // Use first row and column as markers
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
            }
        }

        // Set cells to zero
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                if (matrix[i][0] == 0 || matrix[0][j] == 0)
                    matrix[i][j] = 0;
            }
        }

        // Handle first row
        if (firstRow){
            fill(matrix[0].begin(), matrix[0].end(), 0);
        }

        // Handle first column
        if (firstCol){
            for (int i = 0; i < n; i++)
                matrix[i][0] = 0;
        }
    }
};
// Interview Explanation:
// - Problem Statement: If an element in an M x N matrix is 0, set its entire row and column to 0 in-place.
// - Approach: In-Place State Storage using First Row and Column as Sentinel Bitmaps.
// - Intuition:
//   * Storing row/column zero flags requires O(M + N) space.
//   * To achieve O(1) space, reuse row 0 and col 0 to store these zero flags, using a single boolean variable `firstColZero` to resolve overlap at (0, 0).
//   * Process backwards so updated zeros do not overwrite sentinel flags prematurely.
// - Complexity: Time: O(M * N), Space: O(1) auxiliary space.


// =========================================================
// 38. SEARCH A 2D MATRIX (LC 74)
// =========================================================

class Solution38 {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size(), n = matrix[0].size();
        int low = 0, high = m * n - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int r = mid / n, c = mid % n;
            if (matrix[r][c] == target) return true;
            if (matrix[r][c] < target) low = mid + 1;
            else high = mid - 1;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if target exists in M x N matrix with sorted rows and first element of each row > last of previous.
// - Approach: Virtual 1D Binary Search.
// - Intuition: Matrix is strictly monotonically sorted across all M*N elements; index `mid` maps to `(mid / N, mid % N)`.
// - Complexity: Time: O(log(M * N)), Space: O(1).


// =========================================================
// 39. SURROUNDED REGIONS (LC 130)
// =========================================================

class Solution39 {
    int m, n;
    vector<vector<char>>* b;

    void dfs(int r, int c) {
        if (r < 0 || r >= m || c < 0 || c >= n || (*b)[r][c] != 'O') return;
        (*b)[r][c] = '#';
        dfs(r + 1, c);
        dfs(r - 1, c);
        dfs(r, c + 1);
        dfs(r, c - 1);
    }

public:
    void solve(vector<vector<char>>& board) {
        if (board.empty()) return;
        m = board.size();
        n = board[0].size();
        b = &board;

        // Step 1: Run DFS from border cells
        for (int i = 0; i < m; i++) {
            if (board[i][0] == 'O') dfs(i, 0);
            if (board[i][n - 1] == 'O') dfs(i, n - 1);
        }
        for (int j = 0; j < n; j++) {
            if (board[0][j] == 'O') dfs(0, j);
            if (board[m - 1][j] == 'O') dfs(m - 1, j);
        }

        // Step 2: Flip remaining 'O' to 'X', restore '#' to 'O'
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                else if (board[i][j] == '#') board[i][j] = 'O';
            }
        }
    }
};
// Interview Explanation:
// - Problem Statement: Capture all regions surrounded by 'X' by flipping all surrounded 'O' cells into 'X'.
// - Approach: Boundary Flood-Fill (DFS/BFS) Inversion.
// - Intuition:
//   * Any 'O' connected to the outer boundary can never be surrounded.
//   * Run DFS/BFS from all border 'O' cells, temporarily marking them '#'.
//   * Remaining 'O' cells are completely surrounded -> flip to 'X'. Finally, restore '#' back to 'O'.
// - Complexity: Time: O(M * N), Space: O(M * N) recursion depth.


// =========================================================
// 40. NUMBER OF ISLANDS (LC 200)
// =========================================================

class Solution40 {
    int m, n;
    vector<vector<char>>* g;

    void dfs(int r, int c) {
        if (r < 0 || r >= m || c < 0 || c >= n || (*g)[r][c] != '1') return;
        (*g)[r][c] = '0'; // Sink island in-place
        dfs(r + 1, c);
        dfs(r - 1, c);
        dfs(r, c + 1);
        dfs(r, c - 1);
    }

public:
    int numIslands(vector<vector<char>>& grid) {
        m = grid.size();
        n = grid[0].size();
        g = &grid;
        int count = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    count++;
                    dfs(i, j);
                }
            }
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Count number of connected land components ('1's surrounded by '0's).
// - Approach: Connected Components DFS (In-place Sinking).
// - Intuition: When encountering unvisited land, increment count and flood-fill sink all connected land to '0'.
// - Complexity: Time: O(M * N), Space: O(M * N) recursion stack.


// =========================================================
// 41. ROTTING ORANGES (LC 994)
// =========================================================

class Solution41 {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = (int)grid.size();
        int n = (int)grid[0].size();
        queue<pair<int, int>> q;
        int freshCount = 0;

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 2) {
                    q.push({i, j});
                } else if (grid[i][j] == 1) {
                    freshCount++;
                }
            }
        }

        if (freshCount == 0) return 0;

        int minutes = 0;
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        while (!q.empty() && freshCount > 0) {
            int sz = (int)q.size();
            for (int k = 0; k < sz; ++k) {
                auto [r, c] = q.front();
                q.pop();

                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;
                        freshCount--;
                        q.push({nr, nc});
                    }
                }
            }
            minutes++;
        }
        return (freshCount == 0) ? minutes : -1;
    }
};
// Interview Explanation:
// - Problem Statement: Return minimum minutes until no fresh oranges remain, or -1 if impossible.
// - Approach: Multi-Source Breadth-First Search (BFS).
// - Intuition:
//   * All initially rotten oranges rot their adjacent neighbors concurrently in minute steps.
//   * Enqueue all rotten oranges at level 0, process level-by-level, and track remaining fresh oranges.
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 42. LONGEST PALINDROMIC SUBSTRING (LC 5)
// =========================================================

class Solution42 {
    string str;
    int start = 0, maxLen = 1;

    void expand(int l, int r) {
        int n = str.size();
        while (l >= 0 && r < n && str[l] == str[r]) {
            if (r - l + 1 > maxLen) {
                start = l;
                maxLen = r - l + 1;
            }
            l--;
            r++;
        }
    }

public:
    string longestPalindrome(string s) {
        if (s.size() <= 1) return s;
        str = std::move(s);
        start = 0;
        maxLen = 1;

        int n = str.size();
        for (int i = 0; i < n; i++) {
            expand(i, i);     // Odd-length palindromes
            expand(i, i + 1); // Even-length palindromes
        }
        return str.substr(start, maxLen);
    }
};
// Interview Explanation:
// - Problem Statement: Find the longest palindromic substring in string s.
// - Approach: Expand Around Center.
// - Intuition: A palindrome mirrors around its center. There are 2N - 1 possible centers (single character or character pair).
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.


// =========================================================
// 43. ZIGZAG CONVERSION (LC 6)
// =========================================================

class Solution43 {
public:
    string convert(string s, int numRows) {
        if (numRows == 1) return s;
        vector<string> rows(numRows);
        int row = 0, dir = 1;
        // row index moves down until numRows-1, then up until 0, and repeats
        // dir is +1 for down, -1 for up
        for (char c : s) {
            rows[row] += c;
            if (row == 0) dir = 1; 
            if (row == numRows - 1) dir = -1;
            row += dir;
        }
        string ans;
        for (string& x : rows) ans += x;
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Display a string in a zigzag pattern across numRows rows and read line-by-line.
// - Approach: Simulation with Direction Bouncing.
// - Intuition:
//   * Track current row index. Advance down until reaching numRows - 1, then step up until reaching row 0.
//   * Append characters to their corresponding row buffer and concatenate all rows at the end.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 44. INTEGER TO ROMAN (LC 12)
// =========================================================

class Solution44 {
public:
    string intToRoman(int num) {
        static const vector<pair<int, string>> roman = {
            {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
            {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
            {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"},
            {1, "I"}
        };
        string result;
        for (const auto& [val, sym] : roman) {
            while (num >= val) {
                result += sym;
                num -= val;
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Convert integer to Roman numeral string representation.
// - Approach: Greedy Value-Symbol Matching.
// - Intuition: Greedily subtract largest available Roman numeral symbol values from largest to smallest.
// - Complexity: Time: O(1) (bounded by maximum integer value 3999), Space: O(1).


// =========================================================
// 45. LETTER COMBINATIONS OF A PHONE NUMBER (LC 17)
// =========================================================

class Solution45 {
    vector<string> result;
    string current;
    string d;
    const vector<string> mapping = {
        "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
    };

    void backtrack(int index) {
        if (index == (int)d.size()) {
            result.push_back(current);
            return;
        }
        for (char letter : mapping[d[index] - '0']) {
            current.push_back(letter);
            backtrack(index + 1);
            current.pop_back();
        }
    }

public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        d = std::move(digits);
        result.clear();
        current.clear();
        backtrack(0);
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Return all possible letter combinations that the number could represent from phone keypad.
// - Approach: Backtracking / Depth-First Search.
// - Intuition: Branch recursion for each mapped letter of the current digit.
// - Complexity: Time: O(4^N * N), Space: O(N) recursion stack.


// =========================================================
// 46. VALID PARENTHESES (LC 20)
// =========================================================

class Solution46 {
public:
    bool isValid(string s) {
        stack<char> st;
        for (char c : s) {
            if (c == '(' || c == '{' || c == '[') {
                st.push(c);
            } else {
                if (st.empty()) return false;
                char top = st.top();
                st.pop();
                if ((c == ')' && top != '(') ||
                    (c == '}' && top != '{') ||
                    (c == ']' && top != '[')) {
                    return false;
                }
            }
        }
        return st.empty();
    }
};
// Interview Explanation:
// - Problem Statement: Given a string with '()', '{}', and '[]', verify if parentheses are matched and closed in valid order.
// - Approach: Stack LIFO Matching.
// - Intuition:
//   * Opening brackets must be closed in reverse order of appearance (most recent open closed first).
//   * Push open brackets onto stack; pop and verify match upon encountering a closing bracket.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 47. GENERATE PARENTHESES (LC 22)
// =========================================================

class Solution47 {
    vector<string> ans;
    string cur;
    int limit;

    void solve(int open, int close) {
        if ((int)cur.size() == 2 * limit) {
            ans.push_back(cur);
            return;
        }

        if (open < limit) {
            cur.push_back('(');
            solve(open + 1, close);
            cur.pop_back();
        }

        if (close < open) {
            cur.push_back(')');
            solve(open, close + 1);
            cur.pop_back();
        }
    }

public:
    vector<string> generateParenthesis(int n) {
        ans.clear();
        cur.clear();
        limit = n;
        solve(0, 0);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Given n pairs of parentheses, generate all combinations of well-formed parentheses.
// - Approach: Constrained Backtracking.
// - Intuition:
//   * We can add '(' if openCount < n.
//   * We can add ')' if closeCount < openCount (ensures valid prefix invariant).
// - Complexity: Time: O(4^N / sqrt(N)) (nth Catalan number), Space: O(N) recursion stack.


// =========================================================
// 48. SUBSTRING WITH CONCATENATION OF ALL WORDS (LC 30)
// =========================================================

class Solution48 {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> ans;
        int len = words[0].size(), count = words.size();
        unordered_map<string, int> freq;

        for (string w : words) freq[w]++;

        for (int start = 0; start < len; start++) {
            unordered_map<string, int> window;
            int left = start, used = 0;
            // left pointer marks the start of the current window, right pointer moves in steps of word length
            for (int right = start; right + len <= s.size(); right += len) {
                string word = s.substr(right, len);

                if (!freq.count(word)) { // If the word is not in the original list, reset the window
                    window.clear();
                    used = 0;
                    left = right + len;
                    continue;
                }

                window[word]++, used++; // if the word is in the list, add it to the current window

                while (window[word] > freq[word]) { // if the word frequency in window is greater than in the original list, shrink the window from the left
                    window[s.substr(left, len)]--; // remove leftmost word from window
                    left += len, used--; // shift left pointer and reduce used counter
                }

                if (used == count) { 
                    ans.push_back(left);
                    window[s.substr(left, len)]--;
                    left += len, used--;
                }
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all starting indices of substring(s) in s that is a concatenation of each word in words exactly once.
// - Approach: Multi-Offset Sliding Window with Hash Map Frequencies.
// - Intuition:
//   * Since each word has fixed length `wordLen`, partition string scanning into `wordLen` disjoint sliding windows offset by 0..wordLen-1.
//   * Slide by chunks of `wordLen` and track word frequencies to achieve optimal O(N * wordLen) time.
// - Complexity: Time: O(N * wordLen), Space: O(wordCount * wordLen).


// =========================================================
// 49. GROUP ANAGRAMS (LC 49)
// =========================================================

class Solution49 {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;

        for (const string& s : strs) {
            string key = s;
            sort(key.begin(), key.end());
            groups[key].push_back(s);
        }

        vector<vector<string>> result;
        result.reserve(groups.size());
        for (auto& [_, vec] : groups) {
            result.push_back(std::move(vec));
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Given an array of strings, group the anagrams together in any order.
// - Approach: Sorted String Canonical Form Hash Map Grouping.
// - Intuition:
//   * Two strings are anagrams if and only if their sorted character sequences are identical.
//   * Use sorted string as the hash map key and append matching strings to the bucket vector.
// - Complexity: Time: O(N * K log K) where N = number of strings, K = max string length, Space: O(N * K).


// =========================================================
// 50. ISOMORPHIC STRINGS (LC 205)
// =========================================================

class Solution50 {
public:
    bool isIsomorphic(string s, string t) {
        if (s.length() != t.length()) return false;

        vector<int> m1(256, -1);
        vector<int> m2(256, -1);

        for (int i = 0; i < s.length(); i++) {
            if (m1[s[i]] != m2[t[i]]) return false;
            m1[s[i]] = i;
            m2[t[i]] = i;
        }

        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if two strings s and t are isomorphic (characters can be replaced to get t, preserving character order).
// - Approach: Bi-directional Character Mapping using Last-Seen Index Arrays.
// - Intuition:
//   * Both characters s[i] and t[i] must share the exact same last-seen index position at all times.
//   * If their recorded positions diverge, a character was mapped to multiple targets -> return false.
// - Complexity: Time: O(N), Space: O(1) auxiliary space (fixed alphabet array size 256).


// =========================================================
// 51. MEDIAN OF TWO SORTED ARRAYS (LC 4)
// =========================================================

class Solution51 {
public:
    int kthElement(vector<int>& a, vector<int>& b, int k) {
        if (a.size() > b.size()) return kthElement(b, a, k);

        int n = a.size(), m = b.size();
        int l = max(0, k - m), r = min(k, n);

        while (l <= r) {
            int x = (l + r) / 2;
            int y = k - x;

            int aL = x ? a[x - 1] : INT_MIN;
            int aR = x < n ? a[x] : INT_MAX;
            int bL = y ? b[y - 1] : INT_MIN;
            int bR = y < m ? b[y] : INT_MAX;

            if (aL <= bR && bL <= aR)
                return max(aL, bL);

            if (aL > bR) r = x - 1;
            else l = x + 1;
        }
        return -1;
    }

public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size() + nums2.size();
        if (n % 2 == 1) {
            return kthElement(nums1, nums2, n / 2 + 1);
        } else {
            return (kthElement(nums1, nums2, n / 2) + kthElement(nums1, nums2, n / 2 + 1)) / 2.0;
        }
    }
};
// Interview Explanation:
// - Problem Statement: Find median of two sorted arrays nums1 and nums2 in O(log (m+n)) runtime.
// - Approach: Binary Search on Partitioning of the Smaller Array.
// - Intuition:
//   * Partition both arrays such that left half contains (m + n + 1) / 2 elements and all left elements <= all right elements.
//   * Binary search partition point `i` in the smaller array; corresponding partition `j = (m + n + 1) / 2 - i` in larger array.
// - Complexity: Time: O(log(min(M, N))), Space: O(1).


// =========================================================
// 52. SEARCH IN ROTATED SORTED ARRAY (LC 33)
// =========================================================

class Solution52 {
public:
    int search(vector<int>& nums, int target) {
        int low = 0, high = nums.size() - 1;

        while (low <= high) {
            int mid = (low + high) / 2;

            if (nums[mid] == target) return mid;

            // Left half is sorted
            if (nums[low] <= nums[mid]) {
                if (nums[low] <= target && target < nums[mid])
                    high = mid - 1;
                else
                    low = mid + 1;
            }
            // Right half is sorted
            else {
                if (nums[mid] < target && target <= nums[high])
                    low = mid + 1;
                else
                    high = mid - 1;
            }
        }

        return -1;
    }
};
// Interview Explanation:
// - Problem Statement: Search for target in an array sorted in ascending order and rotated at an unknown pivot.
// - Approach: Modified Binary Search checking sorted segment.
// - Intuition:
//   * At least one half [low..mid] or [mid..high] is strictly sorted at all times.
//   * Check if target falls in the sorted range; if yes, narrow search to that half, otherwise search the opposite half.
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 53. SPLIT ARRAY LARGEST SUM (LC 410)
// =========================================================

class Solution53 {
    bool canSplit(const vector<int>& nums, int k, long long maxCap) {
        int subarrays = 1;
        long long currentSum = 0;
        for (int x : nums) {
            if (currentSum + x > maxCap) {
                subarrays++;
                currentSum = x;
            } else {
                currentSum += x;
            }
        }
        return subarrays <= k;
    }

public:
    int splitArray(vector<int>& nums, int k) {
        long long low = *max_element(nums.begin(), nums.end());
        long long high = accumulate(nums.begin(), nums.end(), 0LL);
        long long ans = high;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (canSplit(nums, k, mid)) {
                ans = mid;
                high = mid - 1; // Try smaller maximum sum
            } else {
                low = mid + 1;
            }
        }
        return (int)ans;
    }
};
// Interview Explanation:
// - Problem Statement: Split array into k non-empty subarrays minimizing the maximum sum among subarrays.
// - Approach: Binary Search on Answer + Greedy Capacity Validation.
// - Intuition: Feasibility function is monotonic: if max sum M is achievable with <= k parts, any M' > M is also achievable.
// - Complexity: Time: O(N log(Sum)), Space: O(1).


// =========================================================
// 54. SINGLE ELEMENT IN A SORTED ARRAY (LC 540)
// =========================================================

class Solution54 {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int low = 0, high = nums.size() - 1, ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (mid % 2 == 1) mid--; // Ensure mid is even for pair comparison
            if (nums[mid] == nums[mid + 1]) { // Pair found, single element is in the right half
                low = mid + 2;
            } else { // No pair, single element is in the left half or at mid
                ans = nums[mid];
                high = mid - 1;
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find the single element appearing once in a sorted array where every other element appears twice.
// - Approach: Even-Index Binary Search.
// - Intuition:
//   * Before the single element, pairs start at even indices: nums[2k] == nums[2k + 1].
//   * After the single element, the pattern shifts: pairs start at odd indices: nums[2k] != nums[2k + 1].
//   * Binary search checks this even-index pair invariant to discard half the search space in O(log N).
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 55. KOKO EATING BANANAS (LC 875)
// =========================================================

class Solution55 {
    bool canEatAll(const vector<int>& piles, int h, int speed) {
        long long hours = 0;
        for (int pile : piles) {
            hours += (pile + speed - 1) / speed;
        }
        return hours <= h;
    }

public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int low = 1, high = *max_element(piles.begin(), piles.end());
        int ans = high;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (canEatAll(piles, h, mid)) {
                ans = mid;
                high = mid - 1; // Try slower speed
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum eating speed k such that Koko eats all bananas within h hours.
// - Approach: Binary Search on Answer (Monotonic Eating Speed).
// - Intuition: Total hours required decreases monotonically with increasing speed. Binary search in range [1, max(pile)].
// - Complexity: Time: O(N log(maxPile)), Space: O(1).


// =========================================================
// 56. CAPACITY TO SHIP PACKAGES WITHIN D DAYS (LC 1011)
// =========================================================

class Solution56 {
    bool canShip(const vector<int>& weights, int days, int cap) {
        int neededDays = 1;
        int currentWeight = 0;
        for (int w : weights) {
            if (currentWeight + w > cap) {
                neededDays++;
                currentWeight = w;
            } else {
                currentWeight += w;
            }
        }
        return neededDays <= days;
    }

public:
    int shipWithinDays(vector<int>& weights, int days) {
        int low = *max_element(weights.begin(), weights.end());
        int high = accumulate(weights.begin(), weights.end(), 0);
        int ans = high;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (canShip(weights, days, mid)) {
                ans = mid;
                high = mid - 1; // Try smaller capacity
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find least ship weight capacity to ship all packages within given days.
// - Approach: Binary Search on Capacity Answer.
// - Intuition: Minimum capacity must be at least max(weight), maximum sum(weights). Check feasibility in O(N).
// - Complexity: Time: O(N log(Sum)), Space: O(1).


// =========================================================
// 57. FIND SMALLEST DIVISOR GIVEN THRESHOLD (LC 1283)
// =========================================================

class Solution57 {
    bool check(const vector<int>& nums, int threshold, int divisor) {
        long long sum = 0;
        for (int x : nums) {
            sum += (x + divisor - 1) / divisor;
        }
        return sum <= threshold;
    }

public:
    int smallestDivisor(vector<int>& nums, int threshold) {
        int low = 1, high = *max_element(nums.begin(), nums.end());
        int ans = high;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (check(nums, threshold, mid)) {
                ans = mid;
                high = mid - 1; // Try smaller divisor
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find smallest divisor such that sum of divisions (rounded up) <= threshold.
// - Approach: Monotonic Binary Search on Divisor.
// - Intuition: Sum is monotonically non-increasing with respect to divisor. Binary search in range [1, max(nums)].
// - Complexity: Time: O(N log(maxVal)), Space: O(1).
