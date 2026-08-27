# CSES Advanced Graph Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 18 problems in the **Advanced Graph Problems** topic.

---

## 1. Acyclic Graph Edges

- **Link**: [Acyclic Graph Edges](https://cses.fi/problemset/task/1707)
- **Problem Statement**: Given an undirected connected graph with $N$ nodes and $M$ edges, assign a direction to every edge such that the resulting directed graph contains no cycles (is a DAG).
- **Interview Pattern**: DAG Orientation via Natural Index Order.
- **Intuition**:
  - To orient undirected edges such that the resulting directed graph has no cycles (DAG), direct every edge $(u, v)$ from the smaller node index to the larger node index ($\min(u, v) \to \max(u, v)$).
  - Because edges only go from smaller to larger numbers, a back-edge is mathematically impossible!
- **Step-by-Step Interview Walkthrough**:
  1. For each input edge $(u, v)$:
  2. If $u < v$, direct $u \to v$.
  3. If $u > v$, direct $v \to u$.
  4. Print directed edges.
- **Pseudocode**:
```text
for (u, v) in edges:
    if u < v: print u, v
    else: print v, u
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 2. Strongly Connected Edges

- **Link**: [Strongly Connected Edges](https://cses.fi/problemset/task/2177)
- **Problem Statement**: Given an undirected graph with $N$ nodes and $M$ edges, assign a direction to every edge such that the resulting directed graph is strongly connected, or output `IMPOSSIBLE`.
- **Interview Pattern**: DFS Tree Edge Orientation + Bridge Check.
- **Intuition**:
  - An undirected graph can be oriented into a Strongly Connected Component (SCC) iff it has **no bridges** (necessary condition by Robbins' Theorem).
  - DFS tree edge orientation: direct tree edges downward (parent $\to$ child) and back edges upward (ancestor $\leftarrow$ descendant).
- **Step-by-Step Interview Walkthrough**:
  1. Run Tarjan's bridge-finding algorithm. If any bridge exists, print `IMPOSSIBLE`.
  2. Run DFS: for tree edge $(u, v)$, direct $u \to v$. For back edge $(u, v)$ with $v$ visited and $v \neq \text{parent}[u]$, direct $u \to v$ if $\text{tin}[v] < \text{tin}[u]$.
  3. Print oriented edges.
- **Pseudocode**:
```text
function dfs(u, p):
    tin[u] = low[u] = ++timer
    for v in adj[u]:
        if v == p: continue
        if tin[v]:
            low[u] = min(low[u], tin[v])
            if tin[v] < tin[u]: oriented_edges.push((u, v))
        else:
            oriented_edges.push((u, v))
            dfs(v, u)
            low[u] = min(low[u], low[v])
            if low[v] > tin[u]: has_bridge = true
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 3. Graph Girth

- **Link**: [Graph Girth](https://cses.fi/problemset/task/1707)
- **Problem Statement**: Given an unweighted, undirected graph with $N$ nodes and $M$ edges, find the girth of the graph (the length of the shortest cycle), or output -1 if the graph is acyclic.
- **Interview Pattern**: Multi-Source BFS for Smallest Cycle in Unweighted Graph.
- **Intuition**:
  - The girth of a graph is the length of its shortest cycle.
  - Running a BFS from every vertex $u$ finds the shortest cycle containing $u$. The overall minimum over all BFS runs gives the graph's girth.
- **Step-by-Step Interview Walkthrough**:
  1. For each node $s \in [1, N]$, initialize `dist[1..N] = INF`, `dist[s] = 0`.
  2. Run BFS from $s$.
  3. When examining edge $(u, v)$:
     - If $v$ is unvisited (`dist[v] == INF`), set `dist[v] = dist[u] + 1`, `parent[v] = u`, enqueue $v$.
     - If $v$ is visited and $v \neq \text{parent}[u]$, cycle found of length $\text{dist}[u] + \text{dist}[v] + 1$. Update global minimum girth.
  4. Return global minimum girth (or -1 if no cycle exists).
- **Pseudocode**:
```text
min_girth = INF
for s = 1 to N:
    dist[1..N] = INF, dist[s] = 0
    queue.push(s)
    while queue not empty:
        u = queue.pop()
        for v in adj[u]:
            if dist[v] == INF:
                dist[v] = dist[u] + 1, parent[v] = u, queue.push(v)
            else if v != parent[u]:
                min_girth = min(min_girth, dist[u] + dist[v] + 1)
```
- **Complexity**: Time: $\mathcal{O}(V \cdot (V + E))$, Space: $\mathcal{O}(V + E)$.

---

## 4. Course Schedule II

- **Link**: [Course Schedule II](https://cses.fi/problemset/task/1757)
- **Problem Statement**: Given $N$ courses and $M$ prerequisites, find the **lexicographically smallest** valid topological ordering of courses.
- **Interview Pattern**: Reverse Topological Sort via Max-Priority Queue.
- **Intuition**:
  - We want to find the **lexicographically smallest** topological ordering.
  - Greedy choice: To make smaller numbers appear as early as possible in the forward order, we should place the largest possible available numbers as late as possible!
  - Reverse the edges and run Kahn's algorithm with a **Max-Heap**.
- **Step-by-Step Interview Walkthrough**:
  1. Reverse all directed edges $u \to v$ into $v \to u$.
  2. Compute out-degrees for reversed graph.
  3. Push all nodes with `outdegree == 0` into a Max-Priority Queue.
  4. Pop largest available node $u$, add to result list. Decrement out-degrees of in-neighbors $v$; if 0, push $v$ into max-heap.
  5. Reverse the resulting list to get the lexicographically smallest topological order.
- **Pseudocode**:
```text
max_pq.push(nodes with outdegree == 0)
while max_pq not empty:
    u = max_pq.pop_max()
    result.push(u)
    for v in in_edges[u]:
        outdegree[v] -= 1
        if outdegree[v] == 0: max_pq.push(v)

reverse(result)
return result
```
- **Complexity**: Time: $\mathcal{O}((V + E) \log V)$, Space: $\mathcal{O}(V + E)$.

---

## 5. Network Breakdown

- **Link**: [Network Breakdown](https://cses.fi/problemset/task/1676)
- **Problem Statement**: Starting with a network of $N$ nodes and $M$ edges, process $K$ edge removals sequentially and output the number of connected components after each removal.
- **Interview Pattern**: Offline Dynamic Connectivity via Reverse Edge Additions with DSU.
- **Intuition**:
  - Removing edges one by one is hard. Reversing the process—starting with the final graph and **adding** edges backwards—allows using Disjoint Set Union (DSU) in $\mathcal{O}(1)$ time!
- **Step-by-Step Interview Walkthrough**:
  1. Mark all $K$ edges that will be broken.
  2. Build initial DSU with remaining non-broken edges. Track component count.
  3. Process the $K$ broken edges in **reverse order** (from last broken to first):
     - Record current component count as answer for query $i$.
     - Add edge back into DSU; if it unites two components, decrement component count.
  4. Reverse the recorded answers and print.
- **Pseudocode**:
```text
build DSU with remaining non-removed edges
comp_count = current_number_of_components

for i = K-1 down to 0:
    ans[i] = comp_count
    (u, v) = removed_edges[i]
    if dsu.unite(u, v):
        comp_count -= 1

print ans[0..K-1]
```
- **Complexity**: Time: $\mathcal{O}((M + K) \cdot \alpha(N))$, Space: $\mathcal{O}(N + M + K)$.

---

## 6. Visiting Cities

- **Link**: [Visiting Cities](https://cses.fi/problemset/task/1202)
- **Problem Statement**: Given a directed graph with $N$ cities and weighted flight routes, find all cities that are visited on **every** minimum-cost path from city 1 to city $N$.
- **Interview Pattern**: Shortest Path DAG + Dominator Tree / Articulation Points on Shortest Path.
- **Intuition**:
  - A city $u$ is visited in *every* shortest path from 1 to $N$ iff $u$ lies on all shortest paths (i.e. deleting $u$ disconnects $N$ from 1 in the Shortest Path DAG).
  - Shortest Path DAG has edges $(u, v)$ where $\text{dist}[1][u] + w + \text{dist}[v][N] = \text{dist}[1][N]$.
- **Step-by-Step Interview Walkthrough**:
  1. Run Dijkstra from 1 to get `dist1[u]` and from $N$ to get `distN[u]`.
  2. Build Shortest Path DAG with edges satisfying $\text{dist1}[u] + w + \text{distN}[v] = \text{dist1}[N]$.
  3. Compute number of shortest paths reaching each node from 1 (`ways1[u]`) and reaching $N$ from each node (`waysN[u]`).
  4. Node $u$ lies on all shortest paths iff $\text{ways1}[u] \times \text{waysN}[u] \equiv \text{ways1}[N] \pmod{\text{MOD}}$.
- **Pseudocode**:
```text
dist1 = dijkstra(start=1)
distN = dijkstra(start=N on reversed graph)

min_len = dist1[N]
for u in nodes:
    if ways1[u] * waysN[u] % MOD == total_ways % MOD:
        mark u as mandatory
```
- **Complexity**: Time: $\mathcal{O}((V + E) \log V)$, Space: $\mathcal{O}(V + E)$.

---

## 7. Nearest Shops

- **Link**: [Nearest Shops](https://cses.fi/problemset/task/2080)
- **Problem Statement**: Given an unweighted graph where a subset of nodes are shops, calculate for every node $u$ the distance to the nearest shop.
- **Interview Pattern**: Multi-Source BFS.
- **Intuition**:
  - Distance from each node to the nearest shop is computed simultaneously by initializing BFS queue with all shop locations at distance 0.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `dist[1..N] = INF`.
  2. Enqueue all shop nodes $S$ with `dist[s] = 0`.
  3. Run BFS. When visiting unvisited neighbor $v$ from $u$, set `dist[v] = dist[u] + 1` and enqueue $v$.
  4. Output `dist` array.
- **Pseudocode**:
```text
dist[1..N] = INF
for s in shops:
    dist[s] = 0
    queue.push(s)

while queue not empty:
    u = queue.pop()
    for v in adj[u]:
        if dist[v] == INF:
            dist[v] = dist[u] + 1
            queue.push(v)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 8. Network Renovation

- **Link**: [Network Renovation](https://cses.fi/problemset/task/2081)
- **Problem Statement**: Given a tree of $N$ nodes, find the minimum number of new edges to add (and which ones) to make the graph 2-edge-connected (no bridges).
- **Interview Pattern**: Tree Leaf Pairing via Centroid Balancing.
- **Intuition**:
  - To make a tree 2-edge-connected with minimum new edges, we must pair up leaf nodes.
  - Minimum edges required is $\lceil L / 2 \rceil$ where $L$ is the number of leaves.
  - Rooting the tree at its centroid guarantees that pairing leaf $i$ with leaf $i + \lfloor L/2 \rfloor$ in DFS leaf order never pairs leaves in the same subtree!
- **Step-by-Step Interview Walkthrough**:
  1. Find a centroid $C$ of the tree.
  2. Root the tree at centroid $C$.
  3. Collect all leaf nodes in DFS order into array `leaves`.
  4. Pair leaf `leaves[i]` with `leaves[i + K]` where $K = \lfloor L / 2 \rfloor$.
- **Pseudocode**:
```text
C = find_centroid(1)
leaves = collect_leaves_dfs(C)
L = leaves.size(), K = L / 2

print ceil(L / 2.0)
for i = 0 to K - 1:
    print leaves[i], leaves[i + K]
if L is odd:
    print leaves[0], leaves[L - 1]
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 9. Flight Route Requests

- **Link**: [Flight Route Requests](https://cses.fi/problemset/task/1684)
- **Problem Statement**: Given a directed graph of flight requests where each component must support reachability, find the minimum number of total flights to build.
- **Interview Pattern**: Weakly Connected Component Functional Graph Analysis.
- **Intuition**:
  - In a directed graph where each component requires reachability, if a weakly connected component contains a directed cycle, $C$ edges are needed (where $C$ is component size). If it is a DAG (tree-like), $C - 1$ edges are needed.
- **Step-by-Step Interview Walkthrough**:
  1. Find weakly connected components using DSU/DFS on undirected version.
  2. For each component: check if it contains any directed cycle (via 3-color DFS).
  3. If component contains a cycle, add component size $C$ to total.
  4. If component is acyclic, add $C - 1$ to total.
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 10. Bus Companies

- **Link**: [Bus Companies](https://cses.fi/problemset/task/2082)
- **Problem Statement**: Given $N$ cities and bus routes that connect node $u$ to a range of cities $[l, r]$ with weight $w$, calculate shortest distances from start city to all cities.
- **Interview Pattern**: Virtual Graph Node Segment Tree / Multi-Layer Dijkstra.
- **Intuition**:
  - Connecting range $[l, r]$ to node $u$ with single edge weight creates up to $\mathcal{O}(N)$ edges naively.
  - Segment Tree virtual nodes reduce range-to-node edge count to $\mathcal{O}(\log N)$ per query.
- **Step-by-Step Interview Walkthrough**:
  1. Build a Segment Tree over graph nodes.
  2. Create zero-weight directed edges from parent segtree node to child segtree nodes.
  3. Add directed edges from query node $u$ to segtree interval nodes covering range $[l, r]$ with weight $w$.
  4. Run Dijkstra from start node on virtual graph.
- **Complexity**: Time: $\mathcal{O}((V + Q \log V) \log V)$, Space: $\mathcal{O}(V \log V + E)$.

---

## 11. Tree Isomorphism I

- **Link**: [Tree Isomorphism I](https://cses.fi/problemset/task/1702)
- **Problem Statement**: Given two rooted trees with $N$ nodes each, determine if they are isomorphic (structurally identical).
- **Interview Pattern**: Rooted Tree Canonical Hashing.
- **Intuition**:
  - Two rooted trees are isomorphic iff their canonical subtree representations match.
  - Recursively hash subtrees by sorting child subtree tuples/strings and mapping unique tuples to integer IDs.
- **Step-by-Step Interview Walkthrough**:
  1. Define a global map `tuple_to_id` to assign unique integer IDs to child tuple vectors.
  2. Post-order DFS: for node $u$, collect child subtree IDs, sort them into a vector `child_ids`.
  3. Map `child_ids` to a unique integer ID for node $u$.
  4. Trees are isomorphic iff `get_hash(root1) == get_hash(root2)`.
- **Pseudocode**:
```text
function get_tree_hash(u, p):
    child_hashes = []
    for v in adj[u]:
        if v != p:
            child_hashes.push(get_tree_hash(v, u))
    sort(child_hashes)
    return assign_unique_id(child_hashes)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 12. Tree Isomorphism II

- **Link**: [Tree Isomorphism II](https://cses.fi/problemset/task/1703)
- **Problem Statement**: Given two unrooted trees with $N$ nodes each, determine if they are isomorphic.
- **Interview Pattern**: Unrooted Tree Isomorphism via Centroid Canonical Hashing.
- **Intuition**:
  - Unrooted trees are isomorphic iff their tree hashes match when rooted at their centroids.
  - A tree has either 1 or 2 centroids.
- **Step-by-Step Interview Walkthrough**:
  1. Find centroids of Tree 1 ($C_{1A}, C_{1B}$) and Tree 2 ($C_{2A}, C_{2B}$).
  2. Compute canonical tree hash for Tree 1 rooted at $C_{1A}$.
  3. Compute canonical tree hash for Tree 2 rooted at $C_{2A}$ (and $C_{2B}$ if present).
  4. If hash matches any centroid pairing, output `YES`, else `NO`.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 13. MST Edge Check

- **Link**: [MST Edge Check](https://cses.fi/problemset/task/1704)
- **Problem Statement**: Given a connected weighted graph with $M$ edges, for each edge determine if it can belong to **at least one** Minimum Spanning Tree of the graph.
- **Interview Pattern**: Kruskal's Same-Weight Batch Processing / Heavy-Light Decomposition.
- **Intuition**:
  - An edge $(u, v, w)$ can belong to *some* Minimum Spanning Tree iff no path between $u$ and $v$ exists using only edges of weight strictly $< w$.
- **Step-by-Step Interview Walkthrough**:
  1. Group all edges by weight $w$ ascending.
  2. For a batch of edges with equal weight $w$:
     - For each edge $(u, v, w)$ in batch: if `dsu.find(u) != dsu.find(v)`, this edge can be part of an MST (`YES`).
     - After testing all edges in batch, unite components in DSU.
- **Pseudocode**:
```text
for batch of edges with same weight w:
    for (u, v, id) in batch:
        if dsu.find(u) != dsu.find(v):
            ans[id] = "YES"
        else: ans[id] = "NO"
        
    for (u, v, id) in batch:
        dsu.unite(u, v)
```
- **Complexity**: Time: $\mathcal{O}(E \log E)$, Space: $\mathcal{O}(V + E)$.

---

## 14. MST Edge Cost

- **Link**: [MST Edge Cost](https://cses.fi/problemset/task/1705)
- **Problem Statement**: Given a connected weighted graph, calculate the cost of a Minimum Spanning Tree if a specific edge $(u, v, w)$ **must** be included in the MST.
- **Interview Pattern**: Tree Max-Edge Binary Lifting / LCA.
- **Intuition**:
  - Adding non-MST edge $(u, v, w)$ creates a cycle. The cost of MST including this edge is $\text{MST\_weight} + w - \max\_edge(u, v)$ on the MST tree.
- **Step-by-Step Interview Walkthrough**:
  1. Build MST using Kruskal's algorithm.
  2. Build Binary Lifting table `max_edge[u][j]` storing maximum edge weight on $2^j$ path ancestor from $u$.
  3. For non-MST query edge $(u, v, w)$: query max edge on MST path between $u$ and $v$ in $\mathcal{O}(\log N)$ time.
  4. Answer = $\text{MST\_weight} + w - \text{max\_edge\_on\_path}(u, v)$.
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 15. Graph Coloring

- **Link**: [Graph Coloring](https://cses.fi/problemset/task/1706)
- **Problem Statement**: Given an undirected graph, find the chromatic number of the graph (minimum number of colors required to color nodes such that no adjacent nodes share the same color).
- **Interview Pattern**: Backtracking with Pruning / Bitset Chromatic Number.
- **Intuition**:
  - Find minimum colors to color graph such that no adjacent nodes share a color.
- **Step-by-Step Interview Walkthrough**:
  1. Test $K = 1, 2, 3, \dots$ colors sequentially.
  2. For color count $K$, run backtracking DFS assigning color $c \in [1, K]$ to node $u$ checking adjacency validity.
  3. Output minimum $K$.
- **Complexity**: Exponential $\mathcal{O}(K^V)$, optimized with degree-ordering heuristics.

---

## 16. Tree Traversals

- **Link**: [Tree Traversals](https://cses.fi/problemset/task/1701)
- **Problem Statement**: Given the Preorder and Inorder traversals of a binary tree, construct the binary tree and output its Postorder traversal.
- **Interview Pattern**: Binary Tree Reconstruction from Preorder & Inorder.
- **Intuition**:
  - Preorder first element is root. Find root in Inorder to split into left and right subtrees recursively.
- **Step-by-Step Interview Walkthrough**:
  1. Preorder root is `pre[pre_start]`.
  2. Find root index `idx` in Inorder.
  3. Left subtree size = `idx - in_start`.
  4. Recurse left subtree and right subtree, output Postorder traversal.
- **Pseudocode**:
```text
function build_postorder(pre_l, pre_r, in_l, in_r):
    if pre_l > pre_r: return
    root_val = pre[pre_l]
    idx = in_pos[root_val]
    left_sz = idx - in_l
    
    build_postorder(pre_l + 1, pre_l + left_sz, in_l, idx - 1)
    build_postorder(pre_l + left_sz + 1, pre_r, idx + 1, in_r)
    print root_val
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 17. Transfer Speeds Sum

- **Link**: [Transfer Speeds Sum](https://cses.fi/problemset/task/1708)
- **Problem Statement**: Given a weighted undirected graph, calculate the sum of maximum flow values over all pairs of nodes $(u, v)$.
- **Interview Pattern**: Gomory-Hu Tree / All-Pairs Max Flow.
- **Intuition**:
  - Sum of max flows between all pairs in a weighted undirected graph equals sum over Gomory-Hu tree edge cuts $\times$ component sizes.
- **Step-by-Step Interview Walkthrough**:
  1. Build Gomory-Hu tree using $N - 1$ max-flow cuts.
  2. For each edge $e = (u, v, w)$ in Gomory-Hu tree:
     - Deleting $e$ splits tree into component sizes $S_1$ and $S_2$.
     - Edge contribution = $w \times S_1 \times S_2$.
  3. Sum contributions over all $N - 1$ tree edges.
- **Complexity**: Time: $\mathcal{O}(V \cdot \text{MaxFlow}(V, E))$, Space: $\mathcal{O}(V + E)$.

---

## 18. Tree Coin Collecting I

- **Link**: [Tree Coin Collecting I](https://cses.fi/problemset/task/1709)
- **Problem Statement**: Given a tree where nodes contain coins and each node has a distance collection limit, find the maximum total coins you can collect.
- **Interview Pattern**: Tree DP with Distance Boundaries / Greedy Matching.
- **Intuition**:
  - Maximize coins collected under subtree distance limits using bottom-up tree DP.
- **Step-by-Step Interview Walkthrough**:
  1. Post-order DFS.
  2. Maintain DP states at node $u$: `dp[u][d]` = max coins collected within depth $d$.
  3. Combine child DP tables bottom-up.
- **Complexity**: Time: $\mathcal{O}(N \cdot K)$, Space: $\mathcal{O}(N \cdot K)$.
