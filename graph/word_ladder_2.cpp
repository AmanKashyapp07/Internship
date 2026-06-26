/**
 * LeetCode 126 - Word Ladder II
 *
 * Description:
 * Find all shortest transformation sequences from beginWord to endWord.
 *
 * Approach:
 * - Perform BFS from `beginWord` to build a graph of shortest paths and compute levels.
 * - Use Backtracking/DFS starting from `endWord` or `beginWord` to trace all paths that strictly follow the levels.
 *
 * Time Complexity: O(N * L * 26 + Paths)
 * Space Complexity: O(N * L + Paths)
 */

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Solution {
    void buildPaths(string w, string& b, unordered_map<string, vector<string>>& par, vector<string>& path, vector<vector<string>>& ans) {
        path.push_back(w);
        if (w == b) ans.push_back({path.rbegin(), path.rend()}); // DFS backtracking: path reconstruction
        else for (string& p : par[w]) buildPaths(p, b, par, path, ans);
        path.pop_back();
    }
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end()), visited;
        vector<vector<string>> ans;
        if (!dict.count(endWord)) return ans;
        unordered_map<string, vector<string>> parent;
        queue<string> q; q.push(beginWord);
        bool found = false;
        // BFS level-by-level to construct parent map for shortest paths
        while (!q.empty() && !found) {
            int sz = q.size(); visited.clear();
            while (sz--) {
                string curr = q.front(); q.pop(); string next = curr;
                for (size_t i = 0; i < curr.size(); i++) {
                    char orig = next[i];
                    for (char ch = 'a'; ch <= 'z'; ch++) {
                        if (ch == orig) continue;
                        next[i] = ch;
                        if (dict.count(next)) {
                            parent[next].push_back(curr);
                            if (visited.insert(next).second) q.push(next);
                            if (next == endWord) found = true;
                        }
                    }
                    next[i] = orig;
                }
            }
            for (const string& w : visited) dict.erase(w); // Prevent cycles by deleting visited nodes
        }
        if (found) { vector<string> path; buildPaths(endWord, beginWord, parent, path, ans); }
        return ans;
    }
};