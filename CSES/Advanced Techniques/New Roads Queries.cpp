// Link: https://cses.fi/problemset/task/2101

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;

    DSU(int n) {
        p.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<pair<int,int>> edge(m + 1);
    for (int i = 1; i <= m; i++)
        cin >> edge[i].first >> edge[i].second;

    vector<int> u(q), v(q);
    vector<int> lo(q), hi(q);

    for (int i = 0; i < q; i++) {
        cin >> u[i] >> v[i];
        if (u[i] == v[i]) {
            lo[i] = hi[i] = 0;
        } else {
            lo[i] = 1;
            hi[i] = m + 1;
        }
    }

    while (true) {
        bool any = false;
        vector<vector<int>> bucket(m + 2);

        for (int i = 0; i < q; i++) {
            if (lo[i] < hi[i]) {
                any = true;
                int mid = (lo[i] + hi[i]) / 2;
                bucket[mid].push_back(i);
            }
        }

        if (!any) break;

        DSU dsu(n);

        for (int day = 1; day <= m; day++) {
            dsu.unite(edge[day].first, edge[day].second);

            for (int id : bucket[day]) {
                if (dsu.find(u[id]) == dsu.find(v[id]))
                    hi[id] = day;
                else
                    lo[id] = day + 1;
            }
        }

        // Queries with mid = m+1
        for (int id : bucket[m + 1])
            lo[id] = m + 1;
    }

    for (int i = 0; i < q; i++) {
        if (lo[i] == m + 1)
            cout << -1 << '\n';
        else
            cout << lo[i] << '\n';
    }
}