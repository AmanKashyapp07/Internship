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
    bool helper(string &s, string &p, int i, int j, vector<vector<int>> &dp) {
        if (i == s.size() && j == p.size()) return true;
        if (j == p.size()) return false;
        if (i == s.size()) {
            for (size_t k = j; k < p.size(); ++k) if (p[k] != '*') return false; // Verify remaining chars in pattern are only '*'
            return true;
        }
        if (dp[i][j] != -1) return dp[i][j];
        if (p[j] == s[i] || p[j] == '?') return dp[i][j] = helper(s, p, i + 1, j + 1, dp);
        if (p[j] == '*') return dp[i][j] = helper(s, p, i + 1, j, dp) || helper(s, p, i, j + 1, dp); // '*' matches zero (j+1) or one/more (i+1)
        return dp[i][j] = false;
    }
public:
    bool isMatch(string s, string p) {
        vector<vector<int>> dp(s.size(), vector<int>(p.size(), -1));
        return helper(s, p, 0, 0, dp);
    }
};