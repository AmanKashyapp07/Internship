/**
 * CSES - Longest Flight Route
 *
 * Problem:
 * Find the longest path from city 1 to city n in a DAG.
 * Print the path or "IMPOSSIBLE" if no path exists.
 *
 * Approach:
 * - Topological sort (Kahn's algorithm).
 * - DP:
 *      dp[v] = longest path ending at v.
 * - Store parent to reconstruct the answer.
 *
 * Time:  O(n + m)
 * Space: O(n + m)
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
    vector<int> indegree(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        indegree[v]++;
    }

    vector<int> dp(n + 1, 0);
    vector<int> parent(n + 1, -1);

    dp[1] = 1;

    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (indegree[i] == 0)
            q.push(i);

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

    if (dp[n] == 0) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    vector<int> path;
    for (int cur = n; cur != -1; cur = parent[cur])
        path.push_back(cur);

    reverse(path.begin(), path.end());

    cout << path.size() << "\n";
    for (int x : path)
        cout << x << " ";
}