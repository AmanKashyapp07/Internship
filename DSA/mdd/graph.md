# CSES Graph Algorithms & Advanced Problems - Master Reference

> **All 41 CSES Graph & Advanced Graph Problems complete with Classic Pattern Identifiers, Precise Problem Statements, Algorithmic Logic, Minimal Informal Code Snippets, and Time/Space Complexities.**

---

## Quick Reference Summary Table

| # | Problem Name | Classic Pattern / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| 1 | **Counting Rooms** | 2D Grid Flood Fill (BFS/DFS) | $O(N \cdot M)$ | $O(N \cdot M)$ |
| 2 | **Labyrinth** | 2D Grid Shortest Path BFS + Direction Tracking | $O(N \cdot M)$ | $O(N \cdot M)$ |
| 3 | **Building Roads** | Disjoint Set Union (DSU) Connected Components | $O(V + E \cdot \alpha(V))$ | $O(V)$ |
| 4 | **Message Route** | Unweighted Shortest Path BFS + Parent Trace | $O(V + E)$ | $O(V)$ |
| 5 | **Building Teams** | Bipartite 2-Coloring (DFS/BFS) | $O(V + E)$ | $O(V)$ |
| 6 | **Round Trip** | Undirected Cycle Detection & Backtrack DFS | $O(V + E)$ | $O(V)$ |
| 7 | **Monsters** | Dual Multi-Source BFS (Simultaneous Spreading) | $O(N \cdot M)$ | $O(N \cdot M)$ |
| 8 | **Shortest Routes I** | Single-Source Dijkstra | $O(E \log V)$ | $O(V + E)$ |
| 9 | **Shortest Routes II** | All-Pairs Floyd-Warshall | $O(V^3)$ | $O(V^2)$ |
| 10 | **High Score** | Bellman-Ford Max Path + Positive Cycle Reachability | $O(V \cdot E)$ | $O(V + E)$ |
| 11 | **Flight Discount** | Dual Dijkstra / State-Space Dijkstra | $O(E \log V)$ | $O(V + E)$ |
| 12 | **Cycle Finding** | Bellman-Ford Negative Cycle Detection | $O(V \cdot E)$ | $O(V + E)$ |
| 13 | **Flight Routes** | $K$-Shortest Paths Dijkstra | $O(K \cdot E \log(V \cdot K))$ | $O(K \cdot V)$ |
| 14 | **Round Trip II** | Directed Cycle Detection (3-State DFS / In-Path) | $O(V + E)$ | $O(V)$ |
| 15 | **Course Schedule** | Kahn's Topological Sort (In-Degree Queue) | $O(V + E)$ | $O(V + E)$ |
| 16 | **Longest Flight Route** | DAG Longest Path via Topo DP | $O(V + E)$ | $O(V + E)$ |
| 17 | **Game Routes** | DAG Path Counting via Topo DP | $O(V + E)$ | $O(V + E)$ |
| 18 | **Investigation** | Dijkstra DP (Dist, Ways, Min/Max Edges) | $O(E \log V)$ | $O(V + E)$ |
| 19 | **Planets Queries I** | Binary Lifting (`up[u][j]`) for $K$-th Successor | $O(N \log K + Q \log K)$ | $O(N \log K)$ |
| 20 | **Planets Cycles** | Functional Graph (Floyd Cycle + Tree Distance) | $O(N)$ | $O(N)$ |
| 21 | **Road Reparation** | Minimum Spanning Tree (Kruskal / Prim) | $O(E \log E)$ | $O(V + E)$ |
| 22 | **Road Construction** | DSU Component Count & Max Size Tracking | $O(E \cdot \alpha(V))$ | $O(V)$ |
| 23 | **Flight Routes Check** | Strongly Connected Check (Dual BFS on $G$ & $G^R$) | $O(V + E)$ | $O(V + E)$ |
| 24 | **Planets and Kingdoms** | Strongly Connected Components (Kosaraju 2-Pass) | $O(V + E)$ | $O(V + E)$ |
| 25 | **Coin Collector** | SCC Condensation DAG + Topo DP | $O(V + E)$ | $O(V + E)$ |
| 26 | **Mail Delivery** | Undirected Eulerian Circuit (Hierholzer) | $O(V + E)$ | $O(V + E)$ |
| 27 | **Teleporters Path** | Directed Eulerian Path $1 \to N$ (Hierholzer) | $O(V + E)$ | $O(V + E)$ |
| 28 | **De Bruijn Sequence** | Eulerian Circuit on $2^{n-1}$ de Bruijn Graph | $O(2^n)$ | $O(2^n)$ |
| 29 | **Hamiltonian Flights** | Bitmask DP (Traveling Salesperson Problem) | $O(N^2 \cdot 2^N)$ | $O(N \cdot 2^N)$ |
| 30 | **Graph Girth** | Shortest Unweighted Cycle (BFS per Node) | $O(V \cdot (V + E))$ | $O(V + E)$ |
| 31 | **Course Schedule II** | Lexicographical Topo Sort (Reverse Graph + Max-PQ) | $O(V \log V + E)$ | $O(V + E)$ |
| 32 | **Strongly Connected Edges**| Tarjan Bridge Check + DFS Tree Orientation | $O(V + E)$ | $O(V + E)$ |
| 33 | **Acyclic Graph Edges** | Vertex Index Orientation ($u < v \implies u \to v$) | $O(V + E)$ | $O(1)$ |
| 34 | **Network Breakdown** | Offline Reverse Query Processing via DSU | $O((V + Q) \cdot \alpha(V))$ | $O(V + Q)$ |
| 35 | **Network Renovation** | Tree Leaf Pair Matching ($i \leftrightarrow i + \lfloor L/2 \rfloor$) | $O(N)$ | $O(N)$ |
| 36 | **Visiting Cities** | Shortest Path Subgraph + Double Hashing | $O(E \log V)$ | $O(V + E)$ |
| 37 | **MST Edge Check / Cost** | Binary Lifting Max Edge Weight on MST | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| 38 | **Transfer Speeds Sum** | Kruskal MST DSU Bottleneck Sum | $O(E \log E)$ | $O(V)$ |
| 39 | **Reachable Nodes** | Bitset Topological Sort DP | $O(V \cdot (V + E) / 64)$ | $O(V \cdot N / 64)$ |
| 40 | **Eulerian Subgraphs** | $2^{E - V + C}$ Even Degree Formula | $O(V + E)$ | $O(V)$ |
| 41 | **Coin Grid** | Bipartite Min Vertex Cover (Kőnig's Theorem & Kuhn) | $O(V \cdot E)$ | $O(V + E)$ |

---

## Detailed Problems & Minimal Informal Code Snippets

### 1. Counting Rooms
* **Classic Pattern**: **2D Grid Flood Fill (BFS / DFS)**
* **Problem Statement**: Given an $N \times M$ grid of floor cells (`'.'`) and wall cells (`'#'`), count the total number of connected components of floor cells (rooms).
* **Algorithm**: Iterate grid. When finding an unvisited floor cell, increment `rooms` and flood fill all 4-directional connected floor cells.
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
// Minimal Snippet
for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
        if (grid[i][j] == '.') { rooms++; floodFill(i, j); }

void floodFill(int r, int c) {
    if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] == '#') return;
    grid[r][c] = '#'; // Mark visited
    for (int d = 0; d < 4; d++) floodFill(r + dr[d], c + dc[d]);
}
```

---

### 2. Labyrinth
* **Classic Pattern**: **2D Grid Shortest Path BFS + Direction Tracking**
* **Problem Statement**: Find the shortest path from `'A'` to `'B'` in an $N \times M$ grid. Print length and path moves (`U, D, L, R`), or `"NO"`.
* **Algorithm**: Queue BFS storing `parentCell` and `parentDir`. Backtrack from `'B'` to `'A'` once target is reached.
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
// Minimal Snippet
q.push({startR, startC}); vis[startR][startC] = true;
while (!q.empty()) {
    auto [r, c] = q.front(); q.pop();
    if (r == endR && c == endC) break;
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i], nc = c + dc[i];
        if (valid(nr, nc) && !vis[nr][nc]) {
            vis[nr][nc] = true; parDir[nr][nc] = dir[i]; par[nr][nc] = {r, c}; q.push({nr, nc});
        }
    }
}
// Backtrack path: while (curr != start) path += parDir[curr], curr = par[curr]; reverse(path);
```

