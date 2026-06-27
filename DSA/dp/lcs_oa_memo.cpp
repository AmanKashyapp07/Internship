#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

/*
================================================================================
LCS (Longest Common Subsequence) Memoization (Top-down) Variants
================================================================================
*/

// =============================================================================
// Helper Functions for LCS Memoization
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// =============================================================================
// 1. Longest Common Subsequence (LCS) - Length & String Reconstruction
// =============================================================================

int lcsHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s1[i - 1] == s2[j - 1]) {
        return dp[i][j] = 1 + lcsHelper(i - 1, j - 1, s1, s2, dp);
    }
    return dp[i][j] = max(lcsHelper(i - 1, j, s1, s2, dp), lcsHelper(i, j - 1, s1, s2, dp));
}

int longestCommonSubsequence(string text1, string text2) {
    int m = text1.length(), n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return lcsHelper(m, n, text1, text2, dp);
}

string getLCS(string text1, string text2) {
    int m = text1.length(), n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    lcsHelper(m, n, text1, text2, dp);
    
    string lcs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs.push_back(text1[i - 1]);
            i--; j--;
        } else {
            // Safely query helper values since top-down might leave some adjacent states unvisited
            int topOp = lcsHelper(i - 1, j, text1, text2, dp);
            int leftOp = lcsHelper(i, j - 1, text1, text2, dp);
            if (topOp > leftOp) i--;
            else j--;
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

// =============================================================================
// 2. Longest Common Substring (Contiguous Match)
// =============================================================================

int substringHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp, int& maxLen) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    // Ensure all subproblems are evaluated to accurately track the global max length
    substringHelper(i - 1, j, s1, s2, dp, maxLen);
    substringHelper(i, j - 1, s1, s2, dp, maxLen);
    
    if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = 1 + substringHelper(i - 1, j - 1, s1, s2, dp, maxLen);
        maxLen = max(maxLen, dp[i][j]);
        return dp[i][j];
    }
    return dp[i][j] = 0;
}

int longestCommonSubstring(string text1, string text2) {
    int m = text1.length(), n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    int maxLen = 0;
    substringHelper(m, n, text1, text2, dp, maxLen);
    return maxLen;
}

// =============================================================================
// 3. Shortest Common Supersequence (SCS) - String Reconstruction
// =============================================================================

string shortestCommonSupersequence(string str1, string str2) {
    int m = str1.length(), n = str2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    lcsHelper(m, n, str1, str2, dp); // Leverage the LCS table framework
    
    string scs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            scs.push_back(str1[i - 1]);
            i--; j--;
        } else {
            if (lcsHelper(i - 1, j, str1, str2, dp) > lcsHelper(i, j - 1, str1, str2, dp)) {
                scs.push_back(str1[i - 1]);
                i--;
            } else {
                scs.push_back(str2[j - 1]);
                j--;
            }
        }
    }
    while (i > 0) scs.push_back(str1[--i]);
    while (j > 0) scs.push_back(str2[--j]);
    
    reverse(scs.begin(), scs.end());
    return scs;
}

// =============================================================================
// 4. Longest Repeating Subsequence
// =============================================================================

int repeatingSubseqHelper(int i, int j, const string& s, vector<vector<int>>& dp) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s[i - 1] == s[j - 1] && i != j) {
        return dp[i][j] = 1 + repeatingSubseqHelper(i - 1, j - 1, s, dp);
    }
    return dp[i][j] = max(repeatingSubseqHelper(i - 1, j, s, dp), repeatingSubseqHelper(i, j - 1, s, dp));
}

int longestRepeatingSubsequence(string str) {
    int n = str.length();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return repeatingSubseqHelper(n, n, str, dp);
}

// =============================================================================
// 5. Edit Distance
// =============================================================================

int editDistanceHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp) {
    if (i == 0) return j;
    if (j == 0) return i;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s1[i - 1] == s2[j - 1]) {
        return dp[i][j] = editDistanceHelper(i - 1, j - 1, s1, s2, dp);
    }
    
    int insertOp  = editDistanceHelper(i, j - 1, s1, s2, dp);
    int deleteOp  = editDistanceHelper(i - 1, j, s1, s2, dp);
    int replaceOp = editDistanceHelper(i - 1, j - 1, s1, s2, dp);
    
    return dp[i][j] = 1 + min({insertOp, deleteOp, replaceOp});
}

int minDistance(string word1, string word2) {
    int m = word1.length(), n = word2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return editDistanceHelper(m, n, word1, word2, dp);
}

// =============================================================================
// 6. Distinct Subsequences
// =============================================================================

int distinctSubseqHelper(int i, int j, const string& s, const string& t, vector<vector<int>>& dp) {
    if (j == 0) return 1;
    if (i == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s[i - 1] == t[j - 1]) {
        return dp[i][j] = distinctSubseqHelper(i - 1, j - 1, s, t, dp) + distinctSubseqHelper(i - 1, j, s, t, dp);
    }
    return dp[i][j] = distinctSubseqHelper(i - 1, j, s, t, dp);
}

int numDistinct(string s, string t) {
    int m = s.length(), n = t.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return distinctSubseqHelper(m, n, s, t, dp);
}



// =============================================================================
// 5. Longest Palindromic Subsequence (LPS)
// =============================================================================

int longestPalindromeSubseq(string s) {
    string t = s;
    reverse(t.begin(), t.end());
    
    int n = s.length();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return lcsHelper(n, n, s, t, dp);
}

// =============================================================================
// 6. Minimum Operations to Convert String A to B (Insertions & Deletions)
// =============================================================================

pair<int, int> minOperations(string s1, string s2) {
    int m = s1.length(), n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    
    int lcsLen = lcsHelper(m, n, s1, s2, dp);
    return {m - lcsLen, n - lcsLen}; // {deletions, insertions}
}

// =============================================================================
// 7. Minimum Insertions to Make a String Palindrome
// =============================================================================

int minInsertions(string s) {
    int n = s.length();
    string t = s;
    reverse(t.begin(), t.end());
    
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    int lpsLen = lcsHelper(n, n, s, t, dp);
    
    return n - lpsLen;
}

// =============================================================================
// 8. Longest Repeating Subsequence
// =============================================================================

int longestRepeatingSubsequence(string str) {
    int n = str.length();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return repeatingSubseqHelper(n, n, str, dp);
}

// =============================================================================
// 9. Edit Distance
// =============================================================================

int minDistance(string word1, string word2) {
    int m = word1.length(), n = word2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return editDistanceHelper(m, n, word1, word2, dp);
}

// =============================================================================
// 10. Distinct Subsequences
// =============================================================================

int numDistinct(string s, string t) {
    int m = s.length(), n = t.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return distinctSubseqHelper(m, n, s, t, dp);
}
