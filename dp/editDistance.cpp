/**
 * Problem: Edit Distance (https://leetcode.com/problems/edit-distance/)
 * Find the minimum number of operations to convert word1 to word2.
 * Time: O(N * M) time, O(N * M) space.
 */
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
    vector<vector<int>> dp;
    int solve(int i, int j, string &s, string &t) {
        if (i == s.size()) return t.size() - j; // Base case: insert remaining characters
        if (j == t.size()) return s.size() - i; // Base case: delete remaining characters
        if (dp[i][j] != -1) return dp[i][j];
        if (s[i] == t[j]) return dp[i][j] = solve(i + 1, j + 1, s, t);
        return dp[i][j] = 1 + min({solve(i, j + 1, s, t), solve(i + 1, j, s, t), solve(i + 1, j + 1, s, t)}); // Insert, Delete, Replace transitions
    }
public:
    int minDistance(string word1, string word2) {
        dp.assign(word1.size(), vector<int>(word2.size(), -1));
        return solve(0, 0, word1, word2);
    }
};