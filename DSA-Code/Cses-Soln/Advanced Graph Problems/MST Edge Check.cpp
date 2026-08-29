// Link: https://cses.fi/problemset/task/3407
#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v, w, id; bool operator<(const Edge& o) const { return w < o.w; } };

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

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) { cin >> edges[i].u >> edges[i].v >> edges[i].w; edges[i].id = i; }
    sort(edges.begin(), edges.end());

    DSU dsu(n);
    vector<bool> inMST(m, false);

    for (int i = 0; i < m; ) {
        int j = i;
        while (j < m && edges[j].w == edges[i].w) j++;
        for (int k = i; k < j; k++) {
            if (dsu.find(edges[k].u) != dsu.find(edges[k].v)) inMST[edges[k].id] = true;
        }
        for (int k = i; k < j; k++) dsu.unite(edges[k].u, edges[k].v);
        i = j;
    }

    for (int i = 0; i < m; i++) cout << (inMST[i] ? "YES\n" : "NO\n");
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each edge, determine if it can belong to AT LEAST ONE Minimum Spanning Tree (CSES 3407).
// - Approach: Kruskal Batching by Edge Weight.
// - Intuition: An edge of weight W can be in an MST iff its endpoints are unconnected in DSU prior to incorporating weight W batch edges.
// - Complexity: Time: O(E \log E), Space: O(V + E).