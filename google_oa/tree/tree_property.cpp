/**
 * Problem: Tree Diameter / Properties
 * Link: N/A
 * Category: Tree Algorithms
 * 
 * Description:
 * Demonstrates standard tree diameter finding via double BFS/DFS.
 * 
 * Logic/Approach:
 * Run DFS from arbitrary node to find furthest, then run DFS from furthest node to find diameter.
 */

#include <iostream>
#include <vector>
using namespace std;

using ll = long long;

int n;
vector<vector<int>> adj;
vector<ll> sz, dist;
// dist[u] = sum of distances from node u to all other nodes
// sz[u] = size of subtree rooted at u
void dfs1(int u, int p, int d) {
    dist[1] += d; 
    sz[u] = 1;
    for (int v : adj[u]) if(v!=p) dfs1(v, u, d + 1), sz[u] += sz[v];
} // build dist[1] and sz[u] for all u

void dfs2(int u, int p) {
    for(int v : adj[u]) if(v!=p) dist[v] = dist[u] - sz[v] + (n - sz[v]), dfs2(v, u);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    adj.assign(n + 1, {});
    sz.assign(n + 1, 0);
    dist.assign(n + 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs1(1, 0, 0); // initial parent is 0 (non-existent)
    dfs2(1, 0); // starting from root node 1, with parent 0 (non-existent)

    for (int i = 1; i <= n; i++)
        cout << dist[i] << " ";
    cout << '\n';

    return 0;
}