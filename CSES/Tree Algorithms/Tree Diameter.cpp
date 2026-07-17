// Link: https://cses.fi/problemset/task/1131

#include <bits/stdc++.h>
using namespace std;

pair<int, int> bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size() - 1;

    vector<int> dist(n + 1, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    int farthestNode = start;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] != -1) continue;

            dist[v] = dist[u] + 1;
            q.push(v);

            if (dist[v] > dist[farthestNode]) {
                farthestNode = v;
            }
        }
    }

    return {farthestNode, dist[farthestNode]};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    auto [A, _] = bfs(1, adj);
    auto [B, diameter] = bfs(A, adj);

    cout << diameter << '\n';

    return 0;
}