// Link: https://cses.fi/problemset/task/1131
#include <bits/stdc++.h>
using namespace std;

pair<int, int> bfs(int start, const vector<vector<int>>& g) {
    int n = g.size() - 1;
    vector<int> dist(n + 1, -1);
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
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    auto [A, _] = bfs(1, g);
    auto [B, diameter] = bfs(A, g);
    cout << diameter << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the diameter (length of the longest path) in an unweighted tree (CSES 1131).
// - Approach: 2-Pass BFS (Root $\to$ Farthest node A $\to$ Farthest node B).
// - Intuition: First BFS from any arbitrary node 1 finds one endpoint of the diameter; second BFS from node A finds the opposite endpoint B and the exact diameter.
// - Complexity: Time: O(N), Space: O(N).