// Link: https://cses.fi/problemset/task/2076
#include <bits/stdc++.h>
using namespace std;

int n, m, timer_ = 0;
vector<vector<int>> adj;
vector<int> tin, low;
vector<pair<int,int>> bridges;

void dfs(int u, int parent) {
    tin[u] = low[u] = ++timer_;
    for (int v : adj[u]) {
        if (v == parent) continue;
        if (tin[v]) { low[u] = min(low[u], tin[v]); }
        else {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back({u, v});
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> m;
    adj.resize(n + 1); tin.assign(n + 1, 0); low.assign(n + 1, 0);
    while (m--) {
        int a, b; cin >> a >> b;
        adj[a].push_back(b); adj[b].push_back(a);
    }
    dfs(1, -1);
    cout << bridges.size() << '\n';
    for (auto [u, v] : bridges) cout << u << ' ' << v << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all bridges (edges whose removal disconnects the graph) (CSES 2076).
// - Approach: Tarjan's Bridge-Finding Algorithm with DFS `tin` (discovery time) and `low` (lowest reachable ancestor).
// - Intuition: Edge $(u, v)$ is a bridge if `low[v] > tin[u]` — meaning no back edge reaches $u$ or above from the subtree of $v$.
// - Complexity: Time: O(V + E), Space: O(V + E).