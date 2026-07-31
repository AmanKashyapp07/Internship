# 🕸️ CSES Graph Algorithms & Advanced Problems - Master Reference

> **All 41 CSES Graph & Advanced Graph Problems complete with Problem Statements, Patterns, C++ Core Code Templates, Time/Space Complexities, and Key Insights.**

---

## Quick Reference Summary Table

| Problem | Primary Pattern / Algorithm | Time | Space |
| :--- | :--- | :--- | :--- |
| **Counting Rooms** | 2D Grid Flood Fill (BFS/DFS) | $O(N \cdot M)$ | $O(N \cdot M)$ |
| **Labyrinth** | 2D Grid BFS + Path Reconstruction (`U,D,L,R`) | $O(N \cdot M)$ | $O(N \cdot M)$ |
| **Building Roads** | Connected Components / DSU | $O(V + E \cdot \alpha(V))$ | $O(V)$ |
| **Message Route** | Unweighted Shortest Path BFS + Parent Trace | $O(V + E)$ | $O(V)$ |
| **Building Teams** | Bipartite 2-Coloring (DFS/BFS) | $O(V + E)$ | $O(V)$ |
| **Round Trip** | Undirected Cycle Detection & Reconstruction | $O(V + E)$ | $O(V)$ |
| **Monsters** | Dual-Layer Multi-Source BFS | $O(N \cdot M)$ | $O(N \cdot M)$ |
| **Shortest Routes I** | Single-Source Dijkstra | $O(E \log V)$ | $O(V + E)$ |
| **Shortest Routes II** | All-Pairs Floyd-Warshall | $O(V^3)$ | $O(V^2)$ |
| **High Score** | Bellman-Ford Max Path + Reachable Positive Cycle | $O(V \cdot E)$ | $O(V + E)$ |
| **Flight Discount** | Dual Dijkstra ($1 \to U$ on $G$, $V \to N$ on $G^R$, Edge $W/2$) | $O(E \log V)$ | $O(V + E)$ |
| **Cycle Finding** | Bellman-Ford Negative Cycle + $N$-pass Backtrack | $O(V \cdot E)$ | $O(V + E)$ |
| **Flight Routes** | $K$ Shortest Paths Dijkstra (bounded PQ per node) | $O(K \cdot E \log(V \cdot K))$ | $O(K \cdot V)$ |
| **Round Trip II** | Directed Cycle Detection (`inPath` / 3-state DFS) | $O(V + E)$ | $O(V)$ |
| **Course Schedule** | Kahn's Topological Sort | $O(V + E)$ | $O(V + E)$ |
| **Longest Flight Route** | DAG Longest Path via Topo DP | $O(V + E)$ | $O(V + E)$ |
| **Game Routes** | DAG Path Counting via Topo DP | $O(V + E)$ | $O(V + E)$ |
| **Investigation** | Dijkstra DP (Dist, Ways, Min Edges, Max Edges) | $O(E \log V)$ | $O(V + E)$ |
| **Planets Queries I** | Binary Lifting (`up[u][j]`) for $K$-th Successor | $O(N \log K + Q \log K)$ | $O(N \log K)$ |
| **Planets Cycles** | Functional Graph (Floyd Cycle + Tree Distance) | $O(N)$ | $O(N)$ |
| **Road Reparation** | Minimum Spanning Tree (Kruskal / Prim) | $O(E \log E)$ | $O(V + E)$ |
| **Road Construction** | DSU tracking Components & Max Size | $O(E \cdot \alpha(V))$ | $O(V)$ |
| **Flight Routes Check** | Strongly Connected Check (BFS from 1 on $G$ & $G^R$) | $O(V + E)$ | $O(V + E)$ |
| **Planets and Kingdoms** | Strongly Connected Components (Kosaraju 2-Pass) | $O(V + E)$ | $O(V + E)$ |
| **Coin Collector** | SCC Condensation DAG + Topo DP | $O(V + E)$ | $O(V + E)$ |
| **Mail Delivery** | Undirected Eulerian Circuit (Hierholzer) | $O(V + E)$ | $O(V + E)$ |
| **Teleporters Path** | Directed Eulerian Path $1 \to N$ (Hierholzer) | $O(V + E)$ | $O(V + E)$ |
| **De Bruijn Sequence** | Eulerian Circuit on $2^{n-1}$ de Bruijn Graph | $O(2^n)$ | $O(2^n)$ |
| **Hamiltonian Flights** | Bitmask DP ($dp[mask][u]$ for TSP / Path) | $O(N^2 \cdot 2^N)$ | $O(N \cdot 2^N)$ |
| **Graph Girth** | Shortest Cycle in Unweighted Graph (BFS per node) | $O(V \cdot (V + E))$ | $O(V + E)$ |
| **Course Schedule II** | Lexicographical Smallest Topo (Reverse Graph + Max-PQ) | $O(V \log V + E)$ | $O(V + E)$ |
| **Strongly Connected Edges**| Bridge Check + DFS Tree/Back-edge Orientation | $O(V + E)$ | $O(V + E)$ |
| **Acyclic Graph Edges** | Direct edge $u \to v$ if $u < v$ | $O(V + E)$ | $O(1)$ |
| **Network Breakdown** | Offline Reverse Query Processing via DSU | $O((V + Q) \cdot \alpha(V))$ | $O(V + Q)$ |
| **Network Renovation** | Tree Leaf Pair Matching ($i \leftrightarrow i + L/2$) | $O(N)$ | $O(N)$ |
| **Visiting Cities** | Shortest Path Subgraph + Double Hashing | $O(E \log V)$ | $O(V + E)$ |
| **MST Edge Check / Cost** | Binary Lifting Max Edge Weight on MST Path | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| **Transfer Speeds Sum** | Kruskal MST DSU Bottleneck Sum ($sz[u] \cdot sz[v] \cdot w$) | $O(E \log E)$ | $O(V)$ |
| **Tree Isomorphism I & II** | Rooted AHU Hashing / Centroid Tree Hashing | $O(N \log N)$ | $O(N)$ |
| **Reachable Nodes** | Bitset Topological Sort DP | $O(V \cdot (V + E) / 64)$ | $O(V \cdot N / 64)$ |
| **Eulerian Subgraphs** | $2^{M - N + C}$ count formula | $O(V + E)$ | $O(V)$ |
| **Coin Grid** | Bipartite Min Vertex Cover (Kőnig's Theorem & Kuhn) | $O(V \cdot E)$ | $O(V + E)$ |

---

## Part 1: CSES Standard Graph Problems & Code Templates

### 1. Counting Rooms
* **Problem Statement**: Given an $N \times M$ grid representing a building map with floor cells (`.`) and wall cells (`#`), count the total number of distinct rooms (connected components of floor cells).
* **Pattern**: **2D Grid Flood Fill (BFS/DFS)**
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
void floodFill(int r, int c, int n, int m, vector<string>& grid) {
    if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] == '#') return;
    grid[r][c] = '#';
    floodFill(r + 1, c, n, m, grid); floodFill(r - 1, c, n, m, grid);
    floodFill(r, c + 1, n, m, grid); floodFill(r, c - 1, n, m, grid);
}
int countRooms(int n, int m, vector<string>& grid) {
    int rooms = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (grid[i][j] == '.') { rooms++; floodFill(i, j, n, m, grid); }
    return rooms;
}
```

---

### 2. Labyrinth
* **Problem Statement**: Given an $N \times M$ grid containing start cell `'A'` and end cell `'B'`, find the shortest path from `'A'` to `'B'`, printing the path length and the sequence of moves (`U, D, L, R`). Print `"NO"` if unreachable.
* **Pattern**: **2D Grid BFS + Parent Pointer Direction Tracking**
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

```cpp
string solveLabyrinth(int n, int m, int startR, int startC, int endR, int endC, vector<string>& grid) {
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    vector<vector<pair<int,int>>> par(n, vector<pair<int,int>>(m));
    vector<vector<char>> parDir(n, vector<char>(m));
    queue<pair<int,int>> q; q.push({startR, startC}); vis[startR][startC] = true;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1}; char dir[] = {'U', 'D', 'L', 'R'};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == endR && c == endC) break;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] != '#' && !vis[nr][nc]) {
                vis[nr][nc] = true; parDir[nr][nc] = dir[i]; par[nr][nc] = {r, c}; q.push({nr, nc});
            }
        }
    }
    if (!vis[endR][endC]) return "NO";
    string path = ""; int currR = endR, currC = endC;
    while (currR != startR || currC != startC) {
        path += parDir[currR][currC];
        auto [pr, pc] = par[currR][currC]; currR = pr; currC = pc;
    }
    reverse(path.begin(), path.end());
    return "YES\n" + to_string(path.size()) + "\n" + path;
}
```

---

### 3. Building Roads
* **Problem Statement**: Given $N$ cities and $M$ existing roads, find the minimum number of new roads required to connect all cities into a single connected network, and list the exact roads to construct.
* **Pattern**: **Disjoint Set Union (DSU) / Connected Components**
* **Complexities**: $O(V + E \cdot \alpha(V))$ Time | $O(V)$ Space

```cpp
struct DSU {
    vector<int> p; DSU(int n) : p(n + 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[b] = a; return true;
    }
};
vector<pair<int,int>> connectComponents(int n, vector<pair<int,int>>& edges) {
    DSU dsu(n); for (auto& [u, v] : edges) dsu.unite(u, v);
    vector<int> roots;
    for (int i = 1; i <= n; i++) if (dsu.p[i] == i) roots.push_back(i);
    vector<pair<int,int>> newRoads;
    for (int i = 0; i < (int)roots.size() - 1; i++) newRoads.push_back({roots[i], roots[i + 1]});
    return newRoads;
}
```

---

### 4. Message Route
* **Problem Statement**: Given a network of $N$ computers and $M$ unweighted connections, find the minimum number of computers in a valid transmission path from computer $1$ to computer $N$, and output the path sequence.
* **Pattern**: **Unweighted Graph Shortest Path (BFS)**
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
vector<int> messageRoute(int n, const vector<vector<int>>& g) {
    vector<int> dist(n + 1, -1), parent(n + 1, -1);
    queue<int> q; q.push(1); dist[1] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) { dist[v] = dist[u] + 1; parent[v] = u; q.push(v); }
        }
    }
    if (dist[n] == -1) return {};
    vector<int> path; for (int u = n; u != -1; u = parent[u]) path.push_back(u);
    reverse(path.begin(), path.end()); return path;
}
```

