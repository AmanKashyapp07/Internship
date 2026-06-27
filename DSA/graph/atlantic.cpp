/**
 * LeetCode 417 - Pacific Atlantic Water Flow
 *
 * Description:
 * There is an m x n rectangular island that borders both the Pacific Ocean (top and left)
 * and Atlantic Ocean (bottom and right). Water can flow in 4 directions to neighboring cells
 * of equal or lower height. Return a list of grid coordinates where water can flow to both oceans.
 *
 * Approach:
 * - Perform Multi-source BFS/DFS starting from the boundaries.
 * - Run one traversal starting from the Pacific boundaries (row 0, col 0) to find all cells that can flow into the Pacific.
 * - Run another traversal starting from the Atlantic boundaries (row m-1, col n-1) to find all cells that can flow into the Atlantic.
 * - Water flows "uphill" from the oceans (neighbor height >= current height).
 * - The intersection of reachable cells from both oceans is the answer.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

#include <queue>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pac(m, vector<bool>(n, false)), atl(m, vector<bool>(n, false));
        queue<pair<int,int>> qp, qa;
        for (int i = 0; i < m; i++) {
            qp.push({i, 0}); pac[i][0] = true;
            qa.push({i, n - 1}); atl[i][n - 1] = true;
        }
        for (int j = 0; j < n; j++) {
            qp.push({0, j}); pac[0][j] = true;
            qa.push({m - 1, j}); atl[m - 1][j] = true;
        }
        int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
        auto bfs = [&](queue<pair<int,int>>& q, vector<vector<bool>>& vis) {
            // Multi-source BFS moving to equal or higher elevations
            while (!q.empty()) {
                auto [r, c] = q.front(); q.pop();
                for (int k = 0; k < 4; k++) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n && !vis[nr][nc] && heights[nr][nc] >= heights[r][c]) {
                        vis[nr][nc] = true; q.push({nr, nc});
                    }
                }
            }
        };
        bfs(qp, pac); bfs(qa, atl);
        vector<vector<int>> ans;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pac[i][j] && atl[i][j]) ans.push_back({i, j}); // Intersection of reachability
        return ans;
    }
};