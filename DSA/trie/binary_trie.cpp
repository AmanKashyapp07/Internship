/**
 * Problem: Binary Trie (XOR Trie)
 * Link: N/A
 * Category: Trie
 * 
 * Description:
 * XOR Trie template supporting maximum XOR queries.
 * 
 * Logic/Approach:
 * Node structure with a size 2 array of binary bits.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// TrieNode for Binary Trie storing bits (0 or 1)
struct TrieNode {
    TrieNode* child[2] = {}; // Left child (0), Right child (1)
    int cnt = 0;             // Number of values passing through this node
};

class BinaryTrie {
public:
    TrieNode* root = new TrieNode();

    // Inserts a number into the trie by checking its binary bits (MSB to LSB)
    void insert(int num) {
        TrieNode* node = root;
        for (int bit = 31; bit >= 0; bit--) {
            int b = (num >> bit) & 1;
            if (!node->child[b]) node->child[b] = new TrieNode();
            node = node->child[b];
            node->cnt++;
        }
    }

    // Finds the maximum XOR value possible with 'num' using greedy bit selection
    int maxXor(int num) {
        TrieNode* node = root;
        int ans = 0;
        for (int bit = 31; bit >= 0; bit--) {
            int b = (num >> bit) & 1;
            int want = 1 - b; // Oppose the current bit to maximize XOR (1 ^ 0 = 1)
            if (node->child[want]) {
                ans |= (1 << bit);
                node = node->child[want];
            } else {
                node = node->child[b];
            }
        }
        return ans;
    }

    // Counts elements 'y' in the trie such that (num XOR y) < k
    int countLessThanK(int num, int k) {
        TrieNode* node = root;
        int ans = 0;
        for (int bit = 31; bit >= 0; bit--) {
            if (!node) break;
            int nBit = (num >> bit) & 1, kBit = (k >> bit) & 1;
            if (kBit == 1) {
                // If k has 1, matching the bit of num yields 0, which is < 1. Add all such elements.
                if (node->child[nBit]) ans += node->child[nBit]->cnt;
                // Continue searching on the opposite branch (yielding XOR 1)
                node = node->child[1 - nBit];
            } else {
                // If k has 0, we must match num's bit to keep XOR result 0
                node = node->child[nBit];
            }
        }
        return ans;
    }
};

// Returns maximum XOR of any two elements in nums (LC 421)
int maximumXORPair(vector<int>& nums) {
    BinaryTrie trie;
    int ans = 0;
    for (int x : nums) {
        trie.insert(x);
        ans = max(ans, trie.maxXor(x));
    }
    return ans;
}

// Solves queries [x, m] offline finding max(x XOR nums[i]) where nums[i] <= m (LC 1707)
vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
    sort(nums.begin(), nums.end());
    int n = queries.size();
    vector<vector<int>> q(n);
    for (int i = 0; i < n; i++) q[i] = {queries[i][1], queries[i][0], i};
    sort(q.begin(), q.end());
    BinaryTrie trie;
    vector<int> ans(n);
    int idx = 0;
    for (auto& it : q) {
        // Insert all numbers that are <= maximum constraint 'm'
        while (idx < nums.size() && nums[idx] <= it[0]) trie.insert(nums[idx++]);
        ans[it[2]] = (idx == 0) ? -1 : trie.maxXor(it[1]);
    }
    return ans;
}

// Finds the maximum XOR sum of a subarray
int maximumSubarrayXOR(vector<int>& nums) {
    BinaryTrie trie;
    trie.insert(0);
    int pref = 0, ans = 0;
    for (int x : nums) {
        pref ^= x;
        ans = max(ans, trie.maxXor(pref));
        trie.insert(pref);
    }
    return ans;
}

// Counts subarrays whose XOR sum is strictly less than k
long long countSubarrayXorLessThanK(vector<int>& nums, int k) {
    BinaryTrie trie;
    trie.insert(0);
    int pref = 0;
    long long ans = 0;
    for (int x : nums) {
        pref ^= x;
        ans += trie.countLessThanK(pref, k);
        trie.insert(pref);
    }
    return ans;
}
