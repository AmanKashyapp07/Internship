# Graph Algorithms: Master Interview & OA Guide

## How to Use This Guide

Every graph problem in an OA or interview follows a recognizable pattern. The workflow is:

1. Read the problem and identify the graph type (grid, graphacency list, implicit).
2. Identify what is being asked (reachability, shortest path, ordering, components, etc.).
3. Map it to one of the 13 categories below.
4. Apply the template for that category.

The pattern recognition section at the end consolidates all signal words into a quick lookup table.

---

## Universal Problem-Solving Framework

Before writing any code, answer these five questions:

**Q1: What are the nodes?**
Cells in a grid, integers, strings, states (e.g., bitmasks), cities, courses.

**Q2: What are the edges?**
graphacent cells, dependencies, flights, roads, explicit pairs in input.

**Q3: Is the graph directed or undirected?**
Dependencies, prerequisites, and one-way roads are directed. Friendships, roads (bidirectional), and grids are usually undirected.

**Q4: Are edges weighted?**
If yes, BFS is not sufficient. Use Dijkstra (non-negative weights), Bellman-Ford (negative weights), or Floyd-Warshall (all-pairs).

**Q5: What is the goal?**
- Visit / count reachable nodes -> BFS or DFS
- Shortest path -> BFS (unweighted), Dijkstra, Bellman-Ford
- Order tasks -> Topological Sort
- Group nodes -> DSU or BFS/DFS for components
- Detect cycle -> DFS coloring or DSU

---

## 1. Graph Traversal (BFS / DFS)

### When to Use
Any problem asking you to visit cells or nodes, count regions, spread something, or find reachability.

### Signal Words
"island", "region", "connected", "flood", "spread", "reachable", "shortest path (unweighted)"

### BFS Template (Grid)

```cpp
class Solution {
public:
    void bfs(vector<vector<int>>& grid, int start_r, int start_c) {
        int rows = grid.size();
        int cols = grid[0].size();
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        queue<pair<int, int>> q;

        q.push({start_r, start_c});
        visited[start_r][start_c] = true;
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (auto& dir : dirs) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc] && grid[nr][nc] != 0) {
                    visited[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
    }
};
```

### DFS Template (Grid)

```cpp
class Solution {
public:
    void dfs(vector<vector<int>>& grid, int r, int c, vector<vector<bool>>& visited) {
        int rows = grid.size();
        int cols = grid[0].size();
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            return;
        }
        if (visited[r][c] || grid[r][c] == 0) {
            return;
        }
        visited[r][c] = true;
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (auto& dir : dirs) {
            dfs(grid, r + dir[0], c + dir[1], visited);
        }
    }
};
```

### Key Problems
| Problem | Trick |
|---|---|
| 200. Number of Islands | DFS/BFS each unvisited '1', count calls |
| 994. Rotting Oranges | Multi-source BFS from all rotten oranges at once |
| 542. 01 Matrix | Multi-source BFS from all 0s simultaneously |
| 417. Pacific Atlantic | Reverse BFS from both borders; intersect visited sets |
| 130. Surrounded Regions | Mark border-connected 'O' first, then flip the rest |

### Common Mistakes
- Forgetting to mark visited before pushing to queue (causes duplicates in BFS).
- Using DFS for shortest path (wrong; DFS does not guarantee shortest).
- Not handling multi-source BFS: add all sources to the queue before starting.

---

## 2. Connected Components

### When to Use
Counting groups, merging groups, asking "how many separate clusters exist?"

### Signal Words
"provinces", "groups", "clusters", "how many connected", "merge"

### Template

```cpp
class Solution {
private:
    void dfs(int node, vector<vector<int>>& graph, vector<bool>& visited) {
        visited[node] = true;
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor, graph, visited);
            }
        }
    }

public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
            graph[edge[1]].push_back(edge[0]);
        }

        vector<bool> visited(n, false);
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(i, graph, visited);
                count++;
            }
        }

        return count;
    }
};
```

