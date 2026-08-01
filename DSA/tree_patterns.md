# 🌲 Master Tree Patterns Guide (Interviews & OAs)

> **The ultimate cheat sheet for recognizing, formulating, and coding Tree algorithms in tech interviews (FAANG, Top Tech) and Online Assessments (OAs).**

---

## 📋 Table of Contents
1. [Tree Traversal & Subtree Recursion Fundamentals](#1-tree-traversal--subtree-recursion-fundamentals)
2. [Pattern 1: Subtree DP & Bottom-Up DFS (Return-to-Parent)](#pattern-1-subtree-dp--bottom-up-dfs-return-to-parent)
3. [Pattern 2: Tree Rerooting DP (2-Pass DFS / All-Nodes-as-Root DP)](#pattern-2-tree-rerooting-dp-2-pass-dfs--all-nodes-as-root-dp)
4. [Pattern 3: Binary Lifting & Lowest Common Ancestor (LCA)](#pattern-3-binary-lifting--lowest-common-ancestor-lca)
5. [Pattern 4: Euler Tour Technique (Tree Flattening to 1D Array)](#pattern-4-euler-tour-technique-tree-flattening-to-1d-array)
6. [Pattern 5: Tree Difference Array (Prefix Sums on Tree Paths)](#pattern-5-tree-difference-array-prefix-sums-on-tree-paths)
7. [Pattern 6: Small-to-Large Merging (DSU on Tree / Sack Algorithm)](#pattern-6-small-to-large-merging-dsu-on-tree--sack-algorithm)
8. [Pattern 7: Centroid & Centroid Decomposition](#pattern-7-centroid--centroid-decomposition)
9. [Pattern 8: Binary Search Tree (BST) & Construction Patterns](#pattern-8-binary-search-tree-bst--construction-patterns)
10. [Pattern 9: Serialization, Deserialization, & Tree Hashing](#pattern-9-serialization-deserialization--tree-hashing)
11. [Master Tree Pattern Decision Matrix](#master-tree-pattern-decision-matrix)

---

## 1. Tree Traversal & Subtree Recursion Fundamentals

Tree problems are naturally recursive because every subtree is itself a valid tree.

```
          1 (Root)
        /   \
       2     3
      / \     \
     4   5     6
```

### Key Traversal Orders
1. **Pre-Order (Root $\to$ Left $\to$ Right)**: Top-down processing (e.g. passing path state down to children).
2. **In-Order (Left $\to$ Root $\to$ Right)**: Processes BST elements in **sorted ascending order**.
3. **Post-Order (Left $\to$ Right $\to$ Root)**: Bottom-up processing (compute children answers before aggregating at parent).
4. **Level-Order (BFS Queue)**: Horizontal layer-by-layer traversal (shortest distance in unweighted tree).

---

## Pattern 1: Subtree DP & Bottom-Up DFS (Return-to-Parent)

### 🔍 Identification Signals
- Compute property for current node using computed attributes of its subtrees (e.g. Subtree size, Max path, Diameter, Subtree sum).
- Uses **Post-Order DFS**: process left & right children first, then combine results at parent.

### 💻 Standard Minimal Templates

#### 1. Subtree Size Calculation
```cpp
// Minimal Snippet
int dfsSubtreeSize(int u, int p) {
    subSize[u] = 1;
    for (int v : adj[u]) {
        if (v != p) { subSize[u] += dfsSubtreeSize(v, u); }
    }
    return subSize[u];
}
```

#### 2. Tree Diameter (Longest Path in Tree)
- **Method 1: 2-Pass BFS** (Works for unweighted/positive weighted trees):
  1. BFS from arbitrary node $1 \to$ find farthest node $A$.
  2. BFS from node $A \to$ find farthest node $B$. Distance $(A, B) =$ Diameter!
- **Method 2: Single-Pass Post-Order DFS**:
  For each node $u$, diameter passing through $u = \text{max\_depth}(\text{left}) + \text{max\_depth}(\text{right})$.

```cpp
// Minimal Single-Pass Post-Order DFS Diameter Snippet
int dfsDiameter(TreeNode* root, int& maxDiag) {
    if (!root) return 0;
    int left = dfsDiameter(root->left, maxDiag);
    int right = dfsDiameter(root->right, maxDiag);
    maxDiag = max(maxDiag, left + right); // Path through root
    return 1 + max(left, right);         // Height returned to parent
}
```

#### 3. Binary Tree Maximum Path Sum (LeetCode 124)
```cpp
// Minimal Snippet
int dfsMaxPath(TreeNode* root, int& globalMax) {
    if (!root) return 0;
    int left = max(0, dfsMaxPath(root->left, globalMax));   // Ignore negative paths
    int right = max(0, dfsMaxPath(root->right, globalMax));
    globalMax = max(globalMax, root->val + left + right);  // Split path at root
    return root->val + max(left, right);                   // Extendable single path to parent
}
```

---

## Pattern 2: Tree Rerooting DP (2-Pass DFS / All-Nodes-as-Root DP)

### 🔍 Identification Signals
- Requirement: Compute an answer for **EVERY node $i = 1 \dots N$ as if it were the root of the tree**.
- Naive approach: Run DFS from every node $\implies O(N^2)$ (TLE!).
- **Tree Rerooting Technique**: Solve in **$O(N)$ total time** using two DFS passes!

### 💡 Core Mechanics
1. **Pass 1 (Bottom-Up DFS)**: Fix root at node $1$. Compute subtree sizes `sub[u]` and the answer for node $1$ (`ans[1]`).
2. **Pass 2 (Top-Down DFS)**: Move root from parent $u$ to child $v$. Transition formula:
$$\text{ans}[v] = \text{ans}[u] - \text{sub}[v] + (N - \text{sub}[v])$$

```
 Moving root u -> v:
 - Subtree of v gains (N - sub[v]) nodes (which were outside v's subtree).
 - Subtree of v loses sub[v] nodes (which become closer to v than u).
```

### 💻 Standard Rerooting Template (Sum of Distances in Tree)

```cpp
// Minimal Tree Rerooting Snippet (CSES Tree Distances II / LeetCode 834)
void dfs1(int u, int p, int d) {
    sub[u] = 1;
    ans[1] += d; // Distance sum for root node 1
    for (int v : adj[u]) {
        if (v != p) {
            dfs1(v, u, d + 1);
            sub[u] += sub[v];
        }
    }
}

void dfs2(int u, int p, int n) {
    for (int v : adj[u]) {
        if (v != p) {
            ans[v] = ans[u] - sub[v] + (n - sub[v]); // Rerooting Transition Formula
            dfs2(v, u, n);
        }
    }
}
```

---

## Pattern 3: Binary Lifting & Lowest Common Ancestor (LCA)

### 🔍 Identification Signals
- Answer $Q$ queries asking for $K$-th ancestor, LCA of nodes $(u, v)$, path distance $\text{dist}(u, v)$, or min/max edge on path $u \leftrightarrow v$.
- Time Complexity: Precomputation $O(N \log N)$, Query $O(\log N)$.

### 💡 Key Formulas
- $\text{dist}(u, v) = \text{depth}[u] + \text{depth}[v] - 2 \cdot \text{depth}[\text{LCA}(u, v)]$
- Binary jump transition: `up[u][j] = up[up[u][j-1]][j-1]` (where `up[u][j]` is $2^j$-th ancestor of $u$).

### 💻 Standard Binary Lifting & LCA Snippet

```cpp
// Minimal Binary Lifting LCA Snippet
void dfs(int u, int p, int d) {
    depth[u] = d; up[u][0] = p;
    for (int j = 1; j < LOG; j++) up[u][j] = up[up[u][j - 1]][j - 1];
    for (int v : adj[u]) if (v != p) dfs(v, u, d + 1);
}

int getLCA(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int j = LOG - 1; j >= 0; j--)
        if (depth[u] - (1 << j) >= depth[v]) u = up[u][j]; // Lift u to depth of v
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--)
        if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
    return up[u][0];
}
```

---

## Pattern 4: Euler Tour Technique (Tree Flattening to 1D Array)

### 🔍 Identification Signals
- Range queries on subtrees or tree paths (e.g. Add $X$ to all nodes in subtree of $u$, query sum of subtree $u$).
- **Core Concept**: Map tree nodes into a 1D array using DFS entry time `in[u]` and exit time `out[u]`.
- **Subtree Property**: All nodes in the subtree of $u$ occupy a contiguous subarray range `[in[u], out[u]]` in the Euler tour array!

```
Tree:        1             DFS Entry/Exit Order:
           /   \           Node 1: in[1]=1, out[1]=6
          2     3          Node 2: in[2]=2, out[2]=4 (Subtree 2 = [2..4])
         / \               Node 4: in[4]=3, out[4]=3
        4   5              Node 5: in[5]=4, out[5]=4
```

### 💻 Standard Subtree Query Template

```cpp
// Minimal Euler Tour Subtree Range Snippet
int timer = 0;
void dfsEuler(int u, int p) {
    in[u] = ++timer;
    flatArray[timer] = val[u];
    for (int v : adj[u]) if (v != p) dfsEuler(v, u);
    out[u] = timer;
}

// Subtree of node u corresponds to range [in[u], out[u]] in Fenwick Tree / Segment Tree!
void updateNode(int u, int x) { fenwick.update(in[u], x); }
long long querySubtree(int u) { return fenwick.queryRange(in[u], out[u]); }
```

---

## Pattern 5: Tree Difference Array (Prefix Sums on Tree Paths)

### 🔍 Identification Signals
- Process $Q$ path update queries: "Add weight $W$ to all nodes/edges on path between $u$ and $v$".
- Output final values of all nodes after processing all $Q$ queries in $O(N + Q)$ total time.

### 💡 Difference Array Formulas
- **Node-Based Path Update $(u \to v)$**:
  $$\text{diff}[u] += W, \quad \text{diff}[v] += W, \quad \text{diff}[\text{LCA}(u, v)] -= W, \quad \text{diff}[\text{parent}(\text{LCA}(u, v))] -= W$$
- **Edge-Based Path Update $(u \to v)$**:
  $$\text{diff}[u] += W, \quad \text{diff}[v] += W, \quad \text{diff}[\text{LCA}(u, v)] -= 2W$$

### 💻 Standard Template (CSES Counting Paths)

```cpp
// Minimal Tree Difference Array Aggregation Snippet
void addPathUpdate(int u, int v, int w) {
    int lca = getLCA(u, v);
    diff[u] += w;
    diff[v] += w;
    diff[lca] -= w;
    if (up[lca][0] != 0) diff[up[lca][0]] -= w;
}

// Post-order DFS aggregates prefix sums from children to parent
void dfsAggregate(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) {
            dfsAggregate(v, u);
            diff[u] += diff[v]; // Pull values from children
        }
    }
    finalVal[u] = diff[u];
}
```

---

## Pattern 6: Small-to-Large Merging (DSU on Tree / Sack Algorithm)

### 🔍 Identification Signals
- Answer queries about subtree contents (e.g. Count number of distinct colors, most frequent element in subtree $u$).
- Avoid expensive heavy data structures per node by reusing the heavy child's map/set!
- Complexity: $O(N \log^2 N)$ or $O(N \log N)$.

### 💻 Sack Algorithm Template (CSES Distinct Colors)

```cpp
// Minimal Sack (DSU on Tree) Snippet
int heavyChild[N];
void dfsSize(int u, int p) {
    sz[u] = 1; int maxSz = 0;
    for (int v : adj[u]) {
        if (v != p) {
            dfsSize(v, u); sz[u] += sz[v];
            if (sz[v] > maxSz) { maxSz = sz[v]; heavyChild[u] = v; }
        }
    }
}

void dfsSack(int u, int p, bool keep) {
    // 1. Process light children (do not keep their maps)
    for (int v : adj[u])
        if (v != p && v != heavyChild[u]) dfsSack(v, u, false);
        
    // 2. Process heavy child (keep its map)
    if (heavyChild[u]) dfsSack(heavyChild[u], u, true);
    
    // 3. Merge light children into heavy child's map
    for (int v : adj[u])
        if (v != p && v != heavyChild[u]) addSubtree(v, u);
    colorSet.insert(color[u]);
    
    ans[u] = colorSet.size(); // Answer for subtree u
    
    // 4. If keep == false, clear map
    if (!keep) colorSet.clear();
}
```

---

## Pattern 7: Centroid & Centroid Decomposition

### 🔍 Identification Signals
- **Centroid**: A node whose removal splits the tree into subtrees of size $\le \lfloor N/2 \rfloor$. Every tree has at least 1 and at most 2 centroids.
- **Centroid Decomposition**: Divide & conquer tree paths of length $K$ in $O(N \log N)$ time by recursively decomposing tree at centroids.

### 💻 Finding a Tree Centroid ($O(N)$)

```cpp
// Minimal Centroid Finder Snippet
int getCentroid(int u, int p, int totalN) {
    for (int v : adj[u]) {
        if (v != p && sz[v] > totalN / 2) {
            return getCentroid(v, u, totalN); // Move toward larger subtree
        }
    }
    return u; // u is the centroid!
}
```

---

## Pattern 8: Binary Search Tree (BST) & Construction Patterns

### 🔍 Identification Signals
- Binary Tree where $\text{Left} < \text{Root} < \text{Right}$.
- Inorder traversal yields **sorted ascending order**.

### 💻 Standard BST Snippets

#### 1. Validate Binary Search Tree (Range Bounds Check)
```cpp
bool isValidBST(TreeNode* root, long long minVal = -1e18, long long maxVal = 1e18) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isValidBST(root->left, minVal, root->val) && 
           isValidBST(root->right, root->val, maxVal);
}
```

#### 2. Construct BST from Preorder Traversal ($O(N)$)
```cpp
TreeNode* bstFromPreorder(vector<int>& pre, int& idx, int bound = INT_MAX) {
    if (idx == pre.size() || pre[idx] > bound) return nullptr;
    TreeNode* root = new TreeNode(pre[idx++]);
    root->left = bstFromPreorder(pre, idx, root->val);
    root->right = bstFromPreorder(pre, idx, bound);
    return root;
}
```

#### 3. Lowest Common Ancestor in BST ($O(H)$)
```cpp
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (p->val < root->val && q->val < root->val) 
        return lowestCommonAncestorBST(root->left, p, q);
    if (p->val > root->val && q->val > root->val) 
        return lowestCommonAncestorBST(root->right, p, q);
    return root; // Split point is the LCA!
}
```

---

## Pattern 9: Serialization, Deserialization, & Tree Hashing

### 🔍 Identification Signals
- Convert tree into string representation and reconstruct back (Codec).
- Subtree Isomorphism: Check if two subtrees are identical in structure/values.

### 💻 Minimal Serialization & Hashing Snippets

#### 1. Preorder Serialization with Null Markers
```cpp
string serialize(TreeNode* root) {
    if (!root) return "#,";
    return to_string(root->val) + "," + serialize(root->left) + serialize(root->right);
}
```

#### 2. AHU Tree Hashing / Merkle Subtree Hashing
```cpp
// Assigns canonical string hash to subtree for isomorphism check
string subtreeHash(TreeNode* root) {
    if (!root) return "#";
    string left = subtreeHash(root->left);
    string right = subtreeHash(root->right);
    return "(" + left + to_string(root->val) + right + ")";
}
```

---

## Master Tree Pattern Decision Matrix

| Problem Type / Requirement | Recommended Tree Pattern | Primary Technique / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| Compute property per subtree | **Subtree DP / Bottom-Up DFS** | Post-Order DFS | $O(N)$ | $O(H)$ |
| Longest path between any 2 nodes | **Tree Diameter** | 2-Pass BFS or Post-Order DFS | $O(N)$ | $O(N)$ |
| Compute answer for ALL nodes as root | **Tree Rerooting DP** | 2-Pass DFS ($1 \to v$ transition) | $O(N)$ | $O(N)$ |
| $K$-th ancestor / LCA / path distance | **Binary Lifting** | `up[u][j]` Table + LCA | $O(N \log N + Q \log N)$ | $O(N \log N)$ |
| Range queries on subtrees | **Euler Tour Technique** | Subtree Range $[in[u], out[u]]$ + BIT | $O((N + Q) \log N)$ | $O(N)$ |
| Path updates $+W$ to all nodes on $u \to v$ | **Tree Difference Array** | Diff array $+W, +W, -W, -W$ | $O(N + Q)$ | $O(N)$ |
| Count distinct elements per subtree | **Sack / Small-to-Large** | Merge light maps to heavy child | $O(N \log^2 N)$ | $O(N \log N)$ |
| Node that splits tree into parts $\le N/2$ | **Centroid Finding** | Subtree size DFS check | $O(N)$ | $O(N)$ |
| Validate / Construct BST | **BST Property** | Inorder sorted / Range Bounds | $O(N)$ | $O(H)$ |
| Tree string conversion / Isomorphism | **Tree Hashing & Codec** | Preorder Null Markers / AHU Hash | $O(N)$ | $O(N)$ |
