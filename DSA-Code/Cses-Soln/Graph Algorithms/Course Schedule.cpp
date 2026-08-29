// Link: https://cses.fi/problemset/task/1679
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

    queue<int> q;
    for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);

    vector<int> topo;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        for (int v : g[u]) if (--inDegree[v] == 0) q.push(v);
    }

    if ((int)topo.size() != n) { cout << "IMPOSSIBLE\n"; return 0; }
    for (int x : topo) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find a valid topological ordering of courses given prerequisite dependencies (CSES 1679).
// - Approach: Kahn's Algorithm (BFS Topological Sort with In-Degrees).
// - Intuition: Repeatedly popping 0 in-degree nodes processes valid courses; if processed count $< N$, a cycle exists.
// - Complexity: Time: O(V + E), Space: O(V + E).