### Key Problems
| Problem | Trick |
|---|---|
| 547. Number of Provinces | Standard component count on graphacency matrix |
| 1319. Make Network Connected | Need at least n-1 edges; count extras with DSU |

---

## 3. Cycle Detection

### When to Use
Problems asking if a cycle exists, whether a schedule is possible, or finding a redundant edge.

### Signal Words
"cycle", "circular dependency", "redundant", "safe nodes"

### Directed Graph: DFS 3-Color

```cpp
class Solution {
private:
    bool dfs(int node, vector<vector<int>>& graph, vector<int>& color) {
        color[node] = 1; // GRAY: visiting
        for (int neighbor : graph[node]) {
            if (color[neighbor] == 1) {
                return true; // back edge = cycle
            }
            if (color[neighbor] == 0 && dfs(neighbor, graph, color)) {
                return true;
            }
        }
        color[node] = 2; // BLACK: visited
        return false;
    }

public:
    bool hasCycleDirected(int n, vector<vector<int>>& graph) {
        vector<int> color(n, 0); // 0: WHITE (unvisited)
        for (int i = 0; i < n; i++) {
            if (color[i] == 0 && dfs(i, graph, color)) {
                return true;
            }
        }
        return false;
    }
};
```

### Undirected Graph: DSU or DFS with parent tracking

```cpp
class Solution {
private:
    int find(int x, vector<int>& parent) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

public:
    bool hasCycleUndirected(int n, vector<vector<int>>& edges) {
        vector<int> parent(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int pu = find(u, parent);
            int pv = find(v, parent);
            if (pu == pv) {
                return true; // same component = cycle
            }
            parent[pu] = pv;
        }
        return false;
    }
};
```

### Key Problems
| Problem | Trick |
|---|---|
| 207. Course Schedule | Directed cycle detection; return False if cycle found |
| 684. Redundant Connection | Undirected; first edge that creates a cycle is the answer |
| 685. Redundant Connection II | Directed; check in-degree violations + cycle simultaneously |
| 802. Find Eventual Safe States | Nodes not in any cycle are safe; reverse edges + topo sort |

---

## 4. Topological Sort

### When to Use
Tasks with dependencies, ordering courses, "which comes first" problems.

### Signal Words
"prerequisites", "order", "before", "dependency", "schedule"

### Kahn's Algorithm (BFS-based, preferred in interviews)

```cpp
class Solution {
public:
    vector<int> topoSort(int n, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(n);
        vector<int> in_degree(n, 0);

        for (auto& edge : prerequisites) {
            graph[edge[1]].push_back(edge[0]);
            in_degree[edge[0]]++;
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> order;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            order.push_back(node);
            for (int neighbor : graph[node]) {
                in_degree[neighbor]--;
                if (in_degree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        return order.size() == n ? order : vector<int>();
    }
};
```

### Key Problems
| Problem | Trick |
|---|---|
| 207 / 210. Course Schedule I & II | Standard topo sort; I asks feasibility, II asks the order |
| 2050. Parallel Courses III | Topo sort + DP for longest path (critical path) |
| 1203. Sort Items by Groups | Two-level topo sort: group-level and item-level |

---

## 5. Shortest Path

### Decision Tree

```
Is the graph unweighted?
  YES -> BFS (O(V+E))
  NO ->
    Are all weights 0 or 1?
      YES -> 0-1 BFS with deque
      NO ->
        Are weights non-negative?
          YES -> Dijkstra (O((V+E) log V))
          NO ->
            Negative cycles possible?
              YES -> Bellman-Ford (also detects cycles)
              NO ->
                Need all-pairs?
                  YES -> Floyd-Warshall (O(V^3))
```

### Dijkstra Template

