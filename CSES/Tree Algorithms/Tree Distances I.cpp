// Link: https://cses.fi/problemset/task/1132

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

pair<int, int> bfs(int start, const vector<vector<int>>& graph) {
    int n = graph.size();

    vector<int> dist(n, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    int farthest = start;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);

                if (dist[v] > dist[farthest]) {
                    farthest = v;
                }
            }
        }
    }

    return {farthest, dist[farthest]};
}

void calculateDistances(int start, const vector<vector<int>>& graph, vector<int>& dist) {
    int n = graph.size();
    dist.assign(n, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> graph(n);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        --a;
        --b;

        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    auto [u, _] = bfs(0, graph);
    auto [v, diameter] = bfs(u, graph);
    vector<int> dist1(n, -1);
    vector<int> dist2(n, -1);

    calculateDistances(u, graph, dist1);
    calculateDistances(v, graph, dist2);
    for(int i=0; i<n; i++) {
        cout << max(dist1[i], dist2[i]) << ' ';
    }
    cout << '\n';
}