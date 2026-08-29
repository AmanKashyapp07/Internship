#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Implement Trie I (Prefix Tree) (LC 208)     | 26-Way Character Array Trie       | O(L)     | O(N * L) |
 | 2  | Implement Trie II (Counts & Erase) (LC 1804)| Prefix & End Occurrence Counting  | O(L)     | O(N * L) |
 | 3  | Design Add & Search Words (LC 211)          | Wildcard '.' Branching Backtrack  | O(26^D)  | O(N * L) |
 | 4  | Word Search II (Boggle Grid) (LC 212)       | 2D DFS + Trie In-Place Node Pruning| O(R*C*4^L| O(W * L) |
 | 5  | Replace Words / Shortest Root (LC 648)      | Earliest Matching Root Prefix DFS | O(N * L) | O(N * L) |
 | 6  | Longest Word With All Prefixes (LC 720)     | Complete Prefix Chain Validation  | O(N * L) | O(N * L) |
 | 7  | Count Distinct Substrings (LC 1698)         | Suffix Insertion Substring Count  | O(N^2)   | O(N^2)   |
 | 8  | Search Suggestions System (LC 1268)         | Trie Node Sorted Top-3 List Cache | O(N L log3|O(N * L) |
 | 9  | Stream of Characters (LC 1032)              | Reversed Word Trie Suffix Match   | O(L)     | O(W * L) |
 | 10 | Map Sum Pairs (LC 677)                      | Trie Delta Propagation / DFS Sum  | O(L)     | O(N * L) |
 | 11 | Concatenated Words (LC 472)                 | Trie + Memoized DFS Word Splitting| O(N*L^2) | O(N * L) |
 | 12 | Word Break via Trie (LC 139)                | 1D DP + Trie Prefix Fast Slicing  | O(N^2)   | O(W * L) |
 | 13 | Palindrome Pairs (LC 336)                   | Reverse Trie + Palindrome Suffixes| O(N * L^2| O(N * L) |
 | 14 | Word Squares (LC 425)                       | Backtracking with Trie Prefix Look| O(N*26^L)| O(N * L) |
 | 15 | Extra Characters in a String (LC 2707)      | Trie + 1D Min-Deletion DP         | O(N^2)   | O(W * L) |
 ====================================================================================================
*/

// ====================================================================================================
// SECTION 1: CORE TRIE DATA STRUCTURES
// ====================================================================================================

struct TrieNode {
    TrieNode* children[26];
    int wordCount;      // Number of words ending at this node
    int prefixCount;    // Number of words having prefix ending at this node
    string fullWord;    // Stores complete string at terminal (optional optimization for LC 212)

    TrieNode() : wordCount(0), prefixCount(0), fullWord("") {
        fill(begin(children), end(children), nullptr);
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() { root = new TrieNode(); }

    // Inserts a word with delta frequency (+1 for insert, -1 for erase)
    void insert(const string& word, int delta = 1) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) {
                cur->children[idx] = new TrieNode();
            }
            cur = cur->children[idx];
            cur->prefixCount += delta;
        }
        cur->wordCount += delta;
        if (cur->wordCount > 0) cur->fullWord = word;
        else cur->fullWord = "";
    }

    // Erases one instance of a word from Trie
    void erase(const string& word) {
        if (countWordsEqualTo(word) > 0) {
            insert(word, -1);
        }
    }

    // Searches if word exists in Trie
    bool search(const string& word) const {
        TrieNode* node = findNode(word);
        return node && node->wordCount > 0;
    }

    // Searches if any word in Trie starts with given prefix
    bool startsWith(const string& prefix) const {
        TrieNode* node = findNode(prefix);
        return node && node->prefixCount > 0;
    }

    // Counts exact occurrences of word
    int countWordsEqualTo(const string& word) const {
        TrieNode* node = findNode(word);
        return node ? node->wordCount : 0;
    }

    // Counts words having given prefix
    int countWordsStartingWith(const string& prefix) const {
        TrieNode* node = findNode(prefix);
        return node ? node->prefixCount : 0;
    }

    // Longest Common Prefix across all strings inserted into Trie
    string longestCommonPrefix() const {
        string lcp = "";
        TrieNode* cur = root;
        while (cur && cur->wordCount == 0) {
            int childCount = 0;
            int nextIdx = -1;
            for (int i = 0; i < 26; i++) {
                if (cur->children[i] && cur->children[i]->prefixCount > 0) {
                    childCount++;
                    nextIdx = i;
                }
            }
            if (childCount != 1) break;
            lcp.push_back((char)('a' + nextIdx));
            cur = cur->children[nextIdx];
        }
        return lcp;
    }

private:
    TrieNode* findNode(const string& s) const {
        TrieNode* cur = root;
        for (char ch : s) {
            int idx = ch - 'a';
            if (!cur->children[idx] || cur->children[idx]->prefixCount <= 0) {
                return nullptr;
            }
            cur = cur->children[idx];
        }
        return cur;
    }
};


// ====================================================================================================
// SECTION 2: TOP INTERVIEW TRIE PROBLEMS & ALGORITHMS
// ====================================================================================================

// =========================================================
// 1. IMPLEMENT TRIE I (PREFIX TREE) (LEETCODE 208)
// =========================================================
class TrieI {
    struct Node {
        Node* links[26] = {};
        bool isEnd = false;
    } root;

public:
    void insert(const string& word) {
        Node* node = &root;
        for (char ch : word) {
            int i = ch - 'a';
            if (!node->links[i]) node->links[i] = new Node();
            node = node->links[i];
        }
        node->isEnd = true;
    }

    bool search(const string& word) {
        Node* node = &root;
        for (char ch : word) {
            int i = ch - 'a';
            if (!node->links[i]) return false;
            node = node->links[i];
        }
        return node->isEnd;
    }

    bool startsWith(const string& prefix) {
        Node* node = &root;
        for (char ch : prefix) {
            int i = ch - 'a';
            if (!node->links[i]) return false;
            node = node->links[i];
        }
        return true;
    }
};
// Interview Explanation:
// - Problem Statement: Implement basic Trie with insert, search, and startsWith operations (LeetCode 208).
// - Approach: 26-way tree node with `links[26]` and boolean flag `isEnd`.
// - Complexity: Time: O(L) per operation where L is string length, Space: O(N * L * 26) memory.


// =========================================================
// 2. IMPLEMENT TRIE II (COUNTS & ERASE) (LEETCODE 1804)
// =========================================================
class TrieII {
    struct Node {
        Node* links[26] = {};
        int countEndsWith = 0;
        int countPrefix = 0;
    } *root = new Node();

public:
    void insert(const string& word) {
        Node* node = root;
        for (char ch : word) {
            int i = ch - 'a';
            if (!node->links[i]) node->links[i] = new Node();
            node = node->links[i];
            node->countPrefix++;
        }
        node->countEndsWith++;
    }

    int countWordsEqualTo(const string& word) {
        Node* node = root;
        for (char ch : word) {
            int i = ch - 'a';
            if (!node->links[i]) return 0;
            node = node->links[i];
        }
        return node->countEndsWith;
    }

    int countWordsStartingWith(const string& prefix) {
        Node* node = root;
        for (char ch : prefix) {
            int i = ch - 'a';
            if (!node->links[i]) return 0;
            node = node->links[i];
        }
        return node->countPrefix;
    }

    void erase(const string& word) {
        Node* node = root;
        for (char ch : word) {
            int i = ch - 'a';
            node = node->links[i];
            node->countPrefix--;
        }
        node->countEndsWith--;
    }
};
// Interview Explanation:
// - Problem Statement: Implement Trie with word frequency counts, prefix frequency counts, and erase support (LeetCode 1804).
// - Approach: Maintain `countEndsWith` and `countPrefix` counters at each node.
// - Complexity: Time: O(L) for all methods, Space: O(N * L * 26).


// =========================================================
// 3. DESIGN ADD & SEARCH WORDS DATA STRUCTURE (LEETCODE 211)
// =========================================================
class WordDictionary {
    struct Node {
        Node* child[26] = {};
        bool isEnd = false;
    } *root = new Node();

    bool dfs(const string& word, int idx, Node* cur) {
        if (!cur) return false;
        if (idx == (int)word.size()) return cur->isEnd;

        char ch = word[idx];
        if (ch == '.') {
            for (int c = 0; c < 26; c++) {
                if (cur->child[c] && dfs(word, idx + 1, cur->child[c])) {
                    return true;
                }
            }
            return false;
        } else {
            int c = ch - 'a';
            return cur->child[c] && dfs(word, idx + 1, cur->child[c]);
        }
    }

public:
    void addWord(const string& word) {
        Node* cur = root;
        for (char ch : word) {
            int c = ch - 'a';
            if (!cur->child[c]) cur->child[c] = new Node();
            cur = cur->child[c];
        }
        cur->isEnd = true;
    }

    bool search(const string& word) {
        return dfs(word, 0, root);
    }
};
// Interview Explanation:
// - Problem Statement: Search words with '.' wildcard character matching any letter (LeetCode 211).
// - Approach: Backtracking DFS across all 26 non-null child branches whenever '.' is encountered.
// - Complexity: Add: O(L), Search: O(26^D) worst-case where D is number of dots, Space: O(N * L).


// =========================================================
// 4. WORD SEARCH II (BOGGLE GRID) (LEETCODE 212)
// =========================================================
class WordSearchII {
    struct TrieNodeLC {
        TrieNodeLC* children[26] = {};
        string word = "";
    };

    void buildTrie(TrieNodeLC* root, const vector<string>& words) {
        for (const string& w : words) {
            TrieNodeLC* cur = root;
            for (char ch : w) {
                int idx = ch - 'a';
                if (!cur->children[idx]) cur->children[idx] = new TrieNodeLC();
                cur = cur->children[idx];
            }
            cur->word = w; // Store complete word at leaf
        }
    }

    void dfs(vector<vector<char>>& board, int r, int c, TrieNodeLC* node, vector<string>& result) {
        char ch = board[r][c];
        if (ch == '#' || !node->children[ch - 'a']) return;

        node = node->children[ch - 'a'];
        if (!node->word.empty()) {
            result.push_back(node->word);
            node->word = ""; // Prevent duplicate matches
        }

        board[r][c] = '#'; // In-place visit marking
        static const int dr[] = {-1, 1, 0, 0};
        static const int dc[] = {0, 0, -1, 1};

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < (int)board.size() && nc >= 0 && nc < (int)board[0].size() && board[nr][nc] != '#') {
                dfs(board, nr, nc, node, result);
            }
        }
        board[r][c] = ch; // Backtrack
    }

