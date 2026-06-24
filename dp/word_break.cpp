/**
 * Problem: Word Break (https://leetcode.com/problems/word-break/)
 * Determine if a string can be segmented into space-separated dictionary words.
 * Time: O(N^2) time, O(N + D) space.
 */
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

class Solution {
    vector<int> dp;
    unordered_set<string> dict;
    string s;
    int n;

    bool solve(int idx) {
        if (idx == n) return true; // Base case: end of string reached successfully
        if (dp[idx] != -1) return dp[idx];
        string cur = "";
        // Transition: try all prefixes s[idx...j]
        for (int j = idx; j < n; j++) {
            cur += s[j];
            if (dict.count(cur) && solve(j + 1)) return dp[idx] = true;
        }
        return dp[idx] = false;
    }
public:
    bool wordBreak(string str, vector<string>& wordDict) {
        s = str; n = s.size();
        for (auto &w : wordDict) dict.insert(w); // Populate lookup set
        dp.assign(n, -1);
        return solve(0);
    }
};