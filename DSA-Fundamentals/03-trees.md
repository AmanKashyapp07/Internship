# Section 03: Trees

> **Focus:** Hierarchical tree structures, balancing invariants, binary heaps, prefix trees (Tries), range query trees (Segment & Fenwick), and disk-optimized multi-way trees (B/B+ Trees).

---

# Table of Contents
1. [Binary Trees (Properties & Traversals)](#1-binary-trees-properties--traversals)
2. [Binary Search Trees (BST)](#2-binary-search-trees-bst)
3. [Self-Balancing Trees (AVL & Red-Black Trees)](#3-self-balancing-trees-avl--red-black-trees)
4. [Heaps & Priority Queues](#4-heaps--priority-queues)
5. [Tries (Prefix Trees)](#5-tries-prefix-trees)
6. [Segment Trees & Fenwick Trees (BIT)](#6-segment-trees--fenwick-trees-bit)
7. [B-Trees & B+ Trees (Database Storage Models)](#7-b-trees--b-trees-database-storage-models)
8. [Quick Recap](#8-quick-recap)

---

# 1. Binary Trees (Properties & Traversals)

A **Binary Tree** is a non-linear hierarchical data structure where each node has at most two children, referred to as the **Left Child** and **Right Child**.

```
                   [ 1 ] (Root, Depth = 0, Level = 1)
                  /     \
            [ 2 ]         [ 3 ] (Depth = 1, Level = 2)
           /     \       /     \
        [ 4 ]   [ 5 ]  [ 6 ]  [ 7 ] (Leaves, Depth = 2, Level = 3)
```

---

### Core Binary Tree Classifications

```
+-----------------------------------------------------------------------------------------+
| 1. FULL BINARY TREE                                                                     |
| Every node has either 0 or 2 children (no node has exactly 1 child).                    |
+-----------------------------------------------------------------------------------------+
| 2. COMPLETE BINARY TREE                                                                 |
| All levels are completely filled except possibly the last level, which is filled from   |
| strictly left to right (Used to back Array Binary Heaps).                               |
+-----------------------------------------------------------------------------------------+
| 3. PERFECT BINARY TREE                                                                  |
| All internal nodes have 2 children and all leaf nodes are at the exact same depth.      |
| Total Nodes N = 2^(H + 1) - 1.                                                          |
+-----------------------------------------------------------------------------------------+
| 4. BALANCED BINARY TREE                                                                 |
| The height of the left and right subtrees of every node differs by at most 1 (Height O(logN)).|
+-----------------------------------------------------------------------------------------+
| 5. DEGENERATE (SKEWED) TREE                                                             |
| Every internal node has only 1 child. Pathological linked list structure (Height O(N)). |
+-----------------------------------------------------------------------------------------+
```

---

### Key Mathematical Formulas
1. **Maximum nodes at Level $L$** (root at $L = 0$): $2^L$.
2. **Maximum nodes in tree of Height $H$** (root height $= 0$): $2^{H + 1} - 1$.
3. **Minimum Height for $N$ nodes:** $H_{\text{min}} = \lceil \log_2(N + 1) \rceil - 1 = \Theta(\log N)$.
4. **Leaf Node vs. Degree-2 Node Theorem:** In any binary tree where every node has 0 or 2 children, the number of leaf nodes $L$ is always:
   $$L = T_2 + 1 \quad (\text{where } T_2 \text{ is the number of internal nodes with 2 children})$$

---

### Tree Traversal Strategies

```
Depth-First Search (DFS) Traversals:
- Pre-Order  (Root -> Left -> Right): [ 1, 2, 4, 5, 3, 6, 7 ] (Cloning, Serialization)
- In-Order   (Left -> Root -> Right): [ 4, 2, 5, 1, 6, 3, 7 ] (Sorted order in BST)
- Post-Order (Left -> Right -> Root): [ 4, 5, 2, 6, 7, 3, 1 ] (Bottom-up deletion, Tree height)

Breadth-First Search (BFS) Traversal:
- Level-Order (Level by Level):       [ 1, 2, 3, 4, 5, 6, 7 ] (Shortest path in unweighted tree)
```

#### Iterative In-Order Traversal (Using Explicit Stack)
```cpp
// Iterative In-Order Traversal (O(N) Time, O(H) Auxiliary Space)
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    
    while (curr != nullptr || !st.empty()) {
        // 1. Reach the leftmost node of the current subtree
        while (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }
        // 2. Pop and process node
        curr = st.top();
        st.pop();
        result.push_back(curr->val);
        
        // 3. Visit right subtree
        curr = curr->right;
    }
    return result;
}
```

#### Morris Traversal ($O(N)$ Time, $O(1)$ Auxiliary Space)
Morris Traversal achieves $O(1)$ space by temporarily creating **Threaded Binary Tree** links: connecting the rightmost node of the left subtree (in-order predecessor) back to the current root node, eliminating stack allocation.

---

### Interview Follow-Up Questions
1. **"Can you reconstruct a unique binary tree given its In-Order and Pre-Order traversals?"**
   - *Answer:* Yes. Pre-Order's first element identifies the root node; finding that root value inside In-Order divides the tree into the Left Subtree elements and Right Subtree elements. Recursively repeat for subtrees. (*Note:* A unique tree *cannot* be built from Pre-Order and Post-Order alone unless the tree is known to be Full).
2. **"What is the maximum depth of the call stack during DFS on an unbalanced tree?"**
   - *Answer:* In a degenerate (skewed) binary tree with $N$ nodes, DFS depth reaches $O(N)$ stack frames, risking a Stack Overflow. On a balanced tree, call stack depth is strictly $O(\log N)$.

---

# 2. Binary Search Trees (BST)

A **Binary Search Tree (BST)** is a binary tree that satisfies the **BST Invariant Property**:
- For every node $X$, all values in the **Left Subtree** are strictly smaller than $X$'s value ($\text{Left} < X$).
- All values in the **Right Subtree** are strictly greater than $X$'s value ($\text{Right} > X$).
- Both Left and Right subtrees must also be valid Binary Search Trees.

```
                    [ 50 ]
                   /      \
             [ 30 ]        [ 70 ]
            /      \      /      \
        [ 20 ]   [ 40 ] [ 60 ]   [ 80 ]
  In-Order Traversal: 20 -> 30 -> 40 -> 50 -> 60 -> 70 -> 80 (Strictly Ascending Sorted Order!)
```

---

### Core Operations Complexity

| Operation | Balanced BST (AVL/Red-Black) | Unbalanced / Skewed BST | Mechanical Reason |
| :--- | :---: | :---: | :--- |
| **Search** | **$O(\log N)$** | **$O(N)$** | Traverses down height $H$; $H = \log N$ if balanced, $H = N$ if skewed. |
| **Insertion** | **$O(\log N)$** | **$O(N)$** | Finds insertion leaf in $O(H)$ time and attaches new node. |
| **Deletion** | **$O(\log N)$** | **$O(N)$** | Finds target and replaces with In-Order Successor/Predecessor. |
| **Min / Max** | **$O(\log N)$** | **$O(N)$** | Follows leftmost (`left->left...`) or rightmost pointers. |

---

### BST Node Deletion (The 3 Structural Cases)
1. **Case 1 (Leaf Node):** Target node has 0 children $\implies$ Simply delete node and set parent pointer to `nullptr`.
2. **Case 2 (Single Child):** Target node has 1 child $\implies$ Bypass target node by connecting parent pointer directly to target's child.
3. **Case 3 (Two Children):** Target node has 2 children:
   - Find the **In-Order Successor** (the minimum value in the right subtree: `curr->right` followed by all `left` pointers).
   - Copy the successor's value into the target node.
   - Delete the successor node from the right subtree (which falls into Case 1 or Case 2).

```cpp
// BST Node Deletion (O(H) Time Complexity)
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;
    
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node found!
        // Case 1 & 2: 0 or 1 child
        if (!root->left) { TreeNode* temp = root->right; delete root; return temp; }
        if (!root->right) { TreeNode* temp = root->left; delete root; return temp; }
        
        // Case 3: 2 children -> Find In-Order Successor (min in right subtree)
        TreeNode* successor = root->right;
        while (successor->left) successor = successor->left;
        
        root->val = successor->val; // Copy value
        root->right = deleteNode(root->right, successor->val); // Delete successor
    }
    return root;
}
```

---

### Lowest Common Ancestor (LCA) in BST vs. Binary Tree
- **In a BST ($O(H)$ Time, $O(1)$ Space):** Start at root. If both values $p, q < \text{root}$, move left. If both $p, q > \text{root}$, move right. The first node where $p$ and $q$ split into different subtrees (or one equals root) is their **LCA**!
- **In a General Binary Tree ($O(N)$ Time):** Must perform post-order DFS to check if $p$ and $q$ reside in left and right subtrees.

---

### Interview Follow-Up Questions
1. **"How do you validate if a binary tree is a valid BST?"**
   - *Answer:* Do NOT just check `node->left < node < node->right` (fails if a deep left grandchild is larger than root). Must enforce a global bounding range via DFS: `isValidBST(node, minVal, maxVal)`, updating ranges on left and right branches.
2. **"How do you find the K-th Smallest Element in a BST in $O(K)$ time?"**
   - *Answer:* Perform an iterative In-Order traversal; decrement $K$ at each visited node. Return the node's value the moment $K == 0$.

---

# 3. Self-Balancing Trees (AVL & Red-Black Trees)

When elements are inserted in sorted order (`1, 2, 3, 4, 5`), a standard BST degenerates into a linked list with $O(N)$ search time. **Self-Balancing Trees** maintain an $O(\log N)$ height invariant through **Tree Rotations**.

```
Tree Rotation Mechanics (Preserves In-Order BST Traversal Order):

       [ Y ]                               [ X ]
      /     \       Right Rotation (RR)   /     \
    [ X ]   [ C ]   ------------------> [ A ]   [ Y ]
   /     \          <-----------------         /     \
 [ A ]   [ B ]      Left Rotation (LL)       [ B ]   [ C ]
```

---

### A. AVL Trees (Strict Height Balancing)
- **Balance Invariant:** For every node $X$, the difference in height between its left and right subtrees (the **Balance Factor**) must be in $\{-1, 0, +1\}$:
  $$\text{Balance Factor}(X) = \text{Height}(\text{Left Subtree}) - \text{Height}(\text{Right Subtree}) \in \{-1, 0, 1\}$$
- **Height Guarantee:** Strictly bounded by $H \le 1.44 \log_2 N$.
- **4 Rotation Cases to Restore Balance:**
  1. **LL Case (Left-Left):** Heavy left child with positive BF $\implies$ Perform **1 Right Rotation** at parent.
  2. **RR Case (Right-Right):** Heavy right child with negative BF $\implies$ Perform **1 Left Rotation** at parent.
  3. **LR Case (Left-Right):** Heavy left child with negative BF $\implies$ Left Rotate child, then Right Rotate parent.
  4. **RL Case (Right-Left):** Heavy right child with positive BF $\implies$ Right Rotate child, then Left Rotate parent.

---

### B. Red-Black Trees (Color-Based Balancing)
A **Red-Black Tree** is a self-balancing BST where every node stores an extra color bit (`RED` or `BLACK`). It enforces **5 Invariant Rules**:

```
+-----------------------------------------------------------------------------------------+
| THE 5 RED-BLACK TREE PROPERTIES                                                         |
| 1. Every node is either RED or BLACK.                                                   |
| 2. The ROOT is always BLACK.                                                            |
| 3. Every LEAF (NIL sentinel node) is BLACK.                                             |
| 4. RED PROPERTY: If a node is RED, both of its children must be BLACK                   |
|    (No two RED nodes can be adjacent).                                                  |
| 5. BLACK-HEIGHT PROPERTY: Every simple path from a node to any of its descendant NIL    |
|    leaves must contain the EXACT SAME number of BLACK nodes.                            |
+-----------------------------------------------------------------------------------------+
```

- **Height Guarantee:** The longest path (alternating Red-Black) is at most **twice as long** as the shortest path (all Black). Max height $H \le 2 \log_2(N + 1) = O(\log N)$.

---

### AVL Trees vs. Red-Black Trees (The Engineering Decision)

| Dimension | AVL Tree | Red-Black Tree |
| :--- | :--- | :--- |
| **Balancing Rigidity** | **Strictly Balanced** ($|h_L - h_R| \le 1$). | **Loosely Balanced** ($H \le 2 \log N$). |
| **Lookup Speed** | **Faster** (due to smaller average height). | Slightly slower (~5-10% more pointer hops). |
| **Insertion / Deletion**| Slower (may trigger up to $O(\log N)$ rotations). | **Faster** (at most **2 rotations for insert**, **3 for delete**). |
| **Industry Applications** | In-memory read-intensive lookup dictionaries. | **Standard Library associative containers:** C++ `std::map` / `std::set`, Java `TreeMap`, Linux Completely Fair Scheduler (CFS). |

---

### Interview Follow-Up Questions
1. **"Why do `std::map` in C++ and `TreeMap` in Java use Red-Black Trees instead of AVL Trees?"**
   - *Answer:* Because production standard libraries handle both reads and frequent writes. Red-Black trees bound insertion rotations to at most 2 and deletion rotations to at most 3 (using color flips for the rest), making write modifications significantly faster than AVL trees.
2. **"What is a Splay Tree and when is it preferred?"**
   - *Answer:* A self-adjusting BST that performs tree rotations ("splaying") to move accessed elements to the root. It guarantees amortized $O(\log N)$ operations and provides $O(1)$ access for highly skewed, frequently accessed elements (temporal cache locality).

---

# 4. Heaps & Priority Queues

A **Binary Heap** is a **Complete Binary Tree** stored contiguously in an array that satisfies the **Heap-Order Property**:
- **Max-Heap:** Every parent node is greater than or equal to its children ($\text{Parent} \ge \text{Children}$). Root holds the maximum element.
- **Min-Heap:** Every parent node is less than or equal to its children ($\text{Parent} \le \text{Children}$). Root holds the minimum element.

```
Min-Heap Tree:                 Array Storage Representation:
        [ 10 ]                  Index:  0    1    2    3    4    5
       /      \                 Value: [10,  15,  30,  40,  50,  100]
    [ 15 ]    [ 30 ]            Parent(i)      = floor((i - 1) / 2)
    /    \    /                 LeftChild(i)   = 2*i + 1
 [ 40 ] [ 50][ 100 ]            RightChild(i)  = 2*i + 2
```

---

### Heap Operations & Algorithmic Complexity

```cpp
// 1. push(val): Append to array end -> Heapify-Up (Shift-Up) -> O(log N) Time
// 2. pop(): Swap root with last element -> Pop array end -> Heapify-Down (Shift-Down) -> O(log N) Time
// 3. top(): Return arr[0] -> O(1) Time
```

#### Why `buildHeap` from an Array Takes $O(N)$ Time (Not $O(N \log N)$)
If we build a heap by calling `push()` $N$ times, it takes $O(N \log N)$.  
However, **Bottom-Up Heap Construction** (`buildHeap`) starts from the last non-leaf node ($\lfloor N/2 \rfloor - 1$) down to index 0, running `heapifyDown()` on each node.

```
Mathematical Proof of O(N) buildHeap:
Nodes at height h do at most h work during heapifyDown.
Number of nodes at height h in a tree of size N is <= ceil(N / 2^(h+1)).

Total Work S = Sum_{h=0}^{log N} (N / 2^(h+1)) * h = (N / 2) * Sum_{h=0}^{log N} (h / 2^h)
Using the infinite geometric Taylor series: Sum_{h=0}^{inf} (h / 2^h) = 2.

Total Work S = (N / 2) * 2 = O(N)  ==> Exactly Linear Time!
```

```cpp
// Min-Heap Implementation in C++
class MinHeap {
    vector<int> heap;
    void heapifyDown(int i) {
        int smallest = i, left = 2 * i + 1, right = 2 * i + 2;
        if (left < heap.size() && heap[left] < heap[smallest]) smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest]) smallest = right;
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }
public:
    void buildHeap(vector<int>& arr) {
        heap = arr;
        for (int i = (heap.size() / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
};
```

---

### Top Interview Patterns with Priority Queues
1. **Top K Frequent Elements / Kth Largest:** Maintain a Min-Heap of fixed capacity $K$. For each incoming element, if greater than `minHeap.top()`, pop top and push new element $\implies$ **$O(N \log K)$ Time, $O(K)$ Space**.
2. **Median in a Data Stream:** Maintain two heaps: a Max-Heap for the lower half of numbers and a Min-Heap for the upper half. The median is calculated from the tops in **$O(1)$ time**, with **$O(\log N)$ insertions**.

---

### Interview Follow-Up Questions
1. **"Can a Binary Heap be used to find an arbitrary element in $O(\log N)$ time?"**
   - *Answer:* No! A heap does not enforce left-to-right ordering between sibling nodes; searching for an arbitrary element requires an $O(N)$ linear scan across the entire backing array.
2. **"What is a Fibonacci Heap and where is it used?"**
   - *Answer:* A collection of heap-ordered trees supporting $O(1)$ amortized `decrease-key` operations. It optimizes Dijkstra’s Shortest Path algorithm runtime from $O((V + E) \log V)$ to $O(E + V \log V)$ on dense graphs.

---

# 5. Tries (Prefix Trees)

A **Trie** is an $m$-ary tree data structure used for efficient retrieval and prefix-based searching of strings over an alphabet $\Sigma$. Unlike standard BSTs where keys are stored inside nodes, a node's position in a Trie defines the string key associated with it.

```
Trie storing: ["cat", "car", "cart", "dog", "dot"]

                    [ ROOT ]
                   /        \
                ['c']       ['d']
                  |           |
                ['a']       ['o']
               /     \     /     \
            ['t']*  ['r']*['g']* ['t']*
              |
            ['t']*   (* indicates isEndOfWord = true)
```

---

### Trie Node Memory Structure & Operations

```cpp
struct TrieNode {
    TrieNode* children[26]; // 26 lowercase English letters
    bool isEndOfWord;
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

class Trie {
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }
    
    // Insert Word: O(L) Time, O(L * Sigma) Space (where L = word length)
    void insert(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->isEndOfWord = true;
    }
    
    // Search Word: O(L) Time, O(1) Auxiliary Space
    bool search(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr->isEndOfWord;
    }
    
    // Prefix Search (startsWith): O(L) Time
    bool startsWith(string prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return true;
    }
};
```

---

### Bitwise Trie: Maximum XOR of Two Numbers in an Array
- Store numbers as 32-bit binary strings in a Binary Trie (branch 0 and branch 1).
- For each number, query the Trie from MSB (bit 31) to LSB (bit 0), always attempting to traverse the **opposite bit branch** (`1 ^ bit`) to maximize XOR sum in **$O(32) = O(1)$ time per query** $\implies$ **$O(N)$ total time**.

---

### Interview Follow-Up Questions
1. **"How do you optimize memory consumption in a Trie storing sparse unicode strings?"**
   - *Answer:* Replace fixed-size arrays `children[26]` with dynamic `std::unordered_map<char, TrieNode*>` or use a **Radix Tree (Compressed Trie)** where non-branching node paths are compacted into single edges (e.g. compressing `c -> a -> r` into `car`).
2. **"Trie vs. Hash Table for dictionary lookups: Which is better?"**
   - *Answer:* Hash Tables provide $O(L)$ average lookups with potential hash collisions; Tries provide deterministic $O(L)$ worst-case bounds, support prefix queries (`startsWith`, autocomplete), and find alphabetically ordered strings without sorting.

---

# 6. Segment Trees & Fenwick Trees (BIT)

When an array undergoes frequent **Range Aggregate Queries** (Range Sum, Range Minimum) interleaved with **Point or Range Updates**, naive arrays take $O(1)$ update but $O(N)$ query, while prefix sum arrays take $O(1)$ query but $O(N)$ update. Segment Trees and Fenwick Trees solve both in **$O(\log N)$ time**.

```
Query vs. Update Complexity Comparison:

Data Structure          | Range Sum Query  | Point Update    | Range Update (Lazy) | Space Overhead
---------------------------------------------------------------------------------------------------
Naive Array             | O(N)             | O(1)            | O(N)                | O(N)
Prefix Sum Array        | O(1)             | O(N)            | O(N)                | O(N)
Fenwick Tree (BIT)      | O(log N)         | O(log N)        | O(log N) (diff arr) | O(N) (1x array)
Segment Tree            | O(log N)         | O(log N)        | O(log N) (Lazy)     | O(4N) (4x array)
```

---

### A. Segment Trees (Range Minimum / Sum Queries & Lazy Propagation)
A **Segment Tree** is a full binary tree where leaf nodes represent single array elements, and internal nodes store the merged aggregate (sum, min, max) of their left and right child segments.

```
Segment Tree for Array: [ 1, 3, 5, 7 ] (Range Sum)

                     [ [0-3]: 16 ]
                    /              \
           [ [0-1]: 4 ]          [ [2-3]: 12 ]
          /            \        /             \
      [ [0]: 1 ]   [ [1]: 3 ]  [ [2]: 5 ]    [ [3]: 7 ]
```

- **Lazy Propagation:** When updating an entire range $[L, R]$ by $+V$, updating all leaf nodes takes $O(N)$. Lazy propagation defers updates to child nodes until that specific child segment is queried, caching updates in a `lazy[]` array to maintain **$O(\log N)$ range updates**.

---

### B. Fenwick Trees / Binary Indexed Trees (BIT)
A **Fenwick Tree** uses bitwise manipulation of the **Lowest Set Bit (`i & (-i)`)** to compute prefix sums and update elements using a compact 1D array with zero pointer overhead.

```cpp
// Fenwick Tree (Binary Indexed Tree)
class FenwickTree {
    vector<int> tree;
    int n;
public:
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    
    // Add delta to index i (1-based): O(log N)
    void update(int i, int delta) {
        for (; i <= n; i += (i & (-i))) {
            tree[i] += delta; // Move to parent covering larger range
        }
    }
    
    // Prefix sum from index 1 to i: O(log N)
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= (i & (-i))) {
            sum += tree[i]; // Strip lowest set bit
        }
        return sum;
    }
    
    // Range sum query [L, R] = query(R) - query(L - 1)
};
```

---

### Interview Follow-Up Questions
1. **"When should you choose a Fenwick Tree over a Segment Tree?"**
   - *Answer:* Use Fenwick Trees for simple cumulative prefix sums or point updates because it requires only $1\times$ array space and has minimal code footprint. Use Segment Trees when range queries require complex associative operations (Range Minimum Query, GCD) or when Range Updates require Lazy Propagation.

---

# 7. B-Trees & B+ Trees (Database Storage Models)

A **B-Tree** is a self-balancing, multi-way search tree designed specifically for storage systems, database index engines (PostgreSQL, MySQL InnoDB), and disk filesystems.

```
Binary Search Tree (Memory Bound):          B+ Tree Node (Disk Block Bound):
           [ 50 ]                           +-------------------------------------+
          /      \                          |  10  |  30  |  50  |  70  |  90     | (Order M = 100+)
       [ 20 ]  [ 80 ]                       +-------------------------------------+
                                            /      |      |      |      |        \
 (Height H = 30 -> 30 Disk Seeks!)       [0-9]  [10-29] [30-49] [50-69][70-89]  [>=90]
                                         (Height H = 3 -> Exactly 3 Disk Page Reads!)
```

---

### Why Databases Use B+ Trees Over Binary Search Trees
1. **Disk I/O Block Architecture:** Disks read and write in fixed **4KB or 8KB Page Blocks**. A BST node contains only 1 value and 2 child pointers (~24 bytes), wasting 99% of an 8KB disk read. A B+ Tree node has a high branching factor $M$ (e.g. $M = 500$), packing hundreds of keys into a single 8KB disk page.
2. **Shallow Tree Height:** For 1 billion records ($N = 10^9$):
   - In a balanced BST: $H = \log_2(10^9) \approx 30$ levels $\implies$ **30 random disk seeks** (~300ms on HDD).
   - In a B+ Tree with $M = 500$: $H = \log_{500}(10^9) \approx 3$ levels $\implies$ **Only 3 disk reads** (<5ms).

---

### B-Tree vs. B+ Tree Differences

| Feature | Standard B-Tree | Database B+ Tree |
| :--- | :--- | :--- |
| **Data Record Storage** | Key and row pointers stored in **all internal and leaf nodes**. | Internal nodes store **only search keys/routing pointers**; ALL actual data records are stored exclusively in **Leaf Nodes**. |
| **Node Capacity** | Internal nodes hold fewer keys due to payload overhead. | Internal nodes hold maximum branching keys, keeping height even smaller. |
| **Range Queries** | Must perform slow In-Order tree traversal. | **Ultra-Fast $O(1)$ Range Scans**: All leaf nodes are linked into a **Doubly Linked List** at the bottom level. |

```
B+ Tree Leaf Linked List Architecture (Range Query 'WHERE age BETWEEN 20 AND 50'):
[ Leaf 1: 10, 15 ] <---> [ Leaf 2: 20, 25, 30 ] <---> [ Leaf 3: 35, 40, 50 ] <---> [ Leaf 4: 60, 70 ]
                          ^                                                ^
                          |-------- Sequential Disk Read via Next Ptr -----|
```

---

### Interview Follow-Up Questions
1. **"Why do relational databases use B+ Trees for primary indexes instead of Hash Tables?"**
   - *Answer:* Hash Tables provide $O(1)$ exact equality lookups (`WHERE id = 5`), but cannot perform range scans (`WHERE age BETWEEN 20 AND 30`), prefix matching, or `ORDER BY` sorting. B+ Trees support both $O(\log N)$ exact lookups and $O(\log N + K)$ range queries via linked leaf nodes.
2. **"B+ Tree vs. LSM-Tree (Log-Structured Merge-Tree) in databases:"**
   - *Answer:* B+ Trees (used in Postgres/MySQL) optimize for **read-heavy workloads** using in-place page updates; LSM-Trees (used in Cassandra/RocksDB) optimize for **ultra-high write throughput** by sequentially appending writes to a memory buffer (MemTable) and flushing SSTables to disk.

---

# 8. Quick Recap

- **Tree Height vs Depth:** Depth is measured down from Root (Root $= 0$); Height is measured up from deepest leaf.
- **Full vs Complete Binary Trees:** Full means 0 or 2 children; Complete means filled left-to-right on the bottom level (ideal for array heaps).
- **AVL vs Red-Black:** AVL is strictly height-balanced ($|h_L - h_R| \le 1$, faster reads); Red-Black bounds insert/delete rotations to $O(1)$, making it the standard for `std::map`.
- **Heap Construction:** Bottom-up `buildHeap` executes in strictly linear **$O(N)$ time**; top-down repeated `push` takes $O(N \log N)$.
- **Tries & Radix Trees:** $O(L)$ search and autocomplete independent of dictionary size $N$; Bitwise Tries solve Maximum XOR in $O(32) = O(1)$ per query.
- **Segment vs. Fenwick:** Fenwick uses `i & (-i)` with $1\times$ space for prefix sums; Segment Trees use $4\times$ array space with Lazy Propagation for range updates.
- **B+ Tree Superiority:** High branching factor ($M > 100$) minimizes disk page reads ($H \le 3$), while doubly linked leaf nodes enable $O(1)$ sequential range scans.
