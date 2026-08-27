#include <bits/stdc++.h>
using namespace std;

class Solution {
public:

    // =========================================================
    // 1. DFS TRAVERSAL
    // =========================================================

    void dfs(int node, vector<vector<int>>& graph, vector<int>& visited) {
        visited[node] = 1;
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) dfs(neighbor, graph, visited);
        }
    }
    // Interview Explanation:
    // - Problem Statement: Perform Depth-First Search (DFS) traversal on a graph from a starting node.
    // - Approach: Recursive Depth-First Search with a visited array.
    // - Intuition: Mark current node as visited, then recursively visit all unvisited adjacent neighbors.
    // - Complexity: Time: O(V + E) traversing vertices and edges, Space: O(V) for visited array and recursion stack.


    // =========================================================
    // 2. BFS TRAVERSAL
    // =========================================================

    void bfs(int start, vector<vector<int>>& graph, vector<int>& visited) {
        queue<int> q;
        q.push(start);
        visited[start] = 1;

        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    q.push(neighbor);
                }
            }
        }
    }
    // Interview Explanation:
    // - Problem Statement: Perform Breadth-First Search (BFS) traversal on a graph level by level.
    // - Approach: Queue-based Breadth-First Search with a visited array.
    // - Intuition: Enqueue start node and mark visited; repeatedly dequeue front, visiting and enqueuing all unvisited neighbors.
    // - Complexity: Time: O(V + E) traversing vertices and edges, Space: O(V) for queue and visited array.


    // =========================================================
    // 3. NUMBER OF CONNECTED COMPONENTS
    // =========================================================

    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
            graph[edge[1]].push_back(edge[0]);
        }

        vector<int> visited(n, 0);
        int components = 0;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                components++;
                dfs(i, graph, visited);
            }
        }
        return components;
    }
    // Interview Explanation:
    // - Problem Statement: Find the total number of connected components in an undirected graph.
    // - Approach: Graph construction + DFS component counting across all nodes.
    // - Intuition: Iterate 0 to n-1; when an unvisited node is found, increment count and DFS-explore its entire connected component.
    // - Complexity: Time: O(V + E) building adjacency list and traversing, Space: O(V + E) for graph storage and visited array.


    // =========================================================
    // 4. CYCLE DETECTION - UNDIRECTED GRAPH
    // =========================================================

    bool cycleDFS(int node, int parent, vector<vector<int>>& graph, vector<int>& visited) {
        visited[node] = 1;
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                if (cycleDFS(neighbor, node, graph, visited)) return true;
            } else if (neighbor != parent) {
                return true;
            }
        }
        return false;
    }

    bool hasCycleUndirected(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
            graph[edge[1]].push_back(edge[0]);
        }

        vector<int> visited(n, 0);
        for (int i = 0; i < n; i++) {
            if (!visited[i] && cycleDFS(i, -1, graph, visited)) return true;
        }
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Detect whether an undirected graph contains a cycle.
    // - Approach: DFS traversal tracking parent pointer.
    // - Intuition: If a visited neighbor is reached that is not the immediate parent of current node, an alternate path / back-edge forms a cycle.
    // - Complexity: Time: O(V + E) traversing vertices and edges, Space: O(V) for recursion stack and visited array.


    // =========================================================
    // 5. CYCLE DETECTION - DIRECTED GRAPH
    // =========================================================

    bool directedDFS(int node, vector<vector<int>>& graph, vector<int>& visited, vector<int>& path) {
        visited[node] = 1;
        path[node] = 1;

        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                if (directedDFS(neighbor, graph, visited, path)) return true;
            } else if (path[neighbor]) {
                return true;
            }
        }
        path[node] = 0;
        return false;
    }

    bool hasCycleDirected(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) graph[edge[0]].push_back(edge[1]);

        vector<int> visited(n, 0), path(n, 0);
        for (int i = 0; i < n; i++) {
            if (!visited[i] && directedDFS(i, graph, visited, path)) return true;
        }
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Detect whether a directed graph contains a cycle.
    // - Approach: DFS with 3-state tracking (visited set and active recursion path).
    // - Intuition: A directed cycle exists if an edge leads to a node currently in the active recursion call stack (`path[neighbor] == 1`).
    // - Complexity: Time: O(V + E) traversing vertices and edges, Space: O(V) for visited and recursion path array.


    // =========================================================
    // 6. BIPARTITE GRAPH - BFS COLORING
    // =========================================================

    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);

        for (int i = 0; i < n; i++) {
            if (color[i] != -1) continue;
            queue<int> q;
            q.push(i);
            color[i] = 0;

            while (!q.empty()) {
                int node = q.front(); q.pop();
                for (int neighbor : graph[node]) {
                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[node];
                        q.push(neighbor);
                    } else if (color[neighbor] == color[node]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    // Interview Explanation:
    // - Problem Statement: Determine whether an undirected graph is bipartite (2-colorable with no adjacent same colors).
    // - Approach: 2-Coloring via Breadth-First Search (BFS).
    // - Intuition: Color start node 0; alternate neighbor colors to `1 - color[node]`; if an adjacent node shares the same color, graph is not bipartite.
    // - Complexity: Time: O(V + E) visiting all vertices and edges, Space: O(V) for queue and color array.


    // =========================================================
    // 7. TOPOLOGICAL SORT - KAHN'S ALGORITHM (BFS)
    // =========================================================

    vector<int> topoSort(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);

        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
            indegree[edge[1]]++;
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        vector<int> order;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            order.push_back(node);

            for (int neighbor : graph[node]) {
                if (--indegree[neighbor] == 0) q.push(neighbor);
            }
        }
        return order.size() == n ? order : vector<int>{};
    }
    // Interview Explanation:
    // - Problem Statement: Find a topological ordering of vertices in a Directed Acyclic Graph (DAG) using BFS.
    // - Approach: In-degree reduction BFS (Kahn's Algorithm).
    // - Intuition: Enqueue vertices with in-degree 0; remove them while decrementing neighbors' in-degrees; if result size < n, a cycle exists.
    // - Complexity: Time: O(V + E) processing all edges and vertices, Space: O(V + E) for adjacency list, in-degree array, and queue.


    // =========================================================
    // 8. TOPOLOGICAL SORT - DFS
    // =========================================================

    void topoDFS(int node, vector<vector<int>>& graph, vector<int>& visited, vector<int>& order) {
        visited[node] = 1;
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) topoDFS(neighbor, graph, visited, order);
        }
        order.push_back(node);
    }

    vector<int> topoSortDFS(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) graph[edge[0]].push_back(edge[1]);

        vector<int> visited(n, 0);
        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) topoDFS(i, graph, visited, order);
        }
        reverse(order.begin(), order.end());
        return order;
    }
    // Interview Explanation:
    // - Problem Statement: Find a topological ordering of a Directed Acyclic Graph using DFS.
    // - Approach: Post-order DFS + array reversal.
    // - Intuition: A node finishes DFS only after all reachable descendants are explored; appending to order on backtrack and reversing yields topological sequence.
    // - Complexity: Time: O(V + E) traversing vertices and edges, Space: O(V + E) for graph storage and recursion call stack.


    // =========================================================
    // 9. SHORTEST PATH - UNWEIGHTED GRAPH
    // =========================================================

    vector<int> shortestPathBFS(int n, vector<vector<int>>& edges, int src) {
        vector<vector<int>> graph(n);
        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
            graph[edge[1]].push_back(edge[0]);
        }

        vector<int> dist(n, -1);
        queue<int> q;
        q.push(src);
        dist[src] = 0;

        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (int neighbor : graph[node]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[node] + 1;
                    q.push(neighbor);
                }
            }
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find the shortest path distances from a source node to all other nodes in an unweighted graph.
    // - Approach: Breadth-First Search (BFS) distance layer propagation.
    // - Intuition: Since all edge weights are uniform (1), the first time BFS reaches any node, it discovers the shortest path.
    // - Complexity: Time: O(V + E) linear graph scan, Space: O(V) for queue and distance array.


    // =========================================================
    // 10. DIJKSTRA - POSITIVE WEIGHTED GRAPH
    // =========================================================

    vector<int> dijkstra(int n, vector<vector<pair<int, int>>>& graph, int src) {
        const int INF = 1e9;
        vector<int> dist(n, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [distance, node] = pq.top(); pq.pop();
            if (distance != dist[node]) continue;

            for (auto [neighbor, weight] : graph[node]) {
                if (distance + weight < dist[neighbor]) {
                    dist[neighbor] = distance + weight;
                    pq.push({dist[neighbor], neighbor});
                }
            }
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find single-source shortest path distances in a graph with non-negative edge weights.
    // - Approach: Greedy shortest path using Min-Heap Priority Queue (`std::greater`).
    // - Intuition: Greedily process node with minimum provisional distance; relax outgoing edges and discard outdated stale heap entries.
    // - Complexity: Time: O((V + E) \log V) heap operations, Space: O(V + E) for adjacency list, distance array, and priority queue.


    // =========================================================
    // 11. 0-1 BFS
    // Edge weights must be 0 or 1
    // =========================================================

    vector<int> zeroOneBFS(int n, vector<vector<pair<int, int>>>& graph, int src) {
        const int INF = 1e9;
        vector<int> dist(n, INF);
        deque<int> dq;

        dist[src] = 0;
        dq.push_front(src);

        while (!dq.empty()) {
            int node = dq.front(); dq.pop_front();
            for (auto [neighbor, weight] : graph[node]) {
                if (dist[node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[node] + weight;
                    if (weight == 0) dq.push_front(neighbor);
                    else dq.push_back(neighbor);
                }
            }
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find shortest path distances in a graph where edge weights are restricted to 0 or 1.
    // - Approach: Double-ended Queue (Deque) BFS (0-1 BFS).
    // - Intuition: 0-weight edges push to front (processed immediately at same distance level), 1-weight edges push to back; mimics Dijkstra in linear time without heap.
    // - Complexity: Time: O(V + E) linear time, Space: O(V) for deque and distance array.


    // =========================================================
    // 12. BELLMAN-FORD
    // Handles negative edges
    // =========================================================

    vector<int> bellmanFord(int n, vector<vector<int>>& edges, int src) {
        const int INF = 1e9;
        vector<int> dist(n, INF);
        dist[src] = 0;

        for (int i = 1; i <= n - 1; i++) {
            bool changed = false;
            for (auto& edge : edges) {
                int u = edge[0], v = edge[1], weight = edge[2];
                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    changed = true;
                }
            }
            if (!changed) break;
        }

        for (auto& edge : edges) {
            int u = edge[0], v = edge[1], weight = edge[2];
            if (dist[u] != INF && dist[u] + weight < dist[v]) return {}; // negative cycle detected
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find single-source shortest paths in graphs with negative edge weights and detect negative cycles.
    // - Approach: Dynamic Programming edge relaxation ($N - 1$ rounds).
    // - Intuition: A simple shortest path contains at most $N - 1$ edges; if distance can still be relaxed in round $N$, a negative cycle exists.
    // - Complexity: Time: O(V \cdot E) edge relaxations, Space: O(V) for distance array.


    // =========================================================
    // 13. FLOYD-WARSHALL
    // All-pairs shortest path
    // =========================================================

    vector<vector<int>> floydWarshall(vector<vector<int>> dist) {
        int n = dist.size();
        const int INF = 1e8;

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find all-pairs shortest paths in a directed weighted graph with positive/negative edge weights.
    // - Approach: Dynamic Programming considering all intermediate vertices $k \in [0, n-1]$.
    // - Intuition: For every pair $(i, j)$, check whether detour through vertex $k$ reduces distance: `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`.
    // - Complexity: Time: O(V^3) triple nested loops, Space: O(V^2) for distance matrix.


    // =========================================================
    // 14. DISJOINT SET UNION (DSU)
    // =========================================================

    class DSU {
    public:
        vector<int> parent, size;

        DSU(int n) {
            parent.resize(n);
            size.resize(n, 1);
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        }

        bool unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return false;
            if (size[a] < size[b]) swap(a, b);
            parent[b] = a;
            size[a] += size[b];
            return true;
        }
    };
    // Interview Explanation:
    // - Problem Statement: Design a Disjoint Set Union (Union-Find) data structure supporting near O(1) find and unite operations.
    // - Approach: Union by Size + Path Compression.
    // - Intuition: Path compression flattens tree during `find`; union by size attaches smaller tree under larger root, keeping depth bounded.
    // - Complexity: Time: O(\alpha(N)) amortized nearly O(1) per operation (Inverse Ackermann), Space: O(N) for parent and size vectors.


    // =========================================================
    // 15. CYCLE DETECTION USING DSU
    // =========================================================

    bool hasCycleDSU(int n, vector<vector<int>>& edges) {
        DSU dsu(n);
        for (auto& edge : edges) {
            if (!dsu.unite(edge[0], edge[1])) return true;
        }
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Detect whether an undirected graph contains a cycle using Disjoint Set Union.
    // - Approach: Incremental edge addition with DSU.
    // - Intuition: If endpoints $u$ and $v$ of an edge already share the same representative root (`find(u) == find(v)`), adding edge forms a cycle.
    // - Complexity: Time: O(E \cdot \alpha(V)) near-linear time, Space: O(V) for DSU parent and size arrays.


    // =========================================================
    // 16. KRUSKAL'S MST
    // =========================================================

    int kruskal(int n, vector<vector<int>>& edges) {
        sort(edges.begin(), edges.end(), [](const vector<int>& a, const vector<int>& b) { return a[2] < b[2]; });
        DSU dsu(n);
        int mstWeight = 0, edgesUsed = 0;

        for (auto& edge : edges) {
            if (dsu.unite(edge[0], edge[1])) {
                mstWeight += edge[2];
                if (++edgesUsed == n - 1) break;
            }
        }
        return mstWeight;
    }
    // Interview Explanation:
    // - Problem Statement: Find the Minimum Spanning Tree (MST) weight of a connected undirected weighted graph.
    // - Approach: Greedy edge selection sorted by weight + DSU (Kruskal's Algorithm).
    // - Intuition: Sort edges ascending; greedily include edges connecting previously disconnected components until $N - 1$ edges are picked.
    // - Complexity: Time: O(E \log E) sorting edges, Space: O(V) for DSU structures.


    // =========================================================
    // 17. PRIM'S MST
    // =========================================================

    int prim(int n, vector<vector<pair<int, int>>>& graph) {
        vector<int> visited(n, 0);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, 0});
        int mstWeight = 0;

        while (!pq.empty()) {
            auto [weight, node] = pq.top(); pq.pop();
            if (visited[node]) continue;
            visited[node] = 1;
            mstWeight += weight;

            for (auto [neighbor, edgeWeight] : graph[node]) {
                if (!visited[neighbor]) pq.push({edgeWeight, neighbor});
            }
        }
        return mstWeight;
    }
    // Interview Explanation:
    // - Problem Statement: Find the Minimum Spanning Tree (MST) weight of an undirected graph using Prim's algorithm.
    // - Approach: Greedy vertex growth using Min-Heap Priority Queue.
    // - Intuition: Grow tree starting from node 0; at each step greedily select minimum weight edge connecting MST to an unvisited vertex.
    // - Complexity: Time: O((V + E) \log V) priority queue operations, Space: O(V + E) for priority queue and visited array.


    // =========================================================
    // 18. NUMBER OF ISLANDS
    // GRID = GRAPH
    // =========================================================

    void islandDFS(int r, int c, vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') return;
        grid[r][c] = '0';
        islandDFS(r + 1, c, grid);
        islandDFS(r - 1, c, grid);
        islandDFS(r, c + 1, grid);
        islandDFS(r, c - 1, grid);
    }

    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size(), islands = 0;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == '1') { islands++; islandDFS(r, c, grid); }
            }
        }
        return islands;
    }
    // Interview Explanation:
    // - Problem Statement: Count the number of islands in an m x n 2D binary grid ('1' land, '0' water).
    // - Approach: Grid DFS flood-fill / Connected components.
    // - Intuition: Traverse each cell; when land '1' is reached, increment island count and sink connected land to '0' using 4-directional DFS.
    // - Complexity: Time: O(M \cdot N) visiting each cell once, Space: O(M \cdot N) worst-case recursion call stack.


    // =========================================================
    // 19. SHORTEST PATH IN BINARY MATRIX
    // =========================================================

    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();
        if (grid[0][0] != 0 || grid[n - 1][n - 1] != 0) return -1;

        queue<pair<int, int>> q;
        q.push({0, 0});
        grid[0][0] = 1;

        int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            if (r == n - 1 && c == n - 1) return grid[r][c];

            for (auto& dir : directions) {
                int nr = r + dir[0], nc = c + dir[1];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 0) {
                    grid[nr][nc] = grid[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
        return -1;
    }
    // Interview Explanation:
    // - Problem Statement: Find the shortest clear path from top-left (0, 0) to bottom-right (n-1, n-1) with 8-directional movement.
    // - Approach: Breadth-First Search (BFS) in 8 directions with cell distance marking.
    // - Intuition: BFS ensures that the first time destination (n-1, n-1) is reached, the path length is minimal; track distance in-place.
    // - Complexity: Time: O(N^2) visiting each matrix cell at most once, Space: O(N^2) for BFS queue.


    // =========================================================
    // 20. WORD LADDER - BFS
    // =========================================================

    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> words(wordList.begin(), wordList.end());
        if (!words.count(endWord)) return 0;

        queue<string> q;
        q.push(beginWord);
        int level = 1;

        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                string word = q.front(); q.pop();
                if (word == endWord) return level;

                for (int i = 0; i < word.size(); i++) {
                    char original = word[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        word[i] = c;
                        if (words.count(word)) {
                            q.push(word);
                            words.erase(word);
                        }
                    }
                    word[i] = original;
                }
            }
            level++;
        }
        return 0;
    }
    // Interview Explanation:
    // - Problem Statement: Find the length of the shortest transformation sequence from beginWord to endWord changing 1 letter at a time.
    // - Approach: Breadth-First Search (BFS) over implicit word graph with hash set lookup.
    // - Intuition: Words are graph nodes; generate neighbors by testing 26 substitutions per character; erase visited words from set to prevent cycles.
    // - Complexity: Time: O(M^2 \cdot N) where M is word length and N is word list size, Space: O(M \cdot N) for queue and word set.
};

