# Graph OA Mindset Blueprint

Use this cheat sheet to quickly identify, formulate, and correctly implement Graph problems during the Online Assessment.

---

## 1. How to Identify a Graph Problem
Look for these key signals in the problem statement:
*   **Networks & Links:** Cities and roads, computers and cables, airports and flights.
*   **Dependencies:** "Task B cannot be started until Task A is completed" (DAG Topological Sort).
*   **Grids & Matrices:** Searching paths through a grid of blocked and unblocked cells.
*   **Game Transitions / Implicit Graph:** "Minimum operations to transform state A into state B." Each unique state becomes a node; transitions between states become edges. Solve with BFS (unweighted) or Dijkstra (weighted).

---

## 2. The 5 Main Graph Families in Google OAs

### Ⅰ. Grid & Component BFS/DFS
*   **What is asked:** Finding connected islands, flood fill, shortest path on unweighted grid.
*   **What to remember:**
    *   Use directional offsets for clean code:
        ```cpp
        int dirs[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
        ```
    *   **Multi-Source BFS:** If there are multiple starting points (like fire spreading, or multiple monsters chasing), push ALL starting nodes into the queue at step 0.

### Ⅱ. Weighted Shortest Paths
*   **What is asked:** Shortest path with edge weights, flight networks with prices.
*   **What to remember:**
    *   Use **Dijkstra** (Priority Queue) for non-negative weights.
    *   Use **Bellman-Ford** if there are negative weights or you need to detect negative cycles.
    *   **State Expansion:** If the problem allows `K` modifications (e.g., skip `K` flights), expand the state representation: `dist[node][k_remaining]`.

### Ⅲ. Topological Sort (DAGs)
*   **What is asked:** Project scheduling, dependency loops, longest paths in dependency graphs.
*   **What to remember:**
    *   Use Kahn's algorithm (using in-degrees) or DFS post-order to find the topological order.
    *   If Kahn's queue ends before visiting all nodes, **there is a cycle** (deadlock).

### Ⅳ. Disjoint Set Union (DSU / Union-Find)
*   **What is asked:** Dynamic connectivity (adding edges and querying if connected), component sizes, Kruskal's Minimum Spanning Tree.
*   **What to remember:**
    *   DSU is great for offline query processing (processing queries in reverse order as edges are "removed").
    *   Always use path compression and union by rank/size for $O(\alpha(N))$ time.

### Ⅴ. Strongly Connected Components (SCC)
*   **What is asked:** Finding cycles, package dependency groupings, grid routing cycles.
*   **What to remember:**
    *   Use **Kosaraju's** (2-pass DFS) or Tarjan's to group cyclic dependencies.
    *   **Condensation Trick:** Group each SCC into a single "super node". Connect super nodes to form a **Directed Acyclic Graph (DAG)**, then run Topological Sort or DP on the resulting DAG.

### Ⅵ. Binary Lifting / LCA (Tree Ancestor Queries)
*   **What is asked:** "Find the lowest common ancestor of U and V", "K-th ancestor of a node", "Distance between two nodes on a tree."
*   **What to remember:**
    *   Precompute `up[node][j]` = the `2^j`-th ancestor of `node` using DFS in `O(N log N)`.
    *   To find LCA: lift both nodes to the same depth, then lift both together until they meet.
    *   Distance between U and V = `depth[U] + depth[V] - 2 * depth[LCA(U, V)]`.

---

## 3. Step-by-Step Traversal Selection Table

| Goal | Graph Type | Algorithm |
| :--- | :--- | :--- |
| Reachability / DFS Tree | Any | **DFS** |
| Shortest Path | Unweighted | **BFS** |
| Shortest Path | Weighted (Non-negative) | **Dijkstra** |
| Shortest Path | Weighted (Negative edges) | **Bellman-Ford** |
| All-Pairs Shortest Path | Small graphs (N <= 400) | **Floyd-Warshall** |
| Task Scheduling / Cycles | Directed Acyclic (DAG) | **Topological Sort** |
| Component Merging | Dynamic Edges | **DSU (Union-Find)** |
| Cyclic Groups | Directed Graph | **Kosaraju's (SCC)** |

---

## ⚡ Top Graph Bugs to Avoid

1.  **Forgot to Reset:** Global adjacency lists and visited arrays retain values from previous test cases. Reset them inside the class entry function.
2.  **Stack Overflow:** In deep tree DFS, limit local variables inside recursions or convert to iterative traversals if tree depth can reach `10^5`.
3.  **Out of Bounds:** Graph vertices can be `1`-indexed. Ensure arrays are sized `N + 1`.
4.  **Priority Queue Order:** In C++, `priority_queue<T>` is a max-heap. For Dijkstra, declare it as:
    ```cpp
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    ```