---

### 3. Building Roads
* **Classic Pattern**: **Disjoint Set Union (DSU) Connected Components**
* **Problem Statement**: Find minimum new roads needed to connect $N$ cities, and list the exact roads to construct.
* **Algorithm**: DSU unite existing roads. Collect representative root for each component. Connect root $i$ to root $i+1$.
* **Complexities**: $O(V + E \cdot \alpha(V))$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
DSU dsu(n);
for (auto& [u, v] : roads) dsu.unite(u, v);
for (int i = 1; i <= n; i++) if (dsu.find(i) == i) roots.push_back(i);
for (int i = 0; i < (int)roots.size() - 1; i++) newRoads.push_back({roots[i], roots[i + 1]});
```

---

### 4. Message Route
* **Classic Pattern**: **Unweighted Graph Shortest Path (BFS + Parent Trace)**
* **Problem Statement**: Find minimum number of computers in a transmission path from node $1$ to node $N$ in an unweighted graph.
* **Algorithm**: Unweighted BFS keeping `parent[v] = u`. Backtrack path $N \to 1$.
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
q.push(1); dist[1] = 0;
while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : adj[u])
        if (dist[v] == -1) { dist[v] = dist[u] + 1; parent[v] = u; q.push(v); }
}
// Trace: for (int u = n; u != -1; u = parent[u]) path.push_back(u); reverse(path);
```

