// Link: https://cses.fi/problemset/task/1709
#include <bits/stdc++.h>
using namespace std;

struct BipartiteGraph {
    int L, R;
    vector<vector<int>> g;
    vector<int> matchL, matchR, vis;

    BipartiteGraph(int L, int R) : L(L), R(R), g(L + 1), matchL(L + 1, 0), matchR(R + 1, 0) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    bool augment(int u) {
        if (vis[u]) return false;
        vis[u] = 1;
        for (int v : g[u]) {
            if (matchR[v] == 0 || augment(matchR[v])) {
                matchR[v] = u; matchL[u] = v; return true;
            }
        }
        return false;
    }

    int maxMatching() {
        int cnt = 0;
        for (int i = 1; i <= L; i++) { vis.assign(L + 1, 0); if (augment(i)) cnt++; }
        return cnt;
    }

    void dfsCover(int u, vector<bool>& visL, vector<bool>& visR) {
        visL[u] = true;
        for (int v : g[u]) {
            if (!visR[v]) {
                visR[v] = true;
                if (matchR[v] != 0 && !visL[matchR[v]]) dfsCover(matchR[v], visL, visR);
            }
        }
    }

    pair<vector<int>, vector<int>> minVertexCover() {
        maxMatching();
        vector<bool> visL(L + 1, false), visR(R + 1, false);
        for (int i = 1; i <= L; i++) if (matchL[i] == 0 && !visL[i]) dfsCover(i, visL, visR);

        vector<int> coverL, coverR;
        for (int i = 1; i <= L; i++) if (!visL[i]) coverL.push_back(i);
        for (int i = 1; i <= R; i++) if (visR[i]) coverR.push_back(i);
        return {coverL, coverR};
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    BipartiteGraph G(n, n);
    for (int i = 1; i <= n; i++) {
        string s; cin >> s;
        for (int j = 1; j <= n; j++) if (s[j - 1] == 'o') G.addEdge(i, j);
    }
    auto [rows, cols] = G.minVertexCover();
    cout << rows.size() + cols.size() << '\n';
    for (int r : rows) cout << 1 << " " << r << '\n';
    for (int c : cols) cout << 2 << " " << c << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum row/column operations to clear all coins from an n x n grid (CSES 1709).
// - Approach: Minimum Vertex Cover in Bipartite Graph via Konig's Theorem.
// - Intuition: Rows are left vertices, columns are right vertices, coins are edges; by Konig's Theorem, size of Minimum Vertex Cover equals Maximum Bipartite Matching.
// - Complexity: Time: O(V \cdot E) = O(N^3), Space: O(N^2).