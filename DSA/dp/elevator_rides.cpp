/**
 * CSES 1653 - Elevator Rides
 *
 * Description:
 * There are n people who want to get to the top of a building using an elevator with weight limit x.
 * Find the minimum number of elevator rides.
 *
 * Approach:
 * - Bitmask Dynamic Programming with Tabulation.
 * - Let `dp[mask]` be a pair `{rides, last_ride_weight}` representing the optimal state when subset `mask` of people have been moved.
 * - For each mask and each person `i` not in mask, update `dp[mask | (1 << i)]` by trying to place person `i` in the current ride or starting a new ride.
 *
 * Time Complexity: O(2^n * n)
 * Space Complexity: O(2^n)
 */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

int main() {
    int n, x;
    cin >> n >> x;

    vector<int> w(n);
    for (int i = 0; i < n; i++)
        cin >> w[i];

    int N = 1 << n;

    // dp[mask] = {minimum rides, weight in last ride}
    vector<pair<int,int>> dp(N, {n + 1, 0}); // dp stores (number of rides, weight in last ride)

    // No person selected
    dp[0] = {1, 0};

    for (int mask = 0; mask < N; mask++) {

        auto [rides, lastWeight] = dp[mask];

        for (int i = 0; i < n; i++) {

            // Person already selected
            if (mask & (1 << i))
                continue;

            int newMask = mask | (1 << i); // Add person i to the current mask

            pair<int,int> cur; // cur is the new state after adding person i

            // Put person i in current ride, because it doesn't exceed the weight limit, In cur, it wasn't updated yet, because cur represents the new state after adding person i, so we need to check if adding person i to the current ride exceeds the weight limit
            if (lastWeight + w[i] <= x) {
                cur = {rides, lastWeight + w[i]};
            }
            // Start a new ride, because adding person i exceeds the weight limit
            else {
                cur = {rides + 1, w[i]};
            }

            dp[newMask] = min(dp[newMask], cur);
        }
    }

    cout << dp[N - 1].first << '\n';
}