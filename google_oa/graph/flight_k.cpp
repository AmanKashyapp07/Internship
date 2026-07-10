/**
 * LeetCode 787 - Cheapest Flights Within K Stops
 *
 * Description:
 * Find the cheapest price from src to dst with at most k stops.
 *
 * Approach:
 * - Dijkstra's algorithm modified to track the stops/flights count.
 * - Store the minimum distance and stop count in a min-heap priority queue.
 * - Keep a tracking array for minimum stops to prune sub-optimal paths.
 *
 * Time Complexity: O(E * log(V))
 * Space Complexity: O(V * K)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

using namespace std;

class Solution
{
public:
    int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
    {
        vector<vector<pair<int, int>>> adj(n);

        for (auto &f : flights)
        {
            adj[f[0]].push_back({f[1], f[2]});
        }

        int maxFlights = k + 1; // as only K stops are allowed, we can take at most K+1 flights

        vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX)); // dist[node][flightsTaken] = minimum cost to reach node with flightsTaken flights

        priority_queue<
            tuple<int, int, int>,
            vector<tuple<int, int, int>>,
            greater<>>
            pq;

        dist[src][0] = 0;
        pq.push({0, src, 0}); // {cost, node, flightsTaken}

        while (!pq.empty())
        {
            auto [cost, node, flightsTaken] = pq.top();
            pq.pop();
            if (cost > dist[node][flightsTaken]) continue; // just discard if we already found a better way to reach this node with flightsTaken flights
            if (node == dst) return cost; // if we reached destination, return the cost

            for (auto &[next, price] : adj[node])
            {
                if (flightsTaken + 1 > maxFlights) continue;

                int newCost = cost + price;

                if (newCost < dist[next][flightsTaken + 1])
                {
                    dist[next][flightsTaken + 1] = newCost;
                    pq.push({newCost, next, flightsTaken + 1});
                }
            }
        }

        return -1;
    }
};