```cpp
class Solution {
public:
    vector<int> dijkstra(int n, vector<vector<pair<int, int>>>& graph, int src) {
        vector<int> dist(n, 1e9);
        dist[src] = 0;
        // Min-heap priority queue storing {cost, node}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [cost, u] = pq.top();
            pq.pop();

            if (cost > dist[u]) {
                continue;
            }

            for (auto& edge : graph[u]) {
                int v = edge.first;
                int w = edge.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }
};
```

### 0-1 BFS Template (weights are 0 or 1)

```cpp
class Solution {
public:
    int bfs_01(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        vector<vector<int>> dist(rows, vector<int>(cols, 1e9));
        dist[0][0] = 0;
        // Deque storing state as {cost, {r, c}}
        deque<pair<int, pair<int, int>>> dq;
        dq.push_back({0, {0, 0}});

        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        while (!dq.empty()) {
            auto [cost, pos] = dq.front();
            dq.pop_front();
            int r = pos.first;
            int c = pos.second;

            if (cost > dist[r][c]) {
                continue;
            }

            for (auto& dir : dirs) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    int w = (grid[nr][nc] == 0) ? 0 : 1;
                    if (cost + w < dist[nr][nc]) {
                        dist[nr][nc] = cost + w;
                        if (w == 0) {
                            dq.push_front({cost, {nr, nc}});
                        } else {
                            dq.push_back({cost + 1, {nr, nc}});
                        }
                    }
                }
            }
        }
        return dist[rows - 1][cols - 1];
    }
};
```

### Bellman-Ford Template

```cpp
class Solution {
public:
    vector<int> bellmanFord(int n, vector<vector<int>>& edges, int src) {
        vector<int> dist(n, 1e9);
        dist[src] = 0;

        for (int i = 0; i < n - 1; i++) {
            for (auto& edge : edges) {
                int u = edge[0];
                int v = edge[1];
                int w = edge[2];
                if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                }
            }
        }

        // Check for negative cycles
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
                return vector<int>(); // Negative cycle exists
            }
        }

        return dist;
    }
};
```

### Floyd-Warshall Template

```cpp
class Solution {
public:
    vector<vector<int>> floydWarshall(int n, vector<vector<int>>& edges) {
        vector<vector<int>> dist(n, vector<int>(n, 1e9));
        for (int i = 0; i < n; i++) {
            dist[i][i] = 0;
        }
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            dist[u][v] = w;
        }

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] != 1e9 && dist[k][j] != 1e9) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        return dist;
    }
};
```

### Key Problems
| Problem | Algorithm | Trick |
|---|---|---|
| 743. Network Delay Time | Dijkstra | Standard single source |
| 787. Cheapest Flights Within K Stops | Bellman-Ford | Run exactly K+1 relaxation rounds |
| 1631. Path With Minimum Effort | Dijkstra | Cost = max edge on path, not sum |
| 1368. Min Cost to Valid Path | 0-1 BFS | Free move in grid direction, cost 1 otherwise |
| 1976. Number of Ways | Dijkstra + count | Track count of shortest paths alongside dist |

---

## 6. Bipartite Graph

### When to Use
Can nodes be split into two groups with no edges within each group? Coloring, matching, conflict problems.

### Signal Words
"two groups", "color with two colors", "bipartition", "no two graphacent same color"

### Template

```cpp
class Solution {
private:
    bool bfs(int start, vector<vector<int>>& graph, vector<int>& color) {
        queue<int> q;
        q.push(start);
        color[start] = 0;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (int neighbor : graph[node]) {
                if (color[neighbor] == -1) {
                    color[neighbor] = 1 - color[node];
                    q.push(neighbor);
                } else if (color[neighbor] == color[node]) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    bool isBipartite(int n, vector<vector<int>>& graph) {
        vector<int> color(n, -1);
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                if (!bfs(i, graph, color)) {
                    return false;
                }
            }
        }
        return true;
    }
};
```

---

## 7. Disjoint Set Union (DSU / Union-Find)

