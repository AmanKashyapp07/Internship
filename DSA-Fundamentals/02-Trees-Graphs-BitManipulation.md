# Master Guide 02: Trees, Graphs & Bit Manipulation

> **Focus:** Hierarchical Structures, Balanced Search Trees (AVL, Red-Black), Heaps, Tries, Segment & Fenwick Trees, B/B+ Trees, Graph Algorithms (Shortest Paths, MST, DSU, Topological Sort, SCCs), and Low-Level Bit Manipulation Hacks & Bitmask DP.
> 
> *Designed to crush top-tier FAANG/HFT technical interviews without missing a single bit of detail.*

---

# Table of Contents
1. [Binary Trees (Properties & Traversals)](#1-binary-trees)
2. [Binary Search Trees (BST) & Invariants](#2-binary-search-trees-bst)
3. [Self-Balancing Trees (AVL & Red-Black Trees)](#3-self-balancing-trees)
4. [Heaps & Priority Queues](#4-heaps--priority-queues)
5. [Tries (Prefix Trees) & Aho-Corasick](#5-tries-prefix-trees)
6. [Segment Trees & Fenwick Trees (BIT)](#6-segment-trees--fenwick-trees)
7. [B-Trees & B+ Trees (Database Storage Models)](#7-b-trees--b-trees)
8. [Graph Representations & Taxonomy](#8-graph-representations)
9. [Breadth-First Search (BFS) & Depth-First Search (DFS)](#9-bfs--dfs)
10. [Shortest Path Algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)](#10-shortest-path-algorithms)
11. [Minimum Spanning Trees (Kruskal & Prim)](#11-minimum-spanning-trees)
12. [Topological Sorting & DAGs](#12-topological-sorting--dags)
13. [Disjoint Set Union (DSU / Union-Find)](#13-disjoint-set-union-dsu)
14. [Cycle Detection, Bridges & SCCs (Tarjan & Kosaraju)](#14-cycle-detection-bridges--sccs)
15. [Bitwise Operators & Two's Complement](#15-bitwise-operators--twos-complement)
16. [Essential Bit Manipulation Hacks](#16-essential-bit-manipulation-hacks)
17. [XOR Properties & Single Number Patterns](#17-xor-properties--patterns)
18. [Bitmasking for Combinatorics & DP](#18-bitmasking-for-dp)
19. [Interview Quick-Fire Recap](#19-interview-quick-fire-recap)

---

# 1. Binary Trees

A **Binary Tree** is a hierarchical data structure where each node has at most two children: **Left Child** and **Right Child**.

```
                   [ 1 ] (Root, Depth = 0, Level = 1)
                  /     \
            [ 2 ]         [ 3 ] (Depth = 1, Level = 2)
           /     \       /     \
        [ 4 ]   [ 5 ]  [ 6 ]  [ 7 ] (Leaves, Depth = 2, Level = 3)
```

### Core Classifications
1. **Full Binary Tree:** Every node has either 0 or 2 children (no node has 1 child).
2. **Complete Binary Tree:** Every level is completely filled, except possibly the last level, which is filled from **left to right**. (Backing structure for Heaps).
3. **Perfect Binary Tree:** All internal nodes have 2 children, and all leaves are at the exact same depth. Total nodes $N = 2^{h+1} - 1$.
4. **Balanced Binary Tree:** Height is $O(\log N)$ (e.g. height difference between left and right subtrees $\le 1$).

### Fundamental Mathematical Formulas
- **Nodes at Level $l$:** At most $2^{l-1}$ nodes (where root is level 1).
- **Total Nodes in Perfect Tree of Height $h$:** $N = 2^{h+1} - 1 \implies h = \log_2(N + 1) - 1 = \Theta(\log N)$.
- **Minimum Height for $N$ nodes:** $h_{min} = \lceil \log_2(N + 1) \rceil - 1$.
- **Maximum Height (Skewed Tree):** $h_{max} = N - 1$ (degenerates into a Linked List!).

### Traversals: DFS (Pre, In, Post) & BFS (Level-Order)
- **In-Order (Left, Root, Right):** Produces strictly sorted output on BSTs!
- **Pre-Order (Root, Left, Right):** Ideal for serialization/cloning tree structures.
- **Post-Order (Left, Right, Root):** Bottom-up processing (subtree size, deletion, DP on trees).

```cpp
// Morrs In-Order Traversal: O(N) Time, O(1) Auxiliary Space! (No recursion stack!)
void morrisTraversal(TreeNode* root) {
    TreeNode* curr = root;
    while (curr != nullptr) {
        if (curr->left == nullptr) {
            cout << curr->val << " ";
            curr = curr->right;
        } else {
            TreeNode* predecessor = curr->left;
            while (predecessor->right != nullptr && predecessor->right != curr) {
                predecessor = predecessor->right;
            }
            if (predecessor->right == nullptr) {
                predecessor->right = curr; // Create temporary thread
                curr = curr->left;
            } else {
                predecessor->right = nullptr; // Revert temporary thread
                cout << curr->val << " ";
                curr = curr->right;
            }
        }
    }
}
```

---

# 2. Binary Search Trees (BST)

A **Binary Search Tree (BST)** enforces the invariant:
$$\text{For every node } X: \quad \text{All keys in LeftSubtree}(X) < X.\text{val} < \text{All keys in RightSubtree}(X)$$

```
                     [ 8 ]
                    /     \
              [ 3 ]         [ 10 ]
             /     \              \
        [ 1 ]       [ 6 ]         [ 14 ]
```

### Time Complexities
- **Search / Insert / Delete:** Average $O(\log N)$, Worst-Case $O(N)$ (skewed tree).

### BST Node Deletion (The 3 Cases)
1. **Node is a Leaf:** Set parent link to `nullptr` and delete node.
2. **Node has 1 Child:** Replace node with its child.
3. **Node has 2 Children:** Replace node's value with its **In-Order Successor** (smallest node in right subtree) or **In-Order Predecessor**, then delete that successor node.

---

# 3. Self-Balancing Trees (AVL & Red-Black Trees)

Self-balancing trees prevent $O(N)$ degradation by executing **Tree Rotations** during insertions/deletions.

```
Tree Rotation (Right Rotation around Y):
        Y                   X
       / \                 / \
      X   T3   ======>   T1   Y
     / \                     / \
    T1  T2                  T2  T3
```

### A. AVL Trees (Strict Balance)
- **Invariant:** For every node, $|\text{Height}(\text{Left}) - \text{Height}(\text{Right})| \le 1$ (Balance Factor $\in \{-1, 0, 1\}$).
- **Rotations:** Left (LL), Right (RR), Left-Right (LR), Right-Left (RL).
- **Trade-off:** Strict balance means faster lookups, but more rotations during frequent insertions/deletions.

### B. Red-Black Trees (Relaxed Balance — `std::map` Engine)
- **Invariants:**
  1. Every node is either **Red** or **Black**.
  2. The root is always **Black**.
  3. No two adjacent nodes are Red (No Red Parent-Child pair).
  4. Every path from root to leaf contains the **exact same number of Black nodes** (Black-Height).
- **Height Guarantee:** Height is at most $2 \log_2(N + 1)$.
- **Trade-off:** Fewer rotations during write operations than AVL trees, making it the gold standard for standard libraries (`std::map`, `std::set`, Java `TreeMap`).

---

# 4. Heaps & Priority Queues

A **Binary Heap** is a **Complete Binary Tree** stored as a flat array satisfying the **Heap Invariant**:
- **Max-Heap:** $\text{Parent} \ge \text{Child}$ (Root is global maximum).
- **Min-Heap:** $\text{Parent} \le \text{Child}$ (Root is global minimum).

```
Array Indexing for Node at index i (0-indexed):
- Parent(i)      = (i - 1) / 2
- LeftChild(i)   = 2 * i + 1
- RightChild(i)  = 2 * i + 2
```

### Core Operations
- **`push()` / `insert()`:** Append to end of array, `heapifyUp()` in $O(\log N)$ time.
- **`pop()` / `extractMin()`:** Swap root with last element, pop back, `heapifyDown()` in $O(\log N)$ time.
- **`buildHeap()` (Floyd's Algorithm):** Heapifies from index $N/2$ down to 0 in **$O(N)$ linear time**!

```cpp
// Floyd's O(N) Build-Heap Proof:
// Sum_{h=0}^{\log N} (n / 2^{h+1}) * O(h) = O(n * Sum(h / 2^h)) = O(N) Total Time!
void buildMinHeap(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDown(arr, n, i);
    }
}
```

---

# 5. Tries (Prefix Trees) & Aho-Corasick

A **Trie** is a tree structure where edges represent characters and paths represent prefix strings.

```
Trie containing ["cat", "cap", "do", "dog"]:
                    (root)
                   /      \
                 ['c']    ['d']
                  |         |
                ['a']     ['o']* ("do")
               /     \      |
           ['t']*   ['p']* ['g']* ("dog")
```

- **Operations:** Insert & Search in $\mathbf{O(L)}$ time where $L$ is key length.
- **Aho-Corasick Automaton:** Combines Trie with KMP failure links to search **dictionary of $K$ patterns** in text $S$ in **$O(|S| + \sum |P_i|)$ time**.

---

# 6. Segment Trees & Fenwick Trees (BIT)

### A. Segment Tree (Range Queries & Range Updates)
- Stores interval aggregates over array range $[0, N-1]$.
- **Operations:** Point Update $O(\log N)$, Range Query $O(\log N)$, Range Update with Lazy Propagation $O(\log N)$.

```cpp
// Range Sum Segment Tree Node Merge
void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) tree[v] = new_val;
    else {
        int tm = (tl + tr) / 2;
        if (pos <= tm) update(2*v, tl, tm, pos, new_val);
        else update(2*v+1, tm+1, tr, pos, new_val);
        tree[v] = tree[2*v] + tree[2*v+1];
    }
}
```

### B. Fenwick Tree / Binary Indexed Tree (BIT)
- Lightweight array structure using lowest set bit (`idx & -idx`) to calculate prefix sums in $O(\log N)$ time with zero pointer overhead.

```cpp
struct Fenwick {
    int n; vector<int> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) { for (; i <= n; i += i & -i) tree[i] += delta; }
    int query(int i) { int sum = 0; for (; i > 0; i -= i & -i) sum += tree[i]; return sum; }
};
```

---

# 7. B-Trees & B+ Trees (Database Storage Models)

Designed for disk-based storage (databases & filesystems) where disk I/O is the primary bottleneck.

```
+-----------------------------------------------------------------------------------------+
| B-TREE                                                                                  |
| - Internal nodes store BOTH keys and data record pointers.                              |
| - Keys can be found in internal nodes before reaching leaves.                           |
+-----------------------------------------------------------------------------------------+
| B+ TREE (Database Gold Standard: MySQL InnoDB / PostgreSQL)                             |
| - Internal nodes store ONLY keys (for routing). Maximum fan-out per page!               |
| - ALL data records are stored exclusively in Leaf nodes.                                |
| - Leaf nodes are doubly-linked together, enabling ultra-fast O(log N + K) range scans! |
+-----------------------------------------------------------------------------------------+
```

---

# 8. Graph Representations

A Graph $G = (V, E)$ consists of Vertices $V$ and Edges $E$.

| Representation | Space | Add Edge | Check Edge $(u, v)$ | Iterate Neighbors | Ideal For |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Adjacency Matrix** | $\Theta(V^2)$ | $O(1)$ | $O(1)$ | $O(V)$ | Dense Graphs ($E \approx V^2$) |
| **Adjacency List** | $\Theta(V + E)$ | $O(1)$ | $O(\text{deg}(u))$ | $O(\text{deg}(u))$ | Sparse Graphs ($E \ll V^2$) |

---

# 9. BFS & DFS

- **BFS (Breadth-First Search):** Backed by FIFO Queue. Guarantees **Shortest Path in Unweighted Graphs**. $O(V + E)$ Time, $O(V)$ Space.
- **DFS (Depth-First Search):** Backed by Call Stack. Used for Topological Sort, Cycle Detection, Connected Components, Bridges/SCCs. $O(V + E)$ Time, $O(V)$ Space.

---

# 10. Shortest Path Algorithms

```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM       | GRAPH TYPE             | TIME COMPLEXITY       | SPACE | LIMITATIONS / EDGE CASES|
+---------------------------------------------------------------------------------------------------+
| Unweighted BFS  | Unweighted             | O(V + E)              | O(V)  | Only unweighted graphs. |
| Dijkstra        | Non-negative weights   | O((V + E) log V)      | O(V)  | FAILS on negative edges.|
| Bellman-Ford    | Weighted (Directed)    | O(V * E)              | O(V)  | Detects negative cycles.|
| Floyd-Warshall  | All-Pairs Shortest Path| O(V^3)                | O(V^2)| Small graphs (V <= 500).|
+---------------------------------------------------------------------------------------------------+
```

```cpp
// Dijkstra's Algorithm in C++
vector<int> dijkstra(int src, int V, const vector<vector<pair<int,int>>>& adj) {
    vector<int> dist(V, 1e9);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

---

# 11. Minimum Spanning Trees (MST)

Connects all $V$ nodes in an undirected graph with minimum total edge weight using $V-1$ edges.

- **Kruskal's Algorithm:** Sort edges ascending by weight, add edge if it connects 2 different DSU components. **Time: $O(E \log E)$**.
- **Prim's Algorithm:** Min-heap expanding closest vertex to current tree. **Time: $O((V + E) \log V)$**.

---

# 12. Topological Sorting & DAGs

A linear ordering of vertices in a Directed Acyclic Graph (DAG) such that for every edge $u \to v$, $u$ comes before $v$.

- **Kahn's Algorithm (BFS):** Process vertices with `indegree == 0`. If processed nodes $< V$, graph contains a cycle!
- **DFS Post-Order:** Reverse of post-order DFS traversal.

---

# 13. Disjoint Set Union (DSU / Union-Find)

Maintains dynamic partition of elements into disjoint sets.
- **Optimizations:**
  1. **Path Compression:** Point visited nodes directly to set root during `find()`.
  2. **Union by Rank / Size:** Attach smaller tree under root of larger tree.
- **Time Complexity:** **Amortized $O(\alpha(N))$** per operation (Inverse Ackermann function $\alpha(N) < 5$ for all universe atoms!).

```cpp
struct DSU {
    vector<int> parent, sz;
    DSU(int n) : parent(n), sz(n, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int i) { return parent[i] == i ? i : parent[i] = find(parent[i]); }
    bool unite(int i, int j) {
        int root_i = find(i), root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            return true;
        }
        return false;
    }
};
```

---

# 14. Cycle Detection, Bridges & SCCs

- **Undirected Cycle:** DFS encounters visited node $v \neq \text{parent}[u]$.
- **Directed Cycle:** DFS encounters node currently on recursion stack (`state == GRAY`).
- **Tarjan's Bridges:** Edge $(u, v)$ is a bridge iff $\text{low}[v] > \text{tin}[u]$.
- **Kosaraju's SCCs:** Two DFS passes (Pass 1: record finish order; Pass 2: DFS on reversed graph in stack order).

---

# 15. Bitwise Operators & Two's Complement

### Two's Complement Representation
$$-X = \sim X + 1$$

- **Left Shift (`a << k`):** Multiplies by $2^k$.
- **Right Shift (`a >> k`):** Divides by $2^k$.

---

# 16. Essential Bit Manipulation Hacks

| Operation | Bitwise Expression | Mechanical Explanation |
| :--- | :--- | :--- |
| **Check $k$-th bit** | `(n >> k) & 1` | Shift bit $k$ to index 0 |
| **Set $k$-th bit** | `n | (1 << k)` | Force bit $k$ to 1 |
| **Clear $k$-th bit** | `n & ~(1 << k)` | Mask bit $k$ to 0 |
| **Toggle $k$-th bit** | `n ^ (1 << k)` | Flip bit $k$ |
| **Isolate lowest set bit** | `n & (-n)` | Two's Complement property |
| **Clear lowest set bit** | `n & (n - 1)` | Clears trailing 1 |
| **Power of 2 check** | `n > 0 && !(n & (n - 1))` | Exactly one bit set |

```cpp
// Brian Kernighan's Algorithm: O(Set Bits)
int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1); // Clears lowest set bit!
        count++;
    }
    return count; // Built-in alternative: __builtin_popcount(n);
}
```

---

# 17. XOR Properties & Single Number Patterns

### Axioms: $x \oplus 0 = x$, $x \oplus x = 0$, $a \oplus b = b \oplus a$.

```cpp
// Single Number III: Find two unique numbers X and Y where all others appear twice
pair<int, int> singleNumber3(const vector<int>& nums) {
    long long xorAll = 0;
    for (int x : nums) xorAll ^= x;
    long long lowestBit = xorAll & (-xorAll); // Isolate lowest set bit
    int x = 0, y = 0;
    for (int num : nums) {
        if (num & lowestBit) x ^= num;
        else y ^= num;
    }
    return {x, y};
}
```

---

# 18. Bitmasking for Combinatorics & DP

Represent subsets of $N$ items as integer bitmasks from $0 \dots 2^N - 1$.
- **Submask Iteration:** Iterate all submasks of mask in $O(3^N)$ total time:

```cpp
// Iterate all submasks of mask in O(3^N) total complexity
for (int mask = 0; mask < (1 << N); mask++) {
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // Process submask
    }
}
```

---

# 19. Interview Quick-Fire Recap

1. **AVL vs Red-Black:** AVL is strictly balanced (faster searches); Red-Black is relaxed balanced (fewer rotations on insert/delete, ideal for `std::map`).
2. **Floyd's Heap Build:** Heapifying from $N/2$ down to 0 builds a binary heap in $O(N)$ linear time.
3. **B+ Tree Leaf Links:** B+ Tree leaves are doubly linked, enabling $O(\log N + K)$ range queries for database storage engines.
4. **Dijkstra Failure:** Dijkstra fails on negative edge weights because processed nodes are assumed finalized. Use Bellman-Ford.
5. **DSU Complexity:** DSU with Path Compression and Union by Rank takes amortized $O(\alpha(N))$ per operation.
6. **Lowest Set Bit Hack:** `n & (-n)` isolates the lowest 1-bit; `n & (n - 1)` clears the lowest 1-bit.
