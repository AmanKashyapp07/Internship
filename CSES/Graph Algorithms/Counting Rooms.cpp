#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<string> grid;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void dfs(int x, int y) {
    grid[x][y] = '#';

    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (nx < 0 || nx >= n || ny < 0 || ny >= m) {
            continue;
        }

        if (grid[nx][ny] != '.') {
            continue;
        }

        dfs(nx, ny);
    }
}

int main() {
    cin >> n >> m;

    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    int rooms = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '.') {
                rooms++;
                dfs(i, j);
            }
        }
    }

    cout << rooms << '\n';

    return 0;
}