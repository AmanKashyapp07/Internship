// Link: https://cses.fi/problemset/task/2101
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        a = find(a); b = find(b); if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, q; cin >> n >> m >> q;
    vector<pair<int,int>> edge(m + 1);
    for (int i = 1; i <= m; i++) cin >> edge[i].first >> edge[i].second;

    vector<int> u(q), v(q), lo(q), hi(q);
    for (int i = 0; i < q; i++) {
        cin >> u[i] >> v[i];
        lo[i] = (u[i] == v[i]) ? 0 : 1;
        hi[i] = (u[i] == v[i]) ? 0 : m + 1;
    }

    // Parallel binary search: O((M + Q) * log M) using DSU per round
    while (true) {
        vector<vector<int>> bucket(m + 2);
        bool any = false;
        for (int i = 0; i < q; i++) {
            if (lo[i] < hi[i]) { any = true; bucket[(lo[i] + hi[i]) / 2].push_back(i); }
        }
        if (!any) break;
        DSU dsu(n);
        for (int day = 1; day <= m; day++) {
            dsu.unite(edge[day].first, edge[day].second);
            for (int id : bucket[day]) {
                if (dsu.find(u[id]) == dsu.find(v[id])) hi[id] = day;
                else lo[id] = day + 1;
            }
        }
        for (int id : bucket[m + 1]) lo[id] = m + 1;
    }

    for (int i = 0; i < q; i++)
        cout << (lo[i] == m + 1 ? -1 : lo[i]) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each query (u, v), find the earliest road index that connects them (CSES 2101).
// - Approach: Parallel Binary Search on DSU — all queries binary-search together; each round rebuilds DSU once.
// - Intuition: Each query independently binary searches its answer in $[1, m]$; grouping all mid-points into buckets allows one DSU pass per round instead of one per query.
// - Complexity: Time: O((M + Q) \cdot \log M), Space: O(N + M + Q).