// Link: https://cses.fi/problemset/task/1746
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    vector<vector<long long>> dp(n, vector<long long>(m + 2, 0));
    if (a[0] == 0) {
        for (int val = 1; val <= m; val++) dp[0][val] = 1;
    } else {
        dp[0][a[0]] = 1;
    }

    for (int i = 1; i < n; i++) {
        if (a[i] == 0) {
            for (int val = 1; val <= m; val++) {
                dp[i][val] = (dp[i - 1][val - 1] + dp[i - 1][val] + dp[i - 1][val + 1]) % MOD;
            }
        } else {
            int val = a[i];
            dp[i][val] = (dp[i - 1][val - 1] + dp[i - 1][val] + dp[i - 1][val + 1]) % MOD;
        }
    }

    long long ans = 0;
    for (int val = 1; val <= m; val++) ans = (ans + dp[n - 1][val]) % MOD;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count array configurations of size n with values $1 \dots m$ such that adjacent differences $|a[i] - a[i-1]| \le 1$ (CSES 1746).
// - Approach: Dynamic Programming (`dp[i][v] = dp[i-1][v-1] + dp[i-1][v] + dp[i-1][v+1]`).
// - Intuition: Transitioning index $i$ at value $v$ sums valid previous values $\{v-1, v, v+1\}$.
// - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).