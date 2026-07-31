# 🌲 CSES Tree Algorithms - Complete Solutions & Patterns Summary

> **All 13 CSES Tree Problems summarized with core techniques, key C++ implementations, time/space complexities, and interview gotchas.**

---

## Quick Reference Summary Table

| Problem | Primary Pattern / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Subordinates** | Subtree Size DFS Recursion | $O(N)$ | $O(N)$ |
| **Tree Matching** | Greedy Matching / Tree DP | $O(N)$ | $O(N)$ |
| **Tree Diameter** | 2-Pass BFS / DFS | $O(N)$ | $O(N)$ |
| **Tree Distances I** | Max Distance to Any Node (2 Endpoints) | $O(N)$ | $O(N)$ |
| **Tree Distances II** | Tree Rerooting DP | $O(N)$ | $O(N)$ |
| **Company Queries I** | Binary Lifting ($K$-th Ancestor) | $O(N \log N + Q \log K)$ | $O(N \log N)$ |
| **Company Queries II** | LCA via Binary Lifting | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| **Distance Queries** | Path Length via LCA Formula | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| **Counting Paths** | Tree Difference Array + Post-order DFS | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| **Subtree Queries** | Euler Tour (Subtree Range) + Fenwick Tree | $O((N + Q) \log N)$ | $O(N)$ |
| **Path Queries** | Euler Tour ($2N$ Path Range) + Fenwick Tree | $O((N + Q) \log N)$ | $O(N)$ |
| **Distinct Colors** | Small-to-Large Merging (DSU on Tree / Sack) | $O(N \log^2 N)$ | $O(N \log N)$ |
| **Finding a Centroid** | Centroid Finding (Subtree Size $\le N/2$) | $O(N)$ | $O(N)$ |

---

## Detailed Problems & Solutions

### 1. Subordinates
* **Problem**: Count number of subordinates for each employee in a company tree.
* **Technique**: **Subtree Size DFS**. Subordinates count for node $u$ is $\text{subtree\_size}[u] - 1$.
* **Time**: $O(N)$ | **Space**: $O(N)$

```cpp
void dfs(int u, int p, const vector<vector<int>>& g, vector<int>& sub) {
    sub[u] = 1;
    for (int v : g[u]) {
        if (v != p) {
            dfs(v, u, g, sub);
            sub[u] += sub[v];
        }
    }
}
// Subordinates for u = sub[u] - 1
```

---

### 2. Tree Matching
* **Problem**: Find maximum cardinality matching (set of edges with no common vertices) in a tree.
* **Technique**: **Greedy Leaf-to-Root Matching** or **Tree DP**.
* **Greedy Choice**: If child $v$ is unmatched and parent $u$ is unmatched, match $(u, v)$!
* **Time**: $O(N)$ | **Space**: $O(N)$

```cpp
void dfs(int u, int p, const vector<vector<int>>& g, vector<bool>& vis, int& matching) {
    for (int v : g[u]) {
        if (v != p) {
            dfs(v, u, g, vis, matching);
            if (!vis[u] && !vis[v]) {
                vis[u] = vis[v] = true;
                matching++;
            }
        }
    }
}
```

---

### 3. Tree Diameter
* **Problem**: Find the longest path between any two nodes in a tree.
* **Technique**: **2-Pass BFS/DFS**.
  1. Run BFS from arbitrary node (e.g. 1) to find farthest node $A$.
  2. Run BFS from node $A$ to find farthest node $B$.
  3. Distance between $A$ and $B$ is the tree diameter!
* **Time**: $O(N)$ | **Space**: $O(N)$

```cpp
pair<int, int> bfs(int src, int n, const vector<vector<int>>& g) {
    vector<int> dist(n + 1, -1);
    queue<int> q; q.push(src); dist[src] = 0;
    int maxNode = src;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (dist[u] > dist[maxNode]) maxNode = u;
        for (int v : g[u]) {
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
        }
    }
    return {maxNode, dist[maxNode]};
}
// Diameter = bfs(bfs(1, n, g).first, n, g).second
```

