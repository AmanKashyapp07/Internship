// Link: https://cses.fi/problemset/task/1691
#include <bits/stdc++.h>
using namespace std;

struct HierholzerUndirected {
    int n, m = 0;
    vector<vector<pair<int, int>>> g;
    vector<bool> used;
    vector<int> deg, path;

    HierholzerUndirected(int n) : n(n), g(n + 1), deg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back({v, m}); g[v].push_back({u, m});
        used.push_back(false); deg[u]++; deg[v]++; m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, id] = g[u].back(); g[u].pop_back();
            if (used[id]) continue;
            used[id] = true;
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getCircuit(int start) {
        for (int i = 1; i <= n; i++) if (deg[i] & 1) return {};
        dfs(start);
        if ((int)path.size() != m + 1) return {};
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    HierholzerUndirected h(n);
    for (int i = 0; i < m; i++) { int u, v; cin >> u >> v; h.addEdge(u, v); }

    vector<int> circuit = h.getCircuit(1);
    if (circuit.empty()) cout << "IMPOSSIBLE\n";
    else {
        for (int u : circuit) cout << u << " ";
        cout << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find an Eulerian circuit starting and ending at node 1 in an undirected graph (CSES 1691).
// - Approach: Hierholzer's Algorithm on Undirected Graph.
// - Intuition: An Eulerian circuit exists iff all vertices have even degree and all edges belong to the connected component.
// - Complexity: Time: O(V + E), Space: O(V + E).