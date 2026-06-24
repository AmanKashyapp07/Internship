/**
 * Problem: Tree Distances I (https://cses.fi/problemset/task/1132)
 * For each node in a tree, find the maximum distance to any other node.
 * Trick is to find the two endpoints of the tree diameter using two BFS traversals, then compute distances from both endpoints.
 * Time: O(V) time, O(V) space.
 */
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// Returns {farthest_node, dist_array} using BFS
pair<int, vector<int>> bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size(); 
    vector<int> dist(n, -1); 
    queue<int> q;
    dist[start] = 0; q.push(start);
    int farthest = start;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1; q.push(v);
                if (dist[v] > dist[farthest]) farthest = v;
            }
        }
    }
    return {farthest, dist};
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    vector<vector<int>> adj(n);
    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v; --u; --v; adj[u].push_back(v); adj[v].push_back(u);
    }
    // Diameter endpoints trick: find endpoints u and v of the tree diameter
    auto [u, _] = bfs(0, adj);
    auto [v, dist1] = bfs(u, adj); // dist1 is a vector which holds distances from u to all other nodes
    auto [__, dist2] = bfs(v, adj); // dist2 is a vector which holds distances from v to all other nodes
    for (int i = 0; i < n; i++) cout << max(dist1[i], dist2[i]) << ' '; // trick : the maximum distance from any node to any other node is the maximum of its distances to the two endpoints of the diameter
    cout << '\n';
}