---

### 5. Building Teams
* **Problem Statement**: Given $N$ pupils and $M$ friendship pairs, partition the pupils into two teams such that no two friends end up in the same team (Bipartite 2-Coloring), or output `"IMPOSSIBLE"`.
* **Pattern**: **Bipartite Graph 2-Coloring (DFS)**
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

```cpp
bool dfsColor(int u, int c, const vector<vector<int>>& g, vector<int>& color) {
    color[u] = c;
    for (int v : g[u]) {
        if (color[v] == 0) { if (!dfsColor(v, 3 - c, g, color)) return false; }
        else if (color[v] == c) return false;
    }
    return true;
}
```

---

### 6. Round Trip & 14. Round Trip II & 12. Cycle Finding (Graph Cycles Suite)
* **Problem Statements**:
  - *Round Trip*: Find any cycle in an undirected graph starting and ending at the same city with $\ge 3$ edges.
  - *Round Trip II*: Find any directed cycle in a directed graph of flight routes.
  - *Cycle Finding*: Find any negative-weight cycle in a directed weighted graph and print its node sequence.
* **Pattern**: **DFS Cycle Backtracking & Bellman-Ford Negative Cycle Detection**
* **Complexities**: $O(V + E)$ for DFS cycles | $O(V \cdot E)$ for negative cycle

