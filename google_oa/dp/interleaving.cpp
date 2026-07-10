#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
// Problem statement is to determine if s3 is an interleaving of s1 and s2.
// interleaving means that the characters of s1 and s2 are merged in a way that maintains the order of characters from both strings. For example, if s1 = "abc" and s2 = "def", then "adbcef" is an interleaving of s1 and s2.
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int n = s1.size();
        int m = s2.size();

        if (n + m != s3.size())
            return false;

        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));

        dp[0][0] = true; // dp[i][j] means s3[0..i+j-1] is an interleaving of s1[0..i-1] and s2[0..j-1]

        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= m; j++) {
                int k = i + j - 1; // index in s3
                if (i-1 >= 0 && s1[i - 1] == s3[k])
                    dp[i][j] = dp[i][j] || dp[i - 1][j];

                if (j-1 >= 0 && s2[j - 1] == s3[k])
                    dp[i][j] = dp[i][j] || dp[i][j - 1];
            }
        }

        return dp[n][m];
    }
};