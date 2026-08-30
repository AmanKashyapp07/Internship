# Advanced Data Structures, Algorithmic Proofs & Systems Theory

> **Scope:** Self-Balancing Tree Invariants (Red-Black vs. AVL & 2-3-4 B-Tree Isomorphism), Pointer Arithmetic Proofs, Formal Amortized Complexity Frameworks (Aggregate, Accounting, Potential), Morris In-Order Tree Threading, Tarjan's Low-Link Graph Algorithms, Disjoint Set Union (DSU) Inverse Ackermann Derivations, Knuth-Morris-Pratt (KMP) Automata, Storage Engine Layouts, Probabilistic Hashing (Bloom Filters, Count-Min Sketch), and Linear-Time Order Statistics (BFPRT).

---

# Table of Contents
1. [Red-Black Trees vs. AVL Trees & 2-3-4 B-Tree Isomorphism](#1-red-black-trees-vs-avl-trees--2-3-4-b-tree-isomorphism)
2. [Floyd's Cycle Detection Algebraic Proof](#2-floyds-cycle-detection-algebraic-proof)
3. [Amortized Analysis: The Three Formal Frameworks](#3-amortized-analysis-the-three-formal-frameworks)
4. [Morris In-Order & Pre-Order Traversal ($O(1)$ Auxiliary Space)](#4-morris-in-order--pre-order-traversal-o1-auxiliary-space)
5. [Tarjan's Low-Link Graph Theory (Bridges, Articulation Points, SCCs)](#5-tarjans-low-link-graph-theory-bridges-articulation-points-sccs)
6. [Disjoint Set Union (DSU) & The Inverse Ackermann Function $\alpha(N)$](#6-disjoint-set-union-dsu--the-inverse-ackermann-function-alphan)
7. [Knuth-Morris-Pratt (KMP) String Search & The LPS Automaton](#7-knuth-morris-pratt-kmp-string-search--the-lps-automaton)
8. [Database Storage Engines: B+ Trees vs. LSM-Trees](#8-database-storage-engines-b-trees-vs-lsm-trees)
9. [Probabilistic Data Structures: Bloom Filters & Count-Min Sketch](#9-probabilistic-data-structures-bloom-filters--count-min-sketch)
10. [Linear-Time Selection: Quickselect vs. Median-of-Medians (BFPRT)](#10-linear-time-selection-quickselect-vs-median-of-medians-bfprt)
11. [Core Theoretical Summary Principles](#11-core-theoretical-summary-principles)

---

# 1. Red-Black Trees vs. AVL Trees & 2-3-4 B-Tree Isomorphism

### The 5 Red-Black Tree Structural Invariants:
1. **Node Coloring:** Every node is either **RED** or **BLACK**.
2. **Root Invariant:** The root node is **BLACK**.
3. **Leaf Invariant:** Every external leaf node (NIL sentinel) is **BLACK**.
4. **Red Non-Consecutiveness:** If a node is **RED**, both of its children must be **BLACK** (No adjacent red nodes along any path).
5. **Black-Height Invariant:** For every node $X$, all paths from $X$ to descendant leaves contain the **exact same number of BLACK nodes** ($\text{bh}(X)$).

```
                  [ 10 (Black) ]
                 /              \
         [ 5 (Red) ]          [ 20 (Black) ]
        /           \
  [ 2 (Black) ]   [ 8 (Black) ]
```

### Height Bound Derivation:
- Let $\text{bh}$ be the black-height of the root.
- Because red nodes cannot be adjacent, the longest possible path alternates between Black and Red nodes, having at most $2 \cdot \text{bh}$ edges.
- The shortest possible path contains strictly Black nodes of length $\text{bh}$.
- A subtree with black-height $\text{bh}$ contains at least $2^{\text{bh}} - 1$ internal nodes.
- Therefore, $N \ge 2^{\text{bh}} - 1 \implies \text{bh} \le \log_2(N + 1)$, guaranteeing maximum height:
  $$h \le 2 \log_2(N + 1) = \mathbf{O(\log N)}$$

### 2-3-4 Tree Isomorphism:
Every Red-Black tree maps directly to an equivalent balanced 2-3-4 B-Tree:
- Black node with 0 Red children $\iff$ **2-node** (1 key, 2 children).
- Black node with 1 Red child $\iff$ **3-node** (2 keys, 3 children).
- Black node with 2 Red children $\iff$ **4-node** (3 keys, 4 children).
- Red-Black tree color-flipping corresponds directly to a **2-3-4 node split**.

```
+----------------------------------------------------------------------------------------------------+
| METRIC             | AVL TREE                                | RED-BLACK TREE                      |
+----------------------------------------------------------------------------------------------------+
| Balance Strictness | Strict: |h_L - h_R| <= 1                | Relaxed: h_max <= 2 * h_min         |
| Maximum Height     | ~ 1.44 log2(N)                          | ~ 2.00 log2(N)                      |
| Lookup Latency     | Optimal (Shorter path lengths)          | Slightly higher (~20-30% more hops) |
| Insert Rotations   | At most 2 rotations                     | At most 2 rotations                 |
| Deletion Rotations | Up to O(log N) rotations                | At most 3 rotations                 |
| Primary Domain     | Read-intensive lookup tables            | General-purpose associative maps    |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Floyd's Cycle Detection Algebraic Proof

```
Linked List Structure:
[ Head ] -----( L )-----> [ Cycle Entry ] ------( k )------> [ Meeting Point ]
                               ^                                    |
                               |----------------( C - k )-----------|
```

### Derivation:
1. Let $L$ be the distance from `Head` to `Cycle Entry`.
2. Let $C$ be the cycle length, and $k$ be the distance from `Cycle Entry` to the `Meeting Point`.
3. Let $v_{\text{slow}} = 1$ node/step and $v_{\text{fast}} = 2$ nodes/step.
4. When pointers collide at `Meeting Point`:
   $$D_{\text{slow}} = L + k$$
   $$D_{\text{fast}} = L + k + n C \quad (n \ge 1)$$
5. Since $D_{\text{fast}} = 2 D_{\text{slow}}$:
   $$L + k + n C = 2(L + k) \implies n C = L + k$$
   $$L = n C - k = (n - 1) C + (C - k)$$
- **Theorem:** Resetting `slow` to `Head` while holding `fast` at `Meeting Point` and advancing both at 1 node/step causes them to traverse $L$ nodes simultaneously, meeting precisely at `Cycle Entry`.

---

# 3. Amortized Analysis: The Three Formal Frameworks

Amortized analysis establishes worst-case bounds over a continuous sequence of $N$ operations.

### 1. The Aggregate Method
Computes the total execution cost $T(N)$ across a sequence of $N$ operations, evaluating the average cost per operation:
$$\text{Amortized Cost } a = \frac{T(N)}{N}$$
- For a dynamic array with doubling, copy operations occur at capacities $1, 2, 4, \dots, N$.
- $T(N) = N \text{ (inserts)} + \sum_{i=0}^{\log_2 N} 2^i = N + (2N - 1) < 3N \implies a = O(1)$.

### 2. The Accounting (Banker's) Method
Assigns an amortized charge (credit) to each operation. Low-cost operations overcharge and deposit surplus credits into a conceptual bank. High-cost operations consume stored credits:
- Dynamic array insertion assigns a charge of **$3 per element**:
  - $1 pays for the immediate memory store.
  - $1 credit stored with the element to pay for its relocation upon capacity doubling.
  - $1 credit stored with an earlier unshifted element to pay for its relocation.
- When capacity doubles, every element has pre-allocated credits, guaranteeing the bank balance remains non-negative.

### 3. The Potential (Physicist's) Method
Defines a state potential function $\Phi(D_i) \ge 0$ over data structure state $D_i$ with $\Phi(D_0) = 0$:
$$a_i = c_i + \Phi(D_i) - \Phi(D_{i-1})$$
- For a dynamic array, choose $\Phi(D_i) = 2 \cdot \text{size} - \text{capacity}$.
- Immediately after doubling ($\text{capacity} = 2 \cdot \text{size}$): $\Phi = 0$.
- Immediately before doubling ($\text{size} = \text{capacity}$): $\Phi = \text{size}$.
- During doubling: $c_i = \text{size} + 1$, and $\Delta \Phi = 0 - \text{size} = -\text{size}$:
  $$a_i = (\text{size} + 1) - \text{size} = 1 = \mathbf{O(1)}$$

---

# 4. Morris In-Order & Pre-Order Traversal ($O(1)$ Auxiliary Space)

Morris Traversal evaluates binary tree traversals in $O(N)$ time and $O(1)$ auxiliary space without recursive call stacks or parent pointer nodes by creating temporary **Threaded Predecessor Links**:

```
Morris Threading Step:
         ( curr )
         /
       ...
         \
      ( In-Order Predecessor ) ----[ Temporary Thread ]----> ( curr )
```

```cpp
void morrisInorderTraversal(TreeNode* root) {
    TreeNode* curr = root;
    while (curr) {
        if (!curr->left) {
            visitNode(curr);
            curr = curr->right;
        } else {
            // Find in-order predecessor
            TreeNode* pred = curr->left;
            while (pred->right && pred->right != curr) {
                pred = pred->right;
            }

            if (!pred->right) {
                pred->right = curr; // Establish thread
                curr = curr->left;
            } else {
                pred->right = nullptr; // Dissolve thread (Restore tree structure)
                visitNode(curr);
                curr = curr->right;
            }
        }
    }
}
```

---

# 5. Tarjan's Low-Link Graph Theory (Bridges, Articulation Points, SCCs)

Tarjan's graph algorithms maintain two structural arrays across Depth-First Search traversals:
1. `tin[u]`: Discovery timestamp when vertex $u$ is visited by the DFS clock.
2. `low[u]`: Earliest discovery timestamp reachable from $u$ via tree edges and at most one back-edge.

```
DFS Tree Edge (u -> v): Forward branch exploration in DFS tree.
Back-Edge (u -> ancestor): Non-tree edge connecting u to an already-active ancestor.
```

### Graph Properties:
- **Bridge Edge Identification:** An undirected edge $(u, v)$ is a Bridge if and only if:
  $$\text{low}[v] > \text{tin}[u]$$
- **Articulation Point (Cut Vertex):** Vertex $u$ is an articulation point if:
  1. $u$ is the root of the DFS tree and has $\ge 2$ independent child branches.
  2. $u$ is a non-root vertex and has a child $v$ satisfying $\text{low}[v] \ge \text{tin}[u]$.
- **Strongly Connected Components (Directed Graphs):** If $\text{low}[u] == \text{tin}[u]$, vertex $u$ is the root of an SCC. Popping nodes from the traversal stack up to $u$ isolates the complete component.

---

# 6. Disjoint Set Union (DSU) & The Inverse Ackermann Function $\alpha(N)$

```
Path Compression:                          Union by Rank:
     ( 4 )                                    ( Root 1, Rank 2 )
     /                                               /      \
   ( 3 )                                    ( Root 2, Rank 1 ) ...
   /       ===>  [ 1 ] <- (2), (3), (4)
 ( 2 )           (Direct root pointers)
 /
( 1 )
```

### Ackermann Growth & Inversion:
The Ackermann function $A(m, n)$ exhibits non-primitive recursive growth:
- $A(1, 1) = 3$
- $A(2, 2) = 7$
- $A(3, 3) = 61$
- $A(4, 4) = 2^{2^{2^{65536}}} - 3$
- $\alpha(N)$ is defined as the functional inverse: $\alpha(N) = \min \{k \mid A(k, k) \ge N\}$.
- Because $A(4, 4)$ exceeds the estimated number of particles in the universe ($10^{80}$), $\alpha(N) \le 4$ for all computable input domains, yielding effectively $O(1)$ amortized disjoint set operations.

---

# 7. Knuth-Morris-Pratt (KMP) String Search & The LPS Automaton

The **Longest Proper Prefix which is also a Suffix (LPS)** array $\pi$ eliminates redundant character comparisons by precomputing state transitions:
$$\pi[i] = \max \{k \mid P[0 \dots k-1] = P[i-k+1 \dots i] \text{ and } k \le i\}$$

```
Pattern: "A B A B C"
i=0: "A"     -> pi[0] = 0
i=1: "AB"    -> pi[1] = 0
i=2: "ABA"   -> pi[2] = 1 ("A")
i=3: "ABAB"  -> pi[3] = 2 ("AB")
i=4: "ABABC" -> pi[4] = 0
pi Array: [ 0, 0, 1, 2, 0 ]
```

### Invariant:
When a character mismatch occurs at index $j$ in pattern $P$, the text pointer $i$ **never decrements or backtracks**. The pattern pointer shifts directly to index $j = \pi[j - 1]$, executing string search in strict $O(N + M)$ time.

---

# 8. Database Storage Engines: B+ Trees vs. LSM-Trees

```
+----------------------------------------------------------------------------------------------------+
| DIMENSION          | B+ TREE (In-Place Relational Engines)   | LSM-TREE (Append-Only Log Stores)   |
+----------------------------------------------------------------------------------------------------+
| Primary Data Path  | Disk Pages (4KB-16KB frames)            | MemTable (RAM) -> Immutable SSTables|
| Write Model        | In-place random page modification       | Sequential append-only disk logging |
| Write Throughput   | Bounded by random disk I/O & page write amp| High (Buffered sequential flushes)  |
| Read Latency       | Deterministic O(log_B N) page lookups   | Multi-tier check (MemTable, Bloom, SST)|
| Storage Efficiency | ~67% page fill factor (Fragmentation)   | Fully compacted dense sorted runs   |
| Background Work    | Page splitting / Root rebalancing       | Background merge compaction         |
+----------------------------------------------------------------------------------------------------+
```

---

# 9. Probabilistic Data Structures: Bloom Filters & Count-Min Sketch

### 1. Bloom Filter False Positive Probability
For an array of $M$ bits, $k$ independent hash functions, and $N$ inserted elements:
1. Probability a specific bit remains 0 after $N$ insertions:
   $$P(\text{bit} = 0) = \left(1 - \frac{1}{M}\right)^{k N} \approx e^{-k N / M}$$
2. Probability of a false positive ($k$ bits independently set to 1):
   $$p \approx \left(1 - e^{-k N / M}\right)^k$$
3. Minimizing $p$ with respect to $k$ yields:
   $$k = \frac{M}{N} \ln 2 \approx 0.693 \frac{M}{N}$$

### 2. Count-Min Sketch (Frequency Approximation)
Maintains a 2D array of counters $D \times W$ with $D$ independent hash functions.
- **Update:** For incoming item $x$, increment $\text{table}[i][h_i(x)]$ for each row $i \in [0, D-1]$.
- **Point Query:** $\hat{f}(x) = \min_{0 \le i < D} \text{table}[i][h_i(x)]$.
- **Guarantee:** $\hat{f}(x) \ge f(x)$ (Never underestimates true frequency). Error is bounded by $\epsilon N$ with probability $1 - \delta$ by sizing $W = \lceil e/\epsilon \rceil$ and $D = \lceil \ln(1/\delta) \rceil$.

---

# 10. Linear-Time Selection: Quickselect vs. Median-of-Medians (BFPRT)

### 1. Quickselect (Average $O(N)$)
Partitions around a pivot and recurses strictly into the single partition containing target rank $K$:
$$T(N) = N + \frac{N}{2} + \frac{N}{4} + \dots = N \sum_{i=0}^{\infty} \left(\frac{1}{2}\right)^i = \mathbf{2N = O(N)}$$
- Degenerates to $O(N^2)$ if pivot selection is consistently adversarial.

### 2. Median-of-Medians (BFPRT Algorithm - Strict Worst-Case $O(N)$)
1. Partition $N$ elements into groups of 5.
2. Compute medians of each 5-element group in $O(1)$ per group ($O(N)$ total).
3. Recursively compute median of the $N/5$ medians ($M^*$).
4. Use $M^*$ as partition pivot. At least $30\%$ of elements are strictly smaller than $M^*$, and $30\%$ are strictly larger, bounding recursive subproblem size to $\le \frac{7}{10} N$.
$$T(N) \le T\left(\frac{N}{5}\right) + T\left(\frac{7N}{10}\right) + O(N) \implies \mathbf{T(N) = O(N)}$$

---

# 11. Core Theoretical Summary Principles

1. **Red-Black 2-3-4 Equivalence:** Red-Black tree color updates directly map to node splitting in multi-way 2-3-4 B-trees.
2. **Floyd's Cycle Proof:** Algebraic distance modeling proves pointer collision occurs at identical offsets from the cycle entry point.
3. **Formal Amortization:** The Aggregate, Accounting, and Potential methods evaluate average computational costs over worst-case execution sequences.
4. **Threaded Morris Navigation:** Leverages null right-child pointers of in-order predecessors to traverse binary trees in $O(1)$ auxiliary space.
5. **Deterministic Linear Selection:** BFPRT Median-of-Medians guarantees strict $O(N)$ worst-case order statistic selection by bounding partition imbalance.
