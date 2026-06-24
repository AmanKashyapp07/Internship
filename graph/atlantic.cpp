/**
 * Problem: Pacific Atlantic Water Flow (https://leetcode.com/problems/pacific-atlantic-water-flow/)
 * Find grid cells from which water can flow to both Pacific and Atlantic oceans.
 * Time: O(R * C) time, O(R * C) space.
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
