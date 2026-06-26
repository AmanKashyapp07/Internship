/**
 * LeetCode 132 - Palindrome Partitioning II (Tabulation)
 *
 * Description:
 * Given a string s, partition s such that every substring of the partition is a palindrome.
 * Return the minimum cuts needed for a palindrome partitioning of s.
 *
 * Approach:
 * - Dynamic Programming with Bottom-Up Tabulation.
 * - Precompute the `isPalindrome` 2D table.
 * - Let `dp[i]` be the minimum cuts to partition the prefix `s[0...i]`.
 * - Transition: If `s[j...i]` is a palindrome, `dp[i] = min(dp[i], 1 + dp[j - 1])`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();

    vector<vector<bool>> isPalindrome(n, vector<bool>(n, false));

    for (int i = 0; i < n; i++) {
        isPalindrome[i][i] = true;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;

            if (len == 2) {
                isPalindrome[i][j] = (s[i] == s[j]);
            } else {
                isPalindrome[i][j] =
                    (s[i] == s[j]) && isPalindrome[i + 1][j - 1];
            }
        }
    }

    vector<int> dp(n, INT_MAX);

    for (int i = 0; i < n; i++) {

        if (isPalindrome[0][i]) {
            dp[i] = 0;
            continue;
        }

        for (int j = 1; j <= i; j++) {
            if (isPalindrome[j][i]) {
                dp[i] = min(dp[i], 1 + dp[j - 1]);
            }
        }
    }

    cout << dp[n - 1] << '\n';

    return 0;
}