```cpp
vector<int> buildCycle(int s, int e, const vector<int>& par) {
    vector<int> cyc; for (int u = e; u != s; u = par[u]) cyc.push_back(u);
    cyc.push_back(s); reverse(cyc.begin(), cyc.end()); return cyc;
}
class UndirectedCycle {
    int n, s = -1, e = -1; vector<vector<int>> g; vector<int> vis, par;
    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) { par[v] = u; if (dfs(v, u)) return true; }
            else { s = v, e = u; return true; }
        }
        return false;
    }
public:
    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i, -1)) return buildCycle(s, e, par);
        return {};
    }
};

class DirectedCycle {
    int n, s = -1, e = -1; vector<vector<int>> g; vector<int> vis, inPath, par;
    bool dfs(int u) {
        vis[u] = inPath[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) { par[v] = u; if (dfs(v)) return true; }
            else if (inPath[v]) { s = v, e = u; return true; }
        }
        inPath[u] = 0; return false;
    }
public:
    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), inPath(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); }
    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i)) return buildCycle(s, e, par);
        return {};
    }
};

struct Edge { int u, v; long long w; };
vector<int> findNegativeCycle(int n, const vector<Edge>& edges) {
    vector<long long> dist(n + 1, 0); vector<int> parent(n + 1, -1); int lastRelaxed = -1;
    for (int i = 1; i <= n; i++) {
        lastRelaxed = -1;
        for (const auto& e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) { dist[e.v] = dist[e.u] + e.w; parent[e.v] = e.u; lastRelaxed = e.v; }
        }
    }
    if (lastRelaxed == -1) return {};
    while (n--) lastRelaxed = parent[lastRelaxed];
    return buildCycle(lastRelaxed, parent[lastRelaxed], parent);
}
```

