#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * ============================================================================
 * DESIGN 11: TRIE (Prefix Tree)
 * ============================================================================
 * 
 * A Trie is an efficient tree-like data structure used for storing and
 * retrieving strings, especially useful for autocomplete and prefix matching.
 * 
 * Key Operations:
 * 1. insert(word)     : Inserts a word into the trie in O(L) time.
 * 2. search(word)     : Returns true if the exact word is present in O(L) time.
 * 3. startsWith(prefix): Returns true if any word begins with prefix in O(L) time.
 * 4. countPrefix(pfx) : Returns number of words with given prefix.
 * 5. erase(word)      : Removes word and prunes dead branching nodes.
 * 
 * Time Complexities (where L is the string length):
 * - insert()    : O(L)
 * - search()    : O(L)
 * - startsWith(): O(L)
 * - erase()     : O(L)
 * Space Complexity: O(N * L * AlphabetSize)
 */

class Trie {
private:
    struct TrieNode {
        TrieNode* children[26];
        bool isEndOfWord;
        int wordCount;   // Number of times this exact word was inserted
        int prefixCount; // Number of words sharing this prefix path

        TrieNode() : isEndOfWord(false), wordCount(0), prefixCount(0) {
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
        }
    };

    TrieNode* root;

    // Helper: Check if node has no active children (used for pruning)
    bool hasNoChildren(TrieNode* node) const {
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) return false;
        }
        return true;
    }

    // Helper: Recursive deletion and bottom-up memory pruning: O(L)
    bool deleteHelper(TrieNode* curr, const string& word, size_t depth) {
        if (curr == nullptr) return false;

        // Base case: Reached the end of the word
        if (depth == word.length()) {
            if (!curr->isEndOfWord) return false; // Word did not exist

            curr->wordCount--;
            curr->prefixCount--;
            if (curr->wordCount == 0) {
                curr->isEndOfWord = false;
            }

            // If node has no children, signal parent to delete it
            return hasNoChildren(curr);
        }

        int idx = word[depth] - 'a';
        if (curr->children[idx] == nullptr) return false; // Path does not exist

        bool shouldDeleteChild = deleteHelper(curr->children[idx], word, depth + 1);

        if (shouldDeleteChild) {
            delete curr->children[idx];
            curr->children[idx] = nullptr;
        }

        curr->prefixCount--;

        // Delete current node if it is not end of another word and has no other children
        return (!curr->isEndOfWord && hasNoChildren(curr));
    }

    // Helper: Clean up entire trie memory
    void destroyTrie(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                destroyTrie(node->children[i]);
            }
        }
        delete node;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        destroyTrie(root);
    }

    // Insert a word into the trie: O(L)
    void insert(const string& word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (curr->children[idx] == nullptr) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
            curr->prefixCount++;
        }
        curr->isEndOfWord = true;
        curr->wordCount++;
    }

    // Search for exact word: O(L)
    bool search(const string& word) const {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (curr->children[idx] == nullptr) {
                return false;
            }
            curr = curr->children[idx];
        }
        return curr != nullptr && curr->isEndOfWord;
    }

    // Check if any word starts with given prefix: O(L)
    bool startsWith(const string& prefix) const {
        TrieNode* curr = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (curr->children[idx] == nullptr) {
                return false;
            }
            curr = curr->children[idx];
        }
        return curr != nullptr;
    }

    // Count how many words are equal to word: O(L)
    int countWordsEqualTo(const string& word) const {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (curr->children[idx] == nullptr) {
                return 0;
            }
            curr = curr->children[idx];
        }
        return curr->wordCount;
    }

    // Count how many words start with prefix: O(L)
    int countWordsStartingWith(const string& prefix) const {
        TrieNode* curr = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (curr->children[idx] == nullptr) {
                return 0;
            }
            curr = curr->children[idx];
        }
        return curr->prefixCount;
    }

    // Delete word with memory pruning: O(L)
    bool erase(const string& word) {
        if (!search(word)) return false;
        deleteHelper(root, word, 0);
        return true;
    }
};
