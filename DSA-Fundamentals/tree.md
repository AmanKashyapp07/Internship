# Trees, Graphs & Bit Manipulation — Interview Master Guide

**Target:** Google, Microsoft, Meta, Amazon, Apple, NVIDIA, Uber, Bloomberg, Atlassian, Adobe, Salesforce, Goldman Sachs, Rubrik, Databricks, etc.

**Priority:** Interview patterns > complexity > behavior > internals > implementation-specific details.

---

# 1. Binary Trees & Core Properties

## The 4 Classifications

```text
Full Binary Tree:        Complete Binary Tree:      Perfect Binary Tree:
       ( 1 )                     ( 1 )                      ( 1 )
      /     \                   /     \                    /     \
    ( 2 )   ( 3 )             ( 2 )   ( 3 )              ( 2 )   ( 3 )
    /   \                     /   \   /                  /   \   /   \
  (4)   (5)                 (4)   (5)(6)               (4)   (5)(6)  (7)
(0 or 2 children)      (Filled left-to-right)       (All leaves same level)
```

### Must-Know Formulas
* Total nodes in perfect tree of height `h`: `N = 2^(h+1) - 1` ==> `h = Theta(log N)`.
* Nodes at level `L`: `2^(L-1)` (1-indexed level).
* Min height for `N` nodes: `ceil(log2(N + 1)) - 1`.
* Max height (skewed tree): `N - 1` (degenerates into linked list).

### The 2 Tree Recursion Paradigms
1. **Top-Down (Pass values down via arguments):** Path sum from root, depth tracking, ancestor constraints.
2. **Bottom-Up (Combine child return values):** Max path sum, subtree sizes, tree height, LCA, serialize/deserialize.

---

# 2. Tree Traversals

| Traversal | Order | Interview Use Case |
|---|---|---|
| **Inorder** | Left -> Root -> Right | Yields **strictly sorted order** on BSTs. |
| **Preorder** | Root -> Left -> Right | Tree serialization, cloning, prefix expressions. |
| **Postorder** | Left -> Right -> Root | Bottom-up aggregation (height, diameter, subtree DP, node deletion). |
| **Level-Order (BFS)** | Level by level | Shortest path from root, zigzag views, vertical/top/bottom views. |

### Space Complexity Trap
* Recursive DFS Space is `O(H)` (Call Stack), where `H = log N` (balanced) or `H = N` (skewed).
* **Morris Traversal:** Inorder traversal in `O(1)` auxiliary space by creating temporary threaded pointers from inorder predecessors.

---

# 3. Binary Search Trees (BST)

## The BST Invariant
For every node `X`:
`All keys in Left Subtree < X.val < All keys in Right Subtree`

```text
         ( 8 )
        /     \
      ( 3 )   ( 10 )
      /   \        \
    ( 1 ) ( 6 )    ( 14 )
```

### Complexity

| Operation | Balanced BST | Skewed BST (Worst Case) |
|---|---|---|
| Search | `O(log N)` | `O(N)` |
| Insert | `O(log N)` | `O(N)` |
| Delete | `O(log N)` | `O(N)` |
| Inorder Traversal | `Theta(N)` | `Theta(N)` |

### BST Interview Must-Know
* **LCA in BST:** If `p.val < root.val && q.val < root.val`, go left. If `p.val > root.val && q.val > root.val`, go right. Otherwise, current `root` is the LCA (`O(H)` time).
* **Node Deletion (3 cases):**
  1. Leaf node: simply delete.
  2. 1 child: replace node with its child.
  3. 2 children: replace node value with **Inorder Successor** (smallest node in right subtree), then recursively delete that successor.

---

# 4. Balanced Trees (AVL & Red-Black Trees)

### Why Balance Matters
* Unbalanced BST degrades to a linked list with `O(N)` lookups.
* **AVL Tree:** Strictly balanced (`|height(left) - height(right)| <= 1`). Faster lookups, slower insertions (more rotations).
* **Red-Black Tree:** Relaxed balance (`longest path <= 2 * shortest path`). Fewer rotations on insert/delete. Backs `std::map`, `std::set`, and Java `TreeMap`.

