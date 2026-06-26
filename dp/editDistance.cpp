/**
 * LeetCode 72 - Edit Distance
 *
 * Description:
 * Given two strings word1 and word2, return the minimum number of operations required to convert word1 to word2.
 * You have three operations: insert, delete, or replace a character.
 *
 * Approach:
 * - Dynamic programming with Memoization.
 * - Let `solve(i, j)` be the minimum operations to convert `s[i...]` to `t[j...]`.
 * - Transition:
 *   - If `s[i] == t[j]`, cost is `solve(i+1, j+1)`.
 *   - Else, take `1 + min({solve(i, j+1), solve(i+1, j), solve(i+1, j+1)})` (insert, delete, replace).
 *
 * Time Complexity: O(m * n) where m = len(word1) and n = len(word2).
 * Space Complexity: O(m * n)
 */

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
    vector<vector<int>> dp;
    int solve(int i, int j, string &s, string &t) {
        if (i == s.size()) return t.size() - j; // Base case: insert remaining characters
        if (j == t.size()) return s.size() - i; // Base case: delete remaining characters
        if (dp[i][j] != -1) return dp[i][j];
        if (s[i] == t[j]) return dp[i][j] = solve(i + 1, j + 1, s, t);
        return dp[i][j] = 1 + min({solve(i, j + 1, s, t), solve(i + 1, j, s, t), solve(i + 1, j + 1, s, t)}); // Insert, Delete, Replace transitions
    }
public:
    int minDistance(string word1, string word2) {
        dp.assign(word1.size(), vector<int>(word2.size(), -1));
        return solve(0, 0, word1, word2);
    }
};