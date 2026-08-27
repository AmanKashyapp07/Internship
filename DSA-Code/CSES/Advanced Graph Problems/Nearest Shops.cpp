// Link: https://cses.fi/problemset/task/3303
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<int> shops(k);
    vector<bool> isShop(n + 1, false);
    for (int i = 0; i < k; i++) { cin >> shops[i]; isShop[shops[i]] = true; }

    vector<vector<int>> g(n + 1);
    while (m--) { int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u); }

    vector<int> d1(n + 1, INF), d2(n + 1, INF), r1(n + 1, 0), r2(n + 1, 0);
    queue<int> q;

    auto relax = [&](int node, int dist, int root) {
        if (root == r1[node] || root == r2[node]) return;
        if (dist < d1[node]) {
            d2[node] = d1[node]; r2[node] = r1[node];
            d1[node] = dist; r1[node] = root;
            q.push(node);
        } else if (dist < d2[node]) {
            d2[node] = dist; r2[node] = root;
            q.push(node);
        }
    };

    for (int s : shops) { d1[s] = 0; r1[s] = s; q.push(s); }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            relax(v, d1[u] + 1, r1[u]);
            if (r2[u] != 0) relax(v, d2[u] + 1, r2[u]);
        }
    }

    for (int i = 1; i <= n; i++) {
        int res = isShop[i] ? d2[i] : d1[i];
        cout << (res == INF ? -1 : res) << (i == n ? '\n' : ' ');
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find distance to nearest shop for non-shop cities, and distance to nearest distinct shop for shop cities (CSES 3303).
// - Approach: Multi-source BFS tracking Best & Second-Best Distances with Distinct Roots.
// - Intuition: Multi-source BFS from all shops maintains top two shortest distances from different shop sources to resolve shop-to-other-shop distances.
// - Complexity: Time: O(V + E), Space: O(V + E).