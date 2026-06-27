#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

/*
================================================================================
LCS (Longest Common Subsequence) OA & Interview Variants
This file contains LeetCode-style class implementations for the most popular LCS variants.
================================================================================
*/

// =============================================================================
// 1. Classic LCS - Length only (LeetCode 1143)
// Time: O(M * N), Space: O(min(M, N)) space-optimized
// =============================================================================
class ClassicLCS {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.length();
        int n = text2.length();
        if (m < n) return longestCommonSubsequence(text2, text1); // Keep second string shorter for O(min(M,N)) space
        
        vector<int> prev(n + 1, 0), curr(n + 1, 0);
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (text1[i - 1] == text2[j - 1]) {
                    curr[j] = 1 + prev[j - 1];
                } else {
                    curr[j] = max(prev[j], curr[j - 1]);
                }
            }
            prev = curr;
        }
        return prev[n];
    }
};

// =============================================================================
// 2. Print LCS - Reconstructing the Subsequence String
// Time: O(M * N), Space: O(M * N)
// =============================================================================
class PrintLCS {
public:
    string getLCS(string text1, string text2) {
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
        
        // Backtrack to reconstruct the LCS string
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
};

// =============================================================================
// 3. Longest Common Substring (Contiguous match)
// Time: O(M * N), Space: O(N) space-optimized
// =============================================================================
class LongestCommonSubstring {
public:
    int longestCommonSubstring(string text1, string text2) {
        int m = text1.length();
        int n = text2.length();
        vector<int> prev(n + 1, 0), curr(n + 1, 0);
        int maxLen = 0;
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (text1[i - 1] == text2[j - 1]) {
                    curr[j] = 1 + prev[j - 1];
                    maxLen = max(maxLen, curr[j]);
                } else {
                    curr[j] = 0; // Reset if characters don't match (must be contiguous)
                }
            }
            prev = curr;
        }
        return maxLen;
    }
};

// =============================================================================
// 4. Shortest Common Supersequence (SCS) - Print String (LeetCode 1092)
// Time: O(M * N), Space: O(M * N)
// =============================================================================
class ShortestCommonSupersequence {
public:
    string shortestCommonSupersequence(string str1, string str2) {
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
        
        // Reconstruct SCS by building path backwards
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
        
        // Append remaining characters if one string finishes early
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
};

// =============================================================================
// 5. Longest Palindromic Subsequence (LPS) (LeetCode 516)
// Time: O(N^2), Space: O(N) space-optimized
// =============================================================================
class LongestPalindromicSubsequence {
public:
    int longestPalindromeSubseq(string s) {
        string t = s;
        reverse(t.begin(), t.end()); // LPS is simply LCS(s, reverse(s))
        
        int n = s.length();
        vector<int> prev(n + 1, 0), curr(n + 1, 0);
        
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    curr[j] = 1 + prev[j - 1];
                } else {
                    curr[j] = max(prev[j], curr[j - 1]);
                }
            }
            prev = curr;
        }
        return prev[n];
    }
};

// =============================================================================
// 6. Minimum Insertions/Deletions to convert String A to B
// Time: O(M * N), Space: O(min(M, N))
// =============================================================================
class ConvertStringToAnother {
public:
    pair<int, int> minOperations(string s1, string s2) {
        int m = s1.length();
        int n = s2.length();
        
        // Use LCS
        ClassicLCS lcsSolver;
        int lcsLen = lcsSolver.longestCommonSubsequence(s1, s2);
        
        int deletions = m - lcsLen;
        int insertions = n - lcsLen;
        return {deletions, insertions};
    }
};

// =============================================================================
// 7. Minimum Insertions to make String Palindrome (LeetCode 1312)
// Time: O(N^2), Space: O(N)
// =============================================================================
class MinInsertionsForPalindrome {
public:
    int minInsertions(string s) {
        int n = s.length();
        LongestPalindromicSubsequence lpsSolver;
        int lpsLen = lpsSolver.longestPalindromeSubseq(s);
        // Minimum characters to insert is simply length minus the existing palindrome length
        return n - lpsLen;
    }
};

// =============================================================================
// 8. Longest Repeating Subsequence
// Time: O(N^2), Space: O(N)
// =============================================================================
class LongestRepeatingSubsequence {
public:
    int longestRepeatingSubseq(string s) {
        int n = s.length();
        vector<int> prev(n + 1, 0), curr(n + 1, 0);
        
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                // Same character but index must be different (i != j)
                if (s[i - 1] == s[j - 1] && i != j) {
                    curr[j] = 1 + prev[j - 1];
                } else {
                    curr[j] = max(prev[j], curr[j - 1]);
                }
            }
            prev = curr;
        }
        return prev[n];
    }
};

// =============================================================================
// 9. Edit Distance (LeetCode 72)
// Time: O(M * N), Space: O(N) space-optimized
// =============================================================================
class EditDistance {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();
        
        vector<int> prev(n + 1, 0), curr(n + 1, 0);
        
        // Base cases
        for (int j = 0; j <= n; ++j) prev[j] = j;
        
        for (int i = 1; i <= m; ++i) {
            curr[0] = i; // Base case for word2 being empty
            for (int j = 1; j <= n; ++j) {
                if (word1[i - 1] == word2[j - 1]) {
                    curr[j] = prev[j - 1]; // No operations needed
                } else {
                    curr[j] = 1 + min({
                        prev[j],    // Delete
                        curr[j - 1], // Insert
                        prev[j - 1]  // Replace
                    });
                }
            }
            prev = curr;
        }
        return prev[n];
    }
};

// =============================================================================
// 10. Distinct Subsequences (LeetCode 115)
// Time: O(M * N), Space: O(N) space-optimized
// =============================================================================
class DistinctSubsequences {
public:
    int numDistinct(string s, string t) {
        int m = s.length();
        int n = t.length();
        
        // Use double to avoid integer overflow in large testcases
        vector<double> dp(n + 1, 0);
        dp[0] = 1; // Base case: empty string target has 1 subsequence
        
        for (int i = 1; i <= m; ++i) {
            // Iterate backwards to use previous computations from the same row
            for (int j = n; j >= 1; --j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[j] = dp[j] + dp[j - 1];
                }
            }
        }
        return (int)dp[n];
    }
};

// =============================================================================
// Main function for simple local verification
// =============================================================================
int main() {
    string s1 = "abcde";
    string s2 = "ace";
    
    ClassicLCS lcsSolver;
    cout << "Classic LCS length: " << lcsSolver.longestCommonSubsequence(s1, s2) << " (Expected: 3)" << endl;
    
    PrintLCS printer;
    cout << "Printed LCS path: " << printer.getLCS(s1, s2) << " (Expected: ace)" << endl;
    
    ShortestCommonSupersequence scsSolver;
    cout << "Shortest Common Supersequence: " << scsSolver.shortestCommonSupersequence("abac", "cab") << " (Expected: cabac or similar)" << endl;
    
    EditDistance ed;
    cout << "Edit Distance (horse -> ros): " << ed.minDistance("horse", "ros") << " (Expected: 3)" << endl;
    
    return 0;
}
