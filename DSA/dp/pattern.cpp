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
    string s1;
    string p1;
    vector<vector<int>> memo;

    bool helper(int i, int j) {
        if (i == s1.size() && j == p1.size()) return true;
        if (j == p1.size()) return false;
        if (i == s1.size()) {
            for (size_t k = j; k < p1.size(); ++k) {
                if (p1[k] != '*') return false; // Verify remaining chars in pattern are only '*'
            }
            return true;
        }
        if (memo[i][j] != -1) return memo[i][j];
        if (p1[j] == s1[i] || p1[j] == '?') return memo[i][j] = helper(i + 1, j + 1);
        if (p1[j] == '*') return memo[i][j] = helper(i + 1, j) || helper(i, j + 1); // '*' matches zero (j+1) or one/more (i+1)
        return memo[i][j] = false;
    }

    bool isMatch(string s, string p) {
        s1 = s;
        p1 = p;
        memo.assign(s1.size(), vector<int>(p1.size(), -1));
        return helper(0, 0);
    }
};