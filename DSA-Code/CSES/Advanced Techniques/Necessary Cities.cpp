// Link: https://cses.fi/problemset/task/2077
#include <bits/stdc++.h>
using namespace std;

int n, m, timer_ = 0;
vector<vector<int>> adj;
vector<int> tin, low;
vector<bool> isArt;

void dfs(int u, int parent) {
    tin[u] = low[u] = ++timer_;
    int children = 0;
    for (int v : adj[u]) {
        if (v == parent) continue;
        if (tin[v]) { low[u] = min(low[u], tin[v]); }
        else {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (parent != -1 && low[v] >= tin[u]) isArt[u] = true;
            children++;
        }
    }
    if (parent == -1 && children > 1) isArt[u] = true;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> m;
    adj.resize(n + 1); tin.assign(n + 1, 0); low.assign(n + 1, 0); isArt.assign(n + 1, false);
    while (m--) {
        int a, b; cin >> a >> b;
        adj[a].push_back(b); adj[b].push_back(a);
    }
    dfs(1, -1);
    vector<int> ans;
    for (int i = 1; i <= n; i++) if (isArt[i]) ans.push_back(i);
    cout << ans.size() << '\n';
    for (int x : ans) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find all articulation points (cities whose removal disconnects the graph) (CSES 2077).
// - Approach: Tarjan's Articulation Point Algorithm using DFS discovery times `tin` and low-link values `low`.
// - Intuition: Node $u$ is an articulation point if (non-root) some child $v$ has `low[v] >= tin[u]`, or (root) it has more than one DFS child.
// - Complexity: Time: O(V + E), Space: O(V + E).