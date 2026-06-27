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

class Solution {
public:
    int size;
    int limit;
    vector<int> input;
    vector<pair<int, int>> memo;

    int getMinElevatorRides(int n, int x, vector<int>& w) {
        size = n;
        limit = x;
        input = w;

        int N = 1 << size;
        memo.assign(N, {size + 1, 0}); // dp stores (number of rides, weight in last ride)

        // No person selected
        memo[0] = {1, 0};

        for (int mask = 0; mask < N; mask++) {
            auto [rides, lastWeight] = memo[mask];

            for (int i = 0; i < size; i++) {
                // Person already selected
                if (mask & (1 << i))
                    continue;

                int newMask = mask | (1 << i); // Add person i to the current mask
                pair<int, int> cur; // cur is the new state after adding person i

                // Put person i in current ride, because it doesn't exceed the weight limit
                if (lastWeight + input[i] <= limit) {
                    cur = {rides, lastWeight + input[i]};
                }
                // Start a new ride, because adding person i exceeds the weight limit
                else {
                    cur = {rides + 1, input[i]};
                }

                memo[newMask] = min(memo[newMask], cur);
            }
        }

        return memo[N - 1].first;
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, x;
    if (!(cin >> n >> x)) return 0;

    vector<int> w(n);
    for (int i = 0; i < n; i++)
        cin >> w[i];

    Solution solver;
    cout << solver.getMinElevatorRides(n, x, w) << '\n';
    return 0;
}