public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNodeLC* root = new TrieNodeLC();
        buildTrie(root, words);

        vector<string> result;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                dfs(board, i, j, root, result);
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find all words from a dictionary present on a 2D character matrix (LeetCode 212).
// - Approach: Build Trie of all target words, then launch 4-directional DFS from every cell using the Trie to prune fruitless paths.
// - Optimization: Clear `node->word = ""` once found to eliminate duplicates without set overhead.
// - Complexity: Time: O(M * N * 4^L), Space: O(W * L) where W is word count, L is max word length.


// =========================================================
// 5. REPLACE WORDS / SHORTEST ROOT (LEETCODE 648)
// =========================================================
string replaceWords(vector<string>& dictionary, string sentence) {
    Trie trie;
    for (const string& rootWord : dictionary) trie.insert(rootWord);

    stringstream ss(sentence);
    string word, result = "";
    bool first = true;

    while (ss >> word) {
        if (!first) result += " ";
        first = false;

        TrieNode* cur = trie.root;
        string rootFound = "";
        bool matched = false;

        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx] || cur->children[idx]->prefixCount <= 0) break;
            cur = cur->children[idx];
            rootFound += ch;
            if (cur->wordCount > 0) {
                matched = true;
                break;
            }
        }

        result += matched ? rootFound : word;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Replace each word in sentence with shortest dictionary root prefix (LeetCode 648).
