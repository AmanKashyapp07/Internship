
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

int numberOfSteps(string beginWord, string endWord, vector<string> &wordList, map<string, int> &wordIndex)
{
   
    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    set<string> visited;
    visited.insert(beginWord);
    while (!q.empty())
    {
        auto [currentWord, steps] = q.front();
        q.pop();
        if (currentWord == endWord)
        {
            return steps;
        }
        for (int i = 0; i < currentWord.size(); i++)
        {
            char originalChar = currentWord[i]; // Store the original character
            for (char c = 'a'; c <= 'z'; c++)
            {
                if (c == originalChar) continue;
                currentWord[i] = c; // Change the character at position i
                if (wordIndex.find(currentWord) != wordIndex.end() && visited.find(currentWord) == visited.end()) // if the new word is in the word list and not visited
                {
                    visited.insert(currentWord); // Mark the new word as visited
                    q.push({currentWord, steps + 1}); // Push the new word and increment the step count
                }
            }
            currentWord[i] = originalChar; // Restore the original character
        }
    }
    return 0;
}

int numberOfStepsBothSidesBFS(string beginWord, string endWord, vector<string>& wordList)
{
    map<string, int> wordIndex;
    for (int i = 0; i < wordList.size(); i++) 
    {
        wordIndex[wordList[i]] = i;
    }
    unordered_set<string> beginSet, endSet, visited;
    beginSet.insert(beginWord);
    endSet.insert(endWord);
    int steps = 1;
    while (!beginSet.empty() && !endSet.empty()){
        if(beginSet.size() > endSet.size()) {
            swap(beginSet, endSet);
        } // always expand the smaller set for efficiency
        unordered_set<string> nextLevel;
        for (const string& word : beginSet) {
            string currentWord = word;
            for (int i = 0; i < currentWord.size(); i++) {
                char originalChar = currentWord[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == originalChar) continue;
                    currentWord[i] = c;
                    if (endSet.find(currentWord) != endSet.end()) {
                        return steps + 1; // Found a connection, steps+1 because we are counting the current step as well
                    }
                    if (wordIndex.find(currentWord) != wordIndex.end() && visited.find(currentWord) == visited.end()) { // if the new word is in the word list and not visited
                        visited.insert(currentWord);
                        nextLevel.insert(currentWord);
                    }
                    currentWord[i] = originalChar; // Restore the original character
                }
            }
        }
        beginSet = nextLevel;
        steps++;
    }
    return 0;
} // this approach is more optimal than the previous one as it reduces the search space by expanding from both ends of the transformation sequence. Time complexity is O(N * M^2) where N is the number of words in the word list and M is the length of each word. Space complexity is O(N) for storing the visited words and the sets for BFS.    

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string beginWord, endWord;
    vector<string> wordList;
    cin >> beginWord >> endWord;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string word;
        cin >> word;
        wordList.push_back(word);
    }
    map<string, int> wordIndex;
    for (int i = 0; i < wordList.size(); i++)
    {
        wordIndex[wordList[i]] = i;
    }
    if (wordIndex.find(endWord) == wordIndex.end())
    {
        cout << 0 << "\n";
    }
    else
    {
        cout << numberOfSteps(beginWord, endWord, wordList, wordIndex) << "\n";
    }

    return 0;
}
