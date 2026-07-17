// Link: https://cses.fi/problemset/task/1751

#include <bits/stdc++.h>
using namespace std;

struct FunctionalGraph {
    int n;
    vector<int> to, indegree, ans;
    vector<vector<int>> rev;

    FunctionalGraph(int n) : n(n) {
        to.resize(n + 1);
        indegree.assign(n + 1, 0);
        ans.assign(n + 1, -1);
        rev.resize(n + 1);
    }

    void addEdge(int u, int v) {
        to[u] = v;
        indegree[v]++;
        rev[v].push_back(u);
    }

    void findCycles() {
        queue<int> q;

        for (int i = 1; i <= n; i++)
            if (indegree[i] == 0)
                q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            int v = to[u];
            if (--indegree[v] == 0)
                q.push(v);
        }

        vector<int> vis(n + 1);

        for (int i = 1; i <= n; i++) {
            if (indegree[i] == 0 || vis[i]) continue;

            int len = 0;
            int u = i;

            do {
                vis[u] = 1;
                len++;
                u = to[u];
            } while (u != i);

            u = i;
            do {
                ans[u] = len;
                u = to[u];
            } while (u != i);
        }
    }

    void dfs(int u) {
        for (int v : rev[u]) {
            if (ans[v] != -1) continue; // if already visited, skip
            ans[v] = ans[u] + 1;
            dfs(v);
        }
    }

    void solve() {
        findCycles();

        for (int i = 1; i <= n; i++)
            if (ans[i] != -1)
                dfs(i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    FunctionalGraph g(n);

    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        g.addEdge(i, x);
    }

    g.solve();

    for (int i = 1; i <= n; i++)
        cout << g.ans[i] << " ";
}