---

### 5. Building Teams
* **Classic Pattern**: **Bipartite Graph 2-Coloring (DFS / BFS)**
* **Problem Statement**: Divide $N$ pupils into two teams such that no two friends are in the same team (2-Coloring).
* **Algorithm**: DFS/BFS assigning color $1$ or $2$ (toggle `3 - color[u]`). If neighbor has same color, return `"IMPOSSIBLE"`.
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
bool dfsColor(int u, int c) {
    color[u] = c;
    for (int v : adj[u]) {
        if (color[v] == 0 && !dfsColor(v, 3 - c)) return false;
        else if (color[v] == c) return false; // Conflict!
    }
    return true;
}
```

---

### 6. Round Trip
* **Classic Pattern**: **Undirected Cycle Detection & Backtrack DFS**
* **Problem Statement**: Find any cycle of length $\ge 3$ in an undirected graph starting and ending at the same city.
* **Algorithm**: DFS tracking `parent`. If neighbor $v \neq \text{parent}[u]$ is visited, cycle found! Trace $u \to v$.
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
bool dfs(int u, int p) {
    vis[u] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (!vis[v]) { parent[v] = u; if (dfs(v, u)) return true; }
        else { cycleStart = v; cycleEnd = u; return true; }
    }
    return false;
}
```

---

### 7. Monsters
* **Classic Pattern**: **Dual Multi-Source BFS (Simultaneous Spreading)**
* **Problem Statement**: Escape from cell `'A'` to any border cell before any monster `'M'` catches you.
* **Algorithm**: (1) Push all monsters into queue $\implies$ `monsterDist[r][c]`. (2) BFS player `'A'` $\implies$ `playerDist[r][c] < monsterDist[r][c]`.
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
// Minimal Snippet
// 1. Monsters BFS
for (auto& m : monsters) { qM.push(m); monsterDist[m.r][m.c] = 0; }
while (!qM.empty()) { /* Standard BFS filling monsterDist */ }

