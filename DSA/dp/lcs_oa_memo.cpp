#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

/*
================================================================================
LCS (Longest Common Subsequence) Memoization (Top-down) Variants
================================================================================
*/

class Solution {
public:
    string str1;
    string str2;
    string str;
    string pattern;
    vector<vector<int>> memo;

    // =============================================================================
    // Helper Functions for LCS Memoization
    // =============================================================================

    int lcsHelper(int i, int j) {
        if (i == 0 || j == 0) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (str1[i - 1] == str2[j - 1]) {
            return memo[i][j] = 1 + lcsHelper(i - 1, j - 1);
        }
        return memo[i][j] = max(lcsHelper(i - 1, j), lcsHelper(i, j - 1));
    }

    int substringHelper(int i, int j, int& length) {
        if (i == 0 || j == 0) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        substringHelper(i - 1, j, length);
        substringHelper(i, j - 1, length);
        
        if (str1[i - 1] == str2[j - 1]) {
            memo[i][j] = 1 + substringHelper(i - 1, j - 1, length);
            length = max(length, memo[i][j]);
            return memo[i][j];
        }
        return memo[i][j] = 0;
    }

    int repeatingSubseqHelper(int i, int j) {
        if (i == 0 || j == 0) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (str[i - 1] == str[j - 1] && i != j) {
            return memo[i][j] = 1 + repeatingSubseqHelper(i - 1, j - 1);
        }
        return memo[i][j] = max(repeatingSubseqHelper(i - 1, j), repeatingSubseqHelper(i, j - 1));
    }

    int editDistanceHelper(int i, int j) {
        if (i == 0) return j;
        if (j == 0) return i;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (str1[i - 1] == str2[j - 1]) {
            return memo[i][j] = editDistanceHelper(i - 1, j - 1);
        }
        
        int insertOp  = editDistanceHelper(i, j - 1);
        int deleteOp  = editDistanceHelper(i - 1, j);
        int replaceOp = editDistanceHelper(i - 1, j - 1);
        
        return memo[i][j] = 1 + min({insertOp, deleteOp, replaceOp});
    }

    int distinctSubseqHelper(int i, int j) {
        if (j == 0) return 1;
        if (i == 0) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (str[i - 1] == pattern[j - 1]) {
            return memo[i][j] = distinctSubseqHelper(i - 1, j - 1) + distinctSubseqHelper(i - 1, j);
        }
        return memo[i][j] = distinctSubseqHelper(i - 1, j);
    }

    // =============================================================================
    // 1. Longest Common Subsequence (LCS) - Length & String Reconstruction
    // =============================================================================

    int longestCommonSubsequence(string text1, string text2) {
        str1 = text1;
        str2 = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        return lcsHelper(m, n);
    }

    string getLCS(string text1, string text2) {
        str1 = text1;
        str2 = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        lcsHelper(m, n);
        
        string lcs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (str1[i - 1] == str2[j - 1]) {
                lcs.push_back(str1[i - 1]);
                i--; j--;
            } else {
                int topOp = lcsHelper(i - 1, j);
                int leftOp = lcsHelper(i, j - 1);
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

    int longestCommonSubstring(string text1, string text2) {
        str1 = text1;
        str2 = text2;
        int m = text1.length(), n = text2.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        int length = 0;
        substringHelper(m, n, length);
        return length;
    }

    // =============================================================================
    // 3. Shortest Common Supersequence (SCS) - String Reconstruction
    // =============================================================================

    string shortestCommonSupersequence(string str1_param, string str2_param) {
        str1 = str1_param;
        str2 = str2_param;
        int m = str1_param.length(), n = str2_param.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        lcsHelper(m, n); // Leverage the LCS table framework
        
        string scs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (str1[i - 1] == str2[j - 1]) {
                scs.push_back(str1[i - 1]);
                i--; j--;
            } else {
                if (lcsHelper(i - 1, j) > lcsHelper(i, j - 1)) {
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

    int longestRepeatingSubsequence(string str_param) {
        str = str_param;
        int n = str_param.length();
        memo.assign(n + 1, vector<int>(n + 1, -1));
        return repeatingSubseqHelper(n, n);
    }

    // =============================================================================
    // 5. Edit Distance
    // =============================================================================

    int minDistance(string word1, string word2) {
        str1 = word1;
        str2 = word2;
        int m = word1.length(), n = word2.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        return editDistanceHelper(m, n);
    }

    // =============================================================================
    // 6. Distinct Subsequences
    // =============================================================================

    int numDistinct(string s, string t) {
        str = s;
        pattern = t;
        int m = s.length(), n = t.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        return distinctSubseqHelper(m, n);
    }

    // =============================================================================
    // 7. Longest Palindromic Subsequence (LPS)
    // =============================================================================

    int longestPalindromeSubseq(string s) {
        str = s;
        pattern = s;
        reverse(pattern.begin(), pattern.end());
        
        str1 = str;
        str2 = pattern;
        int n = s.length();
        memo.assign(n + 1, vector<int>(n + 1, -1));
        return lcsHelper(n, n);
    }

    // =============================================================================
    // 8. Minimum Operations to Convert String A to B (Insertions & Deletions)
    // =============================================================================

    pair<int, int> minOperations(string s1, string s2) {
        str1 = s1;
        str2 = s2;
        int m = s1.length(), n = s2.length();
        memo.assign(m + 1, vector<int>(n + 1, -1));
        
        int lcsLen = lcsHelper(m, n);
        return {m - lcsLen, n - lcsLen}; // {deletions, insertions}
    }

    // =============================================================================
    // 9. Minimum Insertions to Make a String Palindrome
    // =============================================================================

    int minInsertions(string s) {
        str = s;
        pattern = s;
        reverse(pattern.begin(), pattern.end());
        
        str1 = str;
        str2 = pattern;
        int n = s.length();
        memo.assign(n + 1, vector<int>(n + 1, -1));
        int lpsLen = lcsHelper(n, n);
        
        return n - lpsLen;
    }
};
