/**
 * Problem: Binary Trie Demo
 * Link: N/A
 * Category: Trie
 * 
 * Description:
 * Standalone demonstration of binary trie operations.
 * 
 * Logic/Approach:
 * Inserts and searches binary numbers.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// TrieNode for binary trie demo
struct TrieNode {
    TrieNode* child[2] = {};
    int cnt = 0;
};

class BinaryTrie {
public:
    TrieNode* root = new TrieNode();

    // Inserts a number into the trie bit-by-bit
    void insert(int num) {
        TrieNode* node = root;
        for (int bit = 31; bit >= 0; bit--) {
            int b = (num >> bit) & 1;
            if (!node->child[b]) node->child[b] = new TrieNode();
            node = node->child[b];
            node->cnt++;
        }
    }

    // Counts how many elements 'y' satisfy (num XOR y) < k
    int countLessThanK(int num, int k) {
        TrieNode* node = root;
        int ans = 0;
        for (int bit = 31; bit >= 0; bit--) {
            if (!node) break;
            int nBit = (num >> bit) & 1, kBit = (k >> bit) & 1;
            if (kBit == 1) {
                // matching bit yields XOR 0, which is < 1. Add all elements in that branch.
                if (node->child[nBit]) ans += node->child[nBit]->cnt;
                node = node->child[1 - nBit];
            } else {
                node = node->child[nBit];
            }
        }
        return ans;
    }
};

int main() {
    int n, k;
    if (!(cin >> n)) return 0;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    if (!(cin >> k)) return 0;

    BinaryTrie trie;
    long long totalCount = 0;
    trie.insert(nums[0]);
    // Counts pairs (i, j) with XOR < K by checking and inserting elements on the fly
    for (int i = 1; i < n; i++) {
        totalCount += trie.countLessThanK(nums[i], k);
        trie.insert(nums[i]);
    }
    cout << totalCount << "\n";
    return 0;
}
