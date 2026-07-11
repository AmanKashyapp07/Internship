
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;


const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >>m;

    vector<string> g(n);
    for (auto &row : g) cin >> row;

    vector<vector<int>> monster(n, vector<int>(m, INF));
    vector<vector<int>> dist(n, vector<int>(m, INF));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m, {-1, -1}));
    vector<vector<char>> move(n, vector<char>(m));

    queue<pair<int,int>> q;
    pair<int,int> start;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char dir[] = {'U', 'D', 'L', 'R'};

    // Read starting positions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (g[i][j] == 'M') {
                monster[i][j] = 0;
                q.push({i, j});
            }
            if (g[i][j] == 'A')
                start = {i, j};
        }
    }

    // Multi-source BFS from monsters
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (g[nr][nc] == '#' || monster[nr][nc] != INF) continue;

            monster[nr][nc] = monster[r][c] + 1;
            q.push({nr, nc});
        }
    }

    q.push(start);
    dist[start.first][start.second] = 0;

    pair<int,int> end = {-1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == 0 || r == n - 1 || c == 0 || c == m - 1) {
            end = {r, c};
            break;
        }

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            int nd = dist[r][c] + 1;

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (g[nr][nc] == '#' || dist[nr][nc] != INF) continue;
            if (nd >= monster[nr][nc]) continue; // if the monster can reach this cell before or at the same time as us, we cannot go there

            dist[nr][nc] = nd;
            parent[nr][nc] = {r, c};
            move[nr][nc] = dir[k];
            q.push({nr, nc});
        }
    }

    if (end.first == -1) {
        cout << "NO\n";
        return 0;
    }

    string path;
    auto cur = end;
    while(cur != start) {
        auto [r, c] = cur;
        path += move[r][c];
        cur = parent[r][c];
    }
    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << path.size() << '\n';
    cout << path << '\n';
}