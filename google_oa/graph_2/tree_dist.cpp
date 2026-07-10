/**
 * CSES 1132 - Tree Distances I
 *
 * Description:
 * For every node in a tree, find the maximum distance to any other node.
 *
 * Approach:
 * - Find one endpoint of the tree diameter using BFS from any node.
 * - From that endpoint, run BFS again to find the opposite endpoint and store distances (dist1).
 * - Run one more BFS from the second endpoint to get distances (dist2).
 * - For every node, the answer is max(dist1[i], dist2[i]).
 *
 * Time Complexity: O(N)
 * Space Complexity: O(N)
 */

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <iostream>
const long long P = 31;
const long long MOD = 1e9 + 7;
using namespace std;

// Returns {farthest_node, distance_array}
pair<int, vector<int>> bfs(int start, const vector<vector<int>> &adj) {

    int n = adj.size();
    vector<int> dist(n, -1);

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    int farthest = start;

    while (!q.empty()) {

        int node = q.front();
        q.pop();

        for (int next : adj[node]) {

            if (dist[next] != -1) // Already visited
                continue;

            dist[next] = dist[node] + 1;
            q.push(next);

            if (dist[next] > dist[farthest])
                farthest = next;
        }
    }

    return {farthest, dist};
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Find one endpoint of the diameter.
    auto [end1, _] = bfs(0, adj);

    // Find the other endpoint and distances from end1.
    auto [end2, dist1] = bfs(end1, adj);

    // Distances from the other endpoint.
    auto [__, dist2] = bfs(end2, adj);

    // Maximum distance to either diameter endpoint.
    for (int i = 0; i < n; i++) {
        cout << max(dist1[i], dist2[i]) << " ";
    }

    cout << "\n";
}