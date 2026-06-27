/**
 * Problem: CSES Tree Master Template
 * Category: Advanced Tree Algorithms (Graph Theory)
 * * Description:
 * A comprehensive CP template covering standard CSES Tree section patterns:
 * Diameter, Subtree Aggregations, Tree Distance (Rerooting DP), 
 * Binary Lifting (LCA / Ancestry), Path Queries, and Small-to-Large Merging.
 */

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;
using ll = long long;

// =========================================================================
// GLOBAL DATA STRUCTURES & ALIGNMENT
// =========================================================================
const int LOG = 20; // 2^20 is sufficient for N up to 1,000,000
int n;

vector<vector<int>> adj, up;
vector<int> depth, subtree, parentNode, color, distinctAnswer;
vector<ll> distSum, cnt;

// =========================================================================
// 1. FUNDAMENTALS, SUBTREES, & DIAMETER
// =========================================================================

/**
 * Standard baseline DFS traversal.
 */
void dfs(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) dfs(v, u);
    }
}

/**
 * Computes sizes of all subtrees.
 */
int subtreeDFS(int u, int p) {
    subtree[u] = 1;
    for (int v : adj[u]) {
        if (v != p) subtree[u] += subtreeDFS(v, u);
    }
    return subtree[u];
}

/**
 * Computes the diameter of the tree using maximum and second-maximum 
 * subtree heights down from every node.
 */
int diameter = 0;
int diameterDFS(int u, int p) {
    int mx1 = 0, mx2 = 0;
    for (int v : adj[u]) {
        if (v == p) continue;
        int h = diameterDFS(v, u);
        if (h > mx1) { 
            mx2 = mx1; 
            mx1 = h; 
        } else if (h > mx2) {
            mx2 = h;
        }
    }
    diameter = max(diameter, mx1 + mx2);
    return mx1 + 1; // Return maximum depth up to parent node
}

/**
 * Computes simple structural distance array from a source node.
 */
void dfsDistance(int u, int p, int d, vector<int>& dist) {
    dist[u] = d;
    for (int v : adj[u]) {
        if (v != p) dfsDistance(v, u, d + 1, dist);
    }
}

// =========================================================================
// 2. TREE REROOTING DP (Tree Distances I & II)
// =========================================================================

/**
 * Phase 1: Compute baseline subtree metrics and initial sum for the selected root.
 */
void rerootDFS1(int u, int p) {
    subtree[u] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        rerootDFS1(v, u);
        subtree[u] += subtree[v];
        distSum[u] += distSum[v] + subtree[v]; 
    }
}

/**
 * Phase 2: Shift the root dynamically to neighbor node `v`.
 * Transition formula: distSum[v] = distSum[u] - subtree[v] + (n - subtree[v])
 */
void rerootDFS2(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        distSum[v] = distSum[u] - subtree[v] + (n - subtree[v]);
        rerootDFS2(v, u);
    }
}

// =========================================================================
// 3. BINARY LIFTING (LCA, K-TH ANCESTOR, PATH DISTANCE)
// =========================================================================

/**
 * Builds the binary lifting table for O(log N) ancestry queries.
 */
void binaryLiftingDFS(int u, int p) {
    up[u][0] = p;
    for (int j = 1; j < LOG; j++) {
        up[u][j] = (up[u][j - 1] != -1) ? up[up[u][j - 1]][j - 1] : -1;
    }
    for (int v : adj[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            binaryLiftingDFS(v, u);
        }
    }
}

/**
 * Finds the k-th ancestor of node `u` in O(log k) using binary steps.
 */
int kthAncestor(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            u = up[u][j];
            if (u == -1) return -1;
        }
    }
    return u;
}

/**
 * Finds the Lowest Common Ancestor (LCA) of nodes `u` and `v` in O(log N).
 */
int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    // Equalize depth levels
    u = kthAncestor(u, depth[u] - depth[v]);
    if (u == v) return u;
    
    // Lift both nodes simultaneously right below their common intersection
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) { 
            u = up[u][j]; 
            v = up[v][j]; 
        }
    }
    return up[u][0];
}

/**
 * Computes exact number of edges between two nodes using their depths and LCA.
 */
int distanceBetweenNodes(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)];
}

/**
 * Accumulates difference array prefix values on paths (e.g., CSES Counting Paths).
 */
ll countingPathsDFS(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) cnt[u] += countingPathsDFS(v, u);
    }
    return cnt[u];
}

// =========================================================================
// 4. ADVANCED TECHNIQUE: SMALL-TO-LARGE MERGING (Distinct Colors Query)
// =========================================================================

/**
 * Computes distinct color counts inside each subtree.
 * Uses pointer swaps and move semantics to achieve O(N log^2 N) or O(N log N).
 */
set<int> distinctDFS(int u, int p) {
    set<int> cur;
    cur.insert(color[u]);
    
    for (int v : adj[u]) {
        if (v == p) continue;
        
        set<int> child = distinctDFS(v, u);
        
        // Small-to-Large Optimization: Swap state with largest child set
        if (child.size() > cur.size()) {
            swap(cur, child);
        }
        // Merge values from the smaller set into the larger one
        for (int x : child) {
            cur.insert(x);
        }
    }
    distinctAnswer[u] = cur.size();
    return cur; // Return ownership up to parent frame
}

// =========================================================================
// EXECUTIVE DRIVER CODE
// =========================================================================
int main() {
    // Fast Input/Output Pipeline
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (!(cin >> n)) return 0;
    
    // Dynamically sized internal memory arrays
    adj.resize(n + 1);
    depth.assign(n + 1, 0);
    subtree.assign(n + 1, 0);
    up.assign(n + 1, vector<int>(LOG, -1));
    distSum.assign(n + 1, 0);
    cnt.assign(n + 1, 0);
    parentNode.assign(n + 1, -1);
    color.assign(n + 1, 0);
    distinctAnswer.assign(n + 1, 0);
    
    // Parse Tree Topology
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v); 
        adj[v].push_back(u);
    }
    
    return 0;
}