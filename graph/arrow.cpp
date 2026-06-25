/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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
