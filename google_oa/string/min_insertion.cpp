/**
 * Minimum Insertions to Make a String Palindrome
 * Using LCS (0-indexed DP)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define all(x) (x).begin(), (x).end()

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1;
    cin >> s1;

    string s2 = s1;
    reverse(all(s2));

    int n = s1.size();

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (s1[i] == s2[j]) {
                dp[i][j] = 1;
                if (i > 0 && j > 0)
                    dp[i][j] += dp[i - 1][j - 1];
            } else {
                int up = (i > 0) ? dp[i - 1][j] : 0;
                int left = (j > 0) ? dp[i][j - 1] : 0;
                dp[i][j] = max(up, left);
            }
        }
    }

    int lps_length = dp[n - 1][n - 1];
    int min_insertions = n - lps_length;

    cout << min_insertions << '\n';

    return 0;
}