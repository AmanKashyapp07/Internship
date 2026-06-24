/**
 * Problem: Best Time to Buy and Sell Stock III (https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/)
 * Find the maximum profit from at most 2 transactions on stock prices.
 * Time: O(N) time, O(N) space.
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

using ll = long long;
const ll NEG_INF = -4e18;

ll maxProfit(int idx, int count, int hold, vector<int>& arr, vector<vector<vector<ll>>>& dp) {
    if (idx == (int)arr.size()) return 0;
    if (dp[idx][count][hold] != LLONG_MIN) return dp[idx][count][hold];
    ll ans = NEG_INF;
    if (!hold) {
        ans = max(maxProfit(idx + 1, count, 0, arr, dp), maxProfit(idx + 1, count, 1, arr, dp) - arr[idx]); // Skip vs Buy
    } else {
        ans = max(maxProfit(idx + 1, count, 1, arr, dp), (count < 2 ? maxProfit(idx + 1, count + 1, 0, arr, dp) + arr[idx] : NEG_INF)); // Skip vs Sell
    }
    return dp[idx][count][hold] = ans;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];
    vector<vector<vector<ll>>> dp(n, vector<vector<ll>>(3, vector<ll>(2, LLONG_MIN)));
    cout << maxProfit(0, 0, 0, arr, dp) << '\n';
}