# Section 04: Graphs

> **Focus:** Graph representations, BFS/DFS traversals, single-source & all-pairs shortest paths, Minimum Spanning Trees (MST), Topological Sorting, Disjoint Set Union (DSU), and Cycle/Bridge/SCC detection.

---

# Table of Contents
1. [Graph Representations & Taxonomy](#1-graph-representations--taxonomy)
2. [Breadth-First Search (BFS) & Depth-First Search (DFS)](#2-breadth-first-search-bfs--depth-first-search-dfs)
3. [Shortest Path Algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)](#3-shortest-path-algorithms)
4. [Minimum Spanning Trees (Kruskal & Prim)](#4-minimum-spanning-trees-mst)
5. [Topological Sort & DAGs](#5-topological-sort--dags)
6. [Disjoint Set Union (DSU / Union-Find)](#6-disjoint-set-union-dsu--union-find)
7. [Cycle Detection, Bridges & Strongly Connected Components (SCC)](#7-cycle-detection-bridges--scc)
8. [Quick Recap](#8-quick-recap)

---

# 1. Graph Representations & Taxonomy

A **Graph** $G = (V, E)$ consists of a set of **Vertices (Nodes)** $V$ connected by a set of **Edges (Arcs)** $E$.

```
Undirected Graph:                       Directed Graph (DAG):
     [ 0 ] --- (4) --- [ 1 ]                 [ 0 ] --------> [ 1 ]
       |                 |                     |               |
      (2)               (1)                   (2)             (1)
       |                 |                     v               v
     [ 2 ] --- (3) --- [ 3 ]                 [ 2 ] --------> [ 3 ]
```

---

### Graph Representations Comparison

| Representation | Space Complexity | Add Edge | Check Edge $(u, v)$ | Iterate Neighbors of $u$ | Ideal Workload |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Adjacency Matrix** (`matrix[V][V]`) | $\Theta(V^2)$ | $O(1)$ | **$O(1)$** | $O(V)$ | **Dense Graphs** ($E \approx V^2$), constant-time edge existence checks. |
| **Adjacency List** (`vector<vector<int>>`) | **$\Theta(V + E)$** | $O(1)$ | $O(\text{deg}(u))$ | **$O(\text{deg}(u))$** | **Sparse Graphs** ($E \ll V^2$), standard traversals (BFS, DFS, Dijkstra). |
| **Edge List** (`vector<Edge>`) | $\Theta(E)$ | $O(1)$ | $O(E)$ | $O(E)$ | Algorithms that process edges globally (**Kruskal's MST**, Bellman-Ford). |

```cpp
// 1. Adjacency Matrix (V x V 2D Array)
vector<vector<int>> adjMatrix(V, vector<int>(V, 0));
adjMatrix[u][v] = weight; // Directed edge u -> v

// 2. Adjacency List (Vector of Pairs: neighbor, weight)
vector<vector<pair<int, int>>> adjList(V);
adjList[u].push_back({v, weight});

// 3. Edge List
struct Edge { int u, v, weight; };
vector<Edge> edgeList;
```

---

### Graph Classifications
- **Sparse vs. Dense:** Sparse if $|E| = O(V)$; Dense if $|E| \approx V^2$.
- **Directed Acyclic Graph (DAG):** A directed graph with zero directed cycles (the foundation of build pipelines and task scheduling).
- **Bipartite Graph:** Vertices can be partitioned into two disjoint sets $U$ and $V$ such that every edge connects a vertex in $U$ to a vertex in $V$ (testable via 2-color BFS/DFS).

---

### Interview Follow-Up Questions
1. **"Why is Adjacency List favored over Adjacency Matrix in 99% of graph problems?"**
   - *Answer:* Real-world networks (social graphs, web links, road networks) are extremely sparse ($E \ll V^2$). For $V = 100,000$ vertices, an Adjacency Matrix requires $100,000^2 \times 4\text{ bytes} \approx 40\text{ Gigabytes}$ of RAM, whereas an Adjacency List with $E = 500,000$ requires only ~12 Megabytes.

---

# 2. Breadth-First Search (BFS) & Depth-First Search (DFS)

```
Graph Traversals:
- BFS explores outward in concentric ripples (Level by Level using a Queue).
- DFS dives down a single branch as deeply as possible before backtracking (using a Stack/Recursion).
```

---

### A. Breadth-First Search (BFS)
- **Mechanics:** Backed by a **FIFO Queue**. Explores all immediate neighbors at distance $D$ before visiting nodes at distance $D + 1$.
- **Guarantees:** Finds the **Shortest Path in Unweighted Graphs** with minimum edge count.
- **Time Complexity:** $\mathbf{O(V + E)}$ (Every vertex is enqueued once; every edge is inspected once).
- **Space Complexity:** $\mathbf{O(V)}$ (Queue holds at most the maximum width of the graph).

```cpp
// Standard BFS Traversal in C++
void bfs(int startNode, int V, const vector<vector<int>>& adj) {
    vector<bool> visited(V, false);
    queue<int> q;
    
    visited[startNode] = true;
    q.push(startNode);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}
```

---

### B. Depth-First Search (DFS)
- **Mechanics:** Backed by the **Call Stack** (or explicit Stack). Traverses along an edge until a dead end is reached, then backtracks.
- **Applications:** Finding connected components, topological sorting, detecting cycles, finding bridges/articulation points, and solving mazes.
- **Time Complexity:** $\mathbf{O(V + E)}$.
- **Space Complexity:** $\mathbf{O(V)}$ (Call stack depth reaches $O(V)$ in skewed line graphs).

```cpp
// Standard DFS Traversal in C++
void dfs(int u, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v, adj, visited);
        }
    }
}
```

---

### Interview Follow-Up Questions
1. **"What is 0-1 BFS and how does it find shortest paths in $O(V + E)$ time?"**
   - *Answer:* When edge weights are restricted to only $0$ or $1$, standard Dijkstra ($O(E \log V)$) is overkill. We use a **Deque**: if traversing an edge of weight $0$, push to `deque.push_front()`; if weight $1$, push to `deque.push_back()`. This maintains sorted distance order in the queue in $O(V + E)$ linear time.
2. **"How do you count the number of Connected Components in an undirected graph?"**
   - *Answer:* Initialize `count = 0`. Iterate through all vertices $i \in [0, V-1]$; if `!visited[i]`, launch a full BFS/DFS traversal to mark its entire component and increment `count++`. Total time remains $O(V + E)$.

---

# 3. Shortest Path Algorithms

```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM       | GRAPH TYPE             | TIME COMPLEXITY       | SPACE | LIMITATIONS / EDGE CASES|
+---------------------------------------------------------------------------------------------------+
| Unweighted BFS  | Unweighted             | O(V + E)              | O(V)  | Only unweighted graphs. |
| Dijkstra        | Non-negative weights   | O((V + E) log V)      | O(V)  | FAILS on negative edges.|
| Bellman-Ford    | Weighted (Directed)    | O(V * E)              | O(V)  | Detects negative cycles.|
| Floyd-Warshall  | All-Pairs Shortest Path| O(V^3)                | O(V^2)| Small graphs (V <= 500).|
+---------------------------------------------------------------------------------------------------+
```

---

### A. Dijkstra's Algorithm (Greedy + Min-Heap)
Dijkstra maintains a `dist[]` array initialized to $\infty$. It greedily extracts the unvisited vertex with the minimum known distance using a **Min-Priority Queue** and relaxes all of its outgoing edges.

```
Relaxation Formula:
If dist[u] + weight(u, v) < dist[v]:
    dist[v] = dist[u] + weight(u, v)
```

```cpp
// Dijkstra's Shortest Path Algorithm (O((V + E) log V) Time, O(V) Space)
vector<int> dijkstra(int src, int V, const vector<vector<pair<int, int>>>& adj) {
    vector<int> dist(V, INT_MAX);
    // Min-Heap stores pairs of { current_distance, vertex_u }
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue; // Stale heap entry optimization
        
        for (auto& [v, weight] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight; // Edge Relaxation
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

#### Why Does Dijkstra FAIL on Negative Edge Weights?
Dijkstra assumes that once a vertex is popped from the priority queue, its shortest distance is finalized and will never decrease (greedy choice). A negative edge later in the graph can provide a shorter path to an already "finalized" node, causing Dijkstra to produce incorrect distances.

---

### B. Bellman-Ford Algorithm (Edge Relaxation DP)
- **Principle:** In a simple graph with $V$ vertices, the shortest simple path contains at most $V - 1$ edges.
- **Algorithm:** Relax **ALL $E$ edges** repeatedly for **$V - 1$ iterations**.
- **Negative Cycle Detection:** Run a $V$-th iteration. If *any* edge can still be relaxed on the $V$-th pass, the graph contains a **Negative Weight Cycle**.

```cpp
// Bellman-Ford Algorithm: O(V * E) Time Complexity
bool bellmanFord(int src, int V, const vector<Edge>& edges, vector<int>& dist) {
    dist.assign(V, INT_MAX);
    dist[src] = 0;
    
    // Relax all edges V - 1 times
    for (int i = 1; i <= V - 1; i++) {
        for (const auto& e : edges) {
            if (dist[e.u] != INT_MAX && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
            }
        }
    }
    // Check for negative weight cycles on V-th pass
    for (const auto& e : edges) {
        if (dist[e.u] != INT_MAX && dist[e.u] + e.weight < dist[e.v]) {
            return false; // Negative cycle detected!
        }
    }
    return true; // No negative cycles
}
```

---

### C. Floyd-Warshall Algorithm (All-Pairs Shortest Path)
Floyd-Warshall computes the shortest path between **every pair of vertices $(i, j)$** by testing whether an intermediate vertex $k$ provides a shorter route.

$$\text{dp}[i][j] = \min(\text{dp}[i][j], \text{dp}[i][k] + \text{dp}[k][j]) \quad \text{for all } k \in [0, V-1]$$

```cpp
// Floyd-Warshall: O(V^3) Time, O(V^2) Space
void floydWarshall(int V, vector<vector<int>>& dist) {
    for (int k = 0; k < V; k++) {         // Intermediate node MUST be outermost loop!
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    // Negative cycle exists if dist[i][i] < 0 for any vertex i
}
```

---

### Interview Follow-Up Questions
1. **"Why MUST the intermediate node $k$ be in the outermost loop in Floyd-Warshall?"**
   - *Answer:* Because it is a Dynamic Programming algorithm where $\text{dp}[k][i][j]$ depends on subproblems having evaluated all intermediate vertices from $0$ up to $k-1$. Placing $k$ in the inner loop fails to build upon previously computed shortest subpaths.

---

# 4. Minimum Spanning Trees (MST)

A **Minimum Spanning Tree (MST)** of a connected, undirected, edge-weighted graph is a spanning tree (a subgraph connecting all $V$ vertices with exactly $V - 1$ edges and zero cycles) that minimizes the **total sum of edge weights**.

```
Original Graph (Weights):                Minimum Spanning Tree (Total Weight = 6):
     [ 0 ] --- (4) --- [ 1 ]                 [ 0 ]             [ 1 ]
       |                 |                     |                 |
      (2)               (1)                   (2)               (1)
       |                 |                     |                 |
     [ 2 ] --- (3) --- [ 3 ]                 [ 2 ] --- (3) --- [ 3 ]
```

---

### Kruskal's vs. Prim's Algorithm

| Feature | Kruskal's Algorithm | Prim's Algorithm |
| :--- | :--- | :--- |
| **Strategy** | **Edge-Centric Greedy:** Sorts all edges globally. | **Vertex-Centric Greedy:** Expands a connected cut. |
| **Data Structure** | Disjoint Set Union (DSU / Union-Find) + Edge Array. | Min-Priority Queue (Binary Heap) + Adjacency List. |
| **Time Complexity** | $\mathbf{O(E \log E) = O(E \log V)}$ (Edge sorting dominates). | $\mathbf{O((V + E) \log V)}$ (Heap operations). |
| **Ideal Workload** | **Sparse Graphs** ($E \approx V$). | **Dense Graphs** ($E \approx V^2$) using Fibonacci heap ($O(E + V \log V)$). |

```cpp
// Kruskal's Algorithm using DSU
int kruskalMST(int V, vector<Edge>& edges) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight; // Sort edges ascending
    });
    
    DSU dsu(V);
    int mstWeight = 0, edgesCount = 0;
    
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) { // If u and v belong to different components
            mstWeight += e.weight;
            edgesCount++;
            if (edgesCount == V - 1) break; // MST complete!
        }
    }
    return mstWeight;
}
```

---

### Interview Follow-Up Questions
1. **"Can an MST change if all edge weights are incremented by a positive constant $+C$?"**
   - *Answer:* No. An MST always contains exactly $V - 1$ edges. Adding $+C$ to every edge increases the total weight of *every* possible spanning tree by exactly $(V - 1) \cdot C$, preserving the original minimum order. (Note: Shortest paths *do* change if paths have different edge counts).
2. **"Is the MST unique if all edge weights in the graph are distinct?"**
   - *Answer:* Yes. If every edge weight is strictly unique, there exists exactly **one unique Minimum Spanning Tree**.

---

# 5. Topological Sort & DAGs

A **Topological Sort** of a Directed Acyclic Graph (DAG) is a linear ordering of its vertices such that for every directed edge $u \to v$, vertex $u$ appears before vertex $v$ in the ordering.

```
DAG Pipeline Dependencies:
[ Lint (0) ] ----> [ Build (1) ] ----> [ Test (2) ] ----> [ Deploy (3) ]
      |                                      ^
      +--------------> [ Compile (4) ] -------+

Valid Topological Sort: [ 0, 4, 1, 2, 3 ]
```

---

### A. Kahn's Algorithm (BFS In-Degree Queue)
1. Compute the **In-Degree** (count of incoming edges) for every vertex.
2. Enqueue all vertices with $\text{inDegree} == 0$.
3. While the queue is not empty:
   - Pop vertex $u$, append it to the topological order list.
   - For every neighbor $v$ of $u$, decrement $\text{inDegree}[v]--$.
   - If $\text{inDegree}[v] == 0$, push $v$ into the queue.
4. **Cycle Detection Check:** If the topological order contains fewer than $V$ vertices, the graph contains a **Cycle** (deadlock).

```cpp
// Kahn's Algorithm: O(V + E) Time, O(V) Space
vector<int> kahnTopologicalSort(int V, const vector<vector<int>>& adj) {
    vector<int> inDegree(V, 0);
    for (int u = 0; u < V; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }
    
    queue<int> q;
    for (int i = 0; i < V; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    vector<int> topoOrder;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topoOrder.push_back(u);
        
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) {
                q.push(v);
            }
        }
    }
    if (topoOrder.size() != V) return {}; // Cycle detected -> No valid topological sort!
    return topoOrder;
}
```

---

### B. DFS-Based Topological Sort
- Perform DFS. When all descendants of a vertex $u$ have been fully visited, push $u$ onto a **Stack**.
- After DFS completes across all nodes, pop all elements from the stack $\implies$ Yields valid topological ordering in **$O(V + E)$ time**.

---

### Interview Follow-Up Questions
1. **"Can an undirected graph have a topological sort?"**
   - *Answer:* No. An undirected edge $(u, v)$ means $u \to v$ and $v \to u$, creating a 2-node cycle which violates topological ordering.

---

# 6. Disjoint Set Union (DSU / Union-Find)

A **Disjoint Set Union (DSU)** data structure maintains a collection of disjoint (non-overlapping) dynamic sets, supporting two fundamental operations:
1. **`find(i)`:** Determines which subset element $i$ belongs to (returns the representative root).
2. **`union(i, j)`:** Merges the subsets containing elements $i$ and $j$.

```
Path Compression Visualization:
Before find(4):                       After find(4) (Flattens tree directly to Root):
     [ 1 ] (Root)                                 [ 1 ]
       |                                     /    |    \
     [ 2 ]                                 [ 2 ] [ 3 ] [ 4 ]
       |
     [ 3 ]
       |
     [ 4 ]
```

---

### The 2 Core Optimizations

#### 1. Path Compression (`find`)
During `find(i)`, update every visited node's parent pointer to point directly to the root:
`parent[i] = find(parent[i])`. Flattens tree height to $O(1)$ on subsequent lookups.

#### 2. Union by Rank / Size (`union`)
Always attach the tree with smaller rank/depth beneath the root of the tree with larger rank, preventing degenerate tall trees.

```cpp
// Highly Optimized DSU Class (O(alpha(N)) ~ O(1) Time per operation)
class DSU {
    vector<int> parent, rank;
public:
    DSU(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
    }
    
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]); // Path Compression!
    }
    
    bool unite(int i, int j) {
        int rootI = find(i), rootJ = find(j);
        if (rootI == rootJ) return false; // Already in same set
        
        // Union by Rank
        if (rank[rootI] < rank[rootJ]) {
            parent[rootI] = rootJ;
        } else if (rank[rootI] > rank[rootJ]) {
            parent[rootJ] = rootI;
        } else {
            parent[rootJ] = rootI;
            rank[rootI]++;
        }
        return true;
    }
};
```

---

### Time Complexity: The Inverse Ackermann Function $\alpha(N)$
With both Path Compression and Union by Rank enabled, a sequence of $M$ operations on $N$ elements executes in:
$$O(M \cdot \alpha(N))$$
Where $\alpha(N)$ is the **Inverse Ackermann Function**. For all realistic universe sizes ($N \le 10^{80}$ atoms in the universe), $\alpha(N) \le 4$, making DSU operations **effectively $O(1)$ constant time**.

---

### Interview Follow-Up Questions
1. **"What happens to DSU time complexity if you use Path Compression WITHOUT Union by Rank?"**
   - *Answer:* $O(M \log N)$ worst-case. Path compression alone flattens trees during lookups, but unguided unions can still create tall trees before searches occur. Both optimizations together are required to achieve $O(\alpha(N))$.

---

# 7. Cycle Detection, Bridges & SCC

---

### A. Cycle Detection Mechanics

```
+-----------------------------------------------------------------------------------------+
| 1. UNDIRECTED GRAPH CYCLE DETECTION                                                     |
| - DFS/BFS: If an adjacent neighbor v is visited AND v != parent -> Cycle detected!     |
| - DSU: For each edge (u, v), if find(u) == find(v) -> Edge forms a cycle!               |
+-----------------------------------------------------------------------------------------+
| 2. DIRECTED GRAPH CYCLE DETECTION                                                       |
| - 3-Color DFS: Track node states:                                                       |
|     WHITE (0) = Unvisited, GRAY (1) = In current recursion stack, BLACK (2) = Done.     |
|     Encountering a GRAY neighbor indicates a BACK-EDGE -> Cycle detected!               |
| - Kahn's Algorithm: If topoOrder.size() < V -> Cycle detected!                        |
+-----------------------------------------------------------------------------------------+
```

```cpp
// 3-Color DFS Directed Cycle Detection in C++
bool hasCycleDFS(int u, const vector<vector<int>>& adj, vector<int>& state) {
    state[u] = 1; // 1 = GRAY (Visiting)
    for (int v : adj[u]) {
        if (state[v] == 1) return true; // Back-Edge detected!
        if (state[v] == 0 && hasCycleDFS(v, adj, state)) return true;
    }
    state[u] = 2; // 2 = BLACK (Finished)
    return false;
}
```

---

### B. Bridges & Articulation Points (Tarjan's Algorithm)
- **Bridge (Critical Connection):** An edge whose deletion increases the number of connected components.
- **Articulation Point (Cut Vertex):** A vertex whose deletion disconnects the graph.
- **Tarjan's Low-Link Variables:**
  - `disc[u]`: Timestamp when node $u$ was first discovered.
  - `low[u]`: Earliest discovery time reachable from $u$ via subtree edges and at most one back-edge.
- **Bridge Condition:** Edge $(u, v)$ is a bridge if and only if:
  $$\mathbf{low[v] > disc[u]}$$
  *(Meaning: Subtree rooted at $v$ has NO back-edge to $u$ or any ancestor of $u$)*.

```cpp
// Tarjan's Bridge Finding Algorithm: O(V + E) Time
void findBridges(int u, int parent, int& timer, const vector<vector<int>>& adj,
                 vector<int>& disc, vector<int>& low, vector<vector<int>>& bridges) {
    disc[u] = low[u] = ++timer;
    for (int v : adj[u]) {
        if (v == parent) continue;
        if (disc[v]) {
            low[u] = min(low[u], disc[v]); // Back-edge
        } else {
            findBridges(v, u, timer, adj, disc, low, bridges);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) {
                bridges.push_back({u, v}); // Bridge found!
            }
        }
    }
}
```

---

### C. Strongly Connected Components (SCC)
In a directed graph, an **SCC** is a maximal subgraph where every vertex is reachable from every other vertex.
- **Kosaraju's 2-Pass Algorithm ($O(V + E)$):**
  1. Run DFS on original graph, pushing nodes to a stack based on finish times.
  2. Transpose (reverse all directed edges) graph: $G^T$.
  3. Pop nodes from stack and launch DFS on $G^T$; each DFS run discovers one complete SCC.

---

### Interview Follow-Up Questions
1. **"What is the root node condition for being an Articulation Point in Tarjan's algorithm?"**
   - *Answer:* The root of the DFS tree is an articulation point if and only if it has **2 or more independent children** in the DFS tree.

---

# 8. Quick Recap

- **Graph Representation:** Adjacency List for sparse graphs ($O(V + E)$ space); Adjacency Matrix for dense graphs ($O(V^2)$ space, $O(1)$ edge checks).
- **Shortest Path Decision Tree:**
  - Unweighted $\implies$ **BFS** ($O(V + E)$).
  - Non-negative weighted $\implies$ **Dijkstra** ($O((V + E) \log V)$).
  - Negative weights $\implies$ **Bellman-Ford** ($O(V \cdot E)$).
  - All-pairs $\implies$ **Floyd-Warshall** ($O(V^3)$).
- **MST Strategy:** Kruskal's sorts edges ($O(E \log V)$) using DSU; Prim's expands cuts via priority queue.
- **Topological Sorting:** Only valid on DAGs; Kahn's in-degree BFS detects cycles if output length $< V$.
- **DSU Invariant:** Path compression + Union by rank yields practically constant time $O(\alpha(N)) \approx O(1)$.
- **Cycle Detection Rules:** Undirected uses DFS parent checks or DSU; Directed requires 3-color DFS (`GRAY` back-edge) or Kahn's failure.
- **Tarjan's Bridge Formula:** Edge $(u, v)$ is a critical bridge if $\mathbf{low[v] > disc[u]}$.
