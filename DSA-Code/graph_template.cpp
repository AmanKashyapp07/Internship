#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
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

    bool dfsCheckBipartite(int node, int col, vector<int>& color, const vector<vector<int>>& graph) {
        color[node] = col;
        for (int neighbor : graph[node]) {
            if (color[neighbor] == -1) {
                if (!dfsCheckBipartite(neighbor, 1 - col, color, graph)) return false;
            } else if (color[neighbor] == col) {
                return false;
            }
        }
        return true;
    }

    bool isBipartiteDFS(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                if (!dfsCheckBipartite(i, 0, color, graph)) return false;
            }
        }
        return true;
    }
    // Interview Explanation:
    // - Problem Statement: Check if graph is bipartite using Depth-First Search (DFS) 2-coloring (LeetCode 785).
    // - Approach: Recursive DFS 2-Coloring alternate assignment `(1 - col)`.
    // - Intuition: If an adjacent neighbor already has the same color as current node, the graph contains an odd-length cycle and is NOT bipartite.
    // - Complexity: Time: O(V + E), Space: O(V) for recursion stack and color array.


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

    // Print Shortest Path in Weighted Graph (GFG / Striver SDE)
    vector<int> dijkstraPrintPath(int n, vector<vector<pair<int, int>>>& graph, int src, int dest) {
        const int INF = 1e9;
        vector<int> dist(n, INF), parent(n);
        iota(parent.begin(), parent.end(), 0);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [distance, node] = pq.top(); pq.pop();
            if (distance != dist[node]) continue;

            for (auto [neighbor, weight] : graph[node]) {
                if (distance + weight < dist[neighbor]) {
                    dist[neighbor] = distance + weight;
                    parent[neighbor] = node;
                    pq.push({dist[neighbor], neighbor});
                }
            }
        }

        if (dist[dest] == INF) return {-1};
        vector<int> path;
        for (int curr = dest; curr != src; curr = parent[curr]) path.push_back(curr);
        path.push_back(src);
        reverse(path.begin(), path.end());
        return path;
    }
    // Interview Explanation:
    // - Problem Statement: Print the shortest path vertices from source to destination in a weighted graph.
    // - Approach: Dijkstra with parent pointer tracking + Backtracking path reconstruction.
    // - Intuition: Record `parent[neighbor] = node` on every edge relaxation. Backtrack from `dest` to `src` using parent array and reverse.
    // - Complexity: Time: O((V + E) \log V), Space: O(V) for parent and distance arrays.


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
    // 11. DISJOINT SET UNION (DSU)
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
    // 12. CYCLE DETECTION USING DSU
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
    // 13. KRUSKAL'S MST
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
    // 14. PRIM'S MST
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
    // 15. SHORTEST PATH IN BINARY MATRIX
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
    // 16. WORD LADDER - BFS
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


    // =========================================================
    // 17. MULTI-SOURCE BFS
    // =========================================================

    vector<int> multiSourceBFS(int n, vector<vector<int>>& graph, vector<int>& sources) {
        vector<int> dist(n, INT_MAX);
        queue<int> q;
        for (int src : sources) {
            dist[src] = 0;
            q.push(src);
        }
        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (int nbr : graph[node]) {
                if (dist[nbr] > dist[node] + 1) {
                    dist[nbr] = dist[node] + 1;
                    q.push(nbr);
                }
            }
        }
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find shortest distances to all vertices starting from multiple source vertices simultaneously.
    // - Approach: Multi-source BFS pushing all initial sources into queue at depth 0.
    // - Intuition: Pushing all sources upfront propagates wavefronts evenly across the graph in level-order.
    // - Complexity: Time: O(V + E), Space: O(V) for queue and distance array.


    // =========================================================
    // 18. K SHORTEST PATHS
    // =========================================================

    vector<long long> kShortestPaths(int n, vector<vector<pair<int, int>>>& graph, int src, int dest, int k) {
        vector<int> cnt(n, 0);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        pq.push({0, src});
        vector<long long> answer;
        while (!pq.empty()) {
            auto [dist, node] = pq.top(); pq.pop();
            cnt[node]++;
            if (node == dest) {
                answer.push_back(dist);
                if (answer.size() == (size_t)k) return answer;
            }
            if (cnt[node] > k) continue;
            for (auto [nbr, wt] : graph[node]) {
                pq.push({dist + wt, nbr});
            }
        }
        return answer;
    }
    // Interview Explanation:
    // - Problem Statement: Determine top K shortest path distances from source to destination in a weighted graph.
    // - Approach: Modified Dijkstra tracking visit count per node up to K times.
    // - Intuition: Allowing each node to be popped at most K times guarantees finding the K smallest path costs to destination.
    // - Complexity: Time: O(K \cdot E \log V), Space: O(K \cdot V).


    // =========================================================
    // 19. EVENTUAL SAFE STATES
    // =========================================================

    vector<int> eventualSafeNodes(int V, vector<vector<int>>& graph) {
        vector<vector<int>> revGraph(V);
        vector<int> indegree(V, 0);
        for (int i = 0; i < V; ++i) {
            for (auto it : graph[i]) {
                revGraph[it].push_back(i);
                indegree[i]++;
            }
        }
        queue<int> q;
        for (int i = 0; i < V; ++i) if (indegree[i] == 0) q.push(i);
        vector<int> safeNodes;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            safeNodes.push_back(node);
            for (auto it : revGraph[node]) {
                indegree[it]--;
                if (indegree[it] == 0) q.push(it);
            }
        }
        sort(safeNodes.begin(), safeNodes.end());
        return safeNodes;
    }
    // Interview Explanation:
    // - Problem Statement: Find all nodes from which every path leads to a terminal node (no cycle reachable).
    // - Approach: Reversed graph construction + Kahn's Topological Sort from terminal nodes (indegree 0).
    // - Intuition: Terminal nodes have outdegree 0 in original graph; reversing edges turns outdegree 0 into indegree 0 for standard Kahn's BFS.
    // - Complexity: Time: O(V + E \log V), Space: O(V + E).


    // =========================================================
    // 20. SHORTEST PATH IN DAG VIA TOPO SORT
    // =========================================================

    vector<int> shortestPathDAG(int V, const vector<vector<pair<int, int>>>& graph, int src) {
        vector<int> indegree(V, 0);
        for (int i = 0; i < V; i++) {
            for (auto it : graph[i]) indegree[it.first]++;
        }
        queue<int> q;
        for (int i = 0; i < V; i++) if (indegree[i] == 0) q.push(i);
        vector<int> topo;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            topo.push_back(node);
            for (auto it : graph[node]) {
                indegree[it.first]--;
                if (indegree[it.first] == 0) q.push(it.first);
            }
        }
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;
        for (int i = 0; i < V; i++) {
            int node = topo[i];
            if (dist[node] != INT_MAX) {
                for (auto it : graph[node]) {
                    int v = it.first, wt = it.second;
                    if (dist[node] + wt < dist[v]) dist[v] = dist[node] + wt;
                }
            }
        }
        for (int i = 0; i < V; i++) if (dist[i] == INT_MAX) dist[i] = -1;
        return dist;
    }
    // Interview Explanation:
    // - Problem Statement: Find shortest path distances from source to all vertices in a Directed Acyclic Graph (DAG).
    // - Approach: Topological Sort followed by single-pass edge relaxation in topological order.
    // - Intuition: Processing vertices in topological order guarantees all incoming shortest path distances are fully resolved before relaxing outgoing edges.
    // - Complexity: Time: O(V + E), Space: O(V).


    // =========================================================
    // 21. PATH WITH MINIMUM EFFORT
    // =========================================================

    int minimumEffortPath(vector<vector<int>>& heights) {
        int n = heights.size(), m = heights[0].size();
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
        vector<vector<int>> diff(n, vector<int>(m, INT_MAX));
        diff[0][0] = 0;
        pq.push({0, {0, 0}});
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};
        while (!pq.empty()) {
            auto [effort, pos] = pq.top(); pq.pop();
            auto [r, c] = pos;
            if (r == n - 1 && c == m - 1) return effort;
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i], nc = c + dc[i];
                if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                    int newEffort = max(effort, abs(heights[r][c] - heights[nr][nc]));
                    if (newEffort < diff[nr][nc]) {
                        diff[nr][nc] = newEffort;
                        pq.push({newEffort, {nr, nc}});
                    }
                }
            }
        }
        return 0;
    }
    // Interview Explanation:
    // - Problem Statement: Find a path from top-left to bottom-right minimizing the maximum absolute height difference between adjacent cells.
    // - Approach: Modified Dijkstra's Algorithm storing max height difference along the path.
    // - Intuition: Min-Heap priority queue greedily expands the path with current minimum effort bottleneck first.
    // - Complexity: Time: O(N \cdot M \log(N \cdot M)), Space: O(N \cdot M).


    // =========================================================
    // 22. NETWORK DELAY TIME
    // =========================================================

    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int, int>>> graph(n + 1);
        for (auto& t : times) graph[t[0]].push_back({t[1], t[2]});
        vector<int> dist(n + 1, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[k] = 0;
        pq.push({0, k});
        while (!pq.empty()) {
            auto [d, node] = pq.top(); pq.pop();
            if (d > dist[node]) continue;
            for (auto& it : graph[node]) {
                if (d + it.second < dist[it.first]) {
                    dist[it.first] = d + it.second;
                    pq.push({dist[it.first], it.first});
                }
            }
        }
        int mxTime = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            mxTime = max(mxTime, dist[i]);
        }
        return mxTime;
    }
    // Interview Explanation:
    // - Problem Statement: Calculate minimum time required for signal sent from node K to reach all N nodes in network.
    // - Approach: Dijkstra's Algorithm from source K tracking max travel time across all nodes.
    // - Intuition: The time for all nodes to receive the signal is the maximum of all shortest path distances from K.
    // - Complexity: Time: O((V + E) \log V), Space: O(V + E).


    // =========================================================
    // 23. NUMBER OF WAYS TO ARRIVE AT DESTINATION
    // =========================================================

    int countPaths(int n, vector<vector<int>>& roads) {
        const long long INF_VAL = LLONG_MAX;
        const int MOD_VAL = 1e9 + 7;
        vector<vector<pair<long long, int>>> graph(n);
        for (auto& r : roads) {
            graph[r[0]].push_back({r[1], r[2]});
            graph[r[1]].push_back({r[0], r[2]});
        }
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        vector<long long> dist(n, INF_VAL), ways(n, 0);
        dist[0] = 0; ways[0] = 1;
        pq.push({0, 0});
        while (!pq.empty()) {
            auto [d, node] = pq.top(); pq.pop();
            if (d > dist[node]) continue;
            for (auto& it : graph[node]) {
                int v = it.first;
                long long wt = it.second;
                if (d + wt < dist[v]) {
                    dist[v] = d + wt;
                    ways[v] = ways[node];
                    pq.push({dist[v], v});
                } else if (d + wt == dist[v]) {
                    ways[v] = (ways[v] + ways[node]) % MOD_VAL;
                }
            }
        }
        return ways[n - 1] % MOD_VAL;
    }
    // Interview Explanation:
    // - Problem Statement: Count the number of shortest paths from node 0 to node n-1 modulo 10^9+7.
    // - Approach: Dijkstra's algorithm tracking both shortest distance `dist` and ways count `ways`.
    // - Intuition: When finding a strictly shorter path, overwrite `ways[v] = ways[u]`; when finding an equal distance path, accumulate `ways[v] += ways[u]`.
    // - Complexity: Time: O((V + E) \log V), Space: O(V + E).


    // =========================================================
    // 24. NUMBER OF OPERATIONS TO MAKE NETWORK CONNECTED
    // =========================================================

    int makeConnected(int n, vector<vector<int>>& connections) {
        if (connections.size() < (size_t)(n - 1)) return -1;
        DSU ds(n);
        for (auto &e : connections) ds.unite(e[0], e[1]);
        int components = 0;
        for (int i = 0; i < n; i++) if (ds.find(i) == i) components++;
        return components - 1;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum cables to move to connect all n computers (or return -1 if impossible).
    // - Approach: DSU component counting (need at least n-1 edges total, and C components require C-1 moves).
    // - Intuition: If total edges >= n - 1, extra redundant edges can always be re-wired to connect the C disconnected components.
    // - Complexity: Time: O(E \cdot \alpha(V)), Space: O(V).


    // =========================================================
    // 25. MOST STONES REMOVED WITH SAME ROW OR COLUMN
    // =========================================================

    int removeStones(vector<vector<int>>& stones) {
        int maxRow = 0, maxCol = 0;
        for (auto &s : stones) {
            maxRow = max(maxRow, s[0]);
            maxCol = max(maxCol, s[1]);
        }
        DSU ds(maxRow + maxCol + 2);
        unordered_set<int> used;
        for (auto &s : stones) {
            int row = s[0], col = s[1] + maxRow + 1;
            ds.unite(row, col);
            used.insert(row);
            used.insert(col);
        }
        int components = 0;
        for (int node : used) if (ds.find(node) == node) components++;
        return stones.size() - components;
    }
    // Interview Explanation:
    // - Problem Statement: Remove maximum number of stones sharing row/column with another stone.
    // - Approach: DSU linking row nodes to offset column nodes (`col + maxRow + 1`).
    // - Intuition: A connected component of N stones sharing row/col coordinates can always be reduced down to 1 stone; answer is `totalStones - components`.
    // - Complexity: Time: O(N \cdot \alpha(\text{range})), Space: O(N + \text{maxRow} + \text{maxCol}).


    // =========================================================
    // 26. NUMBER OF ISLANDS II (DYNAMIC LAND ADDITION)
    // =========================================================

    vector<int> numIslandsII(int n, int m, vector<vector<int>>& queries) {
        DSU ds(n * m);
        vector<vector<int>> vis(n, vector<int>(m, 0));
        vector<int> ans;
        int cnt = 0;
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};
        for (auto &q : queries) {
            int r = q[0], c = q[1];
            if (vis[r][c]) { ans.push_back(cnt); continue; }
            vis[r][c] = 1;
            cnt++;
            int node = r * m + c;
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nr >= n || nc < 0 || nc >= m || !vis[nr][nc]) continue;
                int adjNode = nr * m + nc;
                if (ds.unite(node, adjNode)) cnt--;
            }
            ans.push_back(cnt);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Maintain island count dynamically as new land cells are added sequentially.
    // - Approach: DSU dynamic component merging with 2D-to-1D index mapping (`r * m + c`).
    // - Intuition: Adding land increments island count by 1; merging with adjacent land cells decrements count whenever `ds.unite` merges two distinct sets.
    // - Complexity: Time: O(Q \cdot \alpha(N \cdot M)), Space: O(N \cdot M).

    // =========================================================
    // 27. BRIDGES IN GRAPH / CRITICAL CONNECTIONS (LEETCODE 1192)
    // =========================================================

    void tarjanBridgeDFS(int node, int parent, int &timer, vector<int> &tin, vector<int> &low,
                         vector<int> &vis, vector<vector<int>> &adj, vector<vector<int>> &bridges) {
        vis[node] = 1;
        tin[node] = low[node] = timer++;
        for (int neighbor : adj[node]) {
            if (neighbor == parent) continue;
            if (!vis[neighbor]) {
                tarjanBridgeDFS(neighbor, node, timer, tin, low, vis, adj, bridges);
                low[node] = min(low[node], low[neighbor]);
                if (low[neighbor] > tin[node]) {
                    bridges.push_back({node, neighbor});
                }
            } else {
                low[node] = min(low[node], tin[neighbor]);
            }
        }
    }

    vector<vector<int>> criticalConnections(int n, vector<vector<int>> &connections) {
        vector<vector<int>> adj(n);
        for (auto &edge : connections) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        vector<int> tin(n, -1), low(n, -1), vis(n, 0);
        vector<vector<int>> bridges;
        int timer = 0;
        tarjanBridgeDFS(0, -1, timer, tin, low, vis, adj, bridges);
        return bridges;
    }
    // Interview Explanation:
    // - Problem Statement: Find all critical connections (bridges) whose removal disconnects the graph.
    // - Approach: Tarjan's Low-Link Depth-First Search ($low[v] > tin[u]$).
    // - Intuition: Track insertion time `tin` and lowest reachable time `low`. If child `v` cannot reach ancestor of `u` via back-edge, edge `(u, v)` is a bridge.
    // - Complexity: Time: O(V + E) single DFS traversal, Space: O(V + E) adjacency graph & recursion stack.


    // =========================================================
    // 28. ARTICULATION POINTS IN GRAPH (TARJAN'S ALGORITHM)
    // =========================================================

    void tarjanAPDFS(int node, int parent, int &timer, vector<int> &tin, vector<int> &low,
                     vector<int> &vis, vector<int> &isAP, vector<vector<int>> &adj) {
        vis[node] = 1;
        tin[node] = low[node] = timer++;
        int children = 0;

        for (int neighbor : adj[node]) {
            if (neighbor == parent) continue;
            if (!vis[neighbor]) {
                children++;
                tarjanAPDFS(neighbor, node, timer, tin, low, vis, isAP, adj);
                low[node] = min(low[node], low[neighbor]);
                if (low[neighbor] >= tin[node] && parent != -1) {
                    isAP[node] = 1;
                }
            } else {
                low[node] = min(low[node], tin[neighbor]);
            }
        }
        if (parent == -1 && children > 1) {
            isAP[node] = 1;
        }
    }

    vector<int> articulationPoints(int V, vector<vector<int>> &adj) {
        vector<int> tin(V, -1), low(V, -1), vis(V, 0), isAP(V, 0);
        int timer = 0;
        for (int i = 0; i < V; ++i) {
            if (!vis[i]) {
                tarjanAPDFS(i, -1, timer, tin, low, vis, isAP, adj);
            }
        }
        vector<int> ans;
        for (int i = 0; i < V; ++i) if (isAP[i]) ans.push_back(i);
        return ans.empty() ? vector<int>{-1} : ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find all articulation points (cut vertices) whose removal increases connected components.
    // - Approach: Tarjan's Low-Link DFS with root degree and subtree reachability conditions.
    // - Intuition: Non-root node $u$ is AP if $low[v] \ge tin[u]$ for some child $v$; Root is AP if it has $> 1$ independent DFS children.
    // - Complexity: Time: O(V + E), Space: O(V) tracking arrays.


    // =========================================================
    // 29. STRONGLY CONNECTED COMPONENTS (KOSARAJU'S ALGORITHM)
    // =========================================================

    void kosarajuDFS1(int node, vector<int> &vis, stack<int> &st, vector<vector<int>> &adj) {
        vis[node] = 1;
        for (int neighbor : adj[node]) {
            if (!vis[neighbor]) kosarajuDFS1(neighbor, vis, st, adj);
        }
        st.push(node);
    }

    void kosarajuDFS2(int node, vector<int> &vis, vector<vector<int>> &adjT, vector<int> &component) {
        vis[node] = 1;
        component.push_back(node);
        for (int neighbor : adjT[node]) {
            if (!vis[neighbor]) kosarajuDFS2(neighbor, vis, adjT, component);
        }
    }

    vector<vector<int>> kosarajuSCC(int V, vector<vector<int>> &adj) {
        stack<int> st;
        vector<int> vis(V, 0);

        // Step 1: Sort nodes by finishing time
        for (int i = 0; i < V; ++i) {
            if (!vis[i]) kosarajuDFS1(i, vis, st, adj);
        }

        // Step 2: Transpose graph (reverse all directed edges)
        vector<vector<int>> adjT(V);
        for (int i = 0; i < V; ++i) {
            for (int neighbor : adj[i]) {
                adjT[neighbor].push_back(i);
            }
        }

        // Step 3: DFS on transposed graph in order of decreasing finish time
        fill(vis.begin(), vis.end(), 0);
        vector<vector<int>> sccList;
        while (!st.empty()) {
            int node = st.top(); st.pop();
            if (!vis[node]) {
                vector<int> component;
                kosarajuDFS2(node, vis, adjT, component);
                sccList.push_back(component);
            }
        }
        return sccList;
    }
    // Interview Explanation:
    // - Problem Statement: Find all Strongly Connected Components (SCCs) in a directed graph.
    // - Approach: Kosaraju's Two-Pass DFS with Graph Transposition.
    // - Intuition: Transposing graph prevents cross-component DFS leakage; processing in decreasing finish time extracts each SCC cleanly.
    // - Complexity: Time: O(V + E) two linear traversals, Space: O(V + E) for transposed graph and stack.


    

    

    // =========================================================
    // 30. CHEAPEST FLIGHTS WITHIN K STOPS (LEETCODE 787)
    // =========================================================

    int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k) {
        vector<vector<pair<int, int>>> adj(n);
        for (auto &f : flights) {
            adj[f[0]].push_back({f[1], f[2]});
        }

        queue<tuple<int, int, int>> q; // {stops, node, cost}
        q.push({0, src, 0});
        vector<int> dist(n, 1e9);
        dist[src] = 0;

        while (!q.empty()) {
            auto [stops, node, cost] = q.front();
            q.pop();

            if (stops > k) continue;

            for (auto &[neighbor, price] : adj[node]) {
                if (cost + price < dist[neighbor] && stops <= k) {
                    dist[neighbor] = cost + price;
                    q.push({stops + 1, neighbor, cost + price});
                }
            }
        }
        return dist[dst] == 1e9 ? -1 : dist[dst];
    }
    // Interview Explanation:
    // - Problem Statement: Find cheapest flight price from src to dst with at most K intermediate stops.
    // - Approach: BFS Level Queue (incrementing stops by 1 per level) with cost relaxation.
    // - Intuition: Standard Dijkstra prioritizes cost over stops which can prune valid K-stop paths; BFS guarantees monotonic stop progression.
    // - Complexity: Time: O(K * E), Space: O(V + E) adjacency list.


    // =========================================================
    // 31. ALIEN DICTIONARY (LEETCODE 269 / GFG)
    // =========================================================

    string findAlienOrder(vector<string> &words, int k) {
        vector<vector<int>> adj(k);
        vector<int> indegree(k, 0);

        for (int i = 0; i < (int)words.size() - 1; ++i) {
            string w1 = words[i], w2 = words[i + 1];
            int len = min(w1.size(), w2.size());
            bool foundDiff = false;
            for (int j = 0; j < len; ++j) {
                if (w1[j] != w2[j]) {
                    adj[w1[j] - 'a'].push_back(w2[j] - 'a');
                    indegree[w2[j] - 'a']++;
                    foundDiff = true;
                    break;
                }
            }
            if (!foundDiff && w1.size() > w2.size()) return ""; // Invalid prefix order
        }

        queue<int> q;
        for (int i = 0; i < k; ++i) if (indegree[i] == 0) q.push(i);
        string order = "";

        while (!q.empty()) {
            int u = q.front(); q.pop();
            order += (char)(u + 'a');
            for (int v : adj[u]) {
                if (--indegree[v] == 0) q.push(v);
            }
        }
        return (int)order.size() == k ? order : "";
    }
    // Interview Explanation:
    // - Problem Statement: Reconstruct the lexicographical character ordering of an alien language from a sorted dictionary of words.
    // - Approach: Directed Graph Construction + Kahn's BFS Topological Sort.
    // - Intuition: Compare adjacent words to find first differing character $c_1 	o c_2$. Topo sort gives valid linear precedence.
    // - Complexity: Time: O(N * |word| + K), Space: O(K) alphabet graph.


    // =========================================================
    // 32. COURSE SCHEDULE I & II (LEETCODE 207 & 210)
    // =========================================================

    bool canFinishCourses(int numCourses, vector<vector<int>> &prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);
        for (auto &p : prerequisites) {
            adj[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i) if (indegree[i] == 0) q.push(i);
        int count = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            count++;
            for (int v : adj[u]) {
                if (--indegree[v] == 0) q.push(v);
            }
        }
        return count == numCourses;
    }

    vector<int> findOrderCourses(int numCourses, vector<vector<int>> &prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);
        for (auto &p : prerequisites) {
            adj[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i) if (indegree[i] == 0) q.push(i);
        vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (--indegree[v] == 0) q.push(v);
            }
        }
        return (int)order.size() == numCourses ? order : vector<int>{};
    }
    // Interview Explanation:
    // - Problem Statement: Detect if course prerequisite cycle exists, and generate valid course completion order.
    // - Approach: Kahn's Algorithm (BFS Topological Sort) on prerequisite graph.
    // - Intuition: Indegree 0 indicates courses with no prerequisites; processing and decrementing neighbor indegrees detects cycles when processed count < V.
    // - Complexity: Time: O(V + E), Space: O(V + E) adjacency graph & indegree array.


    // =========================================================
    // 33. ACCOUNTS MERGE (LEETCODE 721)
    // =========================================================

    vector<vector<string>> accountsMerge(vector<vector<string>> &accounts) {
        int n = accounts.size();
        DSU ds(n);
        unordered_map<string, int> mapMailNode;

        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < (int)accounts[i].size(); ++j) {
                string mail = accounts[i][j];
                if (mapMailNode.find(mail) == mapMailNode.end()) {
                    mapMailNode[mail] = i;
                } else {
                    ds.unite(i, mapMailNode[mail]);
                }
            }
        }

        vector<vector<string>> mergedMail(n);
        for (auto &[mail, id] : mapMailNode) {
            int root = ds.find(id);
            mergedMail[root].push_back(mail);
        }

        vector<vector<string>> ans;
        for (int i = 0; i < n; ++i) {
            if (mergedMail[i].empty()) continue;
            sort(mergedMail[i].begin(), mergedMail[i].end());
            vector<string> temp;
            temp.push_back(accounts[i][0]);
            for (auto &mail : mergedMail[i]) temp.push_back(mail);
            ans.push_back(temp);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Merge accounts sharing the same email under common user names.
    // - Approach: Disjoint Set Union (DSU) mapping emails to account indices.
    // - Intuition: Connect account IDs sharing identical emails via DSU; group emails by parent component root and sort alphabetically.
    // - Complexity: Time: O(N * M \log(NM)), Space: O(N * M).


    // =========================================================
    // 34. M-COLORING PROBLEM (GFG)
    // =========================================================

    bool isColorSafe(int node, vector<int> &color, vector<vector<int>> &graph, int n, int col) {
        for (int k = 0; k < n; ++k) {
            if (k != node && graph[k][node] == 1 && color[k] == col) {
                return false;
            }
        }
        return true;
    }

    bool solveMColoring(int node, vector<int> &color, int m, int n, vector<vector<int>> &graph) {
        if (node == n) return true;
        for (int i = 1; i <= m; ++i) {
            if (isColorSafe(node, color, graph, n, i)) {
                color[node] = i;
                if (solveMColoring(node + 1, color, m, n, graph)) return true;
                color[node] = 0; // backtrack
            }
        }
        return false;
    }

    bool graphColoring(vector<vector<int>> &graph, int m, int n) {
        vector<int> color(n, 0);
        return solveMColoring(0, color, m, n, graph);
    }
    // Interview Explanation:
    // - Problem Statement: Determine if an undirected graph can be colored with at most M colors such that no adjacent vertices share the same color.
    // - Approach: Backtracking vertex assignment from 1 to M.
    // - Intuition: Try colors 1..M on vertex `node`; if safe, recursively color `node + 1`. If no color works, backtrack.
    // - Complexity: Time: O(M^N), Space: O(N) recursion stack and color array.


    // =========================================================
    // 35. FLOOD FILL (LEETCODE 733)
    // =========================================================

    void floodFillDFS(int r, int c, vector<vector<int>> &image, int initialColor, int newColor, int dr[], int dc[]) {
        image[r][c] = newColor;
        int m = image.size(), n = image[0].size();
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && image[nr][nc] == initialColor) {
                floodFillDFS(nr, nc, image, initialColor, newColor, dr, dc);
            }
        }
    }

    vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int color) {
        int initialColor = image[sr][sc];
        if (initialColor != color) {
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            floodFillDFS(sr, sc, image, initialColor, color, dr, dc);
        }
        return image;
    }
    // Interview Explanation:
    // - Problem Statement: Perform flood fill on an image starting from pixel (sr, sc) replacing connected matching pixels with newColor.
    // - Approach: 4-directional Depth-First Search (DFS) / BFS.
    // - Intuition: Mutate matching neighboring pixels to `newColor` in-place, preventing infinite cycles.
    // - Complexity: Time: O(M * N), Space: O(M * N) recursion stack in worst case.


    // =========================================================
    // 36. PACIFIC ATLANTIC WATER FLOW (LEETCODE 417)
    // =========================================================

    void dfsPacificAtlantic(const vector<vector<int>>& heights, vector<vector<bool>>& visited, int r, int c, int m, int n) {
        visited[r][c] = true;
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !visited[nr][nc] && heights[nr][nc] >= heights[r][c]) {
                dfsPacificAtlantic(heights, visited, nr, nc, m, n);
            }
        }
    }

    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        if (heights.empty()) return {};
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));

        for (int i = 0; i < m; i++) {
            dfsPacificAtlantic(heights, pacific, i, 0, m, n);
            dfsPacificAtlantic(heights, atlantic, i, n - 1, m, n);
        }
        for (int j = 0; j < n; j++) {
            dfsPacificAtlantic(heights, pacific, 0, j, m, n);
            dfsPacificAtlantic(heights, atlantic, m - 1, j, m, n);
        }

        vector<vector<int>> result;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pacific[i][j] && atlantic[i][j]) result.push_back({i, j});
            }
        }
        return result;
    }
    // Interview Explanation:
    // - Problem Statement: Find all grid coordinates where water can flow to both the Pacific and Atlantic oceans (LeetCode 417).
    // - Approach: Reverse Uphill DFS/BFS from ocean borders (`heights[nr][nc] >= heights[r][c]`).
    // - Intuition: Water flowing downhill to both oceans is equivalent to water flowing uphill from the Pacific and Atlantic boundaries simultaneously. Intersection of reachable cells gives the answer.
    // - Complexity: Time: O(M * N), Space: O(M * N) for boolean visited matrices.


    // =========================================================
    // 37. EULERIAN PATH / CIRCUIT (HIERHOLZER'S ALGORITHM)
    // =========================================================

    vector<string> findItinerary(vector<vector<string>>& tickets) {
        unordered_map<string, priority_queue<string, vector<string>, greater<string>>> graph;
        for (auto& t : tickets) graph[t[0]].push(t[1]);

        vector<string> route;
        function<void(string)> dfs = [&](string airport) {
            while (!graph[airport].empty()) {
                string next = graph[airport].top();
                graph[airport].pop();
                dfs(next);
            }
            route.push_back(airport);
        };

        dfs("JFK");
        reverse(route.begin(), route.end());
        return route;
    }
    // Interview Explanation:
    // - Problem Statement: Reconstruct itinerary / find Eulerian path visiting all directed edges in lexical order (LeetCode 332).
    // - Approach: Hierholzer's Algorithm using Post-order DFS and Min-Heaps.
    // - Intuition: Greedily follow edges in lexicographical order. When reaching a dead end with no outgoing edges left, append node to route; finally reverse route to obtain valid Eulerian path.
    // - Complexity: Time: O(E \log E), Space: O(V + E) for adjacency list and recursion stack.


    // =========================================================
    // 38. TARJAN'S STRONGLY CONNECTED COMPONENTS (SCC)
    // =========================================================

    void tarjanSCCDFS(int u, int& timer, const vector<vector<int>>& graph, vector<int>& disc,
                      vector<int>& low, vector<bool>& inStack, stack<int>& st, vector<vector<int>>& sccs) {
        disc[u] = low[u] = ++timer;
        st.push(u);
        inStack[u] = true;

        for (int v : graph[u]) {
            if (disc[v] == 0) {
                tarjanSCCDFS(v, timer, graph, disc, low, inStack, st, sccs);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }

        if (low[u] == disc[u]) {
            vector<int> scc;
            while (true) {
                int node = st.top(); st.pop();
                inStack[node] = false;
                scc.push_back(node);
                if (node == u) break;
            }
            sccs.push_back(scc);
        }
    }

    vector<vector<int>> tarjanSCC(int n, const vector<vector<int>>& graph) {
        vector<int> disc(n, 0), low(n, 0);
        vector<bool> inStack(n, false);
        stack<int> st;
        vector<vector<int>> sccs;
        int timer = 0;

        for (int i = 0; i < n; i++) {
            if (disc[i] == 0) tarjanSCCDFS(i, timer, graph, disc, low, inStack, st, sccs);
        }
        return sccs;
    }
    // Interview Explanation:
    // - Problem Statement: Find all Strongly Connected Components (SCCs) in a directed graph in a single DFS pass.
    // - Approach: Tarjan's Single-Pass Low-Link Algorithm with an explicit recursion stack.
    // - Intuition: Maintain discovery times `disc` and lowest reachable ancestor `low`. When DFS finishes exploring `u` and `low[u] == disc[u]`, `u` is the head of an SCC; pop all nodes from stack down to `u`.
    // - Complexity: Time: O(V + E) single DFS pass, Space: O(V) for recursion stack, discovery arrays, and inStack tracking.


    // =========================================================
    // 39. DISTINCT NUMBERS IN EACH SUBARRAY (SLIDING WINDOW)
    // =========================================================

    vector<int> distinctNumbersInWindow(const vector<int>& nums, int k) {
        int n = nums.size();
        if (k > n || k <= 0) return {};
        unordered_map<int, int> freq;
        vector<int> ans;

        for (int i = 0; i < k; i++) freq[nums[i]]++;
        ans.push_back(freq.size());

        for (int i = k; i < n; i++) {
            if (--freq[nums[i - k]] == 0) freq.erase(nums[i - k]);
            freq[nums[i]]++;
            ans.push_back(freq.size());
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Count distinct elements in every sliding window of size k in an array (GFG / Striver SDE #149).
    // - Approach: Sliding Window with Hash Map frequency tracking.
    // - Intuition: Maintain frequency map for window of size k. When sliding, decrement frequency of exiting element (removing key if 0) and increment entering element; `freq.size()` directly gives distinct element count.
    // - Complexity: Time: O(N) single pass, Space: O(K) hash map capacity.
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