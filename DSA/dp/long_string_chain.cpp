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
    bool comparator(const string &a, const string &b) {
        return a.length() < b.length();
    }
    int longestStrChain(vector<string> &words) {
        dictionary = words;
        sort(dictionary.begin(), dictionary.end(), comparator);
        memo.clear();
        int ans = 1;
        // DP state: memo[w] stores the longest chain ending at word w
        for (const string &w : dictionary) {
            memo[w] = 1;
            int n = w.length();
            for(int i=0;i<n;i++){
                string before = w.substr(0, i) + w.substr(i + 1); // string from [0,i-1] + string from [i+1,n-1]
                if (memo.find(before) != memo.end()) {
                   memo[w] = max(memo[w], memo[before] + 1);
                }
            }
            ans = max(ans, memo[w]);
        }
        return ans;
    }
};