---

### 7. Monsters
* **Problem Statement**: You are trapped in an $N \times M$ grid with monsters moving simultaneously to adjacent cells. Escape to any border cell without being caught by monsters (monsters move at the same speed).
* **Pattern**: **Multi-Source BFS (Monsters) + Single-Source BFS (Player)**
* **Complexities**: $O(N \cdot M)$ Time | $O(N \cdot M)$ Space

---

### 8. Shortest Routes I & 9. Shortest Routes II
* **Problem Statements**:
  - *Shortest Routes I*: Calculate the minimum distance from city $1$ to every city $1 \dots N$ in a directed weighted graph.
  - *Shortest Routes II*: Answer $Q$ queries asking for the shortest distance between any pair of cities $(A, B)$.
* **Pattern**: **Dijkstra ($O(E \log V)$) & Floyd-Warshall ($O(V^3)$)**

```cpp
// Shortest Routes I: Dijkstra O(E log V)
vector<long long> dijkstra(int src, int n, const vector<vector<pair<int,int>>>& g) {
    vector<long long> dist(n + 1, 1e18); dist[src] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
        }
    }
    return dist;
}

// Shortest Routes II: Floyd-Warshall O(V^3)
void floydWarshall(int n, vector<vector<long long>>& dist) {
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (dist[i][k] < 1e18 && dist[k][j] < 1e18)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}
```

---

### 10. High Score
* **Problem Statement**: Find the maximum total score obtainable on a path from node $1$ to node $N$ in a directed graph with edge weights. Output `-1` if an arbitrarily large score is possible via a positive cycle reachable from $1$ and reaching $N$.
* **Pattern**: **Bellman-Ford Max Path with Positive Cycle Reachability Check**
* **Complexities**: $O(V \cdot E)$ Time | $O(V + E)$ Space

---

