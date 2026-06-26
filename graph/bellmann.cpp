/**
 * CSES 1197 - Cycle Finding
 *
 * Description:
 * You are given a directed graph. Your task is to find if there exists a negative cycle
 * in the graph, and if there is, print one such cycle.
 *
 * Approach:
 * - Use the Bellman-Ford algorithm to find shortest paths and detect negative cycles.
 * - Relax all edges n times.
 * - If a distance update occurs during the n-th relaxation, a negative cycle exists.
 * - Trace back using the parent pointers of the updated node to reconstruct the negative cycle.
 *
 * Time Complexity: O(V * E)
 * Space Complexity: O(V)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge { int u, v; long long w; };

void bellman_ford(int n, vector<Edge>& edges) {
    vector<long long> dist(n + 1, 0); vector<int> parent(n + 1, -1);
    int x = -1;
    // Bellman-Ford relaxation: run N times. Any update on N-th iteration indicates a negative cycle
    for (int i = 1; i <= n; i++) {
        x = -1;
        for (const auto& e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                x = e.v;
            }
        }
    }
    if (x == -1) { cout << "NO\n"; return; }
    cout << "YES\n";
    // Trace back N times from any relaxed node to guarantee getting inside the negative cycle
    for (int i = 0; i < n; i++) x = parent[x];
    vector<int> cycle;
    int start = x;
    cycle.push_back(start);
    for (x = parent[start]; x != start; x = parent[x]) cycle.push_back(x);
    cycle.push_back(start);
    reverse(cycle.begin(), cycle.end()); // Reconstruct the negative cycle
    for (int node : cycle) cout << node << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) cin >> edges[i].u >> edges[i].v >> edges[i].w;
    bellman_ford(n, edges);
    return 0;
}