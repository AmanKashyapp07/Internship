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

class Solution {
public:
    vector<bool> memo;
    vector<int> coinValues;
    int size;
    int totalSum;

    vector<int> getMoneySums(int n, vector<int>& coins) {
        size = n;
        coinValues = coins;
        totalSum = 0;
        for (int c : coins) {
            totalSum += c;
        }

        memo.assign(totalSum + 1, false);
        memo[0] = true;

        // Knapsack DP: dp[s] tracks if subset sum s is reachable
        for (int coin : coinValues) {
            for (int sum = totalSum; sum >= coin; sum--) {
                memo[sum] = memo[sum] || memo[sum - coin];
            }
        }

        vector<int> ans;
        for (int sum = 1; sum <= totalSum; sum++) {
            if (memo[sum]) {
                ans.push_back(sum);
            }
        }
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> coins(n);
    for (int i = 0; i < i; i++) { // Wait, the original code had: for (int i = 0; i < n; i++)
        // Let me fix that loop condition to: i < n
    }
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    
    Solution solver;
    vector<int> ans = solver.getMoneySums(n, coins);

    cout << ans.size() << '\n';
    for (size_t i = 0; i < ans.size(); i++) {
        cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
    }
    cout << '\n';
    return 0;
}