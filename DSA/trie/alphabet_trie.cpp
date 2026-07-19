#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    Node *c[26] = {};
    int endCnt = 0, preCnt = 0;
};

struct StringTrie {
    Node *root = new Node();

    void insert(const string& s, int d = 1) {
        Node *cur = root;
        for (char ch : s) {
            int i = ch - 'a';
            if (!cur->c[i]) cur->c[i] = new Node();
            cur = cur->c[i]; 
            cur->preCnt += d;
        }
        cur->endCnt += d;
    }
    void remove(const string& s) { insert(s, -1); }

private:
    Node* find(const string& s) {
        Node *cur = root;
        for (char ch : s) if (!(cur = cur->c[ch - 'a'])) return nullptr;
        return cur;
    }

public:
    bool search(const string& s) { Node *n = find(s); return n && n->endCnt > 0; }
    bool startsWith(const string& s) { Node *n = find(s); return n && n->preCnt > 0; }
    int countEqual(const string& s) { Node *n = find(s); return n ? n->endCnt : 0; }
    int countPrefix(const string& s) { Node *n = find(s); return n ? n->preCnt : 0; }
};

// Returns total distinct substrings of string s (including empty string)
int countDistinctSubstrings(const string& s) {
    Node* root = new Node();
    int cnt = 0;
    for (int i = 0; i < s.size(); i++) {
        Node* cur = root;
        for (int j = i; j < s.size(); j++) {
            int k = s[j] - 'a';
            if (!cur->c[k]) {
                cur->c[k] = new Node();
                cnt++;
            }
            cur = cur->c[k];
        }
    }
    return cnt + 1;
}

// Checks if every prefix of word is a complete word in the trie
bool isCompleteString(const string& word, Node* root) {
    Node* cur = root;
    for (char c : word) {
        int i = c - 'a';
        if (!cur->c[i]) return false;
        cur = cur->c[i];
        if (cur->endCnt <= 0) return false;
    }
    return true;
}

// Finds the longest complete string (lexicographically smallest on tie)
string longestCompleteWord(const vector<string>& words) {
    StringTrie trie;
    for (const auto &w : words) trie.insert(w);
    string ans = "";
    for (const auto &w : words) {
        if (!isCompleteString(w, trie.root)) continue;
        if (w.size() > ans.size() || (w.size() == ans.size() && w < ans)) {
            ans = w;
        }
    }
    return ans;
}