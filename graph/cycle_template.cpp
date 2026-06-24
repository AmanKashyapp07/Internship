#include <bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()

/*=============================================================================
    1. UNDIRECTED GRAPH CYCLE RECONSTRUCTION (CSES Round Trip)
=============================================================================*/
struct UndirectedCycle {
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<int> vis, parent; // vis: 0=unvisited, 1=visited

    UndirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, 0), parent(n + 1, -1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v); adj[v].push_back(u);
    }

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue; // Skip immediate parent in undirected graph
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v, u)) return true;
            } else { // Neighbor is visited and not parent -> Cycle found
                startNode = v; endNode = u;
                return true;
            }
        }
        return false;
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) {
                vector<int> cycle = {startNode};
                int cur = endNode;
                while (cur != startNode) {
                    cycle.push_back(cur);
                    cur = parent[cur];
                }
                cycle.push_back(startNode);
                reverse(all(cycle));
                return cycle;
            }
        }
        return {};
    }
};

struct DirectedCycle {
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<bool> vis, inPath;
    vector<int> parent;

    DirectedCycle(int n)
        : n(n),
          adj(n + 1),
          vis(n + 1, false),
          inPath(n + 1, false),
          parent(n + 1, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    bool dfs(int u) {
        vis[u] = true;
        inPath[u] = true;

        for (int v : adj[u]) {
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v)) return true;
            }
            else if (inPath[v]) {   // back edge
                startNode = v;
                endNode = u;
                return true;
            }
        }

        inPath[u] = false;
        return false;
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) {
                vector<int> cycle = {startNode};
                int cur = endNode;
                while (cur != startNode) {
                    cycle.push_back(cur);
                    cur = parent[cur];
                }
                cycle.push_back(startNode);
                reverse(all(cycle));

                return cycle;
            }
        }
        return {};
    }
};
/*=============================================================================
    3. NEGATIVE CYCLE RECONSTRUCTION (CSES Cycle Finding)
=============================================================================*/
struct Edge { int u, v; ll w; };

vector<int> negativeCycle(int n, vector<Edge>& edges) {
    vector<ll> dist(n + 1, 0); vector<int> parent(n + 1, -1);
    int x = -1;
    
    // Run Bellman-Ford N times. If Nth iteration relaxes an edge, a negative cycle exists.
    for (int i = 1; i <= n; i++) {
        x = -1;
        for (auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w; parent[e.v] = e.u; x = e.v; // x is the last relaxed node, which is guaranteed to be part of a negative cycle or can reach one.
            }
        }
    }
    if (x == -1) return {}; // No negative cycle
    
    // Backtrack N times to ensure x is strictly inside the cycle loop
    for (int i = 0; i < n; i++) x = parent[x]; // This is necessary because the node x may not be part of the cycle itself, but rather a node that can reach the cycle. By backtracking N times, we ensure that we land on a node that is guaranteed to be part of the cycle. by iterating N times, we are making sure that we are moving back through the graph enough times to reach a node that is part of the cycle, since the cycle can be at most N nodes long. This guarantees that we are inside the cycle when we start reconstructing it, at some point it will enter the cycle as we backtrack through the parent pointers. Once we are inside the cycle, we can reconstruct the cycle by following the parent pointers until we return to the starting node.
    
    vector<int> cycle;
    int start = x; cycle.push_back(start);
    x=parent[start];
    while (x != start) { cycle.push_back(x); x = parent[x]; }
    cycle.push_back(x); reverse(all(cycle));
    return cycle;
}



/*=============================================================================
    5. SHORTEST CYCLE LENGTH (LeetCode 2608) - 0-indexed
=============================================================================*/
int shortestCycleLength(int n, vector<vector<int>>& adj) {
    int ans = INT_MAX;
    // Multi-source BFS: Find shortest cycle passing through each possible root
    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1), parent(n, -1); queue<int> q;
        dist[src] = 0; q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1; parent[v] = u; q.push(v);
                } else if (parent[u] != v) { // Visited node and not immediate parent -> Cycle
                    ans = min(ans, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return ans == INT_MAX ? -1 : ans;
}

/*=============================================================================
    6. ALL NODES BELONGING TO CYCLES (Kahn's Algorithm) - 0-indexed
=============================================================================*/
vector<int> nodesInCycles(int n, vector<vector<int>>& adj, vector<int>& indeg) {
    queue<int> q; vector<int> alive(n, 1), cycleNodes;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    
    // Peeling layers: Nodes removed by Kahn's can NEVER be part of any cycle
    while (!q.empty()) {
        int u = q.front(); q.pop(); alive[u] = 0;
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    // Remaining unpeeled nodes are locked inside cyclic dependencies
    for (int i = 0; i < n; i++) if (alive[i]) cycleNodes.push_back(i);
    return cycleNodes;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    return 0;
}