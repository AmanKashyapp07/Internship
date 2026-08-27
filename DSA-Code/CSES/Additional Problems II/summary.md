# CSES Additional Problems II - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 6 problems in the **Additional Problems II** topic.

---

## 1. Book Shop II

- **Link**: [Book Shop II](https://cses.fi/problemset/task/1159)
- **Problem Statement**: Given $N$ book types where each book type has price $h_i$, page count $s_i$, and copy limit $k_i$, find the maximum total pages you can buy with total budget $X$.
- **Interview Pattern**: Bounded Knapsack with Binary Decomposition.
- **Intuition**:
  - Naive 0/1 expansion of $k_i$ copies creates $\sum k_i$ items, which is too slow.
  - **Binary Grouping Trick**: Any integer $k$ can be expressed as a sum of powers of two: $1, 2, 4, 8, \dots, R$. Decompose $k_i$ copies into $\mathcal{O}(\log k_i)$ composite items with price $2^j \cdot h_i$ and pages $2^j \cdot s_i$.
  - Run standard 0/1 Knapsack DP on the reduced set of items.
- **Step-by-Step Interview Walkthrough**:
  1. Decompose copy count $k_i$ for each book type into power-of-2 bundles ($1, 2, 4, \dots, R$).
  2. For each bundle with multiplier $m$, create a single item with price $m \times h_i$ and pages $m \times s_i$.
  3. Run 1D space-optimized 0/1 Knapsack DP over target budget $X$.
  4. Output `dp[X]`.
- **Pseudocode**:
```text
items = []
for i = 1 to N:
    cnt = k[i], mult = 1
    while cnt > 0:
        take = min(cnt, mult)
        items.push((price[i] * take, pages[i] * take))
        cnt -= take
        mult *= 2

dp[0..X] = 0
for (p, s) in items:
    for cap = X down to p:
        dp[cap] = max(dp[cap], dp[cap - p] + s)

return dp[X]
```
- **Complexity**: Time: $\mathcal{O}(X \cdot \sum \log k_i)$, Space: $\mathcal{O}(X)$.

---

## 2. Coin Grid

- **Link**: [Coin Grid](https://cses.fi/problemset/task/1709)
- **Problem Statement**: Given an $N \times N$ grid containing coins at certain cells, find the minimum number of row/column sweep operations (and which ones) to clear all coins from the grid.
- **Interview Pattern**: Minimum Vertex Cover in Bipartite Graph (König's Theorem).
- **Intuition**:
  - Model rows as left vertices $L_1 \dots L_N$, columns as right vertices $R_1 \dots R_N$, and coins at $(r, c)$ as edges connecting $L_r \to R_c$.
  - Clearing a row or column corresponds to selecting a vertex to cover all incident edges.
  - **König's Theorem**: In any bipartite graph, the size of the Minimum Vertex Cover equals the size of the Maximum Bipartite Matching.
- **Step-by-Step Interview Walkthrough**:
  1. Construct bipartite graph with $N$ row nodes and $N$ column nodes, with edges for each coin.
  2. Find Maximum Bipartite Matching using Hopcroft-Karp or Augmenting Paths.
  3. To extract Minimum Vertex Cover:
     - Run DFS from unmatched left vertices along alternating paths.
     - Vertices in cover = (unvisited left vertices) $\cup$ (visited right vertices).
  4. Print chosen rows and columns.
- **Pseudocode**:
```text
build bipartite graph with edges for coins
max_matching = compute_matching()

mark visited vertices via DFS from unmatched L nodes:
    unvisited_L = L nodes not reachable via alternating paths
    visited_R = R nodes reachable via alternating paths

min_vertex_cover = unvisited_L + visited_R
return min_vertex_cover
```
- **Complexity**: Time: $\mathcal{O}(V \cdot E) = \mathcal{O}(N^3)$, Space: $\mathcal{O}(N^2)$.

---

## 3. GCD Subsets

- **Link**: [GCD Subsets](https://cses.fi/problemset/task/2417)
- **Problem Statement**: Given an array of $N$ positive integers, count the number of non-empty subsets whose Greatest Common Divisor (GCD) is exactly 1.
- **Interview Pattern**: Inclusion-Exclusion on Multiple Counts / Mobius Inversion.
- **Intuition**:
  - For each divisor $d$, count how many array elements are multiples of $d$ (let this be $c_d$).
  - The number of non-empty subsets where $d$ divides all elements is $2^{c_d} - 1$.
  - Work backwards from $MAX$ down to 1: exact GCD $d$ count $E[d] = (2^{c_d} - 1) - \sum_{m = 2d, 3d, \dots} E[m]$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute frequency of each number in input array.
  2. For each candidate divisor $d \in [1, MAX]$, count total multiples $c_d = \sum_{m = d, 2d, \dots} \text{freq}[m]$.
  3. Iterate $d$ backwards from $MAX$ down to 1:
     - Set $E[d] = (2^{c_d} - 1) \pmod{10^9+7}$.
     - Subtract strict multiple exact counts: $E[d] = (E[d] - \sum_{m=2d, 3d, \dots} E[m]) \pmod{10^9+7}$.
  4. Output $E[1]$.
- **Pseudocode**:
```text
for d = MAX down to 1:
    multiples_cnt = sum(freq[m] for m in d, 2d, 3d...)
    total_subsets = power(2, multiples_cnt) - 1
    
    exact[d] = total_subsets
    for m in 2d, 3d, 4d...:
        exact[d] = (exact[d] - exact[m]) % MOD

return exact[1]
```
- **Complexity**: Time: $\mathcal{O}(MAX \log MAX + N)$, Space: $\mathcal{O}(MAX)$.

---

## 4. Increasing Array II

- **Link**: [Increasing Array II](https://cses.fi/problemset/task/2132)
- **Problem Statement**: Given an array of $N$ integers, find the minimum total cost of operations (incrementing or decrementing any element by 1) to make the array non-decreasing.
- **Interview Pattern**: Slope Trick / Priority Queue Greedy.
- **Intuition**:
  - We are optimizing a convex piecewise-linear function $F_i(x)$ representing the minimum cost to make prefix $1..i$ non-decreasing with last element $x$.
  - **Slope Trick**: Maintain the inflection points of the slope using a Max-Heap. When processing $A[i]$, if $A[i] < \text{max\_heap.top()}$, adding $A[i]$ requires cost $\text{max\_heap.top()} - A[i]$, and we adjust the slope boundaries by popping top and pushing $A[i]$ twice.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain a Max-Heap `pq`.
  2. For each element $x \in A$:
     - Push $x$ into `pq`.
     - If $x < \text{pq.top()}$:
       - Accumulate cost += $\text{pq.top()} - x$.
       - Pop top element from `pq`.
       - Push $x$ into `pq` again to maintain correct slope representation.
  3. Output total accumulated cost.
- **Pseudocode**:
```text
max_heap pq
total_cost = 0

for x in A:
    pq.push(x)
    if x < pq.top():
        total_cost += pq.top() - x
        pq.pop()
        pq.push(x)

return total_cost
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 5. School Excursion

- **Link**: [School Excursion](https://cses.fi/problemset/task/1707)
- **Problem Statement**: Given $N$ students and $M$ pairs who want to be in the same group, partition students into connected components of sizes $S_1, S_2, \dots, S_k$. Determine all target capacity values $K \in [1, N]$ that can be formed by a subset of these group sizes.
- **Interview Pattern**: DSU Component Sizes + Bitset 0/1 Subset Sum Knapsack.
- **Intuition**:
  - Use DSU to find connected component sizes $S_1, S_2, \dots, S_k$.
  - Problem reduces to 0/1 Subset Sum: which numbers $\le N$ can be formed by summing a subset of component sizes?
  - `std::bitset` allows $\mathcal{O}(N^2 / 64)$ ultra-fast subset sum DP using bitwise OR shift operations: `dp |= (dp << size)`.
- **Step-by-Step Interview Walkthrough**:
  1. Compute connected component sizes using DSU.
  2. Group component sizes by frequency to optimize duplicate sizes (or use bitset shift directly).
  3. Initialize `bitset<MAX_N> dp`, set `dp[0] = 1`.
  4. For each component size $s$: `dp |= (dp << s)`.
  5. Print `dp[1..N]` as binary string or flags.
- **Pseudocode**:
```text
comp_sizes = get_dsu_component_sizes()
bitset<MAX_N> dp
dp[0] = 1

for size in comp_sizes:
    dp |= (dp << size)

for k = 1 to N:
    print dp[k] (1 if possible, 0 otherwise)
```
- **Complexity**: Time: $\mathcal{O}(N + N^2 / 64)$, Space: $\mathcal{O}(N / 64)$.

---

## 6. Stick Divisions

- **Link**: [Stick Divisions](https://cses.fi/problemset/task/1161)
- **Problem Statement**: You have a stick of length $X$ that needs to be divided into $N$ sticks of given lengths $L_1, L_2, \dots, L_N$. The cost of splitting a stick of length $S$ into two is $S$. Find the minimum total cost to obtain all $N$ sticks.
- **Interview Pattern**: Reverse Huffman Coding / Min-Heap Greedy.
- **Intuition**:
  - Splitting a stick top-down is equivalent to merging the $N$ target stick lengths bottom-up (Huffman Coding)!
  - At each step, greedily merge the two smallest available stick lengths $a$ and $b$; the cost incurred is $a + b$, and the resulting merged stick $a + b$ is put back.
- **Step-by-Step Interview Walkthrough**:
  1. Insert all $N$ stick lengths $L_i$ into a Min-Heap.
  2. Maintain `total_cost = 0`.
  3. While `heap.size() > 1`:
     - Pop two smallest lengths $a$ and $b$.
     - `total_cost += a + b`.
     - Push $a + b$ back into Min-Heap.
  4. Return `total_cost`.
- **Pseudocode**:
```text
min_heap pq(all stick lengths L_i)
total_cost = 0

while pq.size() > 1:
    a = pq.pop_min()
    b = pq.pop_min()
    cost = a + b
    total_cost += cost
    pq.push(cost)

return total_cost
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.
