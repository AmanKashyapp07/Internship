/**
 * LeetCode 127 - Word Ladder
 *
 * Description:
 * Given two words, beginWord and endWord, and a dictionary wordList, return the number of words
 * in the shortest transformation sequence from beginWord to endWord.
 * Only one letter can be changed at a time, and each transformed word must exist in the word list.
 *
 * Approach:
 * - Breadth-First Search (BFS) and Bidirectional BFS.
 * - Treat words as nodes and single-letter transformations as edges.
 * - Bidirectional BFS starts queue searches from both `beginWord` and `endWord` simultaneously, matching when their levels meet.
 *
 * Time Complexity: O(N * M^2) where N is number of words in wordList, M is length of each word.
 * Space Complexity: O(N)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>

using namespace std;

int numberOfSteps(string beginWord, string endWord, vector<string> &wordList, map<string, int> &wordIndex) {
    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    set<string> visited;
    visited.insert(beginWord);

    while (!q.empty()) {
        auto [currentWord, steps] = q.front();
        q.pop();

        if (currentWord == endWord) {
            return steps;
        }

        for (int i = 0; i < (int)currentWord.size(); i++) {
            char originalChar = currentWord[i];
            for (char c = 'a'; c <= 'z'; c++) {
                if (c == originalChar) continue;
                currentWord[i] = c;
                if (wordIndex.find(currentWord) != wordIndex.end() && visited.find(currentWord) == visited.end()) {
                    visited.insert(currentWord);
                    q.push({currentWord, steps + 1});
                }
            }
            currentWord[i] = originalChar;
        }
    }
    return 0;
}

int numberOfStepsBothSidesBFS(string beginWord, string endWord, vector<string>& wordList) {
    map<string, int> wordIndex;
    for (int i = 0; i < (int)wordList.size(); i++) {
        wordIndex[wordList[i]] = i;
    }

    unordered_set<string> beginSet, endSet, visited;
    beginSet.insert(beginWord);
    endSet.insert(endWord);
    int steps = 1;

    while (!beginSet.empty() && !endSet.empty()) {
        if (beginSet.size() > endSet.size()) {
            swap(beginSet, endSet); // always expand the smaller set
        }

        unordered_set<string> nextLevel;
        for (const string& word : beginSet) {
            string currentWord = word;
            for (int i = 0; i < (int)currentWord.size(); i++) {
                char originalChar = currentWord[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == originalChar) continue;
                    currentWord[i] = c;
                    if (endSet.find(currentWord) != endSet.end()) {
                        return steps + 1;
                    }
                    if (wordIndex.find(currentWord) != wordIndex.end() && visited.find(currentWord) == visited.end()) {
                        visited.insert(currentWord);
                        nextLevel.insert(currentWord);
                    }
                    currentWord[i] = originalChar;
                }
            }
        }
        beginSet = nextLevel;
        steps++;
    }
    return 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string beginWord, endWord;
    if (!(cin >> beginWord >> endWord)) return 0;

    int n;
    if (!(cin >> n)) return 0;

    vector<string> wordList;
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        wordList.push_back(word);
    }

    map<string, int> wordIndex;
    for (int i = 0; i < (int)wordList.size(); i++) {
        wordIndex[wordList[i]] = i;
    }

    if (wordIndex.find(endWord) == wordIndex.end()) {
        cout << 0 << "\n";
    } else {
        cout << numberOfSteps(beginWord, endWord, wordList, wordIndex) << "\n";
    }

    return 0;
}
