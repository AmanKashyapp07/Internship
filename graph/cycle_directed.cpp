/**
 * CSES 1678 - Round Trip II
 *
 * Description:
 * Find any directed cycle in a directed graph. If a cycle exists, print the sequence of vertices.
 *
 * Approach:
 * - Depth-first search (DFS) with node coloring/states.
 * - Nodes can be: unvisited (0), visiting (1), or visited (2).
 * - If during DFS we encounter a node in the "visiting" state (a back edge), we have found a cycle.
 * - Use parent pointers to reconstruct the cycle nodes.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> graph;
vector<int> color, parent, cycle;

bool dfs(int u) {
    color[u] = 1; // 1 = visiting (currently in recursion stack)
    for (int v : graph[u]) {
        if (color[v] == 0) {
            parent[v] = u;
            if (dfs(v)) return true;
        } else if (color[v] == 1) { // Back edge detected
            cycle.push_back(v);
            for (int cur = u; cur != v; cur = parent[cur]) cycle.push_back(cur);
            cycle.push_back(v);
            reverse(cycle.begin(), cycle.end());
            return true;
        }
    }
    color[u] = 2; // 2 = visited
    return false;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    graph.resize(n + 1); color.assign(n + 1, 0); parent.assign(n + 1, -1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; graph[u].push_back(v); }
    for (int i = 1; i <= n; i++) {
        if (color[i] == 0 && dfs(i)) {
            cout << cycle.size() << '\n';
            for (int x : cycle) cout << x << " ";
            cout << '\n'; return 0;
        }
    }
    cout << "IMPOSSIBLE\n";
}