### 11. Flight Discount
* **Problem Statement**: Find the minimum cost to travel from city $1$ to city $N$ given a single discount coupon that halves the weight ($\lfloor W/2 \rfloor$) of one flight edge.
* **Pattern**: **Dual Dijkstra ($1 \to U$ on $G$, $V \to N$ on $G^R$)**
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
long long minFlightDiscount(int n, vector<Edge>& edges, vector<vector<pair<int,int>>>& g, vector<vector<pair<int,int>>>& rg) {
    auto dist1 = dijkstra(1, g);  // 1 -> u on G
    auto dist2 = dijkstra(n, rg); // v -> n on GR
    long long ans = 1e18;
    for (auto& e : edges) ans = min(ans, dist1[e.u] + e.w / 2 + dist2[e.v]);
    return ans;
}
```

---

### 13. Flight Routes ($K$ Shortest Paths)
* **Problem Statement**: Find the $K$ shortest path lengths from city $1$ to city $N$ in a directed weighted graph.
* **Pattern**: **Dijkstra with Bounded Priority Queues per Node**
* **Complexities**: $O(K \cdot E \log(V \cdot K))$ Time | $O(K \cdot V)$ Space

```cpp
vector<long long> kShortestPaths(int n, int k, const vector<vector<pair<int,int>>>& g) {
    vector<priority_queue<long long>> dist(n + 1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[1].push(0); pq.push({0, 1});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u].top()) continue;
        for (auto [v, w] : g[u]) {
            long long nextD = d + w;
            if (dist[v].size() < k) { dist[v].push(nextD); pq.push({nextD, v}); }
            else if (nextD < dist[v].top()) { dist[v].pop(); dist[v].push(nextD); pq.push({nextD, v}); }
        }
    }
    vector<long long> result;
    while (!dist[n].empty()) { result.push_back(dist[n].top()); dist[n].pop(); }
    reverse(result.begin(), result.end()); return result;
}
```

---

### 15. Course Schedule & 16. Longest Flight Route & 17. Game Routes
* **Problem Statements**:
  - *Course Schedule*: Find a valid topological order to complete $N$ courses given prerequisite constraints.
  - *Longest Flight Route*: Find the longest path from city $1$ to city $N$ in a Directed Acyclic Graph (DAG).
  - *Game Routes*: Count the number of distinct paths from level $1$ to level $N$ in a DAG modulo $10^9+7$.
* **Pattern**: **Kahn's Topological Sort & Topo DP**

```cpp
// Game Routes: Topo DP Path Counting O(V + E)
vector<long long> gameRoutes(int n, const vector<vector<int>>& g, vector<int>& inDeg) {
    queue<int> q; for (int i = 1; i <= n; i++) if (inDeg[i] == 0) q.push(i);
    vector<long long> ways(n + 1, 0); ways[1] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            ways[v] = (ways[v] + ways[u]) % MOD;
            if (--inDeg[v] == 0) q.push(v);
        }
    }
    return ways;
}
```

---

### 18. Investigation
* **Problem Statement**: In a directed weighted graph, calculate: (1) min distance $1 \to N$, (2) number of min-distance paths $\pmod{10^9+7}$, (3) min edge count on a min-distance path, and (4) max edge count on a min-distance path.
* **Pattern**: **Multi-Property Dijkstra State Propagation**
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
void investigation(int n, const vector<vector<pair<int,int>>>& g) {
    vector<long long> dist(n + 1, 1e18), ways(n + 1, 0);
    vector<int> minEdges(n + 1, 1e9), maxEdges(n + 1, 0);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[1] = 0; ways[1] = 1; minEdges[1] = 0; maxEdges[1] = 0; pq.push({0, 1});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; ways[v] = ways[u];
                minEdges[v] = minEdges[u] + 1; maxEdges[v] = maxEdges[u] + 1;
                pq.push({dist[v], v});
            } else if (dist[u] + w == dist[v]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
                minEdges[v] = min(minEdges[v], minEdges[u] + 1);
                maxEdges[v] = max(maxEdges[v], maxEdges[u] + 1);
            }
        }
    }
}
```

---

### 19. Planets Queries I & 20. Planets Cycles
* **Problem Statements**:
  - *Planets Queries I*: In a functional graph (out-degree $= 1$), answer $Q$ queries: which planet do you reach after taking $K$ teleports starting from $X$?
  - *Planets Cycles*: For each planet $i$, count how many teleports you will take before landing on a previously visited planet.
* **Pattern**: **Binary Lifting & Functional Graph Floyd Cycle Decomposition**

```cpp
struct BinaryLiftingJumps {
    int LOG = 30; vector<vector<int>> up;
    BinaryLiftingJumps(int n, const vector<int>& succ) : up(n + 1, vector<int>(LOG)) {
        for (int i = 1; i <= n; i++) up[i][0] = succ[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 1; i <= n; i++) up[i][j] = up[up[i][j - 1]][j - 1];
    }
    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++) if ((k >> j) & 1) u = up[u][j];
        return u;
    }
};
```

---

### 21. Road Reparation & 22. Road Construction
* **Problem Statements**:
  - *Road Reparation*: Find the minimum total cost to repair roads so all cities are connected (Minimum Spanning Tree).
  - *Road Construction*: Start with $N$ isolated cities. Process $M$ road additions one by one, printing the number of connected components and max component size after each road.
* **Pattern**: **Kruskal's MST & DSU Component Tracking**

```cpp
long long kruskalMST(int n, vector<Edge>& edges) {
    DSU dsu(n); sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    long long mstCost = 0; int cnt = 0;
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) { mstCost += e.w; cnt++; }
    }
    return cnt == n - 1 ? mstCost : -1;
}
```

---

