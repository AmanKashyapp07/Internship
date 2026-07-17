#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

vector<int> adj[N];
int color[N];
int ans[N];

set<int>* dfs(int u, int p) {
    set<int>* cur = new set<int>();
    cur->insert(color[u]);

    for (int v : adj[u]) {
        if (v == p) continue;

        set<int>* child = dfs(v, u);

        if (cur->size() < child->size())
            swap(cur, child);

        for (int x : *child)
            cur->insert(x);

        delete child;
    }

    ans[u] = cur->size();
    return cur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> color[i];

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    set<int>* root = dfs(1, 0);
    delete root;

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";

    cout << '\n';
}