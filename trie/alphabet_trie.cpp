/**
 * Problem: Alphabetical Trie
 * Link: N/A
 * Category: Trie
 * 
 * Description:
 * Alphabetical trie template.
 * 
 * Logic/Approach:
 * Node structure with a size 26 array of child pointers.
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// TrieNode represents a single character node in the Trie
struct TrieNode {
    TrieNode* child[26] = {}; // Pointers to child nodes (a-z)
    bool isEnd = false;       // True if a word ends at this node
    int cntEnd = 0;           // Number of words ending exactly at this node
    int cntPrefix = 0;        // Number of words sharing this prefix path
};

class Trie {
public:
    TrieNode* root = new TrieNode();

    // Inserts a word into the trie, updating prefix counts along the path
    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->child[idx]) node->child[idx] = new TrieNode();
            node = node->child[idx];
            node->cntPrefix++;
        }
        node->isEnd = true;
        node->cntEnd++;
    }

    // Returns true if the exact word exists in the trie
    bool search(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->child[idx]) return false;
            node = node->child[idx];
        }
        return node->isEnd;
    }

    // Returns true if there is any word starting with the given prefix
    bool startsWith(const string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!node->child[idx]) return false;
            node = node->child[idx];
        }
        return true;
    }

    // Returns how many times the exact word has been inserted
    int countWordsEqualTo(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->child[idx]) return 0;
            node = node->child[idx];
        }
        return node->cntEnd;
    }

    // Returns the total number of words sharing the given prefix
    int countWordsStartingWith(const string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!node->child[idx]) return 0;
            node = node->child[idx];
        }
        return node->cntPrefix;
    }

    // Decrements counts along the path to simulate deletion of a word
    void erase(const string& word) {
        TrieNode* node = root;
        vector<TrieNode*> path;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->child[idx]) return;
            node = node->child[idx];
            path.push_back(node);
        }
        node->cntEnd--;
        if (node->cntEnd == 0) node->isEnd = false;
        for (auto cur : path) cur->cntPrefix--;
    }

    // Returns the longest common prefix among a list of words using Trie traversal
    string longestCommonPrefix(const vector<string>& words) {
        Trie trie;
        for (auto& s : words) trie.insert(s);
        string ans = "";
        TrieNode* node = trie.root;
        while (true) {
            int cnt = 0, nxt = -1;
            for (int i = 0; i < 26; i++) {
                if (node->child[i]) { cnt++; nxt = i; }
            }
            // Stop if there is a branch (cnt != 1) or a word ends at this node
            if (cnt != 1 || node->isEnd) break;
            ans.push_back('a' + nxt);
            node = node->child[nxt];
        }
        return ans;
    }
};

int countDistinctSubstrings(string &s)
{
    TrieNode* root = new TrieNode();

    // Counts number of NEW nodes created.
    // Each new node corresponds to one
    // previously unseen distinct substring.
    int distinctCount = 0;

    int n = s.size();

    // Start every substring from index i
    for(int i=0;i<n;i++)
    {
        TrieNode* node = root;

        // Generate all substrings starting at i
        for(int j=i;j<n;j++)
        {
            int idx = s[j] - 'a';

            // First time reaching this path
            // => new distinct substring found
            if(node->child[idx] == nullptr)
            {
                node->child[idx] = new TrieNode();

                distinctCount++;
            }

            node = node->child[idx];
        }
    }

    // +1 for empty substring ""
    return distinctCount + 1;
}

bool isCompleteString(string word, TrieNode* root) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!node->child[idx]) return false;
        node = node->child[idx];
        if (!node->isEnd) return false; // Check if the prefix is a complete word
    }
    return true;
} // this checks every prefix of the word is present in the trie and is a complete word. If any prefix is not a complete word, it returns false.

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}
