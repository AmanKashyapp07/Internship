// Link: https://cses.fi/problemset/task/2079

#include <bits/stdc++.h>
using namespace std;

struct Centroid {
    int n;
    vector<vector<int>> g;
    vector<int> sz;

    Centroid(int n): n(n), g(n + 1), sz(n + 1) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int dfs(int u, int p) {
        sz[u] = 1;
        for (int v : g[u])
            if (v != p)
                sz[u] += dfs(v, u);
        return sz[u];
    }

    int centroid(int u, int p, int tot) {
        for (int v : g[u])
            if (v != p && sz[v] > tot / 2)
                return centroid(v, u, tot);
        return u;
    }

    int get() {
        int tot = dfs(1, -1);
        return centroid(1, -1, tot);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Centroid ct(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        ct.addEdge(u, v);
    }

    cout << ct.get() << '\n';
}