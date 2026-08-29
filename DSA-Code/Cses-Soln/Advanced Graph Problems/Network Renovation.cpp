// Link: https://cses.fi/problemset/task/1704
#include <bits/stdc++.h>
using namespace std;

void dfs(int u, int p, const vector<vector<int>>& g, vector<int>& leaves) {
    if (g[u].size() == 1) leaves.push_back(u);
    for (int v : g[u]) if (v != p) dfs(v, u, g, leaves);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    if (n == 1) { cout << 0 << "\n"; return 0; }

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }

    vector<int> leaves;
    dfs(1, -1, g, leaves);

    int totalLeaves = leaves.size(), k = (totalLeaves + 1) / 2, half = totalLeaves / 2;
    cout << k << "\n";
    for (int i = 0; i < k; i++) {
        cout << leaves[i] << " " << leaves[(i + half) % totalLeaves] << "\n";
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum new connections to make a tree 2-edge-connected (CSES 1704).
// - Approach: Leaf Pairing via Half-Split Index Shift ($k = \lceil L / 2 \rceil$).
// - Intuition: Gathering all tree leaves and pairing leaf $i$ with leaf $(i + \lfloor L/2 \rfloor) \pmod L$ ensures no bridge remains.
// - Complexity: Time: O(N), Space: O(N).