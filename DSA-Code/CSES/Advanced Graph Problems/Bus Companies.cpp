// Link: https://cses.fi/problemset/task/3158
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;
struct Edge { int to; long long cost; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int cities, companies; cin >> cities >> companies;
    vector<long long> ticket(companies + 1);
    for (int i = 1; i <= companies; i++) cin >> ticket[i];

    int totalNodes = cities + companies;
    vector<vector<Edge>> g(totalNodes + 1);
    for (int i = 1; i <= companies; i++) {
        int stops; cin >> stops;
        int compNode = cities + i;
        while (stops--) {
            int city; cin >> city;
            g[city].push_back({compNode, 0});
            g[compNode].push_back({city, ticket[i]});
        }
    }

    vector<long long> dist(totalNodes + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist[1] = 0; pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (const auto& e : g[u]) {
            if (d + e.cost < dist[e.to]) {
                dist[e.to] = d + e.cost;
                pq.push({dist[e.to], e.to});
            }
        }
    }

    for (int i = 1; i <= cities; i++) {
        cout << dist[i] << (i == cities ? '\n' : ' ');
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum travel cost from city 1 to all other cities using bus companies (CSES 3158).
// - Approach: Virtual Company Auxiliary Nodes + Single-Source Dijkstra Algorithm.
// - Intuition: Create virtual node for each company; entering company node costs 0 and exiting to any served city costs ticket price.
// - Complexity: Time: O((V + E) \log(V + E)), Space: O(V + E).