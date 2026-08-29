// Link: https://cses.fi/problemset/task/1196
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<vector<pair<int, long long>>> g(n + 1);
    while (m--) {
        int u, v; long long w; cin >> u >> v >> w;
        g[u].push_back({v, w});
    }

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    vector<int> cnt(n + 1, 0);
    vector<long long> ans;

    pq.push({0, 1});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (cnt[u] >= k) continue;
        cnt[u]++;
        if (u == n) {
            ans.push_back(d);
            if ((int)ans.size() == k) break;
        }
        for (auto [v, w] : g[u]) pq.push({d + w, v});
    }

    for (int i = 0; i < k; i++) cout << ans[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the k shortest route costs from city 1 to city n (CSES 1196).
// - Approach: K-Shortest Paths Dijkstra (`cnt[u]` pop tracker).
// - Intuition: Min-heap pops nodes in increasing path cost order; allowing each node to be popped up to k times yields k shortest paths.
// - Complexity: Time: O(K \cdot E \log(K \cdot E)), Space: O(K \cdot V + E).