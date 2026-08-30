# Trees, Graph Algorithms & Bitwise Computational Theory

> **Scope:** Binary Tree Structural Bounds, Tree Traversal Paradigms (DFS, BFS, Morris Threading), Binary Search Tree Invariants, Binary Heap Array Mappings, Prefix & Bitwise Tries, Graph Representations, Topological Sorting Algorithms, Shortest Path Formulations, Minimum Spanning Trees (Kruskal, Prim), and Low-Level Bit Manipulation Physics.

---

# Table of Contents
1. [Binary Tree Classifications & Mathematical Properties](#1-binary-tree-classifications--mathematical-properties)
2. [Tree Traversal Paradigms & Space Complexity](#2-tree-traversal-paradigms--space-complexity)
3. [Binary Search Trees (BST) & Node Deletion Mechanics](#3-binary-search-trees-bst--node-deletion-mechanics)
4. [Complete Binary Heaps & Array Serialization](#4-complete-binary-heaps--array-serialization)
5. [Trie (Prefix Tree) & Bitwise XOR Search](#5-trie-prefix-tree--bitwise-xor-search)
6. [Graph Storage Representations & Complexity](#6-graph-storage-representations--complexity)
7. [Graph Traversal: BFS, 0-1 BFS & DFS](#7-graph-traversal-bfs-0-1-bfs--dfs)
8. [Topological Sorting & Cycle Detection Protocols](#8-topological-sorting--cycle-detection-protocols)
9. [Shortest Path Algorithm Taxonomies](#9-shortest-path-algorithm-taxonomies)
10. [Disjoint Set Union & Minimum Spanning Trees](#10-disjoint-set-union--minimum-spanning-trees)
11. [Low-Level Bit Manipulation & Two's Complement Physics](#11-low-level-bit-manipulation--twos-complement-physics)
12. [Core Theoretical Summary Principles](#12-core-theoretical-summary-principles)

---

# 1. Binary Tree Classifications & Mathematical Properties

```
Full Binary Tree:        Complete Binary Tree:      Perfect Binary Tree:
       ( 1 )                     ( 1 )                      ( 1 )
      /     \                   /     \                    /     \
    ( 2 )   ( 3 )             ( 2 )   ( 3 )              ( 2 )   ( 3 )
    /   \                     /   \   /                  /   \   /   \
  (4)   (5)                 (4)   (5)(6)               (4)   (5)(6)  (7)
(Every node 0 or 2)     (Left-aligned contiguous)   (All leaves at same level)
```

### Mathematical Invariants:
1. **Total Nodes in Perfect Tree of Height $h$:** $N = 2^{h+1} - 1 \implies h = \Theta(\log N)$.
2. **Nodes at Depth $d$ (0-indexed):** Exactly $2^d$ nodes.
3. **Minimum Height for $N$ Nodes:** $h_{\min} = \lceil \log_2(N + 1) \rceil - 1$.
4. **Maximum Height for $N$ Nodes (Skewed Path):** $h_{\max} = N - 1$.

---

# 2. Tree Traversal Paradigms & Space Complexity

```
+----------------------------------------------------------------------------------------------------+
| TRAVERSAL ORDER      | RECURSIVE SEQUENCE                    | CANONICAL APPLICATION DOMAIN        |
+----------------------------------------------------------------------------------------------------+
| In-Order             | Left Subtree -> Node -> Right Subtree | Monotonically sorted order on BSTs  |
| Pre-Order            | Node -> Left Subtree -> Right Subtree | Serialization, prefix expressions   |
| Post-Order           | Left Subtree -> Right Subtree -> Node | Subtree DP, height, diameter, free  |
| Level-Order (BFS)    | Layer-by-layer sequence (Queue)       | Shortest distance, boundary views   |
+----------------------------------------------------------------------------------------------------+
```

### Call Stack Footprint:
Standard recursive Depth-First Search consumes $O(H)$ auxiliary memory, where $H \in [\log N, N]$. **Morris In-Order Traversal** eliminates stack allocation entirely, achieving $O(N)$ runtime in strict $O(1)$ auxiliary memory by utilizing threaded predecessor pointers.

---

# 3. Binary Search Trees (BST) & Node Deletion Mechanics

### The BST Invariant:
For every node $X$ in the tree:
$$\forall u \in \text{LeftSubtree}(X), \, \text{val}(u) < \text{val}(X) \quad \text{and} \quad \forall v \in \text{RightSubtree}(X), \, \text{val}(v) > \text{val}(X)$$

### Node Deletion Protocol (3 Disjoint Cases):
1. **Node is a Leaf (0 Children):** Nullify parent pointer and deallocate node.
2. **Node has 1 Child:** Link parent directly to the single descendant child.
3. **Node has 2 Children:** Replace target node's value with its **In-Order Successor** (the minimal node in its right subtree, found by traversing left from `node->right`), then delete that successor node recursively.

---

# 4. Complete Binary Heaps & Array Serialization

A **Complete Binary Tree** maps directly into a flat contiguous array with zero pointer overhead:

```
Array Indices:  [ 0 | 1 | 2 | 3 | 4 | 5 ]
Element Values: [ 50 | 30 | 40 | 10 | 20 | 35 ]

Index Offsets for Node at Index i:
- Parent Index:       floor((i - 1) / 2)
- Left Child Index:   2 * i + 1
- Right Child Index:  2 * i + 2
```

```
+----------------------------------------------------------------------------------------------------+
| OPERATION            | COMPLEXITY         | THEORETICAL MECHANISM                                  |
+----------------------------------------------------------------------------------------------------+
| `top()` (Peak)       | O(1)               | Reads root value at array index 0                      |
| `push(x)`            | O(log N)           | Appends to array end, executes sift-up comparisons     |
| `pop()`              | O(log N)           | Swaps root with tail, truncates, executes sift-down    |
| `build_heap`         | O(N)               | Bottom-up sift-down across non-leaf nodes N/2 down to 0|
+----------------------------------------------------------------------------------------------------+
```

---

# 5. Trie (Prefix Tree) & Bitwise XOR Search

```
Alphabet Prefix Trie:
Root
  |-- 'a' -> 'p' -> 'p' -> 'l' -> 'e' (Terminal)
  |                  |
  |                 's' (Terminal)
  |-- 'b' -> 'a' -> 't' (Terminal)
```

### Bitwise Maximum XOR Trie:
- Stores 32-bit binary representations of integers from Most Significant Bit (bit 31) down to Least Significant Bit (bit 0).
- To maximize $X \oplus Y$, traverse the trie taking the **opposite bit branch** ($1 \oplus 0 = 1$) whenever available, resolving optimal XOR pairs in deterministic $O(32 \cdot N) = O(N)$ time.

---

# 6. Graph Storage Representations & Complexity

```
Adjacency Matrix (V x V):       Adjacency List:
   0  1  2                         [0] -> [1, 2]
0 [0, 1, 1]                        [1] -> [0, 2]
1 [1, 0, 1]                        [2] -> [0, 1]
2 [1, 1, 0]
```

```
+----------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | ADJACENCY MATRIX                      | ADJACENCY LIST                     |
+----------------------------------------------------------------------------------------------------+
| Memory Footprint     | Theta(V^2)                            | Theta(V + E)                       |
| Edge Existence Check | O(1) direct coordinate lookup         | O(deg(u)) list search              |
| Iterate Neighbors    | Theta(V) row scan                     | O(deg(u)) direct vector scan       |
| Optimal Topology     | Dense Graphs (E ~ V^2)                | Sparse Graphs (E << V^2)           |
+----------------------------------------------------------------------------------------------------+
```

---

# 7. Graph Traversal: BFS, 0-1 BFS & DFS

### 1. Breadth-First Search (BFS)
- **Engine:** FIFO Queue.
- **Complexity:** $O(V + E)$ time, $O(V)$ auxiliary space.
- **Invariant:** Explores vertices in strictly non-decreasing path distance order; guarantees **minimal edge distance** on unweighted graphs.

### 2. 0-1 BFS (Double-Ended Queue)
- Computes single-source shortest paths on graphs with edge weights $w \in \{0, 1\}$ in $O(V + E)$ time.
- Weight 0 edge relaxations push to the **front** of the deque (`push_front`); Weight 1 edge relaxations push to the **back** (`push_back`), preserving monotonic priority without logarithmic heap overhead.

### 3. Depth-First Search (DFS)
- **Engine:** LIFO Call Stack.
- **Complexity:** $O(V + E)$ time, $O(V)$ space.
- **Applications:** Strongly connected components, topological sort, bipartite graph coloring.

---

# 8. Topological Sorting & Cycle Detection Protocols

A **Topological Ordering** is a linear permutation of vertices in a Directed Acyclic Graph (DAG) such that for every directed edge $u \to v$, $u$ appears before $v$.

### 1. Kahn's Algorithm (In-Degree BFS Protocol):
1. Compute in-degree $\text{deg}^-(u)$ for all vertices $u \in V$.
2. Enqueue all vertices with $\text{deg}^-(u) == 0$.
3. While queue is non-empty: pop $u$, record in topological order, and decrement in-degree for all out-neighbors $v$. If $\text{deg}^-(v) == 0$, enqueue $v$.
4. **Cycle Invariant:** If the number of processed vertices $< |V|$, the graph contains at least one **directed cycle**.

### 2. DFS 3-Color State Machine:
- **White (0):** Unvisited vertex.
- **Gray (1):** Active on current recursion path. Encountering a Gray neighbor confirms a **back-edge / cycle**.
- **Black (2):** Fully explored vertex and subtree.

---

# 9. Shortest Path Algorithm Taxonomies

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM            | EDGE WEIGHT CONSTRAINTS               | TIME COMPLEXITY    | SPACE COMPLEXITY|
+----------------------------------------------------------------------------------------------------+
| Breadth-First (BFS)  | Uniform Unweighted (w = 1)            | O(V + E)           | O(V)            |
| 0-1 BFS              | Binary weights in {0, 1}              | O(V + E)           | O(V)            |
| Dijkstra's (Heap)    | Strictly non-negative (w >= 0)        | O(E log V)         | O(V)            |
| Bellman-Ford         | Negative weights allowed (Cycle det)  | O(V * E)           | O(V)            |
| Floyd-Warshall       | Dense All-Pairs (Negative allowed)    | Theta(V^3)         | Theta(V^2)      |
+----------------------------------------------------------------------------------------------------+
```

---

# 10. Disjoint Set Union & Minimum Spanning Trees

### Kruskal's vs. Prim's MST Algorithms:
- **Kruskal's Algorithm:** Greedily sorts all edges $E$ in ascending weight order ($O(E \log E)$) and iterates through them, adding edge $(u, v)$ to the spanning forest if `dsu.find(u) != dsu.find(v)`. Optimal for sparse topologies.
- **Prim's Algorithm:** Initializes at an arbitrary root vertex and greedily expands the cut by extracting the minimal incident edge via a Min-Heap ($O(E \log V)$). Optimal for dense topologies.

---

# 11. Low-Level Bit Manipulation & Two's Complement Physics

### Two's Complement Representation:
$$-x = \sim x + 1$$

```cpp
// 1. Bit Testing & Toggling
bool isKthBitSet   = (x & (1 << k)) != 0;
int setKthBit      = x | (1 << k);
int clearKthBit    = x & ~(1 << k);
int toggleKthBit   = x ^ (1 << k);

// 2. Lowest Set Bit (LSB) Extraction & Brian Kernighan Invariant
int isolateLSB     = x & (-x);       // e.g. (1100)_2 & (0100)_2 = (0100)_2
int clearLowestBit = x & (x - 1);    // Resets lowest set bit to 0

// 3. Power of Two Invariant
bool isPowerOfTwo  = (x > 0) && ((x & (x - 1)) == 0);

// 4. In-Place Hardware Popcount
int totalOnes      = __builtin_popcount(x);
```

---

# 12. Core Theoretical Summary Principles

1. **Tree Height Bounds:** Balanced binary trees guarantee $h = \Theta(\log N)$, whereas skewed degenerations reach $h = N - 1$.
2. **In-Order BST Invariance:** An in-order traversal of a valid Binary Search Tree generates a monotonically increasing sequence.
3. **Array-Heap Indexing:** Contiguous binary heaps eliminate pointer storage overhead using algebraic index calculations ($2i + 1, 2i + 2$).
4. **Topological Invariant:** Kahn's in-degree zero reduction terminates prematurely if and only if a directed dependency cycle exists.
5. **Two's Complement LSB Isolation:** The identity $x \, \& \, (-x)$ isolates the least significant active bit in $O(1)$ hardware execution time.
