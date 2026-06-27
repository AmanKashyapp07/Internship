/**
 * CSES 1745 - Money Sums
 *
 * Description:
 * Given n coins with positive values, find all possible sums that can be formed using the coins.
 *
 * Approach:
 * - 0/1 Knapsack style dynamic programming / tabulation.
 * - Let `dp[x]` be a boolean indicating if sum `x` is possible.
 * - Initialize `dp[0] = true` and all other sums as `false`.
 * - Iterate through each coin, and update the DP table backwards (from `totalSum` down to `coin`) to prevent reusing the same coin.
 *
 * Time Complexity: O(n * sum)
 * Space Complexity: O(sum)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, totalSum = 0;
    if (!(cin >> n)) return 0;
    vector<int> coins(n);
    for (int i = 0; i < n; i++) { cin >> coins[i]; totalSum += coins[i]; }
    vector<bool> dp(totalSum + 1, false); dp[0] = true;
    // Knapsack DP: dp[s] tracks if subset sum s is reachable
    for (int coin : coins) {
        for (int sum = totalSum; sum >= coin; sum--) dp[sum] = dp[sum] || dp[sum - coin];
    }
    vector<int> ans;
    for (int sum = 1; sum <= totalSum; sum++) if (dp[sum]) ans.push_back(sum);
    cout << ans.size() << '\n';
    for (size_t i = 0; i < ans.size(); i++) cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
    cout << '\n';
}