### When to Use
Dynamic connectivity, grouping, minimum spanning tree (Kruskal), detecting when components merge.

### Signal Words
"merge", "union", "same group", "connected components dynamically", "redundant connection"

### Template with Path Compression and Union by Rank

```cpp
class DSU {
public:
    vector<int> parent;
    vector<int> rank;
    int components;

    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
        rank.assign(n, 0);
        components = n;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) {
            return false; // already connected
        }
        if (rank[px] < rank[py]) {
            swap(px, py);
        }
        parent[py] = px;
        if (rank[px] == rank[py]) {
            rank[px]++;
        }
        components--;
        return true;
    }
};
```

### Key Problems
| Problem | Trick |
|---|---|
| 721. Accounts Merge | Use email as key; union emails within same account |
| 947. Most Stones Removed | Union stones sharing row or column; answer = n - components |
| 1101. Earliest Moment Everyone Friends | Sort events by time; union until 1 component remains |

---

## 8. Minimum Spanning Tree (MST)

### When to Use
Connect all nodes at minimum total cost, no requirement on path lengths.

### Signal Words
"connect all points", "minimum cost to connect", "cable", "roads"

### Kruskal's Algorithm (DSU-based)

```cpp
class Solution {
public:
    int kruskal(int n, vector<vector<int>>& edges) {
        DSU dsu(n);
        // sort by weight (assume edges element is [u, v, w])
        sort(edges.begin(), edges.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[2] < b[2];
        });

        int mst_cost = 0;
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            if (dsu.unite(u, v)) {
                mst_cost += w;
            }
        }

        return dsu.components == 1 ? mst_cost : -1;
    }
};
```

### Prim's Algorithm (heap-based, good for dense graphs)

```cpp
class Solution {
public:
    int prim(int n, vector<vector<pair<int, int>>>& graph) {
        vector<bool> visited(n, false);
        // Min-heap storing {cost, node}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, 0});
        int total = 0;
        int visited_count = 0;

        while (!pq.empty() && visited_count < n) {
            auto [cost, u] = pq.top();
            pq.pop();

            if (visited[u]) {
                continue;
            }
            visited[u] = true;
            total += cost;
            visited_count++;

            for (auto& edge : graph[u]) {
                int v = edge.first;
                int w = edge.second;
                if (!visited[v]) {
                    pq.push({w, v});
                }
            }
        }

        return visited_count == n ? total : -1;
    }
};
```

---

## 9. Strongly Connected Components (SCC)

### When to Use
In a directed graph, find groups where every node can reach every other node in the group.

### Signal Words
"strongly connected", "condensation", "what can reach what", "DAG of components"

### Kosaraju's Algorithm

```cpp
class Solution {
private:
    void dfs1(int u, vector<vector<int>>& graph, vector<bool>& visited, vector<int>& order) {
        visited[u] = true;
        for (int v : graph[u]) {
            if (!visited[v]) {
                dfs1(v, graph, visited, order);
            }
        }
        order.push_back(u);
    }

    void dfs2(int u, vector<vector<int>>& rgraph, vector<bool>& visited, vector<int>& comp) {
        visited[u] = true;
        comp.push_back(u);
        for (int v : rgraph[u]) {
            if (!visited[v]) {
                dfs2(v, rgraph, visited, comp);
            }
        }
    }

public:
    vector<vector<int>> kosaraju(int n, vector<vector<int>>& graph) {
        vector<bool> visited(n, false);
        vector<int> order;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs1(i, graph, visited, order);
            }
        }

        vector<vector<int>> rgraph(n);
        for (int u = 0; u < n; u++) {
            for (int v : graph[u]) {
                rgraph[v].push_back(u);
            }
        }

        visited.assign(n, false);
        vector<vector<int>> sccs;

        for (int i = n - 1; i >= 0; i--) {
            int u = order[i];
            if (!visited[u]) {
                vector<int> comp;
                dfs2(u, rgraph, visited, comp);
                sccs.push_back(comp);
            }
        }

        return sccs;
    }
};
```