---

### 4. Tree Distances I
* **Problem**: For each node, calculate the maximum distance to any other node.
* **Technique**: **Diameter Endpoints Property**.
* **Key Insight**: The farthest node from any node $u$ in a tree is **always** one of the two endpoints $A$ or $B$ of the tree diameter!
  $$\text{ans}[u] = \max \big( \text{dist}(u, A), \text{dist}(u, B) \big)$$
* **Time**: $O(N)$ | **Space**: $O(N)$

---

### 5. Tree Distances II (Tree Rerooting DP)
* **Problem**: Calculate the sum of distances from each node to all other nodes.
* **Technique**: **Tree Rerooting DP**.
  1. Pass 1: Standard DFS to compute subtree sizes $\text{sz}[u]$ and distance sum for root 1 ($\text{dist}[1] = \sum \text{depth}[i]$).
  2. Pass 2: Rerooting DFS. When moving root from parent $u$ to child $v$:
     $$\text{dist}[v] = \text{dist}[u] + N - 2 \cdot \text{sz}[v]$$
* **Time**: $O(N)$ | **Space**: $O(N)$

```cpp
void dfs1(int u, int p, int depth, const vector<vector<int>>& g, vector<long long>& sz, vector<long long>& dist) {
    sz[u] = 1; dist[1] += depth;
    for (int v : g[u]) {
        if (v != p) {
            dfs1(v, u, depth + 1, g, sz, dist);
            sz[u] += sz[v];
        }
    }
}
void reroot(int u, int p, int n, const vector<vector<int>>& g, const vector<long long>& sz, vector<long long>& dist) {
    for (int v : g[u]) {
        if (v != p) {
            dist[v] = dist[u] + n - 2 * sz[v]; // Rerooting transition!
            reroot(v, u, n, g, sz, dist);
        }
    }
}
```

---

### 6. Company Queries I & 7. Company Queries II & 8. Distance Queries
* **Company Queries I**: Find $K$-th boss (ancestor) using Binary Lifting (`up[u][j]`).
* **Company Queries II**: Find Lowest Common Ancestor (LCA) of nodes $A$ and $B$.
* **Distance Queries**: Distance between $A$ and $B$ $= \text{depth}[A] + \text{depth}[B] - 2 \cdot \text{depth}[\text{LCA}(A, B)]$.
* **Time**: $O(N \log N)$ preprocessing, $O(\log N)$ or $O(\log K)$ per query.

```cpp
int lca(int a, int b, int LOG, const vector<vector<int>>& up, const vector<int>& depth) {
    if (depth[a] < depth[b]) swap(a, b);
    for (int j = LOG - 1; j >= 0; j--)
        if (depth[a] - (1 << j) >= depth[b]) a = up[a][j];
    if (a == b) return a;
    for (int j = LOG - 1; j >= 0; j--)
        if (up[a][j] != up[b][j]) { a = up[a][j]; b = up[b][j]; }
    return up[a][0];
}
```

---

### 9. Counting Paths
* **Problem**: Given $M$ paths $(A_i, B_i)$ on a tree, calculate how many paths pass through each node.
* **Technique**: **Tree Difference Array / Prefix Sums on Trees**.
* **Algorithm**:
  1. For each path between $A$ and $B$, compute $L = \text{LCA}(A, B)$.
  2. Increment endpoints: `cnt[A]++`, `cnt[B]++`.
  3. Decrement LCA and its parent: `cnt[L]--`, `cnt[parent[L]]--` (if parent exists).
  4. Run post-order DFS to aggregate subtree counts: $\text{ans}[u] = \text{cnt}[u] + \sum_{v \in \text{children}} \text{ans}[v]$.
* **Time**: $O(N \log N + M \log N)$ | **Space**: $O(N \log N)$

