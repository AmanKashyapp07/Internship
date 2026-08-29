// Link: https://cses.fi/problemset/task/1130
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
vector<int> g[MAXN];
long long dp[MAXN][2]; // dp[u][0] = max matching in subtree u; dp[u][1] = sum of dp[v][0]

void dfs(int u, int p) {
    long long base = 0;
    for (int v : g[u]) {
        if (v != p) {
            dfs(v, u);
            base += dp[v][0];
        }
    }
    dp[u][1] = base;
    dp[u][0] = base;
    for (int v : g[u]) {
        if (v != p) {
            dp[u][0] = max(dp[u][0], base - dp[v][0] + dp[v][1] + 1);
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    dfs(1, 0);
    cout << dp[1][0] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the maximum size matching (maximum set of disjoint edges) in a tree (CSES 1130).
// - Approach: Subtree Tree DP (`dp[u][0]` includes edge to child, `dp[u][1]` excludes u).
// - Intuition: If node u is matched with child v, edge (u, v) adds +1 to matching while child v must not be matched internally (`dp[v][1]`).
// - Complexity: Time: O(N), Space: O(N).