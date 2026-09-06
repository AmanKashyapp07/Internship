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
// 39. SURROUNDED REGIONS (LC 130)
// =========================================================

class Solution39 {
    int m, n;

    void dfs(vector<vector<char>>& board, int r, int c) {
        if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != 'O') return;
        board[r][c] = '#';
        dfs(board, r + 1, c);
        dfs(board, r - 1, c);
        dfs(board, r, c + 1);
        dfs(board, r, c - 1);
    }

public:
    void solve(vector<vector<char>>& board) {
        m = (int)board.size();
        if (m == 0) return;
        n = (int)board[0].size();

        for (int i = 0; i < m; ++i) {
            if (board[i][0] == 'O') dfs(board, i, 0);
            if (board[i][n - 1] == 'O') dfs(board, i, n - 1);
        }
        for (int j = 0; j < n; ++j) {
            if (board[0][j] == 'O') dfs(board, 0, j);
            if (board[m - 1][j] == 'O') dfs(board, m - 1, j);
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                else if (board[i][j] == '#') board[i][j] = 'O'; 
            }
        }
    }
};
// Interview Explanation:
// - Problem Statement: Capture all regions on an M x N board that are 4-directionally surrounded by 'X'.
// - Approach: Boundary Flood-Fill (DFS/BFS) Inversion.
// - Intuition:
//   * Any 'O' connected to the outer boundary can never be surrounded.
//   * Run DFS/BFS from all border 'O' cells, temporarily marking them '#'.
//   * Remaining 'O' cells are completely surrounded -> flip to 'X'. Finally, restore '#' back to 'O'.
// - Complexity: Time: O(M * N), Space: O(M * N) recursion depth.


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
// 43. ZIGZAG CONVERSION (LC 6)
// =========================================================

class Solution43 {
public:
    string convert(string s, int numRows) {
        if (numRows == 1) return s;
        vector<string> rows(numRows);
        int row = 0, dir = 1;
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
public:
    vector<string> ans;

    void solve(string s, int open, int close, int n) {
        if (s.size() == 2 * n) {
            ans.push_back(s);
            return;
        }

        if (open < n) // If we can still add an opening parenthesis
            solve(s + "(", open + 1, close, n);

        if (close < open) // If we can add a closing parenthesis without violating the balance
            solve(s + ")", open, close + 1, n);
    }

    vector<string> generateParenthesis(int n) {
        solve("", 0, 0, n);
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

class Solution {
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
// - Approach: Canonical Sorted Representation as Hash Map Key.
// - Intuition:
//   * Any two anagrams become identical strings after sorting their characters.
//   * Use sorted string as key in hash map to collect all matching original words.
// - Complexity: Time: O(N * K log K) where K is max string length, Space: O(N * K).


// =========================================================
// 50. ISOMORPHIC STRINGS (LC 205)
// =========================================================

class Solution50 {
public:
    bool isIsomorphic(string s, string t) {
        if (s.size() != t.size()) return false;
        int a[256] = {}, b[256] = {};

        for (int i = 0; i < s.size(); i++) {
            if (a[s[i]] != b[t[i]]) return false;
            a[s[i]] = i + 1; // Store last seen index + 1 to avoid default 0 confusion
            b[t[i]] = i + 1; // Store last seen index + 1 to avoid default 0 confusion
        }

        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if two strings s and t are isomorphic (one-to-one character mapping preserving order).
// - Approach: Bi-Directional Last Seen Index Comparison.
// - Intuition:
//   * If two characters map to each other, they must have appeared at identical earlier indices across both strings.
//   * Using fixed arrays of size 256 avoids dynamic hash table overhead.
// - Complexity: Time: O(N), Space: O(1) (fixed 256 integers).


// =========================================================
// 51. MEDIAN OF TWO SORTED ARRAYS (LC 4)
// =========================================================

class Solution51 {
public:
    int findKthElementinSortedArrays(const vector<int> &a, const vector<int> &b, int k) {
        if (a.size() > b.size())
            return findKthElementinSortedArrays(b, a, k);
        int n = a.size(), m = b.size();
        int low = max(0, k - m);
        int high = min(k, n);

        while (low <= high) {
            int p1 = (low + high) / 2;
            int p2 = k - p1;
            int left1 = (p1 == 0) ? INT_MIN : a[p1 - 1];
            int left2 = (p2 == 0) ? INT_MIN : b[p2 - 1];
            int right1 = (p1 == n) ? INT_MAX : a[p1];
            int right2 = (p2 == m) ? INT_MAX : b[p2];

            if (left1 <= right2 && left2 <= right1)
                return max(left1, left2);
            if (left1 > right2)
                high = p1 - 1;
            else
                low = p1 + 1;
        }

        return -1;
    }
    int findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int totalLen = (int)nums1.size() + (int)nums2.size();
        if (totalLen % 2 == 1) {
            return findKthElementinSortedArrays(nums1, nums2, totalLen / 2 + 1);
        } else {
            int leftMedian = findKthElementinSortedArrays(nums1, nums2, totalLen / 2);
            int rightMedian = findKthElementinSortedArrays(nums1, nums2, totalLen / 2 + 1);
            return (leftMedian + rightMedian) / 2.0;
        }
    }
};
// Interview Explanation:
// - Problem Statement: Find median of two sorted arrays nums1 and nums2 in O(log(m + n)) time.
// - Approach: Binary Search on Partitioning of the Smaller Array.
// - Intuition:
//   * Partition both arrays into left and right halves such that combined left half has (m + n + 1)/2 elements.
//   * Condition for valid median partition: max(left1, left2) <= min(right1, right2).
//   * Binary searching partition i in smaller array directly determines j = halfLen - i.
// - Complexity: Time: O(log(min(M, N))), Space: O(1).

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