// - Approach: Insert dictionary into Trie; for each word, greedily search for the earliest terminal root node.
// - Complexity: Time: O(D * L_d + S) where S is sentence length, Space: O(D * L_d).


// =========================================================
// 6. LONGEST WORD WITH ALL PREFIXES (LEETCODE 720)
// =========================================================
string longestWord(vector<string>& words) {
    Trie trie;
    for (const string& w : words) trie.insert(w);

    string best = "";
    for (const string& w : words) {
        TrieNode* cur = trie.root;
        bool valid = true;
        for (char ch : w) {
            cur = cur->children[ch - 'a'];
            if (!cur || cur->wordCount <= 0) {
                valid = false;
                break;
            }
        }
        if (valid) {
            if (w.size() > best.size() || (w.size() == best.size() && w < best)) {
                best = w;
            }
        }
    }
    return best;
}
// Interview Explanation:
// - Problem Statement: Find longest word whose every prefix is also a complete word in the dictionary (LeetCode 720).
// - Approach: Verify each character transition satisfies `node->wordCount > 0`. Break ties lexicographically.
// - Complexity: Time: O(N * L), Space: O(N * L).


// =========================================================
// 7. COUNT DISTINCT SUBSTRINGS (LEETCODE 1698 / GFG)
// =========================================================
int countDistinctSubstrings(const string& s) {
    TrieNode* root = new TrieNode();
    int distinctCount = 0;
    int n = s.size();

    for (int i = 0; i < n; i++) {
        TrieNode* cur = root;
        for (int j = i; j < n; j++) {
            int idx = s[j] - 'a';
            if (!cur->children[idx]) {
                cur->children[idx] = new TrieNode();
                distinctCount++; // Every new node represents a unique substring
            }
            cur = cur->children[idx];
        }
    }
    return distinctCount + 1; // +1 includes empty substring ""
}
// Interview Explanation:
// - Problem Statement: Count total distinct substrings of a string including empty string (LeetCode 1698 / GFG).
// - Approach: Insert all suffixes of `s` into a Trie. Number of newly allocated nodes equals distinct non-empty substrings.
// - Complexity: Time: O(N^2), Space: O(N^2).


