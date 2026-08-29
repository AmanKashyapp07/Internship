// Link: https://cses.fi/problemset/task/1139
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> g;
vector<int> color, ans;

set<int>* dfs(int u, int p) {
    set<int>* cur = new set<int>();
    cur->insert(color[u]);

    for (int v : g[u]) {
        if (v == p) continue;
        set<int>* child = dfs(v, u);
        if (cur->size() < child->size()) swap(cur, child);
        for (int x : *child) cur->insert(x);
        delete child;
    }
    ans[u] = cur->size();
    return cur;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    g.resize(n + 1); color.resize(n + 1); ans.resize(n + 1);
    for (int i = 1; i <= n; i++) cin >> color[i];
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    set<int>* rootSet = dfs(1, 0);
    delete rootSet;

    for (int i = 1; i <= n; i++) cout << ans[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the number of distinct color values in the subtree of every node (CSES 1139).
// - Approach: Small-to-Large Merging (DSU on Tree / Sack Algorithm).
// - Intuition: Always swap child set into parent set if child set is larger; merging smaller into larger guarantees $O(N \log^2 N)$ overall time.
// - Complexity: Time: O(N \log^2 N), Space: O(N \log N).