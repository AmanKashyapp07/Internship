/**
 * CSES 1673 - High Score
 *
 * Problem:
 * Find the maximum score from city 1 to city n.
 * If the score can be made arbitrarily large using a positive cycle
 * on some valid path, print -1.
 *
 * Approach:
 * - Negate all edge weights.
 * - Run Bellman-Ford to find shortest paths.
 * - Run one more relaxation to find nodes affected by negative cycles.
 * - Propagate the effect of those cycles.
 * - If city n is affected, answer is -1.
 * - Otherwise, the answer is -dist[n].
 *
 * Time:  O(V * E)
 * Space: O(V + E)
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

using ll = long long;

struct Edge {
    int u, v, w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges;
    vector<ll> dist(n + 1, LLONG_MAX);
    vector<bool> affected(n + 1);

    dist[1] = 0;

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        edges.push_back({u, v, -w});
    }

    for (int i = 1; i < n; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] == LLONG_MAX) continue;

            if (dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    // Propagate the effect of negative cycles.
    // If a vertex can still be relaxed, it is on or reachable from a negative cycle.
    // Repeating the relaxation n times marks every vertex reachable from such a cycle.
    for (int i = 1; i <= n; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] == LLONG_MAX) continue;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                affected[v] = true;
            }

            if (affected[u])
                affected[v] = true;
        }
    } // this loop is for checking if the negative cycle is reachable from the source and can reach the destination

    if (affected[n])
        cout << -1 << '\n';
    else
        cout << -dist[n] << '\n';
}