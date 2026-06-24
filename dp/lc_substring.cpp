/**
 * Problem: Longest Common Substring (https://practice.geeksforgeeks.org/problems/longest-common-substring1452/1)
 * Find the length of the longest common substring of two strings.
 * Time: O(N * M) time, O(N * M) space.
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