// 2. Player BFS
qA.push(playerStart); playerDist[start.r][start.c] = 0;
while (!qA.empty()) {
    auto [r, c] = qA.front(); qA.pop();
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i], nc = c + dc[i];
        if (valid(nr, nc) && playerDist[r][c] + 1 < monsterDist[nr][nc]) {
            playerDist[nr][nc] = playerDist[r][c] + 1; qA.push({nr, nc});
        }
    }
}
```

---

### 8. Shortest Routes I & 9. Shortest Routes II

#### 8. Shortest Routes I
* **Classic Pattern**: **Single-Source Dijkstra ($O(E \log V)$)**
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
// Minimal Dijkstra Snippet
pq.push({0, src}); dist[src] = 0;
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d > dist[u]) continue;
    for (auto [v, w] : adj[u])
        if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
}
```

#### 9. Shortest Routes II
* **Classic Pattern**: **All-Pairs Floyd-Warshall ($O(V^3)$)**
* **Complexities**: $O(V^3)$ Time | $O(V^2)$ Space

```cpp
// Minimal Floyd-Warshall Snippet
for (int k = 1; k <= n; k++)
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (dist[i][k] < INF && dist[k][j] < INF)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
```

---

### 10. High Score
* **Classic Pattern**: **Bellman-Ford Max Path + Positive Cycle Reachability Check**
* **Problem Statement**: Maximize path score from node $1 \to N$. Output `-1` if an infinite score is reachable.
* **Algorithm**: Negate edge weights. Run Bellman-Ford $V$ times. If any node relaxes on $V$-th pass and is reachable from $1$ and can reach $N$, output `-1`.
* **Complexities**: $O(V \cdot E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
for (int i = 1; i <= n; i++) {
    for (auto& e : edges) {
        if (dist[e.u] + e.w < dist[e.v]) {
            dist[e.v] = dist[e.u] + e.w;
            if (i == n && reachableFrom1[e.v] && reachesN[e.v]) return -1; // Infinite score!
        }
    }
}
```

---

### 11. Flight Discount
* **Classic Pattern**: **Dual Dijkstra / State-Space Dijkstra**
* **Problem Statement**: Min cost from $1 \to N$ with 1 coupon to halve an edge weight.
* **Algorithm**: `dist1` from $1$ on $G$, `dist2` from $N$ on $G^R$. `ans = min(dist1[u] + w / 2 + dist2[v])`.
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
auto dist1 = dijkstra(1, adj);   // 1 -> u on G
auto dist2 = dijkstra(n, revAdj); // v -> n on GR
long long ans = 1e18;
for (auto& e : edges) ans = min(ans, dist1[e.u] + e.w / 2 + dist2[e.v]);
```

---

### 12. Cycle Finding
* **Classic Pattern**: **Bellman-Ford Negative Cycle Detection**
* **Problem Statement**: Find any negative-weight cycle in a directed graph and print node sequence.
* **Algorithm**: Relax edges $V$ times. If node $x$ relaxes on $V$-th pass, backtrack parent pointers $V$ times to enter cycle.
* **Complexities**: $O(V \cdot E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
for (int i = 1; i <= n; i++) {
    lastRelaxed = -1;
    for (auto& e : edges)
        if (dist[e.u] + e.w < dist[e.v]) { dist[e.v] = dist[e.u] + e.w; parent[e.v] = e.u; lastRelaxed = e.v; }
}
if (lastRelaxed == -1) return "NO";
int cycleStart = lastRelaxed;
for (int i = 0; i < n; i++) cycleStart = parent[cycleStart]; // Step inside cycle
// Backtrack cycleStart -> ... -> cycleStart
```

---

### 13. Flight Routes ($K$ Shortest Paths)
* **Classic Pattern**: **$K$-Shortest Paths Dijkstra**
* **Problem Statement**: Find lengths of the $K$ shortest paths from node $1$ to node $N$.
* **Algorithm**: Maintain max-heap `dist[u]` of size $\le K$ for each node $u$. Push path length into queue when smaller than `dist[v].top()`.
* **Complexities**: $O(K \cdot E \log(V \cdot K))$ Time | $O(K \cdot V)$ Space

```cpp
// Minimal Snippet
dist[1].push(0); pq.push({0, 1});
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d > dist[u].top()) continue;
    for (auto [v, w] : adj[u]) {
        long long nextD = d + w;
        if (dist[v].size() < k) { dist[v].push(nextD); pq.push({nextD, v}); }
        else if (nextD < dist[v].top()) { dist[v].pop(); dist[v].push(nextD); pq.push({nextD, v}); }
    }
}
```

---

### 14. Round Trip II
* **Classic Pattern**: **Directed Cycle Detection (3-State / In-Path DFS)**
* **Problem Statement**: Find any directed cycle in a directed graph.
* **Algorithm**: DFS with 3 states (`0=Unvisited, 1=Visiting, 2=Visited`). If neighbor state is `1` $\implies$ Back-edge cycle!
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
bool dfs(int u) {
    state[u] = 1; // Active in current path
    for (int v : adj[u]) {
        if (state[v] == 0) { parent[v] = u; if (dfs(v)) return true; }
        else if (state[v] == 1) { cycleStart = v; cycleEnd = u; return true; }
    }
    state[u] = 2; return false;
}
```

