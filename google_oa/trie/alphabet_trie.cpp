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
// ------------------------------------------------------------
// Alphabet Trie
//
// insert()
// search()
// startsWith()
// countWordsEqualTo()
// countWordsStartingWith()
// erase()
//
// Time: O(L)
// ------------------------------------------------------------

struct Node {
    Node* child[26] = {};
    bool end = false;
    int endCnt = 0; // stores the count of words that end at this node
    int preCnt = 0; // stores the count of words that pass through this node (prefix count)
};

class Trie {
public:
    Node* root = new Node();
    // --------------------------------------------------------
    // Insert
    // --------------------------------------------------------
    void insert(string word) {
        Node* cur = root;

        for (char c : word) {
            int i = c - 'a';

            if (!cur->child[i])
                cur->child[i] = new Node();

            cur = cur->child[i];
            cur->preCnt++;
        }

        cur->end = true;
        cur->endCnt++;
    }

    // --------------------------------------------------------
    // Search
    // --------------------------------------------------------
    bool search(string word) {
        Node* cur = root;

        for (char c : word) {
            int i = c - 'a';

            if (!cur->child[i])
                return false;

            cur = cur->child[i];
        }

        return cur->end;
    }

    // --------------------------------------------------------
    // Prefix Exists
    // --------------------------------------------------------
    bool startsWith(string prefix) {
        Node* cur = root;

        for (char c : prefix) {
            int i = c - 'a';

            if (!cur->child[i])
                return false;

            cur = cur->child[i];
        }

        return true;
    }

    // --------------------------------------------------------
    // Exact Count
    // --------------------------------------------------------
    int countWordsEqualTo(string word) {
        Node* cur = root;

        for (char c : word) {
            int i = c - 'a';

            if (!cur->child[i])
                return 0;

            cur = cur->child[i];
        }

        return cur->endCnt;
    }

    // --------------------------------------------------------
    // Prefix Count
    // --------------------------------------------------------
    int countWordsStartingWith(string prefix) {
        Node* cur = root;

        for (char c : prefix) {
            int i = c - 'a';

            if (!cur->child[i])
                return 0;

            cur = cur->child[i];
        }

        return cur->preCnt;
    }

    
};

// ------------------------------------------------------------
// Count Distinct Substrings
//
// Every newly created trie node represents one new substring.
// Answer = new nodes + empty string.
// ------------------------------------------------------------
int countDistinctSubstrings(string s) {

    Node* root = new Node();
    int cnt = 0;

    for (int i = 0; i < s.size(); i++) {

        Node* cur = root;

        for (int j = i; j < s.size(); j++) {

            int k = s[j] - 'a';

            if (!cur->child[k]) {
                cur->child[k] = new Node();
                cnt++;
            }

            cur = cur->child[k];
        }
    }

    return cnt + 1;
}

// ------------------------------------------------------------
// Complete String
//
// Every prefix of word must exist as a complete word.
// ------------------------------------------------------------
bool isCompleteString(string word, Node* root) {

    Node* cur = root;

    for (char c : word) {

        int i = c - 'a';

        if (!cur->child[i])
            return false;

        cur = cur->child[i];

        if (!cur->end)
            return false;
    }

    return true;
}

// ------------------------------------------------------------
// Longest Complete String
// ------------------------------------------------------------
string longestWord(vector<string>& words) {

    Trie trie;

    for (auto &w : words)
        trie.insert(w);

    string ans = "";

    for (auto &w : words) {

        if (!isCompleteString(w, trie.root))
            continue;

        if (w.size() > ans.size() ||
           (w.size() == ans.size() && w < ans))
            ans = w;
    }

    return ans;
}