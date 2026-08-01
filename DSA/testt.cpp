#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct Edge {
    int to;
    int64 w;
};

int n, m;
vector<vector<Edge>> graph;
vector<int64> pref;
vector<bool> vis;

bool dfs(int u) {
    vis[u] = true;

    for (auto [v, w] : graph[u]) {
        if (!vis[v]) {
            pref[v] = pref[u] + w;
            if (!dfs(v))
                return false;
        } else { // if the node is already visited, check if the constraint is satisfied
            if (pref[v] != pref[u] + w)
                return false;
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    graph.assign(n + 1, {});
    pref.assign(n + 1, 0);
    vis.assign(n + 1, false);

    for (int i = 0; i < m; i++) {
        int l, r;
        int64 s;
        cin >> l >> r >> s;

        // pref[r] = pref[l-1] + s
        graph[l - 1].push_back({r, s});
        graph[r].push_back({l - 1, -s});
    }

    for (int i = 0; i <= n; i++) {
        if (!vis[i]) { // unvisited node, start a new DFS
            pref[i] = 0;          // arbitrary starting value
            if (!dfs(i)) { // if the DFS returns false, it means we found a contradiction in the constraints
                cout << "NO\n";
                return 0;
            }
        }
    }

    cout << "YES\n";
    for (int i = 1; i <= n; i++) {
        cout << pref[i] - pref[i - 1] << " ";
    }
    cout << "\n";

    return 0;
}