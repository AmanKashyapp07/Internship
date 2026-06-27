/**
 * LeetCode 712 - Minimum ASCII Delete Sum for Two Strings
 *
 * Description:
 * Given two strings s1 and s2, return the lowest ASCII sum of deleted characters to make two strings equal.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Instead of minimizing deletions directly, maximize the ASCII sum of the Longest Common Subsequence (LCS).
 * - Let `solve(i, j)` return the maximum ASCII sum of the LCS of `s1[i...]` and `s2[j...]`.
 * - Transition:
 *   - If `s1[i] == s2[j]`, keep the character: `s1[i] + solve(i + 1, j + 1)`.
 *   - Else, skip one character from either string: `max(solve(i + 1, j), solve(i, j + 1))`.
 * - Answer: `(total ASCII sum of s1 + s2) - 2 * keep`.
 *
 * Time Complexity: O(m * n) where m = s1.size(), n = s2.size().
 * Space Complexity: O(m * n)
 */

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> memo;
    string str1, str2;

    int solve(int i, int j) {
        if (i == str1.size() || j == str2.size())
            return 0;

        if (memo[i][j] != -1)
            return memo[i][j];

        if (str1[i] == str2[j])
            return memo[i][j] = str1[i] + solve(i + 1, j + 1);

        return memo[i][j] = max(solve(i + 1, j),
                              solve(i, j + 1));
    }

    int minimumDeleteSum(string s1, string s2) {
        str1 = s1;
        str2 = s2;
        int total = 0;
        for (char c : s1) total += c;
        for (char c : s2) total += c;

        memo.assign(s1.size(), vector<int>(s2.size(), -1));

        int keep = solve(0, 0);
        return total - 2 * keep;
    }
};