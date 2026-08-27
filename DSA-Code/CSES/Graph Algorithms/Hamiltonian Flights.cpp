// Link: https://cses.fi/problemset/task/1690
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--) {
        int u, v; cin >> u >> v; u--; v--; g[u].push_back(v);
    }

    vector<vector<int>> dp(1 << n, vector<int>(n, 0));
    dp[1][0] = 1;

    for (int mask = 1; mask < (1 << n); mask++) {
        if (!(mask & 1)) continue;
        if ((mask & (1 << (n - 1))) && mask != (1 << n) - 1) continue;

        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u)) || dp[mask][u] == 0) continue;
            for (int v : g[u]) {
                if (!(mask & (1 << v))) {
                    dp[mask | (1 << v)][v] = (dp[mask | (1 << v)][v] + dp[mask][u]) % MOD;
                }
            }
        }
    }
    cout << dp[(1 << n) - 1][n - 1] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count number of Hamiltonian paths from city 1 to city n visiting all cities exactly once (CSES 1690).
// - Approach: Bitmask Dynamic Programming (`dp[mask][u]`).
// - Intuition: `dp[mask][u]` stores number of paths visiting subset `mask` ending at node u; bitmask transitions run in $O(2^N \cdot N)$.
// - Complexity: Time: O(2^N \cdot N^2), Space: O(2^N \cdot N).