// Link: https://cses.fi/problemset/task/1674
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g;
vector<int> sz;

void dfs(int u) {
    sz[u] = 1;
    for (int v : g[u]) {
        dfs(v);
        sz[u] += sz[v];
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    g.resize(n + 1); sz.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        int boss; cin >> boss; g[boss].push_back(i);
    }

    dfs(1);
    for (int i = 1; i <= n; i++) cout << (sz[i] - 1) << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Calculate the total number of subordinates (descendants) for each employee in a company tree (CSES 1674).
// - Approach: Subtree Size DFS Computation.
// - Intuition: The number of subordinates of employee u equals `subtree_size[u] - 1`.
// - Complexity: Time: O(N) single pass, Space: O(N).