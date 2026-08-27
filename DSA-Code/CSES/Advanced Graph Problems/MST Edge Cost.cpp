// Link: https://cses.fi/problemset/task/3409
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;

struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; return true;
    }
};

struct Edge { int u, v, w, id; bool inMST = false; bool operator<(const Edge& o) const { return w < o.w; } };

vector<vector<pair<int, int>>> treeG;
vector<int> depth;
vector<vector<int>> up, mx;

void dfs(int u, int p, int wt) {
    up[u][0] = p; mx[u][0] = wt;
    for (auto [v, w] : treeG[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1; dfs(v, u, w);
    }
}

int getMaxEdge(int u, int v, int n) {
    int ans = 0;
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int j = LOG - 1; j >= 0; j--) {
        if ((diff >> j) & 1) { ans = max(ans, mx[u][j]); u = up[u][j]; }
    }
    if (u == v) return ans;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            ans = max({ans, mx[u][j], mx[v][j]});
            u = up[u][j]; v = up[v][j];
        }
    }
    return max({ans, mx[u][0], mx[v][0]});
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) { cin >> edges[i].u >> edges[i].v >> edges[i].w; edges[i].id = i; }
    vector<Edge> ord = edges; sort(ord.begin(), ord.end());

    DSU dsu(n); treeG.resize(n + 1);
    long long mstCost = 0;
    for (auto &e : ord) {
        if (dsu.unite(e.u, e.v)) {
            mstCost += e.w; e.inMST = true;
            treeG[e.u].push_back({e.v, e.w}); treeG[e.v].push_back({e.u, e.w});
        }
    }

    vector<int> isMST(m, 0);
    for (auto &e : ord) if (e.inMST) isMST[e.id] = 1;

    depth.assign(n + 1, 0); up.assign(n + 1, vector<int>(LOG, 0)); mx.assign(n + 1, vector<int>(LOG, 0));
    dfs(1, 0, 0);
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[up[i][j - 1]][j - 1];
            mx[i][j] = max(mx[i][j - 1], mx[up[i][j - 1]][j - 1]);
        }
    }

    for (int i = 0; i < m; i++) {
        if (isMST[i]) cout << mstCost << '\n';
        else {
            int maxE = getMaxEdge(edges[i].u, edges[i].v, n);
            cout << mstCost + edges[i].w - maxE << '\n';
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each edge, find the minimum weight spanning tree containing that specific edge (CSES 3409).
// - Approach: Kruskal MST + Binary Lifting LCA Max Edge Query (`mstCost + e.w - maxEdgeOnPath`).
// - Intuition: For non-MST edge $e=(u,v,w)$, adding $e$ creates a cycle; replacing the heaviest edge on the tree path between u and v minimizes the constrained spanning tree weight.
// - Complexity: Time: O(E \log E + Q \log V), Space: O(V \log V + E).