---

### 15. Course Schedule & 16. Longest Flight Route & 17. Game Routes

#### 15. Course Schedule
* **Classic Pattern**: **Kahn's Topological Sort (In-Degree Queue)**
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Kahn's Topo Snippet
for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);
while (!q.empty()) {
    int u = q.front(); q.pop(); topo.push_back(u);
    for (int v : adj[u]) if (--inDegree[v] == 0) q.push(v);
}
```

#### 16. Longest Flight Route & 17. Game Routes (DAG DP)
```cpp
// Minimal Topo DP Snippet (Path Counting)
for (int u : topoOrder) {
    for (int v : adj[u]) ways[v] = (ways[v] + ways[u]) % MOD;
}
```

---

### 18. Investigation
* **Classic Pattern**: **Multi-Property Dijkstra State Propagation**
* **Problem Statement**: Calculate min dist $1 \to N$, number of min-dist paths, min edge count, and max edge count.
* **Algorithm**: Propagate 4 states in Dijkstra when relaxing edges.
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
if (dist[u] + w < dist[v]) {
    dist[v] = dist[u] + w; ways[v] = ways[u];
    minEdges[v] = minEdges[u] + 1; maxEdges[v] = maxEdges[u] + 1;
    pq.push({dist[v], v});
} else if (dist[u] + w == dist[v]) {
    ways[v] = (ways[v] + ways[u]) % MOD;
    minEdges[v] = min(minEdges[v], minEdges[u] + 1);
    maxEdges[v] = max(maxEdges[v], maxEdges[u] + 1);
}
```

---

### 19. Planets Queries I & 20. Planets Cycles

#### 19. Planets Queries I
* **Classic Pattern**: **Binary Lifting (`up[u][j]`) for $K$-th Successor**
* **Complexities**: $O(N \log K + Q \log K)$ Time | $O(N \log K)$ Space

```cpp
// Minimal Binary Lifting Jump Snippet
for (int j = 1; j < 30; j++)
    for (int i = 1; i <= n; i++) up[i][j] = up[up[i][j - 1]][j - 1];

int jump(int u, int k) {
    for (int j = 0; j < 30; j++) if ((k >> j) & 1) u = up[u][j];
    return u;
}
```

