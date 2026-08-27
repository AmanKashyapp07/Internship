// Link: https://cses.fi/problemset/task/1745
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> coins(n);
    int totalSum = 0;
    for (int &c : coins) { cin >> c; totalSum += c; }

    vector<bool> dp(totalSum + 1, false);
    dp[0] = true;
    for (int c : coins) {
        for (int sum = totalSum; sum >= c; sum--) {
            dp[sum] = dp[sum] || dp[sum - c];
        }
    }

    vector<int> ans;
    for (int sum = 1; sum <= totalSum; sum++) if (dp[sum]) ans.push_back(sum);

    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << (i + 1 == (int)ans.size() ? '\n' : ' ');
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all distinct possible sums that can be created using a subset of n coins (CSES 1745).
// - Approach: Subset Sum Dynamic Programming (0/1 Knapsack boolean array).
// - Intuition: Iterating capacity backwards (`sum` from `totalSum` down to `c`) computes reachable subset sums without coin re-use.
// - Complexity: Time: O(N \cdot \sum \text{coins}), Space: O(\sum \text{coins}).