# CSES Advanced Techniques - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 13 problems in the **Advanced Techniques** topic.

---

## 1. Hamming Distance

- **Tier**: Tier 2
- **Link**: [Hamming Distance](https://cses.fi/problemset/task/2136)
- **Problem Statement**: Given $N$ binary strings of length $K$, find the minimum Hamming distance (number of differing bits) between any pair of strings.
- **Interview Pattern**: Bitmask XOR + Fast Hardware Popcount.
- **Intuition**:
  - Convert binary strings to 32-bit integers.
  - The Hamming distance between $A[i]$ and $A[j]$ is `popcount(A[i] ^ A[j])`.
  - Comparing all pairs takes $\mathcal{O}(N^2)$ bitwise operations, which is extremely fast with hardware popcount instruction.
- **Step-by-Step Interview Walkthrough**:
  1. Parse input strings into a vector of integers `A[0..N-1]`.
  2. Maintain `min_dist = K`.
  3. Iterate $i$ from 0 to $N-1$ and $j$ from $i+1$ to $N-1$:
     - Compute `dist = __builtin_popcount(A[i] ^ A[j])`.
     - `min_dist = min(min_dist, dist)`.
     - Early exit if `min_dist == 0`.
  4. Return `min_dist`.
- **Pseudocode**:
```text
for i = 0 to N-1: A[i] = parse_binary(string[i])
min_dist = K
for i = 0 to N-1:
    for j = i + 1 to N-1:
        min_dist = min(min_dist, popcount(A[i] ^ A[j]))
        if min_dist == 0: return 0
return min_dist
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N)$.

---

## 2. Meet in the Middle

- **Tier**: Tier 2
- **Link**: [Meet in the Middle](https://cses.fi/problemset/task/1628)
- **Problem Statement**: Given an array of $N$ integers and a target sum $X$, count the number of subsets whose elements sum up to exactly $X$. ($N \le 40$).
- **Interview Pattern**: Meet-in-the-Middle ($2^{N/2}$ Search Space Reduction).
- **Intuition**:
  - Direct $2^N$ subset generation is too slow for $N = 40$ ($2^{40} \approx 10^{12}$).
  - Split the array into two halves of size $N/2$. Generate all $2^{N/2}$ subset sums for each half.
  - For each sum $S_1$ in the left half, binary search / two-pointer for target $X - S_1$ in the sorted right half sums.
- **Step-by-Step Interview Walkthrough**:
  1. Split input into `left` ($0 \dots N/2 - 1$) and `right` ($N/2 \dots N-1$).
  2. Generate all $2^{N/2}$ subset sums for `left` and `right`.
  3. Sort `right` sums array.
  4. For each sum $S_1 \in \text{left\_sums}$:
     - Target needed from right: $T = X - S_1$.
     - Count occurrences of $T$ in `right_sums` using `upper_bound - lower_bound`.
  5. Return total matching subset count.
- **Pseudocode**:
```text
left_sums = generate_all_subset_sums(left_half)
right_sums = generate_all_subset_sums(right_half)
sort(right_sums)

ans = 0
for s in left_sums:
    target = X - s
    ans += count_occurrences(right_sums, target)
return ans
```
- **Complexity**: Time: $\mathcal{O}(2^{N/2} \cdot N)$, Space: $\mathcal{O}(2^{N/2})$.

---

## 3. Corner Subgrid Check

- **Tier**: Tier 2
- **Link**: [Corner Subgrid Check](https://cses.fi/problemset/task/3360)
- **Problem Statement**: Given an $N \times N$ grid of characters, for each of $K$ target uppercase letters, check if there exists a $2 \times 2$ corner subgrid (4 cells forming a rectangle) containing that letter.
- **Interview Pattern**: Column-Pair Hash Map / Boolean Matrix Sweep.
- **Intuition**:
  - A $2 \times 2$ corner rectangle of letter $L$ exists iff there exist two rows $r_1, r_2$ that both contain letter $L$ in the same column pair $(c_1, c_2)$.
- **Step-by-Step Interview Walkthrough**:
  1. For each letter $L \in ['A', 'A'+K-1]$:
  2. Maintain a 2D boolean array `seen[c1][c2]`.
  3. For each row $r$:
     - Collect all column indices `cols` where `grid[r][c] == L`.
     - For every pair $(c_1, c_2)$ in `cols`:
       - If `seen[c1][c2]` is true: rectangle found! Set `found = true` and break.
       - Else mark `seen[c1][c2] = true`.
  4. Print `YES` if `found`, else `NO`.
- **Pseudocode**:
```text
for char L in target_letters:
    seen[0..N-1][0..N-1] = false
    found = false
    for r = 0 to N-1:
        cols = [c for c in 0..N-1 if grid[r][c] == L]
        for (c1, c2) in pairs(cols):
            if seen[c1][c2]: found = true; break
            seen[c1][c2] = true
    print YES if found else NO
```
- **Complexity**: Time: $\mathcal{O}(K \cdot N^2)$, Space: $\mathcal{O}(N^2)$.

---

## 4. Corner Subgrid Count

- **Tier**: Tier 2
- **Link**: [Corner Subgrid Count](https://cses.fi/problemset/task/2137)
- **Problem Statement**: Given an $N \times N$ binary grid, count the total number of $2 \times 2$ corner subgrids with 1s at all 4 corners.
- **Interview Pattern**: Bitset Row AND + Combinatorial $\binom{C}{2}$.
- **Intuition**:
  - Store each row as a `bitset<3000>`.
  - For any row pair $(i, j)$, the number of columns where both rows have a 1 is `C = (row[i] & row[j]).count()`.
  - The number of subgrid rectangles formed by rows $i, j$ is $\binom{C}{2} = C(C - 1) / 2$.
- **Step-by-Step Interview Walkthrough**:
  1. Store each grid row into `bitset<3000> rows[i]`.
  2. Maintain `total_count = 0`.
  3. For row $i$ from 0 to $N-1$ and row $j$ from $i+1$ to $N-1$:
     - Compute common 1-bits: $C = (\text{rows}[i] \ \& \ \text{rows}[j]).\text{count}()$.
     - `total_count += C * (C - 1) / 2`.
  4. Return `total_count`.
- **Pseudocode**:
```text
ans = 0
for i = 0 to N-1:
    for j = i + 1 to N-1:
        common = (rows[i] & rows[j]).count()
        ans += common * (common - 1) / 2
return ans
```
- **Complexity**: Time: $\mathcal{O}(N^3 / 64)$, Space: $\mathcal{O}(N^2 / 8)$.

---

## 5. Eulerian Subgraphs

- **Tier**: Tier 3
- **Link**: [Eulerian Subgraphs](https://cses.fi/problemset/task/2078)
- **Problem Statement**: Given an undirected graph with $N$ nodes and $M$ edges, count the number of Eulerian subgraphs (subsets of edges where every vertex has an even degree) modulo $10^9+7$.
- **Interview Pattern**: Linear Algebra over GF(2) / Graph Connected Components.
- **Intuition**:
  - An Eulerian subgraph corresponds to a vector $x \in \{0, 1\}^M$ satisfying $A x \equiv 0 \pmod 2$, where $A$ is the incidence matrix.
  - The number of solutions is $2^{\text{nullity}(A)} = 2^{M - \text{rank}(A)}$.
  - For an undirected graph with $C$ connected components, $\text{rank}(A) = N - C$. Thus, total Eulerian subgraphs = $2^{M - N + C} \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Count the number of connected components $C$ using DFS/BFS.
  2. Compute exponent $E = M - N + C$.
  3. Output $2^E \pmod{10^9+7}$.
- **Pseudocode**:
```text
components = count_connected_components(graph)
exponent = M - N + components
return power(2, exponent, 10^9 + 7)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 6. Necessary Cities

- **Tier**: Tier 3
- **Link**: [Necessary Cities](https://cses.fi/problemset/task/2077)
- **Problem Statement**: Given a connected undirected graph of $N$ cities and $M$ roads, find all articulation points (cities whose removal disconnects the graph).
- **Interview Pattern**: Tarjan's Articulation Points Algorithm via DFS.
- **Intuition**:
  - A node $u$ is an articulation point if:
    1. Root node: it has $\ge 2$ children in the DFS tree.
    2. Non-root node: it has a child $v$ such that $\text{low}[v] \ge \text{tin}[u]$ (no back-edge from $v$'s subtree reaches an ancestor above $u$).
- **Step-by-Step Interview Walkthrough**:
  1. Run Tarjan's DFS tracking `tin[u]` (discovery order) and `low[u]` (lowest reachable `tin`).
  2. For tree edge $(u, v)$: recurse $v$, update `low[u] = min(low[u], low[v])`. If $\text{low}[v] \ge \text{tin}[u]$ and $u \neq \text{root}$, mark $u$ as articulation point.
  3. For back edge $(u, v)$: update `low[u] = min(low[u], tin[v])`.
  4. Collect all marked articulation points.
- **Pseudocode**:
```text
function dfs(u, parent):
    tin[u] = low[u] = ++timer
    children = 0
    for v in adj[u]:
        if v == parent: continue
        if tin[v]: low[u] = min(low[u], tin[v])
        else:
            dfs(v, u)
            low[u] = min(low[u], low[v])
            if parent != -1 and low[v] >= tin[u]: is_art[u] = true
            children += 1
    if parent == -1 and children > 1: is_art[u] = true
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 7. Necessary Roads

- **Tier**: Tier 3
- **Link**: [Necessary Roads](https://cses.fi/problemset/task/2076)
- **Problem Statement**: Given a connected undirected graph of $N$ cities and $M$ roads, find all bridges (roads whose removal disconnects the graph).
- **Interview Pattern**: Tarjan's Bridge-Finding Algorithm via DFS.
- **Intuition**:
  - An edge $(u, v)$ is a bridge iff $\text{low}[v] > \text{tin}[u]$, meaning no back-edge from $v$'s subtree reaches $u$ or any ancestor of $u$.
- **Step-by-Step Interview Walkthrough**:
  1. Run DFS maintaining `tin[u]` and `low[u]`.
  2. For tree edge $(u, v)$: recurse DFS. If $\text{low}[v] > \text{tin}[u]$, record edge $(u, v)$ as a bridge.
  3. For back-edge $(u, v)$ ($v \neq \text{parent}[u]$): update `low[u] = min(low[u], tin[v])`.
- **Pseudocode**:
```text
function dfs(u, parent):
    tin[u] = low[u] = ++timer
    for v in adj[u]:
        if v == parent: continue
        if tin[v]: low[u] = min(low[u], tin[v])
        else:
            dfs(v, u)
            low[u] = min(low[u], low[v])
            if low[v] > tin[u]: bridges.add(u, v)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 8. New Roads Queries

- **Tier**: Tier 3
- **Link**: [New Roads Queries](https://cses.fi/problemset/task/2101)
- **Problem Statement**: $M$ roads are built sequentially day by day. Process $Q$ queries asking for the earliest day $u$ and $v$ become connected.
- **Interview Pattern**: Parallel Binary Search + Disjoint Set Union (DSU).
- **Intuition**:
  - Each query $(u, v)$ independently binary searches for the answer day in range $[1, M]$.
  - Instead of running $Q$ separate binary searches, process all $Q$ queries in parallel using **Parallel Binary Search**: in each round, group query mid-points into buckets and rebuild DSU once per round.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain range $[\text{lo}[i], \text{hi}[i]]$ for each query $i$.
  2. Loop until all queries converge ($\mathcal{O}(\log M)$ rounds):
     - Assign query $i$ to bucket at $\text{mid} = (\text{lo}[i] + \text{hi}[i]) / 2$.
     - Reset DSU. Iterate days $d = 1 \dots M$: add road $d$ to DSU. For queries in bucket $d$, check if $u, v$ are connected; update $\text{hi}[i] = d$ or $\text{lo}[i] = d + 1$.
  3. Output answers.
- **Pseudocode**:
```text
lo[1..Q] = 1, hi[1..Q] = M + 1
while any lo[i] < hi[i]:
    bucket[1..M+1] = []
    for i = 1 to Q:
        if lo[i] < hi[i]: bucket[(lo[i]+hi[i])/2].push(i)
    dsu.reset()
    for day = 1 to M:
        dsu.unite(road[day])
        for q_id in bucket[day]:
            if dsu.same(u[q_id], v[q_id]): hi[q_id] = day
            else: lo[q_id] = day + 1
```
- **Complexity**: Time: $\mathcal{O}((M + Q) \log M)$, Space: $\mathcal{O}(N + M + Q)$.

---

## 9. Reachability Queries

- **Tier**: Tier 3
- **Link**: [Reachability Queries](https://cses.fi/problemset/task/2143)
- **Problem Statement**: Given a directed graph of $N$ nodes and $M$ edges, process $Q$ reachability queries asking if node $a$ can reach node $b$.
- **Interview Pattern**: Kosaraju SCC Condensation + Bitset Reachability on DAG.
- **Intuition**:
  - Condense the directed graph into a Directed Acyclic Graph (DAG) of Strongly Connected Components (SCCs).
  - Compute reachability bitsets for all component nodes in reverse topological order.
- **Step-by-Step Interview Walkthrough**:
  1. Find SCCs using Kosaraju's algorithm and condense to DAG.
  2. Compute topological order of the DAG.
  3. Propagate bitsets in reverse topological order: `reach[u] |= reach[v]` for all DAG edges $u \to v$.
  4. Query $(a, b)$: return `reach[comp[a]][comp[b]]`.
- **Pseudocode**:
```text
scc_dag = condense_graph_to_scc_dag()
topo_order = topological_sort(scc_dag)

for u in reverse(topo_order):
    reach[u].set(u)
    for v in scc_dag[u]:
        reach[u] |= reach[v]

function query(a, b):
    return reach[comp[a]].test(comp[b])
```
- **Complexity**: Time: $\mathcal{O}(V + E + C^2 / 64 + Q)$, Space: $\mathcal{O}(C^2 / 8)$.

---

## 10. Reachable Nodes

- **Tier**: Tier 3
- **Link**: [Reachable Nodes](https://cses.fi/problemset/task/2138)
- **Problem Statement**: Given a Directed Acyclic Graph (DAG) of $N$ nodes and $M$ edges, calculate for every node $u$ the total number of reachable nodes.
- **Interview Pattern**: Topological Sort + Bitset DP Propagation.
- **Intuition**:
  - In a DAG, node $u$'s reachable set is $u \cup \left( \bigcup_{v \in \text{children}(u)} \text{reachable}(v) \right)$.
  - Processing nodes in reverse topological order allows propagating `bitset` unions in $\mathcal{O}(N/64)$ per edge.
- **Step-by-Step Interview Walkthrough**:
  1. Generate Topological Sort order using Kahn's BFS algorithm.
  2. Allocate `bitset<50005> reachable[N]` globally.
  3. Iterate nodes $u$ in reverse topological order:
     - `reachable[u].set(u)`
     - For neighbor $v$ of $u$: `reachable[u] |= reachable[v]`
  4. Answer for node $u$ is `reachable[u].count()`.
- **Pseudocode**:
```text
topo_order = kahn_topo_sort(graph)
for u in reverse(topo_order):
    reachable[u].set(u)
    for v in adj[u]:
        reachable[u] |= reachable[v]

for u = 1 to N:
    print reachable[u].count()
```
- **Complexity**: Time: $\mathcal{O}((V + E) \cdot V / 64)$, Space: $\mathcal{O}(V^2 / 8)$.

---

## 11. Substring Reversals

- **Tier**: Tier 3
- **Link**: [Substring Reversals](https://cses.fi/problemset/task/2073)
- **Problem Statement**: Given a string $S$ of length $N$, process $M$ range operations where substring $S[l \dots r]$ is reversed. Print the final string.
- **Interview Pattern**: Implicit Treap with Lazy Reversal Propagation.
- **Intuition**:
  - Balanced BST representing sequence order (Implicit Treap). Substring reversal = splitting range $[l, r]$ into a subtree, toggling a lazy `rev` flag, and merging back.
- **Step-by-Step Interview Walkthrough**:
  1. Build Implicit Treap from string characters.
  2. For query $[l, r]$:
     - Split treap into `left` $[0..l-1]$, `mid` $[l..r]$, `right` $[r+1..N-1]$.
     - Toggle `mid->rev ^= 1`.
     - Merge `left`, `mid`, and `right` back together.
  3. Perform in-order traversal propagating lazy `rev` flags to output string.
- **Pseudocode**:
```text
for query (l, r):
    split(root, l - 1, left, mid)
    split(mid, r - l + 1, mid, right)
    mid->rev ^= 1
    root = merge(left, merge(mid, right))

print_inorder(root)
```
- **Complexity**: Time: $\mathcal{O}((N + M) \log N)$, Space: $\mathcal{O}(N)$.

---

## 12. Reversals and Sums

- **Tier**: Tier 3
- **Link**: [Reversals and Sums](https://cses.fi/problemset/task/2074)
- **Problem Statement**: Given an array of $N$ integers, process $M$ queries: (1) Reverse subarray $[l, r]$, (2) Calculate sum of subarray $[l, r]$.
- **Interview Pattern**: Implicit Treap with Lazy Reversal + Augmented Subtree Sum.
- **Intuition**:
  - Maintain an Implicit Treap where each node stores subtree `sum` and `rev` flag.
- **Step-by-Step Interview Walkthrough**:
  1. Build Implicit Treap with node values.
  2. For query Type 1 $[l, r]$: split out `mid` $[l..r]$, toggle `mid->rev ^= 1`, merge back.
  3. For query Type 2 $[l, r]$: split out `mid` $[l..r]$, read `mid->sum`, merge back.
- **Pseudocode**:
```text
for query (type, l, r):
    split(root, l, left, right)
    split(right, r - l + 1, mid, right)
    if type == 1:
        mid->rev ^= 1
    else:
        print mid->sum
    root = merge(left, merge(mid, right))
```
- **Complexity**: Time: $\mathcal{O}((N + M) \log N)$, Space: $\mathcal{O}(N)$.

---

## 13. Cut and Paste

- **Tier**: Tier 3
- **Link**: [Cut and Paste](https://cses.fi/problemset/task/2072)
- **Problem Statement**: Given a string $S$ of length $N$, process $M$ operations: cut substring $S[l \dots r]$ and paste it at the end of the string. Print final string.
- **Interview Pattern**: Implicit Treap Split and Merge Reordering.
- **Intuition**:
  - Substring extraction and re-insertion is achieved by splitting the treap into 3 pieces $[0..l-1], [l..r], [r+1..N-1]$ and merging them in order $[0..l-1] + [r+1..N-1] + [l..r]$.
- **Step-by-Step Interview Walkthrough**:
  1. Build Implicit Treap from string characters.
  2. For query $[l, r]$:
     - Split into `left` $[0..l-1]$, `mid` $[l..r]$, `right` $[r+1..N-1]$.
     - Re-merge as `merge(merge(left, right), mid)`.
  3. Print final string via in-order traversal.
- **Pseudocode**:
```text
for query (l, r):
    split(root, l, left, right)
    split(right, r - l + 1, mid, right)
    root = merge(merge(left, right), mid)

print_inorder(root)
```
- **Complexity**: Time: $\mathcal{O}((N + M) \log N)$, Space: $\mathcal{O}(N)$.
