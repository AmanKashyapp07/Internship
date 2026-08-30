// Link: https://cses.fi/problemset/task/1709
#include <bits/stdc++.h>
using namespace std;

class BipartiteGraph {
    int L, R;
    vector<vector<int>> g;
    vector<int> match_R, match_L, vis;

    bool augment(int u) {
        if (vis[u]) return false;
        vis[u] = 1;
        for (int v : g[u]) {
            if (match_R[v] == 0 || augment(match_R[v])) {
                match_R[v] = u;
                match_L[u] = v;
                return true;
            }
        }
        return false;
    }

    void dfs_vertex_cover(int u, vector<bool>& vis_L, vector<bool>& vis_R) {
        vis_L[u] = true;
        for (int v : g[u]) {
            if (!vis_R[v]) {
                vis_R[v] = true;
                if (match_R[v] != 0 && !vis_L[match_R[v]]) {
                    dfs_vertex_cover(match_R[v], vis_L, vis_R);
                }
            }
        }
    }

public:
    BipartiteGraph(int left_size, int right_size)
        : L(left_size), R(right_size) {
        g.resize(L + 1);
        match_R.assign(R + 1, 0);
        match_L.assign(L + 1, 0);
    }

    void add_edge(int left, int right) {
        g[left].push_back(right);
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        for (int i = 1; i <= L; i++) {
            vis.assign(L + 1, 0);
            augment(i);
        }

        vector<bool> vis_L(L + 1, false), vis_R(R + 1, false);
        for (int i = 1; i <= L; i++) {
            if (match_L[i] == 0 && !vis_L[i]) dfs_vertex_cover(i, vis_L, vis_R);
        }

        vector<int> cover_L, cover_R;
        for (int i = 1; i <= L; i++) if (!vis_L[i]) cover_L.push_back(i);
        for (int i = 1; i <= R; i++) if (vis_R[i]) cover_R.push_back(i);

        return {cover_L, cover_R};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    BipartiteGraph G(n, n);
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;
        for (int j = 1; j <= n; j++) {
            if (s[j - 1] == 'o\) G.add_edge(i, j);
        }
    }

    auto [rows, cols] = G.minimum_vertex_cover();
    cout << rows.size() + cols.size() << "\n";
    for (int r : rows) cout << 1 << " " << r << "\n";
    for (int c : cols) cout << 2 << " " << c << "\n";

    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum row/column operations to clear all coins from an N x N grid (CSES 1709).
// - Approach: Minimum Vertex Cover in Bipartite Graph (König's Theorem).
// - Intuition: Rows and columns form bipartite sets; each coin is an edge. By König's theorem, the minimum vertex cover equals maximum matching, extracted via alternating paths.
// - Complexity: Time: O(V * E) = O(N^3), Space: O(N^2).
