/**
 * CSES 1673 - High Score
 *
 * Description:
 * You are playing a game with n rooms and m tunnels. Each tunnel has a score modifier.
 * Find the maximum score you can get from room 1 to room n, detecting positive-weight cycles
 * that can reach the destination and be reached from the source.
 *
 * Approach:
 * - Negate the edge weights and run the Bellman-Ford algorithm to find the minimum cost (which corresponds to maximum score).
 * - Relax edges n times.
 * - Run a reachability check (e.g. via DFS/BFS) to verify if any nodes updated during the n-th iteration lie on a path from 1 to n.
 * - If yes, the maximum score is infinite (represented as -1).
 * we are finding bellmann ford for longest path in a directed graph with positive weights. We negate the weights and find shortest path using bellmann ford. If any node is updated in the nth iteration and it can reach the destination, then we have a positive cycle and answer is -1.
 * Time Complexity: O(V * E)
 * Space Complexity: O(V + E)
 */
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Edge { int u, v; long long w; };

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<Edge> edges; vector<vector<int>> rev(n + 1);
    for (int i = 0, a, b; i < m; i++) {
        long long w; cin >> a >> b >> w;
        edges.push_back({a, b, -w}); // Negate weights to find longest path using Bellman-Ford
        rev[b].push_back(a);
    }
    // BFS on reverse graph to find all nodes that can reach destination n
    vector<bool> reach(n + 1, false); queue<int> q;
    q.push(n); reach[n] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : rev[u]) if (!reach[v]) { reach[v] = true; q.push(v); }
    }
    const long long INF = 1e18;
    vector<long long> dist(n + 1, INF); dist[1] = 0;
    int x = -1;
    for (int i = 1; i <= n; i++) {
        x = -1;
        for (auto &[u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (reach[v]) x = v; // Update node if it can reach n
            }
        }
    }
    // If a node that can reach n gets updated on the N-th iteration, an infinite loop is detected
    if (x != -1 && dist[x] != INF) cout << -1 << '\n';
    else cout << -dist[n] << '\n';
}