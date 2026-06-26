/**
 * LeetCode 1143 - Longest Common Subsequence
 *
 * Description:
 * Given two strings text1 and text2, return the length of their longest common subsequence.
 *
 * Approach:
 * - Dynamic programming using both Tabulation and Memoization styles.
 * - In Tabulation:
 *   - Let `dp[i][j]` represent the LCS length of `text1[0...i-1]` and `text2[0...j-1]`.
 *   - Transition: `dp[i][j] = dp[i-1][j-1] + 1` if characters match, else `max(dp[i-1][j], dp[i][j-1])`.
 *   - Reconstruct the LCS string by backtracking from `(n, m)`.
 *
 * Time Complexity: O(n * m)
 * Space Complexity: O(n * m)
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class LCS_tabulation {
public:
    pair<int, string> longestCommonSubsequence(string text1, string text2) {
        int n = text1.size(), m = text2.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (text1[i - 1] == text2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        string lcs = ""; int i = n, j = m;
        // Backtrack to reconstruct the LCS string
        while (i > 0 && j > 0) {
            if (text1[i - 1] == text2[j - 1]) { lcs += text1[i - 1]; i--; j--; }
            else if (dp[i - 1][j] > dp[i][j - 1]) i--;
            else j--;
        }
        reverse(lcs.begin(), lcs.end());
        return {dp[n][m], lcs};
    }
};

class LCS_memoization {
    int lcs(string &s, string &t, int i, int j, vector<vector<int>> &dp) {
        if (i < 0 || j < 0) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        if (s[i] == t[j]) return dp[i][j] = 1 + lcs(s, t, i - 1, j - 1, dp);
        return dp[i][j] = max(lcs(s, t, i - 1, j, dp), lcs(s, t, i, j - 1, dp));
    }
public:
    int longestCommonSubsequence(string text1, string text2) {
        vector<vector<int>> dp(text1.size(), vector<int>(text2.size(), -1));
        return lcs(text1, text2, text1.size() - 1, text2.size() - 1, dp);
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    string s, t;
    if (cin >> s >> t) {
        LCS_tabulation solver;
        auto [len, lcs_str] = solver.longestCommonSubsequence(s, t);
        cout << len << '\n';
    }
}