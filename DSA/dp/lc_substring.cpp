/**
 * GeeksforGeeks - Longest Common Substring
 *
 * Description:
 * Given two strings S1 and S2, find the length of the longest common substring.
 *
 * Approach:
 * - 2D dynamic programming.
 * - Let `dp[i][j]` be the length of the longest common suffix of `S1[0...i-1]` and `S2[0...j-1]`.
 * - Transition: `dp[i][j] = dp[i-1][j-1] + 1` if `S1[i-1] == S2[j-1]`, else `0`.
 * - The answer is the maximum value in the DP table.
 *
 * Time Complexity: O(n * m) where n = len(S1) and m = len(S2).
 * Space Complexity: O(n * m)
 */

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int longestCommonSubstr(string a, string b) {
        int n = a.size(), m = b.size(), ans = 0;
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        // DP state: dp[i][j] is the length of common suffix of a[0...i-1] and b[0...j-1]
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (a[i - 1] == b[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1; // Transition
                    ans = max(ans, dp[i][j]);
                }
            }
        }
        return ans;
    }
};