### 23. Flight Routes Check & 24. Planets and Kingdoms & 25. Coin Collector
* **Problem Statements**:
  - *Flight Routes Check*: Check if a directed graph is strongly connected. Print `"NO"` with a counterexample pair if not.
  - *Planets and Kingdoms*: Divide planets into kingdoms such that two planets are in the same kingdom iff they can reach each other (SCCs).
  - *Coin Collector*: Maximize collected coins in a directed graph of rooms by visiting rooms and using one-way tunnels (SCC Condensation DAG Max Path).
* **Pattern**: **Kosaraju's 2-Pass DFS for Strongly Connected Components**

```cpp
struct KosarajuSCC {
    int n; vector<vector<int>> g, rg; vector<bool> vis; vector<int> order, comp;
    KosarajuSCC(int n) : n(n), g(n + 1), rg(n + 1), vis(n + 1), comp(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); rg[v].push_back(u); }
    void dfs1(int u) { vis[u] = true; for (int v : g[u]) if (!vis[v]) dfs1(v); order.push_back(u); }
    void dfs2(int u, int c) { comp[u] = c; for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c); }
    int build() {
        for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
        int c = 0; for (int i = n - 1; i >= 0; i--) if (comp[order[i]] == -1) dfs2(order[i], ++c);
        return c; // Total SCC components!
    }
};
```

---

### 26. Mail Delivery & 27. Teleporters Path & 28. De Bruijn Sequence
* **Problem Statements**:
  - *Mail Delivery*: Find an Eulerian Circuit in an undirected graph starting and ending at crossing 1 traversing every edge once.
  - *Teleporters Path*: Find an Eulerian Path in a directed graph starting at level 1 and ending at level $N$ traversing every edge once.
  - *De Bruijn Sequence*: Construct the shortest binary string containing all binary strings of length $N$ as substrings.
* **Pattern**: **Hierholzer's Eulerian Path/Circuit Algorithm ($O(V + E)$)**

```cpp
void eulerDFS(int u, vector<vector<pair<int,int>>>& g, vector<bool>& usedEdge, vector<int>& path) {
    while (!g[u].empty()) {
        auto [v, id] = g[u].back(); g[u].pop_back();
        if (usedEdge[id]) continue;
        usedEdge[id] = true; eulerDFS(v, g, usedEdge, path);
    }
    path.push_back(u);
}
```

---

### 29. Hamiltonian Flights
* **Problem Statement**: Count the number of valid paths from city $1$ to city $N$ that visit **every** city in the graph exactly once modulo $10^9+7$.
* **Pattern**: **Bitmask DP / Traveling Salesperson Problem (TSP)**
* **Complexities**: $O(N^2 \cdot 2^N)$ Time | $O(N \cdot 2^N)$ Space

```cpp
long long hamiltonianFlights(int n, const vector<vector<int>>& g) {
    vector<vector<long long>> dp(1 << n, vector<long long>(n, 0)); dp[1][0] = 1;
    for (int mask = 1; mask < (1 << n); mask++) {
        if (!(mask & 1)) continue;
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u)) || dp[mask][u] == 0) continue;
            for (int v : g[u]) {
                if (!(mask & (1 << v))) dp[mask | (1 << v)][v] = (dp[mask | (1 << v)][v] + dp[mask][u]) % MOD;
            }
        }
    }
    return dp[(1 << n) - 1][n - 1];
}
```

---

## Part 2: CSES Advanced Graph Problems & Complete Code Templates

### 30. Graph Girth
* **Problem Statement**: Find the length of the shortest cycle in an unweighted undirected graph, or return `-1` if the graph is acyclic.
* **Pattern**: **BFS from Every Node**
* **Complexities**: $O(V \cdot (V + E))$ Time | $O(V + E)$ Space

```cpp
int findGraphGirth(int n, const vector<vector<int>>& g) {
    int girth = INT_MAX;
    for (int src = 1; src <= n; src++) {
        vector<int> dist(n + 1, -1), par(n + 1, -1);
        queue<int> q; q.push(src); dist[src] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (dist[v] == -1) { dist[v] = dist[u] + 1; par[v] = u; q.push(v); }
                else if (par[u] != v) girth = min(girth, dist[u] + dist[v] + 1);
            }
        }
    }
    return girth == INT_MAX ? -1 : girth;
}
```

---

