#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct SCC {
    ll n;
    vector<vector<ll>> adj, radj, sccs, dag;
    stack<ll> order;
    vector<ll> comp;
    vector<bool> vis;

    SCC(ll n) : n(n), adj(n + 1), radj(n + 1), comp(n + 1, -1), vis(n + 1) {}

    void add(ll u, ll v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    void dfs1(ll u) {
        vis[u] = true;
        for (ll v : adj[u])
            if (!vis[v])
                dfs1(v);
        order.push(u);
    }

    void dfs2(ll u, ll c) {
        comp[u] = c;
        sccs[c].push_back(u);
        for (ll v : radj[u])
            if (comp[v] == -1)
                dfs2(v, c);
    }

    vector<vector<ll>> build() {
        for (ll i = 1; i <= n; i++)
            if (!vis[i])
                dfs1(i);

        while (!order.empty()) {
            ll u = order.top();
            order.pop();

            if (comp[u] == -1) {
                sccs.emplace_back();
                dfs2(u, (ll)sccs.size() - 1);
            }
        }
        return sccs;
    }

    vector<vector<ll>> get_dag() {
        dag.assign(sccs.size(), {});

        for (ll u = 1; u <= n; u++) {
            for (ll v : adj[u]) {
                if (comp[u] != comp[v])
                    dag[comp[u]].push_back(comp[v]);
            }
        }

        for (auto &nbrs : dag) {
            sort(nbrs.begin(), nbrs.end());
            nbrs.erase(unique(nbrs.begin(), nbrs.end()), nbrs.end());
        }

        return dag;
    }
};

class DirectedGraphToDAG {
    SCC scc;

public:
    DirectedGraphToDAG(ll n) : scc(n) {}

    vector<vector<ll>> buildDAG(const vector<pair<ll, ll>> &edges) {
        for (auto &e : edges)
            scc.add(e.first, e.second);
        scc.build();
        return scc.get_dag();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;

    SCC scc(n);

    for (ll i = 0; i < m; i++) {
        ll u, v;
        cin >> u >> v;
        scc.add(u, v);
    }

    scc.build();
    auto dag = scc.get_dag();

    ll C = (ll)scc.sccs.size();

    if (C == 1) {
        cout << 0 << '\n';
        return 0;
    }

    vector<ll> indeg(C, 0), outdeg(C, 0);

    for (ll u = 0; u < C; u++) {
        outdeg[u] = (ll)dag[u].size();
        for (ll v : dag[u])
            indeg[v]++;
    }

    vector<ll> sources, sinks;

    for (ll i = 0; i < C; i++) {
        if (indeg[i] == 0) sources.push_back(i);
        if (outdeg[i] == 0) sinks.push_back(i);
    }

    ll k = max((ll)sources.size(), (ll)sinks.size());

    while ((ll)sources.size() < k)
        sources.push_back(sources.back());

    while ((ll)sinks.size() < k)
        sinks.push_back(sinks.back());

    cout << k << '\n';

    for (ll i = 0; i < k; i++) {
        ll from = scc.sccs[sinks[i]][0];
        ll to = scc.sccs[sources[(i + 1) % k]][0];
        cout << from << " " << to << '\n';
    }

    return 0;
}