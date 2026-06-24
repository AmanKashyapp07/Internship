/**
 * Problem: Elevator Rides (https://cses.fi/problemset/task/1653)
 * Find the minimum number of elevator rides to move n people with weight limit x.
 * Time: O(2^N * N) time, O(2^N) space.
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