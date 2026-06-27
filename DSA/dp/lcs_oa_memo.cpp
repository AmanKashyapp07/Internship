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

int lcsMemoHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s1[i - 1] == s2[j - 1]) {
        return dp[i][j] = 1 + lcsMemoHelper(i - 1, j - 1, s1, s2, dp);
    }
    return dp[i][j] = max(lcsMemoHelper(i - 1, j, s1, s2, dp), lcsMemoHelper(i, j - 1, s1, s2, dp));
}

int substringMemoHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp, int& maxLen) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    substringMemoHelper(i - 1, j, s1, s2, dp, maxLen);
    substringMemoHelper(i, j - 1, s1, s2, dp, maxLen);
    
    if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = 1 + substringMemoHelper(i - 1, j - 1, s1, s2, dp, maxLen);
        maxLen = max(maxLen, dp[i][j]);
        return dp[i][j];
    }
    return dp[i][j] = 0;
}

int repeatingSubseqMemoHelper(int i, int j, const string& s, vector<vector<int>>& dp) {
    if (i == 0 || j == 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s[i - 1] == s[j - 1] && i != j) {
        return dp[i][j] = 1 + repeatingSubseqMemoHelper(i - 1, j - 1, s, dp);
    }
    return dp[i][j] = max(repeatingSubseqMemoHelper(i - 1, j, s, dp), repeatingSubseqMemoHelper(i, j - 1, s, dp));
}

int editDistanceMemoHelper(int i, int j, const string& s1, const string& s2, vector<vector<int>>& dp) {
    if (i == 0) return j;
    if (j == 0) return i;
    
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s1[i - 1] == s2[j - 1]) {
        return dp[i][j] = editDistanceMemoHelper(i - 1, j - 1, s1, s2, dp);
    }
    
    int insertOp = editDistanceMemoHelper(i, j - 1, s1, s2, dp);
    int deleteOp = editDistanceMemoHelper(i - 1, j, s1, s2, dp);
    int replaceOp = editDistanceMemoHelper(i - 1, j - 1, s1, s2, dp);
    
    return dp[i][j] = 1 + min({insertOp, deleteOp, replaceOp});
}

int distinctSubseqMemoHelper(int i, int j, const string& s, const string& t, vector<vector<int>>& dp) {
    if (j == 0) return 1;
    if (i == 0) return 0;
    
    if (dp[i][j] != -1) return dp[i][j];
    
    if (s[i - 1] == t[j - 1]) {
        return dp[i][j] = distinctSubseqMemoHelper(i - 1, j - 1, s, t, dp) + distinctSubseqMemoHelper(i - 1, j, s, t, dp);
    }
    return dp[i][j] = distinctSubseqMemoHelper(i - 1, j, s, t, dp);
}

// =============================================================================
// LCS Memoization Interface Functions
// =============================================================================

int longestCommonSubsequenceMemo(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return lcsMemoHelper(m, n, text1, text2, dp);
}

string getLCSMemo(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    lcsMemoHelper(m, n, text1, text2, dp);
    
    string lcs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs.push_back(text1[i - 1]);
            i--;
            j--;
        } else {
            int top = (dp[i - 1][j] != -1) ? dp[i - 1][j] : 0;
            int left = (dp[i][j - 1] != -1) ? dp[i][j - 1] : 0;
            if (top > left) {
                i--;
            } else {
                j--;
            }
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

int longestCommonSubstringMemo(string text1, string text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    int maxLen = 0;
    substringMemoHelper(m, n, text1, text2, dp, maxLen);
    return maxLen;
}

string shortestCommonSupersequenceMemo(string str1, string str2) {
    int m = str1.length();
    int n = str2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    lcsMemoHelper(m, n, str1, str2, dp);
    
    string scs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            scs.push_back(str1[i - 1]);
            i--;
            j--;
        } else {
            int top = (dp[i - 1][j] != -1) ? dp[i - 1][j] : 0;
            int left = (dp[i][j - 1] != -1) ? dp[i][j - 1] : 0;
            if (top > left) {
                scs.push_back(str1[i - 1]);
                i--;
            } else {
                scs.push_back(str2[j - 1]);
                j--;
            }
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

int longestPalindromeSubseqMemo(string s) {
    string t = s;
    reverse(t.begin(), t.end());
    return longestCommonSubsequenceMemo(s, t);
}

pair<int, int> minOperationsMemo(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    int lcsLen = longestCommonSubsequenceMemo(s1, s2);
    return {m - lcsLen, n - lcsLen};
}

int minInsertionsMemo(string s) {
    int n = s.length();
    int lpsLen = longestPalindromeSubseqMemo(s);
    return n - lpsLen;
}

int longestRepeatingSubseqMemo(string s) {
    int n = s.length();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return repeatingSubseqMemoHelper(n, n, s, dp);
}

int minDistanceMemo(string word1, string word2) {
    int m = word1.length();
    int n = word2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return editDistanceMemoHelper(m, n, word1, word2, dp);
}

int numDistinctMemo(string s, string t) {
    int m = s.length();
    int n = t.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return distinctSubseqMemoHelper(m, n, s, t, dp);
}

// =============================================================================
// Local Verification for Memoization
// =============================================================================
int main() {
    string s1 = "abcde";
    string s2 = "ace";
    
    assert(longestCommonSubsequenceMemo(s1, s2) == 3);
    assert(getLCSMemo(s1, s2) == "ace");
    assert(longestCommonSubstringMemo("abcde", "abfce") == 2);
    assert(shortestCommonSupersequenceMemo("abac", "cab") == "cabac");
    assert(longestPalindromeSubseqMemo("bbbab") == 4);
    
    pair<int, int> opsMemo = minOperationsMemo("sea", "eat");
    assert(opsMemo.first == 1 && opsMemo.second == 1);
    
    assert(minInsertionsMemo("mbadm") == 2);
    assert(longestRepeatingSubseqMemo("aabebcdd") == 3);
    assert(minDistanceMemo("horse", "ros") == 3);
    assert(numDistinctMemo("rabbbit", "rabbit") == 3);
    
    cout << "All LCS Memoization tests passed successfully!" << endl;
    return 0;
}
