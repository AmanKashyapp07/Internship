/**
 * Problem: Flight Routes Check (https://cses.fi/problemset/task/1682)
 * Check if the directed graph is strongly connected; if not, output a pair of disconnected nodes.
 * Time: O(V + E) time, O(V + E) space.
 */
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;
stack<int> st;

void dfs1(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
    st.push(u); // Push nodes by finish time
}

void dfs2(int u, vector<int>& comp) {
    vis[u] = true;
    comp.push_back(u);
    for (int v : rg[u]) if (!vis[v]) dfs2(v, comp);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    g.resize(n + 1); rg.resize(n + 1);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; g[u].push_back(v); rg[v].push_back(u); }
    vis.assign(n + 1, false);
    for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
    vis.assign(n + 1, false);
    vector<vector<int>> sccs;
    // Extract SCC components in stack order
    while (!st.empty()) {
        int u = st.top(); 
        st.pop();
        if (vis[u]) continue;
        vector<int> comp; 
        dfs2(u, comp);
        sccs.push_back(comp);
    }
    if (sccs.size() == 1) { cout << "YES\n"; return 0; }
    cout << "NO\n" << sccs[1][0] << ' ' << sccs[0][0] << '\n'; // Route fails from component 2 root to component 1 root
}