---

## 10. Bridges and Articulation Points

### When to Use
Find edges or nodes whose removal disconnects the graph.

### Signal Words
"critical connection", "critical server", "bridge", "cut vertex", "necessary road"

### Tarjan's Bridge Finding

```cpp
class Solution {
private:
    void dfs(int u, int parent, vector<vector<int>>& graph, vector<int>& disc, vector<int>& low, int& timer, vector<vector<int>>& bridges) {
        disc[u] = low[u] = timer++;
        for (int v : graph[u]) {
            if (disc[v] == -1) {
                dfs(v, u, graph, disc, low, timer, bridges);
                low[u] = min(low[u], low[v]);
                if (low[v] > disc[u]) {
                    bridges.push_back({u, v});
                }
            } else if (v != parent) {
                low[u] = min(low[u], disc[v]);
            }
        }
    }

public:
    vector<vector<int>> findBridges(int n, vector<vector<int>>& graph) {
        vector<int> disc(n, -1);
        vector<int> low(n, 0);
        vector<vector<int>> bridges;
        int timer = 0;

        for (int i = 0; i < n; i++) {
            if (disc[i] == -1) {
                dfs(i, -1, graph, disc, low, timer, bridges);
            }
        }

        return bridges;
    }
};
```

---

## 11. DAG Dynamic Programming

### When to Use
Longest/shortest path, counting paths, or optimizing values along a directed acyclic graph.

### Signal Words
"DAG", "longest path", "count paths", "dependency with values", "critical path"

### Template: DP on DAG with Topological Order

```cpp
class Solution {
public:
    int dagDp(int n, vector<vector<pair<int, int>>>& graph, vector<int>& values) {
        vector<int> in_degree(n, 0);
        for (int u = 0; u < n; u++) {
            for (auto& edge : graph[u]) {
                in_degree[edge.first]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> dp = values;
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto& edge : graph[u]) {
                int v = edge.first;
                int w = edge.second;
                dp[v] = max(dp[v], dp[u] + w);
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
```

---

## 12. Tree Algorithms

### Diameter of a Tree

Run two BFS/DFS passes:
1. From any node, find the farthest node u.
2. From u, find the farthest node v.
3. Distance u to v is the diameter.

```cpp
class Solution {
private:
    pair<int, int> bfs(int start, int n, vector<vector<int>>& graph) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        int farthest = start;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                    if (dist[v] > dist[farthest]) {
                        farthest = v;
                    }
                }
            }
        }

        return {farthest, dist[farthest]};
    }

public:
    int treeDiameter(int n, vector<vector<int>>& graph) {
        auto [u, _] = bfs(0, n, graph);
        auto [v, diam] = bfs(u, n, graph);
        return diam;
    }
};
```

### Lowest Common Ancestor (Binary Lifting)

```cpp
class Solution {
public:
    vector<int> depth;
    vector<vector<int>> parent;
    int LOG;

    void buildLca(int n, vector<vector<int>>& graph, int root = 0) {
        LOG = 32 - __builtin_clz(n);
        depth.assign(n, 0);
        parent.assign(LOG, vector<int>(n, -1));

        vector<bool> visited(n, false);
        visited[root] = true;
        queue<int> q;
        q.push(root);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : graph[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    depth[v] = depth[u] + 1;
                    parent[0][v] = u;
                    q.push(v);
                }
            }
        }

        for (int k = 1; k < LOG; k++) {
            for (int v = 0; v < n; v++) {
                if (parent[k - 1][v] != -1) {
                    parent[k][v] = parent[k - 1][parent[k - 1][v]];
                }
            }
        }
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        int diff = depth[u] - depth[v];
        for (int k = 0; k < LOG; k++) {
            if ((diff >> k) & 1) {
                u = parent[k][u];
            }
        }
        if (u == v) {
            return u;
        }
        for (int k = LOG - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }
};
```

