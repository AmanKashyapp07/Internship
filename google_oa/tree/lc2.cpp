#include <vector>
#include <algorithm>
using namespace std;

class Solution {
    public:
    using ll = long long;

    vector<vector<int>> adj;
    vector<int> core;
    vector<int> freq;
    ll ans = 0;
    // product of two numbers is a perfect square if and only if their square-free cores are equal. The square-free core of a number is the product of its prime factors with odd exponents.
    int getSquareFreeCore(int x) {
        int res = 1;
        for (int p = 2; p * p <= x; p++) {
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            if (cnt & 1)
                res *= p;
        }
        if (x > 1)
            res *= x;
        return res;
    }

    void dfs(int u, int par) {
        ans += freq[core[u]];
        freq[core[u]]++;

        for (int v : adj[u]) {
            if (v != par)
                dfs(v, u);
        }

        freq[core[u]]--;
    }
    long long countPerfectSquarePairs(vector<vector<int>>& edges, vector<int>& nums) {
        int n = nums.size();

        adj.assign(n, {});
        core.resize(n);

        for (auto &e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        int mx = 0;
        for (int i = 0; i < n; i++) {
            core[i] = getSquareFreeCore(nums[i]);
            mx = max(mx, core[i]);
        }

        freq.assign(mx + 1, 0);
        ans = 0;

        dfs(0, -1);

        return ans;
    }
};