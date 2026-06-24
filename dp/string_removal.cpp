/**
 * Problem: Delete Operation for Two Strings (https://leetcode.com/problems/delete-operation-for-two-strings/)
 * Find the minimum number of deletions required to make both strings equal.
 * Time: O(N * M) time, O(N * M) space.
 */
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
    vector<vector<int>> dp;
    int solve(int i, int j, string& a, string& b) {
        if (i == a.size()) return b.size() - j; // Base case: delete remaining in b
        if (j == b.size()) return a.size() - i; // Base case: delete remaining in a
        if (dp[i][j] != -1) return dp[i][j];
        if (a[i] == b[j]) return dp[i][j] = solve(i + 1, j + 1, a, b);
        return dp[i][j] = 1 + min(solve(i + 1, j, a, b), solve(i, j + 1, a, b)); // Transition: delete from a or b
    }
public:
    int minDistance(string word1, string word2) {
        dp.assign(word1.size(), vector<int>(word2.size(), -1));
        return solve(0, 0, word1, word2);
    }
};