---

## 13. Advanced Graph

### Eulerian Path (visit every edge exactly once)

Conditions:
- Undirected: 0 or 2 nodes of odd degree. Start at an odd-degree node if 2 exist.
- Directed: At most one node with out-degree - in-degree = 1 (start), and one with in-degree - out-degree = 1 (end).

```cpp
class Solution {
public:
    vector<int> hierholzer(vector<vector<int>>& graph, int start) {
        vector<int> stack = {start};
        vector<int> path;

        while (!stack.empty()) {
            int v = stack.back();
            if (!graph[v].empty()) {
                int u = graph[v].back();
                graph[v].pop_back();
                stack.push_back(u);
            } else {
                path.push_back(stack.back());
                stack.pop_back();
            }
        }
        reverse(path.begin(), path.end());
        return path;
    }
};
```

### Maximum Bipartite Matching (Hungarian / Augmenting Paths)

```cpp
class Solution {
private:
    bool dfs(int u, vector<vector<int>>& graph, vector<bool>& visited, vector<int>& match_l, vector<int>& match_r) {
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = True;
                if (match_r[v] == -1 || dfs(match_r[v], graph, visited, match_l, match_r)) {
                    match_l[u] = v;
                    match_r[v] = u;
                    return true;
                }
            }
        }
        return false;
    }

public:
    int maxMatching(int left_n, int right_n, vector<vector<int>>& graph) {
        vector<int> match_l(left_n, -1);
        vector<int> match_r(right_n, -1);

        int result = 0;
        for (int u = 0; u < left_n; u++) {
            vector<bool> visited(right_n, false);
            if (dfs(u, graph, visited, match_l, match_r)) {
                result++;
            }
        }
        return result;
    }
};
```

---

## Pattern Recognition Quick-Reference

| Signal in Problem | Category | Algorithm |
|---|---|---|
| Grid, island, region, flood | Graph Traversal | BFS or DFS |
| Rotting, spreading from multiple sources | Graph Traversal | Multi-source BFS |
| Shortest path, unweighted | Shortest Path | BFS |
| Shortest path, non-negative weights | Shortest Path | Dijkstra |
| Shortest path, with constraint (e.g., at most K stops) | Shortest Path | Bellman-Ford |
| Negative edges, detect negative cycle | Shortest Path | Bellman-Ford |
| All-pairs shortest path | Shortest Path | Floyd-Warshall |
| 0/1 edge weights | Shortest Path | 0-1 BFS with deque |
| Count groups, provinces, clusters | Connected Components | BFS/DFS or DSU |
| Merge accounts, same group queries | DSU | Union-Find |
| Prerequisites, dependencies, ordering | Topological Sort | Kahn's (BFS) |
| Can you finish all courses? | Cycle Detection | Topo sort or DFS 3-color |
| Redundant edge | Cycle Detection | DSU |
| Two groups, bipartite, coloring | Bipartite Check | BFS 2-color |
| Connect all points, minimum cost | MST | Kruskal or Prim |
| Critical connection, bridge | Bridges | Tarjan |
| Which component leads to which | SCC | Kosaraju or Tarjan |
| Longest path with dependencies | DAG DP | Topo sort + DP |
| Reconstruct itinerary | Eulerian Path | Hierholzer |
| Visit all nodes, bitmask | Hamiltonian / Bitmask DP | BFS + bitmask |
| Ancestor queries, distance in tree | Tree | LCA with binary lifting |
| Tree diameter, farthest node | Tree | Double BFS |

---

## Complexity Cheat Sheet

