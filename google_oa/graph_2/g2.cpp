
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
    for (int i = 0; i < n; i++) if (!vis[i]) dfs1(i, adj, vis, st);

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
