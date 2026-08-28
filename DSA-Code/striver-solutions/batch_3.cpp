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

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 3 (Problems 01 to 17)
// ============================================================================

ListNode* getMid(ListNode* head) {
    ListNode *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode *tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode *mid = getMid(head);
    ListNode *right = mid->next;
    mid->next = nullptr; // sever list into two halves
    ListNode *left = sortList(head);
    right = sortList(right);
    return mergeTwoLists(left, right);
}
// Interview Explanation:
// - Problem Statement: Sort a linked list in O(N log N) time complexity.
// - Approach: Top-Down Merge Sort with fast/slow pointer midpoint splitting.
// - Intuition: Find the midpoint of the linked list using fast & slow pointers (starting fast at `head->next` so mid is end of left half), sever the connection (`mid->next = nullptr`), recursively sort both halves, and merge them with a dummy pointer.
// - Complexity: Time: O(N log N), Space: O(log N) recursion call stack.

ListNode* oddEvenList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode *odd = head, *even = head->next, *even_head = head->next;
    while (even && even->next) {
        odd->next = even->next;
        odd = odd->next;
        even->next = odd->next;
        even = even->next;
    }
    odd->next = even_head;
    return head;
}
// Interview Explanation:
// - Problem Statement: Group all odd-indexed nodes together followed by even-indexed nodes in O(1) extra space.
// - Approach: Two pointers linking odd and even indexed nodes separately.
// - Intuition: Maintain `odd` and `even` pointers while keeping track of `even_head`. Advance by connecting `odd->next = even->next` and `even->next = odd->next`. Finally, connect the tail of the odd list to `even_head`.
// - Complexity: Time: O(N) single pass, Space: O(1) in-place pointer manipulation.

vvi subsets(vi& nums) {
    int n = nums.size();
    int total_subsets = 1 << n;
    vvi ans;
    for (int mask = 0; mask < total_subsets; mask++) {
        vi subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset.push_back(nums[i]);
        }
        ans.push_back(subset);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate all possible subsets (the power set) of an array of unique integers.
// - Approach: Bit Manipulation (binary mask enumeration) or Backtracking.
// - Intuition: An array of size n has 2^n subsets. Each subset corresponds to a unique bitmask from 0 to 2^n - 1, where the i-th bit indicates whether nums[i] is included.
// - Complexity: Time: O(N * 2^N) generating all subsets, Space: O(1) auxiliary space (excluding result).

void findCombinations(int idx, int target, const vi& arr, vi& current, vvi& ans) {
    if (target == 0) {
        ans.push_back(current);
        return;
    }
    for (int i = idx; i < (int)arr.size(); i++) {
        if (arr[i] > target) continue; // prune branch
        current.push_back(arr[i]);
        findCombinations(i, target - arr[i], arr, current, ans); // reuse same index
        current.pop_back();
    }
}

vvi combinationSum(vi& candidates, int target) {
    vvi ans;
    vi current;
    findCombinations(0, target, candidates, current, ans);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all unique combinations of candidates that sum to target, where each number may be used unlimited times.
// - Approach: Backtracking / Depth-First Search with branch pruning.
// - Intuition: At each index, either choose the current candidate (subtracting from target and staying at the same index `i` to allow reuse) or move to subsequent candidates; backtrack by popping the chosen element.
// - Complexity: Time: O(2^T * K) where T is target / min(candidates) and K is average combination length, Space: O(T) recursion depth.

void solveNQueensHelper(int col, int n, vector<string>& board, vector<vector<string>>& ans,
                        vi& left_row, vi& lower_diag, vi& upper_diag) {
    if (col == n) {
        ans.push_back(board);
        return;
    }
    for (int row = 0; row < n; row++) {
        if (left_row[row] == 0 && lower_diag[row + col] == 0 && upper_diag[n - 1 + col - row] == 0) {
            board[row][col] = 'Q';
            left_row[row] = 1;
            lower_diag[row + col] = 1;
            upper_diag[n - 1 + col - row] = 1;

            solveNQueensHelper(col + 1, n, board, ans, left_row, lower_diag, upper_diag);

            board[row][col] = '.';
            left_row[row] = 0;
            lower_diag[row + col] = 0;
            upper_diag[n - 1 + col - row] = 0;
        }
    }
}

vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> ans;
    vector<string> board(n, string(n, '.'));
    vi left_row(n, 0), lower_diag(2 * n - 1, 0), upper_diag(2 * n - 1, 0);
    solveNQueensHelper(0, n, board, ans, left_row, lower_diag, upper_diag);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Place n queens on an n x n chessboard such that no two queens attack each other.
// - Approach: Backtracking column-by-column with O(1) hash lookup arrays for row and diagonal safety.
// - Intuition: Placing one queen per column ensures no column conflicts. To check row and diagonals in O(1): track `row`, `row + col` (anti-diagonal), and `n - 1 + col - row` (main diagonal) using boolean arrays.
// - Complexity: Time: O(N!) search space, Space: O(N) auxiliary arrays + O(N^2) board.

bool isValidSudoku(vector<vector<char>>& board, int row, int col, char c) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == c) return false;
        if (board[i][col] == c) return false;
        if (board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == c) return false;
    }
    return true;
}

