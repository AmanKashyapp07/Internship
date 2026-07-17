#include <bits/stdc++.h>
using namespace std;

struct SCC {
    int n;
    vector<vector<int>> adj, radj, sccs, dag;
    stack<int> order;
    vector<int> comp;
    vector<bool> vis;

    SCC(int n) : n(n), adj(n + 1), radj(n + 1), comp(n + 1, -1), vis(n + 1) {}

    void add(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : adj[u])
            if (!vis[v]) dfs1(v);
        order.push(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        sccs[c].push_back(u);
        for (int v : radj[u])
            if (comp[v] == -1)
                dfs2(v, c);
    }

    vector<vector<int>> build() {
        for (int i = 1; i <= n; i++)
            if (!vis[i]) dfs1(i);

        while (!order.empty()) {
            int u = order.top();
            order.pop();
            if (comp[u] == -1) {
                sccs.emplace_back();
                dfs2(u, (int)sccs.size() - 1);
            }
        }
        return sccs;
    }

    vector<vector<int>> get_dag() {
        dag.assign(sccs.size(), {});
        for (int u = 1; u <= n; u++) {
            for (int v : adj[u]) {
                if (comp[u] != comp[v])
                    dag[comp[u]].push_back(comp[v]);
            }
        }

        for (auto &x : dag) {
            sort(x.begin(), x.end());
            x.erase(unique(x.begin(), x.end()), x.end());
        }

        return dag;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    SCC scc(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        scc.add(u, v);
    }

    scc.build();
    auto dag = scc.get_dag();

    int C = dag.size();

    // Topological order of SCC DAG
    vector<int> indeg(C);
    for (int u = 0; u < C; u++)
        for (int v : dag[u])
            indeg[v]++;

    queue<int> qu;
    for (int i = 0; i < C; i++)
        if (indeg[i] == 0)
            qu.push(i);

    vector<int> topo;
    while (!qu.empty()) {
        int u = qu.front();
        qu.pop();
        topo.push_back(u);
        for (int v : dag[u]) {
            if (--indeg[v] == 0)
                qu.push(v);
        }
    }

    // Reachability bitsets on DAG
    vector<bitset<50000>> reach(C); // 50000 is the maximum number of SCCs, adjust if needed

    for (int i = C - 1; i >= 0; i--) {
        int u = topo[i];
        reach[u].set(u);
        for (int v : dag[u])
            reach[u] |= reach[v];
    }

    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << (reach[scc.comp[a]].test(scc.comp[b]) ? "YES" : "NO") << '\n';
    }

    return 0;
}