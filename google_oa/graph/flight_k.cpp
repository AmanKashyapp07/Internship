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
#include <queue>
#include <tuple>
#include <climits>
using namespace std;

int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k) {
    vector<vector<pair<int, int>>> adj(n);
    for (auto &f : flights) adj[f[0]].push_back({f[1], f[2]});
    int maxFlights = k + 1;
    vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    dist[src][0] = 0; pq.push({0, src, 0}); // state: {cost, node, flightsTaken}
    // State Dijkstra tracking distance by number of flights taken
    while (!pq.empty()) {
        auto [cost, node, flightsTaken] = pq.top(); pq.pop();
        if (cost > dist[node][flightsTaken]) continue;
        if (node == dst) return cost;
        for (auto &[next, price] : adj[node]) {
            if (flightsTaken + 1 <= maxFlights && cost + price < dist[next][flightsTaken + 1]) {
                dist[next][flightsTaken + 1] = cost + price;
                pq.push({cost + price, next, flightsTaken + 1});
            }
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<vector<int>> flights(m, vector<int>(3));
    for (int i = 0; i < m; i++) cin >> flights[i][0] >> flights[i][1] >> flights[i][2];
    int src, dest, k; cin >> src >> dest >> k;
    cout << findCheapestPrice(n, flights, src, dest, k) << '\n';
}