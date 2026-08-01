# 🕸️ Master Graph Patterns Guide - Part 2: Advanced & OA Patterns

> **The definitive reference for Advanced Graph Algorithms: SCC, Bridges, Tree LCA/Lifting, Flow, Matching, 2-SAT, and CP Techniques for Tech Interviews and High-Tier OAs.**

---

## 📋 Table of Contents (Part 2)
1. [Pattern 7: Strongly Connected Components (SCC) & 2-SAT](#pattern-7-strongly-connected-components-scc--2-sat)
2. [Pattern 8: Bridges & Articulation Points (Tarjan's Low-Link Algorithm)](#pattern-8-bridges--articulation-points-tarjans-low-link-algorithm)
3. [Pattern 9: Trees - Binary Lifting, LCA, and Path Queries](#pattern-9-trees---binary-lifting-lca-and-path-queries)
4. [Pattern 10: Eulerian & Hamiltonian Paths](#pattern-10-eulerian--hamiltonian-paths)
5. [Pattern 11: Functional Graphs & Succession Graphs](#pattern-11-functional-graphs--succession-graphs)
6. [Pattern 12: Network Flow & Bipartite Matching](#pattern-12-network-flow--bipartite-matching)
7. [Pattern 13: Advanced OA & CP Graph Techniques (HLD, Centroids, Hashing)](#pattern-13-advanced-oa--cp-graph-techniques-hld-centroids-hashing)
8. [Master Graph Pattern Decision Matrix (Parts 1 & 2)](#master-graph-pattern-decision-matrix-parts-1--2)

---

## Pattern 7: Strongly Connected Components (SCC) & 2-SAT

### 🔍 Identification Signals
- **SCC**: Directed graph where every vertex in a component is reachable from every other vertex in that component.
- **Condensation DAG**: Shrink each SCC into a single node. The resulting graph is guaranteed to be a Directed Acyclic Graph (DAG)!
- **2-SAT**: Satisfiability of boolean formula with 2 variables per clause $(A \lor B) \land (\neg B \lor C) \dots$
  - Model clause $(A \lor B)$ as implication edges: $\neg A \implies B$ and $\neg B \implies A$.
  - Formula is satisfiable iff $x$ and $\neg x$ do **NOT** belong to the same SCC!

### 💻 Standard Templates

#### 1. Kosaraju's 2-Pass DFS Algorithm ($O(V + E)$)
```cpp
void dfs1Kosaraju(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& finishOrder) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfs1Kosaraju(v, adj, visited, finishOrder);
    }
    finishOrder.push_back(u); // Post-order push
}

void dfs2Kosaraju(int u, const vector<vector<int>>& adjRev, vector<int>& component, int compId) {
    component[u] = compId;
    for (int v : adjRev[u]) {
        if (component[v] == -1) dfs2Kosaraju(v, adjRev, component, compId);
    }
}

// Returns component ID for each node
pair<int, vector<int>> getSCCKosaraju(int n, const vector<vector<int>>& adj) {
    vector<vector<int>> adjRev(n + 1);
    for (int u = 1; u <= n; u++) {
        for (int v : adj[u]) adjRev[v].push_back(u);
    }
    
    vector<bool> visited(n + 1, false);
    vector<int> finishOrder;
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) dfs1Kosaraju(i, adj, visited, finishOrder);
    }
    
    vector<int> component(n + 1, -1);
    int compId = 0;
    for (int i = n - 1; i >= 0; i--) {
        int u = finishOrder[i];
        if (component[u] == -1) {
            compId++;
            dfs2Kosaraju(u, adjRev, component, compId);
        }
    }
    return {compId, component};
}
```

#### 2. 2-SAT Solving Template ($O(V + E)$)
```cpp
// Variable x is represented as 2*x (true) and 2*x + 1 (false)
int negateVar(int x) { return x ^ 1; }

bool solve2SAT(int numVars, const vector<pair<int,int>>& clauses, vector<bool>& assignment) {
    int n = 2 * numVars;
    vector<vector<int>> adj(n);
    
    for (auto& [u, v] : clauses) {
        // (u or v) <=> (not u => v) and (not v => u)
        adj[negateVar(u)].push_back(v);
        adj[negateVar(v)].push_back(u);
    }
    
    auto [numComps, comp] = getSCCKosaraju(n - 1, adj);
    
    assignment.resize(numVars);
    for (int i = 0; i < numVars; i++) {
        int trueVar = 2 * i;
        int falseVar = 2 * i + 1;
        if (comp[trueVar] == comp[falseVar]) return false; // Unsatisfiable!
        
        // In Kosaraju, smaller component ID means later topologically
        assignment[i] = comp[trueVar] < comp[falseVar];
    }
    return true;
}
```

---

## Pattern 8: Bridges & Articulation Points (Tarjan's Low-Link Algorithm)

### 🔍 Identification Signals
- **Bridge (Critical Connection)**: Edge whose removal increases number of connected components.
- **Articulation Point (Cut Vertex)**: Vertex whose removal increases number of connected components.
- **Core Condition**:
  - Edge $(u, v)$ is a **Bridge** iff `low[v] > tin[u]`.
  - Vertex $u$ is an **Articulation Point** iff:
    1. $u$ is root of DFS tree and has $\ge 2$ children, OR
    2. $u$ is not root and has child $v$ such that `low[v] >= tin[u]`.

### 💻 Standard Template

```cpp
class BridgeAndCutVertices {
    int timer = 0;
    vector<int> tin, low;
    vector<bool> visited, isCutVertex;
    vector<pair<int,int>> bridges;

public:
    void dfs(int u, int p, const vector<vector<int>>& adj) {
        visited[u] = true;
        tin[u] = low[u] = ++timer;
        int children = 0;
        
        for (int v : adj[u]) {
            if (v == p) continue; // Skip direct parent edge
            if (visited[v]) {
                // Back-edge
                low[u] = min(low[u], tin[v]);
            } else {
                // Forward tree edge
                children++;
                dfs(v, u, adj);
                low[u] = min(low[u], low[v]);
                
                // Bridge check
                if (low[v] > tin[u]) {
                    bridges.push_back({u, v});
                }
                
                // Articulation Point check (non-root)
                if (p != -1 && low[v] >= tin[u]) {
                    isCutVertex[u] = true;
                }
            }
        }
        
        // Articulation Point check (root node)
        if (p == -1 && children > 1) {
            isCutVertex[u] = true;
        }
    }

    void findBridgesAndCuts(int n, const vector<vector<int>>& adj) {
        timer = 0;
        tin.assign(n + 1, 0);
        low.assign(n + 1, 0);
        visited.assign(n + 1, false);
        isCutVertex.assign(n + 1, false);
        bridges.clear();

        for (int i = 1; i <= n; i++) {
            if (!visited[i]) dfs(i, -1, adj);
        }
    }
};
```

---

## Pattern 9: Trees - Binary Lifting, LCA, and Path Queries

### 🔍 Identification Signals
- Queries on tree path between $u$ and $v$ (Distance, Min/Max edge weight on path, $K$-th ancestor).
- Precomputation: $O(N \log N)$ space/time. Query: $O(\log N)$ time.

### 💻 Standard Binary Lifting & LCA Template

```cpp
class TreeLCA {
    int n, LOG;
    vector<int> depth;
    vector<vector<int>> up; // up[u][j] stores 2^j-th ancestor of u
    vector<vector<pair<int,int>>> maxEdge; // maxEdge[u][j] stores max edge weight on path to 2^j ancestor

public:
    TreeLCA(int n, int root, const vector<vector<pair<int,int>>>& adj) {
        this->n = n;
        LOG = ceil(log2(n)) + 1;
        depth.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(LOG, 0));
        maxEdge.assign(n + 1, vector<pair<int,int>>(LOG, {0, 0}));
        
        dfs(root, root, 0, 0, adj);
    }

    void dfs(int u, int p, int d, int w, const vector<vector<pair<int,int>>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        maxEdge[u][0] = {w, w};
        
        for (int j = 1; j < LOG; j++) {
            up[u][j] = up[up[u][j - 1]][j - 1];
            maxEdge[u][j].first = max(maxEdge[u][j - 1].first, maxEdge[up[u][j - 1]][j - 1].first);
        }
        
        for (auto& [v, weight] : adj[u]) {
            if (v != p) dfs(v, u, d + 1, weight, adj);
        }
    }

    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        // 1. Lift u to same depth as v
        for (int j = LOG - 1; j >= 0; j--) {
            if (depth[u] - (1 << j) >= depth[v]) {
                u = up[u][j];
            }
        }
        if (u == v) return u;
        
        // 2. Lift u and v together until parents match
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }
};
```

---

## Pattern 10: Eulerian & Hamiltonian Paths

### 🔍 Identification Signals
- **Eulerian Circuit/Path**: Visit **EVERY EDGE EXACTLY ONCE**.
  - Undirected Existence: All nodes have even degree (Circuit) OR exactly 2 nodes have odd degree (Path).
  - Algorithm: Hierholzer's Algorithm ($O(V + E)$).
- **Hamiltonian Path/TSP**: Visit **EVERY VERTEX EXACTLY ONCE**.
  - Algorithm: Bitmask DP ($O(N^2 \cdot 2^N)$).

### 💻 Standard Hierholzer's Algorithm Template (Eulerian Circuit)

```cpp
vector<int> getEulerianCircuitUndirected(int n, vector<vector<pair<int,int>>>& adj, int numEdges) {
    // adj stores pair<neighbor, edgeIndex>
    vector<bool> usedEdge(numEdges, false);
    vector<int> circuit;
    stack<int> st;
    st.push(1); // Start node
    
    vector<int> headPtr(n + 1, 0); // Pointer to avoid re-scanning edges
    
    while (!st.empty()) {
        int u = st.top();
        bool foundEdge = false;
        
        while (headPtr[u] < adj[u].size()) {
            auto [v, edgeId] = adj[u][headPtr[u]++];
            if (!usedEdge[edgeId]) {
                usedEdge[edgeId] = true;
                st.push(v);
                foundEdge = true;
                break;
            }
        }
        
        if (!foundEdge) {
            circuit.push_back(u);
            st.pop();
        }
    }
    
    if (circuit.size() != numEdges + 1) return {}; // Disconnected graph
    return circuit;
}
```

---

## Pattern 11: Functional Graphs & Succession Graphs

### 🔍 Identification Signals
- Directed graph where **every vertex has out-degree exactly equal to 1** ($f(u) = v$).
- Structure: Always consists of **a set of components, each containing a single cycle with trees rooted on the cycle directed TOWARD the cycle**.
- Useful for: Jump $K$ steps in functional graph ($K \le 10^{18}$), Cycle length calculation.

### 💻 Binary Lifting on Succession Graph ($K$-th Successor in $O(\log K)$)

```cpp
vector<int> getKthSuccessor(int n, const vector<int>& succ, long long k) {
    int LOG = 60; // 2^60 > 10^18
    vector<vector<int>> up(n + 1, vector<int>(LOG));
    
    for (int i = 1; i <= n; i++) up[i][0] = succ[i];
    
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
    
    vector<int> ans(n + 1);
    for (int i = 1; i <= n; i++) {
        int curr = i;
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) curr = up[curr][j];
        }
        ans[i] = curr;
    }
    return ans;
}
```

---

## Pattern 12: Network Flow & Bipartite Matching

### 🔍 Identification Signals
- Maximum capacity flow, bottleneck matching, grid coverage, independent sets.
- **Kőnig's Theorem**: In any bipartite graph, **Max Matching = Min Vertex Cover**.
- **Minimum Path Cover in DAG**: $N - \text{Max Bipartite Matching}$.
- Algorithm: **Dinic's Algorithm** ($O(V^2 E)$ general, $O(E \sqrt{V})$ for Bipartite Matching).

### 💻 Standard Dinic's Algorithm Template

```cpp
struct FlowEdge {
    int u, v;
    long long cap, flow = 0;
};

class Dinic {
    int n, s, t;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    vector<int> level, ptr;

public:
    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n + 1);
        level.resize(n + 1);
        ptr.resize(n + 1);
    }

    void addEdge(int u, int v, long long cap) {
        adj[u].push_back(edges.size());
        edges.push_back({u, v, cap, 0});
        adj[v].push_back(edges.size());
        edges.push_back({v, u, 0, 0}); // Reverse residual edge
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                if (edges[id].cap - edges[id].flow > 0 && level[edges[id].v] == -1) {
                    level[edges[id].v] = level[u] + 1;
                    q.push(edges[id].v);
                }
            }
        }
        return level[t] != -1;
    }

    long long dfs(int u, long long pushed) {
        if (pushed == 0 || u == t) return pushed;
        for (int& cid = ptr[u]; cid < adj[u].size(); cid++) {
            int id = adj[u][cid];
            int v = edges[id].v;
            if (level[u] + 1 != level[v] || edges[id].cap - edges[id].flow == 0) continue;
            long long tr = dfs(v, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long maxFlow() {
        long long flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, 1e18)) {
                flow += pushed;
            }
        }
        return flow;
    }
};
```

---

## Pattern 13: Advanced OA & CP Graph Techniques (HLD, Centroids, Hashing)

### 1. Heavy-Light Decomposition (HLD)
- **Use Case**: Path updates & path queries on trees (e.g. Add $W$ to all nodes on path $u \to v$, Query max value on path $u \to v$).
- **Mechanism**: Decompose tree into heavy chains. Combines with Segment Tree to perform queries in $O(\log^2 N)$ time!

### 2. Centroid Decomposition
- **Use Case**: Divide & conquer on tree paths of length $K$ or distance queries.
- **Mechanism**: Find centroid node $C$ (node whose removal leaves subtrees of size $\le N/2$). Recursively decompose in depth $O(\log N)$.

### 3. Tree Isomorphism & Double Hashing
- **Use Case**: Check if two unrooted trees have identical structural shape.
- **Mechanism**: Compute AHU subtree hashes or Centroid tree polynomial hash.

---

## Master Graph Pattern Decision Matrix (Parts 1 & 2)

| Problem Characteristic / Keyword | Recommended Graph Pattern | Algorithm / Class | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| Unweighted shortest path / min grid steps | **2D / Grid BFS** | `std::queue` | $O(V + E)$ | $O(V)$ |
| Spreading from multiple sources simultaneously | **Multi-Source BFS** | Queue initialized with all sources | $O(V + E)$ | $O(V)$ |
| Shortest path with edge weights $0$ or $1$ | **0-1 BFS** | `std::deque` | $O(V + E)$ | $O(V)$ |
| Dynamic component merge / Undirected cycle | **Disjoint Set Union** | `DSU` (Path Comp + Rank) | $O(E \cdot \alpha(V))$ | $O(V)$ |
| Process edge deletions | **Offline Query DSU** | Reverse Query Processing | $O((V + Q) \cdot \alpha(V))$ | $O(V + Q)$ |
| Directed cycle detection / 3-color states | **Directed DFS** | 3-State Recursion Stack | $O(V + E)$ | $O(V)$ |
| Divide graph into 2 sets without same color | **Bipartite 2-Coloring** | BFS / DFS 2-Coloring | $O(V + E)$ | $O(V)$ |
| Task dependencies / Prerequisite ordering | **Topological Sort** | Kahn's Queue Algorithm | $O(V + E)$ | $O(V + E)$ |
| Non-negative weighted shortest path | **Single-Source Dijkstra** | Min-Priority Queue | $O(E \log V)$ | $O(V + E)$ |
| State-space shortest path (Discount/Fuel) | **State Dijkstra** | `dist[u][state]` PQ | $O(S \cdot E \log(V \cdot S))$ | $O(V \cdot S)$ |
| Negative weights / Negative cycle finding | **Bellman-Ford** | $N$-Pass Relaxation | $O(V \cdot E)$ | $O(V + E)$ |
| All-pairs shortest path ($V \le 500$) | **Floyd-Warshall** | 3 Nested Loops | $O(V^3)$ | $O(V^2)$ |
| Minimum total edge weight tree spanning all nodes | **MST** | Kruskal (DSU) / Prim | $O(E \log E)$ | $O(V + E)$ |
| Maximal reachability components in Directed Graph | **Strongly Connected Components** | Kosaraju / Tarjan | $O(V + E)$ | $O(V + E)$ |
| Boolean formula 2 vars per clause | **2-SAT** | Implication Graph + SCC | $O(V + E)$ | $O(V + E)$ |
| Critical edge / vertex removal breaks connectivity | **Tarjan Low-Link** | `tin` & `low` DFS | $O(V + E)$ | $O(V + E)$ |
| Tree path queries / $K$-th ancestor / LCA | **Binary Lifting** | `up[u][j]` DP Table | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| Visit EVERY EDGE exactly once | **Eulerian Path** | Hierholzer's Algorithm | $O(V + E)$ | $O(V + E)$ |
| Visit EVERY VERTEX exactly once ($N \le 20$) | **Hamiltonian Path / TSP** | Bitmask DP | $O(N^2 \cdot 2^N)$ | $O(N \cdot 2^N)$ |
| Out-degree = 1 for all nodes / Jump $K$ steps | **Functional Graph** | Successor Binary Lifting | $O(N \log K)$ | $O(N \log K)$ |
| Max flow, Min Cut, Grid Min Cover | **Network Flow** | Dinic's Algorithm | $O(V^2 E)$ | $O(V + E)$ |
