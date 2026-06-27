/**
 * LeetCode 132 - Palindrome Partitioning II (Memoization)
 *
 * Description:
 * Given a string s, partition s such that every substring of the partition is a palindrome.
 * Return the minimum cuts needed for a palindrome partitioning of s.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - First precompute a 2D table `isPalindrome[i][j]` representing whether `s[i...j]` is a palindrome.
 * - Let `solve(i)` return the minimum cuts for suffix `s[0...i]`.
 * - Transition: Try matching any palindrome ending at `i`: `1 + solve(j - 1)` for all `j <= i` where `s[j...i]` is a palindrome.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

int solve(int i, vector<int>& memo, vector<vector<bool>>& isPal) {
    if (i < 0) return 0;
    if (memo[i] != -1) return memo[i];
    if (isPal[0][i]) return memo[i] = 0;

    int ans = i; // worst case
    for (int j = i; j >= 0; j--) {
        if (isPal[j][i]) {
            ans = min(ans, 1 + solve(j - 1, memo, isPal));
        }
    }

    return memo[i] = ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

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
                isPalindrome[i][j] = (s[i] == s[j]) && isPalindrome[i + 1][j - 1];
            }
        }
    }

    vector<int> memo(n, -1);
    cout << solve(n - 1, memo, isPalindrome) << '\n';

    return 0;
}
