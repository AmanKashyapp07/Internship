/**
 * CSES 1681 - Game Routes
 *
 * Description:
 * Count the number of ways to play the game from room 1 to room n in a Directed Acyclic Graph (DAG).
 * Return the result modulo 10^9 + 7.
 *
 * Approach:
 * - Find a topological order of the DAG.
 * - Maintain a DP array `dp[u]` representing the number of paths from 1 to u.
 * - Initialize `dp[1] = 1`.
 * - Traverse nodes in topological order, propagating values to neighbors: `dp[v] = (dp[v] + dp[u]) % MOD`.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1); vector<int> in(n + 1, 0), ways(n + 1, 0);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); in[v]++; }
    queue<int> q; ways[1] = 1; // DP base case: 1 way to start at node 1
    for (int i = 1; i <= n; i++) if (in[i] == 0) q.push(i);
    // DP on DAG: process nodes in topological order via Kahn's algorithm
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            ways[v] = (ways[v] + ways[u]) % MOD; // DP state transition: ways[v] = sum(ways[u])
            if (--in[v] == 0) q.push(v);
        }
    }
    cout << ways[n] << "\n";
}