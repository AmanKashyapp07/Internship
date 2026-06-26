/**
 * Problem: Lexicographically Minimal Grid Path
 *
 * Description:
 * Find the lexicographically smallest path from top-left to bottom-right in an n x n grid of characters.
 *
 * Approach:
 * - Use a BFS/layer-by-layer traversal.
 * - At step `d` (from `0` to `2*n-2`), maintain a list of valid reachable cells.
 * - Find the minimum character among all neighbors to the right and down.
 * - Filter the next queue layer to only include cells containing this minimal character.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; if (!(cin >> n)) return 0;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];
    string ans = ""; ans += grid[0][0];
    vector<pair<int, int>> cur = {{0, 0}};
    vector<vector<bool>> vis(n, vector<bool>(n, false)); vis[0][0] = true;
    // Step-by-step BFS: path takes exactly 2*n - 2 steps to reach the destination
    for (int step = 0; step < 2 * n - 2; step++) {
        char best = 'z' + 1;
        for (auto [x, y] : cur) {
            if (x + 1 < n) best = min(best, grid[x + 1][y]);
            if (y + 1 < n) best = min(best, grid[x][y + 1]);
        }
        ans += best;
        vector<pair<int, int>> nxt;
        for (auto [x, y] : cur) {
            if (x + 1 < n && grid[x + 1][y] == best && !vis[x + 1][y]) {
                vis[x + 1][y] = true; nxt.push_back({x + 1, y});
            }
            if (y + 1 < n && grid[x][y + 1] == best && !vis[x][y + 1]) {
                vis[x][y + 1] = true; nxt.push_back({x, y + 1});
            }
        }
        cur = move(nxt); // Keep only the lexicographically optimal path endpoints
    }
    cout << ans << '\n';
}