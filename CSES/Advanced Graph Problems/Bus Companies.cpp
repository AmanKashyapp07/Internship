// Link: https://cses.fi/problemset/task/3158

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

using ll = long long;
const ll INF = numeric_limits<ll>::max();

struct Edge {
    int to;
    ll cost;
};

vector<ll> dijkstra(int n, int start, const vector<vector<Edge>>& graph) {
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currCost, u] = pq.top();
        pq.pop();

        if (currCost != dist[u]) continue;

        for (const auto& edge : graph[u]) {
            ll newCost = currCost + edge.cost;

            if (newCost < dist[edge.to]) {
                dist[edge.to] = newCost;
                pq.push({newCost, edge.to});
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int cities, companies;
    cin >> cities >> companies;

    vector<ll> ticket(companies + 1);

    for (int i = 1; i <= companies; i++)
        cin >> ticket[i];

    int totalNodes = cities + companies;
    vector<vector<Edge>> graph(totalNodes + 1);

    for (int i = 1; i <= companies; i++) {
        int stops;
        cin >> stops;

        int companyNode = cities + i;

        while (stops--) {
            int city;
            cin >> city;

            graph[city].push_back({companyNode, 0});
            graph[companyNode].push_back({city, ticket[i]});
        }
    }

    vector<ll> dist = dijkstra(totalNodes, 1, graph);

    for (int i = 1; i <= cities; i++) {
        cout << dist[i];
        if (i != cities) cout << " ";
    }
    cout << "\n";

    return 0;
}