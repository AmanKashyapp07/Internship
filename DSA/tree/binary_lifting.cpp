/**
 * Problem: Company Queries II (Distance Queries)
 * Link: https://cses.fi/problemset/task/1688
 * Category: Tree Algorithms
 * 
 * Description:
 * Calculate LCA distance between two tree nodes.
 * 
 * Logic/Approach:
 * Precompute depth and binary lifting ancestors up[node][j].
 */

#include <iostream>
#include <vector>

using namespace std;

const int MAX_NODES = 200005;
const int LOG = 20; // Enough for up to 2^19 nodes

vector<int> adj[MAX_NODES];
int up[MAX_NODES][LOG];
int depth[MAX_NODES];

// 1. Precomputation DFS
void dfs(int u, int p, int d) {
    depth[u] = d; // d is the depth of node u from the root
    up[u][0] = p; // The 2^0 (1st) ancestor is the parent
    
    // Fill the rest of the row for node u
    for (int j = 1; j < LOG; j++) {
        up[u][j] = up[up[u][j-1]][j-1]; // The 2^j ancestor is the 2^(j-1) ancestor of the 2^(j-1) ancestor
    }
    
    // Process children
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

// 2. Finding the LCA using Binary Lifting
int get_lca(int u, int v) {
    // Ensure u is the deeper node to simplify the logic
    if (depth[u] < depth[v]) {
        swap(u, v);
    }
    
    // Phase 1: Level them out. Jump u up until it is at the same depth as v.
    int diff = depth[u] - depth[v];
    for (int j = LOG - 1; j >= 0; j--) {
        // If the j-th bit of diff is set, we need to jump 2^j steps
        if ((diff >> j) & 1) {
            u = up[u][j];
        }
    }
    
    // If they were on the same branch, v is the LCA
    if (u == v) {
        return u;
    }
    
    // Phase 2: Jump both up together. 
    // We jump as high as possible without them meeting.
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    } // this loop ensures that u and v are now direct children of the LCA, because we have moved them up until their ancestors diverge
    // phase 2 basically finds the highest ancestor of u and v that is not the same, which means their parents are the same, which is the LCA, by looping from the highest power of 2 down to 0 and checking if their ancestors at that level are different, if they are different we move both u and v up to those ancestors, if they are the same we do nothing and continue to the next lower power of 2, this way we ensure that we are moving them up in the tree as much as possible without them meeting, and when they meet we know that their parent is the LCA, their parent will always be LCA because we are moving them up in the tree and when they meet it means they have a common ancestor, and since we are moving them up in powers of 2, we are guaranteed to find the LCA in logarithmic time, which is the essence of binary lifting, and this is why we can find the LCA in O(log n) time after O(n log n) preprocessing time.
    
    // Now u and v are right below the LCA. One step up is the answer.
    return up[u][0];
}

int main() {
    // Fast I/O (Critical for passing time limits in competitive programming)
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, q;
    if (!(cin >> n >> q)) return 0;
    
    // Read the tree edges
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // Because the tree is undirected
    }
    
    // Initialize depth and build the 'up' table.
    // We assume 1-based indexing for nodes. Root is 1, and its parent is itself (1).
    dfs(1, 1, 0);
    
    // Process each query
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        
        int lca = get_lca(a, b);
        
        // The distance formula
        int distance = depth[a] + depth[b] - 2 * depth[lca];
        
        cout << distance << "\n";
    }
    
    return 0;
}