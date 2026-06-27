/**
 * LeetCode 127 - Word Ladder
 *
 * Description:
 * Given beginWord, endWord, and wordList, return the number of words in the shortest transformation
 * sequence from beginWord to endWord, or 0 if no such sequence exists.
 *
 * Approach:
 * - Breadth-First Search (BFS) to find the shortest path in an unweighted state-space graph.
 * - State is the word. Two words are connected if they differ by exactly one character.
 * - Use a hash set for quick word lookups.
 *
 * Time Complexity: O(N * L * 26) where N is the number of words and L is word length.
 * Space Complexity: O(N * L)
 */

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Solution {
    int numberOfSteps(string b, string e, unordered_map<string, int>& wIdx) {
        // Standard BFS for finding the shortest path in an unweighted state space graph
        queue<pair<string, int>> q; unordered_set<string> vis{b}; q.push({b, 1});
        while (!q.empty()) {
            auto [curr, steps] = q.front(); q.pop();
            if (curr == e) return steps;
            for (int i = 0; i < curr.size(); i++) {
                char orig = curr[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == orig) continue;
                    curr[i] = c; // Try all 26 possible character substitutions
                    if (wIdx.count(curr) && vis.insert(curr).second) q.push({curr, steps + 1});
                }
                curr[i] = orig;
            }
        }
        return 0;
    }
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_map<string, int> wIdx;
        for (int i = 0; i < wordList.size(); i++) wIdx[wordList[i]] = i;
        return !wIdx.count(endWord) ? 0 : numberOfSteps(beginWord, endWord, wIdx);
    }
};