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
    int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
        vector<int> indegree(n + 1, 0);
        vector<vector<int>> graph(n + 1);

        for (const auto& relation : relations) {
            int u = relation[0];
            int v = relation[1];
            graph[u].push_back(v);
            indegree[v]++;
        }

        queue<int> q;
        vector<int> dp(n + 1, 0); // dp[i] will store the minimum time to complete task i

        for (int i = 1; i <= n; ++i) {
            if (indegree[i] == 0) {
                q.push(i);
                dp[i] = time[i - 1];
            }
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbor : graph[node]) {
                indegree[neighbor]--;
                dp[neighbor] = max(dp[neighbor], dp[node] + time[neighbor - 1]); // time[neighbour-1] is the time required to complete the current task and dp[node] is the time taken to complete all its prerequisites which is minimum time to complete the current task
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}
