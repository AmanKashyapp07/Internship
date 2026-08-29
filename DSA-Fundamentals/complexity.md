# Core DSA Algorithms Complexities & Interview Flashcards — Interview Master Guide

**Target:** Google, Microsoft, Meta, Amazon, Apple, NVIDIA, Uber, Bloomberg, Atlassian, Adobe, Salesforce, Goldman Sachs, Rubrik, Databricks, etc.

**Priority:** Interview patterns > complexity > behavior > internals > implementation-specific details.

---

# 1. Master Graph Algorithms Matrix

| Algorithm | Graph Type | Time (Best) | Time (Avg) | Time (Worst) | Space | Failure Mode / Caveats |
|---|---|---|---|---|---|---|
| **BFS** | Unweighted | `O(V + E)` | `O(V + E)` | `O(V + E)` | `O(V)` | Fails to find shortest path on weighted graphs. |
| **0-1 BFS** | Weights in `{0, 1}` | `O(V + E)` | `O(V + E)` | `O(V + E)` | `O(V)` | Deque-based; fails on arbitrary weights. |
| **DFS** | General | `O(V + E)` | `O(V + E)` | `O(V + E)` | `O(V)` | Call stack overflows on deep skewed graphs. |
| **Dijkstra** | Non-negative weights | `O(E log V)` | `O(E log V)` | `O(E log V)` | `O(V)` | **Fails on graphs with negative edge weights.** |
| **Bellman-Ford**| General (Negative allowed) | `O(E)` | `O(V * E)` | `O(V * E)` | `O(V)` | Relaxes `V-1` times; detects negative cycles on `V`-th pass. |
| **Floyd-Warshall**| All-Pairs Shortest Path | `Theta(V^3)` | `Theta(V^3)` | `Theta(V^3)` | `Theta(V^2)` | Intermediate `k` loop MUST be outermost. |
| **Kruskal's MST**| Undirected Weighted | `O(E log E)` | `O(E log E)` | `O(E log E)` | `O(V)` | Sorts edges + uses DSU with Path Compression. |
| **Prim's MST** | Undirected Weighted | `O(E log V)` | `O(E log V)` | `O(E log V)` | `O(V)` | Min-Heap vertex expansion; faster on dense graphs (`E ~ V^2`). |
| **Kahn's TopoSort**| Directed Acyclic (DAG) | `O(V + E)` | `O(V + E)` | `O(V + E)` | `O(V)` | In-degree 0 BFS; cycle exists if output size != `V`. |
| **Tarjan's Bridges**| Undirected | `O(V + E)` | `O(V + E)` | `O(V + E)` | `O(V)` | DFS Low-link values; bridge if `low[v] > tin[u]`. |

---

# 2. Master Sorting Algorithms Matrix

| Algorithm | Best Time | Average Time | Worst Time | Space | Stable? | Key Interview Insight |
|---|---|---|---|---|---|---|
| **Merge Sort** | `O(N log N)` | `O(N log N)` | `O(N log N)` | `O(N)` | **Yes** | Divide & Conquer; standard for Linked Lists. |
| **Quick Sort** | `O(N log N)` | `O(N log N)` | `O(N^2)` | `O(log N)` | **No** | In-place partitioning; high cache locality. |
| **Heap Sort** | `O(N log N)` | `O(N log N)` | `O(N log N)` | `O(1)` | **No** | Guaranteed `O(N log N)` in-place, but non-contiguous jumps. |
| **Insertion Sort** | `O(N)` | `O(N^2)` | `O(N^2)` | `O(1)` | **Yes** | Fastest for small `N <= 16` or nearly-sorted arrays. |
| **Counting Sort** | `O(N + K)` | `O(N + K)` | `O(N + K)` | `O(K)` | **Yes** | Non-comparison sort; optimal when key range `K ~ N`. |
| **Radix Sort** | `O(d * (N + b))` | `O(d * (N + b))` | `O(d * (N + b))` | `O(N + b)` | **Yes** | Stable counting sort per digit position. |
| **Quickselect** | `O(N)` | `O(N)` | `O(N^2)` | `O(1)` | **No** | Finds `K`-th largest element in `O(N)` average time. |

---

# 3. Master Data Structure Operations Matrix

