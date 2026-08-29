// Link: https://cses.fi/problemset/task/1678
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g;
vector<int> color, parent, cycle;

bool dfs(int u) {
    color[u] = 1;
    for (int v : g[u]) {
        if (color[v] == 0) {
            parent[v] = u;
            if (dfs(v)) return true;
        } else if (color[v] == 1) {
            cycle.push_back(v);
            for (int cur = u; cur != v; cur = parent[cur]) cycle.push_back(cur);
            cycle.push_back(v);
            reverse(cycle.begin(), cycle.end());
            return true;
        }
    }
    color[u] = 2;
    return false;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    g.resize(n + 1); color.assign(n + 1, 0); parent.assign(n + 1, -1);
    while (m--) { int u, v; cin >> u >> v; g[u].push_back(v); }

    for (int i = 1; i <= n; i++) {
        if (color[i] == 0 && dfs(i)) {
            cout << cycle.size() << '\n';
            for (int x : cycle) cout << x << ' ';
            cout << '\n';
            return 0;
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find any directed cycle in a directed graph (CSES 1678).
// - Approach: 3-Color DFS Cycle Detection (0=Unvisited, 1=Visiting, 2=Visited).
// - Intuition: A back-edge to a node with `color == 1` indicates a cycle in the active DFS recursion stack.
// - Complexity: Time: O(V + E), Space: O(V + E).