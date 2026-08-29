// Link: https://cses.fi/problemset/task/1197
#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v; long long w; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) cin >> edges[i].u >> edges[i].v >> edges[i].w;

    vector<long long> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    int x = -1;

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

    if (x == -1) { cout << "NO\n"; return 0; }

    cout << "YES\n";
    for (int i = 0; i < n; i++) x = parent[x];
    vector<int> cycle;
    int start = x;
    cycle.push_back(start);
    for (int cur = parent[start]; cur != start; cur = parent[cur]) cycle.push_back(cur);
    cycle.push_back(start);
    reverse(cycle.begin(), cycle.end());

    for (int node : cycle) cout << node << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find any negative weight cycle in a directed weighted graph (CSES 1197).
// - Approach: Bellman-Ford Algorithm with Parent Pointers (N iterations).
// - Intuition: If distances continue relaxing in iteration N, a negative cycle exists; back-tracking parent pointers N times puts us inside cycle.
// - Complexity: Time: O(V \cdot E), Space: O(V + E).