#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct BinaryTrie {
    struct Node {
        Node *c[2] = {};
        int cnt = 0;
    };

    Node *root = new Node();

    void insert(int x, int d = 1) {
        Node *cur = root;
        for (int b = 30; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (!cur->c[i]) cur->c[i] = new Node();
            cur = cur->c[i]; 
            cur->cnt += d;
        }
    }
    void remove(int x) { insert(x, -1); }

    int maxXor(int x) {
        Node *cur = root; 
        int ans = 0;
        for (int b = 30; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (cur->c[i ^ 1] && cur->c[i ^ 1]->cnt > 0) {
                ans |= 1 << b; 
                cur = cur->c[i ^ 1];
            } else {
                cur = cur->c[i];
            }
        }
        return ans;
    }

    int countLessThanK(int x, int k) {
        Node *cur = root; 
        int ans = 0;
        for (int b = 30; b >= 0 && cur; b--) {
            int xb = (x >> b) & 1, kb = (k >> b) & 1;
            if (kb) {
                if (cur->c[xb]) ans += cur->c[xb]->cnt;
                cur = cur->c[xb ^ 1];
            } else {
                cur = cur->c[xb];
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