### 31. Course Schedule II
* **Problem Statement**: Find a topological sort of a Directed Acyclic Graph (DAG) such that the resulting course sequence is **lexicographically smallest**.
* **Pattern**: **Reverse Graph + Kahn's Algorithm with Max-Priority Queue**
* **Complexities**: $O(V \log V + E)$ Time | $O(V + E)$ Space

```cpp
vector<int> lexicographicalTopoSort(int n, const vector<vector<int>>& revG, vector<int>& inDeg) {
    priority_queue<int> maxPQ; for (int i = 1; i <= n; i++) if (inDeg[i] == 0) maxPQ.push(i);
    vector<int> order;
    while (!maxPQ.empty()) {
        int u = maxPQ.top(); maxPQ.pop(); order.push_back(u);
        for (int v : revG[u]) if (--inDeg[v] == 0) maxPQ.push(v);
    }
    reverse(order.begin(), order.end()); return order;
}
```

---

### 32. Strongly Connected Edges
* **Problem Statement**: Given an undirected connected graph, orient every edge to make the resulting directed graph strongly connected, or output `"IMPOSSIBLE"` if a bridge exists.
* **Pattern**: **Bridge Detection + DFS Tree/Back-edge Orientation**
* **Complexities**: $O(V + E)$ Time | $O(V + E)$ Space

```cpp
void orientEdgesDFS(int u, int pEdge, int& timer, const vector<vector<pair<int,int>>>& g, vector<int>& tin, vector<int>& low, vector<bool>& vis, vector<pair<int,int>>& ans, bool& hasBridge) {
    vis[u] = true; tin[u] = low[u] = ++timer;
    for (auto [v, id] : g[u]) {
        if (id == pEdge) continue;
        if (!vis[v]) {
            ans[id] = {u, v}; // Orient tree edge u -> v
            orientEdgesDFS(v, id, timer, g, tin, low, vis, ans, hasBridge);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) hasBridge = true;
        } else {
            low[u] = min(low[u], tin[v]);
            if (tin[v] < tin[u]) ans[id] = {u, v}; // Orient back-edge u -> v
        }
    }
}
```

---

### 33. Acyclic Graph Edges
* **Problem Statement**: Given an undirected graph, orient every edge to convert it into a Directed Acyclic Graph (DAG) without introducing any cycles.
* **Pattern**: **Vertex Index Ordering ($u < v \implies u \to v$)**
* **Complexities**: $O(V + E)$ Time | $O(1)$ Space

```cpp
vector<pair<int,int>> makeAcyclic(const vector<pair<int,int>>& edges) {
    vector<pair<int,int>> oriented;
    for (auto& [u, v] : edges) oriented.push_back(u < v ? pair<int,int>{u, v} : pair<int,int>{v, u});
    return oriented;
}
```

---

### 34. Network Breakdown (Offline Reverse Queries)
* **Problem Statement**: Given a graph of computers and connections, and $Q$ edge removal queries, print the number of connected components remaining after each removal query.
* **Pattern**: **Offline Query Processing via DSU Edge Additions**
* **Complexities**: $O((V + E + Q) \cdot \alpha(V))$ Time | $O(V + E)$ Space

```cpp
// Process queries in REVERSE order starting with initial graph of un-removed edges!
```

---

