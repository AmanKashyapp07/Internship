# CSES Advanced Graph Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 7 problems in the **Advanced Graph Problems** topic.

---

## 1. Graph Girth

- **Tier**: Tier 2
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

## 2. Network Breakdown

- **Tier**: Tier 2
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

## 3. Network Renovation

- **Tier**: Tier 3
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

## 4. Flight Route Requests

- **Tier**: Tier 3
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
- **Pseudocode**:
```text
function min_flights():
    total = 0
    for comp in weakly_connected_components():
        if has_directed_cycle(comp):
            total += size(comp)
        else:
            total += size(comp) - 1
    return total
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 5. Bus Companies

- **Tier**: Tier 3
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
- **Pseudocode**:
```text
function bus_companies():
    build_segtree_virtual_graph()
    for (u, l, r, w) in routes:
        add_range_edges(u, l, r, w)
    dist = dijkstra(start=1, virtual_graph)
    return dist[1..N]
```
- **Complexity**: Time: $\mathcal{O}((V + Q \log V) \log V)$, Space: $\mathcal{O}(V \log V + E)$.

---

## 6. MST Edge Check

- **Tier**: Tier 2
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

## 7. Transfer Speeds Sum

- **Tier**: Tier 3
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
- **Pseudocode**:
```text
function transfer_speeds_sum():
    gh_tree = build_gomory_hu(graph)
    ans = 0
    for (u, v, w) in gh_tree.edges:
        (sz_u, sz_v) = subtree_sizes_after_cut(u, v)
        ans += w * sz_u * sz_v
    return ans
```
- **Complexity**: Time: $\mathcal{O}(V \cdot \text{MaxFlow}(V, E))$, Space: $\mathcal{O}(V + E)$.

---