```cpp
// Difference array update for path A <-> B
int L = lca(a, b);
cnt[a]++; cnt[b]++; cnt[L]--;
if (up[L][0] != -1) cnt[up[L][0]]--;

// Aggregation DFS
void dfsAccumulate(int u, int p, const vector<vector<int>>& g, vector<long long>& cnt) {
    for (int v : g[u]) {
        if (v != p) {
            dfsAccumulate(v, u, g, cnt);
            cnt[u] += cnt[v]; // Subtree count aggregation
        }
    }
}
```

---

### 10. Subtree Queries (Euler Tour Variant A)
* **Problem**: Update node value $u$, query sum of node values in $u$'s subtree.
* **Technique**: **Euler Tour Variant A + Fenwick Tree**.
* **Mapping**: Subtree of node $u$ maps to contiguous index range `[tin[u] - 1, tout[u] - 1]` in flattened tour.
* **Operations**:
  - Update node $u$ to value $X$: `ft.update(tin[u] - 1, X - val[u])`.
  - Subtree Query on node $u$: `ft.query(tin[u] - 1, tout[u] - 1)`.
* **Time**: $O((N + Q) \log N)$ | **Space**: $O(N)$

---

### 11. Path Queries (Euler Tour Variant B)
* **Problem**: Update node value $u$, query sum of node values on path from root to $u$.
* **Technique**: **Euler Tour Variant B ($2N$ size flat array) + Fenwick Tree**.
* **Mapping**:
  - Entry time `tin[u]`: place $+val[u]$.
  - Exit time `tout[u]`: place $-val[u]$.
* **Operations**:
  - Update node $u$ to value $X$ ($\Delta = X - val[u]$): `ft.update(tin[u] - 1, +delta)` AND `ft.update(tout[u] - 1, -delta)`.
  - Root-to-Node Path Query on $u$: prefix sum `ft.query(tin[u] - 1)`.
* **Time**: $O((N + Q) \log N)$ | **Space**: $O(N)$

---

### 12. Distinct Colors (Small-to-Large Merging / Sack)
* **Problem**: Calculate the number of distinct colors in the subtree of each node.
* **Technique**: **Small-to-Large Set Merging** (DSU on Tree / Sack).
* **Algorithm**:
  - Maintain `std::set<int>*` for each subtree.
  - Merge child sets into parent set. Always swap pointer with the largest child set (`swap(cur, child)`) before inserting elements.
  - Guarantees $O(N \log^2 N)$ total time because each element is inserted at most $\log N$ times!
* **Time**: $O(N \log^2 N)$ | **Space**: $O(N \log N)$

```cpp
set<int>* dfs(int u, int p, const vector<int>& color, const vector<vector<int>>& g, vector<int>& ans) {
    set<int>* cur = new set<int>();
    cur->insert(color[u]);
    for (int v : g[u]) {
        if (v == p) continue;
        set<int>* child = dfs(v, u, color, g, ans);
        if (cur->size() < child->size()) swap(cur, child); // Small-to-Large swap!
        for (int x : *child) cur->insert(x);
        delete child;
    }
    ans[u] = cur->size();
    return cur;
}
```

---

### 13. Finding a Centroid
* **Problem**: Find a centroid node in a tree (node whose removal leaves components of size $\le N/2$).
* **Technique**: **Centroid Finding DFS**.
* **Algorithm**: Start at root. Move to any neighbor $v$ that has $\text{subtree\_size}[v] > N/2$. If no such neighbor exists, current node is the centroid!
* **Time**: $O(N)$ | **Space**: $O(N)$

```cpp
int getCentroid(int u, int p, int totalN, const vector<vector<int>>& g, const vector<int>& sz) {
    for (int v : g[u]) {
        if (v != p && sz[v] > totalN / 2)
            return getCentroid(v, u, totalN, g, sz);
    }
    return u; // Found centroid!
}
```

---
*End of CSES Tree Solutions Summary*