### 35. Network Renovation
* **Problem Statement**: Find the minimum number of new edges to add to a tree to make it 2-edge-connected (bridge-free).
* **Pattern**: **Tree Leaf Pair Matching ($i \leftrightarrow i + \lfloor L/2 \rfloor$)**
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
vector<pair<int,int>> renovateTree(const vector<int>& leaves) {
    int L = leaves.size(), k = (L + 1) / 2;
    vector<pair<int,int>> addedEdges;
    for (int i = 0; i < k; i++) addedEdges.push_back({leaves[i], leaves[(i + L / 2) % L]});
    return addedEdges;
}
```

---

### 36. Visiting Cities
* **Problem Statement**: Find all cities $X$ that **must** be visited on every shortest path from city $1$ to city $N$.
* **Pattern**: **Shortest Path Subgraph + Double MOD Hashing**
* **Complexities**: $O(E \log V)$ Time | $O(V + E)$ Space

```cpp
// Shortest path subgraph condition: distF[u] + w + distR[v] == distF[N]
// Mandatory city condition: (waysF[u] * waysR[u]) % MOD == totalWays % MOD
```

---

### 37. MST Edge Check & MST Edge Cost
* **Problem Statement**: Answer $Q$ queries: (1) determine if edge $(u, v, w)$ is in SOME MST or EVERY MST, and (2) compute total MST weight if edge $(u, v, w)$ is forced into the spanning tree.
* **Pattern**: **Kruskal MST + Binary Lifting Max Edge Query**
* **Complexities**: $O(N \log N + Q \log N)$ Time | $O(N \log N)$ Space

```cpp
int queryMaxEdgeMST(int u, int v, int LOG, const vector<vector<int>>& up, const vector<vector<int>>& maxEdge, const vector<int>& depth) {
    int ans = 0; if (depth[u] < depth[v]) swap(u, v);
    for (int j = LOG - 1; j >= 0; j--)
        if (depth[u] - (1 << j) >= depth[v]) { ans = max(ans, maxEdge[u][j]); u = up[u][j]; }
    if (u == v) return ans;
    for (int j = LOG - 1; j >= 0; j--)
        if (up[u][j] != up[v][j]) { ans = max({ans, maxEdge[u][j], maxEdge[v][j]}); u = up[u][j]; v = up[v][j]; }
    return max({ans, maxEdge[u][0], maxEdge[v][0]});
}
```

---

### 38. Transfer Speeds Sum
* **Problem Statement**: Compute the sum of maximum bottleneck path capacities over all $\frac{N(N-1)}{2}$ pairs of nodes in an undirected graph.
* **Pattern**: **Kruskal MST DSU Bottleneck Sum**
* **Complexities**: $O(E \log E)$ Time | $O(V)$ Space

```cpp
long long transferSpeedsSum(int n, vector<Edge>& edges) {
    DSU dsu(n); sort(edges.rbegin(), edges.rend(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    long long totalSum = 0;
    for (auto& e : edges) {
        long long szU = dsu.size(e.u), szV = dsu.size(e.v);
        if (dsu.unite(e.u, e.v)) totalSum += szU * szV * e.w;
    }
    return totalSum;
}
```

---

### 39. Reachable Nodes & Reachability Queries
* **Problem Statement**: For each node in a Directed Acyclic Graph (DAG), count how many nodes are reachable from it.
* **Pattern**: **Bitset Topological Sort DP**
* **Complexities**: $O\Big(\frac{V \cdot (V + E)}{64}\Big)$ Time | $O\Big(\frac{V \cdot N}{64}\Big)$ Space

```cpp
vector<int> countReachableNodes(int n, const vector<vector<int>>& g, const vector<int>& reverseTopo) {
    vector<bitset<50000>> dp(n + 1); vector<int> count(n + 1);
    for (int u : reverseTopo) {
        dp[u].set(u);
        for (int v : g[u]) dp[u] |= dp[v];
        count[u] = dp[u].count();
    }
    return count;
}
```

---

### 40. Eulerian Subgraphs Formula
* **Problem Statement**: Given an undirected graph, find the number of subgraphs where every vertex has an **even degree**.
* **Formula**: $\text{Count} = 2^{E - V + C} \pmod{10^9+7}$, where $C$ is the number of connected components.
* **Complexities**: $O(V + E)$ Time | $O(V)$ Space

---

### 41. Coin Grid (Bipartite Min Vertex Cover)
* **Problem Statement**: Given an $N \times N$ grid with coins, clear all coins using the minimum number of row or column clearing operations.
* **Pattern**: **Kőnig's Theorem & Bipartite Matching (Kuhn's DFS)**
* **Reduction**: Rows $1 \dots N$ on Left, Cols $1 \dots N$ on Right. Min Row/Col Clears $=$ Minimum Vertex Cover $=$ Maximum Bipartite Matching.
* **Complexities**: $O(V \cdot E)$ Time | $O(V + E)$ Space

```cpp
bool augmentKuhn(int u, const vector<vector<int>>& g, vector<int>& matchR, vector<bool>& vis) {
    if (vis[u]) return false; vis[u] = true;
    for (int v : g[u]) {
        if (matchR[v] == 0 || augmentKuhn(matchR[v], g, matchR, vis)) {
            matchR[v] = u; return true;
        }
    }
    return false;
}
```

---
*End of CSES Graph Algorithms & Advanced Problems Master Reference*
