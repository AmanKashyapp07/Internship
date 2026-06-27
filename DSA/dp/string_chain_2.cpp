/**
 * LeetCode 1048 - Longest String Chain
 *
 * Description:
 * Return the length of the longest possible word chain where deleting exactly one character from wordA makes it equal to wordB.
 *
 * Approach:
 * - Sort the words by length.
 * - Dynamic programming using a hash map: `dp[word] = max(dp[word], dp[predecessor] + 1)`.
 *
 * Time Complexity: O(n * L^2) where L is max word length.
 * Space Complexity: O(n * L)
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<string> dictionary;
    unordered_map<string, int> memo;

    int longestStrChain(vector<string>& words) {
        dictionary = words;
        sort(dictionary.begin(), dictionary.end(), [](const string& a, const string& b) {
            return a.size() < b.size(); // Sort by length
        });
        memo.clear();
        int ans = 1;
        // DP state: dp[word] stores the longest chain ending at word
        for (string& word : dictionary) {
            memo[word] = 1;
            for (size_t i = 0; i < word.size(); i++) {
                string prev = word.substr(0, i) + word.substr(i + 1); // Delete character i
                if (memo.count(prev)) memo[word] = max(memo[word], memo[prev] + 1); // Transition
            }
            ans = max(ans, memo[word]);
        }
        return ans;
    }
};