| Data Structure | Access | Search | Insert | Delete | Space Overhead |
|---|---|---|---|---|---|
| **Array / Vector** | `O(1)` | `O(N)` | Amortized `O(1)` at end / `O(N)` mid | `O(1)` at end / `O(N)` mid | Contiguous (0 node overhead) |
| **Doubly Linked List** | `O(N)` | `O(N)` | `O(1)` at known node | `O(1)` at known node | 16-24 bytes pointer overhead per node |
| **Stack / Queue** | `O(1)` top/front | `O(N)` | `O(1)` | `O(1)` | Minimal |
| **Binary Heap (PQ)** | `O(1)` top | `O(N)` | `O(log N)` | `O(log N)` | Contiguous array (0 node overhead) |
| **BST (Balanced)** | N/A | `O(log N)` | `O(log N)` | `O(log N)` | 32-40 bytes per node |
| **Hash Table** | N/A | Avg `O(1)` / Worst `O(N)` | Avg `O(1)` / Worst `O(N)` | Avg `O(1)` / Worst `O(N)` | Bucket vector + node pointers |
| **Trie** | N/A | `O(L)` (word length) | `O(L)` | `O(L)` | 26 pointers per node |
| **DSU (Union-Find)** | N/A | `O(alpha(N)) ~ O(1)` | `O(alpha(N)) ~ O(1)` | N/A | `2N` integers (parent & rank arrays) |
| **Segment Tree** | N/A | `O(log N)` (Range Query) | `O(log N)` (Point Update) | N/A | `4N` array size |
| **Fenwick Tree (BIT)** | N/A | `O(log N)` (Prefix Sum) | `O(log N)` (Point Update) | N/A | `N` array size |

---

# 4. Top 20 Rapid-Fire Interview Flashcards

### 1. Why does Dijkstra fail on negative edge weights?
> **Answer:** Dijkstra greedily marks a node's distance as finalized the first time it is popped from the priority queue. A negative weight edge encountered later could provide a shorter path, which Dijkstra will never re-evaluate.

### 2. Why is Heapify O(N) while N insertions take O(N log N)?
> **Answer:** Heapify works bottom-up: the vast majority of nodes reside near the bottom leaves where height is small (`N/2` nodes take 0 operations, `N/4` take 1). In contrast, inserting `N` times inserts into the leaves and sifts up all the way to the root each time (`O(log N)` per element).

### 3. What is the difference between Kahn's TopoSort and DFS TopoSort?
> **Answer:** Kahn's uses BFS with in-degree counters (naturally detects cycles if output size != `V`). DFS postorder traversal reverses the finishing times (requires explicit 3-color visiting state to detect cycles).

### 4. What is the amortized cost of DSU operations with Path Compression and Union by Rank?
> **Answer:** `O(alpha(N))` per operation, where `alpha` is the Inverse Ackermann function (`alpha(N) < 5` for any physical universe input size).

### 5. Why is MergeSort preferred over QuickSort for Linked Lists?
> **Answer:** Linked lists do not support `O(1)` random access (needed for QuickSort partitioning). MergeSort accesses list nodes sequentially and merges in-place with `O(1)` pointer manipulations and zero extra array allocation.

### 6. What causes a Hash Map to degrade to O(N)?
> **Answer:** Hash collisions forcing all keys into the same bucket list (either due to a poor hash function, small bucket capacity, or an adversarial collision test).

### 7. What is the difference between `lower_bound` and `upper_bound`?
> **Answer:** `lower_bound(k)` returns an iterator to the **first element >= k**. `upper_bound(k)` returns an iterator to the **first element > k**.

### 8. What is the Exact-K reduction pattern in Sliding Window?
> **Answer:** `Count(Exact K) = atMost(K) - atMost(K - 1)`. Used because "at most K" has a monotonic sliding window property while "exact K" does not.

### 9. Why is `std::vector` faster than `std::list` in practice even for frequent insertions?
> **Answer:** Spatial cache locality. Vectors store elements in contiguous memory (64-byte cache lines loaded into L1 cache). Linked lists allocate nodes scattered across heap memory, causing CPU cache misses and pointer-chasing stalls.

### 10. What is the Monotonic Stack invariant?
> **Answer:** Elements in the stack are kept in strictly increasing (or decreasing) order. When a new element violates the order, existing elements are popped and resolved.

