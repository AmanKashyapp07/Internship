#include <bits/stdc++.h>
using namespace std;

struct HierholzerUndirected {
    int n, m = 0;
    vector<vector<pair<int,int>>> g;
    vector<bool> used;
    vector<int> deg, path;

    HierholzerUndirected(int n) : n(n), g(n + 1), deg(n + 1, 0) {}

    // Add an undirected edge
    void addEdge(int u, int v) {
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        used.push_back(false);
        deg[u]++;
        deg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, id] = g[u].back();
            g[u].pop_back();

            if (used[id]) continue;
            used[id] = true;

            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianCircuit(int start) {
        // Every vertex must have even degree.
        for (int i = 1; i <= n; i++)
            if (deg[i] & 1)
                return {};

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        // Every edge must have been used.
        if ((int)path.size() != m + 1)
            return {};

        return path;
    }
};

int main(){
    int n, m;
    cin >> n >> m;
    HierholzerUndirected h(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        h.addEdge(u, v);
    }
    vector<int> circuit = h.getEulerianCircuit(1);
    if (circuit.empty()) {
        cout << "IMPOSSIBLE\n";
    } else {
        for (int u : circuit) {
            cout << u << " ";  
        }
        cout << endl;
    }

}