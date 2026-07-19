// Link: https://cses.fi/problemset/task/1687

#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;

vector<vector<int>> graph;
vector<vector<int>> up;

void dfs(int u, int parent) {
    up[u][0] = parent;

    for (int j = 1; j < LOG; j++) {
        if (up[u][j - 1] == -1)
            up[u][j] = -1;
        else
            up[u][j] = up[up[u][j - 1]][j - 1];
    }

    for (int v : graph[u]) if(v != parent)  dfs(v, u);
    
}

int kthAncestor(int x, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            x = up[x][j];
            if (x == -1) return -1;
        }
    }
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    graph.resize(n + 1);
    up.assign(n + 1, vector<int>(LOG, -1));

    for (int i = 2; i <= n; i++) {
        int boss;
        cin >> boss;
        graph[boss].push_back(i);
    }

    dfs(1, -1);

    while (q--) {
        int x, k;
        cin >> x >> k;
        cout << kthAncestor(x, k) << '\n';
    }

    return 0;
}