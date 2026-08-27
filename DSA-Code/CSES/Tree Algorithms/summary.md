# CSES Tree Algorithms - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 13 problems in the **Tree Algorithms** topic.

---

## 1. Subordinates

- **Link**: [Subordinates](https://cses.fi/problemset/task/1674)
- **Problem Statement**: Given a tree of $N$ nodes rooted at node 1 where each node represents an employee and edges represent manager-employee relationships, calculate the total number of subordinates for each employee.
- **Interview Pattern**: Post-Order Bottom-Up Tree DFS / Subtree Size Calculation.
- **Intuition**:
  - The number of subordinates of a manager node $u$ is simply the total count of nodes in node $u$'s subtree excluding $u$ itself.
  - Subtree size follows a natural post-order recurrence: a node cannot compute its own subtree size until all its children have finished computing theirs.
- **Step-by-Step Interview Walkthrough**:
  1. Root the tree arbitrarily at node 1.
  2. Perform a DFS traversal. For any node $u$, initialize its subtree size `sz[u] = 1` (counting itself).
  3. Recursively visit every child $v$. Upon return, accumulate `sz[u] += sz[v]`.
  4. The answer for node $u$ is `sz[u] - 1`.
- **Pseudocode**:
```text
function dfs(u, parent):
    sz[u] = 1
    for v in adj[u]:
        if v != parent:
            dfs(v, u)
            sz[u] += sz[v]
    ans[u] = sz[u] - 1
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$ (recursion stack + tree adjacency list).

---

## 2. Tree Matching

- **Link**: [Tree Matching](https://cses.fi/problemset/task/1130)
- **Problem Statement**: Given a tree with $N$ nodes, find the maximum number of edges you can select such that no two selected edges share a common endpoint (maximum matching in a tree).
- **Interview Pattern**: Greedy Bottom-Up Matching / Tree Dynamic Programming.
- **Intuition**:
  - We want to pick the maximum number of edges such that no two edges share an endpoint.
  - Greedy choice property: In a tree, it is always optimal to match leaf nodes with their immediate parents as early as possible. If a leaf is not matched to its parent, it can never be matched to anything else.
- **Step-by-Step Interview Walkthrough**:
  1. Traverse the tree bottom-up using Post-Order DFS.
  2. Process all children $v$ of node $u$ first.
  3. When returning to node $u$, check if neither $u$ nor child $v$ has been matched yet.
  4. If both are free, greedily pair $(u, v)$, mark both as `matched = true`, and increment the total matching count.
- **Pseudocode**:
```text
function dfs(u, parent):
    for v in adj[u]:
        if v != parent:
            dfs(v, u)
            if not matched[u] and not matched[v]:
                matched[u] = true
                matched[v] = true
                max_matching += 1
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 3. Tree Diameter

- **Link**: [Tree Diameter](https://cses.fi/problemset/task/1131)
- **Problem Statement**: Given a tree with $N$ nodes, compute the diameter of the tree, which is defined as the maximum distance (number of edges) between any two nodes.
- **Interview Pattern**: Double BFS / Double DFS Theorem for Unweighted Trees.
- **Intuition**:
  - The diameter is the longest path between any two nodes in a tree.
  - **Fundamental Property**: Starting from *any* arbitrary node $X$, the furthest node $A$ reached will always be one of the two endpoints of the tree's diameter. Running a second search from $A$ finds the other endpoint $B$.
- **Step-by-Step Interview Walkthrough**:
  1. Pick node 1 and run BFS/DFS to find the node $A$ at maximum distance from 1.
  2. Node $A$ is guaranteed to be one endpoint of the diameter.
  3. Run a second BFS/DFS starting from node $A$.
  4. The maximum distance found during this second search (to some node $B$) is the diameter of the tree.
- **Pseudocode**:
```text
function bfs(start_node):
    dist[1..N] = -1
    dist[start_node] = 0
    farthest_node = start_node
    queue.push(start_node)
    
    while queue is not empty:
        u = queue.pop()
        if dist[u] > dist[farthest_node]:
            farthest_node = u
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                queue.push(v)
                
    return (farthest_node, dist[farthest_node])

(nodeA, _) = bfs(1)
(nodeB, diameter) = bfs(nodeA)
```
- **Complexity**: Time: $\mathcal{O}(N)$ (two linear tree scans), Space: $\mathcal{O}(N)$.

---

## 4. Tree Distances I

- **Link**: [Tree Distances I](https://cses.fi/problemset/task/1132)
- **Problem Statement**: For each node $u$ in a tree of $N$ nodes, find the maximum distance from node $u$ to any other node in the tree.
- **Interview Pattern**: Extremal Distance Property of Tree Diameter Endpoints.
- **Intuition**:
  - For *every* node $u$ in a tree, the furthest node from $u$ is always one of the two endpoints of the tree's diameter ($D_1$ or $D_2$).
  - Thus, to find the maximum distance from every node $u$, we only need to compute distances from $D_1$ and $D_2$ to all other nodes.
- **Step-by-Step Interview Walkthrough**:
  1. Find the two diameter endpoints $D_1$ and $D_2$ using the 2-BFS method.
  2. Compute `dist1[u]` = distance from $D_1$ to node $u$ for all $u$ via BFS from $D_1$.
  3. Compute `dist2[u]` = distance from $D_2$ to node $u$ for all $u$ via BFS from $D_2$.
  4. For each node $u$, answer is $\max(\text{dist1}[u], \text{dist2}[u])$.
- **Pseudocode**:
```text
(D1, _) = bfs(1)
(D2, _) = bfs(D1)

dist1 = get_all_distances_bfs(D1)
dist2 = get_all_distances_bfs(D2)

for u = 1 to N:
    ans[u] = max(dist1[u], dist2[u])
```
- **Complexity**: Time: $\mathcal{O}(N)$ (3 BFS passes), Space: $\mathcal{O}(N)$.

---

## 5. Tree Distances II

- **Link**: [Tree Distances II](https://cses.fi/problemset/task/1133)
- **Problem Statement**: For each node $u$ in a tree of $N$ nodes, calculate the sum of distances from node $u$ to all other nodes in the tree.
- **Interview Pattern**: Tree Re-rooting Dynamic Programming (In-Out DP).
- **Intuition**:
  - If we move the root from parent node $u$ to child node $v$:
    - All nodes in $v$'s subtree move 1 unit *closer* to the new root (saving $1 \times \text{sz}[v]$ distance).
    - All remaining $N - \text{sz}[v]$ nodes move 1 unit *further* away (adding $1 \times (N - \text{sz}[v])$ distance).
  - Transition formula: $\text{ans}[v] = \text{ans}[u] - \text{sz}[v] + (N - \text{sz}[v])$.
- **Step-by-Step Interview Walkthrough**:
  1. **Phase 1 (Bottom-Up DFS)**: Root tree at node 1. Compute subtree sizes `sz[u]` for all $u$, and compute `ans[1]` (sum of distances from root 1 to all nodes).
  2. **Phase 2 (Top-Down DFS)**: Propagate answers to children. For child $v$ of $u$, calculate `ans[v] = ans[u] - sz[v] + (N - sz[v])` and recurse down.
- **Pseudocode**:
```text
function dfs_bottom_up(u, parent, depth):
    sz[u] = 1
    ans[1] += depth
    for v in adj[u]:
        if v != parent:
            dfs_bottom_up(v, u, depth + 1)
            sz[u] += sz[v]

function dfs_top_down(u, parent):
    for v in adj[u]:
        if v != parent:
            ans[v] = ans[u] - sz[v] + (N - sz[v])
            dfs_top_down(v, u)

dfs_bottom_up(1, 0, 0)
dfs_top_down(1, 0)
```
- **Complexity**: Time: $\mathcal{O}(N)$ (two DFS passes), Space: $\mathcal{O}(N)$.

---

## 6. Company Queries I

- **Link**: [Company Queries I](https://cses.fi/problemset/task/1750)
- **Problem Statement**: Given a tree rooted at node 1 representing a corporate structure, process $Q$ queries asking for the $k$-th ancestor of a given employee node $u$.
- **Interview Pattern**: Binary Lifting / Jump Pointers.
- **Intuition**:
  - Finding the $k$-th ancestor sequentially takes $\mathcal{O}(k)$ time, which is too slow for $Q$ queries.
  - Any integer $k$ can be uniquely represented as a sum of powers of 2 (binary representation).
  - Precomputing `up[u][j]` = $2^j$-th ancestor of node $u$ allows jumping powers of 2 in $\mathcal{O}(\log k)$ time per query.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute `up[u][0] = parent[u]` for all nodes.
  2. Fill DP table: `up[u][j] = up[ up[u][j-1] ][j-1]` (the $2^j$-th ancestor is the $2^{j-1}$-th ancestor of the $2^{j-1}$-th ancestor).
  3. To query $k$-th ancestor of $u$: iterate $j$ from $\log_2 N$ down to 0. If bit $j$ of $k$ is set ($k \ \& \ 2^j$), jump to $u = \text{up}[u][j]$. If $u$ becomes null/0, return -1.
- **Pseudocode**:
```text
// Precomputation
for u = 1 to N: up[u][0] = parent[u]
for j = 1 to LOG-1:
    for u = 1 to N:
        up[u][j] = up[ up[u][j-1] ][j-1]

// Query
function get_kth_ancestor(u, k):
    for j = LOG-1 down to 0:
        if k & (1 << j):
            u = up[u][j]
            if u == 0: return -1
    return u
```
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query: $\mathcal{O}(\log K)$, Space: $\mathcal{O}(N \log N)$.

---

## 7. Company Queries II

- **Link**: [Company Queries II](https://cses.fi/problemset/task/1688)
- **Problem Statement**: Given a tree rooted at node 1, process $Q$ queries asking for the Lowest Common Ancestor (LCA) of two given nodes $u$ and $v$.
- **Interview Pattern**: Lowest Common Ancestor (LCA) using Binary Lifting.
- **Intuition**:
  - Two nodes $u$ and $v$ can find their lowest common ancestor by first aligning their depths, and then jumping upwards together as far as possible without overshooting the LCA.
- **Step-by-Step Interview Walkthrough**:
  1. Compute depths and binary lifting array `up[u][j]` via DFS.
  2. If $\text{depth}[u] < \text{depth}[v]$, swap $u$ and $v$.
  3. Lift $u$ upwards by the difference in depth $\text{depth}[u] - \text{depth}[v]$ so both are at equal depth.
  4. If $u == v$, return $u$ (one was ancestor of other).
  5. Iterate $j$ from $\log_2 N$ down to 0: if `up[u][j] != up[v][j]`, jump both $u = \text{up}[u][j]$ and $v = \text{up}[v][j]$.
  6. Return `up[u][0]` (the immediate parent of the position where they matched).
- **Pseudocode**:
```text
function lca(u, v):
    if depth[u] < depth[v]: swap(u, v)
    
    // Step 1: Equalize depths
    for j = LOG-1 down to 0:
        if depth[u] - (1 << j) >= depth[v]:
            u = up[u][j]
            
    if u == v: return u
    
    // Step 2: Lift together
    for j = LOG-1 down to 0:
        if up[u][j] != up[v][j]:
            u = up[u][j]
            v = up[v][j]
            
    return up[u][0]
```
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 8. Distance Queries

- **Link**: [Distance Queries](https://cses.fi/problemset/task/1135)
- **Problem Statement**: Given a tree with $N$ nodes, process $Q$ queries asking for the shortest distance (number of edges) between node $u$ and node $v$.
- **Interview Pattern**: Tree Path Length via LCA Decomposition.
- **Intuition**:
  - The unique simple path between node $u$ and node $v$ goes up from $u$ to $\text{LCA}(u, v)$ and then down to $v$.
  - Distance formula: $\text{dist}(u, v) = \text{depth}[u] + \text{depth}[v] - 2 \times \text{depth}[\text{LCA}(u, v)]$.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute depths and binary lifting `up[u][j]` array.
  2. For each query $(u, v)$, compute $L = \text{LCA}(u, v)$.
  3. Output $\text{depth}[u] + \text{depth}[v] - 2 \cdot \text{depth}[L]$.
- **Pseudocode**:
```text
function get_distance(u, v):
    L = lca(u, v)
    return depth[u] + depth[v] - 2 * depth[L]
```
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 9. Counting Paths

- **Link**: [Counting Paths](https://cses.fi/problemset/task/1136)
- **Problem Statement**: Given a tree with $N$ nodes and $M$ paths, count for every node $u$ how many of the given $M$ paths contain node $u$.
- **Interview Pattern**: Tree Difference Array / Path Prefix Sums.
- **Intuition**:
  - Similar to 1D difference array `diff[l] += 1`, `diff[r+1] -= 1` for range updates, we can update tree paths in $\mathcal{O}(1)$ time.
  - For path $(u, v)$ with $L = \text{LCA}(u, v)$, increment counts at $u$ and $v$, and decrement at $L$ and $\text{parent}[L]$.
  - When summing bottom-up, node $L$ receives $+1$ from $u$'s branch and $+1$ from $v$'s branch. Subtracting $1$ at $L$ leaves net $+1$ at $L$. Subtracting $1$ at $\text{parent}[L]$ cancels out the path above $L$.
- **Step-by-Step Interview Walkthrough**:
  1. For each path query $(u, v)$:
     - Compute $L = \text{LCA}(u, v)$.
     - `val[u] += 1`, `val[v] += 1`.
     - `val[L] -= 1`.
     - If $\text{parent}[L]$ exists, `val[parent[L]] -= 1`.
  2. Perform post-order DFS: for node $u$, sum up all `val[v]` from its children $v$.
  3. The final `val[u]` is the number of paths passing through node $u$.
- **Pseudocode**:
```text
for each path (u, v):
    L = lca(u, v)
    val[u] += 1
    val[v] += 1
    val[L] -= 1
    if parent[L] != 0:
        val[parent[L]] -= 1

function dfs_accumulate(u, p):
    for v in adj[u]:
        if v != p:
            dfs_accumulate(v, u)
            val[u] += val[v]
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 10. Subtree Queries

- **Link**: [Subtree Queries](https://cses.fi/problemset/task/1137)
- **Problem Statement**: Given a rooted tree where each node has a value, process $Q$ queries: (1) update a node's value, (2) calculate the sum of values in a node's subtree.
- **Interview Pattern**: Euler Tour Technique (Tree Flattening) + Fenwick Tree.
- **Intuition**:
  - A subtree rooted at $u$ is non-contiguous in standard graph representations.
  - Euler tour (DFS entry/exit timestamps) maps any node $u$'s subtree to a **single contiguous subsegment** `[tin[u], tout[u]]` in a 1D array.
  - Updating a node = point update on BIT at `tin[u]`. Subtree sum = range sum on BIT over `[tin[u], tout[u]]`.
- **Step-by-Step Interview Walkthrough**:
  1. Run DFS recording `tin[u]` when entering node $u$ and `tout[u]` when leaving node $u$.
  2. Build a Fenwick Tree (BIT) of size $N$ using initial values placed at `tin[u]`.
  3. Point Update: change value at `tin[u]` in BIT by `new_val - old_val`.
  4. Subtree Query: query BIT range sum from `tin[u]` to `tout[u]`.
- **Pseudocode**:
```text
timer = 0
function dfs_euler(u, parent):
    tin[u] = ++timer
    for v in adj[u]:
        if v != parent:
            dfs_euler(v, u)
    tout[u] = timer

// Operations:
// Update node u: BIT.add(tin[u], val - curr_val[u])
// Query subtree u: BIT.range_query(tin[u], tout[u])
```
- **Complexity**: Time: $\mathcal{O}(N + Q \log N)$, Space: $\mathcal{O}(N)$.

---

## 11. Path Queries

- **Link**: [Path Queries](https://cses.fi/problemset/task/1138)
- **Problem Statement**: Given a tree rooted at node 1 where each node has a value, process $Q$ queries: (1) update a node's value, (2) calculate the sum of values on the path from root to node $u$.
- **Interview Pattern**: Euler Tour + Fenwick Difference Array for Path Sums.
- **Intuition**:
  - Updating node $u$'s value by $+d$ affects the root-to-$x$ path sum for **all** nodes $x$ inside $u$'s subtree.
  - In Euler tour order, $u$'s subtree corresponds to index range `[tin[u], tout[u]]`.
  - Adding $+d$ at `tin[u]` and $-d$ at `tout[u] + 1` in a range-update BIT means the prefix sum at `tin[x]` will automatically include $+d$ if $x$ is in $u$'s subtree!
- **Step-by-Step Interview Walkthrough**:
  1. Flatten tree using Euler Tour DFS (`tin[u]`, `tout[u]`).
  2. Maintain a Fenwick Tree (BIT).
  3. Update node $u$ by value diff $d$: `BIT.add(tin[u], +d)`, `BIT.add(tout[u] + 1, -d)`.
  4. Path Sum from root to $u$: query BIT prefix sum at `tin[u]`.
- **Pseudocode**:
```text
function update_node(u, val):
    diff = val - curr_val[u]
    curr_val[u] = val
    BIT.add(tin[u], +diff)
    BIT.add(tout[u] + 1, -diff)

function query_root_to_u_path(u):
    return BIT.prefix_sum(tin[u])
```
- **Complexity**: Time: $\mathcal{O}(N + Q \log N)$, Space: $\mathcal{O}(N)$.

---

## 12. Distinct Colors

- **Link**: [Distinct Colors](https://cses.fi/problemset/task/1139)
- **Problem Statement**: Given a rooted tree where each node has a color, calculate the number of distinct colors in the subtree of every node $u$.
- **Interview Pattern**: Small-to-Large Set Merging (Sack / DSU on Tree).
- **Intuition**:
  - To count distinct colors in a subtree, each node needs the set union of all color sets of its children.
  - Merging arbitrary sets of total size $N$ naively takes $\mathcal{O}(N^2)$ time.
  - **Small-to-Large Trick**: Always merge the smaller set into the larger set (or `std::swap` pointers). Each element is re-inserted at most $\mathcal{O}(\log N)$ times, reducing overall complexity to $\mathcal{O}(N \log^2 N)$.
- **Step-by-Step Interview Walkthrough**:
  1. Post-order DFS traversal. Each node $u$ starts with `color_set[u] = {color[u]}`.
  2. For each child $v$, recursively call DFS to compute `color_set[v]`.
  3. Compare sizes: if `color_set[v].size() > color_set[u].size()`, `swap(color_set[u], color_set[v])`.
  4. Insert all elements from `color_set[v]` into `color_set[u]` and clear `color_set[v]`.
  5. Record answer `ans[u] = color_set[u].size()`.
- **Pseudocode**:
```text
function dfs(u, parent):
    color_set[u].insert(color[u])
    for v in adj[u]:
        if v != parent:
            dfs(v, u)
            if color_set[v].size() > color_set[u].size():
                swap(color_set[u], color_set[v])
            for c in color_set[v]:
                color_set[u].insert(c)
    ans[u] = color_set[u].size()
```
- **Complexity**: Time: $\mathcal{O}(N \log^2 N)$, Space: $\mathcal{O}(N \log N)$.

---

## 13. Finding a Centroid

- **Link**: [Finding a Centroid](https://cses.fi/problemset/task/2080)
- **Problem Statement**: Given a tree of $N$ nodes, find a centroid of the tree (a node whose removal leaves no connected component with size greater than $\lfloor N/2 \rfloor$).
- **Interview Pattern**: Centroid Traversal / Tree Balance Property.
- **Intuition**:
  - A **Centroid** of a tree is a node whose removal splits the tree into components, each having size $\le \lfloor N/2 \rfloor$.
  - Every tree has at least one centroid.
  - If node $u$ has a child $v$ with subtree size $\text{sz}[v] > N/2$, then $u$ cannot be a centroid, and the centroid **must** lie inside $v$'s subtree!
- **Step-by-Step Interview Walkthrough**:
  1. Compute subtree sizes `sz[u]` for all nodes using a standard initial DFS.
  2. Start at root node 1.
  3. Look at all children $v$ of current node $u$.
  4. If any child $v$ has $\text{sz}[v] > N/2$, move to $v$ and repeat.
  5. If no child has subtree size $> N/2$, current node $u$ is guaranteed to be a centroid.
- **Pseudocode**:
```text
function dfs_sz(u, parent):
    sz[u] = 1
    for v in adj[u]:
        if v != parent:
            dfs_sz(v, u)
            sz[u] += sz[v]

function find_centroid(u, parent):
    for v in adj[u]:
        if v != parent and sz[v] > N / 2:
            return find_centroid(v, u)
    return u
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.
