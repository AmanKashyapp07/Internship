/**
 * LeetCode 2290 - Minimum Obstacle Removal to Reach Corner
 *
 * Description:
 * Given a 2D grid, return the minimum number of obstacles to remove to reach the bottom-right corner.
 *
 * Approach:
 * - Dijkstra's algorithm.
 * - Use a min-priority queue storing `{distance, {row, col}}` to find the shortest path.
 * - Distance is incremented by 1 when entering an obstacle cell.
 *
 * Time Complexity: O(m * n * log(m * n))
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

#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    int minimumObstacles(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // Initialize distances with a large number (acting as INF)
        vector<vector<int>> dist(m, vector<int>(n, 1e9));
        
        // Min-priority queue storing: {distance, {row, col}}
        priority_queue<pair<int, pair<int, int>>, 
                       vector<pair<int, pair<int, int>>>, 
                       greater<pair<int, pair<int, int>>>> pq;
        
        // Start from the top-left corner
        pq.push({0, {0, 0}});
        dist[0][0] = 0;
        
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        while(!pq.empty()) {
            auto [d, rc] = pq.top(); pq.pop();
            int r = rc.first, c = rc.second;
            
            // Because a min-heap guarantees we process the absolute shortest path first,
            // the first time we reach the bottom-right corner, it is guaranteed optimal.
            if (r == m - 1 && c == n - 1) return d;
            
            // If we found a shorter path to this cell already, skip processing
            if (d > dist[r][c]) continue;
            
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i], nc = c + dc[i];
                
                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    int newDist = d + grid[nr][nc]; 
                    
                    if (newDist < dist[nr][nc]) { 
                        dist[nr][nc] = newDist;
                        pq.push({newDist, {nr, nc}});
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