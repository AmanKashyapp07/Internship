// Link: https://cses.fi/problemset/task/1701
#include <bits/stdc++.h>
using namespace std;

map<vector<int>, int> mp;
int nxt = 1;

vector<int> findCenters(int n, const vector<vector<int>>& g) {
    if (n == 1) return {1};
    vector<int> deg(n + 1), leaves;
    for (int i = 1; i <= n; i++) if ((deg[i] = g[i].size()) == 1) leaves.push_back(i);
    while (n > 2) {
        n -= leaves.size();
        vector<int> nxtLeaves;
        for (int u : leaves) {
            for (int v : g[u]) if (--deg[v] == 1) nxtLeaves.push_back(v);
        }
        leaves = nxtLeaves;
    }
    return leaves;
}

int dfs(int u, int p, const vector<vector<int>>& g) {
    vector<int> child;
    for (int v : g[u]) if (v != p) child.push_back(dfs(v, u, g));
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

    vector<int> c1 = findCenters(n, g1), c2 = findCenters(n, g2);
    mp.clear(); nxt = 1;

    bool iso = false;
    for (int r1 : c1) {
        for (int r2 : c2) {
            if (dfs(r1, 0, g1) == dfs(r2, 0, g2)) { iso = true; break; }
        }
        if (iso) break;
    }
    cout << (iso ? "YES\n" : "NO\n");
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) solve();
    return 0;
}

// Interview Explanation:
// - Problem Statement: Determine if two UNROOTED trees are isomorphic (CSES 1701).
// - Approach: Tree Centroid/Center Finding (Leaf Peeling) + Rooted Subtree Canonical Encoding.
// - Intuition: An unrooted tree has 1 or 2 centers; finding centers reduces unrooted tree isomorphism to rooted tree canonical encoding.
// - Complexity: Time: O(N \log N), Space: O(N).