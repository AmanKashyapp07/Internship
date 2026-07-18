// Link: https://cses.fi/problemset/task/1700

#include <bits/stdc++.h>
using namespace std;

struct TreeIsomorphism {

    int n;
    const vector<vector<int>>& g;
    vector<int> id;

    map<vector<int>, int>& mp;
    int& nxt;

    TreeIsomorphism(int nodes, const vector<vector<int>>& graph,
                    map<vector<int>, int>& mp,
                    int& nxt)
        : n(nodes), g(graph), id(nodes + 1), mp(mp), nxt(nxt) {}

    int dfs(int u, int p) {
        vector<int> child;

        for (int v : g[u]) {
            if (v == p) continue;
            child.push_back(dfs(v, u));
        }

        sort(child.begin(), child.end());

        if (!mp.count(child))
            mp[child] = nxt++;

        return id[u] = mp[child];
    }

    int getID(int root = 1) {
        return dfs(root, 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<vector<int>> g1(n + 1), g2(n + 1);

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            g1[u].push_back(v);
            g1[v].push_back(u);
        }

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            g2[u].push_back(v);
            g2[v].push_back(u);
        }

        map<vector<int>, int> mp;
        int nxt = 1;

        TreeIsomorphism T1(n, g1, mp, nxt);
        TreeIsomorphism T2(n, g2, mp, nxt);

        cout << (T1.getID() == T2.getID() ? "YES\n" : "NO\n");
    }

    return 0;
}