// Link: https://cses.fi/problemset/task/1132
#include <bits/stdc++.h>
using namespace std;

pair<int, int> bfs(int start, const vector<vector<int>>& g, vector<int>& dist) {
    int n = g.size();
    dist.assign(n, -1);
    queue<int> q;
    dist[start] = 0; q.push(start);
    int farthest = start;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
                if (dist[v] > dist[farthest]) farthest = v;
            }
        }
    }
    return {farthest, dist[farthest]};
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; u--; v--;
        g[u].push_back(v); g[v].push_back(u);
    }

    vector<int> dummy(n), dist1(n), dist2(n);
    auto [A, _] = bfs(0, g, dummy);
    auto [B, maxD] = bfs(A, g, dist1);
    bfs(B, g, dist2);

    for (int i = 0; i < n; i++) cout << max(dist1[i], dist2[i]) << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each node, find the maximum distance to any other node in the tree (CSES 1132).
// - Approach: Diameter Endpoints BFS (Max distance to diameter endpoints A and B).
// - Intuition: The farthest node from any vertex in a tree must always be one of the two diameter endpoints A or B.
// - Complexity: Time: O(N), Space: O(N).