---

# 5. Heaps & Priority Queues

## Complete Binary Tree in an Array

```text
Index:    [ 0 | 1 | 2 | 3 | 4 | 5 ]
Values:   [50 | 30| 40| 10| 20| 35]

Children of index i:
  Left  = 2*i + 1
  Right = 2*i + 2
Parent of index i:
  Parent = (i - 1) / 2
```

### Complexity & Operations

| Operation | Complexity | Note |
|---|---|---|
| `top()` | `O(1)` | Root element (`arr[0]`). |
| `push()` | `O(log N)` | Append to end, sift up. |
| `pop()` | `O(log N)` | Swap root with end, pop back, sift down. |
| **`build_heap` (Heapify)** | **`O(N)`** | Bottom-up sift-down of all non-leaf nodes. (High-yield interview question!). |

### Why Heapify is O(N) (Not O(N log N))
* `N/2` nodes at height 0 take 0 operations.
* `N/4` nodes at height 1 take 1 operation.
* `N/8` nodes at height 2 take 2 operations.
* Sum: `S = sum (N / 2^(h+1)) * h = N * sum (h / 2^(h+1)) = O(N)`.

---

# 6. Tries (Prefix Trees)

## Architecture

```text
Root
  |-- 'a' -> 'p' -> 'p' -> 'l' -> 'e' (isWord = true)
  |                  |
  |                 's' (isWord = true)
  |-- 'b' -> 'a' -> 't' (isWord = true)
```

### Must Know
* **Insert / Search Word:** `O(L)` where `L` is word length.
* **Prefix Search (`startsWith`):** `O(L)`.
* **Bitwise Trie:** Insert 32-bit binary representations to solve **Maximum XOR Pair** in `O(32 * N) = O(N)` time!

---

# 7. Graph Representations & Taxonomy

```text
Adjacency Matrix (V x V):       Adjacency List:
   0  1  2                         [0] -> [1, 2]
0 [0, 1, 1]                        [1] -> [0, 2]
1 [1, 0, 1]                        [2] -> [0, 1]
2 [1, 1, 0]
```

### Complexity Trade-Offs

| Representation | Space | Edge Lookup (`u -> v`) | Iterate Neighbors of `u` | Best For |
|---|---|---|---|---|
| **Adjacency List** | `O(V + E)` | `O(deg(u))` | `O(deg(u))` | **Sparse Graphs** (`E << V^2`) - 95% of interviews |
| **Adjacency Matrix** | `Theta(V^2)` | `O(1)` | `Theta(V)` | **Dense Graphs** (`E ~ V^2`), Floyd-Warshall |

---

# 8. Graph Traversal: BFS vs DFS

## Breadth-First Search (BFS)
* **Underlying Engine:** FIFO Queue.
* **Time Complexity:** `O(V + E)`
* **Space Complexity:** `O(V)`
* **Golden Rule:** Finds **Shortest Path in Unweighted Graphs**. Mark nodes visited *immediately upon pushing to queue*.
* **Multi-Source BFS:** Push all source nodes into queue at step 0 (e.g. Rotting Oranges, 01 Matrix).
* **0-1 BFS:** Deque-based shortest path for edge weights `in {0, 1}` in strictly `O(V + E)` without Dijkstra log factor.

## Depth-First Search (DFS)
* **Underlying Engine:** System Call Stack / Explicit Stack.
* **Time Complexity:** `O(V + E)`
* **Space Complexity:** `O(V)`
* **Use Cases:** Connected components, cycle detection, path existence, topological sorting, backtracking (mazes, word search).

---

# 9. Topological Sort & Cycle Detection

## Directed Acyclic Graphs (DAGs)

```text
Topological Ordering: Linear ordering of vertices such that for every directed edge u -> v, u comes before v.
```

