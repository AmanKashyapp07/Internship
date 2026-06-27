/**
 * LeetCode 1048 - Longest String Chain
 *
 * Description:
 * Given an array of words, return the length of the longest possible word chain
 * where deleting exactly one character from wordA makes it equal to wordB.
 *
 * Approach:
 * - Sort the words by their length.
 * - Maintain a hash map `dp` where `dp[word]` stores the longest chain ending
 * with `word`.
 * - For each word, generate all possible predecessor strings by deleting one
 * character at a time.
 * - Update `dp[word] = max(dp[word], dp[pred] + 1)`.
 *
 * Time Complexity: O(n * L^2) where L is the maximum word length.
 * Space Complexity: O(n * L)
 */

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution {
public:
    vector<string> dictionary;
    unordered_map<string, int> memo;

    int longestStrChain(vector<string> &words) {
        dictionary = words;
        // Sort words by length to process smaller words first
        sort(dictionary.begin(), dictionary.end(), [](const string &a, const string &b) {
            return a.length() < b.length();
        });
        memo.clear();
        int ans = 1;
        // DP state: memo[w] stores the longest chain ending at word w
        for (const string &w : dictionary) {
            memo[w] = 1;
            for (size_t i = 0; i < w.length(); i++) {
                string prev = w.substr(0, i) + w.substr(i + 1); // Delete i-th character
                if (memo.count(prev))
                    memo[w] = max(memo[w], memo[prev] + 1); // Transition
            }
            ans = max(ans, memo[w]);
        }
        return ans;
    }
};