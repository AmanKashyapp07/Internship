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

class Solution
{
public:
    int minCost(int maxTime, vector<vector<int>> &edges, vector<int> &passingFees)
    {
        int n = passingFees.size();
        const int INF = 1e9;

        vector<vector<pair<int, int>>> graph(n);

        for (auto &e : edges)
        {
            graph[e[0]].push_back({e[1], e[2]});
            graph[e[1]].push_back({e[0], e[2]});
        }

        // cost[node][time] = minimum cost to reach node in exactly 'time'
        vector<vector<int>> cost(n, vector<int>(maxTime + 1, INF));
        cost[0][0] = passingFees[0];

        priority_queue<
            tuple<int, int, int>,
            vector<tuple<int, int, int>>,
            greater<tuple<int, int, int>>>
            pq;

        // {cost, time, node}
        pq.push({passingFees[0], 0, 0});

        while (!pq.empty())
        {
            auto [currCost, currTime, node] = pq.top();
            pq.pop();

            if (currCost > cost[node][currTime])
                continue;

            if (node == n - 1)
                return currCost; // it will be surely the minimum cost to reach the destination node within maxTime since we are using a min-heap priority queue and we are exploring the least cost paths first.

            for (auto &[next, travelTime] : graph[node])
            {
                int newTime = currTime + travelTime;

                if (newTime > maxTime)
                    continue;

                int newCost = currCost + passingFees[next];

                if (newCost < cost[next][newTime]) // if we found a cheaper way to reach 'next' at 'newTime', update and push to the queue
                {
                    cost[next][newTime] = newCost; // Update the cost for reaching 'next' at 'newTime'
                    pq.push({newCost, newTime, next}); // Push the new state into the priority queue for further exploration
                }
            }
        }

        return -1;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}
