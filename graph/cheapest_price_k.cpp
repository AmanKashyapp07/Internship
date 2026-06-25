#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
{
    vector<vector<pair<int, int>>> adj(n);

    for (auto &f : flights)
    {
        adj[f[0]].push_back({f[1], f[2]});
    }

    int maxFlights = k + 1;

    vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX));

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

        if (cost > dist[node][flightsTaken])
            continue;

        if (node == dst)
            return cost;

        for (auto &[next, price] : adj[node])
        {
            if (flightsTaken + 1 > maxFlights)
                continue;

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