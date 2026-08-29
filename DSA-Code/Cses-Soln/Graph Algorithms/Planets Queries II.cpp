// Link: https://cses.fi/problemset/task/1160
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;

struct FunctionalGraph {
    int n;
    vector<int> dist, cyc, comp, pos;
    vector<vector<int>> up;

    FunctionalGraph(const vector<int>& to) {
        n = to.size();
        dist.assign(n, -1); cyc.assign(n, -1); comp.assign(n, -1); pos.assign(n, -1);
        up.assign(n, vector<int>(LOG));
        for (int i = 0; i < n; i++) up[i][0] = to[i];
        for (int j = 1; j < LOG; j++) for (int i = 0; i < n; i++) up[i][j] = up[up[i][j - 1]][j - 1];

        vector<vector<int>> radj(n);
        for (int i = 0; i < n; i++) radj[to[i]].push_back(i);

        int compId = 0;
        vector<int> vis(n, 0);
        for (int i = 0; i < n; i++) {
            if (vis[i]) continue;
            int curr = i;
            while (!vis[curr]) { vis[curr] = 1; curr = to[curr]; }
            if (vis[curr] == 1) {
                vector<int> cycle; int u = curr;
                do { cycle.push_back(u); u = to[u]; } while (u != curr);
                int len = cycle.size();
                for (int j = 0; j < len; j++) {
                    int node = cycle[j]; dist[node] = 0; cyc[node] = len; comp[node] = compId; pos[node] = j;
                }
                auto dfs = [&](auto &self, int v) -> void {
                    for (int nxt : radj[v]) {
                        if (dist[nxt] != -1) continue;
                        dist[nxt] = dist[v] + 1; comp[nxt] = comp[v]; self(self, nxt);
                    }
                };
                for (int node : cycle) dfs(dfs, node);
                compId++;
            }
            curr = i;
            while (vis[curr] == 1) { vis[curr] = 2; curr = to[curr]; }
        }
    }

    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++) if ((k >> j) & 1) u = up[u][j];
        return u;
    }

    int query(int a, int b) {
        if (comp[a] != comp[b]) return -1;
        if (dist[a] == 0 && dist[b] == 0) return (pos[b] - pos[a] + cyc[a]) % cyc[a];
        if (dist[a] > 0 && dist[b] == 0) {
            int entry = jump(a, dist[a]);
            return dist[a] + (pos[b] - pos[entry] + cyc[b]) % cyc[b];
        }
        if (dist[a] >= dist[b]) {
            int d = dist[a] - dist[b];
            if (jump(a, d) == b) return d;
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> to(n);
    for (int i = 0; i < n; i++) { cin >> to[i]; to[i]--; }
    FunctionalGraph fg(to);

    while (q--) {
        int a, b; cin >> a >> b; a--; b--;
        cout << fg.query(a, b) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum teleports to reach planet b from planet a in a functional graph for q queries (CSES 1160).
// - Approach: Functional Graph Decomposition + Binary Lifting.
// - Intuition: Classify nodes into tree paths or cycle nodes; queries compare tree depths via binary lifting jumps or modular cycle offsets.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N \log N).