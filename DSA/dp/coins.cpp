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
    vector<bool> dp_;
    vector<int> coins_;
    int n_;
    int totalSum_;

    vector<int> getMoneySums(int n, vector<int>& coins) {
        n_ = n;
        coins_ = coins;
        totalSum_ = 0;
        for (int c : coins) {
            totalSum_ += c;
        }

        dp_.assign(totalSum_ + 1, false);
        dp_[0] = true;

        // Knapsack DP: dp[s] tracks if subset sum s is reachable
        for (int coin : coins_) {
            for (int sum = totalSum_; sum >= coin; sum--) {
                dp_[sum] = dp_[sum] || dp_[sum - coin];
            }
        }

        vector<int> ans;
        for (int sum = 1; sum <= totalSum_; sum++) {
            if (dp_[sum]) {
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