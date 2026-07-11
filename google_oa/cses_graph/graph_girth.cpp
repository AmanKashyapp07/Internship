/**
 * CSES 1707 - Graph Girth
 *
 * Description:
 * Find the length of the shortest cycle (girth) in an undirected unweighted graph.
 *
 * Approach:
 * - Run Breadth-First Search (BFS) from every node.
 * - For a starting node, maintain distances from it.
 * - If we encounter an edge between two visited nodes that is not a back-edge to the parent,
 *   a cycle is found.
 * - The cycle length is dist[u] + dist[v] + 1.
 * - Keep the minimum cycle length found across all BFS traversals.
 *
 * Time Complexity: O(V * (V + E))
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int ans = INF;

    for (int s = 1; s <= n; s++) {
        vector<bool> vis(n + 1, false);
        vector<int> dist(n + 1, 0);
        vector<int> parent(n + 1, -1);

        queue<int> q;
        q.push(s);
        vis[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
                else if (parent[u] != v) { // if v is visited and not the parent of u, a cycle is found
                    ans = min(ans, dist[u] + dist[v] + 1);
                }
            }
        }
    }

    cout << (ans == INF ? -1 : ans) << '\n';
}