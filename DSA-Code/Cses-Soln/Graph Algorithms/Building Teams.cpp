// Link: https://cses.fi/problemset/task/1668
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    vector<int> color(n + 1, 0);
    for (int start = 1; start <= n; start++) {
        if (color[start]) continue;
        queue<int> q; q.push(start); color[start] = 1;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (!color[v]) { color[v] = 3 - color[u]; q.push(v); }
                else if (color[v] == color[u]) { cout << "IMPOSSIBLE\n"; return 0; }
            }
        }
    }

    for (int i = 1; i <= n; i++) cout << color[i] << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Divide n pupils into two teams such that no two friends are in the same team (CSES 1668).
// - Approach: Bipartite Graph Graph Coloring via BFS (`color[v] = 3 - color[u]`).
// - Intuition: If odd cycles exist, graph is non-bipartite ("IMPOSSIBLE"); otherwise 2-coloring yields valid teams.
// - Complexity: Time: O(V + E), Space: O(V + E).