### 1. Kahn's Algorithm (BFS In-Degree 0)
1. Calculate in-degree for all vertices.
2. Push all vertices with `inDegree == 0` into a Queue.
3. Pop `u`, append to topo order. For each neighbor `v`: decrement `inDegree[v]`. If `inDegree[v] == 0`, push to Queue.
4. **Cycle Invariant:** If `topoOrder.size() != V`, the graph has a **Cycle**!

### 2. DFS 3-Color Cycle Detection
* `0 (White / Unvisited)`: Not yet visited.
* `1 (Gray / Visiting)`: Currently on recursion stack. If encountered again -> **Cycle Detected**!
* `2 (Black / Visited)`: Fully explored and safe.

---

# 10. Shortest Path Algorithms Matrix

| Algorithm | Graph Type | Time Complexity | Space Complexity | Failure Mode |
|---|---|---|---|---|
| **BFS** | Unweighted | `O(V + E)` | `O(V)` | Fails on weighted graphs |
| **0-1 BFS** | Edge weights `in {0, 1}` | `O(V + E)` | `O(V)` | Fails on arbitrary weights |
| **Dijkstra** | Non-negative weights | `O((V + E) log V)` | `O(V)` | **Fails on negative weight edges** |
| **Bellman-Ford** | Negative weights allowed | `O(V * E)` | `O(V)` | **Detects negative weight cycles** |
| **Floyd-Warshall** | All-pairs shortest path | `Theta(V^3)` | `Theta(V^2)` | Intermediate `k` loop MUST be outermost |

---

# 11. Disjoint Set Union (DSU) & MST

## DSU (Union-Find)
* **Path Compression:** `parent[x] = find(parent[x])` flattens tree on lookup.
* **Union by Rank / Size:** Attach smaller tree under root of larger tree.
* **Amortized Time per Op:** `O(alpha(N)) ~ O(1)` (Inverse Ackermann function).

### Kruskal's vs Prim's MST

| Feature | Kruskal's Algorithm | Prim's Algorithm |
|---|---|---|
| **Strategy** | Greedy Edge Selection via DSU | Greedy Vertex Expansion via Min-Heap |
| **Time Complexity** | `O(E log E)` (Sorting edges) | `O(E log V)` |
| **Best For** | Sparse Graphs (`E << V^2`) | Dense Graphs (`E ~ V^2`) |

---

# 12. Bit Manipulation Fundamentals

## Core Operations

```text
AND (&):  1 & 1 = 1, else 0     (Masking / Clearing bits)
OR  (|):  0 | 0 = 0, else 1     (Setting bits)
XOR (^):  Diff = 1, Same = 0    (Toggling / Canceling identical pairs)
NOT (~):  Inverts all bits      (~x = -x - 1 in Two's Complement)
Left (<<):  x << k = x * 2^k
Right (>>): x >> k = x / 2^k
```

### Essential Bit Hacks (MUST KNOW)

```cpp
// 1. Check if k-th bit is set
bool isSet = (x & (1 << k)) != 0;

// 2. Set k-th bit
x |= (1 << k);

// 3. Clear k-th bit
x &= ~(1 << k);

// 4. Toggle k-th bit
x ^= (1 << k);

// 5. Clear lowest set bit (Brian Kernighan's Algorithm)
x = x & (x - 1); // e.g. 1100 & 1011 = 1000

// 6. Isolate lowest set bit (LSB)
int lsb = x & (-x); // e.g. 1100 & 0100 = 0100

// 7. Check if x is a power of 2
bool isPowerOfTwo = (x > 0) && ((x & (x - 1)) == 0);

// 8. Count set bits
int count = __builtin_popcount(x); // GCC intrinsic, single CPU instruction
```

### XOR Properties & Patterns
* `x ^ x = 0`
* `x ^ 0 = x`
* Associative & Commutative: order does not matter.
* **Single Number I (LC 136):** XOR all elements -> duplicate pairs cancel out, leaving the unique element.
* **Single Number III (LC 260):** XOR all -> gives `a ^ b`. Find lowest set bit in `a ^ b` to partition numbers into two groups and isolate `a` and `b`.

