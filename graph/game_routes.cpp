/**
 * Problem: Game Routes (https://cses.fi/problemset/task/1681)
 * Count the number of paths from node 1 to node n in a DAG.
 * Time: O(V + E) time, O(V + E) space.
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