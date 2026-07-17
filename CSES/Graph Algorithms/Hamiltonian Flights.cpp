#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int n, m;
vector<vector<int>> adj;
vector<vector<int>> dp;

int solve(int mask, int u) {
    if (mask == (1 << n) - 1)
        return (u == n - 1);

    int &ans = dp[mask][u];
    if (ans != -1) return ans;

    ans = 0;

    for (int v : adj[u]) {
        if (mask & (1 << v)) continue;

        // City n must be visited last
        if (v == n - 1 && mask != (1 << n) - 1 - (1 << (n - 1)))
            continue;

        ans += solve(mask | (1 << v), v);
        ans %= MOD;
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    adj.assign(n, {});

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        --a;
        --b;
        adj[a].push_back(b);
    }

    dp.assign(1 << n, vector<int>(n, -1));

    cout << solve(1, 0) << '\n';

    return 0;
}