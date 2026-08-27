// Link: https://cses.fi/problemset/task/1693
#include <bits/stdc++.h>
using namespace std;

struct HierholzerDirected {
    int n, m = 0;
    vector<vector<int>> g;
    vector<int> inDeg, outDeg, path;

    HierholzerDirected(int n) : n(n), g(n + 1), inDeg(n + 1, 0), outDeg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v); outDeg[u]++; inDeg[v]++; m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back(); g[u].pop_back(); dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianPath(int start, int end) {
        for (int i = 1; i <= n; i++) {
            if (i == start && outDeg[i] != inDeg[i] + 1) return {};
            else if (i == end && inDeg[i] != outDeg[i] + 1) return {};
            else if (i != start && i != end && inDeg[i] != outDeg[i]) return {};
        }
        dfs(start);
        reverse(path.begin(), path.end());
        if ((int)path.size() != m + 1) return {};
        return path;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    HierholzerDirected H(n);
    for (int i = 0; i < m; i++) { int u, v; cin >> u >> v; H.addEdge(u, v); }

    auto ans = H.getEulerianPath(1, n);
    if (ans.empty()) cout << "IMPOSSIBLE\n";
    else {
        for (int x : ans) cout << x << ' ';
        cout << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find a directed Eulerian path starting at node 1 and ending at node n using all edges (CSES 1693).
// - Approach: Directed Eulerian Path via Hierholzer's Algorithm.
// - Intuition: Valid path requires $\text{out}[1] = \text{in}[1] + 1$, $\text{in}[n] = \text{out}[n] + 1$, and equal in/out degrees for all intermediate nodes.
// - Complexity: Time: O(V + E), Space: O(V + E).