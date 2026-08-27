// Link: https://cses.fi/problemset/task/1634
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x; cin >> n >> x;
    vector<int> coins(n);
    for (int &c : coins) cin >> c;

    vector<int> dp(x + 1, INF);
    dp[0] = 0;
    for (int sum = 1; sum <= x; sum++) {
        for (int c : coins) {
            if (sum >= c) dp[sum] = min(dp[sum], 1 + dp[sum - c]);
        }
    }
    cout << (dp[x] == INF ? -1 : dp[x]) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum coins required to construct sum x (CSES 1634).
// - Approach: Unbounded Knapsack Dynamic Programming (`dp[sum] = min(dp[sum], 1 + dp[sum - c])`).
// - Intuition: Trying all coins at each target sum $1 \dots x$ computes minimum coin count.
// - Complexity: Time: O(N \cdot X), Space: O(X).