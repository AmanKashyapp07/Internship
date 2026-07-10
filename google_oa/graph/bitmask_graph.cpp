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


/**
 * LeetCode 847. Shortest Path Visiting All Nodes
 *
 * Idea:
 * BFS on state = (current node, visited mask).
 *
 * State:
 *   node -> current position
 *   mask -> bitmask of visited nodes
 *
 * Start BFS from every node (multi-source BFS).
 * Each move updates:
 *   newMask = mask | (1 << neighbor)
 *
 * The first time we reach mask == (1<<n)-1,
 * we have visited every node in the shortest possible path.
 *
 * Time:  O((V + E) * 2^V)
 * Space: O(V * 2^V)
 */

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();

        if (n == 1) return 0;

        int allVisited = (1 << n) - 1;

        queue<pair<int, int>> q;                  // {node, mask}
        vector<vector<bool>> vis(n, vector<bool>(1 << n, false));

        // Multi-source BFS
        for (int i = 0; i < n; i++) {
            int mask = 1 << i;
            q.push({i, mask});
            vis[i][mask] = true;
        }

        int steps = 0;

        while (!q.empty()) {
            int sz = q.size();

            while (sz--) {
                auto [u, mask] = q.front();
                q.pop();

                if (mask == allVisited)
                    return steps;

                for (int v : graph[u]) {
                    int newMask = mask | (1 << v);

                    if (vis[v][newMask])
                        continue;

                    vis[v][newMask] = true;
                    q.push({v, newMask});
                }
            }

            steps++;
        }

        return -1;
    }
};