---

# 13. Bitmasking & Subsets

```text
For N elements, there are 2^N subsets (masks from 0 to (1 << N) - 1).

Iterate all subsets of size N:
for (int mask = 0; mask < (1 << N); mask++) {
    for (int i = 0; i < N; i++) {
        if (mask & (1 << i)) {
            // Element i is included in subset
        }
    }
}
```

---

# 14. Top Interview Questions

## Tier 1 — Must Know
1. How do you find the Lowest Common Ancestor (LCA) in a Binary Tree vs BST?
2. Explain the difference between DFS preorder, inorder, and postorder with use cases.
3. Why is `build_heap` (heapify) `O(N)` instead of `O(N log N)`?
4. How do you detect a cycle in a Directed Graph vs an Undirected Graph?
5. Explain Kahn's Algorithm for Topological Sort.
6. How does Dijkstra's Algorithm work and why does it fail on negative edge weights?
7. Explain DSU (Disjoint Set Union) with Path Compression and Union by Rank.
8. How does `x & (x - 1)` work and what are its applications?

## Tier 2 — Strong Candidate
9. How does 0-1 BFS work and why is it faster than Dijkstra for binary weights?
10. How do you find Bridges and Articulation Points in a graph (Tarjan's Low-Link)?
11. How do you implement a Trie and use it for Prefix Matching?
12. How does Bitmask Dynamic Programming work (e.g. Traveling Salesman / Smallest Sufficient Team)?
13. Compare Kruskal's vs Prim's algorithm for Minimum Spanning Trees.

## Tier 3 — Advanced
14. Explain Segment Trees vs Binary Indexed Trees (Fenwick Trees) for range update and point query.
15. Explain Kosaraju's and Tarjan's algorithms for Strongly Connected Components (SCCs).
16. How does Morris Inorder Traversal achieve `O(1)` auxiliary space?

---

# 15. Pattern Recognition Guide

| Clue in Problem Statement | Target Data Structure / Technique |
|---|---|
| Shortest path unweighted graph / grid | **BFS** |
| Shortest path with non-negative edge costs | **Dijkstra** |
| Course prerequisites, build dependencies, task scheduling | **Topological Sort (Kahn's BFS)** |
| Connected components, dynamic connectivity, cycle detection undirected | **DSU (Union-Find)** |
| Kth largest / smallest, running median, top-K streams | **Priority Queue (Min/Max Heap)** |
| Word dictionary, auto-complete, prefix search, max XOR pair | **Trie** |
| Subsets combination (`N <= 20`), states representation | **Bitmask DP / Bitwise operations** |
| Finding single unpaired number | **XOR cancellation** |

---

# 16. Interview Priority

## P0 — Absolutely Master
```text
Binary Tree traversals (DFS Pre/In/Post, BFS Level-order)
LCA in Binary Tree & BST
Kahn's Topological Sort & Cycle Detection
Dijkstra's Algorithm & BFS Shortest Path
Min/Max Heap operations & Top-K pattern
DSU with Path Compression
Core bit hacks: x & (x - 1), x & (-x), XOR cancellation
```

## P1 — Strongly Know
```text
Trie (Prefix Tree)
0-1 BFS with Deque
Bellman-Ford & Negative Cycle Detection
Kruskal's MST
Bitmask subset iteration
Tree serialization / deserialization
```

## P2 — Know Conceptually
```text
Tarjan's Bridges & Articulation Points
Segment Tree & Fenwick Tree (BIT)
Floyd-Warshall all-pairs shortest path
Morris Traversal
```

## P3 — Don't Waste Time Memorizing
```text
Fibonacci Heap mathematical proofs
Heavy-Light Decomposition unless applying to specialized competitive roles
Exact Red-Black tree rebalancing case-1/2/3 rotations
```
