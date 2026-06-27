
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <utility>
#include <tuple>
#include <string>
#include <functional>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
/**
 * Problem: Disjoint Set Union (DSU) / Union-Find Data Structure
 * Keeps track of elements partitioned into disjoint subsets.
 * Provides near-constant time operations to find the representative of a set and union two sets together by size.
 * Complexity: O(alpha(N)) per operation where alpha is the inverse Ackermann function.
 */
class DSU {
    vector<int> parent, size;
public:
    DSU(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int findPar(int node) {
        if (parent[node] == node) return node;
        return parent[node] = findPar(parent[node]);
    }
    bool unionBySize(int u, int v) {
        u = findPar(u); v = findPar(v);
        if (u == v) return false;
        if (size[u] < size[v]) swap(u, v);
        parent[v] = u;
        size[u] += size[v];
        return true;
    }
};

/**
 * Problem: Edge Representation Structure
 * Standard representation of an edge containing source, destination, and edge weight.
 * Includes a comparison operator to allow sorting edges in ascending order of their weights.
 * Complexity: O(1) space, used extensively for Kruskal's Minimum Spanning Tree algorithm.
 */
struct Edge {
    int u, v, wt;
    bool operator<(const Edge& other) const { return wt < other.wt; }
};

/**
 * Problem: Minimum Spanning Tree (Kruskal's Algorithm)
 * Find a subset of edges connecting all vertices with minimum total weight without any cycles.
 * Uses Disjoint Set Union (DSU) to check connectivity after sorting all edges by weight.
 * Complexity: O(E log E) time due to sorting of edges.
 */
ll kruskalMST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    ll mstWeight = 0;
    for (auto& e : edges) {
        if (dsu.unionBySize(e.u, e.v)) mstWeight += e.wt;
    }
    return mstWeight;
}

/**
 * Problem: Minimum Spanning Tree (Prim's Algorithm)
 * Find a subset of edges connecting all vertices with minimum total weight starting from a single node.
 * Uses a priority queue to greedily grow the spanning tree one vertex at a time by adding the cheapest node.
 * Complexity: O(E log V) time with adjacency list representation.
 */
ll primMST(int n, vector<vector<pair<int, int>>>& adj) {
    vector<int> vis(n, 0);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, 0});
    ll mstWeight = 0;
    while (!pq.empty()) {
        auto [wt, node] = pq.top(); pq.pop();
        if (vis[node]) continue;
        vis[node] = 1;
        mstWeight += wt;
        for (auto [nbr, edgeWt] : adj[node]) {
            if (!vis[nbr]) pq.push({edgeWt, nbr});
        }
    }
    return mstWeight;
}

/**
 * Problem: Bipartite Graph Detection helper (BFS)
 * Check if a connected graph component can be colored using only two colors such that no two adjacent vertices share the same color.
 * Uses Breadth-First Search to alternate colors between levels and check for color conflicts.
 * Complexity: O(V + E) time and O(V) space.
 */
bool bipartiteBFS(int src, vector<vector<int>>& adj, vector<int>& color) {
    queue<int> q;
    q.push(src);
    color[src] = 0;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int nbr : adj[node]) {
            if (color[nbr] == -1) {
                color[nbr] = color[node] ^ 1;
                q.push(nbr);
            } else if (color[nbr] == color[node]) {
                return false;
            }
        }
    }
    return true;
}

bool Mcolour(int node, int m, vector<vector<int>>& adj, vector<int>& color) {
    if (node == adj.size()) return true;
    for (int c = 1; c <= m; c++) {
        bool canColor = true;
        for (int nbr : adj[node]) {
            if (color[nbr] == c) {
                canColor = false;
                break;
            }
        }
        if (canColor) {
            color[node] = c;
            if (Mcolour(node + 1, m, adj, color)) return true;
            color[node] = 0; // backtrack
        }
    }
    return false;
} // verifies whether we can color the graph with m colors or not with adjacent nodes not having the same color

/**
 * Problem: Complete Bipartite Graph Verification
 * Verify if the entire graph (possibly consisting of multiple components) is bipartite.
 * Iterates through all vertices and triggers bipartiteBFS on unvisited components to verify coloring viability.
 * Complexity: O(V + E) time complexity.
 */
bool isBipartite(int n, vector<vector<int>>& adj) {
    vector<int> color(n, -1);
    for (int i = 0; i < n; i++) {
        if (color[i] == -1) {
            if (!bipartiteBFS(i, adj, color)) return false;
        }
    }
    return true;
}

const int LOG = 20;
vector<vector<int>> up;
vector<int> depth;  

/**
 * Problem: Binary Lifting table construction
 * Precomputes 2^j-th ancestors for each node in a tree to facilitate fast kth ancestor and LCA queries.
 * Recursively populates the dynamic programming lookup array using parent links in a DFS manner.
 * Complexity: O(N log N) time and space.
 */
