# CSES Tree Algorithms - Master Reference

> **All 13 CSES Tree Problems complete with Classic Pattern Identifiers, Precise Problem Statements, Algorithmic Logic, Minimal Informal Code Snippets, and Time/Space Complexities.**

---

## Quick Reference Summary Table

| # | Problem Name | Classic Pattern / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| 1 | **Subordinates** | Subtree Size DFS Recursion | $O(N)$ | $O(N)$ |
| 2 | **Tree Matching** | Greedy Leaf-to-Root / Tree DP | $O(N)$ | $O(N)$ |
| 3 | **Tree Diameter** | 2-Pass BFS / Post-Order DFS | $O(N)$ | $O(N)$ |
| 4 | **Tree Distances I** | Tree Diameter Endpoints Property | $O(N)$ | $O(N)$ |
| 5 | **Tree Distances II** | Tree Rerooting DP (2-Pass DFS) | $O(N)$ | $O(N)$ |
| 6 | **Company Queries I** | Binary Lifting ($K$-th Ancestor) | $O(N \log N + Q \log K)$ | $O(N \log N)$ |
| 7 | **Company Queries II** | LCA via Binary Lifting | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| 8 | **Distance Queries** | Tree Path Distance via LCA Formula | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| 9 | **Counting Paths** | Tree Difference Array + Post-Order DFS | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| 10 | **Subtree Queries** | Euler Tour Subtree Range $[in, out]$ + BIT | $O((N + Q) \log N)$ | $O(N)$ |
| 11 | **Path Queries** | Euler Tour Entry/Exit $+v / -v$ + BIT | $O((N + Q) \log N)$ | $O(N)$ |
| 12 | **Distinct Colors** | Small-to-Large Set Merging (Sack / DSU on Tree) | $O(N \log^2 N)$ | $O(N \log N)$ |
| 13 | **Finding a Centroid** | Centroid Finding (Subtree Size $\le N/2$) | $O(N)$ | $O(N)$ |

---

## Detailed Problems & Minimal Informal Code Snippets

### 1. Subordinates
* **Classic Pattern**: **Subtree Size DFS Recursion**
* **Problem Statement**: Given a rooted tree of $N$ employees ($1 \dots N$, root is $1$), calculate the total number of subordinates (descendants in subtree) for every employee.
* **Algorithm**: Post-order DFS. For node $u$, $\text{subtree\_size}[u] = 1 + \sum_{v \in \text{children}} \text{subtree\_size}[v]$. Subordinates count $= \text{subtree\_size}[u] - 1$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
void dfs(int u, int p) {
    sub[u] = 1;
    for (int v : adj[u])
        if (v != p) { dfs(v, u); sub[u] += sub[v]; }
}
// Subordinates for u = sub[u] - 1
```

---

### 2. Tree Matching
* **Classic Pattern**: **Greedy Leaf-to-Root Matching / Tree DP**
* **Problem Statement**: Find the maximum number of edges in a matching (set of edges sharing no common vertices) in an unrooted tree.
* **Algorithm**: Leaf-to-root post-order DFS. If both child $v$ and parent $u$ are unmatched, greedily match edge $(u, v)$!
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
void dfs(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
            if (!matched[u] && !matched[v]) {
                matched[u] = matched[v] = true;
                matchingCount++;
            }
        }
    }
}
```

---

### 3. Tree Diameter
* **Classic Pattern**: **2-Pass BFS / Post-Order DFS**
* **Problem Statement**: Calculate the length of the longest path (number of edges) between any two nodes in an unweighted tree.
* **Algorithm**: Run BFS from node $1 \to$ find farthest node $A$. Run BFS from $A \to$ find farthest node $B$. Distance $(A, B)$ is the tree diameter.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
auto [A, d1] = bfs(1);
auto [B, diameter] = bfs(A); // diameter is max distance
```

---

### 4. Tree Distances I
* **Classic Pattern**: **Tree Diameter Endpoints Property**
* **Problem Statement**: For every node $i = 1 \dots N$, find the maximum distance to any other node in the tree.
* **Algorithm**: Find diameter endpoints $A$ and $B$. Run BFS from $A \implies \text{distA}[u]$. Run BFS from $B \implies \text{distB}[u]$. For each node $u$, $\text{max\_dist}[u] = \max(\text{distA}[u], \text{distB}[u])$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
// ans[u] = max(distFromA[u], distFromB[u]);
```

---

