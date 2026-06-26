/**
 * CSES 1193 - Labyrinth
 *
 * Description:
 * You are given a map of a labyrinth with start 'A' and end 'B'.
 * Find a path from start to end. If a path exists, print the path length and step directions ('L', 'R', 'U', 'D').
 *
 * Approach:
 * - Perform a Breadth-First Search (BFS) on the grid from 'A'.
 * - Store the step direction taken to reach each cell.
 * - Backtrack from 'B' to 'A' using the recorded moves to reconstruct the exact path.
 *
 * Time Complexity: O(R * C)
 * Space Complexity: O(R * C)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<string> grid(n); pair<int, int> start, end;
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'A') start = {i, j};
            if (grid[i][j] == 'B') end = {i, j};
        }
    }
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<char>> move_taken(n, vector<char>(m));
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1}; char dir[] = {'U', 'D', 'L', 'R'};
    queue<pair<int, int>> q; q.push(start); vis[start.first][start.second] = true;
    // BFS on grid cell transitions
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && !vis[nr][nc] && grid[nr][nc] != '#') {
                vis[nr][nc] = true; parent[nr][nc] = {r, c}; move_taken[nr][nc] = dir[k]; q.push({nr, nc});
            }
        }
    }
    if (!vis[end.first][end.second]) { cout << "NO\n"; return 0; }
    string path; auto [r, c] = end;
    // Reconstruct directions and path by backtracking from end 'B' to start 'A'
    while (make_pair(r, c) != start) {
        path.push_back(move_taken[r][c]);
        auto [pr, pc] = parent[r][c]; r = pr; c = pc;
    }
    reverse(path.begin(), path.end());
    cout << "YES\n" << path.size() << '\n' << path << '\n';
}