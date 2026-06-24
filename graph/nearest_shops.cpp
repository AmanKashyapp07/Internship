/**
 * Problem: Multi-Source BFS (Nearest Shops)
 * Link: N/A
 * Category: Graph / BFS
 * 
 * Description:
 * Find the shortest distance from each node to the nearest shop node.
 * This is tricky because we need to find nearest shop for shop also, if we do normal BFS, dist[shop] = 0, but we need to find the second nearest shop for shop also. So we need to do multi-source BFS from all shops simultaneously.
 * 
 * Logic/Approach:
 * Multi-source BFS pushing all shop nodes into queue initially.
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 1e9;

vector<int> d1, d2;
vector<int> r1, r2;
queue<int> q;

void relax(int node, int dist, int root) {
    if (root == r1[node] || root == r2[node])
        return;

    if (dist < d1[node]) {
        d2[node] = d1[node];
        r2[node] = r1[node];

        d1[node] = dist;
        r1[node] = root;

        q.push(node);
    }
    else if (dist < d2[node]) {
        d2[node] = dist;
        r2[node] = root;

        q.push(node);
    }
} // this function relax is responsible for updating the distances and root shops for a given node, it checks if the new distance is better than the current nearest or second nearest distance, and if so it updates the distances and root shops accordingly, and also pushes the node into the queue to continue the BFS from that node, this way we are able to find the nearest and second nearest shops for all nodes in the graph efficiently using a multi-source BFS approach.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> shops(k);
    vector<bool> isShop(n + 1, false);

    for (int i = 0; i < k; i++) {
        cin >> shops[i];
        isShop[shops[i]] = true;
    }

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    d1.assign(n + 1, INF); // Distance to the nearest shop
    d2.assign(n + 1, INF); // Distance to the second nearest shop
    r1.assign(n + 1, 0); // Root shop for the nearest shop
    r2.assign(n + 1, 0); // Root shop for the second nearest shop

    for (int shop : shops) {
        d1[shop] = 0;
        r1[shop] = shop;
        q.push(shop);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            relax(v, d1[u] + 1, r1[u]);

            if (r2[u] != 0) // If there is a second nearest shop for u, try to relax with that as well
                relax(v, d2[u] + 1, r2[u]);
        }
    }

    for (int i = 1; i <= n; i++) {
        if (isShop[i]) {
            cout << (d2[i] == INF ? -1 : d2[i]) << ' ';
        } else {
            cout << (d1[i] == INF ? -1 : d1[i]) << ' ';
        }
    }

    cout << '\n';
    return 0;
}