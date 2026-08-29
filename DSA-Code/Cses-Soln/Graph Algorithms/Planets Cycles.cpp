// Link: https://cses.fi/problemset/task/1751
#include <bits/stdc++.h>
using namespace std;

struct FunctionalGraph {
    int n;
    vector<int> to, inDegree, ans;
    vector<vector<int>> rev;

    FunctionalGraph(int n) : n(n), to(n + 1), inDegree(n + 1, 0), ans(n + 1, -1), rev(n + 1) {}

    void addEdge(int u, int v) { to[u] = v; inDegree[v]++; rev[v].push_back(u); }

    void solve() {
        queue<int> q;
        for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (--inDegree[to[u]] == 0) q.push(to[u]);
        }

        vector<int> vis(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            if (inDegree[i] == 0 || vis[i]) continue;
            int len = 0, u = i;
            do { vis[u] = 1; len++; u = to[u]; } while (u != i);
            u = i;
            do { ans[u] = len; u = to[u]; } while (u != i);
        }

        auto dfsTree = [&](auto &self, int u) -> void {
            for (int v : rev[u]) {
                if (ans[v] != -1) continue;
                ans[v] = ans[u] + 1;
                self(self, v);
            }
        };

        for (int i = 1; i <= n; i++) if (ans[i] != -1) dfsTree(dfsTree, i);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    FunctionalGraph g(n);
    for (int i = 1; i <= n; i++) { int x; cin >> x; g.addEdge(i, x); }
    g.solve();
    for (int i = 1; i <= n; i++) cout << g.ans[i] << " ";
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find path length to repeat a planet state for every starting planet in a functional graph (CSES 1751).
// - Approach: Functional Graph Decomposition (Cycles via Kahn's + Trees via Reverse DFS).
// - Intuition: Nodes in a cycle have path length equal to cycle size; nodes in attached trees have length equal to distance to cycle plus cycle size.
// - Complexity: Time: O(N), Space: O(N).