#### 20. Planets Cycles
```cpp
// Minimal Functional Graph Cycle Snippet
// Nodes in cycle: ans[u] = cycle_len; Nodes in tree: ans[u] = tree_depth + cycle_len;
```

---

### 21. Road Reparation & 22. Road Construction

#### 21. Road Reparation (Kruskal MST)
* **Classic Pattern**: **Minimum Spanning Tree (Kruskal)**
* **Complexities**: $O(E \log E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Kruskal Snippet
sort(edges.begin(), edges.end());
for (auto& e : edges) {
    if (dsu.unite(e.u, e.v)) { mstWeight += e.w; count++; }
}
```

#### 22. Road Construction
```cpp
// Minimal DSU Component Tracker Snippet
if (dsu.unite(u, v)) { components--; maxSize = max(maxSize, dsu.size(u)); }
```

---

### 23. Flight Routes Check & 24. Planets and Kingdoms & 25. Coin Collector

#### 24. Planets and Kingdoms (Kosaraju SCC)
* **Classic Pattern**: **Strongly Connected Components (Kosaraju 2-Pass DFS)**
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Kosaraju Snippet
void dfs1(int u) { vis[u] = true; for (int v : adj[u]) if (!vis[v]) dfs1(v); order.push_back(u); }
void dfs2(int u, int c) { comp[u] = c; for (int v : revAdj[u]) if (comp[v] == -1) dfs2(v, c); }
// Run dfs1 on all nodes -> Run dfs2 on order in reverse
```

---

### 26. Mail Delivery & 27. Teleporters Path & 28. De Bruijn Sequence

#### 26. Mail Delivery (Hierholzer's Algorithm)
* **Classic Pattern**: **Undirected Eulerian Circuit**
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Hierholzer Snippet
void eulerDFS(int u) {
    while (!adj[u].empty()) {
        auto [v, id] = adj[u].back(); adj[u].pop_back();
        if (usedEdge[id]) continue;
        usedEdge[id] = true; eulerDFS(v);
    }
    circuit.push_back(u);
}
```

---

### 29. Hamiltonian Flights
* **Classic Pattern**: **Bitmask DP / Traveling Salesperson Problem (TSP)**
* **Complexities**: $O(N^2 \cdot 2^N)$ Time | $O(N \cdot 2^N)$ Space

```cpp
// Minimal Bitmask DP Snippet
dp[1][0] = 1; // start at node 0 with mask 1
for (int mask = 1; mask < (1 << n); mask++) {
    for (int u = 0; u < n; u++) {
        if (!(mask & (1 << u)) || !dp[mask][u]) continue;
        for (int v : adj[u])
            if (!(mask & (1 << v))) dp[mask | (1 << v)][v] = (dp[mask | (1 << v)][v] + dp[mask][u]) % MOD;
    }
}
```

---

### 30. Graph Girth
* **Classic Pattern**: **Shortest Unweighted Cycle (BFS per Node)**
* **Complexities**: $O(V \cdot (V + E))$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
for (int src = 1; src <= n; src++) {
    // Run BFS from src. If neighbor v is visited and v != parent[u]:
    girth = min(girth, dist[u] + dist[v] + 1);
}
```

---

### 31. Course Schedule II
* **Classic Pattern**: **Lexicographical Topo Sort (Reverse Graph + Max-PQ)**
* **Complexities**: $O(V \log V + E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
priority_queue<int> maxPQ; // Push in-degree 0 nodes of REVERSE graph
while (!maxPQ.empty()) {
    int u = maxPQ.top(); maxPQ.pop(); order.push_back(u);
    for (int v : revAdj[u]) if (--inDegree[v] == 0) maxPQ.push(v);
}
reverse(order.begin(), order.end());
```

---

### 32. Strongly Connected Edges
* **Classic Pattern**: **Tarjan Bridge Check + DFS Tree/Back-Edge Orientation**
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
// Tree edge: orient u -> v. Back-edge: orient u -> ancestor.
// If low[v] > tin[u] => Bridge exists (Impossible).
```

---

### 33. Acyclic Graph Edges
* **Classic Pattern**: **Vertex Index Orientation ($u < v \implies u \to v$)**
* **Complexities**: $O(V + E)$ Time | $O(1)$ Space

```cpp
// Minimal Snippet
for (auto& [u, v] : edges) oriented.push_back(u < v ? pair{u, v} : pair{v, u});
```

---

### 34. Network Breakdown (Offline Reverse Queries)
* **Classic Pattern**: **Offline Reverse Query Processing via DSU**
* **Complexities**: $O((V + Q) \cdot \alpha(V))$ Time | $O(V + Q)$ Space

```cpp
// Minimal Snippet
// Process queries backward: add deleted edge to DSU, record numComponents.
```

---

### 35. Network Renovation
* **Classic Pattern**: **Tree Leaf Pair Matching ($i \leftrightarrow i + \lfloor L/2 \rfloor$)**
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
for (int i = 0; i < (L + 1) / 2; i++)
    newEdges.push_back({leaves[i], leaves[(i + L / 2) % L]});
```

---

### 36. Visiting Cities
* **Classic Pattern**: **Shortest Path Subgraph + Double Hashing**
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
// Minimal Snippet
// Node u is mandatory iff (distF[u] + distR[u] == min_dist) AND (waysF[u] * waysR[u] % MOD == totalWays % MOD)
```

---

### 37. MST Edge Check & MST Edge Cost
* **Classic Pattern**: **Binary Lifting Max Edge Weight on MST**
* **Complexities**: $O(N \log N + Q \log N)$ Time | $O(N \log N)$ Space

```cpp
// Minimal Snippet
// New MST cost with forced edge (u, v, w) = MST_Cost + w - queryMaxEdgeOnMST(u, v)
```

---

### 38. Transfer Speeds Sum
* **Classic Pattern**: **Kruskal MST DSU Bottleneck Sum**
* **Complexities**: $O(E \log E)$ Time | $O(V)$ Space

```cpp
// Minimal Snippet
sort(edges.rbegin(), edges.rend()); // Descending order
for (auto& e : edges) {
    if (dsu.unite(e.u, e.v)) totalSum += (long long)sizeU * sizeV * e.w;
}
```

---

### 39. Reachable Nodes
* **Classic Pattern**: **Bitset Topological Sort DP**
* **Complexities**: $O\Big(\frac{V \cdot (V + E)}{64}\Big)$ Time | $O\Big(\frac{V \cdot N}{64}\Big)$ Space

```cpp
// Minimal Snippet
for (int u : reverseTopoOrder) {
    dp[u].set(u);
    for (int v : adj[u]) dp[u] |= dp[v];
    ans[u] = dp[u].count();
}
```

---

### 40. Eulerian Subgraphs Formula
* **Classic Pattern**: **Combinatorial Graph Cycle Space Formula**
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
// Minimal Formula Snippet
// Count = 2^(E - V + C) % (10^9 + 7), where C = number of connected components
```

---

### 41. Coin Grid (Bipartite Min Vertex Cover)
* **Classic Pattern**: **Kőnig's Theorem & Bipartite Matching (Kuhn's DFS)**
* **Complexities**: $O(V \cdot E)$ Time | $O(V + E)$ Space

```cpp
// Minimal Kuhn Matching Snippet
bool augment(int u) {
    if (vis[u]) return false; vis[u] = true;
    for (int v : adj[u])
        if (matchR[v] == 0 || augment(matchR[v])) { matchR[v] = u; return true; }
    return false;
}
// Min Row/Col clears = Max Bipartite Matching = Min Vertex Cover
```

---

*End of CSES Graph Algorithms & Advanced Problems Master Reference*
