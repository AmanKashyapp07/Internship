/**
 * LeetCode 115 - Distinct Subsequences
 *
 * Description:
 * Given two strings s and t, return the number of distinct subsequences of s which equals t.
 *
 * Approach:
 * - Dynamic programming with Memoization.
 * - Let `solve(i, j)` return the number of distinct subsequences of `s[i...]` matching `t[j...]`.
 * - Transition:
 *   - If `s[i] == t[j]`, we can either match them: `solve(i+1, j+1)` or skip `s[i]`: `solve(i+1, j)`.
 *   - If `s[i] != t[j]`, we must skip `s[i]`: `solve(i+1, j)`.
 *
 * Time Complexity: O(m * n) where m = len(s) and n = len(t).
 * Space Complexity: O(m * n)
 */

#include <vector>
#include <string>
using namespace std;

class Solution {
    int countSubsequences(const string &s, const string &t, int i, int j, vector<vector<int>> &memo) {
        if (j == t.size()) return 1; // Base case: complete match
        if (i == s.size()) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        int ans = countSubsequences(s, t, i + 1, j, memo); // Skip s[i]
        if (s[i] == t[j]) ans += countSubsequences(s, t, i + 1, j + 1, memo); // Match s[i] and t[j]
        return memo[i][j] = ans;
    }
public:
    int numDistinct(string s, string t) {
        vector<vector<int>> memo(s.size(), vector<int>(t.size(), -1));
        return countSubsequences(s, t, 0, 0, memo);
    }
};