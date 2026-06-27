/**
 * Problem: N-ary Tree Basics
 * Link: N/A
 * Category: Tree
 * 
 * Description:
 * General N-ary tree representation using adjacency lists.
 * 
 * Logic/Approach:
 * DFS/BFS traversals on tree graph.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int LOG = 20;
vector<vector<int>> adj, up;
vector<int> depth, subtree;
int diameter = 0;

void dfsTraversal(int u, int par) {
    for (int v : adj[u]) {
        if (v != par) dfsTraversal(v, u);
    }
}

void bfs(int root) {
    queue<int> q;
    vector<int> vis(adj.size(), 0);
    q.push(root); vis[root] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = 1;
                q.push(v);
            }
        }
    }
}

int subtreeDFS(int u, int par) {
    subtree[u] = 1;
    for (int v : adj[u]) {
        if (v != par) subtree[u] += subtreeDFS(v, u);
    }
    return subtree[u];
}

int heightDFS(int u, int par) {
    int height = 0;
    for (int v : adj[u]) {
        if (v != par) height = max(height, heightDFS(v, u));
    }
    return height + 1;
}

int diameterDFS(int u, int par) {
    int mx1 = 0, mx2 = 0;
    for (int v : adj[u]) {
        if (v == par) continue;
        int h = diameterDFS(v, u);
        if (h > mx1) { mx2 = mx1; mx1 = h; }
        else if (h > mx2) mx2 = h;
    }
    diameter = max(diameter, mx1 + mx2);
    return mx1 + 1;
}

void binaryLiftingDFS(int u, int par) {
    up[u][0] = par;
    for (int j = 1; j < LOG; j++) {
        up[u][j] = (up[u][j - 1] != -1) ? up[up[u][j - 1]][j - 1] : -1;
    }
    for (int v : adj[u]) {
        if (v != par) {
            depth[v] = depth[u] + 1;
            binaryLiftingDFS(v, u);
        }
    }
}

int kthAncestor(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            u = up[u][j];
            if (u == -1) return -1;
        }
    }
    return u;
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    u = kthAncestor(u, depth[u] - depth[v]);
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
    }
    return up[u][0];
}

int distanceBetweenNodes(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    adj.resize(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v); adj[v].push_back(u);
    }
    depth.assign(n + 1, 0);
    subtree.assign(n + 1, 0);
    up.assign(n + 1, vector<int>(LOG, -1));
    
    dfsTraversal(1, -1);
    subtreeDFS(1, -1);
    int height = heightDFS(1, -1);
    diameterDFS(1, -1);
    binaryLiftingDFS(1, -1);

    cout << "Height = " << height << "\n";
    cout << "Diameter = " << diameter << "\n";

    int q;
    if (cin >> q) {
        while (q--) {
            int u, v;
            cin >> u >> v;
            cout << "LCA = " << lca(u, v) << "\n";
            cout << "Distance = " << distanceBetweenNodes(u, v) << "\n";
        }
    }
    return 0;
}
