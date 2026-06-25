#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int MOD = 1e9 + 7;
    int m, n;

    vector<vector<int>> dp;
    vector<vector<int>> grid;

    vector<pair<int,int>> dirs = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    int dfs(int i, int j) {
        if (dp[i][j] != -1) return dp[i][j];

        long long res = 1; // path consisting of only this cell

        for (auto &d : dirs) {
            int ni = i + d.first;
            int nj = j + d.second;

            if (ni >= 0 && ni < m && nj >= 0 && nj < n &&
                grid[ni][nj] > grid[i][j]) {
                res = (res + dfs(ni, nj)) % MOD;
            }
        }

        return dp[i][j] = res;
    }

    int countPaths(vector<vector<int>>& input) {
        grid = input;
        m = grid.size();
        n = grid[0].size();

        dp.assign(m, vector<int>(n, -1));

        long long ans = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                ans = (ans + dfs(i, j)) % MOD;
            }
        }

        return ans;
    }
};