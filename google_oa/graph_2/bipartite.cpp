/**
 * CSES 1668 - Building Teams
 *
 * Approach:
 * - Run DFS on every connected component.
 * - Color adjacent nodes with opposite colors.
 * - If two adjacent nodes get the same color, the graph is not bipartite.
 *
 * Time: O(V + E)
 * Space: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;


vector<vector<int>> adj;
vector<int> color;

bool dfs(int u, int c) {
    color[u] = c;

    for (int v : adj[u]) {
        if (!color[v]) {
            if (!dfs(v, 3 - c))
                return false;
        }
        else if (color[v] == c)
            return false;
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    adj.assign(n + 1, {});
    color.assign(n + 1, 0);

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        if (color[i]) continue;
        if (!dfs(i, 1)) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    for (int i = 1; i <= n; i++)
        cout << color[i] << ' ';

    cout << '\n';
}