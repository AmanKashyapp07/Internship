/**
 * LeetCode 787 - Cheapest Flights Within K Stops
 *
 * Problem:
 * Given n cities and a list of flights [from, to, price], return the cheapest
 * price from src to dst using at most k stops. Return -1 if no such route exists.
 *
 * Approach:
 * - Build an adjacency list.
 * - Use Dijkstra's algorithm with state:
 *      {cost, node, flightsUsed}
 * - Track the minimum cost for each (node, flightsUsed).
 * - A path can use at most (k + 1) flights.
 *
 * Time:  O(E log V)
 * Space: O(V * K)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights,int src, int dst, int k) {

        vector<vector<pair<int,int>>> adj(n);
        for (auto &f : flights)
            adj[f[0]].push_back({f[1], f[2]});

        int maxFlights = k + 1;
        vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX));
        using T = vector<int>; // {cost, node, flightsUsed}
        priority_queue<T, vector<T>, greater<T>> pq;

        dist[src][0] = 0;
        pq.push({0, src, 0});

        while (!pq.empty()) {
            auto v = pq.top();
            int cost = v[0], node = v[1], used = v[2];
            pq.pop();

            if (cost != dist[node][used]) continue;
            if (node == dst) return cost;
            if (used == maxFlights) continue;

            for (auto &[next, price] : adj[node]) {
                int newCost = cost + price;

                if (newCost < dist[next][used + 1]) {
                    dist[next][used + 1] = newCost;
                    pq.push({newCost, next, used + 1});
                }
            }
        }

        return -1;
    }
};