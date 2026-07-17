// Link: https://cses.fi/problemset/task/1693

#include <bits/stdc++.h>
using namespace std;

struct HierholzerDirected {
    int n, m = 0;
    vector<vector<int>> g;
    vector<int> indeg, outdeg;
    vector<int> path;

    HierholzerDirected(int n)
        : n(n), g(n + 1), indeg(n + 1, 0), outdeg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        outdeg[u]++;
        indeg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back();
            g[u].pop_back();
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianCircuit(int start) {
        for (int i = 1; i <= n; i++) {
            if (indeg[i] != outdeg[i])
                return {};
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1)
            return {};

        return path;
    }

    vector<int> getEulerianPath(int start, int end) {
        if (start == end)
            return getEulerianCircuit(start);

        for (int i = 1; i <= n; i++) {
            if (i == start) {
                if (outdeg[i] != indeg[i] + 1)
                    return {};
            } else if (i == end) {
                if (indeg[i] != outdeg[i] + 1)
                    return {};
            } else {
                if (indeg[i] != outdeg[i])
                    return {};
            }
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1)
            return {};

        return path;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    HierholzerDirected H(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        H.addEdge(u, v);
    }

    auto ans = H.getEulerianPath(1, n);

    if (ans.empty()) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    for (int x : ans)
        cout << x << ' ';
    cout << '\n';

    return 0;
}