/*
 ====================================================================================================
                    ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: GRAPHS (EXPANDED)
 ====================================================================================================

 1. ALGORITHM SELECTION MATRIX (EXTENDED)
    | Problem Type / Constraints                  | Algorithm / Data Structure          | Time Complexity       |
    |:--------------------------------------------|:------------------------------------|:----------------------|
    | Shortest path, unweighted                   | BFS (Queue)                         | O(V + E)              |
    | Shortest path, 0-1 edge weights             | 0-1 BFS (Deque: push_front/back)    | O(V + E)              |
    | Shortest path, non-negative weights         | Dijkstra (Min-Heap Priority Queue)  | O((V + E) log V)      |
    | Shortest path, small weight range [0..W]    | Dial's Algorithm (Bucket Queue)     | O(V + E + W*V)        |
    | Shortest path with negative weights         | Bellman-Ford (Relax edges V-1 times)| O(V * E)              |
    | Detect negative cycle                       | Bellman-Ford (Extra V-th pass)      | O(V * E)              |
    | SPFA (Queue-optimized Bellman-Ford)         | SPFA (Avoid in adversarial OAs)     | O(V * E) worst case   |
    | All-pairs shortest path (V <= 400-500)      | Floyd-Warshall (3 nested loops)     | O(V^3)                |
    | DAG shortest / longest path                 | Topo Sort + relax in order          | O(V + E)              |
    | Connected components / flood-fill           | DFS / BFS / DSU                     | O(V + E) or O(R * C)  |
    | Cycle detection (Undirected)                | DFS (track parent) OR DSU           | O(V + E)              |
    | Cycle detection (Directed)                  | DFS (3 states) OR Kahn's            | O(V + E)              |
    | Dependency resolution / Task ordering       | Topological Sort (Kahn's / DFS)     | O(V + E)              |
    | Count topological orders / detect unique    | Kahn's (Check queue size == 1 step) | O(V + E)              |
    | MST (sparse graph, E small)                 | Kruskal's (Sort edges + DSU)        | O(E log E)            |
    | MST (dense graph, E large)                  | Prim's (Min-Heap Priority Queue)    | O((V + E) log V)      |
    | Second-best MST / MST verification          | Kruskal + Binary Lifting on tree    | O(E log E + Q log V)  |
    | Dynamic connectivity / Merge components     | Disjoint Set Union (DSU / UF)       | O(alpha(N)) ~ O(1)    |
    | Bipartite check / 2-coloring                | BFS / DFS coloring (0 and 1)        | O(V + E)              |
    | Strongly Connected Components (SCC)         | Tarjan's (Single-pass, low-link)    | O(V + E)              |
    | SCC (alternative, easier to explain live)   | Kosaraju's (2 passes + transpose)   | O(V + E)              |
    | Bridges & Articulation Points               | Tarjan's low-link DFS               | O(V + E)              |
    | Max flow / min cut                          | Edmonds-Karp (BFS augmenting paths) | O(V * E^2)            |
    | Max flow, dense / large capacity graphs     | Dinic's Algorithm                   | O(V^2 * E)            |
    | Bipartite matching                          | Hopcroft-Karp OR Kuhn's Algorithm   | O(E * sqrt(V)) / O(VE)|
    | Eulerian path / circuit                     | Hierholzer's Algorithm              | O(E)                  |
    | LCA queries (static tree)                   | Binary Lifting OR Euler Tour + RMQ  | O((V log V) + log V)  |
    | Multi-source shortest distance              | Multi-source BFS / Dijkstra         | O(V + E)              |
    | K-shortest paths                            | Yen's Algorithm                     | O(K * V * (E+V log V))|

 2. GRID AS GRAPH TRANSLATION RULES
    • Cell (r, c) is a vertex with 4 neighbors: (r+1,c), (r-1,c), (r,c+1), (r,c-1).
    • 8-directional: add diagonals (r+1,c+1), (r+1,c-1), (r-1,c+1), (r-1,c-1).
    • Boundary check: `r >= 0 && r < R && c >= 0 && c < C`.
    • In-place marking: overwrite '1' with '0' or store distance directly for O(1) extra space
      (only do this if the interviewer says mutating input is OK — ask first).
    • Direction array trick for cleaner code:
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && !visited[nr][nc]) { ... }
        }
    • Encode (r, c) as a single integer `id = r * C + c` when you need a 1D visited/dist array or DSU.

 3. CYCLE DETECTION CHECKLIST
    • Undirected: `if (visited[neighbor] && neighbor != parent) -> CYCLE!`
      Watch out for multigraphs / parallel edges — track edge index, not just parent node, if
      duplicate edges are allowed.
    • Directed: DFS requires 3 states: 0 = unvisited, 1 = in current recursion call stack (path),
      2 = completely processed.
      `if (state[neighbor] == 1) -> CYCLE!`
      `if (state[neighbor] == 2) -> already resolved, skip`
    • Kahn's algorithm: if `topoOrder.size() != V` -> Directed cycle exists among remaining nodes.
    • DSU cycle detection (undirected only): if `find(u) == find(v)` before union -> adding edge
      (u, v) creates a cycle. Great for "redundant connection" style problems.

 4. LIVE INTERVIEW & OA SURVIVAL PROTOCOL
    • Step 1 — Clarify constraints before writing code:
        - Directed vs undirected?
        - Weighted vs unweighted? Can weights be negative?
        - Is the graph guaranteed connected, or could it be disconnected/have isolated nodes?
        - Self-loops or multi-edges possible?
        - Is it given as edge list, adjacency matrix, or adjacency list? (Changes setup cost.)
        - What are V and E bounds? (Drives algorithm choice — e.g. V^2 matrix DP only ok if V small.)
    • Step 2 — Pick representation:
        vector<vector<int>> graph(n);                       // unweighted
        vector<vector<pair<int,int>>> graph(n);              // weighted: {neighbor, weight}
        Adjacency matrix only when V is small (<= ~500) or graph is dense.
    • Step 3 — Always handle disconnected components:
        for (int i = 0; i < n; i++) if (!visited[i]) bfs(i);
    • Step 4 — Dijkstra stale entry guard: ALWAYS write
        if (d > dist[node]) continue;
        immediately after popping from the heap — lazy deletion is standard and expected.
    • Step 5 — Multi-source BFS: push ALL source vertices into the queue at depth 0 BEFORE the
      main loop starts (e.g. Rotting Oranges, 0-1 Matrix, Walls and Gates).
    • Step 6 — State your complexity out loud once the approach is agreed, before coding — this
      is often what interviewers are silently grading, and it catches wrong-algorithm choices early.
    • Step 7 — Narrate edge cases while coding: empty graph, single node, disconnected graph,
      negative weight when Dijkstra was assumed, self-loop counted as a cycle incorrectly.
    • Step 8 — Test on paper with a tiny example (3-4 nodes) before claiming "done" — traces
      dry-run bugs like off-by-one boundary checks or wrong initial distance (0 vs INF).

 5. COMPLEXITY & PITFALL QUICK-REFERENCE
    • BFS/DFS: O(V + E). Adjacency matrix instead of list silently makes this O(V^2) — mention it.
    • Forgetting `visited` check before pushing (not just before popping) in BFS -> duplicate
      queue entries, wrong distances, sometimes infinite loop on cyclic graphs.
    • Recursive DFS on large graphs (V > ~10^4-10^5) can stack-overflow — mention iterative DFS
      with explicit stack as a fallback.
    • Floyd-Warshall loop order MUST be `for k { for i { for j } } }` — k (intermediate node)
      outermost, or the DP is wrong.
    • Bellman-Ford: relax all edges exactly V-1 times; a Vth pass that still relaxes something
      signals a negative cycle.
    • Dijkstra does NOT work with negative edge weights — falls back to Bellman-Ford if negatives
      are possible, even just one edge.
    • DSU without path compression + union by rank degrades toward O(V) per op on adversarial
      inputs — always include both optimizations in an interview.
 ====================================================================================================
*/