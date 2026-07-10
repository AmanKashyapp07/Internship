/**
 * CSES 1680 - Longest Flight Route
 *
 * Description:
 * Find the longest route from city 1 to city n in a directed acyclic graph (DAG).
 * If no route exists, print "IMPOSSIBLE".
 *
 * Approach:
 * - Perform a Topological Sort on the DAG using Kahn's algorithm or DFS.
 * - Maintain a DP array `dist[u]` representing the longest distance from city 1 to city u.
 * - Process nodes in topological order, relaxing outgoing edges: `dist[v] = max(dist[v], dist[u] + 1)`.
 * - Use a parent array to reconstruct the path from 1 to n.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1), dp(n + 1), parent(n + 1, -1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        indegree[v]++;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (indegree[i] == 0)
            q.push(i);

    dp[1] = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dp[u] && dp[v] < dp[u] + 1) {
                dp[v] = dp[u] + 1;
                parent[v] = u;
            }

            if (--indegree[v] == 0)
                q.push(v);
        }
    }

    if (!dp[n]) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    vector<int> path;
    for (int v = n; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());

    cout << dp[n] << '\n';
    for (int v : path)
        cout << v << ' ';
    cout << '\n';
}