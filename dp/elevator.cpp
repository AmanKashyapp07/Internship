/**
 * CSES 1653 - Elevator Rides
 *
 * Description:
 * There are n people who want to get to the top of a building using an elevator with weight limit x.
 * Find the minimum number of elevator rides.
 *
 * Approach:
 * - Bitmask Dynamic Programming.
 * - Let `dp[mask]` be a pair `{rides, last_ride_weight}` representing the optimal state when subset `mask` of people have been moved.
 * - Transition: For each person `i` not in `mask`:
 *   - If `last_ride_weight + weight[i] <= x`, we can add them to the last ride: `{rides, last_ride_weight + weight[i]}`.
 *   - Else, we start a new ride: `{rides + 1, weight[i]}`.
 *   - Take the lexicographical minimum of the pairs.
 *
 * Time Complexity: O(2^n * n)
 * Space Complexity: O(2^n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

int n, x;
vector<int> w;
vector<pair<int, int>> dp; // dp[mask] = {rides, lastWeight}

pair<int, int> solve(int mask) {
    if (mask == 0) return {1, 0};
    if (dp[mask].first != -1) return dp[mask];
    pair<int, int> best = {2e9, 2e9};
    // Bitmask DP: try adding each person in the mask to the optimal configuration
    for (int i = 0; i < n; i++) {
        if (mask & (1 << i)) {
            auto prev = solve(mask ^ (1 << i));
            pair<int, int> cur = (prev.second + w[i] <= x) ? make_pair(prev.first, prev.second + w[i]) : make_pair(prev.first + 1, w[i]);
            best = min(best, cur);
        }
    }
    return dp[mask] = best;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    if (cin >> n >> x) {
        w.resize(n);
        for (int i = 0; i < n; i++) cin >> w[i];
        dp.assign(1 << n, {-1, -1});
        cout << solve((1 << n) - 1).first << '\n';
    }
}