### 5. Tree Distances II
* **Classic Pattern**: **Tree Rerooting DP (2-Pass DFS)**
* **Problem Statement**: For every node $i = 1 \dots N$, calculate the sum of distances from node $i$ to all other nodes in the tree.
* **Algorithm**:
  1. Pass 1: Compute `sub[u]` and `dist[1]` (sum of depths from root 1).
  2. Pass 2: Rerooting DFS. When moving root $u \to v$: $\text{dist}[v] = \text{dist}[u] + N - 2 \cdot \text{sub}[v]$.
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
void dfs1(int u, int p, int d) {
    sub[u] = 1; dist[1] += d;
    for (int v : adj[u]) if (v != p) { dfs1(v, u, d + 1); sub[u] += sub[v]; }
}
void reroot(int u, int p) {
    for (int v : adj[u]) if (v != p) {
        dist[v] = dist[u] + n - 2 * sub[v]; // Rerooting Transition
        reroot(v, u);
    }
}
```

---

### 6. Company Queries I & 7. Company Queries II & 8. Distance Queries

#### 6. Company Queries I
* **Classic Pattern**: **Binary Lifting ($K$-th Ancestor)**
* **Algorithm**: `up[u][j] = up[up[u][j-1]][j-1]`. Jump $K$ steps in $O(\log K)$ using bits of $K$.

#### 7. Company Queries II
* **Classic Pattern**: **LCA via Binary Lifting**
* **Algorithm**: Lift $u$ to depth of $v$, then lift both together until parents match.

#### 8. Distance Queries
* **Classic Pattern**: **Tree Path Distance Formula**
* **Algorithm**: $\text{dist}(u, v) = \text{depth}[u] + \text{depth}[v] - 2 \cdot \text{depth}[\text{LCA}(u, v)]$.

```cpp
// Minimal LCA Snippet
int getLCA(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int j = LOG - 1; j >= 0; j--)
        if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--)
        if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
    return up[u][0];
}
```

---

### 9. Counting Paths
* **Classic Pattern**: **Tree Difference Array + Post-Order DFS**
* **Problem Statement**: Given $M$ path update queries $(A_i, B_i)$, calculate how many paths pass through each node in the tree.
* **Algorithm**: For each path $(A, B)$, let $L = \text{LCA}(A, B)$. Increment `cnt[A]++`, `cnt[B]++`, decrement `cnt[L]--`, `cnt[parent[L]]--`. Aggregate subtree sums using post-order DFS.
* **Complexities**: $O(N \log N + M \log N)$ Time | $O(N \log N)$ Space

```cpp
// Minimal Snippet
// Path Update: cnt[a]++; cnt[b]++; cnt[L]--; if (parent[L]) cnt[parent[L]]--;
void dfsAggregate(int u, int p) {
    for (int v : adj[u]) if (v != p) { dfsAggregate(v, u); cnt[u] += cnt[v]; }
}
```

---

### 10. Subtree Queries (Euler Tour Variant A)
* **Classic Pattern**: **Euler Tour Subtree Range $[in, out]$ + Fenwick Tree**
* **Problem Statement**: Support point updates on node values and range sum queries for all nodes in a given node's subtree.
* **Algorithm**: Flatten tree via DFS entry `in[u]` and exit `out[u]`. Subtree $u$ corresponds to contiguous range `[in[u], out[u]]` in Fenwick Tree.
* **Complexities**: $O((N + Q) \log N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
void dfs(int u, int p) {
    in[u] = ++timer;
    for (int v : adj[u]) if (v != p) dfs(v, u);
    out[u] = timer;
}
// Update node u: ft.update(in[u], val); Subtree sum u: ft.queryRange(in[u], out[u]);
```

---

### 11. Path Queries (Euler Tour Variant B)
* **Classic Pattern**: **Euler Tour Entry/Exit $+v / -v$ + Fenwick Tree**
* **Problem Statement**: Support point updates on node values and path sum queries from root to node $u$.
* **Algorithm**: Flat array of size $2N$. Place $+val[u]$ at `in[u]` and $-val[u]$ at `out[u]`. Root-to-node path sum $= \text{ft.query}(in[u])$.
* **Complexities**: $O((N + Q) \log N)$ Time | $O(N)$ Space

```cpp
// Minimal Snippet
// Update node u: ft.update(in[u], +delta); ft.update(out[u], -delta);
// Root-to-Node path sum: ft.query(in[u]);
```

---

### 12. Distinct Colors
* **Classic Pattern**: **Small-to-Large Set Merging (Sack / DSU on Tree)**
* **Problem Statement**: Calculate the number of distinct colors in the subtree of every node.
* **Algorithm**: Maintain `set<int>*` per subtree. Recursively merge child sets into parent set, always executing `swap(parentSet, childSet)` if `parentSet->size() < childSet->size()`.
* **Complexities**: $O(N \log^2 N)$ Time | $O(N \log N)$ Space

```cpp
// Minimal Small-to-Large Swap Snippet
set<int>* dfs(int u, int p) {
    set<int>* cur = new set<int>{color[u]};
    for (int v : adj[u]) if (v != p) {
        set<int>* child = dfs(v, u);
        if (cur->size() < child->size()) swap(cur, child); // Small-to-large swap!
        cur->insert(child->begin(), child->end());
        delete child;
    }
    ans[u] = cur->size();
    return cur;
}
```

---

### 13. Finding a Centroid
* **Classic Pattern**: **Centroid Finding (Subtree Size $\le N/2$)**
* **Problem Statement**: Find a centroid node in a tree (a node whose removal leaves connected components of size at most $\lfloor N/2 \rfloor$).
* **Algorithm**: Start at root. Move to any neighbor $v$ that has $\text{sub}[v] > N/2$. If no such neighbor exists, current node is the centroid!
* **Complexities**: $O(N)$ Time | $O(N)$ Space

```cpp
// Minimal Centroid Finder Snippet
int getCentroid(int u, int p, int totalN) {
    for (int v : adj[u])
        if (v != p && sub[v] > totalN / 2) return getCentroid(v, u, totalN);
    return u; // Centroid node!
}
```

---

*End of CSES Tree Algorithms Master Reference*