### 11. How does Floyd's Cycle Detection find the cycle starting node?
> **Answer:** When slow and fast meet at distance `k` inside a cycle of length `C`, the distance from the head to the cycle start equals the remaining distance from the meeting point to the cycle start modulo `C`. Resetting slow to head and advancing both 1 step at a time guarantees they meet at the cycle entry.

### 12. What is the difference between 0/1 Knapsack and Unbounded Knapsack loops?
> **Answer:** In 1D space-optimized DP, 0/1 Knapsack iterates capacity **backwards** (`W down to wt`) to prevent using the same item multiple times. Unbounded Knapsack iterates capacity **forwards** (`wt to W`) to allow item reuse.

### 13. How does Quickselect find the K-th largest element in O(N) average time?
> **Answer:** It uses QuickSort partitioning around a pivot. Unlike QuickSort, it only recurses into the **single partition half** containing index `K`, discarding the other half. Work: `N + N/2 + N/4 + ... = 2N = O(N)`.

### 14. What are the 3 cases of the Master Theorem?
> **Answer:** Compare `f(n) = n^c` with `n^(log_b a)`. Case 1: `log_b a > c` ==> `Theta(n^(log_b a))`. Case 2: `log_b a = c` ==> `Theta(n^c log n)`. Case 3: `log_b a < c` ==> `Theta(n^c)`.

### 15. What is a Sentinel Dummy Node and why is it used?
> **Answer:** A temporary dummy node placed before the head of a linked list (`dummy.next = head`). It eliminates edge cases when deleting the head node or inserting into an empty list.

### 16. How does 0-1 BFS achieve O(V + E) time?
> **Answer:** It uses a Deque instead of a priority queue. Weight 0 edges are pushed to the front (`push_front`), and Weight 1 edges are pushed to the back (`push_back`), maintaining monotonic distance order in `O(1)` per edge.

### 17. How does a Bloom Filter guarantee zero false negatives?
> **Answer:** If an element was inserted, all its `k` hashed bits were set to 1. If any of the `k` bits is 0 during lookup, the element definitively was never inserted.

### 18. What is the time complexity of building a Trie with N words of average length L?
> **Answer:** Time is `O(N * L)`, and space is `O(N * L * Sigma)` where `Sigma` is the alphabet size (e.g. 26).

### 19. How do you clear the lowest set bit in an integer in O(1)?
> **Answer:** `x = x & (x - 1)`. Used in Brian Kernighan's bit counting algorithm and Fenwick Tree updates.

### 20. When should you use a B+ Tree vs an LSM-Tree?
> **Answer:** Use B+ Trees for read-heavy workloads requiring fast point lookups and range scans (e.g. relational databases like Postgres). Use LSM-Trees for write-heavy workloads requiring maximum write throughput via sequential append-only writes (e.g. Cassandra, RocksDB).

---

# 5. The Universal Decision Matrix

```text
Problem asks for:
|-- Shortest path unweighted                    ---> BFS (Queue)
|-- Shortest path 0/1 weights                   ---> 0-1 BFS (Deque)
|-- Shortest path non-negative weights          ---> Dijkstra (Priority Queue)
|-- Shortest path negative weights / cycle check ---> Bellman-Ford
|-- All-pairs shortest path                     ---> Floyd-Warshall
|-- Prerequisite / Dependency resolution        ---> Kahn's TopoSort (BFS)
|-- Dynamic connectivity / Components           ---> DSU (Union-Find)
|-- Minimum Spanning Tree                       ---> Kruskal (DSU) or Prim (Heap)
|-- Top K / Running Median                      ---> Heap (Priority Queue)
|-- Prefix search / Dictionary / Max XOR        ---> Trie
|-- Next Greater / Histogram / Subarray bounds  ---> Monotonic Stack
|-- Moving window min/max                       ---> Monotonic Deque
|-- Subarray sum count / Pair sum lookup        ---> Hash Map / Prefix Sum
|-- Range min/max with updates                  ---> Segment Tree / Fenwick Tree
|-- Optimization with subproblem choices        ---> Dynamic Programming
|-- Min/Max monotonic answer space              ---> Binary Search on Answer
`-- State exploration with backtracking         ---> DFS / Backtracking + Pruning
```
