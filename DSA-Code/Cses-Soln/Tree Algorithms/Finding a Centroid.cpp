// Link: https://cses.fi/problemset/task/2079
#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> g;
vector<int> sz;

int getSubtreeSizes(int u, int p) {
    sz[u] = 1;
    for (int v : g[u]) if (v != p) sz[u] += getSubtreeSizes(v, u);
    return sz[u];
}

int findCentroid(int u, int p) {
    for (int v : g[u]) {
        if (v != p && sz[v] > n / 2) return findCentroid(v, u);
    }
    return u;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    g.resize(n + 1); sz.resize(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    getSubtreeSizes(1, 0);
    cout << findCentroid(1, 0) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find a centroid of a tree (a node whose removal leaves no component of size > N/2) (CSES 2079).
// - Approach: Subtree size computation + Centroid DFS walk down heavy branches.
// - Intuition: If any neighbor subtree has size > N/2, walk into that neighbor; stopping node is guaranteed to be a valid centroid.
// - Complexity: Time: O(N), Space: O(N).