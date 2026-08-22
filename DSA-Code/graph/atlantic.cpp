#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    void bfs(queue<pair<int,int>>& q, vector<vector<bool>>& vis, vector<vector<int>>& h) {
        int m = h.size(), n = h[0].size();
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                if (vis[nr][nc]) continue;
                if (h[nr][nc] < h[r][c]) continue;

                vis[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();

        vector<vector<bool>> pac(m, vector<bool>(n, false));
        vector<vector<bool>> atl(m, vector<bool>(n, false));

        queue<pair<int,int>> qp, qa;

        for (int i = 0; i < m; i++) {
            qp.push({i, 0});
            pac[i][0] = true;

            qa.push({i, n - 1});
            atl[i][n - 1] = true;
        }

        for (int j = 0; j < n; j++) {
            qp.push({0, j});
            pac[0][j] = true;

            qa.push({m - 1, j});
            atl[m - 1][j] = true;
        }

        bfs(qp, pac, heights);
        bfs(qa, atl, heights);

        vector<vector<int>> ans;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pac[i][j] && atl[i][j])
                    ans.push_back({i, j});
            }
        }

        return ans;
    }
};