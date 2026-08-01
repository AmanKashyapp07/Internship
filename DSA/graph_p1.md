# 🕸️ Master Graph Patterns Guide - Part 1: Core & Standard Patterns

> **The definitive reference for Core Graph Algorithms, Traversals, Shortest Paths, Topological Sort, and Spanning Trees for Tech Interviews (FAANG, Top Tech) and Online Assessments (OAs).**

---

## 📋 Table of Contents (Part 1)
1. [Graph Problem-Solving Framework & Representation](#1-graph-problem-solving-framework--representation)
2. [Pattern 1: Traversal & Reachability (BFS, DFS, 0-1 BFS, Multi-Source BFS)](#pattern-1-traversal--reachability-bfs-dfs-0-1-bfs-multi-source-bfs)
3. [Pattern 2: Disjoint Set Union (DSU / Union-Find) & Offline Queries](#pattern-2-disjoint-set-union-dsu--union-find--offline-queries)
4. [Pattern 3: Cycle Detection & Bipartite Graphs](#pattern-3-cycle-detection--bipartite-graphs)
5. [Pattern 4: Topological Sort & DAG DP](#pattern-4-topological-sort--dag-dp)
6. [Pattern 5: Shortest Path Algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)](#pattern-5-shortest-path-algorithms-dijkstra-bellman-ford-floyd-warshall)
7. [Pattern 6: Minimum Spanning Trees (Kruskal & Prim)](#pattern-6-minimum-spanning-trees-kruskal--prim)

---

## 1. Graph Problem-Solving Framework & Representation

When faced with a graph problem in an OA or interview:

### 1. Choose the Right Graph Representation
* **Adjacency List** `vector<vector<pair<int, int>>> adj`: Default choice for sparse graphs ($E \ll V^2$).
* **Adjacency Matrix** `vector<vector<int>> grid`: Best for dense graphs ($E \approx V^2$) or 2D grid maps.
* **Implicit Graph**: Nodes are states, edges are valid state transitions (e.g., Word Ladder, Knight's Tour, Jug Water Problem).

### 2. Time & Space Complexity Guidelines
| $V$ (Vertices) | Max Allowed Algorithm Complexity | Recommended Algorithm |
| :--- | :--- | :--- |
| $V \le 20$ | $O(2^V \cdot V^2)$ | Bitmask DP / Backtracking / TSP |
| $V \le 40$ | $O(2^{V/2})$ | Meet-in-the-Middle |
| $V \le 500$ | $O(V^3)$ | Floyd-Warshall / Min-Cost Max-Flow |
| $V \le 2000$ | $O(V^2)$ | Dense Dijkstra / Standard Dynamic Programming |
| $V \le 10^5$ | $O((V + E) \log V)$ | Dijkstra, Kruskal, Topo Sort, Tarjan, Binary Lifting |
| $V \le 10^6$ | $O(V + E)$ | BFS, DFS, Kahn's Topo Sort, Kosaraju |

---

## Pattern 1: Traversal & Reachability (BFS, DFS, 0-1 BFS, Multi-Source BFS)

### 🔍 Identification Signals
- **BFS**: Unweighted shortest path, level-by-level traversal, minimum steps.
- **DFS**: Path existence, connected components, recursive exhaustive search, backtracking.
- **Multi-Source BFS**: Spreading phenomena from multiple starting points simultaneously (e.g., Rotting Oranges, Fire Spreading).
- **0-1 BFS**: Edge weights are ONLY `0` or `1` (e.g., Grid with free vs cost-1 wall turns). Solve in $O(V + E)$ using `std::deque`!

### 💻 Core Templates

#### 1. 2D Grid Flood Fill & Path Reconstruction
```cpp
// Returns shortest path length and directions ('U','D','L','R') in a 2D grid
pair<int, string> gridBFS(vector<vector<char>>& grid, pair<int,int> start, pair<int,int> target) {
    int R = grid.size(), C = grid[0].size();
    vector<vector<int>> dist(R, vector<int>(C, -1));
    vector<vector<char>> parentDir(R, vector<char>(C, 0));
    vector<vector<pair<int,int>>> parentCell(R, vector<pair<int,int>>(C, {-1, -1}));
    
    queue<pair<int,int>> q;
    q.push(start);
    dist[start.first][start.second] = 0;
    
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char dirChar[] = {'U', 'D', 'L', 'R'};
    
    bool found = false;
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (make_pair(r, c) == target) { found = true; break; }
        
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] != '#' && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                parentDir[nr][nc] = dirChar[i];
                parentCell[nr][nc] = {r, c};
                q.push({nr, nc});
            }
        }
    }
    
    if (!found) return {-1, ""};
    
    // Reconstruct Path
    string path = "";
    pair<int,int> curr = target;
    while (curr != start) {
        char d = parentDir[curr.first][curr.second];
        path += d;
        curr = parentCell[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return {dist[target.first][target.second], path};
}
```

#### 2. Multi-Source BFS (Rotting Oranges / CSES Monsters)
- **Key Insight**: Push ALL initial sources into the queue at $T=0$ simultaneously!

```cpp
int multiSourceBFS(int R, int C, const vector<pair<int,int>>& sources, const vector<vector<char>>& grid) {
    queue<pair<int,int>> q;
    vector<vector<int>> dist(R, vector<int>(C, -1));
    
    for (auto& s : sources) {
        q.push(s);
        dist[s.first][s.second] = 0;
    }
    
    int maxDist = 0;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        maxDist = max(maxDist, dist[r][c]);
        
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] != '#' && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return maxDist;
}
```

#### 3. 0-1 BFS ($O(V + E)$ using `std::deque`)
- Edge weight 0 $\implies$ push to **FRONT** of deque.
- Edge weight 1 $\implies$ push to **BACK** of deque.

```cpp
int zeroOneBFS(int n, int src, int target, const vector<vector<pair<int,int>>>& adj) {
    vector<int> dist(n + 1, 1e9);
    deque<int> dq;
    
    dist[src] = 0;
    dq.push_front(src);
    
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        if (u == target) return dist[u];
        
        for (auto& [v, w] : adj[u]) { // w is either 0 or 1
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist[target] >= 1e9 ? -1 : dist[target];
}
```

---

## Pattern 2: Disjoint Set Union (DSU / Union-Find) & Offline Queries

### 🔍 Identification Signals
- Dynamic connectivity (merging components).
- Cycle detection in undirected graphs.
- **Offline Query Trick**: When edges/nodes are *deleted*, process queries in **REVERSE** (from end to start) so deletions become insertions!

### 💻 Standard DSU Class & Reverse Query Template

```cpp
class DSU {
public:
    vector<int> parent, sz;
    int numComponents;
    int maxSize;

    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
        sz.assign(n + 1, 1);
        numComponents = n;
        maxSize = 1;
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]); // Path Compression
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            // Union by Size
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            maxSize = max(maxSize, sz[root_i]);
            numComponents--;
            return true;
        }
        return false; // Already in same component (cycle detected!)
    }
};
```

#### Offline Reverse Query Processing (Network Breakdown)
```cpp
// Queries ask: "What is the component count after deleting edge i?"
vector<int> offlineNetworkBreakdown(int n, vector<pair<int,int>>& edges, vector<int>& edgeDeletions) {
    vector<bool> isDeleted(edges.size(), false);
    for (int idx : edgeDeletions) isDeleted[idx] = true;
    
    DSU dsu(n);
    // 1. Add all edges that are NEVER deleted
    for (int i = 0; i < edges.size(); i++) {
        if (!isDeleted[i]) dsu.unite(edges[i].first, edges[i].second);
    }
    
    // 2. Process queries in REVERSE order
    vector<int> ans(edgeDeletions.size());
    for (int i = edgeDeletions.size() - 1; i >= 0; i--) {
        ans[i] = dsu.numComponents; // Record component count BEFORE adding back deleted edge
        int edgeIdx = edgeDeletions[i];
        dsu.unite(edges[edgeIdx].first, edges[edgeIdx].second);
    }
    return ans;
}
```

---

## Pattern 3: Cycle Detection & Bipartite Graphs

### 🔍 Identification Signals
- **Undirected Cycle**: Keep track of `parent` node in DFS/BFS to avoid going back to immediate parent.
- **Directed Cycle**: Use 3 states: `0 = UNVISITED`, `1 = VISITING (In Recursion Stack)`, `2 = VISITED`. A cycle exists if we visit a node in state `1`.
- **Bipartite Graph (2-Coloring)**: Can we divide graph into 2 sets such that no two adjacent nodes share the same color? (Fails iff odd length cycle exists).

### 💻 Standard Templates

#### 1. Directed Graph Cycle Detection & Path Reconstruction (3-State DFS)
```cpp
bool dfsDirectedCycle(int u, const vector<vector<int>>& adj, vector<int>& state, vector<int>& parent, int& cycleStart, int& cycleEnd) {
    state[u] = 1; // VISITING (Active in current recursion path)
    
    for (int v : adj[u]) {
        if (state[v] == 0) {
            parent[v] = u;
            if (dfsDirectedCycle(v, adj, state, parent, cycleStart, cycleEnd)) return true;
        } else if (state[v] == 1) { // Back-edge detected!
            cycleStart = v;
            cycleEnd = u;
            return true;
        }
    }
    state[u] = 2; // VISITED
    return false;
}

vector<int> findDirectedCycle(int n, const vector<vector<int>>& adj) {
    vector<int> state(n + 1, 0), parent(n + 1, -1);
    int cycleStart = -1, cycleEnd = -1;
    
    for (int i = 1; i <= n; i++) {
        if (state[i] == 0) {
            if (dfsDirectedCycle(i, adj, state, parent, cycleStart, cycleEnd)) break;
        }
    }
    
    if (cycleStart == -1) return {}; // No cycle
    
    vector<int> cycle;
    cycle.push_back(cycleStart);
    for (int curr = cycleEnd; curr != cycleStart; curr = parent[curr]) {
        cycle.push_back(curr);
    }
    cycle.push_back(cycleStart);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}
```

#### 2. Bipartite 2-Coloring (BFS / DFS)
```cpp
bool isBipartite(int n, const vector<vector<int>>& adj) {
    vector<int> color(n + 1, -1); // -1: uncolored, 0: Color A, 1: Color B
    
    for (int i = 1; i <= n; i++) {
        if (color[i] != -1) continue;
        queue<int> q;
        q.push(i);
        color[i] = 0;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u]; // Color with opposite color
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false; // Conflict! Odd cycle exists.
                }
            }
        }
    }
    return true;
}
```

---

## Pattern 4: Topological Sort & DAG DP

### 🔍 Identification Signals
- Prerequisites, task scheduling, topological order in a Directed Acyclic Graph (DAG).
- Kahn's Algorithm (BFS using in-degrees) or DFS post-order reversal.
- **DAG DP**: Compute longest path, shortest path, or total number of paths in a DAG in $O(V + E)$ time!

### 💻 Standard Templates

#### 1. Kahn's Topological Sort Algorithm ($O(V + E)$)
```cpp
vector<int> topologicalSort(int n, const vector<vector<int>>& adj) {
    vector<int> inDegree(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }
    
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    vector<int> topo;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    
    if (topo.size() != n) return {}; // Graph has a cycle!
    return topo;
}
```

#### 2. DAG DP: Path Counting & Longest Path
```cpp
// Count total distinct paths from src (1) to dest (N) modulo 1e9+7
int countPathsDAG(int n, const vector<vector<int>>& adj, const vector<int>& topoOrder) {
    const int MOD = 1e9 + 7;
    vector<int> dp(n + 1, 0);
    dp[1] = 1; // Base case: 1 path to start node
    
    for (int u : topoOrder) {
        for (int v : adj[u]) {
            dp[v] = (dp[v] + dp[u]) % MOD;
        }
    }
    return dp[n];
}
```

---

## Pattern 5: Shortest Path Algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)

### 🔍 Identification Signals
- **Dijkstra**: Non-negative edge weights ($O(E \log V)$).
- **State-Space Dijkstra**: Nodes contain extra state variables (e.g. `(u, has_discount_used)`).
- **Bellman-Ford**: Negative edge weights, negative cycle detection ($O(V \cdot E)$).
- **Floyd-Warshall**: All-Pairs Shortest Path for $V \le 500$ ($O(V^3)$).

### 💻 Standard Templates

#### 1. Single-Source Dijkstra ($O(E \log V)$)
```cpp
vector<long long> dijkstra(int n, int src, const vector<vector<pair<int, int>>>& adj) {
    vector<long long> dist(n + 1, 1e18);
    // Min-Priority Queue storing pair<distance, node>
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // Lazy deletion / stale pair check
        
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

#### 2. Flight Discount (State-Space Dijkstra)
- Find min cost from $1 \to N$ where **ONE** edge cost can be halved.
- State: `dist[u][0]` = min cost to reach $u$ WITHOUT using discount.
- State: `dist[u][1]` = min cost to reach $u$ HAVING USED discount.

```cpp
long long flightDiscount(int n, const vector<vector<pair<int,int>>>& adj) {
    vector<vector<long long>> dist(n + 1, vector<long long>(2, 1e18));
    using Tuple = tuple<long long, int, int>; // {dist, node, used_discount}
    priority_queue<Tuple, vector<Tuple>, greater<Tuple>> pq;
    
    dist[1][0] = 0;
    pq.push({0, 1, 0});
    
    while (!pq.empty()) {
        auto [d, u, used] = pq.top(); pq.pop();
        if (d > dist[u][used]) continue;
        
        for (auto& [v, w] : adj[u]) {
            // Option 1: Do not use discount on edge u->v
            if (dist[u][used] + w < dist[v][used]) {
                dist[v][used] = dist[u][used] + w;
                pq.push({dist[v][used], v, used});
            }
            // Option 2: Use discount on edge u->v (if not used yet)
            if (used == 0) {
                if (dist[u][0] + w / 2 < dist[v][1]) {
                    dist[v][1] = dist[u][0] + w / 2;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }
    return min(dist[n][0], dist[n][1]);
}
```

#### 3. Bellman-Ford & Negative Cycle Detection ($O(V \cdot E)$)
```cpp
struct Edge { int u, v; long long w; };

vector<int> findNegativeCycle(int n, const vector<Edge>& edges) {
    vector<long long> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    int lastChangedNode = -1;
    
    // Relax edges N times
    for (int i = 1; i <= n; i++) {
        lastChangedNode = -1;
        for (const auto& e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                lastChangedNode = e.v;
            }
        }
    }
    
    if (lastChangedNode == -1) return {}; // No negative cycle
    
    // Backtrack N times to ensure we enter the cycle
    int cycleNode = lastChangedNode;
    for (int i = 0; i < n; i++) cycleNode = parent[cycleNode];
    
    vector<int> cycle;
    for (int curr = cycleNode;; curr = parent[curr]) {
        cycle.push_back(curr);
        if (curr == cycleNode && cycle.size() > 1) break;
    }
    reverse(cycle.begin(), cycle.end());
    return cycle;
}
```

#### 4. Floyd-Warshall All-Pairs Shortest Path ($O(V^3)$)
```cpp
void floydWarshall(int n, vector<vector<long long>>& dist) {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < 1e18 && dist[k][j] < 1e18) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}
```

---

## Pattern 6: Minimum Spanning Trees (Kruskal & Prim)

### 🔍 Identification Signals
- Connect all $V$ nodes using $V-1$ edges such that total edge weight is minimized.
- **Kruskal**: Sort all edges by weight, add via DSU ($O(E \log E)$).
- **Prim**: Priority queue greedy selection from current tree component ($O(E \log V)$).

### 💻 Standard Templates

#### Kruskal's Algorithm
```cpp
struct MSTEdge {
    int u, v;
    long long w;
    bool operator<(const MSTEdge& other) const { return w < other.w; }
};

long long kruskalMST(int n, vector<MSTEdge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    long long mstWeight = 0;
    int edgesAdded = 0;
    
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mstWeight += e.w;
            edgesAdded++;
            if (edgesAdded == n - 1) break;
        }
    }
    
    return (edgesAdded == n - 1) ? mstWeight : -1; // -1 if graph is disconnected
}
```
