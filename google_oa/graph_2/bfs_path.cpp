/**
 * CSES 1193 - Labyrinth
 *
 * Approach:
 * - Run BFS from 'A'.
 * - Store the parent cell and move used to reach every cell.
 * - If 'B' is reachable, backtrack using the parent array.
 *
 * Time: O(N * M)
 * Space: O(N * M)
 */


#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
using namespace std;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    pair<int,int> st, en;

    for (int i = 0; i < n; i++) {
        cin >> grid[i];
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'A') st = {i, j};
            if (grid[i][j] == 'B') en = {i, j};
        }
    }

    vector<vector<bool>> vis(n, vector<bool>(m));
    vector<vector<pair<int,int>>> par(n, vector<pair<int,int>>(m, {-1, -1}));
    vector<vector<char>> move(n, vector<char>(m));

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char dir[] = {'U', 'D', 'L', 'R'};

    queue<pair<int,int>> q;
    q.push(st);
    vis[st.first][st.second] = true;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (vis[nr][nc] || grid[nr][nc] == '#') continue;

            vis[nr][nc] = true;
            par[nr][nc] = {r, c};
            move[nr][nc] = dir[k];
            q.push({nr, nc});
        }
    }

    if (!vis[en.first][en.second]) {
        cout << "NO\n";
        return 0;
    }

    string path;
    auto [r, c] = en;

    while (make_pair(r, c) != st) {
        path += move[r][c];
        tie(r, c) = par[r][c];
    }

    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << path.size() << '\n';
    cout << path << '\n';
}