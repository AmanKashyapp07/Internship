// Link: https://cses.fi/problemset/task/1667
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    vector<int> parent(n + 1, -1);
    queue<int> q; q.push(1); parent[1] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (parent[v] == -1) { parent[v] = u; q.push(v); }
        }
    }

    if (parent[n] == -1) { cout << "IMPOSSIBLE\n"; return 0; }

    vector<int> path;
    for (int cur = n; cur != 0; cur = parent[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());

    cout << path.size() << '\n';
    for (int x : path) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find shortest path in an unweighted graph from node 1 to node n (CSES 1667).
// - Approach: BFS + Parent Array Path Reconstruction.
// - Intuition: BFS visits nodes in increasing distance order; tracking parent pointers reconstructs shortest path.
// - Complexity: Time: O(V + E), Space: O(V + E).