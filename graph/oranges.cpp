/**
 * LeetCode 994 - Rotting Oranges
 *
 * Description:
 * Given a grid containing fresh, rotten, or empty cells, return the minimum number of minutes
 * until all fresh oranges rot. If impossible, return -1.
 *
 * Approach:
 * - Multi-source BFS starting from all rotten orange coordinates.
 * - Keep track of fresh orange counts.
 * - Rot adjacent fresh oranges level-by-level and decrement the fresh orange counter.
 *
 * Time Complexity: O(R * C)
 * Space Complexity: O(R * C)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), fresh = 0, minutes = 0;
        queue<pair<int, int>> q;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 2) q.push({i, j}); // Multi-source BFS initialization
                else if (grid[i][j] == 1) fresh++;
            }
        }
        if (fresh == 0) return 0;
        int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
        while (!q.empty() && fresh > 0) {
            int sz = q.size(); minutes++;
            while (sz--) {
                auto [x, y] = q.front(); q.pop();
                for (int k = 0; k < 4; k++) {
                    int nx = x + dx[k], ny = y + dy[k];
                    if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == 1) {
                        grid[nx][ny] = 2; fresh--; q.push({nx, ny});
                    }
                }
            }
        }
        return fresh == 0 ? minutes : -1;
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
}