#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class Solution {
public:
    vector<vector<int>> memo;
    string text1Val;
    string text2Val;
    string str1;
    string str2;
    string str;
    string pattern;

    // =============================================================================
    // 1. Longest Common Subsequence (LCS) - Length & String Reconstruction
    // =============================================================================

    int longestCommonSubsequence(string text1, string text2) {
        text1Val = text1;
        text2Val = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (text1Val[i - 1] == text2Val[j - 1]) {
                    memo[i][j] = 1 + memo[i - 1][j - 1];
                } else {
                    memo[i][j] = max(memo[i - 1][j], memo[i][j - 1]);
                }
            }
        }
        return memo[m][n];
    }

    string getLCS(string text1, string text2) {
        text1Val = text1;
        text2Val = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (text1Val[i - 1] == text2Val[j - 1]) {
                    memo[i][j] = 1 + memo[i - 1][j - 1];
                } else {
                    memo[i][j] = max(memo[i - 1][j], memo[i][j - 1]);
                }
            }
        }
        
        string lcs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (text1Val[i - 1] == text2Val[j - 1]) {
                lcs.push_back(text1Val[i - 1]);
                i--; j--;
            } else if (memo[i - 1][j] > memo[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(lcs.begin(), lcs.end());
        return lcs;
    }

    // =============================================================================
    // 2. Longest Common Substring (Contiguous Match)
    // =============================================================================

    int longestCommonSubstring(string text1, string text2) {
        text1Val = text1;
        text2Val = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, 0));
        int maxLen = 0;
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (text1Val[i - 1] == text2Val[j - 1]) {
                    memo[i][j] = 1 + memo[i - 1][j - 1];
                    maxLen = max(maxLen, memo[i][j]);
                } else {
                    memo[i][j] = 0;
                }
            }
        }
        return maxLen;
    }

    // =============================================================================
    // 3. Shortest Common Supersequence (SCS) - String Reconstruction
    // =============================================================================

    string shortestCommonSupersequence(string str1_param, string str2_param) {
        str1 = str1_param;
        str2 = str2_param;
        int m = str1.length(), n = str2.length();
        memo.assign(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (str1[i - 1] == str2[j - 1]) {
                    memo[i][j] = 1 + memo[i - 1][j - 1];
                } else {
                    memo[i][j] = max(memo[i - 1][j], memo[i][j - 1]);
                }
            }
        }
        
        string scs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (str1[i - 1] == str2[j - 1]) {
                scs.push_back(str1[i - 1]);
                i--; j--;
            } else if (memo[i - 1][j] > memo[i][j - 1]) {
                scs.push_back(str1[i - 1]);
                i--;
            } else {
                scs.push_back(str2[j - 1]);
                j--;
            }
        }
        
        while (i > 0) scs.push_back(str1[--i]);
        while (j > 0) scs.push_back(str2[--j]);
        
        reverse(scs.begin(), scs.end());
        return scs;
    }

    // =============================================================================
    // 4. Longest Palindromic Subsequence (LPS)
    // =============================================================================

    int longestPalindromeSubseq(string s) {
        str = s;
        pattern = s;
        reverse(pattern.begin(), pattern.end());
        return longestCommonSubsequence(str, pattern);
    }

    // =============================================================================
    // 5. Minimum Operations to Convert String A to B (Insertions & Deletions)
    // =============================================================================

    pair<int, int> minOperations(string s1, string s2) {
        int m = s1.length(), n = s2.length();
        int lcsLen = longestCommonSubsequence(s1, s2);
        return {m - lcsLen, n - lcsLen}; // {deletions, insertions}
    }

    // =============================================================================
    // 6. Minimum Insertions to Make a String Palindrome
    // =============================================================================

    int minInsertions(string s) {
        return s.length() - longestPalindromeSubseq(s);
    }

    // =============================================================================
    // 7. Longest Repeating Subsequence
    // =============================================================================

    int longestRepeatingSubsequence(string str_param) {
        str = str_param;
        int n = str.length();
        memo.assign(n + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (str[i - 1] == str[j - 1] && i != j) {
                    memo[i][j] = 1 + memo[i - 1][j - 1];
                } else {
                    memo[i][j] = max(memo[i - 1][j], memo[i][j - 1]);
                }
            }
        }
        return memo[n][n];
    }

    // =============================================================================
    // 8. Edit Distance
    // =============================================================================

    int minDistance(string word1, string word2) {
        str1 = word1;
        str2 = word2;
        int m = word1.length(), n = word2.length();
        memo.assign(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 0; i <= m; ++i) memo[i][0] = i;
        for (int j = 0; j <= n; ++j) memo[0][j] = j;
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (str1[i - 1] == str2[j - 1]) {
                    memo[i][j] = memo[i - 1][j - 1];
                } else {
                    memo[i][j] = 1 + min({
                        memo[i - 1][j],     // Delete
                        memo[i][j - 1],     // Insert
                        memo[i - 1][j - 1]  // Replace
                    });
                }
            }
        }
        return memo[m][n];
    }

    // =============================================================================
    // 9. Distinct Subsequences
    // =============================================================================

    int numDistinct(string s, string t) {
        str = s;
        pattern = t;
        int m = s.length(), n = t.length();
        vector<vector<unsigned long long>> dp(m + 1, vector<unsigned long long>(n + 1, 0));
        
        for (int i = 0; i <= m; ++i) {
            dp[i][0] = 1;
        }
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (str[i - 1] == pattern[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }
        return static_cast<int>(dp[m][n]);
    }
};

// =============================================================================
// Verification Execution Block
// =============================================================================
int main() {
    Solution solver;

    string s1 = "abcde";
    string s2 = "ace";
    
    assert(solver.longestCommonSubsequence(s1, s2) == 3);
    assert(solver.getLCS(s1, s2) == "ace");
    assert(solver.longestCommonSubstring("abcde", "abfce") == 2);
    assert(solver.shortestCommonSupersequence("abac", "cab") == "cabac");
    assert(solver.longestPalindromeSubseq("bbbab") == 4);
    
    pair<int, int> ops = solver.minOperations("sea", "eat");
    assert(ops.first == 1 && ops.second == 1);
    
    assert(solver.minInsertions("mbadm") == 2);
    assert(solver.longestRepeatingSubsequence("aabebcdd") == 3);
    assert(solver.minDistance("horse", "ros") == 3);
    assert(solver.numDistinct("rabbbit", "rabbit") == 3);
    
    cout << "All clean Tabulation tests passed successfully!" << endl;
    return 0;
}