/**
 * LeetCode 139 - Word Break
 *
 * Description:
 * Given a string s and a dictionary of strings wordDict, return true if s can be segmented
 * into a space-separated sequence of one or more dictionary words.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Let `solve(idx)` check if suffix `s[idx...]` can be segmented.
 * - Iterate `j` from `idx` to `n-1`, check if substring `s[idx...j]` exists in `wordDict` and recursively check `solve(j+1)`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n + d) where d is total size of wordDict.
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