// Link: https://cses.fi/problemset/task/1193
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<string> grid(n);
    pair<int, int> start, finish;

    for (int i = 0; i < n; i++) {
        cin >> grid[i];
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'A') start = {i, j};
            if (grid[i][j] == 'B') finish = {i, j};
        }
    }

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<char>> moveTaken(n, vector<char>(m));

    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    char dir[] = {'U', 'D', 'L', 'R'};

    queue<pair<int, int>> q;
    q.push(start); vis[start.first][start.second] = true;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m || vis[nr][nc] || grid[nr][nc] == '#') continue;
            vis[nr][nc] = true; parent[nr][nc] = {r, c}; moveTaken[nr][nc] = dir[k];
            q.push({nr, nc});
        }
    }

    if (!vis[finish.first][finish.second]) { cout << "NO\n"; return 0; }

    string path;
    auto [r, c] = finish;
    while (make_pair(r, c) != start) {
        path.push_back(moveTaken[r][c]);
        auto [pr, pc] = parent[r][c]; r = pr; c = pc;
    }
    reverse(path.begin(), path.end());

    cout << "YES\n" << path.size() << '\n' << path << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find shortest path from start 'A' to end 'B' in an n x m labyrinth grid (CSES 1193).
// - Approach: 2D Grid BFS + Parent Move Reconstruction.
// - Intuition: Breadth-First Search guarantees shortest path in unweighted grid; parent array tracks directions `U, D, L, R`.
// - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).