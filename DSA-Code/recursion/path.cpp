/**
 * Rat in a Maze - Backtracking
 *
 * Description:
 * Given an N x N grid where 1 represents an open path and 0 represents a blocked path, find all
 * possible paths from top-left (0, 0) to bottom-right (N-1, N-1). The rat can move Up (U), Down (D),
 * Left (L), and Right (R).
 *
 * Approach:
 * - DFS / Backtracking.
 * - Mark the current cell `(r, c)` as visited.
 * - Try moving in all 4 directions in the order of 'D', 'L', 'R', 'U'.
 * - For each valid move (within bounds, open cell, and not visited), recurse to the next cell.
 * - Backtrack by unmarking the current cell as visited to allow other paths to explore it.
 *
 * Time Complexity: O(3^(N^2)) in the worst case (at each cell, we have at most 3 choices).
 * Space Complexity: O(N^2) for the recursion stack and visited grid.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Direction arrays for Down, Left, Right, Up
const int dr[] = {1, 0, 0, -1};
const int dc[] = {0, -1, 1, 0};
const char dir[] = {'D', 'L', 'R', 'U'};

class Solution {
private:
    void dfs(int r, int c, int n, const vector<vector<int>>& grid, vector<vector<int>>& vis, string path, vector<string>& ans) {
        // Reached destination
        if (r == n - 1 && c == n - 1) {
            ans.push_back(path);
            return;
        }
        vis[r][c] = 1;
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            bool isValid = nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 1 && !vis[nr][nc];
            if (isValid) dfs(nr, nc, n, grid, vis, path + dir[k], ans);
        }
        vis[r][c] = 0;
    }

public:
    vector<string> findPath(const vector<vector<int>>& grid) {
        vector<string> ans;
        int n = grid.size();
        if (n == 0 || grid[0][0] == 0 || grid[n - 1][n - 1] == 0) {
            return ans;
        }
        vector<vector<int>> vis(n, vector<int>(n, 0));
        dfs(0, 0, n, grid, vis, "", ans);
        sort(ans.begin(), ans.end());
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    Solution solver;
    vector<string> paths = solver.findPath(grid);

    if (paths.empty()) {
        cout << "No Path\n";
    } else {
        for (const string& path : paths) {
            cout << path << "\n";
        }
    }

    return 0;
}
