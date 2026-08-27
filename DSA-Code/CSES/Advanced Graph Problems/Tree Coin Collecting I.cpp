// Link: https://cses.fi/problemset/task/3114
#include <bits/stdc++.h>
using namespace std;

const int LOG = 19, INF = 1e9;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; if (!(cin >> n >> q)) return 0;

    vector<int> distToCoin(n + 1, INF), c(n + 1);
    queue<int> qBfs;
    for (int i = 1; i <= n; i++) {
        cin >> c[i];
        if (c[i] == 1) { distToCoin[i] = 0; qBfs.push(i); }
    }

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    while (!qBfs.empty()) {
        int u = qBfs.front(); qBfs.pop();
        for (int v : g[u]) {
            if (distToCoin[v] > distToCoin[u] + 1) {
                distToCoin[v] = distToCoin[u] + 1; qBfs.push(v);
            }
        }
    }

    vector<int> depth(n + 1, 0);
    vector<vector<int>> up(n + 1, vector<int>(LOG, 0)), info(n + 1, vector<int>(LOG, INF));

    queue<int> qTree; qTree.push(1);
    info[1][0] = distToCoin[1];

    while (!qTree.empty()) {
        int u = qTree.front(); qTree.pop();
        for (int v : g[u]) {
            if (v != up[u][0]) {
                up[v][0] = u; depth[v] = depth[u] + 1; info[v][0] = distToCoin[v];
                qTree.push(v);
            }
        }
    }

    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            int p = up[i][j - 1];
            if (p != 0) {
                up[i][j] = up[p][j - 1];
                info[i][j] = min(info[i][j - 1], info[p][j - 1]);
            } else {
                up[i][j] = 0; info[i][j] = info[i][j - 1];
            }
        }
    }

    auto lift = [&](int u, int k, int &minVal) {
        for (int j = 0; j < LOG && u != 0; j++) {
            if ((k >> j) & 1) { minVal = min(minVal, info[u][j]); u = up[u][j]; }
        }
        return u;
    };

    auto getLCA = [&](int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        int dummy = INF;
        a = lift(a, depth[a] - depth[b], dummy);
        if (a == b) return a;
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[a][j] != up[b][j]) { a = up[a][j]; b = up[b][j]; }
        }
        return up[a][0];
    };

    auto queryUp = [&](int u, int anc) {
        if (u == anc) return info[u][0];
        int ans = INF, k = depth[u] - depth[anc];
        lift(u, k, ans);
        return min(ans, info[anc][0]);
    };

    while (q--) {
        int a, b; cin >> a >> b;
        int w = getLCA(a, b);
        int pathDist = depth[a] + depth[b] - 2 * depth[w];
        int minDetour = min(queryUp(a, w), queryUp(b, w));
        cout << pathDist + 2 * minDetour << "\n";
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum steps to go from city a to city b collecting at least one coin along the path (CSES 3114).
// - Approach: Multi-source BFS (nearest coin) + Sparse Table Binary Lifting (min detour distance on path).
// - Intuition: Nearest coin distance for path nodes gives detour requirement; answer equals $\text{dist}(a, b) + 2 \cdot \min_{u \in \text{path}(a,b)} \text{coinDist}(u)$.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N \log N).