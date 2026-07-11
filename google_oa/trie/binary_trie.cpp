
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    Node* child[2] = {};
    int cnt = 0; // stores the count of numbers that pass through this node (prefix count)
};

class BinaryTrie {
    

public:
    Node* root = new Node();
    // --------------------------------------------------------
    // Insert
    // --------------------------------------------------------
    void insert(int x) {
        Node* cur = root;

        for (int b = 31; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (!cur->child[bit])
                cur->child[bit] = new Node();

            cur = cur->child[bit];
            cur->cnt++;
        }
    }

    // --------------------------------------------------------
    // Maximum XOR
    // Greedily take opposite bit if possible.
    // --------------------------------------------------------
    int maxXor(int x) {
        Node* cur = root;
        int ans = 0;

        for (int b = 31; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;

            if (cur->child[want]) {
                ans |= (1 << b);
                cur = cur->child[want];
            } else {
                cur = cur->child[bit];
            }
        }

        return ans;
    }

    // --------------------------------------------------------
    // Count numbers y such that
    //      (x XOR y) < k
    // --------------------------------------------------------
    int countLessThanK(int x, int k) {
        Node* cur = root;
        int ans = 0;

        for (int b = 31; b >= 0; b--) {

            if (!cur) break;

            int xb = (x >> b) & 1;
            int kb = (k >> b) & 1;

            if (kb == 1) {

                // XOR bit = 0 is already smaller
                if (cur->child[xb])
                    ans += cur->child[xb]->cnt;

                // Continue with XOR bit = 1
                cur = cur->child[xb ^ 1];

            } else {
                // Must keep XOR bit = 0
                cur = cur->child[xb];
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
// just use trie.maxXor(x) to get the maximum XOR of x with any number in the trie, when looping through nums, we can find the maximum XOR of any two numbers in nums.




// Solves queries [x, m] offline finding max(x XOR nums[i]) where nums[i] <= m (LC 1707)
vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
    sort(nums.begin(), nums.end());
    int n = queries.size();
    vector<vector<int>> q(n);
    for (int i = 0; i < n; i++) q[i] = {queries[i][1], queries[i][0], i};
    sort(q.begin(), q.end()); // Sort queries by maximum constraint 'm' to process them in order
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
// same pattern, just follow prefix XOR and insert into trie, then query for maximum XOR with current prefix to find the maximum subarray XOR sum.


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
// same pattern, just follow prefix XOR and insert into trie, then query for count of subarrays whose XOR sum is less than k using the countLessThanK function.