/**
 * CSES 1690 - Hamiltonian Flights
 *
 * Description:
 * Count the number of Hamiltonian paths from city 1 to city n in a directed graph.
 * Every city must be visited exactly once.
 *
 * Approach:
 * - Use Bitmask Dynamic Programming.
 * - Let dp[mask][u] be the number of ways to reach node u after visiting
 *   exactly the nodes in mask.
 * - Start from node 0 with mask = 1.
 * - For every state, extend the path to each unvisited neighbor.
 *
 * Time Complexity: O(n * 2^n + m * 2^n)
 * Space Complexity: O(n * 2^n)
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

    int target = (1 << n) - 1;
    vector<vector<long long>> dp(1 << n, vector<long long>(n, 0)); // dp[mask][u] = number of ways to reach node u after visiting nodes in mask

    dp[1][0] = 1;

    for (int mask = 1; mask <= target; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            for (int v : graph[u]) {
                if (mask & (1 << v)) continue;
                int nextMask = mask | (1 << v);
                dp[nextMask][v] += dp[mask][u];
                dp[nextMask][v] %= MOD;
            }
        }
    }

    cout << dp[target][n - 1] << '\n';
}