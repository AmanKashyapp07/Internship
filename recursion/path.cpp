/**
 * Rat in a Maze - Backtracking
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define pb       push_back

vector<string> ans;

/*
    D -> Down
    L -> Left
    R -> Right
    U -> Up
*/
int dr[] = {1, 0, 0, -1};
int dc[] = {0, -1, 1, 0};
char dir[] = {'D', 'L', 'R', 'U'};

void dfs(int r, int c,
         vector<vector<int>>& grid,
         vector<vector<int>>& vis,
         string path,
         int n) {

    // Reached destination
    if (r == n - 1 && c == n - 1) {
        ans.pb(path);
        return;
    }

    vis[r][c] = 1;

    for (int k = 0; k < 4; k++) {

        int nr = r + dr[k];
        int nc = c + dc[k];

        bool valid =
            nr >= 0 && nr < n &&
            nc >= 0 && nc < n &&
            grid[nr][nc] == 1 &&
            !vis[nr][nc];

        if (valid) {
            dfs(nr, nc,
                grid, vis,
                path + dir[k],
                n);
        }
    }

    // BACKTRACK
    vis[r][c] = 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<vector<int>> grid(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    if (grid[0][0] == 0) {
        cout << "No Path\n";
        return 0;
    }

    vector<vector<int>> vis(n, vector<int>(n, 0));

    dfs(0, 0, grid, vis, "", n);

    sort(all(ans));

    for (auto& path : ans) {
        cout << path << "\n";
    }

    return 0;
}