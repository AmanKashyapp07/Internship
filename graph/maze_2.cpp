
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
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();

        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        priority_queue<
            pair<int, pair<int, int>>,
            vector<pair<int, pair<int, int>>>,
            greater<>
        > pq;

        dist[start[0]][start[1]] = 0;
        pq.push({0, {start[0], start[1]}});

        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        while (!pq.empty()) {
            auto [d, cell] = pq.top();
            pq.pop();

            auto [r, c] = cell;

            if (d > dist[r][c]) continue;

            for (int k = 0; k < 4; k++) {
                int nr = r;
                int nc = c;
                int steps = 0;

                while (nr + dr[k] >= 0 && nr + dr[k] < m &&
                       nc + dc[k] >= 0 && nc + dc[k] < n &&
                       maze[nr + dr[k]][nc + dc[k]] == 0) {
                    nr += dr[k];
                    nc += dc[k];
                    steps++;
                }

                if (dist[nr][nc] > d + steps) {
                    dist[nr][nc] = d + steps;
                    pq.push({dist[nr][nc], {nr, nc}});
                }
            }
        }

        int ans = dist[destination[0]][destination[1]];
        return ans == INT_MAX ? -1 : ans;
    }
};