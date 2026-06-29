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

class Solution {
public:
    vector<int> memo;
    vector<vector<bool>> isPal;
    string str;
    int size;

    int minCut(string s) {
        str = s;
        size = s.size();
        isPal.assign(size, vector<bool>(size, false));

        for (int i = 0; i < size; i++) {
            isPal[i][i] = true;
        }

        for (int lengthVal = 2; lengthVal <= size; lengthVal++) {
            for (int i = 0; i + lengthVal - 1 < size; i++) {
                int j = i + lengthVal - 1;
                if (lengthVal == 2) {
                    isPal[i][j] = (str[i] == str[j]);
                } else {
                    isPal[i][j] = (str[i] == str[j]) && isPal[i + 1][j - 1];
                }
            }
        }

        memo.assign(size, INT_MAX);

        for (int i = 0; i < size; i++) {
            if (isPal[0][i]) {
                memo[i] = 0;
                continue;
            }

            for (int j = 1; j <= i; j++) {
                if (isPal[j][i]) {
                    memo[i] = min(memo[i], 1 + memo[j - 1]);
                }
            }
        }

        return memo[size - 1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!(cin >> s)) return 0;

    Solution solver;
    cout << solver.minCut(s) << '\n';

    return 0;
}
