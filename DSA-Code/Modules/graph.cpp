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

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | DFS Traversal                               | Recursive Adjacency Traversal     | O(V + E)  | O(V)     |
 | 2  | BFS Traversal                               | Level-by-Level Queue Traversal    | O(V + E)  | O(V)     |
 | 3  | Number of Connected Components              | Unvisited Component DFS / BFS     | O(V + E)  | O(V)     |
 | 4  | Cycle Detection (Undirected Graph)          | Parent-Tracking DFS / BFS         | O(V + E)  | O(V)     |
 | 5  | Cycle Detection (Directed Graph)            | 3-Color / Recursion Path Set DFS  | O(V + E)  | O(V)     |
 | 6  | Bipartite Graph Check (BFS / DFS)           | 2-Color Alternate Parity Check    | O(V + E)  | O(V)     |
 | 7  | Topological Sort (Kahn's Algorithm)         | In-Degree 0 Queue Reduction (BFS) | O(V + E)  | O(V)     |
 | 8  | Topological Sort (DFS)                      | Post-Order DFS Reverse Order      | O(V + E)  | O(V)     |
 | 9  | Shortest Path in Unweighted Graph           | Unit-Edge BFS Distance Array      | O(V + E)  | O(V)     |
 | 10 | Dijkstra's Shortest Path (Positive Weights) | Min-Heap Priority Queue (Greedy)  | O((V+E)lgV| O(V)     |
 | 11 | 0-1 BFS                                     | Double-Ended Queue (push_front/back)| O(V + E)| O(V)     |
 | 12 | Bellman-Ford Algorithm                      | (V - 1) Edge Relaxation + Cycle Det| O(V * E) | O(V)     |
 | 13 | Floyd-Warshall All-Pairs Shortest Path      | 3D Matrix DP Relaxation (k, i, j) | O(V^3)    | O(V^2)   |
 | 14 | Disjoint Set Union (DSU / Union-Find)       | Path Compression & Union by Rank  | O(alpha(N)| O(N)     |
 | 15 | Cycle Detection using DSU                   | Dynamic Edge Component Find Check | O(E alpha)| O(V)     |
 | 16 | Kruskal's Minimum Spanning Tree (MST)       | Edge Weight Sort + DSU Cycle Avoid| O(E logE) | O(V)     |
 | 17 | Prim's Minimum Spanning Tree (MST)          | Cut-Property Min-Heap Expansion   | O((V+E)lgV| O(V)     |
 | 18 | Number of Islands (LC 200)                  | 2D Grid DFS / BFS Flood Fill      | O(R * C)  | O(R * C) |
 | 19 | Shortest Path in Binary Matrix (LC 1091)    | 8-Directional Grid BFS Queue       | O(R * C)  | O(R * C) |
 | 20 | Word Ladder (LC 127)                        | BFS Shortest Transformation Path   | O(N*L*26) | O(N * L) |
 | 21 | Multi-Source BFS (Rotting Oranges / 0-1 Mat)| Multi-Queue Seeded BFS Expansion  | O(R * C)  | O(R * C) |
 | 22 | K Shortest Paths                            | Min-Heap Dijkstra with K-Visits   | O(K E logV| O(K * V) |
 | 23 | Eventual Safe States (LC 802)               | Reverse Graph Kahn's Topo Sort    | O(V + E)  | O(V)     |
 | 24 | Shortest Path in DAG via Topo Sort          | Linear Topo Order Edge Relaxation | O(V + E)  | O(V)     |
 | 25 | Path with Minimum Effort (LC 1631)          | Minimax Height Delta Dijkstra     | O(RC logRC| O(R * C) |
 | 26 | Network Delay Time (LC 743)                 | Single-Source Dijkstra Min-Heap   | O(E logV) | O(V)     |
 | 27 | Number of Ways to Arrive at Destination     | Dijkstra + Combinatorial Path DP  | O(E logV) | O(V)     |
 | 28 | Number of Operations to Connect Network     | DSU Redundant Edge Surplus Count  | O(V + E)  | O(V)     |
 | 29 | Most Stones Removed with Same Row/Col       | Row & Col Coordinate Component DSU| O(N alpha)| O(N)     |
 | 30 | Number of Islands II (Dynamic Land)         | Online 2D Grid DSU Union Merging  | O(K alpha)| O(R * C) |
 | 31 | Critical Connections / Bridges (LC 1192)    | Tarjan's Low-Link DFS (low[v]>tin) | O(V + E)  | O(V)     |
 | 32 | Articulation Points (Cut Vertices)          | Tarjan's Low-Link (low[v]>=tin[u])| O(V + E)  | O(V)     |
 | 33 | Strongly Connected Components (Kosaraju)    | 2-Pass DFS + Transpose Graph Stack| O(V + E)  | O(V)     |
 | 34 | Cheapest Flights Within K Stops (LC 787)    | Bellman-Ford / BFS with Stop Budget| O(K * E) | O(V)     |
 | 35 | Alien Dictionary (LC 269)                   | Lexicographical DAG Topo Sort     | O(N*L + A)| O(A)     |
 | 36 | Course Schedule I & II (LC 207 & 210)       | Kahn's Algorithm / Cycle Topo Sort| O(V + E)  | O(V)     |
 | 37 | Accounts Merge (LC 721)                     | Email-to-Owner DSU Component Group| O(N alpha)| O(N)     |
 | 38 | M-Coloring Problem (GFG)                    | Backtracking Vertex Color Assign  | O(M^V)    | O(V)     |
 | 39 | Flood Fill (LC 733)                         | Connected Component Color Replace | O(R * C)  | O(R * C) |
 | 40 | Pacific Atlantic Water Flow (LC 417)        | Reverse Ocean Boundary DFS / BFS  | O(R * C)  | O(R * C) |
 | 41 | Eulerian Path / Circuit (Hierholzer)        | Hierholzer's In-Out Degree Stack  | O(V + E)  | O(V + E) |
 | 42 | Tarjan's Strongly Connected Components      | Single-Pass Low-Link & Stack SCC  | O(V + E)  | O(V)     |
 | 43 | Distinct Numbers in Sliding Window          | Sliding Window Hash Map Frequencies| O(N)     | O(K)     |
 | 44 | Graph Valid Tree (LC 261)                   | DSU Edge Count & Cycle Verification| O(N a(N))| O(N)     |
 | 45 | Clone Graph (LC 133)                        | BFS / DFS + Node Clone Hash Map    | O(V + E)  | O(V)     |
 | 46 | Coin Collector (CSES 1686)                  | Kosaraju SCC Condensation + DAG DP | O(V + E)  | O(V + E) |
 | 47 | Hamiltonian Flights (CSES 1690)             | Bitmask Dynamic Programming        | O(2^N * N^2)| O(2^N * N)|
 | 48 | Planets Cycles (CSES 1751)                  | Functional Graph Cycle Offsets     | O(N)      | O(N)     |
 | 49 | Graph Girth (CSES 1707)                     | Multi-Source Unweighted BFS        | O(V(V+E)) | O(V + E) |
 | 50 | Network Breakdown (CSES 1676)               | Offline Dynamic DSU Reverse Edges  | O((M+K)a) | O(N+M+K) |
 | 51 | Network Renovation (CSES 2081)              | Leaf Pairing Centroid Shift        | O(N)      | O(N)     |
 | 52 | Flight Route Requests (CSES 1699)           | Weakly Connected Component Cycle   | O(V + E)  | O(V + E) |
 | 53 | Bus Companies (CSES 3158)                   | Virtual Nodes + Dijkstra           | O(E log V)| O(V + E) |
 | 54 | MST Edge Check (CSES 3407)                  | Kruskal Batching by Weight         | O(E log E)| O(V + E) |
 | 55 | Transfer Speeds Sum (CSES 3111)             | Reverse Kruskal DSU Product        | O(N log N)| O(N)     |
 ====================================================================================================
*/

struct GraphNode {
    int val;
    vector<GraphNode*> neighbors;
    GraphNode() : val(0), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val) : val(_val), neighbors(vector<GraphNode*>()) {}
    GraphNode(int _val, vector<GraphNode*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

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
        return (int)order.size() == n ? order : vector<int>{};
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

                for (int i = 0; i < (int)word.size(); i++) {
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
    // =========================================================
    // 40. GRAPH VALID TREE (LEETCODE 261)
    // =========================================================

    bool validTree(int n, vector<vector<int>>& edges) {
        if ((int)edges.size() != n - 1) return false;

        vector<int> parent(n);
        iota(parent.begin(), parent.end(), 0);

        function<int(int)> find = [&](int u) {
            return parent[u] == u ? u : parent[u] = find(parent[u]);
        };

        for (auto& edge : edges) {
            int rootU = find(edge[0]), rootV = find(edge[1]);
            if (rootU == rootV) return false;
            parent[rootU] = rootV;
        }

        return true;
    }
    // Interview Explanation:
    // - Problem Statement: Given n nodes and undirected edges, determine if graph forms a valid tree (LC 261).
    // - Approach: Disjoint Set Union (DSU) verifying edge count == n - 1 and acyclicity.
    // - Complexity: Time: O(N \cdot \alpha(N)), Space: O(N).


    // =========================================================
    // 41. CLONE GRAPH (LEETCODE 133)
    // =========================================================

    GraphNode* cloneGraph(GraphNode* node) {
        if (!node) return nullptr;
        unordered_map<GraphNode*, GraphNode*> copies;
        queue<GraphNode*> q;

        copies[node] = new GraphNode(node->val);
        q.push(node);

        while (!q.empty()) {
            GraphNode* curr = q.front();
            q.pop();

            for (GraphNode* neighbor : curr->neighbors) {
                if (!copies.count(neighbor)) {
                    copies[neighbor] = new GraphNode(neighbor->val);
                    q.push(neighbor);
                }
                copies[curr]->neighbors.push_back(copies[neighbor]);
            }
        }

        return copies[node];
    }
    // Interview Explanation:
    // - Problem Statement: Return a deep copy (clone) of a connected undirected graph (LC 133).
    // - Approach: BFS / DFS with Hash Map mapping original nodes to cloned nodes.
    // - Complexity: Time: O(V + E), Space: O(V) for clone hash map and queue.


    // =========================================================
    // 46. COIN COLLECTOR (CSES 1686)
    // =========================================================

    long long coinCollector(int n, const vector<long long>& coins, const vector<vector<int>>& adj) {
        vector<vector<int>> g(n + 1), rg(n + 1), dag;
        for (int u = 1; u <= n; u++) {
            for (int v : adj[u]) {
                g[u].push_back(v);
                rg[v].push_back(u);
            }
        }
        vector<bool> vis(n + 1, false);
        vector<int> order;
        function<void(int)> dfs1 = [&](int u) {
            vis[u] = true;
            for (int v : g[u]) if (!vis[v]) dfs1(v);
            order.push_back(u);
        };
        for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);

        vector<int> comp(n + 1, 0);
        int sccCount = 0;
        function<void(int, int)> dfs2 = [&](int u, int id) {
            comp[u] = id;
            for (int v : rg[u]) if (!comp[v]) dfs2(v, id);
        };
        reverse(order.begin(), order.end());
        for (int u : order) if (!comp[u]) dfs2(u, ++sccCount);

        vector<long long> sccWeight(sccCount + 1, 0);
        for (int i = 1; i <= n; i++) sccWeight[comp[i]] += coins[i];

        dag.resize(sccCount + 1);
        for (int u = 1; u <= n; u++) {
            for (int v : g[u]) {
                if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
            }
        }

        vector<long long> dp(sccCount + 1, -1);
        function<long long(int)> getDP = [&](int u) -> long long {
            if (dp[u] != -1) return dp[u];
            dp[u] = sccWeight[u];
            for (int v : dag[u]) dp[u] = max(dp[u], sccWeight[u] + getDP(v));
            return dp[u];
        };

        long long maxCoins = 0;
        for (int i = 1; i <= sccCount; i++) maxCoins = max(maxCoins, getDP(i));
        return maxCoins;
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum coins collected by traversing a directed graph (CSES 1686).
    // - Approach: Kosaraju's SCC + Condensation DAG + Dynamic Programming.
    // - Intuition: Contract strongly connected components into a DAG where each node contains total component coins; run DP for longest path.
    // - Complexity: Time: O(V + E), Space: O(V + E).


    // =========================================================
    // 47. HAMILTONIAN FLIGHTS (CSES 1690)
    // =========================================================

    int countHamiltonianFlights(int n, const vector<vector<int>>& adj) {
        const int MOD = 1e9 + 7;
        vector<vector<int>> dp(1 << n, vector<int>(n, 0));
        dp[1][0] = 1;

        for (int mask = 1; mask < (1 << n); mask++) {
            if (!(mask & 1)) continue; // Must start at node 0
            if ((mask & (1 << (n - 1))) && mask != (1 << n) - 1) continue; // Do not reach end early

            for (int u = 0; u < n; u++) {
                if (!(mask & (1 << u)) || dp[mask][u] == 0) continue;
                for (int v : adj[u]) {
                    if (!(mask & (1 << v))) {
                        dp[mask | (1 << v)][v] = (dp[mask | (1 << v)][v] + dp[mask][u]) % MOD;
                    }
                }
            }
        }
        return dp[(1 << n) - 1][n - 1];
    }
    // Interview Explanation:
    // - Problem Statement: Count number of Hamiltonian paths from city 1 to city n visiting all cities exactly once (CSES 1690).
    // - Approach: Bitmask Dynamic Programming (`dp[mask][u]`).
    // - Intuition: `dp[mask][u]` stores number of paths visiting subset `mask` ending at node u; bitmask transitions run in O(2^N * N).
    // - Complexity: Time: O(2^N * N^2), Space: O(2^N * N).


    // =========================================================
    // 48. PLANETS CYCLES (CSES 1751)
    // =========================================================

    vector<int> planetsCycles(int n, const vector<int>& successor) {
        vector<int> inDegree(n + 1, 0), ans(n + 1, -1);
        vector<vector<int>> rev(n + 1);
        for (int i = 1; i <= n; i++) {
            inDegree[successor[i]]++;
            rev[successor[i]].push_back(i);
        }

        queue<int> q;
        for (int i = 1; i <= n; i++) if (inDegree[i] == 0) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (--inDegree[successor[u]] == 0) q.push(successor[u]);
        }

        vector<int> vis(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            if (inDegree[i] == 0 || vis[i]) continue;
            int len = 0, u = i;
            do { vis[u] = 1; len++; u = successor[u]; } while (u != i);
            u = i;
            do { ans[u] = len; u = successor[u]; } while (u != i);
        }

        function<void(int)> dfsTree = [&](int u) {
            for (int v : rev[u]) {
                if (ans[v] != -1) continue;
                ans[v] = ans[u] + 1;
                dfsTree(v);
            }
        };

        for (int i = 1; i <= n; i++) if (ans[i] != -1) dfsTree(i);

        vector<int> result(n);
        for (int i = 1; i <= n; i++) result[i - 1] = ans[i];
        return result;
    }
    // Interview Explanation:
    // - Problem Statement: Find path length to repeat a planet state for every starting planet in a functional graph (CSES 1751).
    // - Approach: Functional Graph Decomposition (Cycles via Kahn's + Trees via Reverse DFS).
    // - Intuition: Nodes in a cycle have path length equal to cycle size; nodes in attached trees have length equal to distance to cycle plus cycle size.
    // - Complexity: Time: O(N), Space: O(N).


    // =========================================================
    // 49. GRAPH GIRTH (CSES 1707)
    // =========================================================

    int findGraphGirth(int n, const vector<vector<int>>& adj) {
        const int INF = 1e9;
        int minGirth = INF;

        for (int i = 1; i <= n; i++) {
            queue<pair<int, int>> q;
            q.push({i, -1});
            vector<int> dist(n + 1, INF);
            dist[i] = 0;

            while (!q.empty()) {
                auto [u, p] = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (v == p) continue;
                    if (dist[v] == INF) {
                        dist[v] = dist[u] + 1;
                        q.push({v, u});
                    } else {
                        minGirth = min(minGirth, dist[u] + dist[v] + 1);
                    }
                }
            }
        }
        return minGirth == INF ? -1 : minGirth;
    }
    // Interview Explanation:
    // - Problem Statement: Find the girth (length of the shortest cycle) in an unweighted undirected graph (CSES 1707).
    // - Approach: BFS from Every Vertex (O(V * (V + E))).
    // - Intuition: Running unweighted BFS from each vertex i finds shortest cycle containing i via cross-edges `dist[u] + dist[v] + 1`.
    // - Complexity: Time: O(V(V + E)), Space: O(V + E).


    // =========================================================
    // 50. NETWORK BREAKDOWN (CSES 1676)
    // =========================================================

    vector<int> networkBreakdown(int n, const vector<pair<int, int>>& allEdges, const vector<pair<int, int>>& queries) {
        struct DSU {
            vector<int> p, sz;
            int comp;
            DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }
            int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
            bool unite(int a, int b) {
                a = find(a); b = find(b);
                if (a == b) return false;
                if (sz[a] < sz[b]) swap(a, b);
                p[b] = a; sz[a] += sz[b]; comp--; return true;
            }
        };

        set<pair<int, int>> removed;
        for (auto [u, v] : queries) {
            if (u > v) swap(u, v);
            removed.insert({u, v});
        }

        DSU dsu(n);
        for (auto [u, v] : allEdges) {
            if (u > v) swap(u, v);
            if (!removed.count({u, v})) dsu.unite(u, v);
        }

        int k = queries.size();
        vector<int> ans(k);
        for (int i = k - 1; i >= 0; i--) {
            ans[i] = dsu.comp;
            dsu.unite(queries[i].first, queries[i].second);
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Output component counts after each edge breakdown query (CSES 1676).
    // - Approach: Offline Processing via Reverse DSU Edge Re-insertion.
    // - Intuition: Reversing edge deletions transforms graph destruction into incremental DSU unions, tracking component count `comp`.
    // - Complexity: Time: O((M + K) * alpha(N)), Space: O(N + M + K).


    // =========================================================
    // 51. NETWORK RENOVATION (CSES 2081)
    // =========================================================

    vector<pair<int, int>> networkRenovation(int n, const vector<vector<int>>& adj) {
        if (n <= 1) return {};
        vector<int> leaves;
        function<void(int, int)> dfs = [&](int u, int p) {
            if (adj[u].size() == 1) leaves.push_back(u);
            for (int v : adj[u]) if (v != p) dfs(v, u);
        };
        dfs(1, -1);

        int totalLeaves = leaves.size();
        int k = (totalLeaves + 1) / 2;
        int half = totalLeaves / 2;
        vector<pair<int, int>> newEdges;
        for (int i = 0; i < k; i++) {
            newEdges.push_back({leaves[i], leaves[(i + half) % totalLeaves]});
        }
        return newEdges;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum new connections to make a tree 2-edge-connected (CSES 2081).
    // - Approach: Leaf Pairing via Half-Split Index Shift (k = ceil(L / 2)).
    // - Intuition: Gathering all tree leaves and pairing leaf i with leaf (i + floor(L/2)) % L ensures no bridge remains.
    // - Complexity: Time: O(N), Space: O(N).


    // =========================================================
    // 52. FLIGHT ROUTE REQUESTS (CSES 1699)
    // =========================================================

    int minFlightRouteRequests(int n, const vector<pair<int, int>>& directedEdges) {
        vector<vector<int>> dG(n + 1), uG(n + 1);
        for (auto [u, v] : directedEdges) {
            dG[u].push_back(v);
            uG[u].push_back(v);
            uG[v].push_back(u);
        }

        vector<int> comp(n + 1, -1), compSize;
        int compCount = 0;
        function<void(int, int)> findComp = [&](int u, int compId) {
            comp[u] = compId;
            for (int v : uG[u]) if (comp[v] == -1) findComp(v, compId);
        };

        for (int i = 1; i <= n; i++) {
            if (comp[i] == -1) {
                compSize.push_back(0);
                findComp(i, compCount++);
            }
        }
        for (int i = 1; i <= n; i++) compSize[comp[i]]++;

        vector<int> state(n + 1, 0);
        vector<bool> hasCycle(compCount, false);
        function<void(int)> detectCycle = [&](int u) {
            state[u] = 1;
            for (int v : dG[u]) {
                if (state[v] == 0) detectCycle(v);
                else if (state[v] == 1) hasCycle[comp[u]] = true;
            }
            state[u] = 2;
        };

        for (int i = 1; i <= n; i++) if (state[i] == 0) detectCycle(i);

        int ans = 0;
        for (int c = 0; c < compCount; c++) ans += hasCycle[c] ? compSize[c] : compSize[c] - 1;
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum flight routes required to satisfy all reachability requirements across n cities (CSES 1699).
    // - Approach: Weakly Connected Components + DFS Cycle Detection.
    // - Intuition: For each weakly connected component of size C: if it contains a directed cycle, it requires C edges; otherwise C-1 edges suffice.
    // - Complexity: Time: O(V + E), Space: O(V + E).


    // =========================================================
    // 53. BUS COMPANIES (CSES 3158)
    // =========================================================

    vector<long long> busCompaniesMinCost(int cities, int companies, const vector<long long>& tickets, const vector<vector<int>>& routes) {
        const long long INF = 1e18;
        struct Edge { int to; long long cost; };
        int totalNodes = cities + companies;
        vector<vector<Edge>> g(totalNodes + 1);

        for (int i = 1; i <= companies; i++) {
            int compNode = cities + i;
            for (int city : routes[i]) {
                g[city].push_back({compNode, 0});
                g[compNode].push_back({city, tickets[i]});
            }
        }

        vector<long long> dist(totalNodes + 1, INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        dist[1] = 0;
        pq.push({0, 1});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (const auto& e : g[u]) {
                if (d + e.cost < dist[e.to]) {
                    dist[e.to] = d + e.cost;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        vector<long long> result(cities);
        for (int i = 1; i <= cities; i++) result[i - 1] = dist[i];
        return result;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum travel cost from city 1 to all other cities using bus companies (CSES 3158).
    // - Approach: Virtual Company Auxiliary Nodes + Single-Source Dijkstra Algorithm.
    // - Intuition: Create virtual node for each company; entering company node costs 0 and exiting to any served city costs ticket price.
    // - Complexity: Time: O((V + E) log(V + E)), Space: O(V + E).


    // =========================================================
    // 54. MST EDGE CHECK (CSES 3407)
    // =========================================================

    vector<bool> mstEdgeCheck(int n, int m, vector<tuple<int, int, int, int>> edges) {
        // tuple: (w, u, v, id)
        sort(edges.begin(), edges.end());
        struct DSU {
            vector<int> p, sz;
            DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
            int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
            bool unite(int a, int b) {
                a = find(a); b = find(b);
                if (a == b) return false;
                if (sz[a] < sz[b]) swap(a, b);
                p[b] = a; sz[a] += sz[b]; return true;
            }
        };

        DSU dsu(n);
        vector<bool> inMST(m, false);

        for (int i = 0; i < m; ) {
            int j = i;
            while (j < m && get<0>(edges[j]) == get<0>(edges[i])) j++;
            for (int k = i; k < j; k++) {
                auto [w, u, v, id] = edges[k];
                if (dsu.find(u) != dsu.find(v)) inMST[id] = true;
            }
            for (int k = i; k < j; k++) {
                auto [w, u, v, id] = edges[k];
                dsu.unite(u, v);
            }
            i = j;
        }
        return inMST;
    }
    // Interview Explanation:
    // - Problem Statement: For each edge, determine if it can belong to AT LEAST ONE Minimum Spanning Tree (CSES 3407).
    // - Approach: Kruskal Batching by Edge Weight.
    // - Intuition: An edge of weight W can be in an MST iff its endpoints are unconnected in DSU prior to incorporating weight W batch edges.
    // - Complexity: Time: O(E log E), Space: O(V + E).


    // =========================================================
    // 55. TRANSFER SPEEDS SUM (CSES 3111)
    // =========================================================

    long long transferSpeedsSum(int n, vector<tuple<long long, int, int>> treeEdges) {
        sort(treeEdges.rbegin(), treeEdges.rend());
        struct DSU {
            vector<int> parent, sz;
            DSU(int n) : parent(n + 1), sz(n + 1, 1) { iota(parent.begin(), parent.end(), 0); }
            int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
            long long unite(int u, int v) {
                u = find(u); v = find(v);
                if (u == v) return 0;
                long long pairs = 1LL * sz[u] * sz[v];
                if (sz[u] < sz[v]) swap(u, v);
                parent[v] = u; sz[u] += sz[v];
                return pairs;
            }
        };

        DSU dsu(n);
        long long ans = 0;
        for (auto &[w, u, v] : treeEdges) {
            long long pairs = dsu.unite(u, v);
            ans += pairs * w;
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Find sum of bottleneck transfer speeds over all pairs of nodes in a tree (CSES 3111).
    // - Approach: Reverse Kruskal DSU Component Product.
    // - Intuition: Sorting tree edges in descending order of weight guarantees that joining components of size S_u and S_v via edge w contributes S_u * S_v * w to total sum.
    // - Complexity: Time: O(N log N), Space: O(N).

};
