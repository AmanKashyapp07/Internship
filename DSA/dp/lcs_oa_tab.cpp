#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

/*
================================================================================
LCS (Longest Common Subsequence) Tabulation (Bottom-up 2D) Variants
================================================================================
*/

// =============================================================================
// LCS Tabulation Interface Functions
// =============================================================================

int longestCommonSubsequenceTab(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

string getLCSTab(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    string lcs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs.push_back(text1[i - 1]);
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

int longestCommonSubstringTab(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    int maxLen = 0;
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
                maxLen = max(maxLen, dp[i][j]);
            } else {
                dp[i][j] = 0;
            }
        }
    }
    return maxLen;
}

string shortestCommonSupersequenceTab(string str1, string str2) {
    int m = str1.length();
    int n = str2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    string scs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            scs.push_back(str1[i - 1]);
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            scs.push_back(str1[i - 1]);
            i--;
        } else {
            scs.push_back(str2[j - 1]);
            j--;
        }
    }
    while (i > 0) {
        scs.push_back(str1[i - 1]);
        i--;
    }
    while (j > 0) {
        scs.push_back(str2[j - 1]);
        j--;
    }
    reverse(scs.begin(), scs.end());
    return scs;
}

int longestPalindromeSubseqTab(string s) {
    string t = s;
    reverse(t.begin(), t.end());
    return longestCommonSubsequenceTab(s, t);
}

pair<int, int> minOperationsTab(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    int lcsLen = longestCommonSubsequenceTab(s1, s2);
    return {m - lcsLen, n - lcsLen};
}

int minInsertionsTab(string s) {
    int n = s.length();
    int lpsLen = longestPalindromeSubseqTab(s);
    return n - lpsLen;
}

int longestRepeatingSubseqTab(string s) {
    int n = s.length();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s[i - 1] == s[j - 1] && i != j) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][n];
}

int minDistanceTab(string word1, string word2) {
    int m = word1.length();
    int n = word2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({
                    dp[i - 1][j],
                    dp[i][j - 1],
                    dp[i - 1][j - 1]
                });
            }
        }
    }
    return dp[m][n];
}

int numDistinctTab(string s, string t) {
    int m = s.length();
    int n = t.length();
    vector<vector<double>> dp(m + 1, vector<vector<double>::value_type>(n + 1, 0));
    
    for (int i = 0; i <= m; ++i) {
        dp[i][0] = 1;
    }
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s[i - 1] == t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    return (int)dp[m][n];
}

// =============================================================================
// Local Verification for Tabulation
// =============================================================================
int main() {
    string s1 = "abcde";
    string s2 = "ace";
    
    assert(longestCommonSubsequenceTab(s1, s2) == 3);
    assert(getLCSTab(s1, s2) == "ace");
    assert(longestCommonSubstringTab("abcde", "abfce") == 2);
    assert(shortestCommonSupersequenceTab("abac", "cab") == "cabac");
    assert(longestPalindromeSubseqTab("bbbab") == 4);
    
    pair<int, int> opsTab = minOperationsTab("sea", "eat");
    assert(opsTab.first == 1 && opsTab.second == 1);
    
    assert(minInsertionsTab("mbadm") == 2);
    assert(longestRepeatingSubseqTab("aabebcdd") == 3);
    assert(minDistanceTab("horse", "ros") == 3);
    assert(numDistinctTab("rabbbit", "rabbit") == 3);
    
    cout << "All LCS Tabulation tests passed successfully!" << endl;
    return 0;
}
