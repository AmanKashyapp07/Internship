// Link: https://cses.fi/problemset/task/1709

#include <bits/stdc++.h>
using namespace std;

class BipartiteGraph {
private:
    int n, L, R;
    vector<vector<int>> graph;
    vector<vector<int>> g_match;
    vector<int> color;
    vector<int> match_R, match_L, vis;

    bool dfs_color(int u, int c) {
        color[u] = c;
        for (int v : graph[u]) {
            if (color[v] == -1) {
                if (!dfs_color(v, c ^ 1)) return false;
            } else if (color[v] == c) {
                return false;
            }
        }
        return true;
    }

    bool augment(int u) {
        if (vis[u]) return false;
        vis[u] = 1;
        for (int v : g_match[u]) {
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
        for (int v : g_match[u]) {
            if (!vis_R[v]) {
                vis_R[v] = true;
                if (match_R[v] != 0 && !vis_L[match_R[v]]) {
                    dfs_vertex_cover(match_R[v], vis_L, vis_R);
                }
            }
        }
    }

public:
    BipartiteGraph(int total_nodes) : n(total_nodes), L(0), R(0) {
        graph.resize(n + 1);
        color.assign(n + 1, -1);
    }

    BipartiteGraph(int left_size, int right_size)
        : n(left_size + right_size), L(left_size), R(right_size) {
        g_match.resize(L + 1);
        match_R.assign(R + 1, 0);
        match_L.assign(L + 1, 0);
    }

    void add_undirected_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    void add_matching_edge(int left, int right) {
        g_match[left].push_back(right);
    }

    bool is_bipartite() {
        for (int i = 1; i <= n; i++) {
            if (color[i] == -1 && !dfs_color(i, 0))
                return false;
        }
        return true;
    }

    int maximum_matching() {
        int matches = 0;
        match_R.assign(R + 1, 0);
        match_L.assign(L + 1, 0);

        for (int i = 1; i <= L; i++) {
            vis.assign(L + 1, 0);
            if (augment(i)) matches++;
        }
        return matches;
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        maximum_matching();

        vector<bool> vis_L(L + 1, false), vis_R(R + 1, false);

        for (int i = 1; i <= L; i++) {
            if (match_L[i] == 0 && !vis_L[i])
                dfs_vertex_cover(i, vis_L, vis_R);
        }

        vector<int> cover_L, cover_R;

        for (int i = 1; i <= L; i++)
            if (!vis_L[i]) cover_L.push_back(i);

        for (int i = 1; i <= R; i++)
            if (vis_R[i]) cover_R.push_back(i);

        return {cover_L, cover_R};
    }

    pair<vector<int>, vector<int>> maximum_independent_set() {
        auto [cover_L, cover_R] = minimum_vertex_cover();

        vector<bool> in_cover_L(L + 1, false), in_cover_R(R + 1, false);

        for (int x : cover_L) in_cover_L[x] = true;
        for (int x : cover_R) in_cover_R[x] = true;

        vector<int> ind_L, ind_R;

        for (int i = 1; i <= L; i++)
            if (!in_cover_L[i]) ind_L.push_back(i);

        for (int i = 1; i <= R; i++)
            if (!in_cover_R[i]) ind_R.push_back(i);

        return {ind_L, ind_R};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    BipartiteGraph G(n, n);

    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;
        for (int j = 1; j <= n; j++) {
            if (s[j - 1] == 'o')
                G.add_matching_edge(i, j);
        }
    }

    auto [rows, cols] = G.minimum_vertex_cover();

    cout << rows.size() + cols.size() << '\n';

    for (int r : rows)
        cout << 1 << " " << r << '\n';

    for (int c : cols)
        cout << 2 << " " << c << '\n';

    return 0;
}