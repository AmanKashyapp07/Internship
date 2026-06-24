/**
 * Problem: Longest String Chain (https://leetcode.com/problems/longest-string-chain/)
 * Find the length of the longest string chain where deleting a character from a word forms the previous word.
 * Time: O(N * L^2) time where L is maximum word length, O(N * L) space.
 */
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution {
public:
    int longestStrChain(vector<string>& words) {
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size(); // Sort by length
        });
        unordered_map<string, int> dp;
        int ans = 1;
        // DP state: dp[word] stores the longest chain ending at word
        for (string& word : words) {
            dp[word] = 1;
            for (size_t i = 0; i < word.size(); i++) {
                string prev = word.substr(0, i) + word.substr(i + 1); // Delete character i
                if (dp.count(prev)) dp[word] = max(dp[word], dp[prev] + 1); // Transition
            }
            ans = max(ans, dp[word]);
        }
        return ans;
    }
};