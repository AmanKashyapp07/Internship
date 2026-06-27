/**
 * LeetCode 44 - Wildcard Matching
 *
 * Description:
 * Given an input string s and a pattern p, implement wildcard pattern matching with support for '?' and '*'.
 *
 * Approach:
 * - Dynamic programming with Memoization.
 * - Let `helper(i, j)` return if `s[i...]` matches `p[j...]`.
 * - Transition:
 *   - If `p[j] == '*'`, we can match 0 characters (`helper(i, j+1)`) or 1+ characters (`helper(i+1, j)`).
 *   - If `p[j] == '?'` or `p[j] == s[i]`, match character: `helper(i+1, j+1)`.
 *
 * Time Complexity: O(len(s) * len(p))
 * Space Complexity: O(len(s) * len(p))
 */

#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    string str;
    string pattern;
    vector<vector<int>> memo;

    bool helper(int i, int j) {
        if (i == str.size() && j == pattern.size()) return true;
        if (j == pattern.size()) return false;
        if (i == str.size()) {
            for (size_t k = j; k < pattern.size(); ++k) {
                if (pattern[k] != '*') return false; // Verify remaining chars in pattern are only '*'
            }
            return true;
        }
        if (memo[i][j] != -1) return memo[i][j];
        if (pattern[j] == str[i] || pattern[j] == '?') return memo[i][j] = helper(i + 1, j + 1);
        if (pattern[j] == '*') return memo[i][j] = helper(i + 1, j) || helper(i, j + 1); // '*' matches zero (j+1) or one/more (i+1)
        return memo[i][j] = false;
    }

    bool isMatch(string s, string p) {
        str = s;
        pattern = p;
        memo.assign(str.size(), vector<int>(pattern.size(), -1));
        return helper(0, 0);
    }
};