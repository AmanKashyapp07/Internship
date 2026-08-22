/**
 * LeetCode 2290 - Minimum Obstacle Removal to Reach Corner
 *
 * Description:
 * Given a 0-indexed 2D integer array grid, return the minimum number of obstacles to remove to reach the bottom-right corner.
 *
 * Approach:
 * - Solve using 0-1 BFS.
 * - Moving to an empty cell (0) has cost 0.
 * - Moving to an obstacle cell (1) has cost 1.
 * - Use a double-ended queue (deque): push 0-cost moves to the front, and 1-cost moves to the back.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

#include <vector>
#include <queue> // for std::deque

using namespace std;

class Solution {
public:
    int minimumObstacles(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // Initialize distances with a large number (acting as INF)
        vector<vector<int>> dist(m, vector<int>(n, 1e9));
        
        // Deque stores pairs of coordinates: {row, col}
        // We don't need to store the distance explicitly because dist[r][c] holds it
        deque<pair<int, int>> dq;
        
        // Start from the top-left corner
        dq.push_front({0, 0});
        dist[0][0] = 0;
        
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        while(!dq.empty()) {
            auto [r, c] = dq.front(); dq.pop_front();
            
            // The first time we pop the destination, it is guaranteed to have the minimum cost
            if (r == m - 1 && c == n - 1) return dist[r][c];
            
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i], nc = c + dc[i];
                
                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    int weight = grid[nr][nc]; // 0 if empty, 1 if obstacle
                    
                    if (dist[r][c] + weight < dist[nr][nc]) { 
                        dist[nr][nc] = dist[r][c] + weight;
                        
                        // 0-1 BFS Insertion Logic:
                        if (weight == 0) {
                            dq.push_front({nr, nc}); // 0-cost moves go to the front
                        } else {
                            dq.push_back({nr, nc});  // 1-cost moves go to the back
                        }
                    }
                }
            }
        }
        
        return dist[m - 1][n - 1];
    }
};