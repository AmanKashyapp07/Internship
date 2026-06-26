/**
 * LeetCode 1368 - Minimum Cost to Make at Least One Valid Path in a Grid
 *
 * Description:
 * Given an m x n grid, each cell has a sign pointing right (1), left (2), down (3), or up (4).
 * You can modify the sign on a cell with cost 1.
 * Return the minimum cost to make at least one valid path from (0,0) to (m-1,n-1).
 *
 * Approach:
 * - Solve using 0-1 Breadth-First Search (BFS) / Dijkstra's algorithm.
 * - Moving in the preferred direction of the current cell has cost 0.
 * - Moving in any other of the 3 directions has cost 1.
 * - Use a double-ended queue (deque): push 0-cost moves to the front, and 1-cost moves to the back.
 * - Keep track of minimum costs in a 2D distance array.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution {
public:
    int minCost(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        vector<vector<int>> dist(m, vector<int>(n, INF)); 
        deque<pair<int, int>> dq;
        
        dist[0][0] = 0;
        dq.push_back({0, 0});
        
        // Match directions cleanly with 1-based indexing from problem statement:
        // 1: Right, 2: Left, 3: Down, 4: Up
        int dr[] = {0, 0, 0, 1, -1};
        int dc[] = {0, 1, -1, 0, 0};
        
        while(!dq.empty()) {
            auto [r, c] = dq.front(); dq.pop_front();
            
            if (r == m - 1 && c == n - 1) return dist[r][c];
            
            int current_dir = grid[r][c];
            
            for (int i = 1; i <= 4; i++) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                
                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    int weight = (current_dir == i) ? 0 : 1; // 0 if moving in the preferred direction, 1 otherwise
                    
                    if (dist[r][c] + weight < dist[nr][nc]) {
                        dist[nr][nc] = dist[r][c] + weight;
                        
                        // 0-1 BFS placement logic
                        if (weight == 0) {
                            dq.push_front({nr, nc});
                        } else {
                            dq.push_back({nr, nc});
                        }
                    }
                }
            }
        }
        
        return dist[m - 1][n - 1];
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}