// =========================================================
// 8. SEARCH SUGGESTIONS SYSTEM (LEETCODE 1268)
// =========================================================
class SearchSuggestions {
    struct SNode {
        SNode* child[26] = {};
        vector<string> top3; // Cache lexicographically top 3 words
    } *root = new SNode();

    void insert(const string& word) {
        SNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->child[idx]) cur->child[idx] = new SNode();
            cur = cur->child[idx];
            if (cur->top3.size() < 3) cur->top3.push_back(word);
        }
    }

public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end()); // Pre-sort guarantees top 3 are smallest
        for (const string& p : products) insert(p);

        vector<vector<string>> ans;
        SNode* cur = root;
        bool dead = false;

        for (char ch : searchWord) {
            if (dead || !cur->child[ch - 'a']) {
                dead = true;
                ans.push_back({});
            } else {
                cur = cur->child[ch - 'a'];
                ans.push_back(cur->top3);
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Suggest up to 3 lexicographically smallest matching products after each typed letter (LeetCode 1268).
// - Approach: Sort products first, then insert into Trie caching up to 3 strings per prefix node.
// - Complexity: Time: O(N log N + N * L + M), Space: O(N * L).


// =========================================================
// 9. STREAM OF CHARACTERS (LEETCODE 1032)
// =========================================================
class StreamChecker {
    struct RNode {
        RNode* child[26] = {};
        bool isEnd = false;
    } *root = new RNode();

    deque<char> stream;
    int maxLen = 0;

    void insertReversed(const string& word) {
        RNode* cur = root;
        for (int i = (int)word.size() - 1; i >= 0; i--) {
            int idx = word[i] - 'a';
            if (!cur->child[idx]) cur->child[idx] = new RNode();
            cur = cur->child[idx];
        }
        cur->isEnd = true;
        maxLen = max(maxLen, (int)word.size());
    }

public:
    StreamChecker(vector<string>& words) {
        for (const string& w : words) insertReversed(w);
    }

    bool query(char letter) {
        stream.push_front(letter);
        if ((int)stream.size() > maxLen) stream.pop_back();

        RNode* cur = root;
        for (char ch : stream) {
            int idx = ch - 'a';
            if (!cur->child[idx]) return false;
            cur = cur->child[idx];
            if (cur->isEnd) return true;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Check if any dictionary word forms a suffix of current incoming stream of characters (LeetCode 1032).
// - Approach: Reverse words into Trie. Match incoming stream from newest character backwards.
// - Complexity: Init: O(W * L), Query: O(L) worst case, Space: O(W * L).


// =========================================================
// 10. MAP SUM PAIRS (LEETCODE 677)
// =========================================================
class MapSum {
    struct MNode {
        MNode* child[26] = {};
        int sumValue = 0;
    } *root = new MNode();

    unordered_map<string, int> keyMap;

public:
    void insert(string key, int val) {
        int delta = val - keyMap[key];
        keyMap[key] = val;

        MNode* cur = root;
        for (char ch : key) {
            int idx = ch - 'a';
            if (!cur->child[idx]) cur->child[idx] = new MNode();
            cur = cur->child[idx];
            cur->sumValue += delta;
        }
    }

    int sum(string prefix) {
        MNode* cur = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!cur->child[idx]) return 0;
            cur = cur->child[idx];
        }
        return cur->sumValue;
    }
};
// Interview Explanation:
// - Problem Statement: Insert key-value pairs and return sum of values of all keys starting with prefix (LeetCode 677).
// - Approach: Propagate delta (`newVal - oldVal`) along all prefix nodes during insertion for O(L) instant sum queries.
// - Complexity: Time: O(L) for both insert and sum, Space: O(N * L).


// =========================================================
// 11. CONCATENATED WORDS (LEETCODE 472)
// =========================================================
class ConcatenatedWords {
    Trie trie;

    bool canForm(const string& word, int start, int count, vector<int>& memo) {
        if (start == (int)word.size()) return count >= 2;
        if (memo[start] != -1) return memo[start];

        TrieNode* cur = trie.root;
        for (int i = start; i < (int)word.size(); i++) {
            int idx = word[i] - 'a';
            if (!cur->children[idx]) break;
            cur = cur->children[idx];
            if (cur->wordCount > 0) {
                if (canForm(word, i + 1, count + 1, memo)) {
                    return memo[start] = 1;
                }
            }
        }
        return memo[start] = 0;
    }

public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        for (const string& w : words) {
            if (!w.empty()) trie.insert(w);
        }

        vector<string> ans;
        for (const string& w : words) {
            if (w.empty()) continue;
            vector<int> memo(w.size(), -1);
            if (canForm(w, 0, 0, memo)) {
                ans.push_back(w);
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all words in dictionary formed by concatenation of at least two shorter words (LeetCode 472).
// - Approach: Insert dictionary into Trie; run memoized DFS word splitting on each candidate.
// - Complexity: Time: O(N * L^2), Space: O(N * L).


// =========================================================
// 12. WORD BREAK USING TRIE (LEETCODE 139)
// =========================================================
bool wordBreakTrie(string s, vector<string>& wordDict) {
    Trie trie;
    for (const string& w : wordDict) trie.insert(w);

    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 0; i < n; i++) {
        if (!dp[i]) continue;
        TrieNode* cur = trie.root;
        for (int j = i; j < n; j++) {
            int idx = s[j] - 'a';
            if (!cur->children[idx]) break;
            cur = cur->children[idx];
            if (cur->wordCount > 0) dp[j + 1] = true;
        }
    }
    return dp[n];
}
// Interview Explanation:
// - Problem Statement: Determine if string can be segmented into space-separated dictionary words (LeetCode 139).
// - Approach: 1D Dynamic Programming where valid prefix indices launch Trie traversals forward.
// - Complexity: Time: O(N^2 + W * L), Space: O(W * L + N).


// =========================================================
// 13. PALINDROME PAIRS (LEETCODE 336)
// =========================================================
class PalindromePairs {
    struct PNode {
        PNode* child[26] = {};
        int wordIndex = -1;
        vector<int> palindromePrefixIndices; // Indices of words where remaining prefix is a palindrome
    };

    bool isPalindrome(const string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }

public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        PNode* root = new PNode();
        int n = words.size();

        // Insert reversed words into Trie
        for (int i = 0; i < n; i++) {
            string w = words[i];
            PNode* cur = root;
            for (int j = (int)w.size() - 1; j >= 0; j--) {
                if (isPalindrome(w, 0, j)) {
                    cur->palindromePrefixIndices.push_back(i);
                }
                int idx = w[j] - 'a';
                if (!cur->child[idx]) cur->child[idx] = new PNode();
                cur = cur->child[idx];
            }
            cur->wordIndex = i;
            cur->palindromePrefixIndices.push_back(i); // empty prefix is palindrome
        }

        vector<vector<int>> ans;
        for (int i = 0; i < n; i++) {
            const string& w = words[i];
            PNode* cur = root;
            int len = w.size();
            for (int j = 0; j < len; j++) {
                // Case 1: Word i is longer than word in trie, and remaining suffix of word i is palindrome
                if (cur->wordIndex != -1 && cur->wordIndex != i && isPalindrome(w, j, len - 1)) {
                    ans.push_back({i, cur->wordIndex});
                }
                cur = cur->child[w[j] - 'a'];
                if (!cur) break;
            }
            if (!cur) continue;

            // Case 2: Word i is shorter/equal, check precomputed palindrome prefixes in trie
            for (int k : cur->palindromePrefixIndices) {
                if (k != i) ans.push_back({i, k});
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all unique pairs of distinct indices (i, j) such that `words[i] + words[j]` is a palindrome (LeetCode 336).
// - Approach: Insert reversed words into Trie with cached palindrome prefix indices, then search forward words.
// - Complexity: Time: O(N * L^2), Space: O(N * L^2).


// =========================================================
// 14. WORD SQUARES (LEETCODE 425)
// =========================================================
class WordSquares {
    struct WNode {
        WNode* child[26] = {};
        vector<int> wordIndices;
    } *root = new WNode();

    void insert(const string& word, int index) {
        WNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->child[idx]) cur->child[idx] = new WNode();
            cur = cur->child[idx];
            cur->wordIndices.push_back(index);
        }
    }

    vector<int> getWithPrefix(const string& prefix) {
        WNode* cur = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!cur->child[idx]) return {};
            cur = cur->child[idx];
        }
        return cur->wordIndices;
    }

    void backtrack(int step, int n, const vector<string>& words, vector<string>& currentSquare, vector<vector<string>>& ans) {
        if (step == n) {
            ans.push_back(currentSquare);
            return;
        }

        string prefix = "";
        for (int i = 0; i < step; i++) {
            prefix += currentSquare[i][step];
        }

        for (int idx : getWithPrefix(prefix)) {
            currentSquare.push_back(words[idx]);
            backtrack(step + 1, n, words, currentSquare, ans);
            currentSquare.pop_back();
        }
    }

public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        if (words.empty()) return {};
        for (int i = 0; i < (int)words.size(); i++) insert(words[i], i);

        int n = words[0].size();
        vector<vector<string>> ans;
        vector<string> currentSquare;

        for (const string& w : words) {
            currentSquare.push_back(w);
            backtrack(1, n, words, currentSquare, ans);
            currentSquare.pop_back();
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all valid word squares of size KxK (LeetCode 425).
// - Approach: Backtracking row-by-row; column requirements define exact prefix queries resolved via Trie in O(L).
// - Complexity: Time: O(N * 26^L), Space: O(N * L).


// =========================================================
// 15. EXTRA CHARACTERS IN A STRING (LEETCODE 2707)
// =========================================================
int minExtraChar(string s, vector<string>& dictionary) {
    Trie trie;
    for (const string& w : dictionary) trie.insert(w);

    int n = s.size();
    vector<int> dp(n + 1, 0);

    for (int i = n - 1; i >= 0; i--) {
        dp[i] = 1 + dp[i + 1]; // Skip current character as extra
        TrieNode* cur = trie.root;
        for (int j = i; j < n; j++) {
            int idx = s[j] - 'a';
            if (!cur->children[idx]) break;
            cur = cur->children[idx];
            if (cur->wordCount > 0) {
                dp[i] = min(dp[i], dp[j + 1]);
            }
        }
    }
    return dp[0];
}
// Interview Explanation:
// - Problem Statement: Find minimum extra characters left over after breaking string into dictionary words (LeetCode 2707).
// - Approach: Suffix 1D DP + Trie forward branch pruning.
// - Complexity: Time: O(N^2 + W * L), Space: O(W * L + N).

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}