/**
 * CSES - K Shortest Routes
 *
 * Problem:
 * Find the k shortest route costs from city 1 to city n.
 * A route may visit the same city multiple times.
 *
 * Approach:
 * - Run a modified Dijkstra.
 * - Each node can be popped from the priority queue at most k times.
 * - The first k times the destination is popped are the answers.
 *
 * Time:  O(kE log E)
 * Space: O(kE)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

using ll = long long;
using pii = pair<ll,int>;

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<pair<int,int>>> adj(n + 1);

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    priority_queue<pii, vector<pii>, greater<>> pq; // storing {cost, node} in min-heap
    vector<int> cnt(n + 1);

    pq.push({0, 1});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if (cnt[u] == k) continue;
        cnt[u]++;

        if (u == n)
            cout << cost << " ";

        for (auto &[v, w] : adj[u])
            pq.push({cost + w, v});
    }
}