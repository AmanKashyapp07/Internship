// Link: https://cses.fi/problemset/task/1194
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<string> g(n);
    for (int i = 0; i < n; i++) cin >> g[i];

    vector<vector<int>> md(n, vector<int>(m, INF));
    queue<pair<int, int>> q;
    pair<int, int> start;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (g[i][j] == 'M') { md[i][j] = 0; q.push({i, j}); }
            if (g[i][j] == 'A') start = {i, j};
        }
    }

    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m || g[nr][nc] == '#' || md[nr][nc] != INF) continue;
            md[nr][nc] = md[r][c] + 1; q.push({nr, nc});
        }
    }

    vector<vector<int>> dist(n, vector<int>(m, INF));
    vector<vector<pair<int, int>>> par(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<char>> moveDir(n, vector<char>(m));
    char dir[] = {'U', 'D', 'L', 'R'};

    q.push(start); dist[start.first][start.second] = 0;
    pair<int, int> finish = {-1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == 0 || r == n - 1 || c == 0 || c == m - 1) { finish = {r, c}; break; }

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m || g[nr][nc] == '#' || dist[nr][nc] != INF) continue;
            int nd = dist[r][c] + 1;
            if (nd >= md[nr][nc]) continue;
            dist[nr][nc] = nd; par[nr][nc] = {r, c}; moveDir[nr][nc] = dir[k];
            q.push({nr, nc});
        }
    }

    if (finish.first == -1) { cout << "NO\n"; return 0; }

    string path;
    auto [r, c] = finish;
    while (make_pair(r, c) != start) {
        path.push_back(moveDir[r][c]);
        auto [pr, pc] = par[r][c]; r = pr; c = pc;
    }
    reverse(path.begin(), path.end());

    cout << "YES\n" << path.size() << '\n' << path << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find path from start 'A' to any boundary cell escaping all monsters 'M' (CSES 1194).
// - Approach: Multi-source BFS for Monster Distances + Single-source BFS for Player Path.
// - Intuition: Player can reach cell $(r, c)$ at time $T$ iff $T < \text{monsterDist}[r][c]$; reaching any grid boundary cell triggers escape.
// - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).