void buildBinaryLifting(int node, int parent, vector<vector<int>>& tree) {
    up[node][0] = parent;
    for (int j = 1; j < LOG; j++) {
        if (up[node][j - 1] != -1) {
            up[node][j] = up[up[node][j - 1]][j - 1];
        }
    }
    for (int child : tree[node]) {
        if (child == parent) continue;
        depth[child] = depth[node] + 1;
        buildBinaryLifting(child, node, tree);
    }
}

/**
 * Problem: Kth Ancestor retrieval
 * Find the k-th ancestor of a node in a tree utilizing precomputed binary lifting table.
 * Decomposes the number k into binary bits to lift the node iteratively by powers of 2.
 * Complexity: O(log N) time per query.
 */
int kthAncestor(int node, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            node = up[node][j];
            if (node == -1) return -1;
        }
    }
    return node;
}

/**
 * Problem: Lowest Common Ancestor (LCA)
 * Find the deepest node in a tree that is a common ancestor of two given vertices.
 * Aligns both nodes to the same depth using binary lifting, then lifts them together to find their junction.
 * Complexity: O(log N) time per LCA query.
 */
int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    u = kthAncestor(u, depth[u] - depth[v]);
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
}

/**
 * Problem: Kosaraju Step 1 (DFS order traversal)
 * Performs a standard depth-first search on the directed graph to order vertices by their completion times.
 * Pushes nodes onto a stack upon backtracking to construct topological-like sorting.
 * Complexity: O(V + E) time and O(V) space.
 */
void dfs1(int node, vector<vector<int>>& adj, vector<int>& vis, stack<int>& st) {
    vis[node] = 1;
    for (int nbr : adj[node]) {
        if (!vis[nbr]) dfs1(nbr, adj, vis, st);
    }
    st.push(node);
}

/**
 * Problem: Kosaraju Step 2 (DFS transpose traversal)
 * Performs depth-first search on the reversed graph to isolate individual strongly connected components.
 * Traverses reachable nodes on transpose edges starting from the source vertices.
 * Complexity: O(V + E) time and O(V) space.
 */
void dfs2(int node, vector<vector<int>>& rev, vector<int>& vis) {
    vis[node] = 1;
    for (int nbr : rev[node]) {
        if (!vis[nbr]) dfs2(nbr, rev, vis);
    }
}

/**
 * Problem: Strongly Connected Components (Kosaraju's Algorithm)
 * Find the number of subgraphs in a directed graph where every vertex is reachable from any other vertex in the subgraph.
 * Utilizes two passes of DFS, one on the original graph and another on the transposed graph.
 * Complexity: O(V + E) time and O(V + E) auxiliary space.
 */
int kosaraju(int n, vector<vector<int>>& adj) {
    stack<int> st;
    vector<int> vis(n, 0);
    for (int i = 0; i < n; i++) {
        if (!vis[i]) dfs1(i, adj, vis, st);
    }
    vector<vector<int>> rev(n);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) rev[v].push_back(u);
    }
    fill(vis.begin(), vis.end(), 0);
    int sccCount = 0;
    while (!st.empty()) {
        int node = st.top(); st.pop();
        if (!vis[node]) {
            sccCount++;
            dfs2(node, rev, vis);
        }
    }
    return sccCount;
}

vector<int> tin, low;
int timer;
vector<pair<int, int>> bridges;

/**
 * Problem: Bridge Detection in Undirected Graph (Tarjan's algorithm)
 * Find all edges in an undirected graph whose deletion increases the count of connected components.
 * Utilizes DFS to compute entry times (tin) and lowest reachable times (low) to detect bridges when low[nbr] > tin[node].
 * Complexity: O(V + E) time and O(V) space.
 */
void bridgeDFS(int node, int parent, vector<vector<int>>& adj, vector<int>& vis) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    for (int nbr : adj[node]) {
        if (nbr == parent) continue;
        if (!vis[nbr]) {
            bridgeDFS(nbr, node, adj, vis);
            low[node] = min(low[node], low[nbr]);
            if (low[nbr] > tin[node]) bridges.push_back({node, nbr});
        } else {
            low[node] = min(low[node], tin[nbr]);
        }
    }
} // basic dfs with low and tin values to find bridges in undirected graph

vector<int> articulation;

/**
 * Problem: Articulation Points Detection (Tarjan's algorithm)
 * Identify all vertices in an undirected graph whose removal disconnects the graph.
 * Computes DFS entry times and low links to identify nodes satisfying low[nbr] >= tin[node].
 * Complexity: O(V + E) time and O(V) space.
 */
void articulationDFS(int node, int parent, vector<vector<int>>& adj, vector<int>& vis) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    int children = 0;
    for (int nbr : adj[node]) {
        if (nbr == parent) continue;
        if (!vis[nbr]) {
            articulationDFS(nbr, node, adj, vis);
            low[node] = min(low[node], low[nbr]);
            if (parent != -1 && low[nbr] >= tin[node]) articulation.push_back(node); // if low[nbr] >= tin[node], then node is an articulation point, here, we do not check root node because it is handled separately, as root node can be articulation point if it has more than one child
            children++;
        } else {
            low[node] = min(low[node], tin[nbr]);
        }
    }
    if (parent == -1 && children > 1) articulation.push_back(node); // root node with more than one child is an articulation point
}
