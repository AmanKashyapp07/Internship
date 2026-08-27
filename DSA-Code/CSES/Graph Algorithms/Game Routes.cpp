// Link: https://cses.fi/problemset/task/1680
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    vector<int> inDegree(n + 1, 0);
    while (m--) {
        int u, v; cin >> u >> v; g[u].push_back(v); inDegree[v]++;
    }

    vector<long long> ways(n + 1, 0);
    ways[1] = 1;
    queue<int> q;
    for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            ways[v] = (ways[v] + ways[u]) % MOD;
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    cout << ways[n] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find number of ways to travel from game level 1 to level n modulo 10^9+7 in a DAG (CSES 1680).
// - Approach: Topological Sort DP (`ways[v] += ways[u]`).
// - Intuition: Processing nodes in topological order allows pushing path counts forward along directed edges.
// - Complexity: Time: O(V + E), Space: O(V + E).
