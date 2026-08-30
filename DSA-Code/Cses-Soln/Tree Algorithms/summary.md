# CSES Tree Algorithms - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 3 problems in the **Tree Algorithms** topic.

---

## 1. Tree Matching

- **Tier**: Tier 2
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

## 2. Counting Paths

- **Tier**: Tier 3
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

## 3. Distinct Colors

- **Tier**: Tier 3
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
