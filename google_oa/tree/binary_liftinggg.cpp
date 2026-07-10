#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int LOG = 20;

vector<vector<int>> adj, up;
vector<int> depth;
vector<long long> cnt, ans;

void init(int n) {
    adj.assign(n + 1, {});
    up.assign(n + 1, vector<int>(LOG, -1));
    depth.assign(n + 1, 0);
    cnt.assign(n + 1, 0);
    ans.assign(n + 1, 0);
}

void dfs(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p; // first ancestor (parent)
    for (int j = 1; j < LOG; j++) {
        int prev = up[u][j - 1];
        if (prev != -1) up[u][j] = up[prev][j - 1];
    }
    for (int v : adj[u]) if (v != p) dfs(v, u, d + 1);
}

int lift(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) {
            u = up[u][j];
            if (u == -1) return -1; // if we try to lift beyond the root, return -1
        }
    } // lifting u in those positions where k has 1s in its binary representation
    return u;
}

int lca(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b); // always lift the deeper node
    a = lift(a, depth[a] - depth[b]); // bring a and b to the same depth
    if (a == b) return a;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]){
            a=up[a][j];
            b=up[b][j];
        }
    }

    return up[a][0];
}

void dfs2(int u, int p) {
    for (int v : adj[u]) if (v != p) dfs2(v, u), cnt[u] += cnt[v];
    ans[u] = cnt[u];
} // returns count of paths passing through each node after processing all queries
// cnt[u] = number of paths that pass through node u
void solve_company_queries_1() {
    int n, q;
    cin >> n >> q;

    init(n);

    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    dfs(1, -1, 0);

    while (q--) {
        int x, k;
        cin >> x >> k;
        cout << lift(x, k) << "\n";
    }
}

void solve_company_queries_2() {
    int n, q;
    cin >> n >> q;

    init(n);

    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    dfs(1, -1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << "\n";
    }
}

void solve_distance_queries() {
    int n, q;
    cin >> n >> q;

    init(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, -1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;

        int x = lca(a, b);
        cout << depth[a] + depth[b] - 2 * depth[x] << "\n";
    }
}

void solve_counting_paths() {
    int n, m;
    cin >> n >> m;

    init(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, -1, 0);
    while (m--) {
        int a, b;
        cin >> a >> b;
        int x = lca(a, b);
        cnt[a]++;
        cnt[b]++;
        cnt[x]--;

        if (up[x][0] != -1)
            cnt[up[x][0]]--;
    }

    dfs2(1, -1);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // solve_company_queries_1();
    // solve_company_queries_2();
    // solve_distance_queries();
    // solve_counting_paths();

    return 0;
}