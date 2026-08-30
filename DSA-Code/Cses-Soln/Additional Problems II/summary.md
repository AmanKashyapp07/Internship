# CSES Additional Problems II - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 9 problems in the **Additional Problems II** topic.

---

## 1. Book Shop II

- **Tier**: Tier 2
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

- **Tier**: Tier 3
- **Link**: [Coin Grid](https://cses.fi/problemset/task/1709)
- **Problem Statement**: Given an $N \times N$ grid containing coins at certain cells, find the minimum number of row and column sweep operations (and their indices) to clear all coins from the grid.
- **Interview Pattern**: Minimum Vertex Cover in Bipartite Graph (König's Theorem).
- **Intuition**:
  - Model rows as left vertices $L_1 \dots L_N$, columns as right vertices $R_1 \dots R_N$, and coins at $(r, c)$ as bipartite edges $L_r \to R_c$.
  - By **König's Theorem**, the size of the Minimum Vertex Cover in a bipartite graph equals the size of the Maximum Matching.
  - Alternating path reachability from unmatched left vertices separates vertices into the minimal cover: unvisited left vertices $\cup$ visited right vertices.
- **Step-by-Step Interview Walkthrough**:
  1. Construct bipartite graph with $N$ row nodes and $N$ column nodes with matching edges for coin positions.
  2. Find Maximum Bipartite Matching using augmenting paths.
  3. Run DFS from unmatched left vertices along alternating paths to mark reachable vertices.
  4. Selected rows = unvisited left nodes; selected columns = visited right nodes.
- **Pseudocode**:
```text
build bipartite graph with matching edges
max_matching = compute_augmenting_paths()
run DFS from unmatched L nodes along alternating edges
cover_rows = [i for i in 1..N if not visited_L[i]]
cover_cols = [j for j in 1..N if visited_R[j]]
return cover_rows + cover_cols
```
- **Complexity**: Time: $\mathcal{O}(V \cdot E) = \mathcal{O}(N^3)$, Space: $\mathcal{O}(N^2)$.

---

## 3. Food Division

- **Tier**: Tier 2
- **Link**: [Food Division](https://cses.fi/problemset/task/1189)
- **Problem Statement**: Given initial food amounts $A_1 \dots A_N$ and desired food amounts $B_1 \dots B_N$ for $N$ children sitting in a circle, find the minimum total food units transferred between adjacent children.
- **Interview Pattern**: Circular Flow Balance + 1D Median Minimization.
- **Intuition**:
  - Let $x_i$ be the net food transferred from child $i$ to child $i+1$.
  - Flow balance equations yield $x_i = x_N + P[i]$ where $P[i] = \sum_{j=1}^i (A_j - B_j)$ is the prefix imbalance.
  - Total moves $= \sum_{i=1}^N |x_N + P[i]| = \sum_{i=1}^N |-P[i] - x_N|$. This sum of absolute deviations is minimized when $x_N$ is chosen as the median of $\{-P[1], -P[2], \dots, -P[N]\}$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute running prefix imbalance $P[i] = P[i-1] + (A[i] - B[i])$.
  2. Construct array $neg\_P = [-P[0], -P[1], \dots, -P[N-1]]$.
  3. Sort $neg\_P$ and extract median $M = neg\_P[N / 2]$.
  4. Sum $\sum_{i=0}^{N-1} |-P[i] - M|$ and output total moves.
- **Pseudocode**:
```text
P[i] = prefix_sum(A - B)
neg_P = [-P[i] for i in 0..N-1]
sort(neg_P)
median = neg_P[N / 2]
total_moves = sum(abs(-P[i] - median) for i in 0..N-1)
return total_moves
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 4. GCD Subsets

- **Tier**: Tier 3
- **Link**: [GCD Subsets](https://cses.fi/problemset/task/3161)
- **Problem Statement**: Given an array of $N$ positive integers, count the number of non-empty subsets whose Greatest Common Divisor (GCD) is exactly 1 modulo $10^9+7$.
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

## 5. Increasing Array II

- **Tier**: Tier 3
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

## 6. K Subset Sums I

- **Tier**: Tier 3
- **Link**: [K Subset Sums I](https://cses.fi/problemset/task/3108)
- **Problem Statement**: Given an array of $N$ integers, compute the $K$ smallest subset sums in non-decreasing sorted order.
- **Interview Pattern**: Non-Negative Transformation + State-Branching Min-Heap.
- **Intuition**:
  - The minimum subset sum is the sum of all negative elements $B = \sum_{A[i] < 0} A[i]$.
  - Choosing/not choosing negative elements maps directly to including positive magnitudes $|A[i]|$.
  - Sort $|A[i]|$. In a Min-Heap, maintain state $(sum, idx)$. From state $(sum, idx)$, branch into two disjoint states: (1) include $A[idx]$, and (2) replace $A[idx-1]$ with $A[idx]$. This generates every subset sum exactly once in ascending order.
- **Step-by-Step Interview Walkthrough**:
  1. Calculate base sum $B = \sum_{x < 0} x$.
  2. Replace all $x \leftarrow |x|$ and sort array in ascending order.
  3. Initialize min-priority queue with empty subset `{0, 0}`.
  4. While $|\text{ans}| < K$:
     - Pop $(sum, idx)$, append $B + sum$ to result.
     - If $idx < N$, push `{sum + A[idx], idx + 1}`.
     - If $idx > 0$ and $idx < N$, push `{sum - A[idx - 1] + A[idx], idx + 1}`.
  5. Output $K$ smallest sums.
- **Pseudocode**:
```text
base = sum(x for x in A if x < 0)
A = sorted([abs(x) for x in A])
pq = min_heap([{sum: 0, idx: 0}])
ans = []

while len(ans) < K and not pq.empty():
    sum, idx = pq.pop()
    ans.push(base + sum)
    if idx < N:
        pq.push({sum + A[idx], idx + 1})
        if idx > 0:
            pq.push({sum - A[idx - 1] + A[idx], idx + 1})

return ans
```
- **Complexity**: Time: $\mathcal{O}(N \log N + K \log K)$, Space: $\mathcal{O}(N + K)$.

---

## 7. Programmers and Artists

- **Tier**: Tier 2
- **Link**: [Programmers and Artists](https://cses.fi/problemset/task/2426)
- **Problem Statement**: Given $N$ candidates where candidate $i$ has programming skill $P_i$ and art skill $A_i$, choose exactly $A$ programmers and $B$ artists ($A + B \le N$) to maximize total skill value.
- **Interview Pattern**: Difference Greedy Sorting + Dual Prefix/Suffix Min-Heaps.
- **Intuition**:
  - Candidates with higher $P_i - A_i$ should preferentially be assigned as programmers.
  - Sort candidates by $P_i - A_i$ descending. The optimal assignment splits candidates at some index $k$: candidates $1 \dots k$ supply $A$ programmers and candidates $k+1 \dots N$ supply $B$ artists.
  - Precompute `pref[i]` (best $A$ programmers in prefix $0 \dots i$) using a Min-Heap of size $A$, and `suff[i]` (best $B$ artists in suffix $i \dots N-1$) using a Min-Heap of size $B$. The answer is $\max_i (\text{pref}[i] + \text{suff}[i+1])$.
- **Step-by-Step Interview Walkthrough**:
  1. Sort candidates in descending order of $(P_i - A_i)$.
  2. Compute `pref[i]`: maintain a min-heap of programmer skills capped at size $A$.
  3. Compute `suff[i]`: maintain a min-heap of artist skills capped at size $B$ iterating backwards.
  4. Iterate split point $i \in [A-1, N-B-1]$ and maximize $\text{pref}[i] + \text{suff}[i+1]$.
- **Pseudocode**:
```text
sort candidates by (P[i] - A[i]) desc
pref = compute_prefix_top_A_programmers()
suff = compute_suffix_top_B_artists()
best = max(pref[i] + suff[i + 1] for i in A-1..N-B-1)
return best
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 8. School Excursion

- **Tier**: Tier 2
- **Link**: [School Excursion](https://cses.fi/problemset/task/1706)
- **Problem Statement**: Given $N$ students and $M$ friendship constraints, partition students into connected components of sizes $S_1, S_2, \dots, S_k$. Determine all capacity values $K \in [1, N]$ that can be formed by a subset of these group sizes.
- **Interview Pattern**: DSU Component Sizes + Bitset 0/1 Subset Sum Knapsack.
- **Intuition**:
  - Find connected component sizes $S_1, S_2, \dots, S_k$ using DSU.
  - Problem reduces to 0/1 Subset Sum on component sizes.
  - `std::bitset` allows $\mathcal{O}(N^2 / 64)$ fast bit-parallel subset sum transitions: `dp |= (dp << size)`.
- **Step-by-Step Interview Walkthrough**:
  1. Find component sizes using Disjoint Set Union (DSU).
  2. Initialize `bitset<MAX_N> dp` with `dp[0] = 1`.
  3. For each component size $s$: `dp |= (dp << s)`.
  4. Print `dp[1..N]` as binary flags.
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

## 9. Stick Divisions

- **Tier**: Tier 2
- **Link**: [Stick Divisions](https://cses.fi/problemset/task/1161)
- **Problem Statement**: You have a stick of length $X$ that needs to be divided into $N$ sticks of given lengths $L_1, L_2, \dots, L_N$. The cost of splitting a stick of length $S$ into two is $S$. Find the minimum total cost to obtain all $N$ sticks.
- **Interview Pattern**: Reverse Huffman Coding / Min-Heap Greedy.
- **Intuition**:
  - Splitting a stick top-down is equivalent to merging the $N$ target stick lengths bottom-up (Huffman Coding)!
  - At each step, greedily merge the two smallest available stick lengths $a$ and $b$; the cost incurred is $a + b$, and the resulting merged stick $a + b$ is put back into the heap.
- **Step-by-Step Interview Walkthrough**:
  1. Insert all $N$ stick lengths $L_i$ into a Min-Heap.
  2. Initialize `total_cost = 0`.
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
