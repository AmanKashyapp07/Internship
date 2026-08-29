// Link: https://cses.fi/problemset/task/1653
#include <bits/stdc++.h>
using namespace std;

int n, x;
vector<int> w;
vector<pair<int, int>> dp;

pair<int, int> solve(int mask) {
    if (!mask) return {1, 0};
    if (dp[mask].first != -1) return dp[mask];

    pair<int, int> best = {n + 1, 0};
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) continue;
        auto prev = solve(mask ^ (1 << i));
        best = min(best, prev.second + w[i] <= x
            ? pair(prev.first, prev.second + w[i])
            : pair(prev.first + 1, w[i]));
    }
    return dp[mask] = best;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> x;
    w.resize(n);
    for (int &i : w) cin >> i;
    dp.assign(1 << n, {-1, -1});

    cout << solve((1 << n) - 1).first << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum elevator rides to transport n people of weights $w_i$ with capacity x (CSES 1653).
// - Approach: Bitmask Dynamic Programming (`dp[mask] = {rides, current_ride_weight}`).
// - Intuition: Optimizing pair state `{rides, current_weight}` lexicographically over subset bitmasks yields global optimum in $O(2^N \cdot N)$.
// - Complexity: Time: O(2^N \cdot N), Space: O(2^N).