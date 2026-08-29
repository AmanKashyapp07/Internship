// Link: https://cses.fi/problemset/task/1192
#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<string> grid;
int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};

void dfs(int x, int y) {
    grid[x][y] = '#';
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '.') dfs(nx, ny);
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if (!(cin >> n >> m)) return 0;
    grid.resize(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    int rooms = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '.') { rooms++; dfs(i, j); }
        }
    }
    cout << rooms << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count connected components of floor cells `.` in an n x m grid (CSES 1192).
// - Approach: 2D Grid DFS / BFS Flood Fill (`grid[x][y] = '#'`).
// - Intuition: Iterating over grid and starting DFS at unvisited floor cells counts total distinct room components.
// - Complexity: Time: O(N \cdot M), Space: O(N \cdot M) recursion stack.