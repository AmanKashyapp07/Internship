/**
 * LeetCode 1092 - Shortest Common Supersequence
 *
 * Description:
 * Given two strings str1 and str2, return the shortest string that has both str1 and str2 as subsequences.
 *
 * Approach:
 * - Solve using Longest Common Subsequence (LCS).
 * - First build the LCS DP table.
 * - Backtrack from `(n, m)` to reconstruct the SCS string:
 *   - If characters match, add to SCS once and move diagonally.
 *   - Else, add the character that corresponds to the larger DP value direction and move.
 *
 * Time Complexity: O(n * m)
 * Space Complexity: O(n * m)
 */

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    string shortestCommonSupersequence(string str1, string str2) {
        int n = str1.size(), m = str2.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (str1[i - 1] == str2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        string ans = ""; int i = n, j = m;
        // Backtrack to reconstruct the shortest common supersequence
        while (i > 0 && j > 0) {
            if (str1[i - 1] == str2[j - 1]) { ans += str1[i - 1]; i--; j--; }
            else if (dp[i - 1][j] >= dp[i][j - 1]) { ans += str1[i - 1]; i--; }
            else { ans += str2[j - 1]; j--; }
        }
        while (i > 0) { ans += str1[i - 1]; i--; }
        while (j > 0) { ans += str2[j - 1]; j--; }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};