#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    // 1. Longest Common Subsequence (Length)
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.size(), n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (text1[i - 1] == text2[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        return dp[m][n];
    }

    // 2. Longest Common Subsequence (Reconstruct String)
    string getLCS(string text1, string text2) {
        int m = text1.size(), n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (text1[i - 1] == text2[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }

        // Reconstruct LCS
        string lcs;
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (text1[i - 1] == text2[j - 1]) {
                lcs.push_back(text1[i - 1]);
                --i; --j;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                --i;
            } else {
                --j;
            }
        }
        reverse(lcs.begin(), lcs.end());
        return lcs;
    }

    // 3. Longest Common Substring (Contiguous)
    int longestCommonSubstring(string text1, string text2) {
        int m = text1.size(), n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        int maxLen = 0;

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                    maxLen = max(maxLen, dp[i][j]);
                } else {
                    dp[i][j] = 0;
                }
            }
        return maxLen;
    }

    // 4. Shortest Common Supersequence
    string shortestCommonSupersequence(string str1, string str2) {
        int m = str1.size(), n = str2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (str1[i - 1] == str2[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }

        // Reconstruct SCS
        string scs;
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (str1[i - 1] == str2[j - 1]) {
                scs.push_back(str1[i - 1]);
                --i; --j;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                scs.push_back(str1[i - 1]);
                --i;
            } else {
                scs.push_back(str2[j - 1]);
                --j;
            }
        }
        while (i > 0) scs.push_back(str1[--i]);
        while (j > 0) scs.push_back(str2[--j]);

        reverse(scs.begin(), scs.end());
        return scs;
    }

    // 5. Longest Palindromic Subsequence
    int longestPalindromeSubseq(string s) {
        string rev = s;
        reverse(rev.begin(), rev.end());
        return longestCommonSubsequence(s, rev);
    }

    // 6. Min Insertions + Deletions to convert s1 to s2
    pair<int, int> minOperations(string s1, string s2) {
        int lcs = longestCommonSubsequence(s1, s2);
        return {s1.size() - lcs, s2.size() - lcs};
    }

    // 7. Minimum Insertions to make Palindrome
    int minInsertions(string s) {
        return s.size() - longestPalindromeSubseq(s);
    }

    // 8. Longest Repeating Subsequence
    int longestRepeatingSubsequence(string s) {
        int n = s.size();
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j) {
                if (s[i - 1] == s[j - 1] && i != j)
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        return dp[n][n];
    }

    // 9. Edit Distance (Levenshtein)
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));

        for (int i = 0; i <= m; ++i) dp[i][0] = i;
        for (int j = 0; j <= n; ++j) dp[0][j] = j;

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (word1[i - 1] == word2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        return dp[m][n];
    }

    // 10. Distinct Subsequences
    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();
        vector<vector<unsigned long long>> dp(m + 1, vector<unsigned long long>(n + 1, 0));

        for (int i = 0; i <= m; ++i) dp[i][0] = 1;

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j) {
                if (s[i - 1] == t[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
                else
                    dp[i][j] = dp[i - 1][j];
            }
        return dp[m][n];
    }
};

int main() {
    Solution solver;

    assert(solver.longestCommonSubsequence("abcde", "ace") == 3);
    assert(solver.getLCS("abcde", "ace") == "ace");
    assert(solver.longestCommonSubstring("abcde", "abfce") == 2);
    assert(solver.shortestCommonSupersequence("abac", "cab") == "cabac");
    assert(solver.longestPalindromeSubseq("bbbab") == 4);
    assert(solver.minInsertions("mbadm") == 2);
    assert(solver.longestRepeatingSubsequence("aabebcdd") == 3);
    assert(solver.minDistance("horse", "ros") == 3);
    assert(solver.numDistinct("rabbbit", "rabbit") == 3);

    cout << "All tests passed successfully!" << endl;
    return 0;
}