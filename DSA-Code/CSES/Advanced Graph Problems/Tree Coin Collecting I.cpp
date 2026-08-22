// Link: https://cses.fi/problemset/task/3114

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>

using namespace std;

const int MAXN = 200005;
const int LOG = 19;     // 2^18 = 262144, enough for N = 200000
const int INF = 1e9;

int up[MAXN][LOG];
int info[MAXN][LOG];
int depth[MAXN];

// Multi-source BFS variables
int dist_to_coin[MAXN];
int c[MAXN];
vector<int> g[MAXN];

void build_binary_lifting(int n, int root) {
    // 1. Iterative BFS to avoid Deep Recursion / Stack Overflow
    queue<int> q;
    q.push(root);
    
    depth[root] = 0;
    up[root][0] = 0; // 0 means no ancestor
    info[root][0] = dist_to_coin[root];
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : g[u]) {
            if (v != up[u][0]) { // Don't go back to parent
                up[v][0] = u;
                depth[v] = depth[u] + 1;
                info[v][0] = dist_to_coin[v];
                q.push(v);
            }
        }
    }
    
    // 2. Build the Sparse Table (Binary Lifting)
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            int parent = up[i][j - 1];
            if (parent != 0) {
                up[i][j] = up[parent][j - 1];
                info[i][j] = min(info[i][j - 1], info[parent][j - 1]);
            } else {
                up[i][j] = 0;
                info[i][j] = info[i][j - 1];
            }
        }
    }
}

// Lifts 'u' up by 'k' steps and updates the min_val along the way
int lift(int u, int k, int &min_val) {
    for (int j = 0; j < LOG && u != 0; j++) {
        if (k & (1 << j)) {
            min_val = min(min_val, info[u][j]);
            u = up[u][j];
        }
    }
    return u;
}

int get_lca(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);
    
    int dummy = INF;
    a = lift(a, depth[a] - depth[b], dummy); // bring to same level
    
    if (a == b) return a;
    
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }
    return up[a][0];
}

// Minimum detour going from u straight up to ancestor 'anc'
int queryUp(int u, int anc) {
    if (u == anc) return info[u][0];
    
    int ans = INF;
    int k = depth[u] - depth[anc];
    
    lift(u, k, ans);
    return min(ans, info[anc][0]); // include the ancestor node itself
}

int queryPath(int u, int v) {
    int w = get_lca(u, v);
    return min(queryUp(u, w), queryUp(v, w));
}

int get_dist(int u, int v) {
    int w = get_lca(u, v);
    return depth[u] + depth[v] - 2 * depth[w];
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    queue<int> q_bfs;
    
    for (int i = 1; i <= n; i++) {
        dist_to_coin[i] = INF;
        cin >> c[i];
        if (c[i] == 1) {
            dist_to_coin[i] = 0;
            q_bfs.push(i);
        }
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // Step 1: Multi-Source BFS to compute nearest coin for each node
    while (!q_bfs.empty()) {
        int u = q_bfs.front();
        q_bfs.pop();

        for (int v : g[u]) {
            if (dist_to_coin[v] > dist_to_coin[u] + 1) {
                dist_to_coin[v] = dist_to_coin[u] + 1;
                q_bfs.push(v);
            }
        }
    }

    // Step 2: Initialize optimized Binary Lifting
    build_binary_lifting(n, 1);

    // Step 3: Process queries
    while (q--) {
        int a, b;
        cin >> a >> b;

        int path_dist = get_dist(a, b);
        int min_detour = queryPath(a, b);

        int ans = path_dist + 2 * min_detour;
        cout << ans << "\n";
    }

    return 0;
}