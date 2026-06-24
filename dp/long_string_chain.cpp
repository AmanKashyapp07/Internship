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
        // Sort words by length to process smaller words first
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.length() < b.length();
        });
        unordered_map<string, int> dp;
        int ans = 1;
        // DP state: dp[w] stores the longest chain ending at word w
        for (const string& w : words) {
            dp[w] = 1;
            for (size_t i = 0; i < w.length(); i++) {
                string prev = w.substr(0, i) + w.substr(i + 1); // Delete i-th character
                if (dp.count(prev)) dp[w] = max(dp[w], dp[prev] + 1); // Transition
            }
            ans = max(ans, dp[w]);
        }
        return ans;
    }
};