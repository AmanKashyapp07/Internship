/**
 * Problem: Longest Flight Route (https://cses.fi/problemset/task/1680)
 * Find the longest route from city 1 to city n in a directed acyclic graph.
 * Algorithm: Topological sort + DP on DAG.
 * Time: O(V + E) time, O(V + E) space.
 */
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1); vector<int> inDegree(n + 1, 0), answer(n + 1, 0), parent(n + 1, -1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); inDegree[v]++; }
    queue<int> q;
    for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);
    answer[1] = 1;
    // DP on DAG: topo-sort via Kahn's algorithm
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (answer[u] > 0 && answer[v] < answer[u] + 1) { // DP transition: answer[v] = max(answer[v], answer[u] + 1)
                answer[v] = answer[u] + 1; parent[v] = u;
            }
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    if (answer[n] == 0) { cout << "IMPOSSIBLE\n"; return 0; }
    vector<int> result;
    // Reconstruct the longest path by backtracking from n via parent pointers
    for (int cur = n; cur != -1; cur = parent[cur]) result.push_back(cur);
    reverse(result.begin(), result.end());
    cout << answer[n] << '\n';
    for (int x : result) cout << x << " ";
    cout << '\n';
}
