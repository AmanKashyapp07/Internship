// Link: https://cses.fi/problemset/task/1680
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    vector<int> inDegree(n + 1, 0);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); inDegree[v]++;
    }

    vector<int> dist(n + 1, -1), parent(n + 1, -1);
    dist[1] = 1;
    queue<int> q;
    for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[u] != -1 && dist[u] + 1 > dist[v]) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
            }
            if (--inDegree[v] == 0) q.push(v);
        }
    }

    if (dist[n] == -1) { cout << "IMPOSSIBLE\n"; return 0; }

    vector<int> path;
    for (int cur = n; cur != -1; cur = parent[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());

    cout << dist[n] << '\n';
    for (int x : path) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find longest path from city 1 to city n in a DAG (CSES 1680).
// - Approach: Topological Sort DP + Parent Pointer Reconstruction.
// - Intuition: Processing nodes in topological order relaxes path lengths `dist[v] = max(dist[v], dist[u] + 1)` in linear time.
// - Complexity: Time: O(V + E), Space: O(V + E).
