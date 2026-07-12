#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;

vector<int> buildCycle(int start, const vector<int>& parent) {
    vector<int> cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];

        if (cur == start)
            break;
    }

    return cycle;
}
/*=============================================================================
    1. UNDIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class UndirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<int> vis, parent;

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue; 
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v, u)) return true;
            } else { 
                startNode = v; endNode = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, 0), parent(n + 1, -1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v); adj[v].push_back(u);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    2. DIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class DirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<bool> vis, inPath;
    vector<int> parent;

    bool dfs(int u) {
        vis[u] = inPath[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v)) return true;
            } else if (inPath[v]) {   
                startNode = v; endNode = u;
                return true;
            }
        }
        inPath[u] = false;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, false), inPath(n + 1, false), parent(n + 1, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    3. NEGATIVE WEIGHT CYCLE FINDING (Bellman-Ford, 1-indexed)
=============================================================================*/
struct Edge { 
    int u, v; 
    ll w; 
};

vector<int> findNegativeCycle(int n, const vector<Edge>& edges) {
    vector<ll> dist(n + 1, 0); 
    vector<int> parent(n + 1, -1);
    int lastRelaxedNode = -1;
    
    for (int i = 1; i <= n; i++) {
        lastRelaxedNode = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w; 
                parent[e.v] = e.u; 
                lastRelaxedNode = e.v; 
            }
        }
    }
    if (lastRelaxedNode == -1) return {}; 
    
    // Backtrack N times to guarantee entry into the cycle structure
    for (int i = 0; i < n; i++) {
        lastRelaxedNode = parent[lastRelaxedNode];
    }
    return buildCycle(lastRelaxedNode, parent);
}

/*=============================================================================
    4. SHORTEST CYCLE LENGTH / GIRT (BFS, 0-indexed)
=============================================================================*/
int findShortestCycle(int n, const vector<vector<int>>& adj) {
    int minCycleLen = INT_MAX;
    
    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1), parent(n, -1); 
        queue<int> q;
        dist[src] = 0; 
        q.push(src);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1; 
                    parent[v] = u; 
                    q.push(v);
                } else if (parent[u] != v) { 
                    minCycleLen = min(minCycleLen, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return minCycleLen == INT_MAX ? -1 : minCycleLen;
}

/*=============================================================================
    5. IDENTIFY ALL CYCLIC DEPENDENCY NODES (Kahn's Peeling, 0-indexed)
=============================================================================*/
vector<int> getNodesInCycles(int n, const vector<vector<int>>& adj, vector<int>& indegree) {
    queue<int> q; 
    vector<bool> isAcyclic(n, false); 
    vector<int> cyclicNodes;
    
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    while (!q.empty()) {
        int u = q.front(); q.pop(); 
        isAcyclic[u] = true;
        for (int v : adj[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (!isAcyclic[i]) cyclicNodes.push_back(i);
    }
    return cyclicNodes;
}

int main() {
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);
    return 0;
}