/**
 * CSES 1668 - Building Teams
 *
 * Description:
 * There are n pupils. Divide them into two teams such that no two friends are in the same team.
 *
 * Approach:
 * - Graph bipartiteness check using 2-coloring.
 * - Traverse the graph using BFS or DFS.
 * - Assign alternating colors (1 and 2) to adjacent nodes.
 * - If we find an edge between two nodes of the same color, it is impossible to bipartition the graph.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    vector<int> color(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        if (color[i]) continue;
        queue<int> q; q.push(i); color[i] = 1; // 2-coloring start state
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (!color[v]) {
                    color[v] = 3 - color[u]; // Toggle color between 1 and 2
                    q.push(v);
                } else if (color[v] == color[u]) { // Odd cycle detected
                    cout << "IMPOSSIBLE\n"; return 0;
                }
            }
        }
    }
    for (int i = 1; i <= n; i++) cout << color[i] << " ";
    cout << '\n';
}