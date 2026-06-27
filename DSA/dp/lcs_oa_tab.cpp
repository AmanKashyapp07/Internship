#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// =============================================================================
// 1. Longest Common Subsequence (LCS) - Length & String Reconstruction
// =============================================================================

int longestCommonSubsequence(string text1, string text2) {
  int m = text1.length(), n = text2.length();
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

string getLCS(string text1, string text2) {
  int m = text1.length(), n = text2.length();
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

// =============================================================================
// 2. Longest Common Substring (Contiguous Match)
// =============================================================================

int longestCommonSubstring(string text1, string text2) {
  int m = text1.length(), n = text2.length();
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

// =============================================================================
// 3. Shortest Common Supersequence (SCS) - String Reconstruction
// =============================================================================

string shortestCommonSupersequence(string str1, string str2) {
  int m = str1.length(), n = str2.length();
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

  while (i > 0)
    scs.push_back(str1[--i]);
  while (j > 0)
    scs.push_back(str2[--j]);

  reverse(scs.begin(), scs.end());
  return scs;
}

// =============================================================================
// 4. Longest Palindromic Subsequence (LPS)
// =============================================================================

int longestPalindromeSubseq(string s) {
  string t = s;
  reverse(t.begin(), t.end());
  return longestCommonSubsequence(s, t);
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

int minInsertions(string s) { return s.length() - longestPalindromeSubseq(s); }

// =============================================================================
// 7. Longest Repeating Subsequence
// =============================================================================

int longestRepeatingSubsequence(string str) {
  int n = str.length();
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (str[i - 1] == str[j - 1] && i != j) {
        dp[i][j] = 1 + dp[i - 1][j - 1];
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  return dp[n][n];
}

// =============================================================================
// 8. Edit Distance
// =============================================================================

int minDistance(string word1, string word2) {
  int m = word1.length(), n = word2.length();
  vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

  for (int i = 0; i <= m; ++i)
    dp[i][0] = i;
  for (int j = 0; j <= n; ++j)
    dp[0][j] = j;

  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (word1[i - 1] == word2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + min({
                           dp[i - 1][j],    // Delete
                           dp[i][j - 1],    // Insert
                           dp[i - 1][j - 1] // Replace
                       });
      }
    }
  }
  return dp[m][n];
}

// =============================================================================
// 9. Distinct Subsequences
// =============================================================================

int numDistinct(string s, string t) {
  int m = s.length(), n = t.length();
  // Using unsigned long long to elegantly prevent runtime overflow profiles on
  // LeetCode tests
  vector<vector<unsigned long long>> dp(m + 1,
                                        vector<unsigned long long>(n + 1, 0));

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
  return static_cast<int>(dp[m][n]);
}

// =============================================================================
// Verification Execution Block
// =============================================================================
int main() {
  string s1 = "abcde";
  string s2 = "ace";

  assert(longestCommonSubsequence(s1, s2) == 3);
  assert(getLCS(s1, s2) == "ace");
  assert(longestCommonSubstring("abcde", "abfce") == 2);
  assert(shortestCommonSupersequence("abac", "cab") == "cabac");
  assert(longestPalindromeSubseq("bbbab") == 4);

  pair<int, int> ops = minOperations("sea", "eat");
  assert(ops.first == 1 && ops.second == 1);

  assert(minInsertions("mbadm") == 2);
  assert(longestRepeatingSubsequence("aabebcdd") == 3);
  assert(minDistance("horse", "ros") == 3);
  assert(numDistinct("rabbbit", "rabbit") == 3);

  cout << "All clean Tabulation tests passed successfully!" << endl;
  return 0;
}