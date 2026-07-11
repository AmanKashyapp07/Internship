/**
 * CSES 1690 - Hamiltonian Flights
 *
 * Problem:
 * Count the number of Hamiltonian paths from city 1 to city n
 * in a directed graph.
 *
 * Every city must be visited exactly once.
 *
 * Approach:
 * - Use Bitmask DP.
 * - Let dp[mask][u] be the number of ways to reach city u
 *   after visiting exactly the cities in mask.
 * - Start from city 1 with mask = 1.
 * - Extend every state to an unvisited neighbor.
 *
 * Time:  O((n + m) * 2^n)
 * Space: O(n * 2^n)
 */

#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n);

    while (m--) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
    }

    int fullMask = (1 << n) - 1;

    vector<vector<long long>> dp(1 << n, vector<long long>(n));

    dp[1][0] = 1;

    for (int mask = 1; mask <= fullMask; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;

            for (int v : graph[u]) {
                if (mask & (1 << v)) continue;

                int nextMask = mask | (1 << v);
                dp[nextMask][v] = (dp[nextMask][v] + dp[mask][u]) % MOD;
            }
        }
    }

    cout << dp[fullMask][n - 1] << '\n';
}