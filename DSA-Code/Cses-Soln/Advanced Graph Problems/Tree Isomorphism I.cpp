// Link: https://cses.fi/problemset/task/1700
#include <bits/stdc++.h>
using namespace std;

map<vector<int>, int> mp;
int nxt = 1;

int dfs(int u, int p, const vector<vector<int>>& g) {
    vector<int> child;
    for (int v : g[u]) {
        if (v != p) child.push_back(dfs(v, u, g));
    }
    sort(child.begin(), child.end());
    if (!mp.count(child)) mp[child] = nxt++;
    return mp[child];
}

void solve() {
    int n; cin >> n;
    vector<vector<int>> g1(n + 1), g2(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g1[u].push_back(v); g1[v].push_back(u);
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g2[u].push_back(v); g2[v].push_back(u);
    }

    mp.clear(); nxt = 1;
    cout << (dfs(1, 0, g1) == dfs(1, 0, g2) ? "YES\n" : "NO\n");
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) solve();
    return 0;
}

// Interview Explanation:
// - Problem Statement: Determine if two rooted trees at node 1 are isomorphic (CSES 1700).
// - Approach: Subtree Canonical Encoding via Hashing / Integer Canonical Mapping.
// - Intuition: Recursively assign unique integer IDs to sorted child ID tuples; equal root IDs imply tree structural equivalence.
// - Complexity: Time: O(N \log N), Space: O(N).