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
 * Problem: Breadth-First Search (BFS) Traversal
 * Given an unweighted graph and a starting node, visit all reachable nodes level by level.
 * It is commonly used to find the shortest path in unweighted graphs or perform level-order traversal.
 * Complexity: O(V + E) time, where V is vertices and E is edges.
 */
vector<int> bfs(int n, vector<vector<int>>& adj, int src) {
    vector<int> vis(n, 0), order;
    queue<int> q;
    q.push(src);
    vis[src] = 1;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        order.push_back(node);
        for (int nbr : adj[node]) {
            if (!vis[nbr]) {
                vis[nbr] = 1;
                q.push(nbr);
            }
        }
    }
    return order;
}

/**
 * Problem: Depth-First Search (DFS) Traversal
 * Given a graph and a starting node, traverse as deep as possible along each branch before backtracking.
 * It is highly useful for structural analysis, topological sorting, and finding path connectivity.
 * Complexity: O(V + E) time and O(V) space for recursive call stack.
 */
void dfs(int node, vector<vector<int>>& adj, vector<int>& vis) {
    vis[node] = 1;
    for (int nbr : adj[node]) {
        if (!vis[nbr]) dfs(nbr, adj, vis);
    }
}

/**
 * Problem: Count Connected Components in Undirected Graph
 * Find the total number of disconnected subgraphs in a given undirected graph structure.
 * Traverses all unvisited vertices sequentially and launches DFS/BFS to explore each complete component.
 * Complexity: O(V + E) time, ensuring each node and edge is examined at most once.
 */
int countComponents(int n, vector<vector<int>>& adj) {
    vector<int> vis(n, 0);
    int components = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            components++;
            dfs(i, adj, vis);
        }
    }
    return components;
}

/**
 * Problem: Topological Sort (Kahn's Algorithm)
 * Generate a linear ordering of vertices in a Directed Acyclic Graph (DAG) using indegrees.
 * Repeatedly processes vertices with indegree zero using a queue and reduces indegrees of neighbors.
 * Complexity: O(V + E) time, also implicitly detects cycles if topological size != V.
 */
vector<int> kahnTopo(int n, vector<vector<int>>& adj) {
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) indegree[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    vector<int> topo;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        topo.push_back(node);
        for (int nbr : adj[node]) {
            indegree[nbr]--;
            if (indegree[nbr] == 0) q.push(nbr);
        }
    }
    return topo;
}

/**
 * Problem: Shortest Path in Unweighted Graph
 * Calculate the shortest distance from a single source node to all other nodes in an unweighted graph.
 * Uses a queue-based Breadth-First Search (BFS) to guarantee optimal distance calculation per level.
 * Complexity: O(V + E) time and O(V) space for storing distance values.
 */
vector<int> shortestPathUnweighted(int n, vector<vector<int>>& adj, int src) {
    vector<int> dist(n, INF);
    queue<int> q;
    q.push(src);
    dist[src] = 0;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int nbr : adj[node]) {
            if (dist[node] + 1 < dist[nbr]) {
                dist[nbr] = dist[node] + 1;
                q.push(nbr);
            }
        }
    }
    return dist;
}

/**
 * Problem: Shortest Path with Non-Negative Weights (Dijkstra)
 * Find the shortest paths from a source vertex to all other vertices in a weighted graph.
 * Utilizes a priority queue to greedily relax edges starting from the closest unvisited node.
 * Complexity: O(E log V) time, where negative edge weights are not allowed.
 */
vector<ll> dijkstra(int n, vector<vector<pair<int, int>>>& adj, int src) {
    vector<ll> dist(n, LINF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, node] = pq.top(); pq.pop();
        if (d > dist[node]) continue;
        for (auto [nbr, wt] : adj[node]) {
            if (dist[node] + wt < dist[nbr]) {
                dist[nbr] = dist[node] + wt;
                pq.push({dist[nbr], nbr});
            }
        }
    }
    return dist;
}

/**
 * Problem: Shortest Path in a Directed Acyclic Graph (DAG)
 * Find the shortest path from a source node to all other nodes when the graph contains no cycles.
 * Obtains a topological ordering of the vertices first, then relaxes outgoing edges of each node sequentially.
 * Complexity: O(V + E) time, which is faster than Dijkstra for acyclic structures.
 */
vector<ll> shortestPathDAG(int n, vector<vector<pair<int, int>>>& adj, int src) {
    vector<vector<int>> dag(n);
    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u]) dag[u].push_back(v);
    }
    vector<int> topo = kahnTopo(n, dag);
    vector<ll> dist(n, LINF);
    dist[src] = 0;
    for (int node : topo) {
        if (dist[node] == LINF) continue;
        for (auto [nbr, wt] : adj[node]) {
            dist[nbr] = min(dist[nbr], dist[node] + wt);
        }
    }
    return dist;
}

/**
 * Problem: Single-Source Shortest Path with Negative Weights (Bellman-Ford)
 * Computes shortest distances from a single source to all vertices in a weighted graph containing negative edges.
 * Iteratively relaxes all E edges V-1 times to propagate shortest distance values.
 * Complexity: O(V * E) time complexity, and can handle negative weight edges.
 */
vector<ll> bellmanFord(int n, vector<vector<int>>& edges, int src) {
    vector<ll> dist(n, LINF);
    dist[src] = 0;
    for (int i = 1; i <= n - 1; i++) {
        for (auto& e : edges) {
            int u = e[0], v = e[1], wt = e[2];
            if (dist[u] != LINF && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }
    return dist;
}

/**
 * Problem: Negative Cycle Detection (Bellman-Ford check)
 * Verify if a weighted graph contains a cycle whose total edge weight sum is negative.
 * Runs an extra relaxation pass on all edges after V-1 iterations; any further change indicates a cycle.
 * Complexity: O(E) time when run after a full Bellman-Ford run has been completed.
 */
bool hasNegativeCycle(int n, vector<vector<int>>& edges, vector<ll>& dist) {
    for (auto& e : edges) {
        int u = e[0], v = e[1], wt = e[2];
        if (dist[u] != LINF && dist[u] + wt < dist[v]) return true;
    }
    return false;
}

/**
 * Problem: All-Pairs Shortest Path (Floyd-Warshall)
 * Calculate the shortest distances between every pair of vertices in a weighted directed graph.
 * Dynamic programming approach that systematically considers each vertex as an intermediate routing step.
 * Complexity: O(V^3) time and O(V^2) space, best suited for dense, small graphs.
 */
void floydWarshall(vector<vector<ll>>& dist) {
    int n = dist.size();
    for(int i = 0; i < n; i++) dist[i][i] = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                if(dist[j][i] != LINF && dist[i][k] != LINF) {
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);
                }
            }
        }
    }
}