bool solveSudoku(vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                for (char c = '1'; c <= '9'; c++) {
                    if (isValidSudoku(board, i, j, c)) {
                        board[i][j] = c;
                        if (solveSudoku(board)) return true;
                        board[i][j] = '.'; // backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Solve a 9x9 Sudoku puzzle by filling empty cells '.' such that rows, columns, and 3x3 sub-boxes contain digits '1'-'9'.
// - Approach: Recursive Backtracking with constraint validation.
// - Intuition: Traverse the board to find the first empty cell, try digits '1' through '9'. If placing digit `c` is valid in the current row, column, and 3x3 box (`3*(r/3) + i/3, 3*(c/3) + i%3`), place it and recurse. If dead end, reset cell to '.' and backtrack.
// - Complexity: Time: O(9^(empty_cells)) worst case, Space: O(81) recursion stack.

bool isSafeColor(int node, const vvi& adj, vi& color, int col) {
    for (int neighbor : adj[node]) {
        if (color[neighbor] == col) return false;
    }
    return true;
}

bool solveMColor(int node, int n, int m, const vvi& adj, vi& color) {
    if (node == n) return true;
    for (int c = 1; c <= m; c++) {
        if (isSafeColor(node, adj, color, c)) {
            color[node] = c;
            if (solveMColor(node + 1, n, m, adj, color)) return true;
            color[node] = 0; // backtrack
        }
    }
    return false;
}

bool graphColoring(int n, const vector<pii>& edges, int m) {
    vvi adj(n);
    for (auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vi color(n, 0);
    return solveMColor(0, n, m, adj, color);
}
// Interview Explanation:
// - Problem Statement: Determine if an undirected graph of n vertices can be colored with at most m colors such that no two adjacent vertices have the same color.
// - Approach: Backtracking vertex-by-vertex (assigning color 1 to m).
// - Intuition: For each vertex from 0 to n - 1, test every color from 1 to m. If no neighbor currently has that color, assign it and recurse to node + 1. If all choices fail, backtrack to previous node.
// - Complexity: Time: O(M^N) upper bound, Space: O(N) for color array and recursion stack.

bool dfsWordSearch(vector<vector<char>>& board, const string& word, int i, int j, int idx) {
    if (idx == (int)word.size()) return true;
    if (i < 0 || i >= (int)board.size() || j < 0 || j >= (int)board[0].size() || board[i][j] != word[idx]) {
        return false;
    }

    char temp = board[i][j];
    board[i][j] = '#'; // mark visited in-place

    bool found = dfsWordSearch(board, word, i + 1, j, idx + 1) ||
                 dfsWordSearch(board, word, i - 1, j, idx + 1) ||
                 dfsWordSearch(board, word, i, j + 1, idx + 1) ||
                 dfsWordSearch(board, word, i, j - 1, idx + 1);

    board[i][j] = temp; // unmark / backtrack
    return found;
}

bool exist(vector<vector<char>>& board, string word) {
    int m = board.size(), n = board[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == word[0] && dfsWordSearch(board, word, i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if a word exists in a 2D grid of characters through horizontally or vertically adjacent cells without reusing the same cell.
// - Approach: DFS + Backtracking with in-place visited marking.
// - Intuition: Start DFS from every cell matching `word[0]`. Temporarily replace the current cell with '#' to prevent self-crossing during the search. Recurse in 4 directions for `word[idx + 1]`. Backtrack by restoring original character.
// - Complexity: Time: O(M * N * 4^L) where L is word length, Space: O(L) recursion depth.

vi nextGreaterElement(vi& nums) {
    int n = nums.size();
    vi nge(n, -1);
    stack<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && st.top() <= nums[i]) {
            st.pop();
        }
        if (!st.empty()) nge[i] = st.top();
        st.push(nums[i]);
    }

    return nge;
}
// Interview Explanation:
// - Problem Statement: For each element in the array, find the first greater element to its right (or -1 if none exists).
// - Approach: Monotonic Decreasing Stack traversed from right to left.
// - Intuition: Elements smaller than or equal to `nums[i]` can never be the next greater element for any element to the left of `i`. Pop them from the stack; the stack top then represents the immediate next greater element.
// - Complexity: Time: O(N) each element pushed and popped at most once, Space: O(N) stack space.

int trap(vi& height) {
    int l = 0, r = height.size() - 1;
    int left_max = 0, right_max = 0;
    int water = 0;

    while (l < r) {
        if (height[l] <= height[r]) {
            if (height[l] >= left_max) left_max = height[l];
            else water += left_max - height[l];
            l++;
        } else {
            if (height[r] >= right_max) right_max = height[r];
            else water += right_max - height[r];
            r--;
        }
    }

    return water;
}
// Interview Explanation:
// - Problem Statement: Calculate how much water can be trapped after raining on an elevation map.
// - Approach: Two Pointers (left and right) with dynamic running max barriers.
// - Intuition: Water trapped above any bar is determined by `min(max_left, max_right) - height[i]`. If `height[l] <= height[r]`, water level is bounded by `left_max` regardless of right side; update `left_max` or accumulate water `left_max - height[l]`. Symmetrically handle right pointer.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space (optimal vs O(N) prefix/suffix arrays).

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    vi freq(26, 0);
    for (int i = 0; i < (int)s.size(); i++) {
        freq[s[i] - 'a']++;
        freq[t[i] - 'a']--;
    }
    for (int f : freq) if (f != 0) return false;
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if string t is an anagram of string s.
// - Approach: Frequency array counting.
// - Intuition: Anagrams have identical character distributions. Increment frequency for characters in s and decrement for t in a single pass; verify all counts resolve to 0.
// - Complexity: Time: O(N) single pass, Space: O(1) 26-size frequency array.

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> ans;
    for (auto& [key, list] : groups) {
        ans.push_back(list);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Group strings that are anagrams of each other.
// - Approach: Hash Map using sorted string as canonical key.
// - Intuition: Sorting each string creates an identical canonical key for all anagrams (e.g. "eat", "tea", "ate" -> "aet"). Group original strings under their canonical keys.
// - Complexity: Time: O(N * K log K) where N is number of strings and K is max string length, Space: O(N * K).

bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(') st.push(')');
        else if (c == '{') st.push('}');
        else if (c == '[') st.push(']');
        else {
            if (st.empty() || st.top() != c) return false;
            st.pop();
        }
    }
    return st.empty();
}
// Interview Explanation:
// - Problem Statement: Determine if input string of brackets '()', '{}', '[]' is valid (correctly opened and closed in proper order).
// - Approach: Stack matching closing brackets.
// - Intuition: Push expected matching closing bracket onto the stack upon seeing an open bracket. When encountering a closing bracket, verify that stack is not empty and top matches current character.
// - Complexity: Time: O(N) single pass, Space: O(N) stack space.

bool isPalindrome(string s) {
    int l = 0, r = s.size() - 1;
    while (l < r) {
        while (l < r && !isalnum(s[l])) l++;
        while (l < r && !isalnum(s[r])) r--;
        if (tolower(s[l]) != tolower(s[r])) return false;
        l++; r--;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if a string is a palindrome after converting uppercase to lowercase and removing all non-alphanumeric characters.
// - Approach: Two Pointers inward convergence with alphanumeric filtering.
// - Intuition: Advance left and right pointers past non-alphanumeric characters, compare case-insensitive characters, and converge until pointers meet.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

string longestPalindrome(string s) {
    int n = s.size(), start = 0, max_len = 0;

    auto expandAroundCenter = [&](int l, int r) {
        while (l >= 0 && r < n && s[l] == s[r]) {
            l--; r++;
        }
        int len = r - l - 1;
        if (len > max_len) {
            max_len = len;
            start = l + 1;
        }
    };

    for (int i = 0; i < n; i++) {
        expandAroundCenter(i, i);     // odd length palindromes
        expandAroundCenter(i, i + 1); // even length palindromes
    }

    return s.substr(start, max_len);
}
// Interview Explanation:
// - Problem Statement: Find the longest palindromic substring in string s.
// - Approach: Expand Around Center for odd and even midpoints (2N - 1 centers).
// - Intuition: Every palindrome expands symmetrically from its center. Check 2N - 1 centers: odd centers (single char `(i, i)`) and even centers (pair `(i, i+1)`), expanding outward as long as boundary characters match.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.

int countSubstrings(string s) {
    int n = s.size(), count = 0;

    auto expandCount = [&](int l, int r) {
        int cnt = 0;
        while (l >= 0 && r < n && s[l] == s[r]) {
            cnt++;
            l--; r++;
        }
        return cnt;
    };

    for (int i = 0; i < n; i++) {
        count += expandCount(i, i);     // odd centers
        count += expandCount(i, i + 1); // even centers
    }

    return count;
}
// Interview Explanation:
// - Problem Statement: Count how many palindromic substrings exist in string s.
// - Approach: Expand Around Center across all 2N - 1 possible centers.
// - Intuition: From each character and character gap, expand outward while characters match, incrementing the palindrome count for each valid symmetric expansion.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.

string encodeStrings(const vector<string>& strs) {
    string encoded = "";
    for (const string& s : strs) {
        encoded += to_string(s.size()) + "#" + s;
    }
    return encoded;
}

vector<string> decodeStrings(const string& s) {
    vector<string> ans;
    int i = 0, n = s.size();

    while (i < n) {
        int delimiter = s.find('#', i);
        int len = stoi(s.substr(i, delimiter - i));
        ans.push_back(s.substr(delimiter + 1, len));
        i = delimiter + 1 + len;
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Design an algorithm to encode a list of strings to a single string and decode it back without delimiter ambiguity.
// - Approach: Length-prefixed encoding with delimiter (`<length>#<string>`).
// - Intuition: Prefixing each string with its byte length followed by a delimiter '#' guarantees that arbitrary special characters (including '#' or newline) within the string payload are not misinterpreted as delimiters.
// - Complexity: Time: O(Total Characters) for both encode and decode, Space: O(1) auxiliary space.