| Algorithm | Time | Space |
|---|---|---|
| BFS / DFS | O(V + E) | O(V) |
| Dijkstra (binary heap) | O((V + E) log V) | O(V) |
| Bellman-Ford | O(V * E) | O(V) |
| Floyd-Warshall | O(V^3) | O(V^2) |
| 0-1 BFS | O(V + E) | O(V) |
| Topological Sort (Kahn's) | O(V + E) | O(V) |
| DSU with path compression + rank | O(alpha(n)) per query | O(V) |
| Kruskal's MST | O(E log E) | O(V) |
| Prim's MST (heap) | O((V + E) log V) | O(V) |
| Tarjan's SCC / Bridges | O(V + E) | O(V) |
| LCA (binary lifting) | O(V log V) build, O(log V) query | O(V log V) |

---

## Problem List by Category

### 1. Graph Traversal (BFS / DFS)
**LeetCode:** 200, 695, 733, 994, 542, 1091, 417, 130, 286, 841
**CSES:** Counting Rooms, Labyrinth, Message Route, Monsters

### 2. Connected Components
**LeetCode:** 547, 200, 323, 1319
**CSES:** Counting Rooms, Building Roads, Road Construction

### 3. Cycle Detection
**LeetCode:** 207, 210, 684, 685, 802
**CSES:** Round Trip, Round Trip II

### 4. Topological Sort
**LeetCode:** 207, 210, 1136, 2050, 1203
**CSES:** Course Schedule, Longest Flight Route, Game Routes

### 5. Shortest Path
**LeetCode (BFS):** 1091 | **CSES:** Message Route
**LeetCode (0-1 BFS):** 1368, 2290
**LeetCode (Dijkstra):** 743, 1631, 778, 787, 1976, 1928, 505, 1334
**CSES (Dijkstra):** Shortest Routes I, Flight Discount, Flight Routes, Investigation
**LeetCode (Bellman-Ford):** 787 | **CSES:** High Score, Cycle Finding
**LeetCode (Floyd-Warshall):** 1334 | **CSES:** Shortest Routes II

### 6. Bipartite Graph
**LeetCode:** 785, 886 | **CSES:** Building Teams

### 7. DSU
**LeetCode:** 547, 684, 721, 947, 1319, 1101 | **CSES:** Road Construction

### 8. Minimum Spanning Tree
**LeetCode:** 1584, 1135 | **CSES:** Road Reparation

### 9. Strongly Connected Components
**LeetCode:** 1192, 802 | **CSES:** Planets and Kingdoms, Coin Collector

### 10. Bridges & Articulation Points
**LeetCode:** 1192 | **CSES:** Necessary Roads, Necessary Cities

### 11. DAG DP
**LeetCode:** 2050, 797, 2328, 1857
**CSES:** Longest Flight Route, Game Routes, Coin Collector, Hamiltonian Flights

### 12. Tree Algorithms
**LeetCode:** 543, 124, 236, 863, 834, 310
**CSES:** Tree Diameter, Tree Distances I & II, Tree Matching, Company Queries I & II, Distance Queries

### 13. Advanced Graph
**LeetCode (Euler):** 332 | **CSES:** Mail Delivery, Teleporters Path
**LeetCode (Flow/Matching):** 1820, 1947 | **CSES:** Download Speed, School Dance, Police Chase
**LeetCode (Bitmask):** 847 | **CSES:** Hamiltonian Flights

---

## Top 30 CSES Graph Problems (Priority Order)

1. Counting Rooms
2. Labyrinth
3. Monsters
4. Message Route
5. Building Roads
6. Building Teams
7. Round Trip
8. Round Trip II
9. Course Schedule
10. Shortest Routes I
11. Shortest Routes II
12. High Score
13. Flight Discount
14. Flight Routes
15. Investigation
16. Road Reparation
17. Road Construction
18. Longest Flight Route
19. Game Routes
20. Planets and Kingdoms
21. Coin Collector
22. Necessary Roads
23. Necessary Cities
24. Tree Diameter
25. Company Queries I
26. Company Queries II
27. Distance Queries
28. Hamiltonian Flights
29. Mail Delivery
30. Download Speed